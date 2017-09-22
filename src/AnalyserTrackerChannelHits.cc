/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */


#include "mica/AnalyserTrackerChannelHits.hh"

#include "TCanvas.h"

#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiDigit.hh"

namespace mica {

AnalyserTrackerChannelHits::AnalyserTrackerChannelHits() {
  for (int iStation = 0; iStation < 5; ++iStation) {
    for (int iPlane = 0; iPlane < 3; ++iPlane) {
      std::string title = "Channel Occupancy Station " + std::to_string(iStation+1)
                          + " Plane " + std::to_string(iPlane);
      std::string name = "S" + std::to_string(iStation+1) + "P" + std::to_string(iPlane);
      std::string tku_title = "TkU " + title;
      std::string tku_name = "TkU " + name;
      std::string tkd_title = "TkD " + title;
      std::string tkd_name = "TkD " + name;
      mTkU.push_back(std::unique_ptr<TH1I>(new TH1I(tku_name.c_str(),
                                                    tku_title.c_str(), 214, 0, 213)));
      mTkD.push_back(std::unique_ptr<TH1I>(new TH1I(tkd_name.c_str(),
                                                    tkd_title.c_str(), 214, 0, 213)));
      mTkU[iStation+iPlane]->GetXaxis()->SetTitle("Channel Number");
      mTkD[iStation+iPlane]->GetXaxis()->SetTitle("Channel Number");
    }
  }
}

bool AnalyserTrackerChannelHits::analyse(MAUS::ReconEvent* const aReconEvent,
                                         MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  for (auto dig : sfevt->digits()) {
    int index = 3*(dig->get_station()-1) + dig->get_plane();
    if (dig->get_tracker() == 0) {
      mTkU[index]->Fill(dig->get_channel());
    } else if (dig->get_tracker() == 1) {
      mTkD[index]->Fill(dig->get_channel());
    }
  }
  return true;
}

void AnalyserTrackerChannelHits::draw(TVirtualPad* aPad) {
  GetStyle()->SetOptStat(111111);
  int nPadsPerStation = 3;
  int nStations = 5;
  int nPlanes = 3;

  std::vector<TVirtualPad*> pads;
  pads.push_back(aPad);
  for (int i = 1; i < nStations; ++i) {
    TVirtualPad* p = new TCanvas();
    pads.push_back(p);
  }

  for (int iStation = 0; iStation < nStations; ++iStation) {
    pads[iStation]->Divide(nPlanes, 2);
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane) {
      pads[iStation]->cd(iPlane+1);
      int index = 3*iStation + iPlane;
      mTkU[index]->Draw();
      pads[iStation]->cd(nPlanes + iPlane + 1);
      mTkD[index]->Draw();
    }
  }

  for (int i = 0; i < nStations; ++i) {
    AddPad(pads[i]);
  }
}

} // ~namespace mica
