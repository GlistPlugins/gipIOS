#include "gIOSInterface.h"

#include "gIOSWindow.h"
#include "gAppManager.h"

struct AppParameters
{
    std::string appName;
    gBaseApp* baseApp;
    int width;
    int height;
    int unitwidth;
    int unitheight;
    int windowMode;
    int screenScaling;
    bool isResizable;
    int loopmode;
};

gAppManager* appmanager{};
AppParameters params{};

void init(void* baseApp, const char* appName, int windowMode, int unitWidth, int unitHeight, int screenScaling, int width, int height, bool isResizable)
{
    params = AppParameters{std::string(appName), (gBaseApp*)baseApp, width, height, unitWidth, unitHeight, windowMode, screenScaling, isResizable, G_LOOPMODE_NORMAL};
}
void setup()
{
    // Mirrors the Android startup: the window takes the screen's real pixel
    // size (the drawable size measured on the first frame - see getViewBounds),
    // while the unit space keeps the design size the app was started with.
    // Touches arrive in the same pixel space, so hit-testing and scrolling see
    // consistent coordinates. The bounds captured in viewDidLoad are the
    // storyboard's design-time size and must not be used for either.
    ViewBounds bounds = getViewBounds();
    appmanager = new gAppManager(params.appName, params.baseApp, static_cast<int>(bounds.width), static_cast<int>(bounds.height), params.windowMode, params.unitwidth, params.unitheight, params.screenScaling, params.isResizable, params.loopmode);

    appmanager->initialize();
    appmanager->setup();
    // One-time start: on iOS gAppManager::loop() only marks the app running and
    // seeds the frame clock (its actual loops are compiled out). Calling it once
    // here instead of every frame keeps starttime intact, so iosLoop() measures
    // real frame deltas - the scroll fling physics depend on them.
    appmanager->loop();
}

void loop()
{
    appmanager->iosLoop();
}

void stop()
{
    appmanager->stop();
    
    delete appmanager;
}

bool setDefaultFbo(int fbo)
{
    return gFbo::defaultfbo = fbo;
}
