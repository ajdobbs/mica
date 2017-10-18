#ifndef IANALYSER_HH
#define IANALYSER_HH

#include "mica/AnalyserBase.hh"

namespace mica {

/** @class IAnalyser  Template class inserted between AnalyserBase and the concrete daughter
 *  classes. The purpose of the class is to enable CRTP, allowing the merge function to be defined.
 *  We want to make merge virtual void to force the daughter classes implement it, however merge
 *  requires its argument to be the daughter class type, hence the neccessity of CRTP.
 *  May also implement a Clone here, easier than doing it in each daughter class (another CRTP use).
 *
 *  @tparam Derived The daughter class type
 */
template <typename Derived>
class IAnalyser : public AnalyserBase {
  public:
    IAnalyser() {};
    virtual ~IAnalyser() {};

    /**  Converts the AnalyserBase pointer into the derived class pointer and passes this to merge
      *
      * @param aAnalyser The analyser whose internal data is to be added to the current data
      */
    virtual bool Merge(AnalyserBase* aAnalyser) override {
      Derived* derivedAnalyser = dynamic_cast<Derived*>(aAnalyser);
      if (!derivedAnalyser)
        return false;
      merge(derivedAnalyser);
      return true;
    }

  private:
    /**  Add data from the internal data variables of another analyser instance to the internal
      *  data variables of this analyser instance
      *
      * @param aAnalyser The analyser whose internal data is to be added to the current data
      */
    virtual void merge(Derived* aAnalyser) = 0;
};
} // ~namespace mica

#endif
