/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */
#include "mica/AnalyserTrackerPRSeedResidual.hh"

#include <string>

namespace mica {

AnalyserTrackerPRSeedResidual::AnalyserTrackerPRSeedResidual() : mLogScale {false} {
  for (int i = 0; i < 5; ++i) {
    std::string tku_title = "TkU Seed Residual Station " + std::to_string(i+1);
    std::string tku_name= "hResidualsTkUS" + std::to_string(i+1);
    mHResidualsTkU.push_back(new TH1D(tku_name.c_str(), tku_title.c_str(), 100, -30, 30));
    mHResidualsTkU[i]->GetXaxis()->SetTitle("Residual (mm)");

    std::string tkd_title = "TkD Seed Residual Station " + std::to_string(i+1);
    std::string tkd_name = "hResidualsTkDS" + std::to_string(i+1);
    mHResidualsTkD.push_back(new TH1D(tkd_name.c_str(), tkd_title.c_str(), 100, -30, 30));
    mHResidualsTkD[i]->GetXaxis()->SetTitle("Residual (mm)");
  }
}

bool AnalyserTrackerPRSeedResidual::analyse(MAUS::ReconEvent* const aReconEvent,
                                            MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  for (auto trk : sfevt->helicalprtracks()) {
    for (auto sp : trk->get_spacepoints_pointers()) {
      double npe = sp->get_npe();
      double pull = sp->get_prxy_pull();
      if (sp->get_tracker() == 0) {
        mHResidualsTkU[sp->get_station() - 1]->Fill(pull);
      } else {
        mHResidualsTkD[sp->get_station() - 1]->Fill(pull);
      }
    } // Loop over seed spacepoints
  } // ~Loop over tracks
  return true;
}

bool AnalyserTrackerPRSeedResidual::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  aPad->Divide(5, 2);
  for (int i = 0; i < 5; ++i) {
    aPad->cd(i+1);
    if (mLogScale) aPad->GetPad(i+1)->SetLogy(1);
    mHResidualsTkU[i]->Draw();
    aPad->cd(i+6);
    if (mLogScale) aPad->GetPad(i+6)->SetLogy(1);
    mHResidualsTkD[i]->Draw();
  }
  return true;
}
} // ~namespace mica
