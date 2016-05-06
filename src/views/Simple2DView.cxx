/*
 * Simple2DView.cxx
 *
 *  Created on: Apr 29, 2016
 *      Author: saiola
 */

#include "Simple2DView.h"
#include <sstream>

Simple2DView::Simple2DView(const std::string& histName, const std::string& title, const std::string& options, const std::string& padoptions) :
  View(histName, title)
{
  SetNumberOfPads(1,1);

  ViewPad *pad = new ViewPad;

  std::stringstream optionparser(padoptions);
  std::string opt;
  while(getline(optionparser, opt, ';')){
    pad->AddOption(opt);
  }

  pad->AddDrawable(histName, options);
  SetPad(0, 0, pad);
}

Simple2DView::~Simple2DView()
{
}

