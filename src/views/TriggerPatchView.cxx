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

	SetNumberOfPads(4);

	std::stringstream mystream;

  mystream << "TriggerPatches_" << patchname << "_" << patchtype;
  SetName(mystream.str());

  mystream.clear();
  mystream << "Trigger patches: " << patchname << ", " << patchtype;
  SetTitle(mystream.str());

	ViewPad *npatches = new ViewPad;
	npatches->AddOption("loxy");
	mystream.clear();
	mystream << "EMCTRQA_histEMCalNPatches" << patchname << patchtype;
	npatches->AddDrawable(mystream.str(), "color=blue");
  mystream.clear();
  mystream << "EMCTRQA_histDCalNPatches" << patchname << patchtype;
  npatches->AddDrawable(mystream.str(), "color=green,drawoption=same");
	SetPad(0, npatches);

  ViewPad *maxpatch = new ViewPad;
  maxpatch->AddOption("loxy");
  mystream.clear();
  mystream << "EMCTRQA_histEMCalMaxPatchAmp" << patchname << patchtype;
  npatches->AddDrawable(mystream.str(), "color=blue");
  mystream.clear();
  mystream << "EMCTRQA_histDCalMaxPatchAmp" << patchname << patchtype;
  maxpatch->AddDrawable(mystream.str(), "color=green,drawoption=same");
  SetPad(1, maxpatch);

  ViewPad *maxpos = new ViewPad;
  mystream.clear();
  mystream << "EMCTRQA_histMaxEdgePos" << patchname << patchtype;
  maxpos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(2, maxpos);

  ViewPad *amppos = new ViewPad;
  mystream.clear();
  mystream << "EMCTRQA_histAmpEdgePos" << patchname << patchtype;
  amppos->AddDrawable(mystream.str(), "drawoption=colz");
  SetPad(3, amppos);
}

TriggerPatchView::~TriggerPatchView() {
}

