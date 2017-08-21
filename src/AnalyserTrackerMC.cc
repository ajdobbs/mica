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

#include <string>

#include "TCanvas.h"
#include "TLatex.h"

#include "mica/AnalyserTrackerMC.hh"
#include "src/common_cpp/DataStructure/TOFEvent.hh"
#include "src/common_cpp/DataStructure/SciFiEvent.hh"
#include "src/common_cpp/DataStructure/SciFiBasePRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiHelicalPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiStraightPRTrack.hh"
#include "src/common_cpp/DataStructure/SciFiSpacePoint.hh"


namespace mica {

AnalyserTrackerMC::AnalyserTrackerMC() : mRefStation {1},
                                         mRefPlane {0},
                                         mNStations {5},
                                         mNPlanes {3},
                                         mLookup {nullptr} {
  // Do nothing
}

AnalyserTrackerMC::~AnalyserTrackerMC() {
  clear_mc_data();
  clear_lookup();
}

bool AnalyserTrackerMC::analyse(MAUS::ReconEvent* const aReconEvent, MAUS::MCEvent* const aMCEvent) {
  if (analyse_mc(aMCEvent)) {
    return analyse_recon(aReconEvent);
  } else {
    return false;
  }
  return false;
}

bool AnalyserTrackerMC::analyse_mc(MAUS::MCEvent* const aMCEvent) {
  bool lookup_pass = make_lookup(aMCEvent);
  if (!lookup_pass) {
    std::cerr << "WARNING: AnalyserTrackerMC::analyse_mc: Failed to make SciFiLookup\n";
  } else if (mLookup->get_hits_map().size() == 0) {
    lookup_pass = false;
  } else {
    std::cerr << "INFO: AnalyserTrackerMC::analyse_mc: Lookup hit map size: "
              << mLookup->get_hits_map().size() << std::endl;
  }
  if (!lookup_pass) return false;

  bool data_pass = fill_mc_track_data(aMCEvent);
  return (lookup_pass && data_pass);
}

void AnalyserTrackerMC::clear_lookup() {
  if (mLookup) delete mLookup;
  mLookup = nullptr;
}

void AnalyserTrackerMC::clear_mc_data() {
  for (auto datum : mMCDataTkU) {
    if (datum) delete datum;
  }
  for (auto datum : mMCDataTkD) {
    if (datum) delete datum;
  }
  mMCDataTkU.resize(0);
  mMCDataTkD.resize(0);
}

std::map<int, std::vector<int> > \
    AnalyserTrackerMC::calc_stations_hit_by_track(std::map<int,
                                                           std::vector<MAUS::SciFiHit*> >& hit_map,
                                                  int aNPlanes) {
  std::map<int, std::vector<int> > stations_hit_by_track; // stations_hit_by_track[trk_id]
  for (auto& station : hit_map) {
    // planes_hit_by_track[track_id][plane] = true or false - did track generate hit in this plane
    std::map<int, std::map<int, bool> > planes_hit_by_track;
    for (auto hit : station.second) {
      planes_hit_by_track[hit->GetTrackId()][hit->GetChannelId()->GetPlaneNumber()] = true;
    }
    for (auto& trk : planes_hit_by_track) {
      if (trk.second.size() >= aNPlanes) { // Did track produce hits in >= 2 planes of this station
        stations_hit_by_track[trk.first].push_back(station.first);
      }
    }
  }
  return stations_hit_by_track;
}

bool AnalyserTrackerMC::fill_mc_track_data(MAUS::MCEvent* const aMCEvent) {
  if (!aMCEvent) return false;

  auto hits = aMCEvent->GetSciFiHits();
  std::map<int, std::vector<MAUS::SciFiHit*> > hit_map_tku;
  std::map<int, std::vector<MAUS::SciFiHit*> > hit_map_tkd;

  // Sort scifi hits into a map from station number to a vector holding the hits for that station
  for (auto&& hit_ref : *hits) {
    MAUS::SciFiHit* hit = &hit_ref;
    if (hit->GetChannelId()->GetTrackerNumber() == 0) {
      hit_map_tku[hit->GetChannelId()->GetStationNumber()].push_back(hit);
    } else if (hit->GetChannelId()->GetTrackerNumber() == 1) {
      hit_map_tkd[hit->GetChannelId()->GetStationNumber()].push_back(hit);
    }
  }

  // Loop over each station, & see what tracks produced hits in >= mNPlanes planes for that station
  std::map<int, std::vector<int> > trk_id_to_stations_hit_tku = \
      calc_stations_hit_by_track(hit_map_tku, mNPlanes);
  std::map<int, std::vector<int> > trk_id_to_stations_hit_tkd = \
      calc_stations_hit_by_track(hit_map_tkd, mNPlanes);

  // We now have a map for each tracker from mc track id to the station numbers for which it
  // generated hits in 2 or more planes. Next, see which track ids produced such hits in mNStations
  // or more stations, that is all the track ids which could have created a reconstructible track
  for (auto trk : trk_id_to_stations_hit_tku) {
    if (trk.second.size() >= mNStations) {
      MCTrackData* data = nullptr;
      for (auto hit : hit_map_tku[trk.first]) {
        if (hit->GetChannelId()->GetStationNumber() == mRefStation && \
            hit->GetChannelId()->GetPlaneNumber() == mRefPlane) {
          data = new MCTrackData();
          data->tracker = 0;
          data->track_id = trk.first;
          data->pid = hit->GetParticleId();
          data->energy = hit->GetEnergy();
          data->pos = hit->GetPosition();
          data->mom = hit->GetMomentum();
          mMCDataTkU.push_back(data);
          break;
        }
      }
      if (!data) {
        std::cerr << "WARNING: No hit present in TkU reference plane\n";
      } else {
        std::cerr << "INFO: Created MCTrackData for TkU\n";
      }
    }
  }
  for (auto trk : trk_id_to_stations_hit_tkd) {
    if (trk.second.size() >= mNStations) {
      MCTrackData* data = nullptr;
      for (auto hit : hit_map_tkd[trk.first]) {
        if (hit->GetChannelId()->GetStationNumber() == mRefStation && \
            hit->GetChannelId()->GetPlaneNumber() == mRefPlane) {
          data = new MCTrackData();
          data->tracker = 1;
          data->track_id = trk.first;
          data->pid = hit->GetParticleId();
          data->energy = hit->GetEnergy();
          data->pos = hit->GetPosition();
          data->mom = hit->GetMomentum();
          mMCDataTkD.push_back(data);
          break;
        }
      }
      if (!data) {
        std::cerr << "WARNING: No hit present in TkD reference plane\n";
      } else {
        std::cerr << "INFO: Created MCTrackData for TkD\n";
      }
    }
  }
  return true;
}

bool AnalyserTrackerMC::make_lookup(MAUS::MCEvent* const aMCEvent) {
  if (!aMCEvent) return false;
  if (mLookup) delete mLookup;
  mLookup = new MAUS::SciFiLookup();
  return mLookup->make_hits_map(aMCEvent);
}
} // ~namespace mica
