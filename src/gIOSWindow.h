/*
 * gIOSWindow.h
 *
 *  Created on: Mar 11, 2023
 *      Created by: kayra
 */

#ifndef G_IOSWINDOW_H
#define G_IOSWINDOW_H

#include "gBaseWindow.h"
#include <array>

class gIOSWindow : public gBaseWindow {
public:
static constexpr int VIRTUAL_AXIS_COUNT = 6;
static constexpr int VIRTUAL_BUTTON_COUNT = 15;
    gIOSWindow();
	virtual ~gIOSWindow();
    
    void initialize(int width, int height, int windowMode, bool isResizable) override;
    bool getShouldClose() override;
    bool isJoystickPresent(int joystickId) override;

    bool isGamepadButtonPressed(int joystickId, int buttonId) override;

    const float* getJoystickAxes(int joystickId, int* axisCountPtr) override;

    void setVirtualGamepadConnected(int gamepadId, bool connected) override;

    void setVirtualGamepadAxis(int gamepadId, int axisId, float value) override;

    void setVirtualGamepadButton(int gamepadId, int buttonId, bool pressed) override;
    bool supportsVulkan() const override;
    void getVulkanInstanceExtensions(std::vector<const char*>& extensions) const override;
    bool createVulkanSurface(void* instance, void* surface) override;
    
    static gIOSWindow* getWindow();
    
    // We do not handle update and close methods here, as they are controlled
    // from the Objective-C++ part.
private:
    bool virtualgamepadconnected;
    std::array<float, VIRTUAL_AXIS_COUNT> virtualgamepadaxes;
    std::array<bool, VIRTUAL_BUTTON_COUNT> virtualgamepadbuttons;
};

void passEventToIOSWindow(int eventType);

#endif /* GIP_IOSWINDOW_H */
