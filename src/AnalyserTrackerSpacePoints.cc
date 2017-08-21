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

#include "mica/AnalyserTrackerSpacePoints.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"


namespace mica {

AnalyserTrackerSpacePoints::AnalyserTrackerSpacePoints() : mHNpeTKU(NULL),
                                                                     mHNpeTKD(NULL) {
  mHNpeTKU = new TH1D("hNpeTKU", "NPE TkU", 100, 0, 200);
  mHNpeTKU->GetXaxis()->SetTitle("NPE");
  mHNpeTKD = new TH1D("hNpeTKD", "NPE TkD", 100, 0, 200);
  mHNpeTKD->GetXaxis()->SetTitle("NPE");

  mHStationNumTKU = new TH1D("hStationNumTKU", "Station Number TkU", 5, 1, 6);
  mHStationNumTKU->GetXaxis()->SetTitle("Station Number");
  mHStationNumTKD = new TH1D("hStationNumTKD", "Station Number TkD", 5, 1, 6);
  mHStationNumTKD->GetXaxis()->SetTitle("Station Number");

  mHPositionTKU = new TH2D("hPosTKU", "Position TkU", 100, -150, 150, 100, -150, 150);
  mHPositionTKU->GetXaxis()->SetTitle("x (mm)");
  mHPositionTKU->GetYaxis()->SetTitle("y (mm)");
  mHPositionTKD = new TH2D("hPosTKD", "Position TkD", 100, -150, 150, 100, -150, 150);
  mHPositionTKD->GetXaxis()->SetTitle("x (mm)");
  mHPositionTKD->GetYaxis()->SetTitle("y (mm)");
}

bool AnalyserTrackerSpacePoints::analyse(MAUS::ReconEvent* const aReconEvent,
                                         MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  // std::cerr << "Found " << sfevt->scifitracks().size() << " tracks" << std::endl;
  for (auto sp : sfevt->spacepoints()) {
    if (sp->get_tracker() == 0) {
      mHNpeTKU->Fill(sp->get_npe());
      mHStationNumTKU->Fill(sp->get_station());
      mHPositionTKU->Fill(sp->get_position().x(), sp->get_position().y());
    } else {
      mHNpeTKD->Fill(sp->get_npe());
      mHStationNumTKD->Fill(sp->get_station());
      mHPositionTKD->Fill(sp->get_position().x(), sp->get_position().y());
    }
  }

  return true;
}

void AnalyserTrackerSpacePoints::draw(TVirtualPad* aPad) {
  aPad->Divide(3, 2);
  aPad->cd(1);
  mHPositionTKU->Draw("COLZ");
  aPad->cd(2);
  mHStationNumTKU->Draw();
  aPad->cd(3);
  mHNpeTKU->Draw();
  aPad->cd(4);
  mHPositionTKD->Draw("COLZ");
  aPad->cd(5);
  mHStationNumTKD->Draw();
  aPad->cd(6);
  mHNpeTKD->Draw();
}
} // ~namespace mica
