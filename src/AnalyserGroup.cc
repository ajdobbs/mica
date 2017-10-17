#include "mica/AnalyserGroup.hh"

namespace mica {

bool AnalyserGroup::Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  bool success = true;
  for (auto an : mAnalysers) {
    bool lSuccess = an->Analyse(aReconEvent, aMCEvent);
    if (!lSuccess) success = false;
  }
  return success;
}

// AnalyserGroup* AnalyserGroup::Clone() {
//   AnalyserGroup* newGroup = new AnalyserGroup();
//   for (auto an : mAnalysers) {
//     newGroup->AddAnalyser(mAnalysers.Clone());
//   }
// }

std::vector<std::shared_ptr<TVirtualPad>> AnalyserGroup::Draw() {
  std::vector<std::shared_ptr<TVirtualPad>> pads;
  for (auto an : mAnalysers) {
    an->Draw();
    auto new_pads = an->GetPads();
    pads.insert(std::end(pads), std::begin(new_pads), std::end(new_pads));
  }
  return pads;
}

void AnalyserGroup::MakePlots(const std::string& ofname) {
  std::vector<std::shared_ptr<TVirtualPad>> pads;
  std::vector<std::shared_ptr<TStyle> > styles;
  for (auto an : mAnalysers) {
    an->Draw();
    for (auto pad : an->GetPads()) {
      if (pad) {
        pads.push_back(pad);
        styles.push_back(an->GetStyle());
      }
    }
  }
  std::cout << "Found " << pads.size() << " canvases, saving to pdf." << std::endl;
  for (size_t i = 0; i < pads.size(); ++i) {
    if (styles[i]) styles[i]->cd();
    pads[i]->Update();
    if (i == 0) {
      pads[i]->SaveAs((ofname + "(").c_str(), "pdf");
    } else if (i == (pads.size() - 1)) {
      pads[i]->SaveAs((ofname + ")").c_str(), "pdf");
    } else {
      pads[i]->SaveAs(ofname.c_str(), "pdf");
    }
  }
}

bool AnalyserGroup::Merge(AnalyserGroup* aAnalyserGroup) {
  if (mAnalysers.size() != aAnalyserGroup->NAnalysers())
    return false;

  bool success = true;
  for (size_t i = 0; i < mAnalysers.size(); ++i) {
    bool lSuccess = mAnalysers[i]->Merge((*aAnalyserGroup)[i]);
    if (!lSuccess) success = false;
  }
  return success;
}
} // ~namespace mica
