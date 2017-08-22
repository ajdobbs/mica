#ifndef CUTSTOFTIME_HH
#define CUTSTOFTIME_HH

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/CutsBase.hh"


namespace mica {

class CutsTOFTime : public CutsBase {
  public:
    CutsTOFTime();
    virtual ~CutsTOFTime() {}

    virtual bool Cut(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

  private:
    double mLowerTimeCut;
    double mUpperTimeCut;
};
} // ~namespace mica

#endif
