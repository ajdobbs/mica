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
  int nbins = 100;
  mHCircleChiSqTKU = new TH1D("hCircleChiSqTKU", "PR Circle #chi^{2}_{\nu} TkU", nbins, 0, 20);
  mHCircleChiSqTKU->GetXaxis()->SetTitle("Circle #chi^{2}_{\nu} ");
  mHCircleChiSqTKD = new TH1D("hCircleChiSqTKD", "PR  Circle #chi^{2}_{\nu}  TkD", nbins, 0, 20);
  mHCircleChiSqTKD->GetXaxis()->SetTitle("Circle #chi^{2}_{\nu} ");

  mHSZChiSqTKU = new TH1D("hSZChiSqTKU", "PR SZ #chi^{2}_{\nu}  TkU", nbins, 0, 50);
  mHSZChiSqTKU->GetXaxis()->SetTitle("SZ #chi^{2}_{\nu} ");
  mHSZChiSqTKD = new TH1D("hSZChiSqTKD", "PR SZ #chi^{2}_{\nu}  TkD", nbins, 0, 50);
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

bool AnalyserTrackerPRStats::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetStyle()->SetOptStat(111111);
  GetPads()[0]->Divide(4, 2);

  GetPads()[0]->cd(1);
  mHCircleChiSqTKU->Draw();
  GetPads()[0]->cd(2);
  GetPads()[0]->GetPad(2)->SetLogy(1);
  mHCircleChiSqTKU->Draw();

  GetPads()[0]->cd(3);
  mHSZChiSqTKU->Draw();
  GetPads()[0]->cd(4);
  GetPads()[0]->GetPad(4)->SetLogy(1);
  mHSZChiSqTKU->Draw();

  GetPads()[0]->cd(5);
  mHCircleChiSqTKD->Draw();
  GetPads()[0]->cd(6);
  GetPads()[0]->GetPad(6)->SetLogy(1);
  mHCircleChiSqTKD->Draw();

  GetPads()[0]->cd(7);
  mHSZChiSqTKD->Draw();
  GetPads()[0]->cd(8);
  GetPads()[0]->GetPad(8)->SetLogy(1);
  mHSZChiSqTKD->Draw();

  return true;
}
} // ~namespace mica

