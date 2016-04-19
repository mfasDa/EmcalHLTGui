/*
 * FastORHitsView.cxx
 *
 *  Created on: 18.04.2016
 *      Author: markusfasel
 */

#include <views/FastORHitsView.h>

FastORHitsView::FastORHitsView() : View("FastOR_hits", "FastOR hits") {

	SetNumberOfPads(4);

	ViewPad *l0amppad = new ViewPad;
	l0amppad->AddDrawable("EMCTRQA_histFastORL0", "");
	SetPad(0, l0amppad);

	ViewPad *l1amppad = new ViewPad;
	l1amppad->AddDrawable("EMCTRQA_histFastORL1", "");
	SetPad(1, l1amppad);

	ViewPad *l0largeamppad = new ViewPad;
	l0largeamppad->AddDrawable("EMCTRQA_histFastORL0LargeAmp", "");
	SetPad(2, l0largeamppad);

	ViewPad *l1largeamppad = new ViewPad;
	l1largeamppad->AddDrawable("EMCTRQA_histFastORL1LargeAmp", "");
	SetPad(3, l1largeamppad);

}

FastORHitsView::~FastORHitsView() {
}

