#ifndef HISTOGRAMHANDLER_H_
#define HISTOGRAMHANDLER_H_
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

#include <string>
#include <vector>
#include <TH1.h>
#include "shared_ptr.h"
#include "Synchronized.h"

class TH1;

class HistogramHandler : public Synchronized {
public:
	HistogramHandler();
	virtual ~HistogramHandler();

	void SetHistogram(TH1 *hist);

	EMCALHLTGUI::shared_ptr<TH1> FindHistogram(const std::string &name);
	EMCALHLTGUI::shared_ptr<TH1> FindHistogramOriginal(const std::string &name);

	void SetTimeStamp();
	void Clear();
	void ClearOriginal();

protected:
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::iterator  FindInVector(const std::string &name, std::vector<EMCALHLTGUI::shared_ptr<TH1> > &ref);
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >::const_iterator  ConstFindInVector(const std::string &name, const std::vector<EMCALHLTGUI::shared_ptr<TH1> > &ref) const;

	TH1 *MakeSubtraction(const TH1 *data, const TH1 *ref) const;
	TH1 *CopyHelper(const TH1 *in) const;

private:
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >	fOriginal;		///< Original histogram
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >	fTimeStamp;		///< Time stamp for subtraction
	std::vector<EMCALHLTGUI::shared_ptr<TH1> >	fSubtracted;		///< Histogram after subtraction
};

#endif /* HISTOGRAMHANDLER_H_ */
