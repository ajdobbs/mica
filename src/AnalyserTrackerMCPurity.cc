/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#include <map>

#include "mica/AnalyserTrackerMCPurity.hh"
#include "TLatex.h"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"

namespace mica {

AnalyserTrackerMCPurity::AnalyserTrackerMCPurity() : mHTracksMatched(nullptr) {
  mHTracksMatched = new TH1I("hTracksMatched", "Recon Tracks Matched to MC Track IDs", 13, -3, 10);
}

bool AnalyserTrackerMCPurity::analyse_recon(MAUS::ReconEvent* const aReconEvent) {
  // std::cout << "Found " << GetMCDataTkU().size() << " TkU tracks & " << GetMCDataTkD().size() <<
  //              " TkD tracks\n";

  // Loop over pattern recognition helical tracks
  MAUS::SciFiEvent* sfevt = aReconEvent->GetSciFiEvent();
  for (auto trk : sfevt->helicalprtracks()) {
    int track_id = find_mc_track_id(trk);
    std::cout << "Found associated mc track id: " << track_id << std::endl;
    mHTracksMatched->Fill(track_id);
  }

  clear_lookup();
  clear_mc_data();
  return true;
}

void AnalyserTrackerMCPurity::draw(TVirtualPad* aPad) {
  aPad->Divide(2);
  aPad->cd(1);
  TLatex tl;
  tl.SetTextSize(0.03);
  tl.DrawLatex(0.1, 0.8, "PatRec Purity");
  aPad->cd();
  tl.Draw();
  aPad->Update();
  aPad->cd(2);
  mHTracksMatched->Draw();
  aPad->Update();
}

int AnalyserTrackerMCPurity::find_mc_track_id(MAUS::SciFiBasePRTrack* trk) {
  std::vector<MAUS::SciFiHit*> all_track_hits;
  auto htrk = dynamic_cast<MAUS::SciFiHelicalPRTrack*>(trk); // NOLINT
  if (htrk) std::cerr << "Tk" << htrk->get_tracker() << " ";
  std::cerr << "Recon track at " << trk << " has " << trk->get_spacepoints_pointers().size()
            << " seed spacepoints\n";
  for (auto spnt : trk->get_spacepoints_pointers()) {
    for (auto clus : spnt->get_channels_pointers()) {
      for (auto dig : clus->get_digits_pointers()) {
        // Perform the digits to hits lookup
        std::vector<MAUS::SciFiHit*> hits;
        bool lkup_result = GetLookup()->get_hits(dig, hits);
        if (!lkup_result) {
          std::cerr << "WARNING: AnalyserTrackerMCPurity: Lookup failed\n";
          continue;
        }
        std::cerr << "Adding " << hits.size() << " hits, ";
        all_track_hits.insert(all_track_hits.end(), hits.begin(), hits.end());
        std::cerr << "now have " << all_track_hits.size() << " in total\n";
      }
    }
  }
  std::cerr << "Found " << all_track_hits.size() << " hits for this track\n";

  if (all_track_hits.size() == 0) return -1;

  // Sort track's scifi hits into map from station # to a vector holding the hits for that station
  std::map<int, std::vector<MAUS::SciFiHit*> > hit_map;
  for (auto hit : all_track_hits) {
    hit_map[hit->GetChannelId()->GetStationNumber()].push_back(hit);
  }

  // Do we have 1 and only 1 mc_track_id which produced hits in >= 3 stations for this rec track
  std::map<int, std::vector<int> > hit_stations = calc_stations_hit_by_track(hit_map, 2);
  std::vector<int> reconstructible_mc_track_ids;
  for (auto mctrack : hit_stations) {
    std::cerr << " Track id " << mctrack.first << " hit " << mctrack.second.size() << " stations\n";
    if (mctrack.second.size() >= 3) reconstructible_mc_track_ids.push_back(mctrack.first);
  }
  int mc_track_id = -1;
  if (reconstructible_mc_track_ids.size() == 1) {
    mc_track_id = reconstructible_mc_track_ids[0];
  } else if (reconstructible_mc_track_ids.size() < 1) {
    mc_track_id = -2;
  } else if (reconstructible_mc_track_ids.size() > 1) {
    mc_track_id = -3;
  }
  return mc_track_id;
}

} // ~namespace mica
