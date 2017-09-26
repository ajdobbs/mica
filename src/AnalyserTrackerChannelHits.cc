/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */


#include "mica/AnalyserTrackerChannelHits.hh"

#include "TCanvas.h"

#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiDigit.hh"

namespace mica {

AnalyserTrackerChannelHits::AnalyserTrackerChannelHits() {
  int nChannels = 214;
  for (int iStation = 0; iStation < 5; ++iStation) {
    for (int iPlane = 0; iPlane < 3; ++iPlane) {
      std::string title = "Station " + std::to_string(iStation+1)
                          + " Plane " + std::to_string(iPlane);
      std::string name = "S" + std::to_string(iStation+1) + "P" + std::to_string(iPlane);

      // Set up channel occupancy histograms
      std::string tku_title_occ = "TkU Channel Occupancy " + title;
      std::string tku_name_occ = "TkUOcc" + name;
      std::string tkd_title_occ = "TkD Channel Occupancy " + title;
      std::string tkd_name_occ = "TkDOcc" + name;
      mTkU.emplace_back(new TH1I(tku_name_occ.c_str(), tku_title_occ.c_str(),
                                 nChannels, 0, nChannels));
      mTkD.emplace_back(new TH1I(tkd_name_occ.c_str(), tkd_title_occ.c_str(),
                                 nChannels, 0, nChannels));
      mTkU.back()->GetXaxis()->SetTitle("Channel Number");
      mTkD.back()->GetXaxis()->SetTitle("Channel Number");

      // Set up npe vs channel number plots
      std::string tku_title_npe = "TkU Channel vs NPE " + title;
      std::string tku_name_npe = "TkUNPE" + name;
      std::string tkd_title_npe = "TkD Channel vs NPE " + title;
      std::string tkd_name_npe = "TkDNPE" + name;
      mNPETkU.emplace_back(new TH2D(tku_name_npe.c_str(), tku_title_npe.c_str(),
                                    nChannels, 0, nChannels, 100, 0, 70));
      mNPETkD.emplace_back(new TH2D(tkd_name_npe.c_str(), tkd_title_npe.c_str(),
                                    nChannels, 0, nChannels, 100, 0, 70));
      mNPETkU.back()->GetXaxis()->SetTitle("Channel Number");
      mNPETkU.back()->GetYaxis()->SetTitle("NPE");
      mNPETkD.back()->GetXaxis()->SetTitle("Channel Number");
      mNPETkD.back()->GetYaxis()->SetTitle("NPE");
    }
  }
}

bool AnalyserTrackerChannelHits::analyse(MAUS::ReconEvent* const aReconEvent,
                                         MAUS::MCEvent* const aMCEvent) {
  // A few checks
  if (!aReconEvent)
    return false;
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  // Populate the plots
  for (auto dig : sfevt->digits()) {
    int index = 3*(dig->get_station()-1) + dig->get_plane();
    if (dig->get_tracker() == 0) {
      mTkU[index]->Fill(dig->get_channel());
      mNPETkU[index]->Fill(dig->get_channel(), dig->get_npe());
    } else if (dig->get_tracker() == 1) {
      mTkD[index]->Fill(dig->get_channel());
      mNPETkD[index]->Fill(dig->get_channel(), dig->get_npe());
    }
  }
  return true;
}

void AnalyserTrackerChannelHits::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  int nStations = 5;
  int nPlanes = 3;

  // Set up the pads
  std::vector<std::shared_ptr<TVirtualPad>> pads_occ;
  pads_occ.push_back(aPad);
  std::vector<std::shared_ptr<TVirtualPad>> pads_npe;
  pads_npe.push_back(std::shared_ptr<TVirtualPad>(new TCanvas()));
  for (int i = 1; i < nStations; ++i) {
    pads_occ.push_back(std::shared_ptr<TVirtualPad>(new TCanvas()));
    pads_npe.push_back(std::shared_ptr<TVirtualPad>(new TCanvas()));
  }

  // Draw the plots
  for (int iStation = 0; iStation < nStations; ++iStation) {
    pads_occ[iStation]->Divide(nPlanes, 2);
    pads_npe[iStation]->Divide(nPlanes, 2);
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane) {
      pads_occ[iStation]->cd(iPlane+1);
      int index = 3*iStation + iPlane;
      mTkU[index]->Draw();
      pads_npe[iStation]->cd(iPlane+1);
      mNPETkU[index]->Draw("COLZ");
      pads_occ[iStation]->cd(nPlanes + iPlane + 1);
      mTkD[index]->Draw();
      pads_npe[iStation]->cd(nPlanes + iPlane + 1);
      mNPETkD[index]->Draw("COLZ");
    }
  }

  // Add the pads to the analyser internal store
  for (int i = 0; i < nStations; ++i) {
    AddPad(pads_occ[i]);
  }
  for (int i = 0; i < nStations; ++i) {
    AddPad(pads_npe[i]);
  }
}

} // ~namespace mica
