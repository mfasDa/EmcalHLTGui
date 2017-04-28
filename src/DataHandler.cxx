/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *                                                                                      *
 *  This program is free software: you can redistribute it and/or modify                *
 *  it under the terms of the GNU General Public License as published by                *
 *  the Free Software Foundation, either version 3 of the License, or                   *
 *  (at your option) any later version.                                                 *
 *                                                                                      *
 *  This program is distributed in the hope that it will be useful,                     *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                      *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 *  GNU General Public License for more details.                                        *
 *                                                                                      *
 *  You should have received a copy of the GNU General Public License                   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.               *
 ****************************************************************************************/
#include "zmq.h"

#include <iostream>

#include <TH1.h>
#include <TMessage.h>
#include <TObject.h>
#include <TSystem.h>

#include "DataHandler.h"
#include "EmcalZMQhelpers.h"

DataHandler::DataHandler() :
Synchronized(),
fRunNumber(0),
fNumberOfEvents(0),
fHLTmode("B"),
fData(),
fZMQcontext(NULL),
fZMQin(NULL),
fZMQconfigIN("REQ>tcp://alihlt-dcsgw01.cern.ch:60422"),
fZMQsocketModeIN(-1)
{
	TMessage::EnableSchemaEvolutionForAll(kTRUE);
	fZMQcontext = zmq_ctx_new();
}

DataHandler::~DataHandler() {
	Clear();
}

void DataHandler::Clear(){
	fData.ClearOriginal();
}

EMCALHLTGUI::shared_ptr<TH1> DataHandler::FindHistogram(const std::string &histname){
	Wait();
	Lock();
	EMCALHLTGUI::shared_ptr<TH1> result = fData.FindHistogram(histname);
	if(!result) std::cerr << "Not found " << histname << std::endl;
	Unlock();
	return result;
}

bool DataHandler::DoRequest(){
    	fZMQsocketModeIN = EmcalZMQhelpers::emcalzmq_socket_init(fZMQin, fZMQcontext, fZMQconfigIN.Data());
    	EmcalZMQhelpers::emcalzmq_msg_send("CONFIG", "select=EMC*", fZMQin, ZMQ_SNDMORE);
    	EmcalZMQhelpers::emcalzmq_msg_send("", "", fZMQin, 0);

    	//wait for the data
    	zmq_pollitem_t sockets[] = {
      		{ fZMQin, 0, ZMQ_POLLIN, 0 },
    	};
    	int rc = zmq_poll(sockets, 1, 10000);

    	if (rc==-1 && errno==ETERM) {
      		Printf("jumping out of main loop");
      		return false;
    	}

    	if (!(sockets[0].revents & ZMQ_POLLIN)) {
      		//server died
      		Printf("connection timed out, server %s died?", fZMQconfigIN.Data());
      		fZMQsocketModeIN = EmcalZMQhelpers::emcalzmq_socket_init(fZMQin, fZMQcontext, fZMQconfigIN.Data());
      		std::cout << fZMQsocketModeIN << std::endl;
      		if (fZMQsocketModeIN < 0) return false;
    	}
   	return true;
}

void DataHandler::GetData(){
	std::cout << "Getting data ..." << std::endl;
	//Wait();
	// std::cout << "Process locked by GetData" << std::endl;
	//Lock();
	Clear();

	EmcalZMQhelpers::emcalZMQmsg message;
	EmcalZMQhelpers::emcalzmq_msg_recv(&message, fZMQin, 0);

	//process message, deserialize objects, puth them in the container
	for (EmcalZMQhelpers::emcalZMQmsg::iterator i=message.begin(); i!=message.end(); ++i)
	{
		if (EmcalZMQhelpers::emcalzmq_msg_iter_check_id(i, "INFO")==0)
		{
			//check if we have a runnumber in the string
			std::string info;
			EmcalZMQhelpers::emcalzmq_msg_iter_data(i,info);
			//Printf("processing INFO %s", info.c_str());

			EmcalZMQhelpers::stringMap fInfoMap = EmcalZMQhelpers::ParseParamString(info);

			fRunNumber = atoi(fInfoMap["run"].c_str());
			fHLTmode = fInfoMap["HLTmode"];
			continue;
		}

		TObject* object;
		EmcalZMQhelpers::emcalzmq_msg_iter_data(i, object);
		fData.SetHistogram(static_cast<TH1 *>(object));

		// Check how many events are in the message
		if(!TString(object->GetName()).CompareTo("EMCTRQA_histEvents")){
			TH1F *histEventCounter = (TH1F *)object;
			printf("Message contains data from %d events\n", static_cast<Int_t>(histEventCounter->GetBinContent(1)));
			fNumberOfEvents = static_cast<Int_t>(histEventCounter->GetBinContent(1));
		}

	} //for iterator i
	EmcalZMQhelpers::emcalzmq_msg_close(&message);
	std::cout << "Process Unlocked " << std::endl;
	//Unlock();
}

bool DataHandler::Update(){
	std::cout << "Update cycle started ..." << std::endl;
	Wait();
	std::cout << "Process locked by update ..." << std::endl;
	Lock();
	if(DoRequest()){
		GetData();
		fLock = false;
		return true;
	}
	Unlock();
	std::cout << "Process unlocked by update ..." << std::endl;
	return false;
}

