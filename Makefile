include src/sources.mk
include ${ROOTSYS}/etc/Makefile.arch

DEPS=
OBJS=$(SRCS:%.cxx=%.o)

PACKAGE=EMCALHLTgui

all: $(PACKAGE)

ROOTINCLUDE=$(shell root-config --incdir)
ROOTLIBS=$(shell root-config --libs --glibs)
ROOTEXTRALIBS=()
ROOTCFLAGS=$(shell root-config --cflags)
ROOTMAJOR=$(shell root-config --version | cut -d . -f1)
DICTGEN=
ifeq ($(ROOTMAJOR), 6)
	DICTGEN=rootcling
else
	DICTGEN=rootcint
endif
ALICEINCLUDE=${ALICE_ROOT}/include
ALICELIBS=-L${ALICE_ROOT}/lib -lAliHLTGlobal -lAliHLTHOMER -lHLTbase -lRAWDatabase -lSTEERBase
ZMQINLUDE=${ZMQ}/include
ZMQLIBS=-L${ZMQ}/lib -lzmq

$(PACKAGE): $(OBJS) src/G__$(PACKAGE).o
	@echo "Linking" $@
	$(CXX) $^ -o $@ $(ROOTLIBS) $(ALICELIBS) $(ZMQLIBS) 
	rm src/*.o
	
%.o : %.cxx $(DEPS)
	@echo "Compiling" $< ...
	$(CXX) -c $< -o $@ $(ROOTCFLAGS) -I$(ROOTINCLUDE) -I$(ALICEINCLUDE) -I$(ZMQINLUDE) -I$(PWD)
	
src/G__$(PACKAGE).cxx: $(HDRS) $(DHDRS)
	@echo "Generating dictionary ..."
	$(DICTGEN) -f $@ -c $(CINTFLAGS) -I$(PWD) $^	
	
clean:
	@echo "Cleanup ..."
	rm -rf src/*.o EMCALHLTgui
	
tar:
	mkdir -p $(PACKAGE)src/src/
	$(foreach fl, $(SRCS), cp $(fl) $(PACKAGE)src/src/;)
	$(foreach fl, $(HDRS), cp $(fl) $(PACKAGE)src/src/;)
	$(foreach fl, $(DHDRS), cp $(fl) $(PACKAGE)src/src/;)
	cp src/sources.mk $(PACKAGE)src/src/
	cp Makefile $(PACKAGE)src/
	tar czvf $(PACKAGE)src.tar.gz $(PACKAGE)src	