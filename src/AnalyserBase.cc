/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "TCanvas.h"

#include "mica/AnalyserBase.hh"

namespace mica {

AnalyserBase::AnalyserBase() {
  // Do nothing
  mStyle = std::make_shared<TStyle>(*gStyle);
}

AnalyserBase::~AnalyserBase() {
  // if (mStyle) delete mStyle;
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

TVirtualPad* AnalyserBase::Draw(TVirtualPad* aPad) {
  if (!aPad) {
    aPad = new TCanvas();
  }
  draw(aPad);

  // If the mPads vector is empty, add the current pad to the list
  if (mPads.size() == 0)
    AddPad(aPad);

  return aPad;
}
} // ~namespace mica
