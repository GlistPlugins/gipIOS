/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef EXAMPLES_TESTEXAMPLE_GAPP_H_
#define EXAMPLES_TESTEXAMPLE_GAPP_H_

#include "gBaseApp.h"
#include "gIOSApp.h"


class gApp : public gIOSApp {
public:
	gApp();
	~gApp();

	void setup();
	void update();
    
    void resume();
    void pause();
};

#endif /* EXAMPLES_TESTEXAMPLE_GAPP_H_ */
