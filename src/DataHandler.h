/*
 * DataHandler.h
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#ifndef DATAHANDLER_H_
#define DATAHANDLER_H_

#include <string>
#include <vector>
#include <TString.h>

class TH1;
class TObject;

class DataHandler {
	int 							fRunNumber;
	std::string						fHLTmode;
	std::vector<TObject *>			fData;

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

	void Update();
	TH1 *FindHistogram(const std::string & histname);

	int GetRunNumber() const { return fRunNumber; }
	const std::string &GetHLTMode() const { return fHLTmode; }

	void Clear();
};

#endif /* DATAHANDLER_H_ */
