include src/sources.mk
include ${ROOTSYS}/etc/Makefile.arch

DEPS=
OBJS=$(SRCS:%.cxx=%.o)

all: EMCALHLTgui

ROOTINCLUDE=$(shell root-config --incdir)
ROOTLIBS=$(shell root-config --libs --glibs)
ROOTEXTRALIBS=()
ROOTCFLAGS=$(shell root-config --cflags)
ALICEINCLUDE=${ALICE_ROOT}/include
ALICELIBS=-L${ALICE_ROOT}/lib -lAliHLTGlobal -lAliHLTHOMER -lHLTbase -lRAWDatabase -lSTEERBase
ZMQINLUDE=${ZMQ}/include
ZMQLIBS=-L${ZMQ}/lib -lzmq

EMCALHLTgui: $(OBJS)
	@echo "Linking" $@
	$(CXX) $^ -o $@ $(ROOTLIBS) $(ALICELIBS) $(ZMQLIBS) 
	rm src/*.o
	
%.o : %.cxx $(DEPS)
	@echo "Compiling" $< ...
	$(CXX) -c $< -o $@ $(ROOTCFLAGS) -I$(ROOTINCLUDE) -I$(ALICEINCLUDE) -I$(ZMQINLUDE)
	
clean:
	@echo "Cleanup ..."
	rm -rf src/*.o EMCALHLTgui