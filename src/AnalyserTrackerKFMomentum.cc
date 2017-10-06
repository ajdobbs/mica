/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTrackerKFMomentum.hh"

#include <algorithm>
#include <cmath>

#include "TRef.h"

#include "src/common_cpp/DataStructure/SciFiTrack.hh"
#include "src/common_cpp/DataStructure/SciFiTrackPoint.hh"

namespace mica {

AnalyserTrackerKFMomentum::AnalyserTrackerKFMomentum() : mAnalysisStation{1},
                                                         mAnalysisPlane{0},
                                                         mHPUSDS{nullptr} {
  int nbins = 100;
  mHPUSDS = std::unique_ptr<TH2D>(new TH2D("hPUSDS", "TkU p vs TkD p",
                                             nbins, 0, 300, nbins, 0, 300));
  mHPUSDS->GetXaxis()->SetTitle("tku p (MeV/c)");
  mHPUSDS->GetYaxis()->SetTitle("tkd p (MeV/c)");
}

bool AnalyserTrackerKFMomentum::analyse(MAUS::ReconEvent* const aReconEvent,
                                        MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent) return false;
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt) return false;

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
  if (!tku_good && !tkd_good) return false;
  double tku_mag =
    sqrt(mom_tku.x()*mom_tku.x() + mom_tku.y()*mom_tku.y() + mom_tku.z()*mom_tku.z());
  double tkd_mag =
    sqrt(mom_tkd.x()*mom_tkd.x() + mom_tkd.y()*mom_tkd.y() + mom_tkd.z()*mom_tkd.z());
  mHPUSDS->Fill(tku_mag, tkd_mag);

  return true;
}

bool AnalyserTrackerKFMomentum::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  mHPUSDS->Draw("COLZ");

  return true;
}

bool AnalyserTrackerKFMomentum::GetMomentum(const MAUS::SciFiTrack* const trk,
                                            MAUS::ThreeVector& mom) {
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
