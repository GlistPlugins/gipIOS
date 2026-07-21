#import "gIOSAppDelegate.h"

#import <UIKit/UIStoryboard.h>

#import "gIOSViewController.h"
#import "gIOSInterface.h"
#import "gIOSViewController.h"
#import "gIOSWindow.h"

static bool g_IsTerminating = false;

@implementation gIOSAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return YES;
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    fireEvent<gAppResumeEvent>();
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    fireEvent<gAppPauseEvent>();
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    g_IsTerminating = true;
}

// Every touch used to be written into slot 0 of a count-sized array while the
// fired event still claimed count inputs, so anything past the first slot was
// read uninitialized on multi-touch. One event per touch with an honest count
// of 1 keeps the single-touch behaviour and closes that hole. The inputs are
// only borrowed for the duration of callEvent; the handlers copy what they keep.
static void fireTouchSet(NSSet<UITouch*>* touches, ActionType actiontype)
{
    UIView* view = getView();
    // locationInView reports points; the engine works in the drawable's pixel
    // space (the same convention as the Android surface), so scale up here.
    CGFloat scale = view.contentScaleFactor;
    int fingerid = 0;
    for (UITouch* touch in touches) {
        CGPoint point = [touch locationInView: view];

        TouchInput touchinput;
        touchinput.type = correctTouchTypeForGlist(touch.type);
        touchinput.fingerid = fingerid;
        touchinput.pointerindex = 0;
        touchinput.x = static_cast<int>(point.x * scale);
        touchinput.y = static_cast<int>(point.y * scale);

        fireEvent<gTouchEvent>(1, &touchinput, 0, actiontype);

        fingerid++;
    }
}

- (void)touchesBegan:(NSSet<UITouch*> *)touches withEvent:(nullable UIEvent *)event
{
    fireTouchSet(touches, ACTIONTYPE_DOWN);
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    fireTouchSet(touches, ACTIONTYPE_MOVE);
}
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    fireTouchSet(touches, ACTIONTYPE_UP);
}
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    fireTouchSet(touches, ACTIONTYPE_UP);
}

@end

bool getIsTerminating()
{
    return g_IsTerminating;
}

InputType correctTouchTypeForGlist(UITouchType type)
{
    switch (type) {
        case UITouchTypeDirect: return INPUTTYPE_FINGER;
        case UITouchTypePencil: return INPUTTYPE_STYLUS;
        default:                return INPUTTYPE_UNKNOWN;
    }
}
