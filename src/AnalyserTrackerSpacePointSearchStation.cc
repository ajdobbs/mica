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

#include "src/common_cpp/Analysis/AnalyserTrackerSpacePointSearchStation.hh"

#include <string>

namespace MAUS {
namespace Analysis {

AnalyserTrackerSpacePointSearchStation::AnalyserTrackerSpacePointSearchStation() {
  for (int i = 0; i < 5; ++i) {
    std::string seeds_title = "Seed Pull vs NPE Station " + std::to_string(i+1);
    std::string seeds_name= "hSeedsS" + std::to_string(i+1);
    mHSeeds.push_back(new TH2D(seeds_name.c_str(), seeds_title.c_str(), 100, 0, 30, 100, 0, 200));
    mHSeeds[i]->GetXaxis()->SetTitle("Pull (mm)");
    mHSeeds[i]->GetYaxis()->SetTitle("NPE");

    std::string addons_title = "Add-On Seed Pull vs NPE Station " + std::to_string(i+1);
    std::string addons_name= "hAddOnsS" + std::to_string(i+1);
    mHAddOns.push_back(new TH2D(addons_name.c_str(), addons_title.c_str(),
                                100, 0, 30, 100, 0, 200));
    mHAddOns[i]->GetXaxis()->SetTitle("Pull (mm)");
    mHAddOns[i]->GetYaxis()->SetTitle("NPE");
  }
}

bool AnalyserTrackerSpacePointSearchStation::analyse(ReconEvent* const aReconEvent,
                                              MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  for (auto trk : sfevt->helicalprtracks()) {
    for (auto sp : trk->get_spacepoints_pointers()) {
      double npe = sp->get_npe();
      double pull = sp->get_prxy_pull();
      mHSeeds[sp->get_station() - 1]->Fill(pull, npe);
      if (sp->get_add_on()) {
        mHAddOns[sp->get_station() - 1]->Fill(pull, npe);
      }
    } // Loop over seed spacepoints
  } // ~Loop over tracks
  return true;
}

void AnalyserTrackerSpacePointSearchStation::draw(TVirtualPad* aPad) {
  aPad->Divide(5, 2);
  for (int i = 0; i < 5; ++i) {
    aPad->cd(i+1);
    mHSeeds[i]->Draw("COLZ");
    aPad->cd(i+6);
    mHAddOns[i]->Draw("COLZ");
  }
}
} // ~namespace Analysis
} // ~namespace MAUS
