#ifndef CUT_CALCULATOR
#define CUT_CALCULATOR

#include <unordered_set>

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

// Declaration of the CutCalculator EDProducer which produces various flags
// indicating whether the event and each object passed the user-defined cuts.
class CutCalculator : public edm::stream::EDProducer<>
{
  public:
    CutCalculator (const edm::ParameterSet &);
    ~CutCalculator ();

    void produce (edm::Event &, const edm::EventSetup &);

  private:
    ////////////////////////////////////////////////////////////////////////////
    // Private methods used in calculating the cut decisions.
    ////////////////////////////////////////////////////////////////////////////
    bool setObjectFlags (const Cut &, unsigned) const;
    void updateCrossTalk (const Cut &, unsigned) const;
    bool unpackCuts ();
    void getTwoObjs (string, string &, string &);
    bool evaluateComparison (int, const string &, int) const;
    string getObjToGet (string);
    vector<string> splitString (const string &) const;
    bool evaluateTriggers (const edm::Event &) const;
    bool setEventFlags () const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private variables initialized by the constructor.
    ////////////////////////////////////////////////////////////////////////////
    edm::ParameterSet  collections_;
    edm::ParameterSet  cuts_;
    bool               firstEvent_;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Private variables set after unpacking the cuts ParameterSet.
    ////////////////////////////////////////////////////////////////////////////
    unordered_set<string>  objectsToGet_;
    Cuts                   unpackedCuts_;
    vector<string>         unpackedTriggersToVeto_;
    vector<string>         unpackedTriggers_;
    ////////////////////////////////////////////////////////////////////////////

    // Object collections which can be gotten from the event.
    Collections handles_;
    Tokens tokens_;

    // Payload for this EDProducer.
    auto_ptr<CutCalculatorPayload>  pl_;

    // Function for initializing the ValueLookupTree objects, one for each cut.
    bool initializeValueLookupForest (Cuts &, Collections * const);
};

#endif
