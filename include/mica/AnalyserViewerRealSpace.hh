/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERVIEWERREALSPACE_HH
#define ANALYSERVIEWERREALSPACE_HH

#include <vector>
#include <memory>

#include "TVirtualPad.h"
#include "TGraph.h"
#include "TF1.h"
#include "TArc.h"

#include "mica/AnalyserBase.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"

namespace mica {

/** @class AnalyserViewerRealSpace
 *         Anayser class which produces event viewer plots of real space
 *  @author A. Dobbs
 */
class AnalyserViewerRealSpace : public AnalyserBase {
  public:
    AnalyserViewerRealSpace();
    virtual ~AnalyserViewerRealSpace() {}

  private:
    virtual bool analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) override;
    virtual bool draw(std::shared_ptr<TVirtualPad> aPad) override;
    virtual void update() override;

    void clear_vectors();

    TArc make_helix_xy(double x0, double y0, double rad);
    TF1  make_str_track(double c, double m, double zmin, double zmax);
    TF1  make_helix_xz(int handness, double circle_x0, double rad, double dsdz, double sz_c,
                       double zmin, double zmax);
    TF1  make_helix_yz(double circle_x0, double circle_y0, double rad, double dsdz,
                       double x0, double y0, double zmin, double zmax);

    void print_track_info(const MAUS::SciFiHelicalPRTrack* const trk);

    const double mRadToPt = 0.9; //< Assumes 3T field, then bfield * 0.3
    const double mZMin = 0.0;
    const double mZMax = 1200.0;

    std::vector<double> mXTkU;
    std::vector<double> mYTkU;
    std::vector<double> mZTkU;
    std::vector<double> mXTkD;
    std::vector<double> mYTkD;
    std::vector<double> mZTkD;

    std::vector<TArc> mHtrkXYTkU;
    std::vector<TF1>  mHtrkZXTkU;
    std::vector<TF1>  mHtrkZYTkU;
    std::vector<TArc> mHtrkXYTkD;
    std::vector<TF1>  mHtrkZXTkD;
    std::vector<TF1>  mHtrkZYTkD;

    std::unique_ptr<TGraph> mGrXYTkU;
    std::unique_ptr<TGraph> mGrXYTkD;
    std::unique_ptr<TGraph> mGrZXTkU;
    std::unique_ptr<TGraph> mGrZXTkD;
    std::unique_ptr<TGraph> mGrZYTkU;
    std::unique_ptr<TGraph> mGrZYTkD;
};
} // ~namespace mica

#endif
