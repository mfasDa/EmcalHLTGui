/*
 * EMCALHLTgui.h
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#ifndef EMCALHLTGUI_H_
#define EMCALHLTGUI_H_

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>

#include <map>
#include <string>

class TGMainFrame;
class TGListBox;
class TGLabel;
class TRootEmbeddedCanvas;
class TVirtualPad;

class DataHandler;
class Updater;
class ViewHandler;
class ViewPad;
class ViewDrawable;

class EMCALHLTgui : public TGMainFrame {
	RQ_OBJECT("EMCALGUI");

	// View
	TGListBox					*fViewSelection;
	TRootEmbeddedCanvas			*fCanvas;
	TGLabel						*fRunLabel;

	// Model
	ViewHandler					*fViewHandler;
	std::map<int, std::string>	fViewLookup;
	std::string					fCurrentView;
	int 						fRunNumber;

	// Controller
	DataHandler					*fDataHandler;
	Updater						*fTimer;

protected:
	void HandlePadOptions(TVirtualPad *output, const ViewPad *padhandler);
	void ProcessDrawable(const ViewDrawable &drawable, bool drawoptions);
	Color_t FindColor(const std::string &color) const;

public:
	EMCALHLTgui();
	virtual ~EMCALHLTgui();

	void SetViewHandler(ViewHandler *handler);
	void SetDataHandler(DataHandler *handler) { fDataHandler = handler; }

	void StartUpdateCycle();
	void SetRunNumber(int runnumber);
	void Update();
	void ChangeView(int viewentry);
	void RedrawView();
};

#endif /* EMCALHLTGUI_H_ */
