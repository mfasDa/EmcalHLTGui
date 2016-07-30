#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include <string>
#include <vector>
#include <TString.h>
#include <TH1.h>
#include "HistogramHandler.h"
#include "Synchronized.h"
#include "shared_ptr.h"

class TH1;
class TObject;

class DataHandler : public Synchronized {
	int 							fRunNumber;
	int 							fNumberOfEvents;
	std::string						fHLTmode;
	HistogramHandler					fData;

	// ZMQ stuff
	void* fZMQcontext;
	void* fZMQin;
	TString fZMQconfigIN;
	int fZMQsocketModeIN;

protected:
	bool DoRequest();
	void GetData();

public:
	DataHandler();
	virtual ~DataHandler();

	bool Update();
	EMCALHLTGUI::shared_ptr<TH1> FindHistogram(const std::string & histname);
	HistogramHandler &GetHistogramHandler() { return fData; }

	int GetRunNumber() const { return fRunNumber; }
	int GetNumberOfEvents() const { return fNumberOfEvents; }
	const std::string &GetHLTMode() const { return fHLTmode; }

	void Clear();
};

#endif /* DATAHANDLER_H_ */
