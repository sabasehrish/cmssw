#ifndef EventFilter_CSCDigiToRaw_h
#define EventFilter_CSCDigiToRaw_h

/** \class CSCDigiToRaw
 *
 *  \author A. Tumanov - Rice
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "EventFilter/CSCRawToDigi/interface/CSCEventData.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class FEDRawDataCollection;
class CSCReadoutMappingFromFile;
class CSCChamberMap;

struct local_CSCDigiToRaw {
  int alctWindowMin_;
  int alctWindowMax_;
  int clctWindowMin_;
  int clctWindowMax_;
  int preTriggerWindowMin_;
  int preTriggerWindowMax_;
  uint16_t formatVersion_;
  bool usePreTriggers_;
  bool packEverything_;
  uint16_t theFormatVersion; 
  bool usePreTriggers;
  bool packEverything;
  local_CSCDigiToRaw (const edm::ParameterSet & pset, 
                      uint16_t format_version, 
                      bool use_pre_triggers, 
                      bool packEverything) : 
    alctWindowMin_(pset.getParameter<int>("alctWindowMin")),
    alctWindowMax_(pset.getParameter<int>("alctWindowMax")),
    clctWindowMin_(pset.getParameter<int>("clctWindowMin")),
    clctWindowMax_(pset.getParameter<int>("clctWindowMax")),
    preTriggerWindowMin_(pset.getParameter<int>("preTriggerWindowMin")),
    preTriggerWindowMax_(pset.getParameter<int>("preTriggerWindowMax")),
    formatVersion_(format_version),
    usePreTriggers_(use_pre_triggers),
    packEverything_(packEverything) {}
};

class CSCDigiToRaw {
public:
  /// Constructor
  explicit CSCDigiToRaw(const local_CSCDigiToRaw info); //const edm::ParameterSet & pset, 
                       // uint16_t theFormatVersion = 2005, 
                      //  bool usePreTriggers = true,
                      //  bool packEverything = false);

  /// Take a vector of digis and fill the FEDRawDataCollection
  void createFedBuffers(const CSCStripDigiCollection& stripDigis,
			const CSCWireDigiCollection& wireDigis, 
                        const CSCComparatorDigiCollection& comparatorDigis,
                        const CSCALCTDigiCollection& alctDigis,
                        const CSCCLCTDigiCollection& clctDigis,
                        const CSCCLCTPreTriggerCollection& preTriggers,
                        const CSCCorrelatedLCTDigiCollection& correlatedLCTDigis,
			FEDRawDataCollection& fed_buffers,
		        const CSCChamberMap* theMapping, 
			edm::Event & e); //, 
			// uint16_t theFormatVersion = 2005, 
			// bool usePreTriggers = true,
			// bool packEverything = false);

private:
  void beginEvent(const CSCChamberMap* electronicsMap);

  // specialized because it reverses strip direction
  void add(const CSCStripDigiCollection& stripDigis, 
           const CSCCLCTPreTriggerCollection& preTriggers);
  void add(const CSCWireDigiCollection& wireDigis,
           const CSCALCTDigiCollection & alctDigis);
  // may require CLCTs to read out comparators.  Doesn't add CLCTs.
  void add(const CSCComparatorDigiCollection & comparatorDigis,
           const CSCCLCTDigiCollection & clctDigis);
  void add(const CSCALCTDigiCollection & alctDigis);
  void add(const CSCCLCTDigiCollection & clctDigis);
  void add(const CSCCorrelatedLCTDigiCollection & corrLCTDigis);
  /// pick out the correct data object for this chamber
  CSCEventData & findEventData(const CSCDetId & cscDetId);

  std::map<CSCDetId, CSCEventData> theChamberDataMap;
  const CSCChamberMap* theElectronicsMap;
  int alctWindowMin_;
  int alctWindowMax_;
  int clctWindowMin_;
  int clctWindowMax_;
  int preTriggerWindowMin_;
  int preTriggerWindowMax_;
  uint16_t formatVersion_;
  bool usePreTriggers_;
  bool packEverything_;
};




#endif
