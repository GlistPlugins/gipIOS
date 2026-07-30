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

gIOSWindow::gIOSWindow() : virtualgamepadconnected(false) {
    vsync = true;
    window = this;
    virtualgamepadaxes.fill(0.0f);
    virtualgamepadbuttons.fill(false);
}

gIOSWindow::~gIOSWindow() {
}

void gIOSWindow::initialize(int width, int height, int windowMode, bool isResizable)
{
    gBaseWindow::initialize(width, height, windowMode, isResizable);
    setVirtualGamepadConnected(gBaseWindow::VIRTUAL_GAMEPAD_ID, true);
}

bool gIOSWindow::getShouldClose()
{
    return getIsTerminating();
}

gIOSWindow* gIOSWindow::getWindow()
{
    return window;
}

bool gIOSWindow::isJoystickPresent(int joystickId) {
    return joystickId == gBaseWindow::VIRTUAL_GAMEPAD_ID &&
           virtualgamepadconnected;
}

const float* gIOSWindow::getJoystickAxes(
    int joystickId,
    int* axisCountPtr
) {
    if(axisCountPtr) {
        *axisCountPtr = 0;
    }

    if(joystickId != gBaseWindow::VIRTUAL_GAMEPAD_ID ||
       !virtualgamepadconnected) {
        return nullptr;
    }

    if(axisCountPtr) {
        *axisCountPtr = VIRTUAL_AXIS_COUNT;
    }

    return virtualgamepadaxes.data();
}

bool gIOSWindow::isGamepadButtonPressed(
    int joystickId,
    int buttonId
) {
    if(joystickId != gBaseWindow::VIRTUAL_GAMEPAD_ID ||
       !virtualgamepadconnected) {
        return false;
    }

    if(buttonId < 0 ||
       buttonId >= VIRTUAL_BUTTON_COUNT) {
        return false;
    }

    return virtualgamepadbuttons[buttonId];
}

void gIOSWindow::setVirtualGamepadConnected(
    int gamepadId,
    bool connected
) {
    if(gamepadId != gBaseWindow::VIRTUAL_GAMEPAD_ID) {
        return;
    }

    if(virtualgamepadconnected == connected) {
        return;
    }

    virtualgamepadconnected = connected;

    if(!connected) {
        virtualgamepadaxes.fill(0.0f);
        virtualgamepadbuttons.fill(false);

        gJoystickDisconnectEvent event{gamepadId};
        callEvent(event);
        return;
    }

    gJoystickConnectEvent event{
        gamepadId,
        true
    };

    callEvent(event);
}

void gIOSWindow::setVirtualGamepadAxis(
    int gamepadId,
    int axisId,
    float value
) {
    if(gamepadId != gBaseWindow::VIRTUAL_GAMEPAD_ID ||
       !virtualgamepadconnected) {
        return;
    }

    if(axisId < 0 ||
       axisId >= VIRTUAL_AXIS_COUNT) {
        return;
    }

    if(value < -1.0f) {
        value = -1.0f;
    }
    else if(value > 1.0f) {
        value = 1.0f;
    }

    virtualgamepadaxes[axisId] = value;
}

void gIOSWindow::setVirtualGamepadButton(
    int gamepadId,
    int buttonId,
    bool pressed
) {
    if(gamepadId != gBaseWindow::VIRTUAL_GAMEPAD_ID ||
       !virtualgamepadconnected) {
        return;
    }

    if(buttonId < 0 ||
       buttonId >= VIRTUAL_BUTTON_COUNT) {
        return;
    }

    virtualgamepadbuttons[buttonId] = pressed;
}
