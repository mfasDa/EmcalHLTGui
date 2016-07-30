/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *                                                                                      *
 *  This program is free software: you can redistribute it and/or modify                *
 *  it under the terms of the GNU General Public License as published by                *
 *  the Free Software Foundation, either version 3 of the License, or                   *
 *  (at your option) any later version.                                                 *
 *                                                                                      *
 *  This program is distributed in the hope that it will be useful,                     *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                      *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 *  GNU General Public License for more details.                                        *
 *                                                                                      *
 *  You should have received a copy of the GNU General Public License                   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.               *
 ****************************************************************************************/
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

  	myviewhandler->InsertView(new Simple2DView("EMCTRQA_histFastORL0Amp", "L0 amplitudes", "drawoption=colz", "drawtru"));
  	myviewhandler->InsertView(new Simple2DView("EMCTRQA_histFastORL1Amp", "L1 amplitudes", "drawoption=colz", "drawtru"));
  	myviewhandler->InsertView(new Simple2DView("EMCTRQA_histFastORL0Time", "L0 trigger time", "drawoption=colz"));

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
