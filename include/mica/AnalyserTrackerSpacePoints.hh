/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERSPACEPOINTS_HH
#define ANALYSERTRACKERSPACEPOINTS_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"

namespace mica {

/** @class AnalyserTrackerSpacePoints
 *         Anayser class for producing spacepoint level plots for the trackers
 *  @author A. Dobbs
 */
class AnalyserTrackerSpacePoints : public AnalyserBase {
  public:
    AnalyserTrackerSpacePoints();
    virtual ~AnalyserTrackerSpacePoints() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    TH1D* mHNpeTKU; ///< Spacepoint NPE plot for TkU
    TH1D* mHNpeTKD; ///< Spacepoint NPE plot for TkD
    TH1D* mHStationNumTKU; ///< spacepoints per station plot for TkU
    TH1D* mHStationNumTKD; ///< spacepoints per station plot for TkD
    TH2D* mHXYTKU; ///< xy plot for TkU (over all stations)
    TH2D* mHXYTKD; ///< xy plot for TkD (over all stations)
    std::vector<std::unique_ptr<TH2D> > mXYPerStationTkU; ///< xy plots for TkU, order by station
    std::vector<std::unique_ptr<TH2D> > mXYPerStationTkD; ///< xy plots for TkD, order by station
    std::vector<std::unique_ptr<TH2D> > mXYPerStationTripletsTkU; ///< xy plots for TkU triplets
    std::vector<std::unique_ptr<TH2D> > mXYPerStationTripletsTkD; ///< xy plots for TkD triplets
    std::vector<std::unique_ptr<TH2D> > mXYPerStationDoubletsTkU; ///< xy plots for TkU triplets
    std::vector<std::unique_ptr<TH2D> > mXYPerStationDoubletsTkD; ///< xy plots for TkD triplets
};
} // ~namespace mica

#endif
