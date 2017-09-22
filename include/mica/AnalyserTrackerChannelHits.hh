/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef AnalyserTrackerChannelHits_hh
#define AnalyserTrackerChannelHits_hh

#include <vector>
#include <memory>

#include "TVirtualPad.h"
#include "TH1.h"

#include "mica/AnalyserBase.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"

namespace mica {

/** @class AnalyserTrackerChannelHits
 *         Anayser class which produces histos of tracker channel occupancy
 *  @author A. Dobbs
 */
class AnalyserTrackerChannelHits : public AnalyserBase {
  public:
    AnalyserTrackerChannelHits();
    virtual ~AnalyserTrackerChannelHits() {}

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);
    virtual void draw(TVirtualPad* aPad);

  private:
    std::vector<std::unique_ptr<TH1I> > mTkU; ///< Plots for TkU, order by station then by plane
    std::vector<std::unique_ptr<TH1I> > mTkD; ///< Plots for TkD, order by station then by plane
};
} // ~namespace mica

#endif
