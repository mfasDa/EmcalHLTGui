/*
 * BasicTriggerAmplitudeView.cxx
 *
 *  Created on: 18.04.2016
 *      Author: markusfasel
 */

#include "views/BasicTriggerAmplitudeView.h"

BasicTriggerAmplitudeView::BasicTriggerAmplitudeView() : View("basic amps", "Trigger amplitudes") {

	SetNumberOfPads(4);

	ViewPad *l0amppad = new ViewPad;
	l0amppad->AddDrawable("EMCTRQA_histFastORL0", "");
	SetPad(0, l0amppad);
	ViewPad *l1amppad = new ViewPad;
	l1amppad->AddDrawable("EMCTRQA_histFastORL1", "");
	SetPad(1, l1amppad);
	ViewPad *l0amp2dpad = new ViewPad;
	l0amp2dpad->AddDrawable("EMCTRQA_histFastORL0Amp", "");
	SetPad(2, l0amp2dpad);
	ViewPad *l0timepad = new ViewPad;
	l0timepad->AddDrawable("EMCTRQA_histFastORL0Time", "");
	SetPad(3, l0timepad);

}

BasicTriggerAmplitudeView::~BasicTriggerAmplitudeView() {
}

