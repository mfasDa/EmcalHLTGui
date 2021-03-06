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
#include <views/FastORHitsView.h>

FastORHitsView::FastORHitsView() : View("FastOR_hits", "FastOR hits")
{
	SetNumberOfPads(2,2);

	ViewPad *l0amppad = new ViewPad;
	l0amppad->AddDrawable("EMCTRQA_histFastORL0", "");
	SetPad(0, 0, l0amppad);

	ViewPad *l1amppad = new ViewPad;
	l1amppad->AddDrawable("EMCTRQA_histFastORL1", "");
	SetPad(0, 1, l1amppad);

	ViewPad *l0largeamppad = new ViewPad;
	l0largeamppad->AddDrawable("EMCTRQA_histFastORL0LargeAmp", "");
	SetPad(1, 0, l0largeamppad);

	ViewPad *l1largeamppad = new ViewPad;
	l1largeamppad->AddDrawable("EMCTRQA_histFastORL1LargeAmp", "");
	SetPad(1, 1, l1largeamppad);

}

FastORHitsView::~FastORHitsView() {
}

