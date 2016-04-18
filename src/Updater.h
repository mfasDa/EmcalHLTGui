/*
 * Updater.h
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#ifndef UPDATER_H_
#define UPDATER_H_

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
