/*
 * Simple2DView.cxx
 *
 *  Created on: Apr 29, 2016
 *      Author: saiola
 */

#include "Simple2DView.h"

Simple2DView::Simple2DView(const std::string& histName, const std::string& title) :
  View(histName, title)
{
  SetNumberOfPads(1);

  ViewPad *pad = new ViewPad;
  pad->AddDrawable(histName, "drawoption=colz");
  SetPad(0, pad);
}

Simple2DView::~Simple2DView()
{
}

