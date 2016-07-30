#ifndef EMCALHLTGUI_H_
#define EMCALHLTGUI_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TH1.h>
#include <TRootEmbeddedCanvas.h>

#include "shared_ptr.h"

#include <map>
#include <string>
#include <vector>

class TGMainFrame;
class TGListBox;
class TGButton;
class TGLabel;
class TRootEmbeddedCanvas;
class TVirtualPad;

class DataHandler;
class Updater;
class ViewHandler;
class ViewPad;
class ViewDrawable;

class EMCALHLTgui : public TGMainFrame {
	RQ_OBJECT("EMCALHLTgui");

	// View
	TGListBox					*fViewSelection;
	TRootEmbeddedCanvas				*fCanvas;
	TGLabel						*fRunLabel;
	TGLabel						*fEventLabel;
	TGButton					*fResetButton;

	// Model
	ViewHandler					*fViewHandler;
	std::map<int, std::string>			fViewLookup;
	std::string					fCurrentView;
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >	fCurrentHistos;			///< Need a vector of shared pointers to keep the histograms in memory as long as they are in the view
	int 						fRunNumber;
	int 						fNumberOfEvents;
	int 						fNumberOfEventsMonitor;

	// Controller
	DataHandler					*fDataHandler;
	Updater						*fTimer;

protected:
	void HandlePadOptions(TVirtualPad *output, const ViewPad *padhandler);
	void ProcessDrawable(const ViewDrawable &drawable, bool drawoptions);
	Color_t FindColor(const std::string &color) const;
	int GetNumberOfEventsMonitor();

public:
	EMCALHLTgui();
	virtual ~EMCALHLTgui();

	void SetViewHandler(ViewHandler *handler);
	void SetDataHandler(DataHandler *handler) { fDataHandler = handler; }

	void StartUpdateCycle();
	void SetRunNumber(Int_t runnumber);
	void SetNumberOfEvents(int total, int monitor);
	//void Update();
	void ChangeView(Int_t viewentry);
	void RedrawView();
	void DrawTRUgrid(TVirtualPad *output);

	void ResetCallback();

	ClassDef(EMCALHLTgui, 1)
};

#endif /* EMCALHLTGUI_H_ */
