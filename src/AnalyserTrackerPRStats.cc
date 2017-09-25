/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserTrackerPRStats.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"

namespace mica {

AnalyserTrackerPRStats::AnalyserTrackerPRStats() : mHCircleChiSqTKU(NULL),
                                                   mHCircleChiSqTKD(NULL),
                                                   mHSZChiSqTKU(NULL),
                                                   mHSZChiSqTKD(NULL) {
  mHCircleChiSqTKU = new TH1D("hCircleChiSqTKU", "PR Circle #chi^{2}_{\nu} TkU", 100, 0, 50);
  mHCircleChiSqTKU->GetXaxis()->SetTitle("Circle #chi^{2}_{\nu} ");
  mHCircleChiSqTKD = new TH1D("hCircleChiSqTKD", "PR  Circle #chi^{2}_{\nu}  TkD", 100, 0, 50);
  mHCircleChiSqTKD->GetXaxis()->SetTitle("Circle #chi^{2}_{\nu} ");

  mHSZChiSqTKU = new TH1D("hSZChiSqTKU", "PR SZ #chi^{2}_{\nu}  TkU", 100, 0, 50);
  mHSZChiSqTKU->GetXaxis()->SetTitle("SZ #chi^{2}_{\nu} ");
  mHSZChiSqTKD = new TH1D("hSZChiSqTKD", "PR SZ #chi^{2}_{\nu}  TkD", 100, 0, 50);
  mHSZChiSqTKD->GetXaxis()->SetTitle("SZ #chi^{2}_{\nu} ");
}

bool AnalyserTrackerPRStats::analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt)
    return false;

  for (auto trk : sfevt->helicalprtracks()) {
    if (trk->get_tracker() == 0) {
      mHCircleChiSqTKU->Fill(trk->get_circle_chisq() / trk->get_circle_ndf());
      mHSZChiSqTKU->Fill(trk->get_line_sz_chisq() / trk->get_line_sz_ndf() );
    } else if (trk->get_tracker() == 1) {
      mHCircleChiSqTKD->Fill(trk->get_circle_chisq() / trk->get_circle_ndf());
      mHSZChiSqTKD->Fill(trk->get_line_sz_chisq() / trk->get_line_sz_ndf());
    }
  }
}

void AnalyserTrackerPRStats::draw(TVirtualPad* aPad) {
  GetStyle()->SetOptStat(111111);
  aPad->Divide(4, 2);

  aPad->cd(1);
  mHCircleChiSqTKU->Draw();
  aPad->cd(2);
  aPad->GetPad(2)->SetLogy(1);
  mHCircleChiSqTKU->Draw();

  aPad->cd(3);
  mHSZChiSqTKU->Draw();
  aPad->cd(4);
  aPad->GetPad(4)->SetLogy(1);
  mHSZChiSqTKU->Draw();

  aPad->cd(5);
  mHCircleChiSqTKD->Draw();
  aPad->cd(6);
  aPad->GetPad(6)->SetLogy(1);
  mHCircleChiSqTKD->Draw();

  aPad->cd(7);
  mHSZChiSqTKD->Draw();
  aPad->cd(8);
  aPad->GetPad(8)->SetLogy(1);
  mHSZChiSqTKD->Draw();
}
} // ~namespace mica
