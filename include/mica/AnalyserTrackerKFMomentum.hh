/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTRACKERKFMOMENTUM_HH
#define ANALYSERTRACKERKFMOMENTUM_HH

#include <vector>
#include <memory>

#include "TVirtualPad.h"
#include "TH2.h"

#include "mica/AnalyserBase.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/SciFiSeed.hh"
#include "src/common_cpp/DataStructure/SciFiTrack.hh"

namespace mica {

/** @class AnalyserTrackerKFMomentum
 *         Anayser class which produces plots of the kalman fit momentum
 *  @author A. Dobbs
 */
class AnalyserTrackerKFMomentum : public AnalyserBase {
  public:
    AnalyserTrackerKFMomentum();
    virtual ~AnalyserTrackerKFMomentum() {}

    /** @brief Analyse the given event
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief After analysing all the events, draw the results
     *  @param aPad ROOT TPad to draw results on
     */
    virtual void draw(std::shared_ptr<TVirtualPad> aPad);

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
    std::unique_ptr<TH2D> mHPUSDS; ///< Plot tku vs tkd p

    /** @brief Extract the momentum at the specified surface
     *  @param[in] trk The SciFiTrack
     *  @param[out] mom The momentum
     *  @return Bool representing success of fail
     */
     bool GetMomentum(const MAUS::SciFiTrack* const trk, MAUS::ThreeVector& mom);
};
} // ~namespace mica

#endif
