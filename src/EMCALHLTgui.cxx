/*
 * EMCALHLTgui.cxx
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#include "EMCALHLTgui.h"

#include <TCanvas.h>
#include <TH1.h>

#include <TGWindow.h>
#include <TGLabel.h>
#include <TGListBox.h>
#include <TRootEmbeddedCanvas.h>

#include "DataHandler.h"
#include "Updater.h"
#include "View.h"
#include "ViewHandler.h"

EMCALHLTgui::EMCALHLTgui() :
	fMain(NULL),
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
	fMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
	fMain->SetWindowName("EMCAL HLT Monitor");


	TGVerticalFrame *vframe = new TGVerticalFrame(fMain , 200, 600);

	fViewSelection = new TGListBox(vframe);
	fViewSelection->Connect("Clicked()", "EMCALGUI", this, "RedrawView()");
	vframe->AddFrame(fViewSelection, new TGLayoutHints(kLHintsNormal, 10, 10, 200, 10));

	fRunLabel = new TGLabel(vframe);
	fRunLabel->SetText(Form("Run: %d", fRunNumber));
	vframe->AddFrame(fRunLabel, new TGLayoutHints());

	fMain->AddFrame(vframe, new TGLayoutHints(kLHintsNormal,10,10,10,10));

	fCanvas = new TRootEmbeddedCanvas("plot", fMain, 600, 400);
	fMain->AddFrame(fCanvas, new TGLayoutHints(kLHintsLeft, 200, 10, 10, 10));

	fMain->MapWindow();

	fTimer = new Updater();
	fTimer->SetGUI(this);
}

EMCALHLTgui::~EMCALHLTgui() {
	if(fTimer){
		fTimer->Stop();
		delete fTimer;
	}
	if(fDataHandler) delete fDataHandler;
	if(fViewHandler) delete fViewHandler;
	fMain->Cleanup();
	delete fMain;
}

void EMCALHLTgui::SetViewHandler(ViewHandler *handler) {
	fViewHandler = handler;
	int icounter = 0;
	for(std::map<std::string, View *>::const_iterator it = fViewHandler->GetListOfViews().begin(); it != fViewHandler->GetListOfViews().end(); ++it){
		fViewSelection->AddEntry(it->second->GetTitle().c_str(), icounter);
		fViewLookup.insert(std::pair<int, std::string>(icounter, it->first));
	}
}


void EMCALHLTgui::ChangeView(){
	fCurrentView = fViewLookup.find(fViewSelection->GetSelectedEntry()->EntryId())->second;
	RedrawView();
}

void EMCALHLTgui::RedrawView(){
	const View *myview = fViewHandler->FindView(fCurrentView);
	if(!myview) return;
	fDataHandler->Update();

	TCanvas *internalCanvas = fCanvas->GetCanvas();
	internalCanvas->DivideSquare(myview->GetNumberOfPads());

	for(Int_t ipad = 1; ipad <= myview->GetNumberOfPads() ; ipad++){
		TVirtualPad * mypad = internalCanvas->cd(ipad);
		const ViewPad *currentpad = myview->GetPad(ipad-1);
		if(!currentpad) continue;
		HandlePadOptions(mypad, currentpad);

		int ndrawable = 0;
		for(std::vector<ViewDrawable *>::const_iterator diter = currentpad->GetListOfDrawables().begin(); diter != currentpad->GetListOfDrawables().end(); ++diter){
			ProcessDrawable(*(*diter), ndrawable != 0);
			ndrawable++;
		}
	}

	internalCanvas->Update();
	fCurrentView = myview->GetName();
}

void EMCALHLTgui::HandlePadOptions(TVirtualPad *output, const ViewPad *options){
	for(std::vector<std::string>::const_iterator optiter = options->GetListOfOptions().begin(); optiter != options->GetListOfOptions().end(); ++optiter){
		if(*optiter == "logx") output->SetLogx();
		if(*optiter == "logy") output->SetLogy();
		if(*optiter == "logz") output->SetLogz();
	}
}

void EMCALHLTgui::ProcessDrawable(const ViewDrawable &drawable, bool drawsame){
	std::string drawoption = "";
	TH1 *hist = fDataHandler->FindHistogram(drawable.GetName());
	if(!hist) return;

	for(std::vector<std::string>::const_iterator optiter = drawable.GetOptions().begin(); optiter != drawable.GetOptions().end(); ++optiter){
		std::string key, value;
		size_t delim;
		if((delim = optiter->find("=")) != std::string::npos){
			key = optiter->substr(0, delim - 1);
			value = optiter->substr(delim+1, optiter->length() - 1);
		} else {
			key = *optiter;
			value = "";
		}
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
	}
}

void EMCALHLTgui::StartUpdateCycle(){
	fTimer->SetDataHandler(fDataHandler);
	fTimer->Start(1000);
}
