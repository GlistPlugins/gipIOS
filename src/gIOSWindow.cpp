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
    gBaseWindow::initialize(width, height, windowmode, isResizable);
}

bool gIOSWindow::getShouldClose()
{
    return getIsTerminating();
}

gIOSWindow* gIOSWindow::getWindow()
{
    return window;
}
