#ifndef UPDATER_H_
#define UPDATER_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/
#include <TTimer.h>

class EMCALHLTgui;
class DataHandler;

class Updater : public TTimer {
	DataHandler				*fDataHandler;
	EMCALHLTgui				*fGui;
public:
	Updater(Long_t timeout = 0);
	virtual ~Updater();

	void SetGUI(EMCALHLTgui *gui) { fGui = gui; }
	void SetDataHandler(DataHandler *handler) { fDataHandler = handler; }

	virtual bool Notify();
};

#endif /* UPDATER_H_ */
