#ifndef TOWERBLOCKFORMATTER_H
#define TOWERBLOCKFORMATTER_H

#include <iostream>
#include <vector>
#include <map>

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "EventFilter/EcalDigiToRaw/interface/BlockFormatter.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"


// 
// The crystals corresponding to a given FE in a given FED
//



class TowerBlockFormatter : public BlockFormatter {
 public :

	TowerBlockFormatter(EcalDigiToRaw *es) : BlockFormatter(es) {};
	~TowerBlockFormatter();
        static const int kCardsPerTower = 5;     // Number of VFE cards per trigger tower
        void DigiToRaw(const EBDataFrame& dataframe, FEDRawData& rawdata, const EcalElectronicsMapping* TheMapping);
        void DigiToRaw(const EEDataFrame& dataframe, FEDRawData& rawdata, const EcalElectronicsMapping* TheMapping, int bx_, int lv1_) const;
	void StartEvent();
	void EndEvent(FEDRawDataCollection* productRawData);

	std::map<int, std::map<int,int> >* GetFEDorder() {return FEDorder; }

 private :
        //make a class out of these member data, and this should be the return value of StartEvent
        //pass that as input to the DigiToRaw and that will help us make it const
        std::map<int, std::map<int,int> >* FEDmap;
	std::map<int, std::map<int,int> >* FEDorder;


};



#endif


