/*
 * main.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include <cstdlib>

#include "EMCALHLTgui.h"
#include "ViewHandler.h"
#include "DataHandler.h"

int main(int argc, const char **argv){
	ViewHandler *myviewhandler = new ViewHandler;
	DataHandler *mydatahandler = new DataHandler;

	// Define views

	EMCALHLTgui gui;
	gui.SetViewHandler(myviewhandler);
	gui.SetDataHandler(mydatahandler);

	return EXIT_SUCCESS;
}
