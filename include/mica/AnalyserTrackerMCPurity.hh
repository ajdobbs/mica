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

/** @class AnalyserTrackerMCPurity Analyser class which calculates patrec purity
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
