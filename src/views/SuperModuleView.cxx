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
#include "views/SuperModuleView.h"

#include <sstream>

SuperModuleView::SuperModuleView(const std::string &det, const std::string &histname, const std::string &histtitle, int minSM, int maxSM) :
View() {

  std::stringstream mystream;

  mystream << histname << "_" << det;
  SetName(mystream.str());

  mystream.str(std::string());
  mystream << histtitle << ": " << det;
  SetTitle(mystream.str());

  int npads = (maxSM - minSM + 1);

	SetNumberOfPads(npads/2, 2);

	for (int iSM = minSM; iSM <= maxSM; iSM++) {
	  // this will place SM in their physical position, e.g.
	  // 4 5
	  // 2 3
	  // 0 1
	  int ipad = 2 * (iSM % 2) + maxSM - iSM - 1;
	  int row = ipad % 2;
	  int col = ipad / 2;
	  ViewPad *pad = new ViewPad;
	  mystream.str(std::string());
	  mystream << "EMCTRQA_" << histname << "_SM" << iSM;
	  pad->AddDrawable(mystream.str(), "drawoption=colz");
	  SetPad(row, col, pad);
	}
}

SuperModuleView::~SuperModuleView() {
}

