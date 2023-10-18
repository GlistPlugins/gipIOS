/*
 * gIOSInterface.h
 *
 *  Created on: Mar 11, 2023
 *      Created by: kayra
 */
 
#ifndef G_IOSINTERFACE_H_
#define G_IOSINTERFACE_H_

#include "gIOSWindow.h"

void init(void* baseApp, const char* appName, int windowMode, int unitWidth, int unitHeight, int screenScaling, int width, int height, bool isResizable);
void setup();
void loop();
void stop();

typedef struct ViewBounds
{
    float width;
    float height;
};

// Implementation in gIOSViewController.mm
ViewBounds getViewBounds();

// Implementation in gIOSAppDelegate.mm
bool getIsTerminating();

bool setDefaultFbo(int fbo);

template<typename T, typename... Args>
void fireEvent(Args&&... args)
{
    T event(std::forward<Args>(args)...);
    
    auto* window = gIOSWindow::getWindow();
    window->callEvent(event);
}

#endif // G_IOSINTERFACE_H_
