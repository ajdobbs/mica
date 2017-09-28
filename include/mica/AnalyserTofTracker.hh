/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERTOFTRACKER_HH
#define ANALYSERTOFTRACKER_HH

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

/** @class AnalyserTofTracker
 *         Anayser class which produces plots of tof12 time vs tracker momentum
 *  @author A. Dobbs
 */
class AnalyserTofTracker : public AnalyserBase {
  public:
    AnalyserTofTracker();
    virtual ~AnalyserTofTracker() {}

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
    std::unique_ptr<TH2D> mHPTkU; ///< Plot of tof12 time vs tku total momentum
    std::unique_ptr<TH2D> mHPTkD; ///< Plot of tof12 time vs tkd total momentum
    std::unique_ptr<TH2D> mHPtTkU; ///< Plot of tof12 time vs tku pt
    std::unique_ptr<TH2D> mHPtTkD; ///< Plot of tof12 time vs tkd pt
    std::unique_ptr<TH2D> mHPzTkU; ///< Plot of tof12 time vs tku pz
    std::unique_ptr<TH2D> mHPzTkD; ///< Plot of tof12 time vs tkd pz

    /** @brief Extract the momentum at the specified surface
     *  @param[in] trk The SciFiTrack
     *  @param[out] mom The momentum
     *  @return Bool representing success of fail
     */
     bool GetMomentum(const MAUS::SciFiTrack* const trk, MAUS::ThreeVector& mom);

};
} // ~namespace mica

#endif
