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
#include <views/TriggerPatchView.h>
#include <sstream>

TriggerPatchView::TriggerPatchView(const std::string& patchname, const std::string& patchtype) :
View() {

	SetNumberOfPads(1, 2);

	std::stringstream mystream;

  mystream << "TriggerPatches_" << patchname << "_" << patchtype;
  SetName(mystream.str());

  mystream.str(std::string());
  mystream << "Trigger patches: " << patchname << ", " << patchtype;
  SetTitle(mystream.str());

  ViewPad *maxpos = new ViewPad;
  maxpos->AddOption("drawtru");
  mystream.str(std::string());
  mystream << "EMCTRQA_histMaxEdgePos" << patchname << patchtype;
  maxpos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(0, 0, maxpos);

  ViewPad *amppos = new ViewPad;
  amppos->AddOption("drawtru");
  mystream.str(std::string());
  mystream << "EMCTRQA_histAmpEdgePos" << patchname << patchtype;
  amppos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(0, 1, amppos);
}

TriggerPatchView::~TriggerPatchView() {
}

