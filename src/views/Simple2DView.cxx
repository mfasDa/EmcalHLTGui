/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *                                                                                      *
 *  This program is free software: you can redistribute it and/or modify                *
 *  it under the terms of the GNU General Public License as published by                *
 *  the Free Software Foundation, either version 3 of the License, or                   *
 *  (at your option) any later version.                                                 *
 *                                                                                      *
 *  This program is distributed in the hope that it will be useful,                     *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                      *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 *  GNU General Public License for more details.                                        *
 *                                                                                      *
 *  You should have received a copy of the GNU General Public License                   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.               *
 ****************************************************************************************/
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

