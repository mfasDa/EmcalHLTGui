#ifndef VIEWS_SIMPLE2DVIEW_H_
#define VIEWS_SIMPLE2DVIEW_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include "View.h"

class Simple2DView: public View {
public:
  Simple2DView(const std::string& histName, const std::string& title, const std::string& options, const std::string& padoptions="");
  virtual ~Simple2DView();
};

#endif /* VIEWS_SIMPLE2DVIEW_H_ */
