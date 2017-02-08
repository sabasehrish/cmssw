/** \file
 *  \author A. Tumanov - Rice
 */

#include "EventFilter/CSCRawToDigi/src/CSCDigiToRawModule.h"
#include "EventFilter/CSCRawToDigi/src/CSCDigiToRaw.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "CondFormats/DataRecord/interface/CSCChamberMapRcd.h"

CSCDigiToRawModule::CSCDigiToRawModule(const edm::ParameterSet & pset) 
  :local_info(pset, 2005, false, true)
{
  //theStrip = pset.getUntrackedParameter<string>("DigiCreator", "cscunpacker");
  
  theFormatVersion =  pset.getParameter<unsigned int>("useFormatVersion"); 	// pre-LS1 - '2005'. post-LS1 - '2013'
  usePreTriggers = pset.getParameter<bool>("usePreTriggers"); 			// disable checking CLCT PreTriggers digis
  packEverything_ = pset.getParameter<bool>("packEverything");                  // don't check for consistency with trig primitives
                                                                                // overrides usePreTriggers

  wd_token = consumes<CSCWireDigiCollection>( pset.getParameter<edm::InputTag>("wireDigiTag") );
  sd_token = consumes<CSCStripDigiCollection>( pset.getParameter<edm::InputTag>("stripDigiTag") );
  cd_token = consumes<CSCComparatorDigiCollection>( pset.getParameter<edm::InputTag>("comparatorDigiTag") );
  pr_token = consumes<CSCCLCTPreTriggerCollection>( pset.getParameter<edm::InputTag>("preTriggerTag") );
  al_token = consumes<CSCALCTDigiCollection>( pset.getParameter<edm::InputTag>("alctDigiTag") );
  cl_token = consumes<CSCCLCTDigiCollection>( pset.getParameter<edm::InputTag>("clctDigiTag") );
  co_token = consumes<CSCCorrelatedLCTDigiCollection>( pset.getParameter<edm::InputTag>("correlatedLCTDigiTag") );

  produces<FEDRawDataCollection>("CSCRawData"); 

}


CSCDigiToRawModule::~CSCDigiToRawModule(){
 // delete packer;
}

void CSCDigiToRawModule::fillDescriptions(edm::ConfigurationDescriptions & descriptions) {
/*** From python/cscPacker_cfi.py
    wireDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi"),
    stripDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigi"),
    comparatorDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"),
    alctDigiTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
    clctDigiTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
    preTriggerTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
    correlatedLCTDigiTag = cms.InputTag("simCscTriggerPrimitiveDigis", "MPCSORTED"),
    # if min parameter = -999 always accept
    alctWindowMin = cms.int32(-3),
    alctWindowMax = cms.int32(3),
    clctWindowMin = cms.int32(-3),
    clctWindowMax = cms.int32(3),
    preTriggerWindowMin = cms.int32(-3),
    preTriggerWindowMax = cms.int32(1)
*/

  edm::ParameterSetDescription desc;

  desc.add<unsigned int>("useFormatVersion",2005)->
  setComment("Set to 2005 for pre-LS1 CSC data format, 2013 - new post-LS1 CSC data format");
  desc.add<bool>("usePreTriggers", true)->
  setComment("Set to false if CSCCLCTPreTrigger digis are not available");
  desc.add<bool>("packEverything", false)->
  setComment("Set to true to disable trigger-related constraints on readout data");

  desc.add<edm::InputTag>("wireDigiTag", edm::InputTag("simMuonCSCDigis","MuonCSCWireDigi"));
  desc.add<edm::InputTag>("stripDigiTag",edm::InputTag("simMuonCSCDigis","MuonCSCStripDigi"));
  desc.add<edm::InputTag>("comparatorDigiTag", edm::InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"));
  desc.add<edm::InputTag>("alctDigiTag",edm::InputTag("simCscTriggerPrimitiveDigis"));
  desc.add<edm::InputTag>("clctDigiTag",edm::InputTag("simCscTriggerPrimitiveDigis"));
  desc.add<edm::InputTag>("preTriggerTag", edm::InputTag("simCscTriggerPrimitiveDigis"));
  desc.add<edm::InputTag>("correlatedLCTDigiTag",edm::InputTag("simCscTriggerPrimitiveDigis", "MPCSORTED"));

  desc.add<int32_t>("alctWindowMin", -3)->
  setComment("If min parameter = -999 always accept");
  desc.add<int32_t>("alctWindowMax",  3);
  desc.add<int32_t>("clctWindowMin", -3)->
  setComment("If min parameter = -999 always accept");
  desc.add<int32_t>("clctWindowMax",  3);
  desc.add<int32_t>("preTriggerWindowMin", -3)->
  setComment("If min parameter = -999 always accept");
  desc.add<int32_t>("preTriggerWindowMax",  1);

  descriptions.add("cscPacker", desc);
}


void CSCDigiToRawModule::produce( edm::StreamID, edm::Event & e, const edm::EventSetup& c ) const{
  ///reverse mapping for packer
  //local_CSCDigiToRaw local_info;
  //local_info.  
 //create a local packer object here: 
  CSCDigiToRaw *packer = new CSCDigiToRaw(local_info); 
  edm::ESHandle<CSCChamberMap> hcham;
  c.get<CSCChamberMapRcd>().get(hcham); 
  const CSCChamberMap* theMapping = hcham.product();

  auto fed_buffers = std::make_unique<FEDRawDataCollection>();

  // Take digis from the event
  edm::Handle<CSCWireDigiCollection> wireDigis;
  edm::Handle<CSCStripDigiCollection> stripDigis;
  edm::Handle<CSCComparatorDigiCollection> comparatorDigis;
  edm::Handle<CSCALCTDigiCollection> alctDigis;
  edm::Handle<CSCCLCTDigiCollection> clctDigis;
  edm::Handle<CSCCLCTPreTriggerCollection> preTriggers;
  edm::Handle<CSCCorrelatedLCTDigiCollection> correlatedLCTDigis;

  e.getByToken( wd_token, wireDigis );
  e.getByToken( sd_token, stripDigis );
  e.getByToken( cd_token, comparatorDigis );
  e.getByToken( al_token, alctDigis );
  e.getByToken( cl_token, clctDigis );
  if (usePreTriggers)
     e.getByToken( pr_token, preTriggers );
  e.getByToken( co_token, correlatedLCTDigis );

  // Create the packed data
  packer->createFedBuffers(*stripDigis, *wireDigis, *comparatorDigis, 
                           *alctDigis, *clctDigis, *preTriggers, *correlatedLCTDigis,
                           *(fed_buffers.get()), theMapping, e); //, theFormatVersion, usePreTriggers,
			  // packEverything_);
  
  // put the raw data to the event
  e.put(std::move(fed_buffers), "CSCRawData");
  //delete packer;
}


