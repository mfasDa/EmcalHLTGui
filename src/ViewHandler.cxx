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
#include "ViewHandler.h"
#include <iostream>

ViewHandler::ViewHandler() {

}

ViewHandler::~ViewHandler() {
	for(std::map<std::string, View *>::iterator it = fViewMap.begin(); it != fViewMap.end(); ++it){
		delete it->second;
	}
}

void ViewHandler::InsertView(View *myview){
	std::map<std::string, View *>::iterator res = fViewMap.find(myview->GetName());
	if(res != fViewMap.end()){
		delete res->second;
		res->second = myview;
	} else{
		fViewMap.insert(std::pair<std::string, View *>(myview->GetName(), myview));
	}
}

const View *ViewHandler::FindView(const std::string &name) const {
	const View *result = NULL;
	std::map<std::string, View *>::const_iterator found = fViewMap.find(name);
	if(found != fViewMap.end()) result = found->second;
	return result;
}
