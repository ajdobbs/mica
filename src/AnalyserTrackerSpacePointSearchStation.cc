/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTrackerSpacePointSearchStation.hh"

#include <string>

namespace mica {

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

bool AnalyserTrackerSpacePointSearchStation::analyse(MAUS::ReconEvent* const aReconEvent,
                                              MAUS::MCEvent* const aMCEvent) {
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

bool AnalyserTrackerSpacePointSearchStation::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetPads()[0]->Divide(5, 2);
  for (int i = 0; i < 5; ++i) {
    GetPads()[0]->cd(i+1);
    mHSeeds[i]->Draw("COLZ");
    GetPads()[0]->cd(i+6);
    mHAddOns[i]->Draw("COLZ");
  }
  return true;
}
} // ~namespace mica

