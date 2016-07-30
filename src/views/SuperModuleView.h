#ifndef VIEWS_SUPERMODULEVIEW_H_
#define VIEWS_SUPERMODULEVIEW_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include "View.h"

class SuperModuleView : public View {
public:
  SuperModuleView(const std::string &det, const std::string &histname, const std::string &histtitle, int minSM, int maxSM);
	virtual ~SuperModuleView();
};

#endif
