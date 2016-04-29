/*
 * TriggerPatchView.cxx
 *
 *  Created on: 19.04.2016
 *      Author: saiola
 */

#include <views/TriggerPatchView.h>
#include <sstream>

TriggerPatchView::TriggerPatchView(const std::string& patchname, const std::string& patchtype) :
View() {

	SetNumberOfPads(2);

	std::stringstream mystream;

  mystream << "TriggerPatches_" << patchname << "_" << patchtype;
  SetName(mystream.str());

  mystream.str(std::string());
  mystream << "Trigger patches: " << patchname << ", " << patchtype;
  SetTitle(mystream.str());

  ViewPad *maxpos = new ViewPad;
  mystream.str(std::string());
  mystream << "EMCTRQA_histMaxEdgePos" << patchname << patchtype;
  maxpos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(0, maxpos);

  ViewPad *amppos = new ViewPad;
  mystream.str(std::string());
  mystream << "EMCTRQA_histAmpEdgePos" << patchname << patchtype;
  amppos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(1, amppos);
}

TriggerPatchView::~TriggerPatchView() {
}

