/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */


#include "mica/AnalyserFactory.hh"
#include "mica/AnalyserTofTracker.hh"
#include "mica/AnalyserTrackerAngularMomentum.hh"
#include "mica/AnalyserTrackerChannelHits.hh"
#include "mica/AnalyserTrackerKFMomentum.hh"
#include "mica/AnalyserTrackerKFStats.hh"
#include "mica/AnalyserTrackerMCPRResiduals.hh"
#include "mica/AnalyserTrackerMCPurity.hh"
#include "mica/AnalyserTrackerPREfficiency.hh"
#include "mica/AnalyserTrackerPRSeedNPEResidual.hh"
#include "mica/AnalyserTrackerPRSeedResidual.hh"
#include "mica/AnalyserTrackerPRStats.hh"
#include "mica/AnalyserTrackerSpacePoints.hh"
#include "mica/AnalyserTrackerSpacePointSearch.hh"
#include "mica/AnalyserTrackerSpacePointSearchStation.hh"

namespace mica {

AnalyserBase* AnalyserFactory::CreateAnalyser(const std::string& aName) {
  if (aName == "AnalyserTofTracker") return new AnalyserTofTracker();
  if (aName == "AnalyserTrackerAngularMomentum") return new AnalyserTrackerAngularMomentum();
  if (aName == "AnalyserTrackerChannelHits") return new AnalyserTrackerChannelHits();
  if (aName == "AnalyserTrackerKFMomentum") return new AnalyserTrackerKFMomentum();
  if (aName == "AnalyserTrackerKFStats") return new AnalyserTrackerKFStats();
  if (aName == "AnalyserTrackerMCPRResiduals") return new AnalyserTrackerMCPRResiduals();
  if (aName == "AnalyserTrackerMCPurity") return new AnalyserTrackerMCPurity();
  if (aName == "AnalyserTrackerPREfficiency") return new AnalyserTrackerPREfficiency();
  if (aName == "AnalyserTrackerPRSeedNPEResidual") return new AnalyserTrackerPRSeedNPEResidual();
  if (aName == "AnalyserTrackerPRSeedResidual") return new AnalyserTrackerPRSeedResidual();
  if (aName == "AnalyserTrackerPRStats") return new AnalyserTrackerPRStats();
  if (aName == "AnalyserTrackerSpacePoints") return new AnalyserTrackerSpacePoints();
  if (aName == "AnalyserTrackerSpacePointSearch") return new AnalyserTrackerSpacePointSearch();
  if (aName == "AnalyserTrackerSpacePointSearchStation")
    return new AnalyserTrackerSpacePointSearchStation();
  return nullptr;
}

std::vector<AnalyserBase*> AnalyserFactory::CreateAnalysers(const std::vector<std::string>& aNames) {
  std::vector<AnalyserBase*> analysers;
  for (auto s : aNames) {
     analysers.push_back(CreateAnalyser(s));
  }
  return analysers;
}

AnalyserGroup AnalyserFactory::CreateAnalyserGroup(const std::vector<std::string>& aNames) {
  AnalyserGroup analysers;
  for (auto s : aNames) {
     analysers.AddAnalyser(CreateAnalyser(s));
  }
  return analysers;
}

std::shared_ptr<AnalyserBase> AnalyserFactory::CreateSharedAnalyser(const std::string &aName) {
  return std::shared_ptr<AnalyserBase>(CreateAnalyser(aName));
}

std::vector<std::shared_ptr<AnalyserBase>>
    AnalyserFactory::CreateSharedAnalysers(const std::vector<std::string>& aNames) {
  std::vector<std::shared_ptr<AnalyserBase>> analysers;
  for (auto s : aNames) {
     analysers.push_back(CreateSharedAnalyser(s));
  }
  return analysers;
}

std::unique_ptr<AnalyserBase>  AnalyserFactory::CreateUniqueAnalyser(const std::string &aName) {
  return std::unique_ptr<AnalyserBase>(CreateAnalyser(aName));
}

std::vector<std::unique_ptr<AnalyserBase>>
    AnalyserFactory::CreateUniqueAnalysers(const std::vector<std::string>& aNames) {
  std::vector<std::unique_ptr<AnalyserBase>> analysers;
  for (auto s : aNames) {
     analysers.push_back(CreateUniqueAnalyser(s));
  }
  return analysers;
}

} // ~namespace mica
