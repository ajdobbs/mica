/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include <string>
#include <vector>

#include "TCanvas.h"
#include "TLatex.h"

#include "mica/AnalyserTrackerPREfficiency.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiBasePRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiStraightPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"

namespace mica {

AnalyserTrackerPREfficiency::AnalyserTrackerPREfficiency() : mCheckTOF(true),
                                                             mCheckTOFSpacePoints(true),
                                                             mAllowMultiHitStations(true),
                                                             mCheckTkU(true),
                                                             mCheckTkD(true),
                                                             mNEvents(0),
                                                             mTkUGoodEvents(0),
                                                             mTkU5ptTracks(0),
                                                             mTkU4to5ptTracks(0),
                                                             mTkDGoodEvents(0),
                                                             mTkD5ptTracks(0),
                                                             mTkD4to5ptTracks(0),
                                                             mLowerTimeCut(27.0),
                                                             mUpperTimeCut(50.0) {
  // mOf1.open("tracker-patrec-efficiency.txt");
}

AnalyserTrackerPREfficiency::~AnalyserTrackerPREfficiency() {
  // mOf1.close();
}

bool AnalyserTrackerPREfficiency::analyse(MAUS::ReconEvent* const aReconEvent,
                                          MAUS::MCEvent* const aMCEvent) {
  if (!aReconEvent)
    return false;

  ++mNEvents;

  MAUS::TOFEvent* tofevt = aReconEvent->GetTOFEvent();
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();

  // Check if we a good expected track event in TkU and separately in TkD
  // --------------------------------------------------------------------

  // Bools to hold if after considering all relevant detectors including trackers
  // we expect to have reconstructed one track in the trackers (tku and tkd separately)
  bool good_event_tku = false;
  bool good_event_tkd = false;

  // Check TOF
  bool good_tof = check_good_tof_event(tofevt);

  // Check TkU, but don't don't look at recon tracks yet, only up to spacepoints
  bool good_tku_4pt = false; // Should reconstruct a 4pt, not enough sp for 5pt
  bool good_tku_5pt = false; // Should reconstrcut full 5pt
  if (mCheckTkU) {
    check_good_tk_event(sfevt, 0, good_tku_4pt, good_tku_5pt);
  } else {
    good_tku_4pt = true;
    good_tku_5pt = true;
  }

  // For now, only call an event good if we expect a full 5pt track
  if (good_tof && good_tku_5pt) {
    good_event_tku = true;
    ++mTkUGoodEvents;
  }

  // Check TkD but don't don't look at recon tracks yet, only up to spacepoints
  bool good_tkd_4pt = false; // Should reconstruct a 4pt, not enough sp for 5pt
  bool good_tkd_5pt = false; // Should reconstrcut full 5pt
  if (mCheckTkD) {
    check_good_tk_event(sfevt, 1, good_tkd_4pt, good_tkd_5pt);
  } else {
    good_tkd_4pt = true;
    good_tkd_5pt = true;
  }

  // For now, only call an event good if we expect a full 5pt track
  if (good_tof && good_tkd_5pt) {
    good_event_tkd = true;
    ++mTkDGoodEvents;
  }

  // Save some time and stop here is neither tracker has a good event
  if (!good_event_tku && !good_event_tkd)
    return true;

  // Check if patrec actually reconstructed the track as expected
  // ------------------------------------------------------------

  // Sort by tracker
  std::vector<MAUS::SciFiBasePRTrack*> tku;
  std::vector<MAUS::SciFiBasePRTrack*> tkd;

  std::vector<MAUS::SciFiHelicalPRTrack*> htrks = sfevt->helicalprtracks();
  for (auto trk : htrks) {
    if (trk->get_tracker() == 0) {
      tku.push_back(trk);
    } else {
      tkd.push_back(trk);
    }
  }
  std::vector<MAUS::SciFiStraightPRTrack*> strks = sfevt->straightprtracks();
  for (auto trk : strks) {
    if (trk->get_tracker() == 0) {
      tku.push_back(trk);
    } else {
      tkd.push_back(trk);
    }
  }

  // If we have a good event and only one track, increment the counters
  if (good_event_tku && tku.size() == 1) {
    ++mTkU4to5ptTracks;
    if (tku[0]->get_num_points() == 5)
      ++mTkU5ptTracks;
  }
  if (good_event_tkd && tkd.size() == 1) {
    ++mTkD4to5ptTracks;
    if (tkd[0]->get_num_points() == 5)
      ++mTkD5ptTracks;
  }

  // All done for this event
  return true;
}

void AnalyserTrackerPREfficiency::clear() {
  mNEvents = 0;
  mTkUGoodEvents = 0;
  mTkU5ptTracks = 0;
  mTkU4to5ptTracks = 0;
  mTkDGoodEvents = 0;
  mTkD5ptTracks = 0;
  mTkD4to5ptTracks = 0;
}

void AnalyserTrackerPREfficiency::check_good_tk_event(MAUS::SciFiEvent* evt, int trker_num,
                                                      bool& good4pt, bool& good5pt) {
  good4pt = false;
  good5pt = false;

  if (!evt)
    return;

  // Check number of spacepoints per station meets cuts
  std::vector<int> num_spoints_per_station(5, 0); // length 5, all zeros

  for (auto sp : evt->spacepoints()) {
    if (sp->get_tracker() != trker_num)
      continue;
    ++(num_spoints_per_station.at(sp->get_station() - 1));
  }

  int stations_empty = 0;  // num stations with less than 1 sp
  int stations_multi = 0;  // num stations with more than 1 sp
  int stations_hit = 0;    // num stations with more than 0 sp
  int stations_ideal = 0;  // num stations with exactly 1 sp
  for (int n : num_spoints_per_station) {
    if (n < 1)
      ++stations_empty;
    if (n > 1)
      ++stations_multi;
    if (n > 0)
      ++stations_hit;
    if (n == 1)
      ++stations_ideal;
  }

  // Do we want idea events only (1 sp only per tracker station) or allow multiple sp
  // per station, just not in enough stations for more than one track to be possible?
  int stations_multi_cut = 4; // Non ideal allowed
  if (!mAllowMultiHitStations)
    stations_multi_cut = 1; // Ideal events only

  // Require 4 stations ONLY are hit
  if (stations_hit == 4 && stations_multi < stations_multi_cut)
    good4pt = true;

  // Require all 5 stations are hit
  if (stations_hit == 5 && stations_multi < stations_multi_cut)
    good5pt = true;

  return;
}

bool AnalyserTrackerPREfficiency::check_good_tof_event(MAUS::TOFEvent* evt) {
  // Do we care about TOF? If not return true
  if (!mCheckTOF && !mCheckTOFSpacePoints)
    return true;

  // If we care about TOF but the data is missing, return false
  if (!evt)
    return false;
  if (!evt->GetTOFEventSpacePointPtr())
    return false;

  // Check the TOF spacepoints - 1 and only 1 in TOF1 and 1 and only 1 in TOF2
  // Note: If we are checking the actual time-of-flight we need this to be true, so we do not
  // need to mCheckTOFSpacePoints, as we already know one of our two flags must be true
  std::vector<MAUS::TOFSpacePoint>*  tof1_spoints = \
      evt->GetTOFEventSpacePointPtr()->GetTOF1SpacePointArrayPtr();
  std::vector<MAUS::TOFSpacePoint>*  tof2_spoints = \
      evt->GetTOFEventSpacePointPtr()->GetTOF2SpacePointArrayPtr();
  if (!tof1_spoints || !tof2_spoints) {
    return false;
  }

  if ((tof1_spoints->size() != 1) || (tof2_spoints->size() != 1))
    return false;

  // OK, if we have reached this point, then the tof spacepoints criteria is satisfied,
  // if we are not looking at tof timing we can return true
  if (!mCheckTOF)
    return true;

  // Lastly, check the time-of-flight between TOF1 and TOF2
  double dt = tof2_spoints->at(0).GetTime() - tof1_spoints->at(0).GetTime();
  if (dt > mLowerTimeCut && dt < mUpperTimeCut)
    return true;

  // We should only arrive here if everything passed except the time-of-flight cut
  return false;
}

bool AnalyserTrackerPREfficiency::draw(std::shared_ptr<TVirtualPad> aPad) {
  double tku_5pt_eff = static_cast<double>(mTkU5ptTracks) / static_cast<double>(mTkUGoodEvents);
  double tku_4to5pt_eff =
      static_cast<double>(mTkU4to5ptTracks) / static_cast<double>(mTkUGoodEvents);
  double tkd_5pt_eff = static_cast<double>(mTkD5ptTracks) / static_cast<double>(mTkDGoodEvents);
  double tkd_4to5pt_eff =
      static_cast<double>(mTkD4to5ptTracks) /  static_cast<double>(mTkDGoodEvents);

  std::cerr << mNEvents << " "
            << mTkU5ptTracks << " " << mTkU4to5ptTracks << " " << mTkUGoodEvents << " "
            << mTkD5ptTracks << " " << mTkD4to5ptTracks << " " << mTkDGoodEvents << " "
            << tku_5pt_eff << " " <<  tku_4to5pt_eff
            <<  " " << tkd_5pt_eff <<  " " << tkd_4to5pt_eff << std::endl;

  TLatex tl;
  tl.SetTextSize(0.05);
  tl.DrawLatex(0.1, 0.9, "PatRec Efficiency");
  tl.SetTextSize(0.03);

  std::string boolstring_tku = "false";
  std::string boolstring_tkd = "false";
  std::string boolstring_tof = "false";
  std::string boolstring_tof_sp = "false";
  if (mCheckTkU)
    boolstring_tku = "true";
  else
    boolstring_tku = "false";
  if (mCheckTkD)
    boolstring_tkd = "true";
  else
    boolstring_tkd = "false";
  if (mCheckTOF)
    boolstring_tof = "true";
  else
    boolstring_tof = "false";
  if (mCheckTOF || mCheckTOFSpacePoints)
    boolstring_tof_sp = "true";
  else
    boolstring_tof_sp = "false";

  double tline = 0.8;
  double sep = 0.075;
  tl.DrawLatex(0.1, tline, ("Check TkU spacepoints: " + boolstring_tku).c_str());
  tl.DrawLatex(0.1, tline-sep, ("Check TkD spacepoints: " + boolstring_tkd).c_str());
  tl.DrawLatex(0.1, tline-sep*2, ("Check TOF1 and TOF2 spacepoints: "
                                    + boolstring_tof_sp).c_str());
  tl.DrawLatex(0.1, tline-sep*3, ("Check time-of-flight TOF2 - TOF1 : " + boolstring_tof).c_str());

  tl.DrawLatex(0.1, tline-sep*4, ("Total Number of Events: " + std::to_string(mNEvents)).c_str());
  tl.DrawLatex(0.1, tline-sep*5, ("TkU 5pt Efficiency: " + std::to_string(tku_5pt_eff) + "  ("
    + std::to_string(mTkU5ptTracks) + "/" + std::to_string(mTkUGoodEvents) + ")").c_str());
  tl.DrawLatex(0.1, tline-sep*6, ("TkU 4-5pt Efficiency: " + std::to_string(tku_4to5pt_eff) + " ("
    + std::to_string(mTkU4to5ptTracks) + "/" + std::to_string(mTkUGoodEvents) + ")").c_str());
  tl.DrawLatex(0.1, tline-sep*7, ("TkD 5pt Efficiency: " + std::to_string(tkd_5pt_eff) + "  ("
    + std::to_string(mTkD5ptTracks) + "/" + std::to_string(mTkDGoodEvents) + ")").c_str());
  tl.DrawLatex(0.1, tline-sep*8, ("TkU 4-5pt Efficiency: " + std::to_string(tkd_4to5pt_eff) + "  ("
    + std::to_string(mTkD4to5ptTracks)+ "/" + std::to_string(mTkDGoodEvents) + ")").c_str());

  GetPads()[0]->cd();
  tl.Draw();
  GetPads()[0]->Update();

  return true;
}
} // ~namespace mica

