/*
 * DataHandler.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include "zmq.h"

#include "DataHandler.h"
#include "EmcalZMQhelpers.h"

DataHandler::DataHandler() :
fRunNumber(0),
fHLTmode("B"),
fData(),
fZMQcontext(NULL),
fZMQin(NULL),
fZMQconfigIN("REQ>tcp://localhost:60211"),
fZMQsocketModeIN(-1)
{
}

DataHandler::~DataHandler() {
	Clear();
}

void DataHandler::Clear(){
	for(std::vector<TObject *>::iterator it = fData.begin(); it != fData.end(); ++it)
		delete *it;
}

bool DataHandler::DoRequest(){
    emcalzmq_msg_send("CONFIG", "*EMC*", fZMQin, ZMQ_SNDMORE);
    emcalzmq_msg_send("", "", fZMQin, 0);

    //wait for the data
    zmq_pollitem_t sockets[] = {
      { fZMQin, 0, ZMQ_POLLIN, 0 },
    };
    int rc = zmq_poll(sockets, 1, 100000);

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
      if (fZMQsocketModeIN < 0) return NULL;
      continue;
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
		fData.push_back(object);

	} //for iterator i
	emcalzmq_msg_close(&message);

}

void DataHandler::Update(){
	if(DoRequest()) GetData();
}
