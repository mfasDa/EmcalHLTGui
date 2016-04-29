/*
 * main.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include <cstdlib>

#include <TApplication.h>
#include <views/FastORHitsView.h>
#include <views/SuperModuleView.h>
#include <views/TriggerPatchView.h>
#include <views/Simple2DView.h>

#include "EMCALHLTgui.h"
#include "ViewHandler.h"
#include "View.h"
#include "DataHandler.h"


int main(int argc, char **argv){
	TApplication theApp("emcapp", &argc, argv);
	ViewHandler *myviewhandler = new ViewHandler;
	DataHandler *mydatahandler = new DataHandler;

	// Define views
	myviewhandler->InsertView(new FastORHitsView);

  myviewhandler->InsertView(new Simple2DView("L0 amplitudes", "histFastORL0Amp", "drawoption=colz;drawtru"));
  myviewhandler->InsertView(new Simple2DView("L1 amplitudes", "histFastORL1Amp", "drawoption=colz;drawtru"));
  myviewhandler->InsertView(new Simple2DView("L0 trigger time", "histFastORL0Time", "drawoption=colz"));

  myviewhandler->InsertView(new TriggerPatchView("EMCL0", "Online"));
  myviewhandler->InsertView(new TriggerPatchView("EMCGAH", "Online"));
  myviewhandler->InsertView(new TriggerPatchView("EMCGAL", "Online"));
  myviewhandler->InsertView(new TriggerPatchView("EMCJEH", "Online"));
  myviewhandler->InsertView(new TriggerPatchView("EMCJEL", "Online"));

  myviewhandler->InsertView(new TriggerPatchView("EMCL0", "Recalc"));
  myviewhandler->InsertView(new TriggerPatchView("EMCGAH", "Recalc"));
  myviewhandler->InsertView(new TriggerPatchView("EMCJEH", "Recalc"));

  myviewhandler->InsertView(new TriggerPatchView("EMCL0", "Offline"));
  myviewhandler->InsertView(new TriggerPatchView("EMCGAH", "Offline"));
  myviewhandler->InsertView(new TriggerPatchView("EMCJEH", "Offline"));

	EMCALHLTgui gui;
	gui.SetViewHandler(myviewhandler);
	gui.SetDataHandler(mydatahandler);
	gui.StartUpdateCycle();
	theApp.Run();

	return EXIT_SUCCESS;
}
