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

#ifndef ANALYSERTRACKERPREFFICIENCY_HH
#define ANALYSERTRACKERPREFFICIENCY_HH

#include <iostream>
#include <fstream>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/Analysis/AnalyserBase.hh"

namespace MAUS {
namespace Analysis {

/** @class AnalyserTrackerPREfficiency Analyser class which produces tracker pattern recognition
 *                                     efficiency results, uses reconstrcuted data only (no MC)
 *  @author A. Dobbs
 *  @var mCheckTOF Should we check time-of-flight between TOF1 and TOF2. Requires 1 and only 1
 *       spacepoint in both TOF1 and TOF2, so if set to true it will override mCheckTOFSpacePoints.
 *  @var mCheckTOFSpacePoints Should we check there is 1 and only 1 spacepoint in both TOF1 and
 *       TOF2? mCheckTOF being set to true will override this flag, & force it to be a requirement.
 *  @var mAllowMultiHitStations Should we allow non-deal events to be considered, that is, events
 *       with more than one spacepoint per tracker station for a given tracker. Even when set to
 *       true we still require there not be enough spacepoints present to form two distinct tracks.
 *  @var mCheckTkU Should we check TkU criteria at all, or just pass the event as good? May want 
 *       to ignore TkU if we wanted to just check TkD for example.
 *  @var mCheckTkD Should we check TkD criteria at all, or just pass the event as good? May want 
 *       to ignore TkD if we wanted to just check TkU for example.
 *  @var mNEvents Counter, number of events analysed
 *  @var mTkUGoodEvents Counter, number of events where we expect a track in TkU
 *  @var mTkU5ptTracks Counter, number of 5pt tracks actually reconstructed in TkU
 *  @var mTkU4to5ptTracks Counter, number of 4 or 5pt tracks actually reconstructed in TkU
 *  @var mTkDGoodEvents Counter, number of events where we expect a track in TkD
 *  @var mTkD5ptTracks Counter, number of 5pt tracks actually reconstructed in TkD
 *  @var mTkD4to5ptTracks Counter, number of 4 or 5pt tracks actually reconstructed in TkD
 *  @var mLowerTimeCut Minimum time-of-flight between TOF1 and TOF2 for event to be classed as good,
 *       if mCheckTOF is set true
 *  @var mUpperTimeCut Maximum time-of-flight between TOF1 and TOF2 for event to be classed as good,
 *       if mCheckTOF is set true
 */

class AnalyserTrackerPREfficiency : public AnalyserBase {
  public:
    AnalyserTrackerPREfficiency();
    virtual ~AnalyserTrackerPREfficiency();

    virtual bool analyse(ReconEvent* const aReconEvent, MCEvent* const aMCEvent);

    virtual void clear();

    virtual void draw(TVirtualPad* aPad);

    bool GetCheckTOF() const { return mCheckTOF; }
    void SetCheckTOF(bool aBool) { mCheckTOF = aBool; }

    bool GetCheckTOFSpacePoints() const { return mCheckTOFSpacePoints; }
    void SetCheckTOFSpacePoints(bool aBool) { mCheckTOFSpacePoints = aBool; }

    bool GetAllowMultiHitStations() const { return mAllowMultiHitStations; }
    void SetAllowMultiHitStations(bool aBool) { mAllowMultiHitStations = aBool; }

    bool GetCheckTkU() const { return mCheckTkU; }
    void SetCheckTkU(bool aBool) { mCheckTkU = aBool; }

    bool GetCheckTkD() const { return mCheckTkD; }
    void SetCheckTkD(bool aBool) { mCheckTkD = aBool; }

  private:
    bool mCheckTOF;
    bool mCheckTOFSpacePoints;
    bool mAllowMultiHitStations;
    bool mCheckTkU;
    bool mCheckTkD;

    int mNEvents;

    int mTkUGoodEvents;
    int mTkU5ptTracks;
    int mTkU4to5ptTracks;

    int mTkDGoodEvents;
    int mTkD5ptTracks;
    int mTkD4to5ptTracks;

    double mLowerTimeCut;
    double mUpperTimeCut;

    ofstream mOf1;

    /** @brief Check a tracker to see if a single track is expected. Set the input bools to
     *         say whether a 4pt track or a 5pt track are expected.
     *  @param[in] evt SciFiEvent containing the spacepoints
     *  @param[in] trker_num Which tracker to analyse
     *  @param[out] good4pt Do we expect a 4pt track to be reconstructed, but NOT a 5pt track
     *  @param[out] good5pt Do we expect a 5pt track to be reconstructed
     */
    void check_good_tk_event(SciFiEvent* evt, int trker_num, bool& good4pt, bool& good5pt);

    /** @brief Check the TOFs to see if the event passes the choosen criteria for a good event
     *  @return Bool indicating if this is a good event
     */
    bool check_good_tof_event(TOFEvent* evt);
};
} // ~namespace Analysis
} // ~namespace MAUS

#endif
