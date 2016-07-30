/*
 * Updater.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include <ctime>
#include <iostream>

#include "EMCALHLTgui.h"
#include "DataHandler.h"
#include "Updater.h"

Updater::Updater(Long_t timeout) :
	TTimer(timeout),
	fGui(NULL),
	fDataHandler(NULL)
{

}

Updater::~Updater() {
}

bool Updater::Notify(){
	time_t rawtime;
	time(&rawtime);
	struct tm *emitsig = localtime(&rawtime);
	std::cout << "Notify called at time stamp " << asctime(emitsig) << std::endl;
	if(fDataHandler->Update()){
		fGui->SetRunNumber(fDataHandler->GetRunNumber());
		fGui->SetNumberOfEvents(fDataHandler->GetNumberOfEvents(), -1);
		fGui->RedrawView();
		Reset();
		return true;
	}
	Reset();
	return false;
}
