#ifndef AnalyserTrackerMCPRResiduals_HH
#define AnalyserTrackerMCPRResiduals_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH1.h"

#include "mica/AnalyserBase.hh"
#include "mica/IAnalyser.hh"

#include "src/common_cpp/DataStructure/ReconEvent.hh"

namespace mica {

/** @class AnalyserTrackerMCPRResiduals
 *         Analyser class which calculates pattern recognition position and momentum residuals.
 *  @author A. Dobbs
 */

class AnalyserTrackerMCPRResiduals : public IAnalyser<AnalyserTrackerMCPRResiduals> {
  public:
    AnalyserTrackerMCPRResiduals();

    virtual ~AnalyserTrackerMCPRResiduals() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent,
                         MAUS::MCEvent* const aMCEvent) override;

    virtual void draw(TVirtualPad* aPad) override;

    virtual void merge(AnalyserTrackerMCPRResiduals* aAnalyser) override;

  private:
    const double mBfield = 3.0;

    TH1D* mHTkUPositionResidualsX;
    TH1D* mHTkUPositionResidualsY;
    TH1D* mHTkUMomentumResidualsT;
    TH1D* mHTkUMomentumResidualsZ;

    TH1D* mHTkDPositionResidualsX;
    TH1D* mHTkDPositionResidualsY;
    TH1D* mHTkDMomentumResidualsT;
    TH1D* mHTkDMomentumResidualsZ;
};
} // ~namespace mica

#endif
