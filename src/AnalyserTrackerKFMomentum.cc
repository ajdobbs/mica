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
                                                         mHPxUSDS{nullptr},
                                                         mHPyUSDS{nullptr},
                                                         mHPzUSDS{nullptr} {
  int nbins = 100;
  mHPxUSDS = std::unique_ptr<TH2D>(new TH2D("hPxUSDS", "TkU px vx TkD px",
                                             nbins, -100, 100, nbins, -100, 100));
  mHPxUSDS->GetXaxis()->SetTitle("tku px (MeV/c)");
  mHPxUSDS->GetYaxis()->SetTitle("tkd px (MeV/c)");
  mHPyUSDS = std::unique_ptr<TH2D>(new TH2D("hPyUSDS", "TkU py vx TkD py",
                                             nbins, -100, 100, nbins, -100, 100));
  mHPyUSDS->GetXaxis()->SetTitle("tku py (MeV/c)");
  mHPyUSDS->GetYaxis()->SetTitle("tkd py (MeV/c)");
  mHPzUSDS = std::unique_ptr<TH2D>(new TH2D("hPzUSDS", "TkU pz vx TkD pz",
                                             nbins, 0, 350, nbins, 0, 350));
  mHPzUSDS->GetXaxis()->SetTitle("tku pz (MeV/c)");
  mHPzUSDS->GetYaxis()->SetTitle("tkd pz (MeV/c)");
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
  mHPxUSDS->Fill(mom_tku.x(), mom_tkd.x());
  mHPyUSDS->Fill(mom_tku.y(), mom_tkd.y());
  mHPzUSDS->Fill(mom_tku.z(), mom_tkd.z());

  return true;
}

void AnalyserTrackerKFMomentum::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  aPad->Divide(3, 1);
  aPad->cd(1);
  mHPxUSDS->Draw("COLZ");
  aPad->cd(2);
  mHPyUSDS->Draw("COLZ");
  aPad->cd(3);
  mHPzUSDS->Draw("COLZ");
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
