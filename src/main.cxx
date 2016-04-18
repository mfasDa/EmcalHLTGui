/*
 * main.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include <cstdlib>

#include <TApplication.h>

#include "EMCALHLTgui.h"
#include "ViewHandler.h"
#include "View.h"
#include "DataHandler.h"

#include "views/BasicTriggerAmplitudeView.h"

int main(int argc, char **argv){
	TApplication theApp("emcapp", &argc, argv);
	ViewHandler *myviewhandler = new ViewHandler;
	DataHandler *mydatahandler = new DataHandler;

	// Define views
	myviewhandler->InsertView("basicamps", new BasicTriggerAmplitudeView);

	EMCALHLTgui gui;
	gui.SetViewHandler(myviewhandler);
	gui.SetDataHandler(mydatahandler);
	gui.StartUpdateCycle();
	theApp.Run();

	return EXIT_SUCCESS;
}
