/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERBASE_HH
#define ANALYSERBASE_HH

#include <vector>
#include <memory>

#include "TVirtualPad.h"
#include "TStyle.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "mica/CutsBase.hh"

namespace mica {

/** @class AnalyserBase
 *    Base class for all analysers. Defines an interface via the draw and analyse
 *    functions, which must be overidden in daughter classes. Client code calls
 *    Draw and Analyse. Applies any cuts selected prior to passing events
 *    to daughter routines.
 *  @author A. Dobbs
 */
class AnalyserBase {
  public:
    AnalyserBase();
    virtual ~AnalyserBase();

    /** @brief Check the cuts, then if they are passed calls the daughter class analyse method
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    bool Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief Analyse the given event, to be overidden by concrete daughter classes
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) = 0;

    /** @brief Check to see if a valid pad has been supplied, if not make a new one,
     *         then call draw
     *  @param aPad ROOT TPad to draw results on
     */
    std::shared_ptr<TVirtualPad> Draw(std::shared_ptr<TVirtualPad> aPad=nullptr);

    /** @brief After analysing all the events, draw the results,
     *         to be overidden by concrete daughter classes
     *  @param aPad ROOT TPad to draw results on
     */
    virtual void draw(std::shared_ptr<TVirtualPad> aPad) = 0;

    /** @brief Add a pad to the list of internal pad pointers */
    void AddPad(std::shared_ptr<TVirtualPad> aPad) { mPads.push_back(aPad); }

    /** @brief Return all the pads */
    std::vector<std::shared_ptr<TVirtualPad>> const GetPads() { return mPads; }

    /** @brief Add a cut, only events which pass the cut will be processed
     *  @param aCut The cut to add
     */
    void AddCut(CutsBase* aCut) { mCuts.push_back(aCut); }

    /** @brief Return the cuts enter for the analyser */
    std::vector<CutsBase*> GetCuts() { return mCuts; }

    /** @brief Set the cuts, only events which pass all the cuts will be processed */
    void SetCuts(std::vector<CutsBase*>& aCuts) { mCuts = aCuts; }

    /** @brief Return a pointer to the ROOT TStyle used
      * to set the plotting style for this analyser
      * @return pointer to the ROOT TStyle for this analyser
      */
    std::shared_ptr<TStyle> GetStyle() { return mStyle; }

  private:
    /** @brief Apply the cuts held by the mCuts members to the event given as arguments, if they
     *         pass return true, otherwise false - no cuts just causes return true.
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed
     */
    bool ApplyCuts(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    std::vector<std::shared_ptr<TVirtualPad>> mPads; ///< The canvas upon which the plots are drawn
    std::vector<CutsBase*> mCuts; ///< The cuts to apply before admitting an event for analysis
    std::shared_ptr<TStyle> mStyle; ///< The ROOT TStyle to be applied to the canvases
};
} // ~namespace mica


#endif
