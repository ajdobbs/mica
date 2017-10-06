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
 *         Singleton factory class for the creation of MICA analysers. New MICA analyser
 *         types should be registered with the the CreateAnalyser method.
 *  @author A. Dobbs
 */
class AnalyserFactory {
  public:
    /** Create a new instance of the analyser type represented by the string arg */
    static AnalyserBase* CreateAnalyser(const std::string& aName);

    /** Create a vector of analysers, with the specific types defined the vector of strings arg */
    static std::vector<AnalyserBase*> CreateAnalysers(const std::vector<std::string>& aNames);

    /** Create a new instance of the analyser type represented by the string arg */
    static std::shared_ptr<AnalyserBase> CreateSharedAnalyser(const std::string& aName);

    /** Create a vector of analysers, with the specific types defined the vector of strings arg */
    static std::vector<std::shared_ptr<AnalyserBase>>
      CreateSharedAnalysers(const std::vector<std::string>& aNames);

    /** Create a new instance of the analyser type represented by the string arg */
    static std::unique_ptr<AnalyserBase> CreateUniqueAnalyser(const std::string& aName);

    /** Create a vector of analysers, with the specific types defined the vector of strings arg */
    static std::vector<std::unique_ptr<AnalyserBase>>
      CreateUniqueAnalysers(const std::vector<std::string>& aNames);
};
} // ~namespace mica

#endif
