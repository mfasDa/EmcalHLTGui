APPSRC=src/main.cxx

DEPSRC:=src/ViewHandler.cxx \
		src/View.cxx \
		src/EmcalZMQhelpers.cxx \
		src/DataHandler.cxx \
		src/Updater.cxx \
		src/EMCALHLTgui.cxx \
		src/views/FastORHitsView.cxx \
		src/views/SuperModuleView.cxx \
		src/views/TriggerPatchView.cxx
		
SRCS=$(DEPSRC) $(APPSRC)
		
HDRS=$(DEPSRC:%.cxx=%.h)

DHDRS=src/EMCALHLTguiLinkDef.h
