#ifndef AnalyserTrackerMCPRResiduals_HH
#define AnalyserTrackerMCPRResiduals_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH1.h"

#include "mica/AnalyserTrackerMC.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"



namespace mica {

/** @class AnalyserTrackerMCPRResiduals
 *         Analyser class which calculates pattern recognition position and momentum residuals.
 *  @author A. Dobbs
 */

class AnalyserTrackerMCPRResiduals : public AnalyserTrackerMC {
  public:
    AnalyserTrackerMCPRResiduals();

    virtual ~AnalyserTrackerMCPRResiduals() {}

    virtual bool analyse_recon(MAUS::ReconEvent* const aReconEvent) override;

    virtual void draw(TVirtualPad* aPad) override;

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
