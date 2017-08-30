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

std::vector<TVirtualPad*> AnalyserGroup::Draw() {
  std::vector<TVirtualPad*> pads;
  for (auto an : mAnalysers) {
    an->Draw();
    auto new_pads = an->GetPads();
    pads.insert(std::end(pads), std::begin(new_pads), std::end(new_pads));
  }
  return pads;
}

bool AnalyserGroup::Merge(AnalyserGroup* aAnalyserGroup) {
  if (mAnalysers.size() != aAnalyserGroup.GetAnalysers().size())
    return false;

  bool success = true;
  for (size_t i = 0; i < mAnalysers.size(); ++i) {
    bool lSuccess = mAnalysers[i]->Merge(aAnalyserGroup.GetAnalysers()[i]);
    if (!lSuccess) success = false;
  }
  return success;
}
} // ~namespace mica
