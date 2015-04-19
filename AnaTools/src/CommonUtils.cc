#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

vector<string>
anatools::getSingleObjects (string inputLabel)
{
  vector<string> singleObjects;
  size_t hyphen;
  while ((hyphen = inputLabel.find ('-')) != string::npos)
    {
      singleObjects.push_back (plural (inputLabel.substr (0, hyphen)));
      inputLabel = inputLabel.substr (hyphen + 1);
    }
  singleObjects.push_back (plural (inputLabel));
  sort (singleObjects.begin (), singleObjects.end ());

  return singleObjects;
}

string
anatools::concatenateInputCollection (const vector<string> &inputCollections)
{
  string catInputCollection = "";
  for (auto collection = inputCollections.begin (); collection != inputCollections.end (); collection++)
    {
      if (collection != inputCollections.begin ())
        catInputCollection += "-";
      catInputCollection += singular (*collection);
    }
  return plural (catInputCollection);
}

string
anatools::capitalize (string input)
{
  input.front () = toupper (input.front ());
  return input;
}

string
anatools::singular (string input)
{
  if (tolower (input.back ()) == 's')
    return input.substr (0, input.size () - 1);
  else
    return input;
}

string
anatools::plural (string input)
{
  if (tolower (input.back ()) == 's')
    return input;
  else
    return input + "s";
}

string &
anatools::ltrim (string &s)
{
  return s.erase (0, s.find_first_not_of (" \t\f\n\r"));
}

string &
anatools::rtrim (string &s)
{
  return s.erase (s.find_last_not_of (" \t\f\n\r") + 1);
}

string &
anatools::trim (string &s)
{
  return ltrim (rtrim (s));
}

bool
anatools::firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

bool
anatools::collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> > a, pair<string, tuple<unsigned, unsigned, void *> > b)
{
  return (get<0> (a.second) < get<0> (b.second));
}

void
anatools::getRequiredCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, Collections &handles, const edm::Event &event, const Tokens * const tokens)
{
  static bool firstEvent = true;

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if (tokens)
    {
      if  (VEC_CONTAINS  (objectsToGet,  "beamspots")       &&  collections.exists  ("beamspots"))       getCollection  (collections.getParameter<edm::InputTag>  ("beamspots"),       handles.beamspots,       event,  tokens->beamspots);
      if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")         &&  collections.exists  ("bxlumis"))         getCollection  (collections.getParameter<edm::InputTag>  ("bxlumis"),         handles.bxlumis,         event,  tokens->bxlumis);
      if  (VEC_CONTAINS  (objectsToGet,  "electrons")       &&  collections.exists  ("electrons"))       getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),       handles.electrons,       event,  tokens->electrons);
      if  (VEC_CONTAINS  (objectsToGet,  "events")          &&  collections.exists  ("events"))          getCollection  (collections.getParameter<edm::InputTag>  ("events"),          handles.events,          event,  tokens->events);
      if  (VEC_CONTAINS  (objectsToGet,  "genjets")         &&  collections.exists  ("genjets"))         getCollection  (collections.getParameter<edm::InputTag>  ("genjets"),         handles.genjets,         event,  tokens->genjets);
      if  (VEC_CONTAINS  (objectsToGet,  "jets")            &&  collections.exists  ("jets"))            getCollection  (collections.getParameter<edm::InputTag>  ("jets"),            handles.jets,            event,  tokens->jets);
      if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")     &&  collections.exists  ("mcparticles"))     getCollection  (collections.getParameter<edm::InputTag>  ("mcparticles"),     handles.mcparticles,     event,  tokens->mcparticles);
      if  (VEC_CONTAINS  (objectsToGet,  "mets")            &&  collections.exists  ("mets"))            getCollection  (collections.getParameter<edm::InputTag>  ("mets"),            handles.mets,            event,  tokens->mets);
      if  (VEC_CONTAINS  (objectsToGet,  "muons")           &&  collections.exists  ("muons"))           getCollection  (collections.getParameter<edm::InputTag>  ("muons"),           handles.muons,           event,  tokens->muons);
      if  (VEC_CONTAINS  (objectsToGet,  "photons")         &&  collections.exists  ("photons"))         getCollection  (collections.getParameter<edm::InputTag>  ("photons"),         handles.photons,         event,  tokens->photons);
      if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")  &&  collections.exists  ("primaryvertexs"))  getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),  handles.primaryvertexs,  event,  tokens->primaryvertexs);
      if  (VEC_CONTAINS  (objectsToGet,  "superclusters")   &&  collections.exists  ("superclusters"))   getCollection  (collections.getParameter<edm::InputTag>  ("superclusters"),   handles.superclusters,   event,  tokens->superclusters);
      if  (VEC_CONTAINS  (objectsToGet,  "taus")            &&  collections.exists  ("taus"))            getCollection  (collections.getParameter<edm::InputTag>  ("taus"),            handles.taus,            event,  tokens->taus);
      if  (VEC_CONTAINS  (objectsToGet,  "tracks")          &&  collections.exists  ("tracks"))          getCollection  (collections.getParameter<edm::InputTag>  ("tracks"),          handles.tracks,          event,  tokens->tracks);
      if  (VEC_CONTAINS  (objectsToGet,  "triggers")        &&  collections.exists  ("triggers"))        getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),        handles.triggers,        event,  tokens->triggers);
      if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")        &&  collections.exists  ("trigobjs"))        getCollection  (collections.getParameter<edm::InputTag>  ("trigobjs"),        handles.trigobjs,        event,  tokens->trigobjs);
    }
  else
    {
      if  (VEC_CONTAINS  (objectsToGet,  "beamspots")       &&  collections.exists  ("beamspots"))       getCollection  (collections.getParameter<edm::InputTag>  ("beamspots"),       handles.beamspots,       event);
      if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")         &&  collections.exists  ("bxlumis"))         getCollection  (collections.getParameter<edm::InputTag>  ("bxlumis"),         handles.bxlumis,         event);
      if  (VEC_CONTAINS  (objectsToGet,  "electrons")       &&  collections.exists  ("electrons"))       getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),       handles.electrons,       event);
      if  (VEC_CONTAINS  (objectsToGet,  "events")          &&  collections.exists  ("events"))          getCollection  (collections.getParameter<edm::InputTag>  ("events"),          handles.events,          event);
      if  (VEC_CONTAINS  (objectsToGet,  "genjets")         &&  collections.exists  ("genjets"))         getCollection  (collections.getParameter<edm::InputTag>  ("genjets"),         handles.genjets,         event);
      if  (VEC_CONTAINS  (objectsToGet,  "jets")            &&  collections.exists  ("jets"))            getCollection  (collections.getParameter<edm::InputTag>  ("jets"),            handles.jets,            event);
      if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")     &&  collections.exists  ("mcparticles"))     getCollection  (collections.getParameter<edm::InputTag>  ("mcparticles"),     handles.mcparticles,     event);
      if  (VEC_CONTAINS  (objectsToGet,  "mets")            &&  collections.exists  ("mets"))            getCollection  (collections.getParameter<edm::InputTag>  ("mets"),            handles.mets,            event);
      if  (VEC_CONTAINS  (objectsToGet,  "muons")           &&  collections.exists  ("muons"))           getCollection  (collections.getParameter<edm::InputTag>  ("muons"),           handles.muons,           event);
      if  (VEC_CONTAINS  (objectsToGet,  "photons")         &&  collections.exists  ("photons"))         getCollection  (collections.getParameter<edm::InputTag>  ("photons"),         handles.photons,         event);
      if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")  &&  collections.exists  ("primaryvertexs"))  getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),  handles.primaryvertexs,  event);
      if  (VEC_CONTAINS  (objectsToGet,  "superclusters")   &&  collections.exists  ("superclusters"))   getCollection  (collections.getParameter<edm::InputTag>  ("superclusters"),   handles.superclusters,   event);
      if  (VEC_CONTAINS  (objectsToGet,  "taus")            &&  collections.exists  ("taus"))            getCollection  (collections.getParameter<edm::InputTag>  ("taus"),            handles.taus,            event);
      if  (VEC_CONTAINS  (objectsToGet,  "tracks")          &&  collections.exists  ("tracks"))          getCollection  (collections.getParameter<edm::InputTag>  ("tracks"),          handles.tracks,          event);
      if  (VEC_CONTAINS  (objectsToGet,  "triggers")        &&  collections.exists  ("triggers"))        getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),        handles.triggers,        event);
      if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")        &&  collections.exists  ("trigobjs"))        getCollection  (collections.getParameter<edm::InputTag>  ("trigobjs"),        handles.trigobjs,        event);
    }

  if  (VEC_CONTAINS  (objectsToGet,  "userVariables")   &&  collections.exists  ("userVariables"))
    {
      handles.uservariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> >  ("uservariables"))
        {
          handles.uservariables.resize (handles.uservariables.size () + 1);
          getCollection (collection, handles.uservariables.back (), event);
        }
    }

  if (firstEvent)
    clog << "Will print any collections not retrieved.  These INFO messages may be safely ignored." << endl;
  if (firstEvent && !handles.beamspots.isValid ())
    clog << "INFO: did not retrieve beamspots collection from the event." << endl;
  if (firstEvent && !handles.bxlumis.isValid ())
    clog << "INFO: did not retrieve bxlumis collection from the event." << endl;
  if (firstEvent && !handles.electrons.isValid ())
    clog << "INFO: did not retrieve electrons collection from the event." << endl;
  if (firstEvent && !handles.events.isValid ())
    clog << "INFO: did not retrieve events collection from the event." << endl;
  if (firstEvent && !handles.genjets.isValid ())
    clog << "INFO: did not retrieve genjets collection from the event." << endl;
  if (firstEvent && !handles.jets.isValid ())
    clog << "INFO: did not retrieve jets collection from the event." << endl;
  if (firstEvent && !handles.mcparticles.isValid ())
    clog << "INFO: did not retrieve mcparticles collection from the event." << endl;
  if (firstEvent && !handles.mets.isValid ())
    clog << "INFO: did not retrieve mets collection from the event." << endl;
  if (firstEvent && !handles.muons.isValid ())
    clog << "INFO: did not retrieve muons collection from the event." << endl;
  if (firstEvent && !handles.photons.isValid ())
    clog << "INFO: did not retrieve photons collection from the event." << endl;
  if (firstEvent && !handles.primaryvertexs.isValid ())
    clog << "INFO: did not retrieve primaryvertexs collection from the event." << endl;
  if (firstEvent && !handles.superclusters.isValid ())
    clog << "INFO: did not retrieve superclusters collection from the event." << endl;
  if (firstEvent && !handles.taus.isValid ())
    clog << "INFO: did not retrieve taus collection from the event." << endl;
  if (firstEvent && !handles.tracks.isValid ())
    clog << "INFO: did not retrieve tracks collection from the event." << endl;
  if (firstEvent && !handles.triggers.isValid ())
    clog << "INFO: did not retrieve triggers collection from the event." << endl;
  if (firstEvent && !handles.trigobjs.isValid ())
    clog << "INFO: did not retrieve trigobjs collection from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

  firstEvent = false;
}

#ifdef ROOT6
  #include "FWCore/Utilities/interface/BaseWithDict.h"
  #include "FWCore/Utilities/interface/FunctionWithDict.h"
  #include "FWCore/Utilities/interface/MemberWithDict.h"
  #include "FWCore/Utilities/interface/ObjectWithDict.h"
  #include "FWCore/Utilities/interface/TypeWithDict.h"

  double
  anatools::getMember (const string &type, const void * const obj, const string &member)
  {
    double value = numeric_limits<int>::min ();
    edm::TypeWithDict t = edm::TypeWithDict::byName (type);
    edm::ObjectWithDict *o = new edm::ObjectWithDict (t, (void *) obj);
    const edm::MemberWithDict &dataMember = t.dataMemberByName (member);
    const edm::FunctionWithDict &functionMember = t.functionMemberByName (member);
    string dataMemberType = "", functionMemberType = "";

    if (dataMember)
      dataMemberType = dataMember.typeOf ().name ();
    if (functionMember)
      functionMemberType = functionMember.finalReturnType ().name ();
    try
      {
        if (dataMemberType != "")
          {
            if (dataMemberType == "float")
              value = dataMember.get (*o).objectCast<float> ();
            else if (dataMemberType == "double")
              value = dataMember.get (*o).objectCast<double> ();
            else if (dataMemberType == "long double")
              value = dataMember.get (*o).objectCast<long double> ();
            else if (dataMemberType == "char")
              value = dataMember.get (*o).objectCast<char> ();
            else if (dataMemberType == "int")
              value = dataMember.get (*o).objectCast<int> ();
            else if (dataMemberType == "unsigned")
              value = dataMember.get (*o).objectCast<unsigned> ();
            else if (dataMemberType == "bool")
              value = dataMember.get (*o).objectCast<bool> ();
            else if (dataMemberType == "unsigned int")
              value = dataMember.get (*o).objectCast<unsigned int> ();
            else
              clog << "WARNING: \"" << member << "\" has unrecognized type \"" << dataMemberType << "\"" << endl;
          }
        else if (functionMemberType != "")
          {
            if (functionMemberType == "float")
              value = invoke<float> (functionMemberType, o, functionMember);
            else if (functionMemberType == "double")
              value = invoke<double> (functionMemberType, o, functionMember);
            else if (functionMemberType == "long double")
              value = invoke<long double> (functionMemberType, o, functionMember);
            else if (functionMemberType == "char")
              value = invoke<char> (functionMemberType, o, functionMember);
            else if (functionMemberType == "int")
              value = invoke<int> (functionMemberType, o, functionMember);
            else if (functionMemberType == "unsigned")
              value = invoke<unsigned> (functionMemberType, o, functionMember);
            else if (functionMemberType == "bool")
              value = invoke<bool> (functionMemberType, o, functionMember);
            else if (functionMemberType == "unsigned int")
              value = invoke<unsigned int> (functionMemberType, o, functionMember);
            else
              clog << "WARNING: \"" << member << "()\" has unrecognized return type \"" << functionMemberType << "\"" << endl;
          }
        else
          throw 0;
      }
    catch (...)
      {
        bool found = false;
        for (const auto &bi : edm::TypeBases (t))
          {
            edm::BaseWithDict baseClass (bi);
            string baseName = baseClass.name ();
            try
              {
                value = getMember (baseName, obj, member);
                found = true;
                break;
              }
            catch (...)
              {
                continue;
              }
          }
        if (!found)
          throw;
      }
    delete o;

    return value;
  }

  template<class T> T
  anatools::invoke (const string &returnType, edm::ObjectWithDict * const o, const edm::FunctionWithDict &functionMember)
  {
    T mem;
    edm::ObjectWithDict *value = new edm::ObjectWithDict (edm::TypeWithDict::byName (returnType), &mem);
    functionMember.invoke (*o, value);
    return value->objectCast<T> ();
  }
#else
  #include "Reflex/Base.h"
  #include "Reflex/Member.h"
  #include "Reflex/Object.h"
  #include "Reflex/Type.h"

  double
  anatools::getMember (const string &type, const void * const obj, const string &member)
  {
    double value = numeric_limits<int>::min ();
    Reflex::Type t = Reflex::Type::ByName (type);
    Reflex::Object *o = new Reflex::Object (t, (void *) obj);
    string memberType;

    try
      {
        const Reflex::Object &retObj = getMember (t, *o, member, memberType);
        if (memberType == "float")
          value = Reflex::Object_Cast<float> (retObj);
        else if (memberType == "double")
          value = Reflex::Object_Cast<double> (retObj);
        else if (memberType == "long double")
          value = Reflex::Object_Cast<long double> (retObj);
        else if (memberType == "char")
          value = Reflex::Object_Cast<char> (retObj);
        else if (memberType == "int")
          value = Reflex::Object_Cast<int> (retObj);
        else if (memberType == "unsigned")
          value = Reflex::Object_Cast<unsigned> (retObj);
        else if (memberType == "bool")
          value = Reflex::Object_Cast<bool> (retObj);
        else if (memberType == "unsigned int")
          value = Reflex::Object_Cast<unsigned int> (retObj);
        else
          clog << "WARNING: \"" << member << "\" has unrecognized type \"" << memberType << "\"" << endl;
      }
    catch (...)
      {
        clog << "WARNING: unable to access member \"" << member << "\" from \"" << type << "\"" << endl;
      }
    delete o;

    return value;
  }

  const Reflex::Object &
  anatools::getMember (const Reflex::Type &t, const Reflex::Object &o, const string &member, string &memberType)
  {
    string typeName = t.Name (Reflex::FINAL | Reflex::SCOPED);
    size_t dot = member.find ('.'),
           asterisk = typeName.rfind ('*');

    if (t.IsReference ())
      {
        clog << "WARNING: unable to access members which are references" << endl;
        throw 0;
      }
    if (t.IsPointer ())
      {
        Reflex::Type derefType = Reflex::Type::ByName (typeName.substr (0, asterisk) + typeName.substr (asterisk + 1));
        void *obj = o.Address ();
        Reflex::Object *derefObj = new Reflex::Object (derefType, (void *) *((void **) obj));
        return getMember (derefType, *derefObj, member, memberType);
      }
    if (dot != string::npos)
      {
        try
          {
            const Reflex::Object &subObj = getMember (t, o, member.substr (0, dot), memberType);
            Reflex::Type subType = Reflex::Type::ByName (memberType);
            string subMember = member.substr (dot + 1);
            return getMember (subType, subObj, subMember, memberType);
          }
        catch (...)
          {
            const Reflex::Object &subObj = getMember (t, o, member.substr (0, dot), memberType);
            Reflex::Type subType = Reflex::Type::ByName (memberType);
            string subMember = (member.substr (0, dot) == "operator->" ? "" : "operator->.") + member.substr (dot + 1);
            return getMember (subType, subObj, subMember, memberType);
          }
      }

    Reflex::Type dataMemberType, functionMemberType;
    string dataMemberTypeName, functionMemberTypeName;

    dataMemberType = t.DataMemberByName (member).TypeOf ();
    functionMemberType = t.FunctionMemberByName (member).TypeOf ().ReturnType ();
    dataMemberTypeName = dataMemberType.Name (Reflex::FINAL | Reflex::SCOPED);
    functionMemberTypeName = functionMemberType.Name (Reflex::FINAL | Reflex::SCOPED);
    try
      {
        if (dataMemberTypeName != "")
          {
            memberType = dataMemberTypeName;
            Reflex::Object *retObj = new Reflex::Object (o.Get (member));
            return (*retObj);
          }
        else if (functionMemberTypeName != "")
          {
            memberType = functionMemberTypeName;
            return invoke (memberType, o, member);
          }
        else
          throw 0;
      }
    catch (...)
      {
        for (auto bi = t.Base_Begin (); bi != t.Base_End (); bi++)
          {
            try
              {
                return getMember (bi->ToType (), o.CastObject (bi->ToType ()), member, memberType);
              }
            catch (...)
              {
                continue;
              }
          }
        throw;
      }

    throw 0;
  }

const Reflex::Object &
anatools::invoke (const string &returnType, const Reflex::Object &o, const string &member)
{
  Reflex::Type t = Reflex::Type::ByName (returnType);
  Reflex::Object *value = new Reflex::Object (t.Construct ());
  o.Invoke (member, value);
  return (*value);
}
#endif

#if IS_VALID(beamspots)
  string  anatools::getObjectType  (const  TYPE(beamspots)         &obj)  {  return  "beamspot";         }
  string  anatools::getObjectClass  (const  TYPE(beamspots)         &obj)  {  return  TYPE_STR(beamspots);         }
#endif
#if IS_VALID(bxlumis)
  string  anatools::getObjectType  (const  TYPE(bxlumis)         &obj)  {  return  "bxlumi";         }
  string  anatools::getObjectClass  (const  TYPE(bxlumis)         &obj)  {  return  TYPE_STR(bxlumis);         }
#endif
#if IS_VALID(electrons)
  string  anatools::getObjectType  (const  TYPE(electrons)       &obj)  {  return  "electron";       }
  string  anatools::getObjectClass  (const  TYPE(electrons)       &obj)  {  return  TYPE_STR(electrons);       }
#endif
#if IS_VALID(events)
  string  anatools::getObjectType  (const  TYPE(events)          &obj)  {  return  "event";          }
  string  anatools::getObjectClass  (const  TYPE(events)          &obj)  {  return  TYPE_STR(events);          }
  template<> int anatools::getObjectHash<TYPE(events)> (const TYPE(events) &object){
    int run, lumi, evt;
    run = abs(int(getMember (object, "run")));
    lumi = abs(int(getMember (object, "lumi")));
    evt = abs(int(getMember (object, "evt")));
    return run + lumi + evt;
  }
#endif
#if IS_VALID(genjets)
  string  anatools::getObjectType  (const  TYPE(genjets)         &obj)  {  return  "genjet";         }
  string  anatools::getObjectClass  (const  TYPE(genjets)         &obj)  {  return  TYPE_STR(genjets);         }
#endif
#if IS_VALID(jets)
  string  anatools::getObjectType  (const  TYPE(jets)            &obj)  {  return  "jet";            }
  string  anatools::getObjectClass  (const  TYPE(jets)            &obj)  {  return  TYPE_STR(jets);            }
#endif
#if IS_VALID(mcparticles)
  string  anatools::getObjectType  (const  TYPE(mcparticles)     &obj)  {  return  "mcparticle";     }
  string  anatools::getObjectClass  (const  TYPE(mcparticles)     &obj)  {  return  TYPE_STR(mcparticles);     }
#endif
#if IS_VALID(mets)
  string  anatools::getObjectType  (const  TYPE(mets)            &obj)  {  return  "met";            }
  string  anatools::getObjectClass  (const  TYPE(mets)            &obj)  {  return  TYPE_STR(mets);            }
  template<> int anatools::getObjectHash<TYPE(mets)> (const TYPE(mets)& object){
    int px_mev, py_mev;
    px_mev = fabs(int(10000 * getMember (object, "px")));
    py_mev = fabs(int(10000 * getMember (object, "py")));
    return px_mev + py_mev;
  }
#endif
#if IS_VALID(muons)
  string  anatools::getObjectType  (const  TYPE(muons)           &obj)  {  return  "muon";           }
  string  anatools::getObjectClass  (const  TYPE(muons)           &obj)  {  return  TYPE_STR(muons);           }
#endif
#if IS_VALID(photons)
  string  anatools::getObjectType  (const  TYPE(photons)         &obj)  {  return  "photon";         }
  string  anatools::getObjectClass  (const  TYPE(photons)         &obj)  {  return  TYPE_STR(photons);         }
#endif
#if IS_VALID(primaryvertexs)
  string  anatools::getObjectType  (const  TYPE(primaryvertexs)  &obj)  {  return  "primaryvertex";  }
  string  anatools::getObjectClass  (const  TYPE(primaryvertexs)  &obj)  {  return  TYPE_STR(primaryvertexs);  }
  template<> int anatools::getObjectHash<TYPE(primaryvertexs)> (const TYPE(primaryvertexs)& object){
    int x_mum, y_mum, z_mum;
    x_mum = abs(int(10000 * getMember (object, "x")));
    y_mum = abs(int(10000 * getMember (object, "y")));
    z_mum = abs(int(10000 * getMember (object, "z")));
    return x_mum + y_mum + z_mum;
  }
#endif
#if IS_VALID(superclusters)
  string  anatools::getObjectType  (const  TYPE(superclusters)   &obj)  {  return  "supercluster";   }
  string  anatools::getObjectClass  (const  TYPE(superclusters)   &obj)  {  return  TYPE_STR(superclusters);   }
#endif
#if IS_VALID(taus)
  string  anatools::getObjectType  (const  TYPE(taus)            &obj)  {  return  "tau";            }
  string  anatools::getObjectClass  (const  TYPE(taus)            &obj)  {  return  TYPE_STR(taus);            }
#endif
#if IS_VALID(tracks)
  string  anatools::getObjectType  (const  TYPE(tracks)          &obj)  {  return  "track";          }
  string  anatools::getObjectClass  (const  TYPE(tracks)          &obj)  {  return  TYPE_STR(tracks);          }
#endif
#if IS_VALID(trigobjs)
  string  anatools::getObjectType  (const  TYPE(trigobjs)        &obj)  {  return  "trigobj";        }
  string  anatools::getObjectClass  (const  TYPE(trigobjs)        &obj)  {  return  TYPE_STR(trigobjs);        }
#endif

// user-defined cases
string anatools::getObjectType (const VariableProducerPayload& obj){ return "uservariable"; }
string anatools::getObjectClass (const VariableProducerPayload& obj){ return "VariableProducerPayload"; }

void
anatools::getAllTokens (const edm::ParameterSet &collections, edm::ConsumesCollector &&cc, Tokens &tokens)
{
  if (collections.exists ("bxlumis"))
    tokens.bxlumis = cc.consumes<vector<TYPE(bxlumis)> > (collections.getParameter<edm::InputTag> ("bxlumis"));
  if (collections.exists ("electrons"))
    tokens.electrons = cc.consumes<vector<TYPE(electrons)> > (collections.getParameter<edm::InputTag> ("electrons"));
  if (collections.exists ("events"))
    tokens.events = cc.consumes<vector<TYPE(events)> > (collections.getParameter<edm::InputTag> ("events"));
  if (collections.exists ("genjets"))
    tokens.genjets = cc.consumes<vector<TYPE(genjets)> > (collections.getParameter<edm::InputTag> ("genjets"));
  if (collections.exists ("jets"))
    tokens.jets = cc.consumes<vector<TYPE(jets)> > (collections.getParameter<edm::InputTag> ("jets"));
  if (collections.exists ("mcparticles"))
    tokens.mcparticles = cc.consumes<vector<TYPE(mcparticles)> > (collections.getParameter<edm::InputTag> ("mcparticles"));
  if (collections.exists ("mets"))
    tokens.mets = cc.consumes<vector<TYPE(mets)> > (collections.getParameter<edm::InputTag> ("mets"));
  if (collections.exists ("muons"))
    tokens.muons = cc.consumes<vector<TYPE(muons)> > (collections.getParameter<edm::InputTag> ("muons"));
  if (collections.exists ("photons"))
    tokens.photons = cc.consumes<vector<TYPE(photons)> > (collections.getParameter<edm::InputTag> ("photons"));
  if (collections.exists ("primaryvertexs"))
    tokens.primaryvertexs = cc.consumes<vector<TYPE(primaryvertexs)> > (collections.getParameter<edm::InputTag> ("primaryvertexs"));
  if (collections.exists ("superclusters"))
    tokens.superclusters = cc.consumes<vector<TYPE(superclusters)> > (collections.getParameter<edm::InputTag> ("superclusters"));
  if (collections.exists ("taus"))
    tokens.taus = cc.consumes<vector<TYPE(taus)> > (collections.getParameter<edm::InputTag> ("taus"));
  if (collections.exists ("tracks"))
    tokens.tracks = cc.consumes<vector<TYPE(tracks)> > (collections.getParameter<edm::InputTag> ("tracks"));
  if (collections.exists ("trigobjs"))
    tokens.trigobjs = cc.consumes<vector<TYPE(trigobjs)> > (collections.getParameter<edm::InputTag> ("trigobjs"));
  if (collections.exists ("triggers"))
    tokens.triggers = cc.consumes<TYPE(triggers)> (collections.getParameter<edm::InputTag> ("triggers"));
}
