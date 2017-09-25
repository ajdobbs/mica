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

  // Some parameters for the plots
  int nbins = 100;
  double xmax = 200.0;
  double xmin = -200.0;
  double ymax = 200.0;
  double ymin = -200.0;
  std::string xlabel = "x (mm)";
  std::string ylabel = "y (mm)";

  // Initialise the general spacepoint info plots
  mHNpeTKU = new TH1D("hNpeTKU", "NPE TkU", nbins, 0, 200);
  mHNpeTKU->GetXaxis()->SetTitle("NPE");
  mHNpeTKD = new TH1D("hNpeTKD", "NPE TkD", nbins, 0, 200);
  mHNpeTKD->GetXaxis()->SetTitle("NPE");

  mHStationNumTKU = new TH1D("hStationNumTKU", "Station Number TkU", 5, 1, 6);
  mHStationNumTKU->GetXaxis()->SetTitle("Station Number");
  mHStationNumTKD = new TH1D("hStationNumTKD", "Station Number TkD", 5, 1, 6);
  mHStationNumTKD->GetXaxis()->SetTitle("Station Number");

  mHXYTKU = new TH2D("hXYTKU", "Position TkU", nbins, xmin, xmax, nbins, ymin, ymax);
  mHXYTKU->GetXaxis()->SetTitle(xlabel.c_str());
  mHXYTKU->GetYaxis()->SetTitle(ylabel.c_str());
  mHXYTKD = new TH2D("hXYTKD", "Position TkD", nbins, xmin, xmax, nbins, ymin, ymax);
  mHXYTKD->GetXaxis()->SetTitle(xlabel.c_str());
  mHXYTKD->GetYaxis()->SetTitle(ylabel.c_str());

  // Initialise the x-y per station spacepoint plots (for all, triplets only, doublets only)
  for (int iStation = 0; iStation < 5; ++iStation) {
    std::string name = "S" + std::to_string(iStation+1);

    std::string tku_title = "TkU XY " + name;
    std::string tku_name = "hTkUXY" + name;
    std::string tkd_title = "TkD XY " + name;
    std::string tkd_name = "hTkDXY" + name;
    mXYPerStationTkU.push_back(std::unique_ptr<TH2D>(new TH2D(tku_name.c_str(), tku_title.c_str(),
        nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationTkD.push_back(std::unique_ptr<TH2D>(new TH2D(tkd_name.c_str(), tkd_title.c_str(),
        nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationTkU[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationTkU[iStation]->GetYaxis()->SetTitle(ylabel.c_str());
    mXYPerStationTkD[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationTkD[iStation]->GetYaxis()->SetTitle(ylabel.c_str());

    std::string tku_tri_title = "TkU XY Triplets " + name;
    std::string tku_tri_name = "hTkUXYTriplets" + name;
    std::string tkd_tri_title = "TkD XY Triplets " + name;
    std::string tkd_tri_name = "hTkDXYTriplets" + name;
    mXYPerStationTripletsTkU.push_back(std::unique_ptr<TH2D>(new TH2D(tku_tri_name.c_str(),
        tku_tri_title.c_str(), nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationTripletsTkD.push_back(std::unique_ptr<TH2D>(new TH2D(tkd_tri_name.c_str(),
        tkd_tri_title.c_str(), nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationTripletsTkU[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationTripletsTkU[iStation]->GetYaxis()->SetTitle(ylabel.c_str());
    mXYPerStationTripletsTkD[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationTripletsTkD[iStation]->GetYaxis()->SetTitle(ylabel.c_str());

    std::string tku_dou_title = "TkU XY Doublets " + name;
    std::string tku_dou_name = "hTkUXYDoublets" + name;
    std::string tkd_dou_title = "TkD XY Doublets " + name;
    std::string tkd_dou_name = "hTkDXYDoublets" + name;
    mXYPerStationDoubletsTkU.push_back(std::unique_ptr<TH2D>(new TH2D(tku_dou_name.c_str(),
        tku_dou_title.c_str(), nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationDoubletsTkD.push_back(std::unique_ptr<TH2D>(new TH2D(tkd_dou_name.c_str(),
        tkd_dou_title.c_str(), nbins, xmin, xmax, nbins, ymin, ymax)));
    mXYPerStationDoubletsTkU[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationDoubletsTkU[iStation]->GetYaxis()->SetTitle(ylabel.c_str());
    mXYPerStationDoubletsTkD[iStation]->GetXaxis()->SetTitle(xlabel.c_str());
    mXYPerStationDoubletsTkD[iStation]->GetYaxis()->SetTitle(ylabel.c_str());
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
      if (sp->get_channels_pointers().size() == 3) {
        mXYPerStationTripletsTkU[sp->get_station() - 1]->Fill(sp->get_position().x(),
                                                              sp->get_position().y());
      } else if (sp->get_channels_pointers().size() == 2) {
        mXYPerStationDoubletsTkU[sp->get_station() - 1]->Fill(sp->get_position().x(),
                                                              sp->get_position().y());
      }
    } else {
      mHNpeTKD->Fill(sp->get_npe());
      mHStationNumTKD->Fill(sp->get_station());
      mHXYTKD->Fill(sp->get_position().x(), sp->get_position().y());
      mXYPerStationTkD[sp->get_station() - 1]->Fill(sp->get_position().x(), sp->get_position().y());
      if (sp->get_channels_pointers().size() == 3) {
        mXYPerStationTripletsTkD[sp->get_station() - 1]->Fill(sp->get_position().x(),
                                                              sp->get_position().y());
      } else if (sp->get_channels_pointers().size() == 2) {
        mXYPerStationDoubletsTkD[sp->get_station() - 1]->Fill(sp->get_position().x(),
                                                              sp->get_position().y());
      }
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
  TVirtualPad* padXYTriplets = new TCanvas();
  TVirtualPad* padXYDoublets = new TCanvas();
  padXY->Divide(nStations, 2);
  padXYTriplets->Divide(nStations, 2);
  padXYDoublets->Divide(nStations, 2);
  for (int iStation = 0; iStation < nStations; ++iStation) {
    padXY->cd(iStation+1);
    mXYPerStationTkU[iStation]->Draw("COLZ");
    padXYTriplets->cd(iStation+1);
    mXYPerStationTripletsTkU[iStation]->Draw("COLZ");
    padXYDoublets->cd(iStation+1);
    mXYPerStationDoubletsTkU[iStation]->Draw("COLZ");
    padXY->cd(nStations+iStation+1);
    mXYPerStationTkD[iStation]->Draw("COLZ");
    padXYTriplets->cd(nStations+iStation+1);
    mXYPerStationTripletsTkD[iStation]->Draw("COLZ");
    padXYDoublets->cd(nStations+iStation+1);
    mXYPerStationDoubletsTkD[iStation]->Draw("COLZ");
  }

  // Add the pads
  AddPad(aPad);
  AddPad(padXY);
  AddPad(padXYTriplets);
  AddPad(padXYDoublets);
}
} // ~namespace mica
