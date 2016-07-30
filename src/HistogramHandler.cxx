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
#include <TClass.h>
#include <TH2.h>

#include "HistogramHandler.h"

HistogramHandler::HistogramHandler() :
Synchronized(),
fOriginal(),
fTimeStamp(),
fSubtracted()
{

}

HistogramHandler::~HistogramHandler() {
	Clear();
}

void HistogramHandler::Clear(){
	// Clear histograms. Histograms are handled by shared pointers, so calling clear is sufficient
	fOriginal.clear();
	fTimeStamp.clear();
	fSubtracted.clear();
}

void HistogramHandler::ClearOriginal() {
	// Instead of clearing all histograms clear only the original data - all others stay
	fOriginal.clear();	
}

std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator HistogramHandler::ConstFindInVector(const std::string &name, const std::vector<EMCALHLTGUI::shared_ptr<TH1> > &ref) const {
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator result = ref.end();
	for(std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator it = ref.begin(); it != ref.end(); ++it){
		if(std::string((*it)->GetName()) == name){
			result = it;
			break;
		}
	}
	return result;
}

std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator HistogramHandler::FindInVector(const std::string &name, std::vector<EMCALHLTGUI::shared_ptr<TH1> > &ref) {
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator result = ref.end();
	for(std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator it = ref.begin(); it != ref.end(); ++it){
		if(std::string((*it)->GetName()) == name){
			result = it;
			break;
		}
	}
	return result;
}

void HistogramHandler::SetHistogram(TH1 *newhist){
	// Find histogram in the list of histograms
	// if it exists, delete it
	// 
	// Asynchronous access protected by a lock
	Wait();
	Lock();
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator foundOrig = FindInVector(newhist->GetName(), fOriginal);
	if(foundOrig != fOriginal.end()) fOriginal.erase(foundOrig);

	// Set the new histogram
	fOriginal.push_back(EMCALHLTGUI::shared_ptr<TH1>(newhist));

	// Do subtraction
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator foundTimestamp = ConstFindInVector(newhist->GetName(), fTimeStamp);
	if(foundTimestamp != fTimeStamp.end()){
		TH1 *subtracted = MakeSubtraction(newhist, foundTimestamp->get());
		std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator foundSubtracted = FindInVector(newhist->GetName(), fSubtracted);
		if(foundSubtracted != fSubtracted.end()) fSubtracted.erase(foundSubtracted);
		fSubtracted.push_back(EMCALHLTGUI::shared_ptr<TH1>(subtracted));
	}
	Unlock();
}

EMCALHLTGUI::shared_ptr<TH1> HistogramHandler::FindHistogram(const std::string &name) {
	// Steps:
	// 1st in subtracted
	// 2nd if not found try in original
	// can't be const as it is a synchronized process
	Wait();
	Lock();
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator result = ConstFindInVector(name, fSubtracted);
	Unlock();
	// debugging - check if original histogram found
	if(result != fSubtracted.end()) std::cout << "found subtracted histogram for name " << name << std::endl;
	return (result != fSubtracted.end())? *result : FindHistogramOriginal(name);
}

EMCALHLTGUI::shared_ptr<TH1> HistogramHandler::FindHistogramOriginal(const std::string &name) {
	Wait();
	Lock();
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator result = ConstFindInVector(name, fOriginal);
	Unlock();
	return (result != fOriginal.end()) ? *result : EMCALHLTGUI::shared_ptr<TH1>(NULL);
}

TH1 *HistogramHandler::MakeSubtraction(const TH1 *data, const TH1 *ref) const {
	TH1 *result = CopyHelper(data);
	result->Add(ref, -1.);
	return result;
}

TH1 *HistogramHandler::CopyHelper(const TH1 *in) const {
	std::cout << "Copy helper called" << std::endl;
	TH1 *result = NULL;
	std::string histtype = in->IsA()->GetName();
	if(histtype == "TH1C"){
		result = new TH1C(*static_cast<const TH1C *>(in));
	} else if(histtype == "TH1I"){
		result = new TH1I(*static_cast<const TH1I *>(in));
	} else if(histtype == "TH1S"){
		result = new TH1S(*static_cast<const TH1S *>(in));
	} else if(histtype == "TH1F"){
		result = new TH1F(*static_cast<const TH1F *>(in));
	} else if(histtype == "TH1D"){
		result = new TH1D(*static_cast<const TH1D *>(in));
	} else if(histtype == "TH2C"){
		result = new TH2C(*static_cast<const TH2C *>(in));
	} else if(histtype == "TH2I"){
		result = new TH2I(*static_cast<const TH2I *>(in));
	} else if(histtype == "TH2S"){
		result = new TH2S(*static_cast<const TH2S *>(in));
	} else if(histtype == "TH2F"){
		result = new TH2F(*static_cast<const TH2F *>(in));
	} else if(histtype == "TH2D"){
		result = new TH2D(*static_cast<const TH2D *>(in));
	}
	if(result) result->SetDirectory(NULL);
	return result;
}

void HistogramHandler::SetTimeStamp(){
	// 1st: Clean the content of the old time stamp (check that histograms are not used in fOriginal)
	// 2nd: Set data in fOriginal as new time stamp
	// 3rd: Update the subtracted histograms (first clean everything, than subtract)
	// as histograms are handled by shared pointers cleaning is done automatically when clear is called.
	//
	// As the Histogram handler is use in asychronous processes a semaphore has to be used to protect against
	// simulatous read / write
	std::cout << "Setting new time stamp ..." << std::endl;
	Wait();
	Lock();
	std::cout << "Histogram handler, locked by SetTimeStamp()" << std::endl;
	fTimeStamp.clear();

	// set the new time stamp
	for(std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator source = fOriginal.begin(); source != fOriginal.end(); ++source) fTimeStamp.push_back(*source);

	// update subtracted
	fSubtracted.clear();
	for(std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator subiter = fOriginal.begin(); subiter != fOriginal.end(); ++subiter){
		std::string histname = (*subiter)->GetName();
		fSubtracted.push_back(EMCALHLTGUI::shared_ptr<TH1>(MakeSubtraction(subiter->get(), ConstFindInVector(histname, fTimeStamp)->get())));
	}

	// work done - release lock
	std::cout << "Histogram handler, SetTimeStamp(), lock released ..." << std::endl;
	Unlock();
}

