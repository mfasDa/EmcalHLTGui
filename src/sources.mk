APPSRC=src/main.cxx

DEPSRC:=src/ViewHandler.cxx \
		src/View.cxx \
		src/EmcalZMQhelpers.cxx \
		src/DataHandler.cxx \
		src/Updater.cxx \
		src/EMCALHLTgui.cxx
		
SRCS=$(DEPSRC) $(APPSRC)
		
HDRS=$(DEPHDRS:%.cxx=%.h)
