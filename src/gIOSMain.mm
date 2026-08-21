#import <UIKit/UIApplication.h>

#import "gIOSAppDelegate.h"
#import "gIOSInterface.h"

int ios_main(void* baseApp, const char* appName, int windowMode, int unitWidth, int unitHeight, int screenScaling, int width, int height, bool isResizable, int renderEngine)
{
    init(baseApp, appName, windowMode, unitWidth, unitHeight, screenScaling, width, height, isResizable, renderEngine);
    UIApplicationMain(0, nil, nil, NSStringFromClass([gIOSAppDelegate class]));
}
