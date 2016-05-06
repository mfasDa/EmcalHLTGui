/*
 * EMCALHLTgui.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */
#include <iostream>

#include "EMCALHLTgui.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TStyle.h>

#include <TGWindow.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TRootEmbeddedCanvas.h>

#include "DataHandler.h"
#include "Updater.h"
#include "View.h"
#include "ViewHandler.h"

ClassImp(EMCALHLTgui)

EMCALHLTgui::EMCALHLTgui() :
	TGMainFrame(gClient->GetRoot(), 1200, 8000, kHorizontalFrame),
	fViewSelection(NULL),
	fRunLabel(NULL),
	fCanvas(NULL),
	fViewHandler(NULL),
	fCurrentView(""),
	fViewLookup(),
	fRunNumber(0),
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

	fRunLabel = new TGLabel(vframe);
	fRunLabel->SetText(Form("Run: %d", fRunNumber));
	vframe->AddFrame(fRunLabel, new TGLayoutHints(kLHintsBottom));

	AddFrame(vframe, new TGLayoutHints(kLHintsLeft|kLHintsExpandY,10,10,10,10));

	fCanvas = new TRootEmbeddedCanvas("plot", this, 1024, 768);
	AddFrame(fCanvas, new TGLayoutHints(kLHintsRight|kLHintsExpandX|kLHintsExpandY, 10, 10, 10, 10));
	
	MapSubwindows();
	MapWindow();
	Resize();
	MapRaised();

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

  TCanvas *internalCanvas = fCanvas->GetCanvas();
  internalCanvas->Clear();
  internalCanvas->Divide(myview->GetNumberOfRowPads(), myview->GetNumberOfColPads());

  for(Int_t row = 1; row <= myview->GetNumberOfRowPads() ; row++){
    for(Int_t col = 1; col <= myview->GetNumberOfColPads() ; col++){
      TVirtualPad * mypad = internalCanvas->cd(myview->GetNumberOfColPads()*row + col);
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

void EMCALHLTgui::ProcessDrawable(const ViewDrawable &drawable, bool drawsame){
	std::string drawoption = "";
	TH1 *hist = fDataHandler->FindHistogram(drawable.GetName());
	if(!hist){
		std::cerr << "Histogram " << drawable.GetName() << " not found" << std::endl;
		return;
	}

	for(std::vector<std::string>::const_iterator optiter = drawable.GetOptions().begin(); optiter != drawable.GetOptions().end(); ++optiter){
		std::cout << "drawoptions " << *optiter <<std::endl;
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
	std::cout << "Histogram " << hist->GetName() << ", draw option " << drawoption << std::endl;
	hist->Draw(drawoption.c_str());
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

void EMCALHLTgui::StartUpdateCycle(){
	if(fDataHandler->Update()) SetRunNumber(fDataHandler->GetRunNumber());
	fTimer->SetDataHandler(fDataHandler);
	fTimer->TurnOn();
}
