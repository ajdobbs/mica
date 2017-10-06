/* This file is part of the MICA (Muon Ionization Cooling Analysis) package.
 * Author: A. Dobbs
 */

#ifndef ANALYSERFACTORY_HH
#define ANALYSERFACTORY_HH

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mica/AnalyserBase.hh"

namespace mica {

/** @class AnalyserFactory
 *         Singleton factory class for the creation of analysers.
 *  @author A. Dobbs
 */
class AnalyserFactory {

  public:
    static AnalyserBase* CreateAnalyser(const std::string& aName);
    static std::vector<AnalyserBase*> CreateAnalysers(const std::vector<std::string>& aNames);
    static std::shared_ptr<AnalyserBase> CreateSharedAnalyser(const std::string& aName);
    static std::vector<std::shared_ptr<AnalyserBase>>
      CreateSharedAnalysers(const std::vector<std::string>& aNames);
    static std::unique_ptr<AnalyserBase> CreateUniqueAnalyser(const std::string& aName);
    static std::vector<std::unique_ptr<AnalyserBase>>
      CreateUniqueAnalysers(const std::vector<std::string>& aNames);

};

} // ~namespace mica

#endif
