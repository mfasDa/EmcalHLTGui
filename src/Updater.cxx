/*
 * Updater.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
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
	std::cout << "Calling update" << std::endl;
	if(fDataHandler->Update()){
		fGui->SetRunNumber(fDataHandler->GetRunNumber());
		fGui->RedrawView();
		Reset();
		return true;
	}
	Reset();
	return false;
}
