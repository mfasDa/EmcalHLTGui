#ifndef VIEWHANDLER_H_
#define VIEWHANDLER_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include <string>
#include <map>
#include "View.h"

class ViewHandler {
	std::map<std::string, View *> 				fViewMap;

public:
	ViewHandler();
	virtual ~ViewHandler();

	void InsertView(View *myview);
	const View *FindView(const std::string &name) const;
	const std::map<std::string, View *> GetListOfViews() const { return fViewMap; }
	int GetNumberOfViews() const { return fViewMap.size(); }
};

#endif /* VIEWHANDLER_H_ */
