/*
 * gipIOSWindow.cpp
 *
 *  Created on: Mar 11, 2023
 *      Author: Noyan Culum
 *      Created by: kayra
 */

#include <OpenGLES/ES3/gl.h>

#include "gIOSWindow.h"
#include "gIOSInterface.h"

gIOSWindow* window = nullptr;

gIOSWindow::gIOSWindow() {
    vsync = true;
    window = this;
}

gIOSWindow::~gIOSWindow() {
}

void gIOSWindow::initialize(int width, int height, int windowMode, bool isResizable)
{
    // windowMode, the parameter - not the windowmode member, which still holds
    // its G_WINDOWMODE_NONE default at this point.
    gBaseWindow::initialize(width, height, windowMode, isResizable);
}

bool gIOSWindow::getShouldClose()
{
    return getIsTerminating();
}

void gIOSWindow::showKeyboard()
{
    showIOSKeyboard();
}

void gIOSWindow::hideKeyboard()
{
    hideIOSKeyboard();
}

gIOSWindow* gIOSWindow::getWindow()
{
    return window;
}
