/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERMC_HH
#define ANALYSERTRACKERMC_HH

#include <vector>
#include <map>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/AnalyserBase.hh"
#include "src/common_cpp/Recon/SciFi/SciFiLookup.hh"

namespace mica {

/** @struct MCTrackData
 *          Simple data container to hold tracker track info deduced from MC
 *  @author A. Dobbs
 */
struct MCTrackData {
  int tracker; ///< The tracker number, 0 = TkU, 1 = TkD
  int track_id; ///< The track id number
  int pid; ///< The PGD particle id
  double energy; ///< The track energy at the analysis plane
  MAUS::ThreeVector pos; ///< The track position at the analysis plane
  MAUS::ThreeVector mom; ///< The track momentum at the analysis plane
  std::vector<int> stations_hit; ///< The tracker stations the track created hits in
};

/** @class AnalyserTrackerMC
 *         Analyser class which calculates tracker data using MC info.
 *         Daughter classes then use that data for actual analysis. The data produced is held in
 *         MCTrackData members for TkU and TkD separately.
 *  @author A. Dobbs
 */
class AnalyserTrackerMC : public AnalyserBase {
  public:
    AnalyserTrackerMC();
    ~AnalyserTrackerMC();

    /** @brief Delete the lookup table and set the member pointer to nullptr */
    virtual void clear_lookup();

    /** @brief Cycle through data vectors, delete every object, set vectors to zero size */
    virtual void clear_mc_data();

    /** @brief Return the scifi MC lookup table */
    MAUS::SciFiLookup* GetLookup() const { return mLookup; }

    /** @brief Return the calculated mc data for TkU */
    std::vector<MCTrackData*> GetMCDataTkU() const { return mMCDataTkU; }

    /** @brief Return the calculated mc data for TkD */
    std::vector<MCTrackData*> GetMCDataTkD() const { return mMCDataTkD; }

    /** Create a map from mc track ids to vectors containing the station numbers for which
     *  that track generated hits in mNPlanes or more planes (usually = 3) i.e. a spacepoint
     *  from the track is expected in each of the stations listed in the vector.
     *  To be used with one tracker at a time.
     *  @param[in] hit_map A map from mc track ids to the the SciFiHits produced by that tracker
     *             for a single tracker.
     *  @param[in] aNPlanes The number planes per station which need to be hit for the station
     *             to count as one hit by the track
     *  @return A map from mc track ids to vectors containing the station numbers for which
     *          that track generated hits in mNPlanes or more planes, for a single tracker
     */
    std::map<int, std::vector<int> > calc_stations_hit_by_track( \
        std::map<int, std::vector<MAUS::SciFiHit*> >& hit_map, int aNPlanes);

  private:
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) override;

    /** @brief Analyse MC data, calls fill_mc_track_data and make_lookup
     *  @param[in] aMCEvent MCEvent to analyse
     *  @return Did the analysis succeed
     */
    virtual bool analyse_mc(MAUS::MCEvent* const aMCEvent);

    /** @brief Analyse the recon data, to be implemented in daughter classes
     *  @param[in] aReconEvent MAUS::ReconEvent to analyse, corresponding to aMCEvent
     *  @return Did the analysis succeed
     */
    virtual bool analyse_recon(MAUS::ReconEvent* const aReconEvent) = 0;

    int mRefStation; ///< Reference surface to use
    int mRefPlane;   ///< Reference plane to use
    int mNStations;  ///< # of stations hit for event to be classed as reconstructible
    int mNPlanes;    ///< # of planes hit per station hit for event to be classed as reconstructible
    std::vector<MCTrackData*> mMCDataTkU; ///< MC data for TkU, AnalyserTrackerMC owns the memory
    std::vector<MCTrackData*> mMCDataTkD; ///< MC data for TkD, AnalyserTrackerMC owns the memory
    MAUS::SciFiLookup* mLookup;           ///< The Lookup table, AnalyserTrackerMC owns the memory

    /** @brief Populate the mMCData members
     *  @param[in] aMCEvent MAUS::MCEvent to analyse
     *  @return Did the data members get populated successfully
     */
    bool fill_mc_track_data(MAUS::MCEvent* const aMCEvent);

    /** @brief Populate the lookup table member
     *  @param[in] aMCEvent MCEvent to analyse
     *  @return Was the lookup successfully created
     */
    bool make_lookup(MAUS::MCEvent* const aMCEvent);
};
} // ~namespace mica


#endif
