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

#ifndef ANALYSERTRACKERPRSEEDNPERESIDUAL_HH
#define ANALYSERTRACKERPRSEEDNPERESIDUAL_HH

#include <vector>

#include "TVirtualPad.h"
#include "TH2.h"

#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/Analysis/AnalyserBase.hh"

namespace MAUS {
namespace Analysis {

class AnalyserTrackerPRSeedNPEResidual : public AnalyserBase {
  public:
    AnalyserTrackerPRSeedNPEResidual();
    ~AnalyserTrackerPRSeedNPEResidual() {}

    virtual bool analyse(ReconEvent* const aReconEvent, MCEvent* const aMCEvent);

    virtual void draw(TVirtualPad* aPad);

  private:
    std::vector<TH2D*> mHResidualsTkU;
    std::vector<TH2D*> mHResidualsTkD;
};
} // ~namespace Analysis
} // ~namespace MAUS

#endif
