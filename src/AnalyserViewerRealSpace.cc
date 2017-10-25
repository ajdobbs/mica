/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include "mica/AnalyserViewerRealSpace.hh"

#include <algorithm>
#include <cmath>
#include <iomanip>

#include "TRef.h"
#include "TAxis.h"

#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/TOFEventSpacePoint.hh"
#include "src/common_cpp/DataStructure/TOFSpacePoint.hh"
#include "src/common_cpp/DataStructure/SciFiTrack.hh"
#include "src/common_cpp/DataStructure/SciFiTrackPoint.hh"

namespace mica {

AnalyserViewerRealSpace::AnalyserViewerRealSpace() {
  // Do nothing
}

bool AnalyserViewerRealSpace::analyse(MAUS::ReconEvent* const aReconEvent,
                                      MAUS::MCEvent* const aMCEvent) {
  // Populate position vectors from event spacepoints
  if (!aReconEvent) return false;
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt) return false;
  clear_vectors();

  for (auto sp : sfevt->spacepoints()) {
    if (sp->get_tracker() == 0) {
      mXTkU.push_back(sp->get_position().x());
      mYTkU.push_back(sp->get_position().y());
      mZTkU.push_back(sp->get_position().z());
    } else if (sp->get_tracker() == 1) {
      mXTkD.push_back(sp->get_position().x());
      mYTkD.push_back(sp->get_position().y());
      mZTkD.push_back(sp->get_position().z());
    }
  }

  // Populate track fit function vectors
  for (auto trk : sfevt->helicalprtracks()) {
    double x0 = trk->get_reference_position().x();
    double y0 = trk->get_reference_position().y();
    double xc = trk->get_circle_x0();
    double yc = trk->get_circle_y0();
    double rad = trk->get_R();
    double dsdz = trk->get_dsdz();
    double sz_c = trk->get_line_sz_c();

    if (trk->get_tracker() == 0) {
      mHtrkXYTkU.push_back(make_helix_xy(xc, yc, rad));
      mHtrkZXTkU.push_back(make_helix_xz(-1, xc, rad, dsdz, sz_c, mZMin, mZMax));
      mHtrkZYTkU.push_back(make_helix_yz(xc, yc, rad, dsdz, -x0, y0, mZMin, mZMax));
    } else if (trk->get_tracker() == 1) {
      mHtrkXYTkD.push_back(make_helix_xy(xc, yc, rad));
      mHtrkZXTkD.push_back(make_helix_xz(-1, xc, rad, dsdz, sz_c, mZMin, mZMax));
      mHtrkZYTkD.push_back(make_helix_yz(xc, yc, rad, dsdz, x0, y0, mZMin, mZMax));
    }
    print_track_info(trk);
  }

  return true;
}

bool AnalyserViewerRealSpace::draw(std::shared_ptr<TVirtualPad> aPad) {
  GetPads()[0]->Divide(3, 2);
  update();
  return true;
}

void AnalyserViewerRealSpace::update() {
  mGrXYTkU = std::unique_ptr<TGraph>(new TGraph(mXTkU.size(), &(mXTkU[0]), &(mYTkU[0])));
  mGrZXTkU = std::unique_ptr<TGraph>(new TGraph(mZTkU.size(), &(mZTkU[0]), &(mXTkU[0])));
  mGrZYTkU = std::unique_ptr<TGraph>(new TGraph(mZTkU.size(), &(mZTkU[0]), &(mYTkU[0])));
  mGrXYTkD = std::unique_ptr<TGraph>(new TGraph(mXTkD.size(), &(mXTkD[0]), &(mYTkD[0])));
  mGrZXTkD = std::unique_ptr<TGraph>(new TGraph(mZTkD.size(), &(mZTkD[0]), &(mXTkD[0])));
  mGrZYTkD = std::unique_ptr<TGraph>(new TGraph(mZTkD.size(), &(mZTkD[0]), &(mYTkD[0])));

  GetPads()[0]->cd(1);
  mGrXYTkU->SetTitle("X-Y Projection TkU");
  mGrXYTkU->GetXaxis()->SetTitle("x(mm)");
  mGrXYTkU->GetYaxis()->SetTitle("y(mm)");
  mGrXYTkU->SetMarkerStyle(20);
  mGrXYTkU->SetMarkerColor(kBlack);
  mGrXYTkU->Draw("AP same");
  for (auto& fit : mHtrkXYTkU) {
    fit.Draw("same");
  }

  GetPads()[0]->cd(2);
  mGrZXTkU->SetTitle("Z-X Projection TkU");
  mGrZXTkU->GetXaxis()->SetTitle("z(mm)");
  mGrZXTkU->GetYaxis()->SetTitle("x(mm)");
  mGrZXTkU->SetMarkerStyle(20);
  mGrZXTkU->SetMarkerColor(kBlack);
  mGrZXTkU->Draw("AP same");
  for (auto& fit : mHtrkZXTkU) {
    fit.Draw("same");
  }

  GetPads()[0]->cd(3);
  mGrZYTkU->SetTitle("Z-Y Projection TkU");
  mGrZYTkU->GetXaxis()->SetTitle("z(mm)");
  mGrZYTkU->GetYaxis()->SetTitle("y(mm)");
  mGrZYTkU->SetMarkerStyle(20);
  mGrZYTkU->SetMarkerColor(kBlack);
  mGrZYTkU->Draw("AP same");
  for (auto& fit : mHtrkZYTkU) {
    fit.Draw("same");
  }

  GetPads()[0]->cd(4);
  mGrXYTkD->SetTitle("X-Y Projection TkD");
  mGrXYTkD->GetXaxis()->SetTitle("x(mm)");
  mGrXYTkD->GetYaxis()->SetTitle("y(mm)");
  mGrXYTkD->SetMarkerStyle(20);
  mGrXYTkD->SetMarkerColor(kBlack);
  mGrXYTkD->Draw("AP same");
  for (auto& fit : mHtrkXYTkD) {
    fit.Draw("same");
  }

  GetPads()[0]->cd(5);
  mGrZXTkD->SetTitle("Z-X Projection TkD");
  mGrZXTkD->GetXaxis()->SetTitle("z(mm)");
  mGrZXTkD->GetYaxis()->SetTitle("x(mm)");
  mGrZXTkD->SetMarkerStyle(20);
  mGrZXTkD->SetMarkerColor(kBlack);
  mGrZXTkD->Draw("AP same");
  for (auto& fit : mHtrkZXTkD) {
    fit.Draw("same");
  }

  GetPads()[0]->cd(6);
  mGrZYTkD->SetTitle("Z-Y Projection TkD");
  mGrZYTkD->GetXaxis()->SetTitle("z(mm)");
  mGrZYTkD->GetYaxis()->SetTitle("y(mm)");
  mGrZYTkD->SetMarkerStyle(20);
  mGrZYTkD->SetMarkerColor(kBlack);
  mGrZYTkD->Draw("AP same");
  for (auto& fit : mHtrkZYTkD) {
    fit.Draw("same");
  }
}

void AnalyserViewerRealSpace::clear_vectors() {
  mXTkU.resize(0);
  mYTkU.resize(0);
  mZTkU.resize(0);
  mXTkD.resize(0);
  mYTkD.resize(0);
  mZTkD.resize(0);
  mHtrkXYTkU.resize(0);
  mHtrkZXTkU.resize(0);
  mHtrkZYTkU.resize(0);
  mHtrkXYTkD.resize(0);
  mHtrkZXTkD.resize(0);
  mHtrkZYTkD.resize(0);
}

TArc AnalyserViewerRealSpace::make_helix_xy(double x0, double y0, double rad) {
  TArc arc = TArc(x0, y0, rad);
  arc.SetFillStyle(0); // 0 - Transparent
  arc.SetLineColor(kBlue);
  return arc;
};

TF1 AnalyserViewerRealSpace::make_str_track(double c, double m, double zmin, double zmax) {
  // Note: in the function expression, x is just the independent variable, which
  // in this case is the z coordinate in the tracker coordinate system
  TF1 trk = TF1("trk", "[0]+([1]*x)", zmin, zmax);
  trk.SetParameters(c, m);
  trk.SetLineColor(kRed);
  return trk;
};

TF1 AnalyserViewerRealSpace::make_helix_xz(int handness, double circle_x0, double rad, double dsdz,
                                           double sz_c, double zmin, double zmax) {
    // The x in the cos term is actually representing z (the indep variable)
    TF1 func = TF1("xz_func", "[0]-[4]*([1]*cos((1/[1])*([2]*x+[3])))", zmin, zmax);
    func.SetParameter(0, circle_x0);
    func.SetParameter(1, rad);
    func.SetParameter(2, dsdz);
    func.SetParameter(3, sz_c);
    func.SetParameter(4, handness);
    func.SetLineColor(kBlue);
    return func;
};



TF1 AnalyserViewerRealSpace::make_helix_yz(double circle_x0, double circle_y0, double rad, double dsdz,
                                           double x0, double y0, double zmin, double zmax) {
    // The x in the cos term is actually representing z (the indep variable)
    // TF1 func = TF1("yz_func", "[0]+([1]*sin((1/[1])*([2]*x+[3])))", zmin, zmax);
    TF1 func = TF1("yz_func", "[1] + ([5]-[1])*cos((1/[2])*([3]*x)) + ([4]-[0])*sin((1/[2])*[3]*x)", zmin, zmax);
    func.SetParameter(0, circle_x0);
    func.SetParameter(1, circle_y0);
    func.SetParameter(2, rad);
    func.SetParameter(3, dsdz);
    func.SetParameter(4, x0);
    func.SetParameter(5, y0);
    func.SetLineColor(kBlue);
    return func;
};

void AnalyserViewerRealSpace::print_track_info(const MAUS::SciFiHelicalPRTrack* const trk) {
  double rad = trk->get_R();
  double pt = rad * mRadToPt;
  double pz = pt / trk->get_dsdz();
  std::cout << "Tracker " << trk->get_tracker() << + ", ";
  std::cout << "Num points " << trk->get_num_points() << ", ";
  std::cout << "Charge " << trk->get_charge() << ", ";
  std::cout << "R = " << std::setprecision(4) << trk->get_R() << "mm, ";
  std::cout << "X0 = " << std::setprecision(4) << trk->get_circle_x0() << "mm, ";
  std::cout << "Y0 = " <<  std::setprecision(4) << trk->get_circle_y0() << "mm,\n";
  std::cout << "dsdz " <<  std::setprecision(4) << trk->get_dsdz() << ", ";
  std::cout << "pt = " <<  std::setprecision(4) << pt << "MeV/c, ";
  std::cout << "pz = " <<  std::setprecision(4) << pz << "MeV/c, ";
  std::cout << "xy_chi2 = " << std::setprecision(4) << trk->get_circle_chisq() << ", ";
  std::cout << "sz_c = " << std::setprecision(4) << trk->get_line_sz_c() << ", ";
  std::cout << "sz_chi2 = " << std::setprecision(4) << trk->get_line_sz_chisq() << "\n";
};

} // ~namespace mica
