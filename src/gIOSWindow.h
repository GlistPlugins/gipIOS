/*
 * gIOSWindow.h
 *
 *  Created on: Mar 11, 2023
 *      Created by: kayra
 */

#ifndef G_IOSWINDOW_H
#define G_IOSWINDOW_H

#include "gBaseWindow.h"

class gIOSWindow : public gBaseWindow {
public:
    gIOSWindow();
	virtual ~gIOSWindow();
    
    void initialize(int width, int height, int windowMode, bool isResizable) override;
    bool getShouldClose() override;
    
    static gIOSWindow* getWindow();
    
    // We do not handle update and close methods here, as they are controlled
    // from the Objective-C++ part.
};

void passEventToIOSWindow(int eventType);

#endif /* GIP_IOSWINDOW_H */
