/*
 * View.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include <sstream>

#include "View.h"

View::View() :
  fName("DefaultView"),
  fTitle("DefaultView"),
  fNRowPads(0),
  fNColPads(0),
  fPads()
{

}

View::View(const std::string &name, const std::string &title) :
	fName(name),
	fTitle(title),
	fNRowPads(0),
	fNColPads(0),
	fPads()
{

}

const ViewPad *View::GetPad(int row, int col) const {
	if(row < 0 || row >= fNRowPads) return NULL;
	if(col < 0 || row >= fNColPads) return NULL;
	std::map<std::pair<int,int>, ViewPad *>::const_iterator mypad = fPads.find(std::pair<int,int>(row,col));
	if(mypad != fPads.end()) return mypad->second;
	return NULL;
}

void View::SetPad(int row, int col, ViewPad *padDef){
	std::map<std::pair<int,int>, ViewPad *>::iterator mypad = fPads.find(std::pair<int,int>(row,col));
	if(mypad != fPads.end()){
		delete mypad->second;
		mypad->second = padDef;
	} else {
		fPads.insert(std::pair<std::pair<int,int>, ViewPad *>(std::pair<int,int>(row,col), padDef));
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
