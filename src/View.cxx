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
