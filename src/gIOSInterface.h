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

// Implementation in gIOSViewController.mm
void showIOSKeyboard();
void hideIOSKeyboard();

// Implementation in gIOSAppDelegate.mm
bool getIsTerminating();

bool setDefaultFbo(int fbo);

template<typename T, typename... Args>
void fireEvent(Args&&... args)
{
    T event(std::forward<Args>(args)...);

    // UIKit lifecycle callbacks (applicationDidBecomeActive in particular) can
    // arrive before the first frame has created the engine window; firing into
    // a null window crashed there.
    auto* window = gIOSWindow::getWindow();
    if(!window) {
        return;
    }
    window->callEvent(event);
}

#endif // G_IOSINTERFACE_H_
