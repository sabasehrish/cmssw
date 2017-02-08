#ifndef EventFilter_CSCDigiToRawModule_h
#define EventFilter_CSCDigiToRawModule_h

/** \class CSCDigiToRawModule
 *
 *  \author A. Tumanov - Rice
 */

#include <FWCore/Framework/interface/ConsumesCollector.h>
#include <FWCore/Framework/interface/global/EDProducer.h>
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "EventFilter/CSCRawToDigi/src/CSCDigiToRaw.h"

namespace edm {
   class ConfigurationDescriptions;
}

class CSCDigiToRaw;

class CSCDigiToRawModule : public edm::global::EDProducer<> {
 public:
  /// Constructor
  CSCDigiToRawModule(const edm::ParameterSet & pset);

  /// Destructor
  virtual ~CSCDigiToRawModule();

  // Operations
  virtual void produce( edm::StreamID, edm::Event&, const edm::EventSetup& ) const override;

  // Fill parameters descriptions
  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);

 private:

  unsigned int 	theFormatVersion; // Select which version of data format to use Pre-LS1: 2005, Post-LS1: 2013
  bool		usePreTriggers;   // Select if to use Pre-Triigers CLCT digis
  bool		packEverything_;   // bypass all cuts and (pre)trigger requirements

  //change to non-pointer? 
  //CSCDigiToRaw * packer;
  local_CSCDigiToRaw local_info;
  edm::EDGetTokenT<CSCWireDigiCollection>             wd_token;
  edm::EDGetTokenT<CSCStripDigiCollection>            sd_token;
  edm::EDGetTokenT<CSCComparatorDigiCollection>       cd_token;
  edm::EDGetTokenT<CSCALCTDigiCollection>             al_token;
  edm::EDGetTokenT<CSCCLCTDigiCollection>             cl_token;
  edm::EDGetTokenT<CSCCLCTPreTriggerCollection>       pr_token;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection>    co_token;

};
#endif


