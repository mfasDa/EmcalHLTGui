/*
 * TriggerPatchView.h
 *
 *  Created on: 19.04.2016
 *      Author: saiola
 */

#ifndef VIEWS_TRIGGERPATCHVIEW_H_
#define VIEWS_TRIGGERPATCHVIEW_H_

#include "View.h"

class TriggerPatchView : public View {
public:
  TriggerPatchView(const std::string& patchname, const std::string& patchtype);
	virtual ~TriggerPatchView();
};

#endif
