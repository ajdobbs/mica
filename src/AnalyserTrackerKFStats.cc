/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTrackerKFStats.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"

namespace mica {

AnalyserTrackerKFStats::AnalyserTrackerKFStats() : mHChiSqTKU{nullptr},
                                                   mHChiSqTKD{nullptr},
                                                   mHPValueTKU{nullptr},
                                                   mHPValueTKD{nullptr} {
  int nbins = 100;
  mHChiSqTKU = new TH1D("hChiSqTKU", "KF #chi^{2}_{\nu} TkU", nbins, 0, 20);
  mHChiSqTKU->GetXaxis()->SetTitle("#chi^{2}_{\nu} ");
  mHChiSqTKD = new TH1D("hChiSqTKD", "KF  #chi^{2}_{\nu}  TkD", nbins, 0, 20);
  mHChiSqTKD->GetXaxis()->SetTitle("#chi^{2}_{\nu} ");

  mHPValueTKU = new TH1D("hPValueTKU", "KF p-value TkU", nbins, 0, 1);
  mHPValueTKU->GetXaxis()->SetTitle("p-value");
  mHPValueTKD = new TH1D("hPValueTKD", "KF p-value TkD", nbins, 0, 1);
  mHPValueTKD->GetXaxis()->SetTitle("p-value");
}

bool AnalyserTrackerKFStats::analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  for (auto trk : sfevt->scifitracks()) {
    if (trk->tracker() == 0) {
      mHChiSqTKU->Fill(trk->chi2() / trk->ndf());
      mHPValueTKU->Fill(trk->P_value());
    } else if (trk->tracker() == 1) {
      mHChiSqTKD->Fill(trk->chi2() / trk->ndf());
      mHPValueTKD->Fill(trk->P_value());
    }
  }
}

bool AnalyserTrackerKFStats::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  GetPads()[0]->Divide(2, 2);

  GetPads()[0]->cd(1);
  mHChiSqTKU->Draw();
  GetPads()[0]->cd(2);
  mHPValueTKU->Draw();
  GetPads()[0]->cd(3);
  mHChiSqTKD->Draw();
  GetPads()[0]->cd(4);
  mHPValueTKD->Draw();

  return true;
}
} // ~namespace mica
