/*
 * Updater.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include "EMCALHLTgui.h"
#include "DataHandler.h"
#include "Updater.h"

Updater::Updater() :
	TTimer(),
	fGui(NULL),
	fDataHandler(NULL)
{

}

Updater::~Updater() {
}

bool Updater::Notify(){
	if(fDataHandler->Update()){
		fGui->SetRunNumber(fDataHandler->GetRunNumber());
		fGui->RedrawView();
		return true;
	}
	return false;
}
