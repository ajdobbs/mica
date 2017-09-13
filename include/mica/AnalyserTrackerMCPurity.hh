/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERMCPURITY_HH
#define ANALYSERTRACKERMCPURITY_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH1.h"

#include "mica/AnalyserTrackerMC.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSeed.hh"
#include "src/common_cpp/DataStructure/SciFiTrack.hh"
#include "src/common_cpp/DataStructure/SciFiBasePRTrack.hh"


namespace mica {

/** @class AnalyserTrackerMCPurity
 *         Analyser class which calculates pattern recognition purity.
 *  @author A. Dobbs
 */

class AnalyserTrackerMCPurity : public AnalyserTrackerMC {
  public:
    AnalyserTrackerMCPurity();

    virtual ~AnalyserTrackerMCPurity() {}

    virtual bool analyse_recon(MAUS::ReconEvent* const aReconEvent) override;

    virtual void draw(TVirtualPad* aPad) override;

  private:
    int find_mc_track_id(MAUS::SciFiBasePRTrack* trk);
    TH1I* mHTracksMatched;
};
} // ~namespace mica


#endif
