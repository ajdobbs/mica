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

/** @class AnalyserTrackerAngularMomentum Anayser class which produces plots of ang. mom. vs radius
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
