#ifndef __EmcalZMQhelpers__
#define __EmcalZMQhelpers__

// a helper library for using ZMQ with ROOT, focussed on multipart messaging
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

#include <string>
#include <map>
#include "TString.h"
#include <Rtypes.h>

struct zmq_msg_t;
class TVirtualStreamerInfo;

namespace EmcalZMQhelpers
{
struct DataTopic;

extern void* gZMQcontext; //a global ZMQ context

//convenience typedefs:
//define a map of strings
typedef std::map<std::string,std::string> stringMap;
typedef std::pair<zmq_msg_t*, zmq_msg_t*> emcalZMQframe;
typedef std::vector<emcalZMQframe> emcalZMQmsg;
typedef std::vector<std::pair<std::string, std::string> > emcalZMQmsgStr;
typedef std::vector<std::pair<std::string, std::string> > emcalStringVec;
typedef std::vector<TVirtualStreamerInfo*> emcalZMQrootStreamerInfo;

//  Init and bind/connect a ZMQ socket using a string:
//  PUB@tcp://*:123123
//  SUB>tcp://localhost:123123,@tcp://*:454545
//  timeout is in ms, -1 is wait forever
int emcalzmq_socket_init(void*& socket, void* context, std::string config, int timeout=-1, int highWaterMark=10);
int emcalzmq_socket_close(void*& socket, int linger=0);
int emcalzmq_socket_state(void* socket);

//get the global context
void* emcalzmq_context();

// extract the socket mode from a config string
int emcalzmq_socket_type(std::string config);
int emcalzmq_socket_type(void* socket);
const char* emcalzmq_socket_name(int socketType);

//  --------------------------------------------------------------------------
//  Attach a socket to zero or more endpoints. If endpoints is not null,
//  parses as list of ZeroMQ endpoints, separated by commas, and prefixed by
//  '@' (to bind the socket) or '>' (to attach the socket - alternative: '-'). 
//  Returns 0 if all endpoints were valid, or -1 if there was a syntax error. 
//  If the endpoint does not start with '@' or '>'('-'), the serverish
//  argument defines whether it is used to bind (serverish = true)
//  or connect (serverish = false).
int emcalzmq_attach (void *self, const char *endpoints, bool serverish=false);
int emcalzmq_detach (void *self, const char *endpoints, bool serverish=false);

//general multipart messages (emcalZMQmsg)
//to access, just iterate over it.
int emcalzmq_msg_recv(emcalZMQmsg* message, void* socket, int flags);
int emcalzmq_msg_add(emcalZMQmsg* message, DataTopic* topic, TObject* object, int compression=0, emcalZMQrootStreamerInfo* streamers=NULL);
int emcalzmq_msg_add(emcalZMQmsg* message, const DataTopic* topic, const std::string& data);
int emcalzmq_msg_add(emcalZMQmsg* message, const DataTopic* topic, void* buffer, int size);
int emcalzmq_msg_add(emcalZMQmsg* message, const std::string& topic, const std::string& data);
int emcalzmq_msg_copy(emcalZMQmsg* dst, emcalZMQmsg* src);
int emcalzmq_msg_send(emcalZMQmsg* message, void* socket, int flags);
int emcalzmq_msg_close(emcalZMQmsg* message);

//ROOT streamers
int emcalzmq_msg_prepend_streamer_infos(emcalZMQmsg* message, emcalZMQrootStreamerInfo* streamers);
int emcalzmq_msg_iter_init_streamer_infos(emcalZMQmsg::iterator it);
void emcalzmq_update_streamerlist(emcalZMQrootStreamerInfo* streamers, const TObjArray* newStreamers);

//checking identity of the frame via iterator
int emcalzmq_msg_iter_check(emcalZMQmsg::iterator it, const DataTopic& topic);
int emcalzmq_msg_iter_check_id(emcalZMQmsg::iterator it, const DataTopic& topic);
int emcalzmq_msg_iter_check_id(emcalZMQmsg::iterator it, const std::string& topic);
//helpers for accessing data via iterators
int emcalzmq_msg_iter_topic(emcalZMQmsg::iterator it, std::string& topic);
int emcalzmq_msg_iter_data(emcalZMQmsg::iterator it, std::string& data);
int emcalzmq_msg_iter_topic(emcalZMQmsg::iterator it, DataTopic& topic);
int emcalzmq_msg_iter_data(emcalZMQmsg::iterator it, TObject*& object);
int emcalzmq_msg_iter_data(emcalZMQmsg::iterator it, void*& buffer, size_t& size);

//string messages, no need to close, strings are copied
int emcalzmq_msg_send(std::string topic, std::string data, void* socket, int flags);
int emcalzmq_msg_recv(emcalZMQmsgStr* message, void* socket, int flags);

//send a single block (one header + payload), ZMQ_SNDMORE should not be used
int emcalzmq_msg_send(DataTopic& topic, TObject* object, void* socket, int flags, int compression=0, emcalZMQrootStreamerInfo* streamers=NULL);
int emcalzmq_msg_send(const DataTopic& topic, const std::string& data, void* socket, int flags);

//deallocate an object - callback for ZMQ
void emcalzmq_deleteTObject(void*, void* object);
void emcalzmq_deleteTopic(void*, void* object);

const int kDataTypefIDsize = 8;
const int kDataTypefOriginSize = 4;
const int kDataTypeTopicSize = kDataTypefIDsize+kDataTypefOriginSize;

//Helper functions
bool Topicncmp(const char* topic, const char* reference, int topicSize=kDataTypeTopicSize, int referenceSize=kDataTypeTopicSize);
ULong64_t CharArr2uint32(const char* str);
ULong64_t CharArr2uint64(const char* str);

struct BaseDataTopic
{
  static const UInt_t fgkMagicNumber;
  UInt_t fMagicNumber;
  UInt_t fHeaderSize;
  UInt_t fFlags;
  UInt_t fBaseHeaderVersion;
  ULong64_t fHeaderDescription;
  ULong64_t fHeaderSerialization;
  BaseDataTopic();
  BaseDataTopic(UInt_t size, ULong64_t desc, ULong64_t seri);
  static BaseDataTopic* Get(void* buf) {
    return (*reinterpret_cast<UInt_t*>(buf)==fgkMagicNumber)?
           reinterpret_cast<BaseDataTopic*>(buf):
           NULL;
  }
};

//the data header, describes the data frame
struct DataTopic : public BaseDataTopic
{
  static const ULong64_t fgkDataTopicDescription;
  static const UInt_t fgkTopicSerialization;
  ULong64_t fDataDescription[2];
  UInt_t fDataOrigin;
  UInt_t fReserved;
  ULong64_t fDataSerialization;
  ULong64_t fSpecification;                  /// data specification of the data block
  ULong64_t fPayloadSize;

  //ctor
  DataTopic()
    : BaseDataTopic(sizeof(DataTopic), fgkDataTopicDescription, fgkTopicSerialization)
    , fDataDescription()
    , fDataOrigin(0)
    , fReserved(0)
    , fDataSerialization(0)
    , fSpecification(0)
    , fPayloadSize(0)
  {
    fDataDescription[0]=0;
    fDataDescription[1]=0;
  }

  //ctor
  DataTopic(const char* id, const char* origin, int spec )
    : BaseDataTopic(sizeof(DataTopic), fgkDataTopicDescription, fgkTopicSerialization)
    , fDataDescription()
    , fDataOrigin(0)
    , fReserved(0)
    , fDataSerialization(0)
    , fSpecification(spec)
    , fPayloadSize(0)
  {
    fDataDescription[0] = 0;
    fDataDescription[1] = CharArr2uint64(id);
    fDataOrigin = CharArr2uint32(origin);
  }

  bool operator==( const DataTopic& dt )
  {
    bool topicMatch = Topicncmp(dt.GetIDstr(),GetIDstr());
    return topicMatch;
  }

  std::string Description() const
  {
    std::string description(GetIDstr(),
                            sizeof(fDataDescription[1])+sizeof(fDataOrigin));
    description+=" spec:";
    char numstr[21];
    snprintf(numstr, 21, "%llx", fSpecification);
    description+=numstr;
    return description;
  }

  inline std::string GetOrigin() const {
    std::string origin(GetOriginStr(), sizeof(fDataOrigin));
    return origin;
  }
  inline std::string GetID() const {
    std::string id(GetIDstr(), sizeof(fDataDescription[1]));
    return id;
  }
  UInt_t GetSpecification() const {return fSpecification;}
  inline const ULong64_t* GetIDptr() const {return &fDataDescription[1];}
  inline const char* GetIDstr() const {return reinterpret_cast<const char*>(GetIDptr());}
  inline const UInt_t* GetOriginPtr() const {return &fDataOrigin;}
  inline const char* GetOriginStr() const {return reinterpret_cast<const char*>(GetOriginPtr());}
  inline void SetID(ULong64_t id) {fDataDescription[1]=id;}
  inline void SetOrigin(UInt_t origin) {fDataOrigin = origin;}
  inline void SetID(const char* s) {fDataDescription[1]=*reinterpret_cast<const ULong64_t*>(s);}
  inline void SetOrigin(const char* s) {fDataOrigin = *reinterpret_cast<const UInt_t*>(s);}
  inline void SetSpecification(UInt_t spec) {fSpecification=spec;}
  inline void SetSerialization(ULong64_t s) {fDataSerialization=s;}
  static DataTopic* Get(void* buf) {
    BaseDataTopic* bdt = BaseDataTopic::Get(buf);
    return (bdt && bdt->fHeaderDescription==fgkDataTopicDescription)?
            reinterpret_cast<DataTopic*>(buf):NULL;
  }
};

//common data type definitions, compatible with AliHLTDataTypes v25
const DataTopic kDataTypeStreamerInfos("ROOTSTRI","***\n",0);
const DataTopic kDataTypeInfo("INFO____","***\n",0);
const DataTopic kDataTypeConfig("CONFIG__","***\n",0);
const DataTopic kDataTypeTObject("ROOTTOBJ","***\n",0);
const DataTopic kDataTypeTH1("ROOTHIST","***\n",0);

extern const ULong64_t kSerializationROOT;

//a general utility to tokenize strings
std::vector<std::string> TokenizeString(const std::string input, const std::string delimiters);
//parse 
stringMap ParseParamString(const std::string paramString);
std::string GetParamString(const std::string param, const std::string paramstring);

//load ROOT libraries specified in comma separated string
int LoadROOTlibs(std::string libstring, bool verbose=false);

//helper function to print a hex/ASCII dump of some memory
void hexDump (const char* desc, void* addr, int len);

//______________________________________________________________________________
inline ULong64_t CharArr2uint64(const char* str)
{
	return((ULong64_t) str[0] |
         (str[0] ? ((ULong64_t) str[1] << 8 |
         (str[1] ? ((ULong64_t) str[2] << 16 |
         (str[2] ? ((ULong64_t) str[3] << 24 |
         (str[3] ? ((ULong64_t) str[4] << 32 |
         (str[4] ? ((ULong64_t) str[5] << 40 |
         (str[5] ? ((ULong64_t) str[6] << 48 |
         (str[6] ? ((ULong64_t) str[7] << 56 )
          : 0)) : 0)) : 0)) : 0)) : 0)) : 0)) : 0));
}

inline ULong64_t CharArr2uint32(const char* str)
{
	return((UInt_t) str[0] |
         (str[0] ? ((UInt_t) str[1] << 8 |
         (str[1] ? ((UInt_t) str[2] << 16 |
         (str[2] ? ((UInt_t) str[3] << 24)
          : 0)) : 0)) : 0));
}

}  //end namespace EmcalZMQhelpers

#endif

