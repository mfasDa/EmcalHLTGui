#ifndef SYNCHRONIZED_H
#define SYNCHRONIZED_H
/****************************************************************************************
 *  Simple monitoring program for ALICE EMCAL QA histograms provided by the ALICE HLT   *
 *  Copyright (C) 2016 The ALICE collaboration                                          *
 *  See cxx file for more details														*
 ****************************************************************************************/

class Synchronized{
protected:
	bool			fLock;			///< Semaphore

public:
	Synchronized();
	~Synchronized() {}

	void Lock() { fLock = true; }
	void Unlock() { fLock = false; }

	bool IsLocked() { return fLock; }

	void Wait() const;
};

#endif
