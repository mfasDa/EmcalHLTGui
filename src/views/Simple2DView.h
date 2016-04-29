/*
 * Simple2DView.h
 *
 *  Created on: Apr 29, 2016
 *      Author: saiola
 */

#ifndef VIEWS_SIMPLE2DVIEW_H_
#define VIEWS_SIMPLE2DVIEW_H_

#include "View.h"

class Simple2DView: public View {
public:
  Simple2DView(const std::string& histName, const std::string& title);
  virtual ~Simple2DView();
};

#endif /* VIEWS_SIMPLE2DVIEW_H_ */
