#include "mica/AnalyserTrackerPRSeedNPEResidual.hh"

#include <string>


namespace mica {

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

bool AnalyserTrackerPRSeedNPEResidual::analyse(MAUS::ReconEvent* const aReconEvent,
                                              MAUS::MCEvent* const aMCEvent) {
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
} // ~namespace mica
