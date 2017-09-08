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

#include "TCanvas.h"
#include "TStyle.h"

#include "src/common_cpp/DataStructure/Hit.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"



namespace mica {

AnalyserTrackerMCPRResiduals::AnalyserTrackerMCPRResiduals() : mHTkUMCPositionX{nullptr},
                                                               mHTkUMCPositionY{nullptr},
                                                               mHTkUMCMomentumT{nullptr},
                                                               mHTkUMCMomentumZ{nullptr},
                                                               mHTkURecPositionX{nullptr},
                                                               mHTkURecPositionY{nullptr},
                                                               mHTkURecMomentumT{nullptr},
                                                               mHTkURecMomentumZ{nullptr},
                                                               mHTkUPositionResidualsX{nullptr},
                                                               mHTkUPositionResidualsY{nullptr},
                                                               mHTkUMomentumResidualsT{nullptr},
                                                               mHTkUMomentumResidualsZ{nullptr},
                                                               mHTkDMCPositionX{nullptr},
                                                               mHTkDMCPositionY{nullptr},
                                                               mHTkDMCMomentumT{nullptr},
                                                               mHTkDMCMomentumZ{nullptr},
                                                               mHTkDRecPositionX{nullptr},
                                                               mHTkDRecPositionY{nullptr},
                                                               mHTkDRecMomentumT{nullptr},
                                                               mHTkDRecMomentumZ{nullptr},
                                                               mHTkDPositionResidualsX{nullptr},
                                                               mHTkDPositionResidualsY{nullptr},
                                                               mHTkDMomentumResidualsT{nullptr},
                                                               mHTkDMomentumResidualsZ{nullptr}  {
  mHTkUMCPositionX = new TH1D("hTkUMCPositionX", "TkU MC x ", 100, -170, 170);
  mHTkUMCPositionY = new TH1D("hTkUMCPositionY", "TkU MC y ", 100, -170, 170);
  mHTkUMCMomentumT = new TH1D("hTkUMCMomentumT", "TkU MC pt ", 100, 0, 100);
  mHTkUMCMomentumZ = new TH1D("hTkUMCMomentumZ", "TkU MC pz ", 100, 0, 300);
  mHTkURecPositionX = new TH1D("hTkURecPositionX", "TkU Recon x ", 100, -170, 170);
  mHTkURecPositionY = new TH1D("hTkURecPositionY", "TkU Recon y ", 100, -170, 170);
  mHTkURecMomentumT = new TH1D("hTkURecMomentumT", "TkU Recon pt ", 100, 0, 100);
  mHTkURecMomentumZ = new TH1D("hTkURecMomentumZ", "TkU Recon pz ", 100, -300, 300);
  mHTkUPositionResidualsX = new TH1D("hTkUPositionResidualsX", "TkU x Residuals", 100, -5, 5);
  mHTkUPositionResidualsY = new TH1D("hTkUPositionResidualsY", "TkU y Residuals", 100, -5, 5);
  mHTkUMomentumResidualsT = new TH1D("hTkUMomentumResidualsT", "TkU pt Residuals", 100, -20, 20);
  mHTkUMomentumResidualsZ = new TH1D("hTkUMomentumResidualsZ", "TkU pz Residuals", 100, -50, 200);

  mHTkDMCPositionX = new TH1D("hTkDMCPositionX", "TkD MC x ", 100, -170, 170);
  mHTkDMCPositionY = new TH1D("hTkDMCPositionY", "TkD MC y ", 100, -170, 170);
  mHTkDMCMomentumT = new TH1D("hTkDMCMomentumT", "TkD MC pt ", 100, 0, 100);
  mHTkDMCMomentumZ = new TH1D("hTkDMCMomentumZ", "TkD MC pz ", 100, 0, 300);
  mHTkDRecPositionX = new TH1D("hTkDRecPositionX", "TkU Recon x ", 100, -170, 170);
  mHTkDRecPositionY = new TH1D("hTkDRecPositionY", "TkU Recon y ", 100, -170, 170);
  mHTkDRecMomentumT = new TH1D("hTkDRecMomentumT", "TkU Recon pt ", 100, 0, 100);
  mHTkDRecMomentumZ = new TH1D("hTkDRecMomentumZ", "TkU Recon pz ", 100, -300, 300);
  mHTkDPositionResidualsX = new TH1D("hTkDPositionResidualsX", "TkD x Residuals", 100, -5, 5);
  mHTkDPositionResidualsY = new TH1D("hTkDPositionResidualsY", "TkD y Residuals", 100, -5, 5);
  mHTkDMomentumResidualsT = new TH1D("hTkDMomentumResidualsT", "TkD pt Residuals", 100, -20, 20);
  mHTkDMomentumResidualsZ = new TH1D("hTkDMomentumResidualsZ", "TkD pz Residuals", 100, -50, 200);
}

bool AnalyserTrackerMCPRResiduals::analyse(MAUS::ReconEvent* const aReconEvent,
                                           MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent || !aMCEvent) {
    return false;
  }

  // Find a hit from a muon in the tracker references plane for each tracker
  MAUS::SciFiHit* tku_ref_hit = nullptr;
  MAUS::SciFiHit* tkd_ref_hit = nullptr;
  for (auto&& hitref : *(aMCEvent->GetSciFiHits())) {
    MAUS::SciFiHit* hit = &hitref;
    int pid = hit->GetParticleId();
    if (pid != 13 && pid != -13)
      continue;
    if (hit->GetChannelId()->GetStationNumber() == 1 && \
        hit->GetChannelId()->GetPlaneNumber() == 0) {
      if (hit->GetChannelId()->GetTrackerNumber() == 0) {
        tku_ref_hit = hit;
      } else if (hit->GetChannelId()->GetTrackerNumber() == 1) {
        tkd_ref_hit = hit;
      }
    }
    if (tku_ref_hit && tkd_ref_hit)
      break;
  }

  if (!tku_ref_hit || !tkd_ref_hit) {
    return false;
  }

  // Access the SciFi Event
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  if (!sfevt) {
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

  // Require 1 recon track in each tracker
  if (nTkURecTracks != 1 || nTkDRecTracks != 1) {
    return false;
  }

  if (tku_trk->get_spacepoints_pointers().size() != 5 ||
      tkd_trk->get_spacepoints_pointers().size() != 5) {
    return false;
  }

  // At this point we should an event that is good to analyse
  // Calculate the recon postion at the reference station, and the recon momentum (average)
  double tku_x = 0.0;
  double tku_y = 0.0;
  for (auto sp : tku_trk->get_spacepoints_pointers()) {
    if (sp->get_station() == 1) {
      tku_x = sp->get_global_position().x();
      tku_y = sp->get_global_position().y();
    }
  }

  double tkd_x = 0.0;
  double tkd_y = 0.0;
  for (auto sp : tkd_trk->get_spacepoints_pointers()) {
    if (sp->get_station() == 1) {
      tkd_x = sp->get_global_position().x();
      tkd_y = sp->get_global_position().y();
    }
  }

  double tku_dx = tku_x + tku_ref_hit->GetPosition().x();
  double tku_dy = tku_y - tku_ref_hit->GetPosition().y();

  double tku_ptmc = sqrt(tku_ref_hit->GetMomentum().x()*tku_ref_hit->GetMomentum().x() +
                         tku_ref_hit->GetMomentum().y()*tku_ref_hit->GetMomentum().y());
  double tku_ptrec = 0.3*mBfield*tku_trk->get_R(); // 0.3 comes from mom being in MeV and rad in mm
  double tku_dpt = tku_ptrec - tku_ptmc;

  double tku_pzrec = tku_ptrec / tku_trk->get_dsdz();
  double tku_dpz =  tku_pzrec + tku_ref_hit->GetMomentum().z();

  double tkd_dx = tkd_x - tkd_ref_hit->GetPosition().x();
  double tkd_dy = tkd_y - tkd_ref_hit->GetPosition().y();

  double tkd_ptmc = sqrt(tkd_ref_hit->GetMomentum().x()*tkd_ref_hit->GetMomentum().x() +
                         tkd_ref_hit->GetMomentum().y()*tkd_ref_hit->GetMomentum().y());
  double tkd_ptrec = 0.3*mBfield*tkd_trk->get_R(); // 0.3 comes from mom being in MeV and rad in mm
  double tkd_dpt = tkd_ptrec - tkd_ptmc;

  double tkd_pzrec = tkd_ptrec / tkd_trk->get_dsdz();
  double tkd_dpz = tkd_pzrec + tkd_ref_hit->GetMomentum().z();

  // Fill the histograms
  mHTkUMCPositionX->Fill(tku_ref_hit->GetPosition().x());
  mHTkUMCPositionY->Fill(tku_ref_hit->GetPosition().y());
  mHTkUMCMomentumT->Fill(tku_ptmc);
  mHTkUMCMomentumZ->Fill(tku_ref_hit->GetMomentum().z());

  mHTkURecPositionX->Fill(tku_x);
  mHTkURecPositionY->Fill(tku_y);
  mHTkURecMomentumT->Fill(tku_ptrec);
  mHTkURecMomentumZ->Fill(tku_pzrec);

  mHTkUPositionResidualsX->Fill(tku_dx);
  mHTkUPositionResidualsY->Fill(tku_dy);
  mHTkUMomentumResidualsT->Fill(tku_dpt);
  mHTkUMomentumResidualsZ->Fill(tku_dpz);

  mHTkDMCPositionX->Fill(tkd_ref_hit->GetPosition().x());
  mHTkDMCPositionY->Fill(tkd_ref_hit->GetPosition().y());
  mHTkDMCMomentumT->Fill(tkd_ptmc);
  mHTkDMCMomentumZ->Fill(tkd_ref_hit->GetMomentum().z());

  mHTkDRecPositionX->Fill(tkd_x);
  mHTkDRecPositionY->Fill(tkd_y);
  mHTkDRecMomentumT->Fill(tkd_ptrec);
  mHTkDRecMomentumZ->Fill(tkd_pzrec);

  mHTkDPositionResidualsX->Fill(tkd_dx);
  mHTkDPositionResidualsY->Fill(tkd_dy);
  mHTkDMomentumResidualsT->Fill(tkd_dpt);
  mHTkDMomentumResidualsZ->Fill(tkd_dpz);

  return true;
}

void AnalyserTrackerMCPRResiduals::draw(TVirtualPad* aPad) {
  GetStyle()->SetOptStat(111111);

  // Draw the MC distribution
  TVirtualPad* padMC = new TCanvas();
  padMC->cd();
  padMC->Divide(4, 2);
  padMC->cd(1);
  mHTkUMCPositionX->Draw();
  padMC->cd(2);
  mHTkUMCPositionY->Draw();
  padMC->cd(3);
  mHTkUMCMomentumT->Draw();
  padMC->cd(4);
  mHTkUMCMomentumZ->Draw();
  padMC->cd(5);
  mHTkDMCPositionX->Draw();
  padMC->cd(6);
  mHTkDMCPositionY->Draw();
  padMC->cd(7);
  mHTkDMCMomentumT->Draw();
  padMC->cd(8);
  mHTkDMCMomentumZ->Draw();

  // Draw the Recon distribution
  TVirtualPad* padRec = new TCanvas();
  padRec->cd();
  padRec->Divide(4, 2);
  padRec->cd(1);
  mHTkURecPositionX->Draw();
  padRec->cd(2);
  mHTkURecPositionY->Draw();
  padRec->cd(3);
  mHTkURecMomentumT->Draw();
  padRec->cd(4);
  mHTkURecMomentumZ->Draw();
  padRec->cd(5);
  mHTkDRecPositionX->Draw();
  padRec->cd(6);
  mHTkDRecPositionY->Draw();
  padRec->cd(7);
  mHTkDRecMomentumT->Draw();
  padRec->cd(8);
  mHTkDRecMomentumZ->Draw();

  // Draw the residuals with a linear scale
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

  // Draw the residuals with a log scale
  TVirtualPad* pad2 = new TCanvas();
  pad2->Divide(4, 2);
  pad2->cd(1);
  pad2->GetPad(1)->SetLogy(1);
  mHTkUPositionResidualsX->Draw();
  pad2->cd(2);
  pad2->GetPad(2)->SetLogy(1);
  mHTkUPositionResidualsY->Draw();
  pad2->cd(3);
  pad2->GetPad(3)->SetLogy(1);
  mHTkUMomentumResidualsT->Draw();
  pad2->cd(4);
  pad2->GetPad(4)->SetLogy(1);
  mHTkUMomentumResidualsZ->Draw();
  pad2->cd(5);
  pad2->GetPad(5)->SetLogy(1);
  mHTkDPositionResidualsX->Draw();
  pad2->cd(6);
  pad2->GetPad(6)->SetLogy(1);
  mHTkDPositionResidualsY->Draw();
  pad2->cd(7);
  pad2->GetPad(7)->SetLogy(1);
  mHTkDMomentumResidualsT->Draw();
  pad2->cd(8);
  pad2->GetPad(8)->SetLogy(1);
  mHTkDMomentumResidualsZ->Draw();

  AddPad(padMC);
  AddPad(padRec);
  AddPad(aPad);
  AddPad(pad2);
}
} // ~namespace mica
