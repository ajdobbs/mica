/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERANGULARMOMENTUM_HH
#define ANALYSERTRACKERANGULARMOMENTUM_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "mica/AnalyserBase.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSeed.hh"
#include "src/common_cpp/DataStructure/SciFiTrack.hh"


namespace mica {

/** @class AnalyserTrackerAngularMomentum
 *         Anayser class which produces plots of ang. mom. vs radius
 *  @author A. Dobbs
 *  @var mAnalysisStation The tracker station to calculate all values at (default 1)
 *  @var mAnalysisPlane The tracker plane to calculate all values at (default 0)
 *  @var mHAngMomTKU Plot of ang. mom. vs radius for TkU
 *  @var mHAngMomTKD Plot of ang. mom. vs radius for TkD
 */

class AnalyserTrackerAngularMomentum : public AnalyserBase {
  public:
    AnalyserTrackerAngularMomentum();
    virtual ~AnalyserTrackerAngularMomentum() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

    int GetAnalysisStation() const { return mAnalysisStation; }
    void SetAnalysisStation(int aAnalysisStation) { mAnalysisStation = aAnalysisStation; }

    int GetAnalysisPlane() const { return mAnalysisPlane; }
    void SetAnalysisPlane(int aAnalysisPlane) { mAnalysisPlane = aAnalysisPlane; }

  private:
    int mAnalysisStation;
    int mAnalysisPlane;
    TH2D* mHAngMomTKU;
    TH2D* mHAngMomTKD;

    /** @brief Extract the seed associated with a SciFiTrack. Checks the validity of the pointer */
    MAUS::SciFiSeed* ExtractSeed(MAUS::SciFiTrack* aTrack) const;
};
} // ~namespace mica


#endif
