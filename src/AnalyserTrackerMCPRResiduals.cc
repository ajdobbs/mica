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

#include "mica/AnalyserTrackerMCPRResiduals.hh"

#include <cmath>

#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"



namespace mica {

AnalyserTrackerMCPRResiduals::AnalyserTrackerMCPRResiduals() : mHTkUPositionResidualsX{nullptr},
                                                               mHTkUPositionResidualsY{nullptr},
                                                               mHTkUMomentumResidualsT{nullptr},
                                                               mHTkUMomentumResidualsZ{nullptr},
                                                               mHTkDPositionResidualsX{nullptr},
                                                               mHTkDPositionResidualsY{nullptr},
                                                               mHTkDMomentumResidualsT{nullptr},
                                                               mHTkDMomentumResidualsZ{nullptr}  {
  mHTkUPositionResidualsX = new TH1D("hTkUPositionResidualsX", "TkU x Residuals", 100, -5, 5);
  mHTkUPositionResidualsY = new TH1D("hTkUPositionResidualsY", "TkU y Residuals", 100, -5, 5);
  mHTkUMomentumResidualsT = new TH1D("hTkUMomentumResidualsT", "TkU pt Residuals", 100, -10, 10);
  mHTkUMomentumResidualsZ = new TH1D("hTkUMomentumResidualsZ", "TkU pz Residuals", 100, -50, 200);
  mHTkDPositionResidualsX = new TH1D("hTkDPositionResidualsX", "TkD x Residuals", 100, -5, 5);
  mHTkDPositionResidualsY = new TH1D("hTkDPositionResidualsY", "TkD y Residuals", 100, -5, 5);
  mHTkDMomentumResidualsT = new TH1D("hTkDMomentumResidualsT", "TkD pt Residuals", 100, -10, 10);
  mHTkDMomentumResidualsZ = new TH1D("hTkDMomentumResidualsZ", "TkD pz Residuals", 100, -50, 200);
}

bool AnalyserTrackerMCPRResiduals::analyse_recon(MAUS::ReconEvent* const aReconEvent) {
  if (!aReconEvent) {
    clear_lookup();
    clear_mc_data();
    return false;
  }


  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt) {
    clear_lookup();
    clear_mc_data();
    return false;
  }

  // Loop over all the scifi tracks in this event
  int nTkURecTracks = 0;
  int nTkDRecTracks = 0;
  MAUS::SciFiHelicalPRTrack* tku_trk = nullptr;
  MAUS::SciFiHelicalPRTrack* tkd_trk = nullptr;
  for (auto trk : sfevt->helicalprtracks()) {
    if (trk->get_tracker() == 0) {
      ++nTkURecTracks;
      tku_trk = trk;
    } else if (trk->get_tracker() == 1) {
      ++nTkDRecTracks;
      tkd_trk = trk;
    }
  }

  // std::cout << "TkU Rec tracks: " << nTkURecTracks << ", TkD Rec tracks: " << nTkDRecTracks
  //           << "TkU MC tracks: " << GetMCDataTkU().size() << ", TkD MC tracks: "
  //           << GetMCDataTkD().size() << std::endl;

  // Require 1 recon and 1 MC track in each tracker
  if (nTkURecTracks != 1 || nTkDRecTracks != 1) {
    clear_lookup();
    clear_mc_data();
    return false;
  }
  if (GetMCDataTkU().size() != 1 || GetMCDataTkD().size() != 1) {
    clear_lookup();
    clear_mc_data();
    return false;
  }

  if (tku_trk->get_spacepoints_pointers().size() != 5 ||
      tkd_trk->get_spacepoints_pointers().size() != 5) {
    clear_lookup();
    clear_mc_data();
    return false;
  }

  // At this point we should an event that is good to analyse
  // Calculate the recon postion at the reference station, and the recon momentum (average)
  double tku_x = 0.0;
  double tku_y = 0.0;
  for (auto sp : tku_trk->get_spacepoints_pointers()) {
    if (sp->get_station() == 1) {
      tku_x = sp->get_position().x();
      tku_y = sp->get_position().y();
    }
  }

  double tkd_x = 0.0;
  double tkd_y = 0.0;
  for (auto sp : tkd_trk->get_spacepoints_pointers()) {
    if (sp->get_station() == 1) {
      tkd_x = sp->get_position().x();
      tkd_y = sp->get_position().y();
    }
  }

  std::cerr << tku_x << " " << GetMCDataTkU()[0]->pos.x() << std::endl;
  double tku_dx = tku_x + GetMCDataTkU()[0]->pos.x();
  double tku_dy = tku_y - GetMCDataTkU()[0]->pos.y();

  double tku_ptmc = sqrt(GetMCDataTkU()[0]->mom.x()*GetMCDataTkU()[0]->mom.x() +
                         GetMCDataTkU()[0]->mom.y()*GetMCDataTkU()[0]->mom.y());
  double tku_ptrec = 0.3*mBfield*tku_trk->get_R(); // 0.3 comes from mom being in MeV and rad in mm

  double tku_dpt = tku_ptrec - tku_ptmc;
  double tku_dpz = (tku_ptrec / tku_trk->get_dsdz()) + GetMCDataTkU()[0]->mom.z();

  double tkd_dx = tkd_x - GetMCDataTkD()[0]->pos.x();
  double tkd_dy = tkd_y - GetMCDataTkD()[0]->pos.y();

  double tkd_ptmc = sqrt(GetMCDataTkD()[0]->mom.x()*GetMCDataTkD()[0]->mom.x() +
                         GetMCDataTkD()[0]->mom.y()*GetMCDataTkD()[0]->mom.y());
  double tkd_ptrec = 0.3*mBfield*tkd_trk->get_R(); // 0.3 comes from mom being in MeV and rad in mm

  double tkd_dpt = tkd_ptrec - tkd_ptmc;
  double tkd_dpz = (tkd_ptrec / tkd_trk->get_dsdz()) + GetMCDataTkD()[0]->mom.z();

  // Fill the histograms
  mHTkUPositionResidualsX->Fill(tku_dx);
  mHTkUPositionResidualsY->Fill(tku_dy);
  mHTkUMomentumResidualsT->Fill(tku_dpt);
  mHTkUMomentumResidualsZ->Fill(tku_dpz);

  mHTkDPositionResidualsX->Fill(tkd_dx);
  mHTkDPositionResidualsY->Fill(tkd_dy);
  mHTkDMomentumResidualsT->Fill(tkd_dpt);
  mHTkDMomentumResidualsZ->Fill(tkd_dpz);

  clear_lookup();
  clear_mc_data();
  return true;
}

void AnalyserTrackerMCPRResiduals::draw(TVirtualPad* aPad) {
  aPad->cd();
  aPad->Divide(4, 2);
  aPad->cd(1);
  mHTkUPositionResidualsX->Draw();
  aPad->cd(2);
  mHTkUPositionResidualsY->Draw();
  aPad->cd(3);
  mHTkUMomentumResidualsT->Draw();
  aPad->cd(4);
  mHTkUMomentumResidualsZ->Draw();
  aPad->cd(5);
  mHTkDPositionResidualsX->Draw();
  aPad->cd(6);
  mHTkDPositionResidualsY->Draw();
  aPad->cd(7);
  mHTkDMomentumResidualsT->Draw();
  aPad->cd(8);
  mHTkDMomentumResidualsZ->Draw();
}
} // ~namespace mica
