/*
 * View.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include <sstream>

#include "View.h"

View::View(const std::string &name, const std::string &title) :
	fName(name),
	fTitle(title),
	fNPads(0),
	fPads()
{

}

const ViewPad *View::GetPad(int pad) const {
	if(pad >= fNPads) return NULL;
	std::map<int, ViewPad *>::const_iterator mypad = fPads.find(pad);
	if(mypad != fPads.end()) return mypad->second;
	return NULL;
}

void View::SetPad(int padId, ViewPad *padDef){
	std::map<int, ViewPad *>::iterator mypad = fPads.find(padId);
	if(mypad != fPads.end()){
		delete mypad->second;
		mypad->second = padDef;
	} else {
		fPads.insert(std::pair<int, ViewPad *>(padId, padDef));
	}
}

ViewPad::~ViewPad(){
	for(std::vector<ViewDrawable *>::const_iterator it = fDrawables.begin(); it != fDrawables.end(); ++it)
		delete *it;
}

void ViewPad::AddDrawable(const std::string &histname, const std::string &options){
	ViewDrawable *drawable = new ViewDrawable(histname);
	std::stringstream optionparser(options);
	std::string opt;
	while(getline(optionparser, opt, ';')){
		drawable->AddOption(opt);
	}
	fDrawables.push_back(drawable);
}
