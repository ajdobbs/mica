/* This file is part of MAUS: http://micewww.pp.rl.ac.uk/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "TRef.h"

#include "src/common_cpp/Analysis/AnalyserTrackerAngularMomentum.hh"
#include "src/common_cpp/DataStructure/SciFiTrackPoint.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"

namespace MAUS {
namespace Analysis {

AnalyserTrackerAngularMomentum::AnalyserTrackerAngularMomentum() : mAnalysisStation(1),
                                                                   mAnalysisPlane(0),
                                                                   mHAngMomTKU(NULL),
                                                                   mHAngMomTKD(NULL) {
  mHAngMomTKU = new TH2D("hAngMomTKU", "Canonical AM vs Radius TkU", 200, -4000, 2000, 200, 0, 150);
  mHAngMomTKU->GetXaxis()->SetTitle("xPy - yPx (mm MeV/c)");
  mHAngMomTKU->GetYaxis()->SetTitle("Radius (mm)");
  mHAngMomTKD = new TH2D("hAngMomTKD", "Canonical AM vs Radius TkD", 200, -4000, 2000, 200, 0, 150);
  mHAngMomTKD->GetXaxis()->SetTitle("xPy - yPx (mm MeV/c)");
  mHAngMomTKD->GetYaxis()->SetTitle("Radius (mm)");
}

bool AnalyserTrackerAngularMomentum::analyse(ReconEvent* const aReconEvent,
                                             MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  // Loop over all the scifi tracks in this event
  for (auto trk : sfevt->scifitracks()) {

    // Access the SciFiSeed associate with this track
    SciFiSeed* seed = ExtractSeed(trk);
    if (!seed)
      continue;

    // Access the PR track from the SciFiSeed in order to extract the radius
    TObject* pr_track_obj = seed->getPRTrackTobject();
    if (!pr_track_obj) {
      std::cerr << "Empty PR track TObject pointer, skipping track" << std::endl;
      continue;
    }
    SciFiHelicalPRTrack* htrk =
        dynamic_cast<SciFiHelicalPRTrack*>(pr_track_obj); // NOLINT(runtime/rtti)
    if (!htrk) {
      std::cerr << "PR track dynamic cast failed, source pointer address "
                << pr_track_obj << std::endl;
      continue;
    }
    double radius = htrk->get_R();

    // Pull out the associated trackpoints, get the position and momentum for the selected station
    // and plane, and then fill the histograms
    for (auto tp : trk->scifitrackpoints()) {
      if ((tp->station() == mAnalysisStation) && (tp->plane() == mAnalysisPlane)) {
        ThreeVector pos = tp->pos();
        ThreeVector mom = tp->mom();
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

void AnalyserTrackerAngularMomentum::draw(TVirtualPad* aPad) {
  aPad->Divide(2, 1);
  aPad->cd(1);
  mHAngMomTKU->Draw("COLZ");
  aPad->cd(2);
  mHAngMomTKD->Draw("COLZ");
}

SciFiSeed* AnalyserTrackerAngularMomentum::ExtractSeed(SciFiTrack* aTrack) const {
  SciFiSeed* seed = aTrack->scifi_seed();
  if (!seed) {
    std::cerr << "Empty seed pointer at: " << seed << std::endl;
  }
  TObject* seed_obj = aTrack->scifi_seed_tobject();
  if (!seed_obj) {
    std::cerr << "Empty seed TObject pointer" << std::endl;
  }
  if (!seed && !seed_obj) {
    std::cerr << "No seed pointer at all" << std::endl;
    return NULL;
  }
  if (!seed && seed_obj) {
    seed = dynamic_cast<SciFiSeed*>(seed_obj); // NOLINT(runtime/rtti)
    if (!seed) {
      std::cerr << "Dynamic cast from SciFiSeed TObject failed" << std::endl;
      return NULL;
    }
  }
  return seed;
}


} // ~namespace Analysis
} // ~namespace MAUS
