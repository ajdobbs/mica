/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "TRef.h"

#include "mica/AnalyserTrackerAngularMomentum.hh"
#include "src/common_cpp/DataStructure/SciFiTrackPoint.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"

namespace mica {

AnalyserTrackerAngularMomentum::AnalyserTrackerAngularMomentum() : mAnalysisStation(1),
                                                                   mAnalysisPlane(0),
                                                                   mHAngMomTKU(NULL),
                                                                   mHAngMomTKD(NULL) {
  mHAngMomTKU = new TH2D("hAngMomTKU", "PR Canonical AM vs Radius TkU",
                        200, -4000, 2000, 200, 0, 150);
  mHAngMomTKU->GetXaxis()->SetTitle("xPy - yPx (mm MeV/c)");
  mHAngMomTKU->GetYaxis()->SetTitle("Radius (mm)");
  mHAngMomTKD = new TH2D("hAngMomTKD", "PR Canonical AM vs Radius TkD",
                         200, -4000, 2000, 200, 0, 150);
  mHAngMomTKD->GetXaxis()->SetTitle("xPy - yPx (mm MeV/c)");
  mHAngMomTKD->GetYaxis()->SetTitle("Radius (mm)");
}

bool AnalyserTrackerAngularMomentum::analyse(MAUS::ReconEvent* const aReconEvent,
                                             MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  // Loop over all the scifi tracks in this event
  for (auto trk : sfevt->scifitracks()) {

    // Access the SciFiSeed associate with this track
    MAUS::SciFiSeed* seed = ExtractSeed(trk);
    if (!seed)
      continue;

    // Access the PR track from the SciFiSeed in order to extract the radius
    TObject* pr_track_obj = seed->getPRTrackTobject();
    if (!pr_track_obj) {
      // std::cerr << "Empty PR track TObject pointer, skipping track" << std::endl;
      continue;
    }
    MAUS::SciFiHelicalPRTrack* htrk =
        dynamic_cast<MAUS::SciFiHelicalPRTrack*>(pr_track_obj); // NOLINT(runtime/rtti)
    if (!htrk) {
      // std::cerr << "PR track dynamic cast failed, source pointer address "
      //          << pr_track_obj << std::endl;
      continue;
    }
    double radius = htrk->get_R();

    // Pull out the associated trackpoints, get the position and momentum for the selected station
    // and plane, and then fill the histograms
    for (auto tp : trk->scifitrackpoints()) {
      if ((tp->station() == mAnalysisStation) && (tp->plane() == mAnalysisPlane)) {
        MAUS::ThreeVector pos = tp->pos();
        MAUS::ThreeVector mom = tp->mom();
        double am = (pos.x() *  mom.y()) - (pos.y() * mom.x());
        if (tp->tracker() == 0) {
          mHAngMomTKU->Fill(am, radius);
        } else if (tp->tracker() == 1) {
          mHAngMomTKD->Fill(am, radius);
        }
      } // ~if on reference plane
    } // ~Loop over trackpoints
  } // ~Loop over tracks
  return true;
}

bool AnalyserTrackerAngularMomentum::draw(std::shared_ptr<TVirtualPad> aPad) {
  // If no data, don't bother drawing the canvas
  if (mHAngMomTKU->GetEntries() < 1.0 && mHAngMomTKD->GetEntries() < 1.0) {
    return false;
  }

  GetPads()[0]->Divide(2, 1);
  GetPads()[0]->cd(1);
  mHAngMomTKU->Draw("COLZ");
  GetPads()[0]->cd(2);
  mHAngMomTKD->Draw("COLZ");

  return true;
}

MAUS::SciFiSeed* AnalyserTrackerAngularMomentum::ExtractSeed(MAUS::SciFiTrack* aTrack) const {
  MAUS::SciFiSeed* seed = aTrack->scifi_seed();
  if (!seed) {
    // std::cerr << "Empty seed pointer at: " << seed << std::endl;
  }
  TObject* seed_obj = aTrack->scifi_seed_tobject();
  if (!seed_obj) {
    std::cerr << "Empty seed TObject pointer" << std::endl;
  }
  if (!seed && !seed_obj) {
    // std::cerr << "No seed pointer at all" << std::endl;
    return NULL;
  }
  if (!seed && seed_obj) {
    seed = dynamic_cast<MAUS::SciFiSeed*>(seed_obj); // NOLINT(runtime/rtti)
    if (!seed) {
      // std::cerr << "Dynamic cast from SciFiSeed TObject failed" << std::endl;
      return NULL;
    }
  }
  return seed;
}

void AnalyserTrackerAngularMomentum::merge(AnalyserTrackerAngularMomentum* aAnalyser) {
  mHAngMomTKU->Add(aAnalyser->mHAngMomTKU);
  mHAngMomTKD->Add(aAnalyser->mHAngMomTKD);
}
} // ~namespace mica
