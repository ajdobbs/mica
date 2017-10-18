/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef AnalyserTrackerMCPRResiduals_HH
#define AnalyserTrackerMCPRResiduals_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH1.h"
#include "TH2.h"

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

  private:
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent,
                         MAUS::MCEvent* const aMCEvent) override;
    virtual bool draw(std::shared_ptr<TVirtualPad> aPad) override;
    virtual void merge(AnalyserTrackerMCPRResiduals* aAnalyser) override;

    const double mBfield = 3.0;

    TH1D* mHTkUMCPositionX;
    TH1D* mHTkUMCPositionY;
    TH1D* mHTkUMCMomentumT;
    TH1D* mHTkUMCMomentumZ;

    TH1D* mHTkURecPositionX;
    TH1D* mHTkURecPositionY;
    TH1D* mHTkURecMomentumT;
    TH1D* mHTkURecMomentumZ;

    TH1D* mHTkUPositionResidualsX;
    TH1D* mHTkUPositionResidualsY;
    TH1D* mHTkUMomentumResidualsT;
    TH1D* mHTkUMomentumResidualsZ;

    TH2D* mHTkUPtResPt;
    TH2D* mHTkUPzResPt;
    TH2D* mHTkUPtResPzRec;
    TH2D* mHTkUPzResPzRec;

    TH1D* mHTkDMCPositionX;
    TH1D* mHTkDMCPositionY;
    TH1D* mHTkDMCMomentumT;
    TH1D* mHTkDMCMomentumZ;

    TH1D* mHTkDRecPositionX;
    TH1D* mHTkDRecPositionY;
    TH1D* mHTkDRecMomentumT;
    TH1D* mHTkDRecMomentumZ;

    TH1D* mHTkDPositionResidualsX;
    TH1D* mHTkDPositionResidualsY;
    TH1D* mHTkDMomentumResidualsT;
    TH1D* mHTkDMomentumResidualsZ;

    TH2D* mHTkDPtResPt;
    TH2D* mHTkDPzResPt;
    TH2D* mHTkDPtResPzRec;
    TH2D* mHTkDPzResPzRec;
};
} // ~namespace mica

#endif
