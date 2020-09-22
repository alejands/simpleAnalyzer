// -*- C++ -*-
//
// Package:    simpleAnalyzer/cutFlowAnalyzer
// Class:      cutFlowAnalyzer
// 
/**\class cutFlowAnalyzer cutFlowAnalyzer.cc simpleAnalyzer/cutFlowAnalyzer/plugins/cutFlowAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alejandro Sanchez
//         Created:  Mon, 14 Sep 2020 22:13:59 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class cutFlowAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
    public:
        explicit cutFlowAnalyzer(const edm::ParameterSet&);
        ~cutFlowAnalyzer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        bool runTriggerSelection(const edm::Event&, const edm::EventSetup&);
        bool runSingleMuonSelection (const edm::Event&, const edm::EventSetup&);
        // ----------member data ---------------------------

        edm::EDGetTokenT<edm::TriggerResults> triggerResultsT_;
        std::vector<int> passCut;
};

//
// constants, enums and typedefs
//
static const bool debug = true;
//
// static data member definitions
//

//
// constructors and destructor
//
cutFlowAnalyzer::cutFlowAnalyzer(const edm::ParameterSet& iConfig)

{
    //now do what ever initialization is needed
    usesResource("TFileService");

    triggerResultsT_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"));
}


cutFlowAnalyzer::~cutFlowAnalyzer()
{
 
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
cutFlowAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    // Event Count
    passCut[0]++;

    // Trigger
    if ( runTriggerSelection(iEvent, iSetup) ) passCut[1]++;

    // Lepton
    if ( runSingleMuonSelection(iEvent, iSetup) ) passCut[2]++;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
    Handle<ExampleData> pIn;
    iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
    ESHandle<SetupData> pSetup;
    iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
cutFlowAnalyzer::beginJob()
{
    passCut.push_back(0); // total Events
    passCut.push_back(0); // trigger
    passCut.push_back(0); // n leptons
}

// ------------ method called once each job just after ending the event loop  ------------
void 
cutFlowAnalyzer::endJob() 
{
    std::cout << "N total       : " <<  passCut[0] << std::endl;
    std::cout << "N pass trigger: " <<  passCut[1] << std::endl;
    std::cout << "N single muon : " <<  passCut[2] << std::endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
cutFlowAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}


bool cutFlowAnalyzer::runTriggerSelection(const edm::Event& iEvent, const edm::EventSetup& iSetup){

    edm::Handle<edm::TriggerResults> triggers;
    iEvent.getByToken(triggerResultsT_, triggers);

    const edm::TriggerNames &triggerNames = iEvent.triggerNames(*triggers);

    if (debug) {
        std::cout << "Num triggers: " << triggers->size() << std::endl;

        for (unsigned int iT=0; iT < triggers->size(); iT++){
            std::cout << " name["<<iT<<"]: "<<triggerNames.triggerName(iT) << std::endl;
        }
    }

    std::string singleMuTrigger = "HLT_IsoMu27_v*";

    std::vector< std::vector<std::string>::const_iterator > triggerMatches 
             = edm::regexMatch(triggerNames.triggerNames(), singleMuTrigger);
    
    if (debug) std::cout << "N matches: " << triggerMatches.size() << std::endl; 

    if ( !triggerMatches.empty() ) {
       for ( auto const &iT : triggerMatches ) {
            if (debug){
                std::cout << " Name["<<triggerNames.triggerIndex(*iT)<<"]: "<<*iT<<" -> "
                      << triggers->accept(triggerNames.triggerIndex(*iT)) << std::endl;
            }
            if ( triggers->accept(triggerNames.triggerIndex(*iT)) ) return true;
        }
    }
    else if (debug) std::cout << "No trigger matches for " << singleMuTrigger << std::endl;
    
    return false;

}

bool cutFlowAnalyzer::runSingleMuonSelection (const edm::Event& iEvent, const edm::EventSetup& iSetup){
    return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(cutFlowAnalyzer);
