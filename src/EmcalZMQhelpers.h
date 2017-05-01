#ifndef __EmcalHLTZMQhelpers__
#define __EmcalHLTZMQhelpers__

// a helper library for using ZMQ with ALIROOT, focussed on multipart messaging
// this lib implements the HLT specific interface, for general use cases
// see AliZMQhelpers.h
// blame: Mikolaj Krzewicki, mikolaj.krzewicki@cern.ch
// some of it might be inspired by czmq.h (http://czmq.zeromq.org)

// Copyright (C) 2015 Goethe University Frankfurt
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "EmcalZMQhelpersBase.h"
#include "AliHLTDataTypes.h"

namespace EmcalZMQhelpers
{

//this is just to implement the methods which depend on aliroot HLT definitions
//for ZMQ communication
struct EmcalHLTDataTopic : public DataTopic
{
  //ctor
  EmcalHLTDataTopic()
    : DataTopic()
  {
  }

  //copy ctor
  EmcalHLTDataTopic(const AliHLTComponentDataType& dataType)
    : DataTopic()
  {
    SetID(dataType.fID);
    SetOrigin(dataType.fOrigin);
  }

  //copy ctor
  EmcalHLTDataTopic(const AliHLTComponentBlockData& blockData)
    : DataTopic()
  {
    SetSpecification(blockData.fSpecification);
    SetID(blockData.fDataType.fID);
    SetOrigin(blockData.fDataType.fOrigin);
    if (strncmp(blockData.fDataType.fID,"ROOT",4)==0) {
      SetSerialization(kSerializationROOT);
    }
  }

  //partial (no fSpecification) copy from AliHLTComponentDataType
  EmcalHLTDataTopic& operator=( const AliHLTComponentDataType& dataType )
  {
    SetID(dataType.fID);
    SetOrigin(dataType.fOrigin);
    return *this;
  }

  //assignment from a AliHLTComponentBlockData
  EmcalHLTDataTopic& operator=( const AliHLTComponentBlockData& blockData )
  {
    SetSpecification(blockData.fSpecification);
    SetID(blockData.fDataType.fID);
    SetOrigin(blockData.fDataType.fOrigin);
    if (strncmp(blockData.fDataType.fID,"ROOT",4)==0) {
      SetSerialization(kSerializationROOT);
    }
    return *this;
  }

  bool operator==( const EmcalHLTDataTopic& dt )
  {
    bool topicMatch =  Topicncmp(dt.GetIDstr(),GetIDstr());
    return topicMatch;
  }

  bool operator==( const AliHLTComponentDataType& dataType)
  {
    AliHLTComponentDataType dt;
    memcpy(dt.fID, &fDataDescription[2], kAliHLTComponentDataTypefIDsize);
    memcpy(dt.fOrigin, &fDataOrigin, kAliHLTComponentDataTypefIDsize);
    return dt==dataType;
  }

  void Fill(AliHLTComponentDataType& dt)
  {
    memcpy( dt.fID, &fDataDescription[1], kAliHLTComponentDataTypefIDsize );
    memcpy( dt.fOrigin, &fDataOrigin, kAliHLTComponentDataTypefOriginSize );
  }

};


int emcalzmq_msg_iter_check_id(emcalZMQmsg::iterator it, const EmcalHLTDataTopic& topic);
int emcalzmq_msg_send(const EmcalHLTDataTopic& topic, TObject* object, void* socket, int flags,
                    int compression=0, emcalZMQrootStreamerInfo* streamers=NULL);
int emcalzmq_msg_send(const EmcalHLTDataTopic& topic, const std::string& data, void* socket, int flags);

}  //end namespace AliZMQhelpers

#endif

