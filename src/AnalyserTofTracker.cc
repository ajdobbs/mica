/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTofTracker.hh"

#include <algorithm>
#include <cmath>

#include "TRef.h"

#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/TOFEventSpacePoint.hh"
#include "src/common_cpp/DataStructure/TOFSpacePoint.hh"
#include "src/common_cpp/DataStructure/SciFiTrack.hh"
#include "src/common_cpp/DataStructure/SciFiTrackPoint.hh"

namespace mica {

AnalyserTofTracker::AnalyserTofTracker() : mAnalysisStation{1},
                                           mAnalysisPlane{0},
                                           mHTKU{nullptr},
                                           mHTKD{nullptr} {
  int nbins = 100;
  mHTKU = std::unique_ptr<TH2D>(new TH2D("hTKU", "ToF12 Time vs TkU Momentum",
                                         nbins, 25, 50, nbins, 0, 300));
  mHTKU->GetXaxis()->SetTitle("tof12 (ns)");
  mHTKU->GetYaxis()->SetTitle("Momentum (MeV/c)");
  mHTKD = std::unique_ptr<TH2D>(new TH2D("hTKD", "ToF12 Time vs TkD Momentum",
                                         nbins, 25, 50, nbins, 0, 300));
  mHTKD->GetXaxis()->SetTitle("tof12 (ns)");
  mHTKD->GetYaxis()->SetTitle("Momentum (MeV/c)");
}

bool AnalyserTofTracker::analyse(MAUS::ReconEvent* const aReconEvent,
                                 MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent) return false;
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt) return false;
  MAUS::TOFEvent* tofevt = aReconEvent->GetTOFEvent();
  if (!tofevt) return false;

  // Check and pull out the tof data
  MAUS::TOFEventSpacePoint* tofevtsp = tofevt->GetTOFEventSpacePointPtr();
  if (!tofevtsp) return false;
  std::vector<MAUS::TOFSpacePoint> tof1sps = tofevtsp->GetTOF1SpacePointArray();
  std::vector<MAUS::TOFSpacePoint> tof2sps = tofevtsp->GetTOF2SpacePointArray();
  if (tof1sps.size() != 1 || tof2sps.size() != 1) return false;
  double tof12 = tof2sps[0].GetTime() - tof1sps[0].GetTime();
  std::cerr << "TOF data valid, tof12 = " << tof12 << std::endl;


  // Check and pull out the tracker data
  std::vector<MAUS::SciFiTrack*> trks = sfevt->scifitracks();
  if (trks.size() != 2) return false;
  std::vector<MAUS::SciFiTrack*> tku_trks;
  std::vector<MAUS::SciFiTrack*> tkd_trks;
  for (auto trk : trks) {
    if (trk->tracker() == 0)
      tku_trks.push_back(trk);
    else if (trk->tracker() == 1)
      tkd_trks.push_back(trk);
  }
  if (tku_trks.size() != 1 && tkd_trks.size() != 1) return false;

  // Pull out the associated trackpoints, get the momentum
  MAUS::ThreeVector mom_tku;
  MAUS::ThreeVector mom_tkd;
  bool tku_good = false;
  bool tkd_good = false;
  for (auto trk : trks) {
    tku_good = GetMomentum(trk, mom_tku);
    tkd_good = GetMomentum(trk, mom_tkd);
  } // ~Loop over tracks

  // Fill the histograms
  if (!tku_good && !tkd_good) return false;
  if (tku_good) mHTKU->Fill(tof12, mom_tku.mag());
  if (tkd_good) mHTKD->Fill(tof12, mom_tkd.mag());

  return true;
}

void AnalyserTofTracker::draw(std::shared_ptr<TVirtualPad> aPad) {
  aPad->Divide(2, 1);
  aPad->cd(1);
  mHTKU->Draw("COLZ");
  aPad->cd(2);
  mHTKD->Draw("COLZ");
}

bool AnalyserTofTracker::GetMomentum(const MAUS::SciFiTrack* const trk, MAUS::ThreeVector& mom) {
  MAUS::ThreeVector mom_out;
  bool found = false;
  for (auto tp : trk->scifitrackpoints()) {
    if ((tp->station() == mAnalysisStation) && (tp->plane() == mAnalysisPlane)) {
      mom_out = tp->mom();
      found = true;
    }
  }
  if (found) mom = mom_out;
  return found;
}

} // ~namespace mica
