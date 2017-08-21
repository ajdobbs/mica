/* This file is part of MAUS: http://micewww.pp.rl.ac.uk/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "TCanvas.h"

#include "mica/AnalyserBase.hh"


namespace mica {

AnalyserBase::AnalyserBase() {
  // Do nothing
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
  return aPad;
}
} // ~namespace mica

