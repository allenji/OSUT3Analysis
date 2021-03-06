#include "OSUT3Analysis/Collections/plugins/EventvariableProducer.h"

#if IS_VALID(eventvariables)

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

EventvariableProducer::EventvariableProducer (const edm::ParameterSet &cfg) :
  collections_ (cfg.getParameter<edm::ParameterSet> ("collections"))
{
  collection_ = collections_.getParameter<edm::InputTag> ("eventvariables");

  produces<osu::Eventvariable> (collection_.instance ());
}

EventvariableProducer::~EventvariableProducer ()
{
}

void
EventvariableProducer::produce (edm::Event &event, const edm::EventSetup &setup)
{
  edm::Handle<TYPE (eventvariables)> collection;
  if (!anatools::getCollection (collection_, collection, event, false))
    return;

  pl_ = auto_ptr<osu::Eventvariable> (new osu::Eventvariable (*collection));
  event.put (pl_, collection_.instance ());
  pl_.reset ();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventvariableProducer);

#endif
