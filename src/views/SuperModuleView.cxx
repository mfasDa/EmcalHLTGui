/*
 * SuperModuleView.cxx
 *
 *  Created on: 19.04.2016
 *      Author: saiola
 */

#include "SuperModuleView.h"

#include <sstream>

SuperModuleView::SuperModuleView(const std::string &det, const std::string &histname, const std::string &histtitle, int minSM, int maxSM) :
View() {

  std::stringstream mystream;

  mystream << histname << "_" << det;
  SetName(mystream.str());

  mystream.clear();
  mystream << histtitle << ": " << det;
  SetTitle(mystream.str());

  int npads = (maxSM - minSM + 1);

	SetNumberOfPads(npads);

	for (int iSM = minSM; iSM <= maxSM; iSM++) {
	  // this will place SM in their physical position, e.g.
	  // 4 5
	  // 2 3
	  // 0 1
	  int ipad = 2 * (iSM % 2) + npads - iSM - 2;
	  ViewPad *pad = new ViewPad;
	  mystream.clear();
	  mystream << "EMCTRQA_" << histname << "_SM" << iSM;
	  pad->AddDrawable(mystream.str(), "drawoption=colz");
	  SetPad(ipad, pad);
	}
}

SuperModuleView::~SuperModuleView() {
}
