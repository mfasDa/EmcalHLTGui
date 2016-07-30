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
#include <iostream>

#include "EMCALHLTgui.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TStyle.h>

#include <TGWindow.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>

#include "DataHandler.h"
#include "HistogramHandler.h"
#include "Updater.h"
#include "View.h"
#include "ViewHandler.h"

ClassImp(EMCALHLTgui)

EMCALHLTgui::EMCALHLTgui() :
	TGMainFrame(gClient->GetRoot(), 1200, 8000, kHorizontalFrame),
	fViewSelection(NULL),
	fRunLabel(NULL),
	fEventLabel(NULL),
	fResetButton(NULL),
	fCanvas(NULL),
	fViewHandler(NULL),
	fCurrentView(""),
	fCurrentHistos(),
	fViewLookup(),
	fRunNumber(0),
	fNumberOfEvents(0),
	fNumberOfEventsMonitor(0),
	fDataHandler(NULL),
	fTimer(NULL)
{
  	gStyle->SetOptStat(0);

	SetWindowName("EMCAL HLT Monitor");

	TGVerticalFrame *vframe = new TGVerticalFrame(this);

	fViewSelection = new TGListBox(vframe, 1);
	fViewSelection->Resize(400, 800);
	fViewSelection->Connect("Selected(Int_t)", "EMCALHLTgui", this, "ChangeView(Int_t)");
	vframe->AddFrame(fViewSelection, new TGLayoutHints(kLHintsTop|kLHintsExpandY));

	TGHorizontalFrame *steerframe = new TGHorizontalFrame(vframe);
	vframe->AddFrame(steerframe, new TGLayoutHints(kLHintsBottom));

	fRunLabel = new TGLabel(steerframe);
	fRunLabel->SetText(Form("Run: %d", fRunNumber));
	steerframe->AddFrame(fRunLabel, new TGLayoutHints(kLHintsLeft,10,10,3,3));

	fEventLabel = new TGLabel(steerframe);
	fEventLabel->SetText(Form("Number of Event: %d", fNumberOfEvents));
	steerframe->AddFrame(fEventLabel, new TGLayoutHints(kLHintsLeft, 10,10,3,3));

	fResetButton = new TGTextButton(steerframe, "&Reset");
	fResetButton->Connect("Clicked()", "EMCALHLTgui", this, "ResetCallback()");
	steerframe->AddFrame(fResetButton, new TGLayoutHints(kLHintsLeft, 10, 10, 3, 3));

	AddFrame(vframe, new TGLayoutHints(kLHintsLeft|kLHintsExpandY,10,10,10,10));

	fCanvas = new TRootEmbeddedCanvas("plot", this, 1024, 768);
	AddFrame(fCanvas, new TGLayoutHints(kLHintsRight|kLHintsExpandX|kLHintsExpandY, 10, 10, 10, 10));
	
	MapSubwindows();
	MapWindow();
	Resize();
	MapRaised();

	std::cout << "view constructed, setting timer ..." << std::endl;
	fTimer = new Updater(10000);
	fTimer->SetGUI(this);
}

EMCALHLTgui::~EMCALHLTgui() {
	if(fTimer){
		fTimer->TurnOff();
		delete fTimer;
	}
	if(fDataHandler) delete fDataHandler;
	if(fViewHandler) delete fViewHandler;
	Cleanup();
}

void EMCALHLTgui::SetViewHandler(ViewHandler *handler) {
	fViewHandler = handler;
	int icounter = 0;
	const std::map<std::string, View *> &viewlist = fViewHandler->GetListOfViews();
	for(std::map<std::string, View *>::const_iterator it = viewlist.begin(); it != viewlist.end(); ++it){
		fViewSelection->AddEntry(it->second->GetTitle().c_str(), icounter);
		fViewLookup.insert(std::pair<int, std::string>(icounter, it->first));
		icounter++;
	}
}


void EMCALHLTgui::ChangeView(Int_t viewentry){
	fCurrentView = fViewLookup.find(viewentry)->second;
	RedrawView();
}

void EMCALHLTgui::RedrawView(){
  	const View *myview = fViewHandler->FindView(fCurrentView);
  	if(!myview) return;

  	fCurrentHistos.clear();
  	TCanvas *internalCanvas = fCanvas->GetCanvas();
  	internalCanvas->Clear();
  	internalCanvas->Divide(myview->GetNumberOfColPads(), myview->GetNumberOfRowPads());

  	for(Int_t row = 1; row <= myview->GetNumberOfRowPads() ; row++){
    		for(Int_t col = 1; col <= myview->GetNumberOfColPads() ; col++){
      			TVirtualPad * mypad = internalCanvas->cd(myview->GetNumberOfColPads()*(row-1) + col);
      			const ViewPad *currentpad = myview->GetPad(row-1, col-1);
      			if(!currentpad) continue;

      			int ndrawable = 0;
      			for(std::vector<ViewDrawable *>::const_iterator diter = currentpad->GetListOfDrawables().begin(); diter != currentpad->GetListOfDrawables().end(); ++diter){
        			ProcessDrawable(*(*diter), diter != currentpad->GetListOfDrawables().begin());
        			ndrawable++;
      			}

      			HandlePadOptions(mypad, currentpad);
      			mypad->Update();
    		}
  	}
  	internalCanvas->Update();
  	fCurrentView = myview->GetName();

	// Debugging - get number of events shown in the histogram
	int neventsMonitor = GetNumberOfEventsMonitor(), neventsTotal = fDataHandler->GetNumberOfEvents();
	std::cout << "Found event hist with " << neventsMonitor << " events (total " << neventsTotal << ")" << std::endl;

  	// Update the number of events
  	SetNumberOfEvents(neventsTotal, neventsMonitor);
}

void EMCALHLTgui::HandlePadOptions(TVirtualPad *output, const ViewPad *options){
	const std::vector<std::string> &globalopts = options->GetListOfOptions();
	for(std::vector<std::string>::const_iterator optiter = globalopts.begin(); optiter != globalopts.end(); ++optiter){
		if(*optiter == "logx") output->SetLogx();
		if(*optiter == "logy") output->SetLogy();
		if(*optiter == "logz") output->SetLogz();
		if(*optiter == "drawtru") DrawTRUgrid(output);
	}
}

void EMCALHLTgui::DrawTRUgrid(TVirtualPad *output){
  	output->cd();
  	gStyle->SetLineStyle(kDashed);
  	gStyle->SetLineWidth(2);
  	TLine* line = 0;
  	// Draw grid for TRUs in full EMCal SMs
  	for (int x = 8; x < 48; x+=8) {
    		line = new TLine(x, 0, x, 60);
    		line->Draw();
  	}
  	for (int y = 12; y <= 60; y+=12) {
    		line = new TLine(0, y, 48, y);
    		line->Draw();
  	}
  	// Draw grid for TRUs in 1/3 EMCal SMs
  	line = new TLine(0, 64, 48, 64);
  	line->Draw();
  	line = new TLine(24, 60, 24, 64);
  	line->Draw();

  	// Draw grid for TRUs in 2/3 DCal SMs
  	for (int x = 8; x < 48; x+=8) {
    		if (x == 24) continue; // skip PHOS hole
    		line = new TLine(x, 64, x, 100);
    		line->Draw();
  	}
  	for (int y = 76; y < 100; y+=12) {
    		line = new TLine(0, y, 16, y);
    		line->Draw();
    		// skip PHOS hole
    		line = new TLine(32, y, 48, y);
    		line->Draw();
  	}
  	// Draw grid for TRUs in 1/3 DCal SMs
  	line = new TLine(0, 100, 48, 100);
  	line->Draw();
  	line = new TLine(24, 100, 24, 104);
  	line->Draw();
}


int EMCALHLTgui::GetNumberOfEventsMonitor(){
	// Method cannot be const due to locking issues
	int result = 0;
	EMCALHLTGUI::shared_ptr<TH1> eventhist = fDataHandler->FindHistogram("EMCTRQA_histEvents");
	if(eventhist){
		result = static_cast<int>(eventhist->GetBinContent(1));
	} else {
		std::cerr << "Event counter histogram not found in the data handler" << std::endl;
	}
	return result;
}

void EMCALHLTgui::ProcessDrawable(const ViewDrawable &drawable, bool drawsame){
	std::string drawoption = "";
	EMCALHLTGUI::shared_ptr<TH1> hist = fDataHandler->FindHistogram(drawable.GetName());
	if(!hist){
		std::cerr << "Histogram " << drawable.GetName() << " not found" << std::endl;
		return;
	}

	for(std::vector<std::string>::const_iterator optiter = drawable.GetOptions().begin(); optiter != drawable.GetOptions().end(); ++optiter){
		//std::cout << "drawoptions " << *optiter <<std::endl;
		std::string key, value;
		size_t delim;
		if((delim = optiter->find("=")) != std::string::npos){
			key = optiter->substr(0, delim);
			value = optiter->substr(delim+1, optiter->length() - 1);
		} else {
			key = *optiter;
			value = "";
		}
		//std::cout << "Key"	
		if(key == "drawoption") drawoption = value;
		if(key == "color"){
			Color_t col = FindColor(value);
			hist->SetMarkerColor(col);
			hist->SetLineColor(col);
		}
		if(key == "marker"){
			hist->SetMarkerSize(atoi(value.c_str()));
		}
	}

	if(drawsame && (drawoption.find("same") == std::string::npos)) drawoption += "same";
	//std::cout << "Histogram " << hist->GetName() << ", draw option " << drawoption << std::endl;
	hist->Draw(drawoption.c_str());
	fCurrentHistos.push_back(hist);
}

Color_t EMCALHLTgui::FindColor(const std::string &colname) const{
	Color_t col = kBlack;
	if(colname == "blue") col = kBlue;
	else if(colname == "red") col = kRed;
	else if(colname == "green") col = kGreen;
	return col;
}

void EMCALHLTgui::SetRunNumber(int runnumber) {
	if(runnumber != fRunNumber){
		fRunLabel->SetText(Form("Run: %d", runnumber));
		fRunNumber = runnumber;
		Layout();
	}
}

void EMCALHLTgui::SetNumberOfEvents(int total, int monitor){
	fNumberOfEvents = total;
	if(monitor > -1) fNumberOfEventsMonitor = monitor;
	fEventLabel->SetText(Form("Number of events: %d (Plotting %d)", fNumberOfEvents, fNumberOfEventsMonitor));
	Layout();
}

void EMCALHLTgui::StartUpdateCycle(){
	std::cout << "Starting regular update cycle ... " << std::endl;
	if(fDataHandler->Update()) SetRunNumber(fDataHandler->GetRunNumber());
	fTimer->SetDataHandler(fDataHandler);
	fTimer->TurnOn();
}


void EMCALHLTgui::ResetCallback(){
	// Set the new timestamp
	std::cout << "Reset button called" << std::endl;
	fDataHandler->GetHistogramHandler().SetTimeStamp();
	SetNumberOfEvents(fDataHandler->GetNumberOfEvents(), GetNumberOfEventsMonitor());
	RedrawView();
}
