#  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT
#  Copyright (C) 2016 The ALICE collaboration
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	rm src/views/*.o
	
%.o : %.cxx $(DEPS)
	@echo "Compiling" $< ...
	$(CXX) -c $< -o $@ $(ROOTCFLAGS) -I$(ROOTINCLUDE) -I$(ALICEINCLUDE) -I$(ZMQINLUDE) -I$(PWD) -I$(PWD)/src
	
src/G__$(PACKAGE).cxx: $(HDRS) $(DHDRS)
	@echo "Generating dictionary ..."
	$(DICTGEN) -f $@ -c $(CINTFLAGS) -I$(PWD)/src $^	
	
clean:
	@echo "Cleanup ..."
	rm -rf src/*.o EMCALHLTgui
	
tar:
	mkdir -p $(PACKAGE)src/
	cp -r src $(PACKAGE)src/
	rm -rf $(PACKAGE)src/src/*.o $(PACKAGE)src/src/G_* $(PACKAGE)src/EMCALHLTgui
	cp Makefile $(PACKAGE)src/
	tar czvf $(PACKAGE)src.tar.gz $(PACKAGE)src	
	rm -rf $(PACKAGE)src