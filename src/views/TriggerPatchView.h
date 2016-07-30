#ifndef VIEWS_TRIGGERPATCHVIEW_H_
#define VIEWS_TRIGGERPATCHVIEW_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include "View.h"

class TriggerPatchView : public View {
public:
  TriggerPatchView(const std::string& patchname, const std::string& patchtype);
	virtual ~TriggerPatchView();
};

#endif
