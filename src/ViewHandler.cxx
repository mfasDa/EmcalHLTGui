/*
 * ViewHandler.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include "ViewHandler.h"

ViewHandler::ViewHandler() {
	// TODO Auto-generated constructor stub

}

ViewHandler::~ViewHandler() {
	for(std::map<std::string, View *>::iterator it = fViewMap.begin(); it != fViewMap.end(); ++it){
		delete it->second;
	}
}

void ViewHandler::InsertView(const std::string &viewname, View *myview){
	std::map<std::string, View *>::iterator res = fViewMap.find(viewname);
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
