/*
 * ViewHandler.h
 *
 *  Created on: 15.04.2016
 *      Author: markusfasel
 */

#ifndef VIEWHANDLER_H_
#define VIEWHANDLER_H_

#include <string>
#include <map>
#include "View.h"

class ViewHandler {
	std::map<std::string, View *> 				fViewMap;

public:
	ViewHandler();
	virtual ~ViewHandler();

	void InsertView(const std::string &viewname, View *myview);
	const View *FindView(const std::string &name) const;
	const std::map<std::string, View *> GetListOfViews() const { return fViewMap; }
};

#endif /* VIEWHANDLER_H_ */
