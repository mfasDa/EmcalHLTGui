#ifndef VIEW_H_
#define VIEW_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include <string>
#include <map>
#include <vector>

class ViewDrawable {
	std::string 						fHistname;
	std::vector<std::string> 			fOptions;

public:
	ViewDrawable(const std::string &histname) : fHistname(histname), fOptions() {}
	~ViewDrawable() {}

	void AddOption(const std::string &option) { fOptions.push_back(option); }

	const std::string &GetHistname() const { return fHistname; }
	const std::vector<std::string > &GetOptions() const { return fOptions; }


	void SetName(const std::string &name) { fHistname = name; }
	const std::string &GetName() const { return fHistname; }

};

class ViewPad {
	std::vector<ViewDrawable *>			fDrawables;
	std::vector<std::string> 			fPadOptions;

public:
	ViewPad() : fDrawables(), fPadOptions() {}
	~ViewPad();

	void AddOption(const std::string &opt) { fPadOptions.push_back(opt); }
	void AddDrawable(const std::string &histname, const std::string &drawoptions);

	const std::vector<std::string> &GetListOfOptions() const { return fPadOptions; }
	const std::vector<ViewDrawable *> &GetListOfDrawables() const { return fDrawables; }
};

class View {
	std::string 					  	fName;
	std::string						  	fTitle;
	int                       fNRowPads;
	int                       fNColPads;

	std::map<std::pair<int,int>, ViewPad *>  fPads;
public:
	View();
	View(const std::string &name, const std::string &title);
	virtual ~View() {}

	void SetNumberOfPads(int col, int row) { fNRowPads = col; fNColPads = row; }
	void DefinePad();

	int GetNumberOfRowPads() const { return fNRowPads; }
	int GetNumberOfColPads() const { return fNColPads; }
	const ViewPad* GetPad(int row, int col) const;
	void SetPad(int row, int col, ViewPad *padDef);

	std::string GetName() const { return fName; }
	std::string GetTitle() const { return fTitle; }

	void SetName(const std::string &name) { fName = name; }
	void SetTitle(const std::string &title) { fTitle = title; }
};

#endif /* VIEW_H_ */
