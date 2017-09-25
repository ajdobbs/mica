/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTrackerSpacePoints.hh"

#include "TCanvas.h"

#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"

namespace mica {

AnalyserTrackerSpacePoints::AnalyserTrackerSpacePoints() : mHNpeTKU{nullptr},
                                                           mHNpeTKD{nullptr},
                                                           mHStationNumTKU{nullptr},
                                                           mHStationNumTKD{nullptr},
                                                           mHXYTKU{nullptr},
                                                           mHXYTKD{nullptr} {

  // Initialise the general spacepoint info plots
  mHNpeTKU = new TH1D("hNpeTKU", "NPE TkU", 100, 0, 200);
  mHNpeTKU->GetXaxis()->SetTitle("NPE");
  mHNpeTKD = new TH1D("hNpeTKD", "NPE TkD", 100, 0, 200);
  mHNpeTKD->GetXaxis()->SetTitle("NPE");

  mHStationNumTKU = new TH1D("hStationNumTKU", "Station Number TkU", 5, 1, 6);
  mHStationNumTKU->GetXaxis()->SetTitle("Station Number");
  mHStationNumTKD = new TH1D("hStationNumTKD", "Station Number TkD", 5, 1, 6);
  mHStationNumTKD->GetXaxis()->SetTitle("Station Number");

  mHXYTKU = new TH2D("hXYTKU", "Position TkU", 100, -150, 150, 100, -150, 150);
  mHXYTKU->GetXaxis()->SetTitle("x (mm)");
  mHXYTKU->GetYaxis()->SetTitle("y (mm)");
  mHXYTKD = new TH2D("hXYTKD", "Position TkD", 100, -150, 150, 100, -150, 150);
  mHXYTKD->GetXaxis()->SetTitle("x (mm)");
  mHXYTKD->GetYaxis()->SetTitle("y (mm)");

  // Initialise the x-y per station spacepoint plots
  for (int iStation = 0; iStation < 5; ++iStation) {
    std::string title = "Station " + std::to_string(iStation+1);
    std::string name = "S" + std::to_string(iStation+1);
    std::string tku_title = "TkU XY " + title;
    std::string tku_name = "hTkUXY" + name;
    std::string tkd_title = "TkD XY " + title;
    std::string tkd_name = "hTkDXY" + name;
    mXYPerStationTkU.push_back(std::unique_ptr<TH2D>(new TH2D(tku_name.c_str(), tku_title.c_str(),
                                                              100, -150, 150, 100, -150, 150)));
    mXYPerStationTkD.push_back(std::unique_ptr<TH2D>(new TH2D(tkd_name.c_str(), tkd_title.c_str(),
                                                              100, -150, 150, 100, -150, 150)));
    mXYPerStationTkU[iStation]->GetXaxis()->SetTitle("x (mm)");
    mXYPerStationTkU[iStation]->GetYaxis()->SetTitle("y (mm)");
    mXYPerStationTkD[iStation]->GetXaxis()->SetTitle("x (mm)");
    mXYPerStationTkD[iStation]->GetYaxis()->SetTitle("y (mm)");
  }
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
      mHXYTKU->Fill(sp->get_position().x(), sp->get_position().y());
      mXYPerStationTkU[sp->get_station() - 1]->Fill(sp->get_position().x(), sp->get_position().y());
    } else {
      mHNpeTKD->Fill(sp->get_npe());
      mHStationNumTKD->Fill(sp->get_station());
      mHXYTKD->Fill(sp->get_position().x(), sp->get_position().y());
      mXYPerStationTkD[sp->get_station() - 1]->Fill(sp->get_position().x(), sp->get_position().y());
    }
  }

  return true;
}

void AnalyserTrackerSpacePoints::draw(TVirtualPad* aPad) {
  // Draw the general spacepoint information plots
  aPad->Divide(3, 2);
  aPad->cd(1);
  mHXYTKU->Draw("COLZ");
  aPad->cd(2);
  mHStationNumTKU->Draw();
  aPad->cd(3);
  mHNpeTKU->Draw();
  aPad->cd(4);
  mHXYTKD->Draw("COLZ");
  aPad->cd(5);
  mHStationNumTKD->Draw();
  aPad->cd(6);
  mHNpeTKD->Draw();

  // Draw the real space plots per station
  int nStations = 5;
  TVirtualPad* padXY = new TCanvas();
  padXY->cd();
  padXY->Divide(nStations, 2);
  for (int iStation = 0; iStation < nStations; ++iStation) {
    padXY->cd(iStation+1);
    mXYPerStationTkU[iStation]->Draw("COLZ");
    padXY->cd(nStations+iStation+1);
    mXYPerStationTkD[iStation]->Draw("COLZ");
  }

  // Add the pads
  AddPad(aPad);
  AddPad(padXY);
}
} // ~namespace mica
