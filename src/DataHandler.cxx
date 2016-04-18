/*
 * DataHandler.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include "zmq.h"

#include <iostream>

#include <TH1.h>
#include <TMessage.h>
#include <TObject.h>

#include "DataHandler.h"
#include "EmcalZMQhelpers.h"

DataHandler::DataHandler() :
fRunNumber(0),
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
	for(std::vector<TObject *>::iterator it = fData.begin(); it != fData.end(); ++it)
		delete *it;
}

TH1 *DataHandler::FindHistogram(const std::string &histname){
	std::cout << "Finding histo " << histname << std::endl; 
	TH1 *result = NULL;
	for(std::vector<TObject *>::iterator it = fData.begin(); it != fData.end(); ++it){
		std::cout << "Next iteration" << std::endl;
		std::cout << "Histo " << (*it)->GetName() << std::endl;
		if(std::string((*it)->GetName()) == histname){
			result = static_cast<TH1 *>(*it);
			break;
		}
	}
	if(!result) std::cout << "Not found " << histname << std::endl;
	return result;
}

bool DataHandler::DoRequest(){
    std::cout << "Sending  config request " << fZMQconfigIN << std::endl;
    fZMQsocketModeIN = emcalzmq_socket_init(fZMQin, fZMQcontext, fZMQconfigIN.Data());
    std::cout << fZMQsocketModeIN << std::endl;
    emcalzmq_msg_send("CONFIG", "select=EMC*", fZMQin, ZMQ_SNDMORE);
    emcalzmq_msg_send("", "", fZMQin, 0);

    //wait for the data
    zmq_pollitem_t sockets[] = {
      { fZMQin, 0, ZMQ_POLLIN, 0 },
    };
    int rc = zmq_poll(sockets, 1, 10000);

    if (rc==-1 && errno==ETERM)
    {
      Printf("jumping out of main loop");
      return false;
    }

    if (!(sockets[0].revents & ZMQ_POLLIN))
    {
      //server died
      Printf("connection timed out, server %s died?", fZMQconfigIN.Data());
      fZMQsocketModeIN = emcalzmq_socket_init(fZMQin, fZMQcontext, fZMQconfigIN.Data());
      std::cout << fZMQsocketModeIN << std::endl;
      if (fZMQsocketModeIN < 0) return false;
    } else {
	std::cout << "connection established" << std::endl;
    }
    return true;
}

void DataHandler::GetData(){
	Clear();

	aliZMQmsg message;
	emcalzmq_msg_recv(&message, fZMQin, 0);

	//process message, deserialize objects, puth them in the container
	for (aliZMQmsg::iterator i=message.begin(); i!=message.end(); ++i)
	{
		if (emcalzmq_msg_iter_check(i, "INFO")==0)
		{
			//check if we have a runnumber in the string
			std::string info;
			emcalzmq_msg_iter_data(i,info);
			Printf("processing INFO %s", info.c_str());

			stringMap fInfoMap = ParseParamString(info);

			fRunNumber = atoi(fInfoMap["run"].c_str());
			//fHLTmode = fInfoMap["HLTmode"];
			continue;
		}

		TObject* object;
		emcalzmq_msg_iter_data(i, object);
		printf("received object %s\n", object->GetName());
		fData.push_back(object);

	} //for iterator i
	emcalzmq_msg_close(&message);

}

bool DataHandler::Update(){
	if(DoRequest()){
		GetData();
		return true;
	}
	return false;
}
