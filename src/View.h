/*
 * View.h
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#ifndef VIEW_H_
#define VIEW_H_

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
	const std::string GetName() const { return fHistname; }

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
	std::string 						fName;
	std::string							fTitle;
	int									fNPads;

	std::map<int, ViewPad *>			fPads;
public:
	View();
	View(const std::string &name, const std::string &title);
	virtual ~View() {}

	void SetNumberOfPads(int npads) { fNPads = npads; }
	void DefinePad();

	int GetNumberOfPads() const { return fNPads; }
	const ViewPad* GetPad(int pad) const;
	void SetPad(int padId, ViewPad *padDef);

	std::string GetName() const { return fName; }
	std::string GetTitle() const { return fTitle; }

	void SetName(const std::string &name) { fName = name; }
	void SetTitle(const std::string &title) { fTitle = title; }
};

#endif /* VIEW_H_ */
