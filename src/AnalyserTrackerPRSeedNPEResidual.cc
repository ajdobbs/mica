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

#include "src/common_cpp/Analysis/AnalyserTrackerPRSeedNPEResidual.hh"

#include <string>

namespace MAUS {
namespace Analysis {

AnalyserTrackerPRSeedNPEResidual::AnalyserTrackerPRSeedNPEResidual() {
  for (int i = 0; i < 5; ++i) {
    std::string tku_title = "TkU Seed Residual vs NPE Station " + std::to_string(i+1);
    std::string tku_name= "hResidualsTkUS" + std::to_string(i+1);
    mHResidualsTkU.push_back(new TH2D(tku_name.c_str(), tku_title.c_str(),
                                      100, 0, 30, 100, 0, 200));
    mHResidualsTkU[i]->GetXaxis()->SetTitle("Residual (mm)");
    mHResidualsTkU[i]->GetYaxis()->SetTitle("NPE");

    std::string tkd_title = "TkD Seed Residual vs NPE Station " + std::to_string(i+1);
    std::string tkd_name = "hResidualsTkDS" + std::to_string(i+1);
    mHResidualsTkD.push_back(new TH2D(tkd_name.c_str(), tkd_title.c_str(),
                                      100, 0, 30, 100, 0, 200));
    mHResidualsTkD[i]->GetXaxis()->SetTitle("Residual (mm)");
    mHResidualsTkD[i]->GetYaxis()->SetTitle("NPE");
  }
}

bool AnalyserTrackerPRSeedNPEResidual::analyse(ReconEvent* const aReconEvent,
                                              MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  for (auto trk : sfevt->helicalprtracks()) {
    for (auto sp : trk->get_spacepoints_pointers()) {
      double npe = sp->get_npe();
      double pull = sp->get_prxy_pull();
      if (sp->get_tracker() == 0) {
        mHResidualsTkU[sp->get_station() - 1]->Fill(pull, npe);
      } else {
        mHResidualsTkD[sp->get_station() - 1]->Fill(pull, npe);
      }
    } // Loop over seed spacepoints
  } // ~Loop over tracks
  return true;
}

void AnalyserTrackerPRSeedNPEResidual::draw(TVirtualPad* aPad) {
  aPad->Divide(5, 2);
  for (int i = 0; i < 5; ++i) {
    aPad->cd(i+1);
    mHResidualsTkU[i]->Draw("COLZ");
    aPad->cd(i+6);
    mHResidualsTkD[i]->Draw("COLZ");
  }
}
} // ~namespace Analysis
} // ~namespace MAUS
