

#ifndef CUTSBASE_HH
#define CUTSBASE_HH

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"


namespace mica {

class CutsBase {
  public:
    CutsBase() {}
    virtual ~CutsBase() {}

    virtual bool Cut(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) = 0;
};
} // ~namespace mica


#endif
