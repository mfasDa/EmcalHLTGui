APPSRC=src/main.cxx

DEPSRC:=src/ViewHandler.cxx \
		src/View.cxx \
		src/EmcalZMQhelpersBase.cxx \
		src/EmcalZMQhelpers.cxx \
		src/Synchronized.cxx \
		src/DataHandler.cxx \
		src/HistogramHandler.cxx \
		src/Updater.cxx \
		src/EMCALHLTgui.cxx \
		src/views/FastORHitsView.cxx \
		src/views/SuperModuleView.cxx \
		src/views/TriggerPatchView.cxx \
		src/views/Simple2DView.cxx
		
SRCS=$(DEPSRC) $(APPSRC)
		
HDRS=$(DEPSRC:%.cxx=%.h)

DHDRS=src/EMCALHLTguiLinkDef.h
