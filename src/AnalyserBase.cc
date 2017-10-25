/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "TCanvas.h"

#include "mica/AnalyserBase.hh"

namespace mica {

AnalyserBase::AnalyserBase() {
  mStyle = std::make_shared<TStyle>(*gStyle); // Make a style for this analyser
  // AddPad(std::shared_ptr<TVirtualPad>(new TCanvas())); // Have a default canvas ready
}

AnalyserBase::~AnalyserBase() {
  // Do nothing (members are shared pointers which will take care of themselves)
}

bool AnalyserBase::ApplyCuts(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  for (auto cut : mCuts) {
    bool result = cut->Cut(aReconEvent, aMCEvent);
    if (!result)
      return false;
  }
  return true;
}

bool AnalyserBase::Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  bool result = ApplyCuts(aReconEvent, aMCEvent);
  if (!result)
    return false;

  return analyse(aReconEvent, aMCEvent);
}

std::shared_ptr<TVirtualPad> AnalyserBase::Draw() {
  if (mPads.size() == 0) { // No canvases ready so set one up
    AddPad(std::shared_ptr<TVirtualPad>(new TCanvas()));
  }
  bool success = draw(GetPads()[0]);

  return mPads[0];
}
} // ~namespace mica
