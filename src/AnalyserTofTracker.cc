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
                                           mHPTkU{nullptr},
                                           mHPTkD{nullptr},
                                           mHPtTkU{nullptr},
                                           mHPtTkD{nullptr},
                                           mHPzTkU{nullptr},
                                           mHPzTkD{nullptr} {
  int nbins = 100;
  mHPTkU = std::unique_ptr<TH2D>(new TH2D("hPTkU", "ToF12 Time vs TkU Momentum",
                                         nbins, 25, 50, nbins, 0, 300));
  mHPTkU->GetXaxis()->SetTitle("tof12 (ns)");
  mHPTkU->GetYaxis()->SetTitle("Total Momentum (MeV/c)");
  mHPTkD = std::unique_ptr<TH2D>(new TH2D("hPTkD", "ToF12 Time vs TkD Momentum",
                                         nbins, 25, 50, nbins, 0, 300));
  mHPTkD->GetXaxis()->SetTitle("tof12 (ns)");
  mHPTkD->GetYaxis()->SetTitle("Total Momentum (MeV/c)");

  mHPtTkU = std::unique_ptr<TH2D>(new TH2D("hPtTkU", "ToF12 Time vs TkU pt",
                                         nbins, 25, 50, nbins, 0, 300));
  mHPtTkU->GetXaxis()->SetTitle("tof12 (ns)");
  mHPtTkU->GetYaxis()->SetTitle("pt (MeV/c)");
  mHPtTkD = std::unique_ptr<TH2D>(new TH2D("hPtTkD", "ToF12 Time vs TkD pt",
                                         nbins, 25, 50, nbins, 0, 300));
  mHPtTkD->GetXaxis()->SetTitle("tof12 (ns)");
  mHPtTkD->GetYaxis()->SetTitle("pt (MeV/c)");

  mHPzTkU = std::unique_ptr<TH2D>(new TH2D("hPzTkU", "ToF12 Time vs TkU pz",
                                         nbins, 25, 50, nbins, 0, 300));
  mHPzTkU->GetXaxis()->SetTitle("tof12 (ns)");
  mHPzTkU->GetYaxis()->SetTitle("pz (MeV/c)");
  mHPzTkD = std::unique_ptr<TH2D>(new TH2D("hPzTkD", "ToF12 Time vs TkD pz",
                                           nbins, 25, 50, nbins, 0, 300));
  mHPzTkD->GetXaxis()->SetTitle("tof12 (ns)");
  mHPzTkD->GetYaxis()->SetTitle("pz (MeV/c)");
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
  bool tku_good = GetMomentum(tku_trks[0], mom_tku);
  bool tkd_good = GetMomentum(tkd_trks[0], mom_tkd);


  // Fill the histograms
  // if (!tku_good && !tkd_good) return false;
  double tku_mag =
    sqrt(mom_tku.x()*mom_tku.x() + mom_tku.y()*mom_tku.y() + mom_tku.z()*mom_tku.z());
  double tkd_mag =
    sqrt(mom_tkd.x()*mom_tkd.x() + mom_tkd.y()*mom_tkd.y() + mom_tkd.z()*mom_tkd.z());
  if (tku_good) {
    mHPTkU->Fill(tof12, tku_mag);
    mHPtTkU->Fill(tof12, sqrt(mom_tku.x()*mom_tku.x() + mom_tku.y()*mom_tku.y()));
    mHPzTkU->Fill(tof12, mom_tku.z());
  }
  if (tkd_good) {
    mHPTkD->Fill(tof12, tkd_mag);
    mHPtTkD->Fill(tof12, sqrt(mom_tkd.x()*mom_tkd.x() + mom_tkd.y()*mom_tkd.y()));
    mHPzTkD->Fill(tof12, mom_tkd.z());
  }

  return true;
}

bool AnalyserTofTracker::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  aPad->Divide(3, 2);
  aPad->cd(1);
  mHPTkU->Draw("COLZ");
  aPad->cd(2);
  mHPtTkU->Draw("COLZ");
  aPad->cd(3);
  mHPzTkU->Draw("COLZ");
  aPad->cd(4);
  mHPTkD->Draw("COLZ");
  aPad->cd(5);
  mHPtTkD->Draw("COLZ");
  aPad->cd(6);
  mHPzTkD->Draw("COLZ");

  return true;
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
