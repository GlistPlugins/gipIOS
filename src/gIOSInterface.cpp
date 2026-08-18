#include "gIOSInterface.h"

#include "gIOSWindow.h"
#include "gAppManager.h"

struct AppParameters
{
    std::string appName;
    gBaseApp* baseApp;
    int unitWidth;
    int unitHeight;
    int width;
    int height;
    int windowMode;
    int screenScaling;
    bool isResizable;
    int loopmode;
    int renderEngine;
};

gAppManager* appmanager{};
AppParameters params{};

void init(void* baseApp, const char* appName, int windowMode, int unitWidth, int unitHeight, int screenScaling, int width, int height, bool isResizable, int renderEngine)
{
#ifndef GLIST_HAS_VULKAN
    if(renderEngine == G_RENDERER_VK) renderEngine = G_RENDERER_GL;
#endif
    params = AppParameters{std::string(appName), (gBaseApp*)baseApp, unitWidth, unitHeight, width, height, windowMode, screenScaling, isResizable, G_LOOPMODE_NORMAL, renderEngine};
}

bool isIOSVulkanRequested()
{
    return params.renderEngine == G_RENDERER_VK;
}
void setup()
{
    ViewBounds bounds = getViewBounds();
    int unitwidth = static_cast<int>(bounds.width);
    int unitheight = static_cast<int>(bounds.height);
    appmanager = new gAppManager(params.appName, params.baseApp, unitwidth, unitheight, params.windowMode, params.unitWidth, params.unitHeight, params.screenScaling, params.isResizable, params.loopmode);
    appmanager->setRenderEngine(params.renderEngine);

    appmanager->initialize();
    appmanager->setup();
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
