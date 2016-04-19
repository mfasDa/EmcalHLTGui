/*
 * SuperModuleView.h
 *
 *  Created on: 18.04.2016
 *      Author: saiola
 */

#ifndef VIEWS_SUPERMODULEVIEW_H_
#define VIEWS_SUPERMODULEVIEW_H_

#include "View.h"

class SuperModuleView : public View {
public:
  SuperModuleView(const std::string &det, const std::string &histname, const std::string &histtitle, int minSM, int maxSM);
	virtual ~SuperModuleView();
};

#endif
