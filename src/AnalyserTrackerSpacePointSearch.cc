#include "mica/AnalyserTrackerSpacePointSearch.hh"

namespace mica {

AnalyserTrackerSpacePointSearch::AnalyserTrackerSpacePointSearch() : mHSeeds(NULL), mHAddOns(NULL) {
  mHSeeds = new TH2D("hSeeds", "Seed Pull vs NPE", 100, 0, 30, 100, 0, 200);
  mHSeeds->GetXaxis()->SetTitle("Pull (mm)");
  mHSeeds->GetYaxis()->SetTitle("NPE");

  mHAddOns = new TH2D("hAddOns", "Add-On Seed Pull vs NPE", 100, 0, 30, 100, 0, 200);
  mHAddOns->GetXaxis()->SetTitle("Pull (mm)");
  mHAddOns->GetYaxis()->SetTitle("NPE");
}

bool AnalyserTrackerSpacePointSearch::analyse(MAUS::ReconEvent* const aReconEvent,
                                              MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  for (auto trk : sfevt->helicalprtracks()) {
    for (auto sp : trk->get_spacepoints_pointers()) {
      double npe = sp->get_npe();
      double pull = sp->get_prxy_pull();
      mHSeeds->Fill(pull, npe);
      if (sp->get_add_on()) {
        mHAddOns->Fill(pull, npe);
      }
    } // Loop over seed spacepoints
  } // ~Loop over tracks
  return true;
}

void AnalyserTrackerSpacePointSearch::draw(TVirtualPad* aPad) {
  aPad->Divide(3, 2);
  aPad->cd(1);
  mHSeeds->Draw("COLZ");
  aPad->cd(2);
  mHSeeds->ProjectionX()->Draw();
  aPad->cd(3);
  mHSeeds->ProjectionY()->Draw();
  aPad->cd(4);
  mHAddOns->Draw("COLZ");
  aPad->cd(5);
  mHAddOns->ProjectionX()->Draw();
  aPad->cd(6);
  mHAddOns->ProjectionY()->Draw();
}
} // ~namespace mica
