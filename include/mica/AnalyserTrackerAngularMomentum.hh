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
 */
class AnalyserTrackerAngularMomentum : public AnalyserBase {
  public:
    AnalyserTrackerAngularMomentum();
    virtual ~AnalyserTrackerAngularMomentum() {}

    /** @brief Analyse the given event
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief After analysing all the events, draw the results
     *  @param aPad ROOT TPad to draw results on
     */
    virtual bool draw(std::shared_ptr<TVirtualPad> aPad);

    /** @brief Return the tracker station at which parameters are evaluated */
    int GetAnalysisStation() const { return mAnalysisStation; }

    /** @brief Set the tracker station at which parameters are evaluated */
    void SetAnalysisStation(int aAnalysisStation) { mAnalysisStation = aAnalysisStation; }

    /** @brief Get the tracker plane at which parameters are evaluated */
    int GetAnalysisPlane() const { return mAnalysisPlane; }

    /** @brief Set the tracker plane at which parameters are evaluated */
    void SetAnalysisPlane(int aAnalysisPlane) { mAnalysisPlane = aAnalysisPlane; }

  private:
    int mAnalysisStation; ///< The tracker station to calculate all values at (default 1)
    int mAnalysisPlane; ///< The tracker plane to calculate all values at (default 0)
    TH2D* mHAngMomTKU; ///< Plot of ang. mom. vs radius for TkU
    TH2D* mHAngMomTKD; ///< Plot of ang. mom. vs radius for TkD

    /** @brief Extract the seed associated with a SciFiTrack. Checks the validity of the pointer.
     *  @param aTrack Pointer to the track
     *  @return The seed corresponding to the input track
     */
    MAUS::SciFiSeed* ExtractSeed(MAUS::SciFiTrack* aTrack) const;
};
} // ~namespace mica

#endif
