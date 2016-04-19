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

	myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFastORL0", "FastOR L0 hits", 0, 5));
	myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFastORL0", "FastOR L0 hits", 6, 11));
	myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFastORL0", "FastOR L0 hits", 12, 15));
	myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFastORL0", "FastOR L0 hits", 16, 19));

  myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFastORL0Amp", "FastOR L0 amplitude", 0, 5));
  myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFastORL0Amp", "FastOR L0 amplitude", 6, 11));
  myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFastORL0Amp", "FastOR L0 amplitude", 12, 15));
  myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFastORL0Amp", "FastOR L0 amplitude", 16, 19));

  myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFEEvsTRU", "FEE vs TRU amplitude", 0, 5));
  myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFEEvsTRU", "FEE vs TRU amplitude", 6, 11));
  myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFEEvsTRU", "FEE vs TRU amplitude", 12, 15));
  myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFEEvsTRU", "FEE vs TRU amplitude", 16, 19));

  myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFastORL1", "FastOR L1 hits", 0, 5));
  myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFastORL1", "FastOR L1 hits", 6, 11));
  myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFastORL1", "FastOR L1 hits", 12, 15));
  myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFastORL1", "FastOR L1 hits", 16, 19));

  myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFastORL1Amp", "FastOR L1 amplitude", 0, 5));
  myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFastORL1Amp", "FastOR L1 amplitude", 6, 11));
  myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFastORL1Amp", "FastOR L1 amplitude", 12, 15));
  myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFastORL1Amp", "FastOR L1 amplitude", 16, 19));

  myviewhandler->InsertView(new SuperModuleView("EMCAL1", "histFEEvsSTU", "FEE vs STU amplitude", 0, 5));
  myviewhandler->InsertView(new SuperModuleView("EMCAL2", "histFEEvsSTU", "FEE vs STU amplitude", 6, 11));
  myviewhandler->InsertView(new SuperModuleView("DCAL1", "histFEEvsSTU", "FEE vs STU amplitude", 12, 15));
  myviewhandler->InsertView(new SuperModuleView("DCAL2", "histFEEvsSTU", "FEE vs STU amplitude", 16, 19));

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
