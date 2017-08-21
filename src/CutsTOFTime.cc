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

#include <vector>

#include "mica/CutsTOFTime.hh"

#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/TOFEventSpacePoint.hh"
#include "src/common_cpp/DataStructure/TOFSpacePoint.hh"


namespace mica {

CutsTOFTime::CutsTOFTime() : mLowerTimeCut(27.0), mUpperTimeCut(50.0) {}

bool CutsTOFTime::Cut(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  MAUS::TOFEvent* tofEvent = aReconEvent->GetTOFEvent();
  if (!tofEvent)
    return false;

  std::vector<MAUS::TOFSpacePoint>* tof1Spoints =
      tofEvent->GetTOFEventSpacePointPtr()->GetTOF1SpacePointArrayPtr();
  std::vector<MAUS::TOFSpacePoint>* tof2Spoints =
      tofEvent->GetTOFEventSpacePointPtr()->GetTOF2SpacePointArrayPtr();

  if (tof1Spoints->size() != 1 || tof2Spoints->size() != 1)
    return false;

  double dt = tof2Spoints->at(0).GetTime() - tof1Spoints->at(0).GetTime();
  if (dt > mLowerTimeCut && dt < mUpperTimeCut)
    return true;

  return false;
}

} // ~namespace mica
