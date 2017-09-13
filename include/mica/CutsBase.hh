/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef CUTSBASE_HH
#define CUTSBASE_HH

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"

namespace mica {

/** @class CutsBase
 *         Base class for cuts, main Cut method returns true if event passes cut, false otherwise
 *  @author A. Dobbs
 */
class CutsBase {
  public:
    CutsBase() {}
    virtual ~CutsBase() {}

    /** @brief Apply the cut to the event, return true if passed, false if not */
    virtual bool Cut(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) = 0;
};
} // ~namespace mica

#endif
