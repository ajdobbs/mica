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

#ifndef ANALYSERBASE_HH
#define ANALYSERBASE_HH

#include <vector>

#include "TVirtualPad.h"
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
 *  @var mCuts The cuts to apply before admitting an event for analysis
 */

class AnalyserBase {
  public:
    AnalyserBase();
    virtual ~AnalyserBase() {}

    /** @brief Check the cuts, then if they are passed calls the daughter class analyse method
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed and the analysis happened
     */
    bool Analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    /** @brief Analyse the given event, to be overidden by concrete daughter classes */
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) = 0;

    /** @brief Check to see if a valid pad has been supplied, if not make a new one,
     *         then call draw
     *  @param aPad ROOT TPad to draw results on
     */
    TVirtualPad* Draw(TVirtualPad* aPad = NULL);

    /** @brief After analysing all the events, draw the results,
     *         to be overidden by concrete daughter classes
     *  @param aPad ROOT TPad to draw results on
     */
    virtual void draw(TVirtualPad* aPad) = 0;

    /** @brief Add a cut, only events which pass the cut will be processed
     *  @param aCut The cut to add
     */
    void AddCut(CutsBase* aCut) { mCuts.push_back(aCut); }

    /** @brief Return the cuts enter for the analyser */
    std::vector<CutsBase*> GetCuts() { return mCuts; }

    /** @brief Set the cuts, only events which pass all the cuts will be processed */
    void SetCuts(std::vector<CutsBase*>& aCuts) { mCuts = aCuts; }

  private:
    /** @brief Apply the cuts held by the mCuts members to the event given as arguments, if they
     *         pass return true, otherwise false - no cuts just causes return true.
     *  @param aReconEvent The recon event
     *  @param aMCEvent The corresponding MC event
     *  @return Boolean indicating if the cuts passed
     */
    bool ApplyCuts(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent);

    std::vector<CutsBase*> mCuts;
};
} // ~namespace mica


#endif
