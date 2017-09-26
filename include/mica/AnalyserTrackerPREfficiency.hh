/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
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
#include "mica/AnalyserBase.hh"

namespace mica {

/** @class AnalyserTrackerPREfficiency
 *         Analyser class which produces tracker pattern recognition
 *         efficiency results, uses reconstrcuted data only (no MC)
 *  @author A. Dobbs
 */
class AnalyserTrackerPREfficiency : public AnalyserBase {
  public:
    AnalyserTrackerPREfficiency();
    virtual ~AnalyserTrackerPREfficiency();

    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief Reset the internal counter variables to 0 */
    virtual void clear();

    virtual void draw(std::shared_ptr<TVirtualPad> aPad);

    /** @brief Return if we are checking time-of-flight between TOF1 and TOF2, overrules
      * CheckTOFSpacePoints
      */
    bool GetCheckTOF() const { return mCheckTOF; }

    /** @brief Set if we are checking time-of-flight between TOF1 and TOF2
     *  CheckTOFSpacePoints
     */
    void SetCheckTOF(bool aBool) { mCheckTOF = aBool; }

    /** @brief Return if we are checking TOF spacepoint, will be overruled if CheckTOF is true */
    bool GetCheckTOFSpacePoints() const { return mCheckTOFSpacePoints; }
    /** @brief Set if we are checking TOF spacepoint, will be overruled if CheckTOF is true */
    void SetCheckTOFSpacePoints(bool aBool) { mCheckTOFSpacePoints = aBool; }

    /** @brief Return if we allow non-ideal events to be considered */
    bool GetAllowMultiHitStations() const { return mAllowMultiHitStations; }
    /** @brief Set if we allow non-ideal events to be considered */
    void SetAllowMultiHitStations(bool aBool) { mAllowMultiHitStations = aBool; }

    /** Return if we are checking TkU criteria at all */
    bool GetCheckTkU() const { return mCheckTkU; }
    /** Set if we are checking TkU criteria at all */
    void SetCheckTkU(bool aBool) { mCheckTkU = aBool; }

    /** Return if we are checking TkD criteria at all */
    bool GetCheckTkD() const { return mCheckTkD; }
    /** Set if we are checking TkD criteria at all */
    void SetCheckTkD(bool aBool) { mCheckTkD = aBool; }

  private:
    bool mCheckTOF; ///< Should we check time-of-flight between TOF1 and TOF2. Requires 1 and only 1
                    ///< spacepoint in both TOF1 and TOF2, so if set to true it will override
                    ///< mCheckTOFSpacePoints.
    bool mCheckTOFSpacePoints; ///< Should we check there is 1 and only 1 spacepoint in both TOF1
                               ///< and TOF2? mCheckTOF being set to true will override this flag,
                               ///< and force it to be a requirement.
    bool mAllowMultiHitStations; ///< Should we allow non-ideal events to be considered, that is,
                                 ///< events with more than one spacepoint per tracker station for
                                 ///< a given tracker. Even when set to true we still require there
                                 ///< not be enough spacepoints present to form two distinct tracks.
    bool mCheckTkU; ///< Should we check TkU criteria at all, or just pass the event as good? May
                    ///< want to ignore TkU if we wanted to just check TkD for example.
    bool mCheckTkD; ///< Should we check TkD criteria at all, or just pass the event as good? May
                    ///< want to ignore TkD if we wanted to just check TkU for example.

    int mNEvents; ///< Counter, number of events analysed
    int mTkUGoodEvents; ///< Counter, number of events where we expect a track in TkU
    int mTkU5ptTracks; ///< Counter, number of 5pt tracks actually reconstructed in TkU
    int mTkU4to5ptTracks; ///< Counter, number of 4 or 5pt tracks actually reconstructed in TkU
    int mTkDGoodEvents; ///< Counter, number of events where we expect a track in TkD
    int mTkD5ptTracks; ///< Counter, number of 5pt tracks actually reconstructed in TkD
    int mTkD4to5ptTracks; ///< Counter, number of 4 or 5pt tracks actually reconstructed in TkD

    double mLowerTimeCut; ///< Minimum time-of-flight between TOF1 and TOF2 for event to be classed
                          ///< as good, if mCheckTOF is set true
    double mUpperTimeCut; ///< Maximum time-of-flight between TOF1 and TOF2 for event to be classed
                          ///< as good, if mCheckTOF is set true

    ofstream mOf1; ///< output filestream for efficiency data

    /** @brief Check a tracker to see if a single track is expected. Set the input bools to
     *         say whether a 4pt track or a 5pt track are expected.
     *  @param[in] evt SciFiEvent containing the spacepoints
     *  @param[in] trker_num Which tracker to analyse
     *  @param[out] good4pt Do we expect a 4pt track to be reconstructed, but NOT a 5pt track
     *  @param[out] good5pt Do we expect a 5pt track to be reconstructed
     */
    void check_good_tk_event(MAUS::SciFiEvent* evt, int trker_num, bool& good4pt, bool& good5pt);

    /** @brief Check the TOFs to see if the event passes the choosen criteria for a good event
     *  @return Bool indicating if this is a good event
     */
    bool check_good_tof_event(MAUS::TOFEvent* evt);
};
} // ~namespace mica


#endif
