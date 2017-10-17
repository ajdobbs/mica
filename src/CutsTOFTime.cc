/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
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

