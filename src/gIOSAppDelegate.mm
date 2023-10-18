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

- (void)touchesBegan:(NSSet<UITouch*> *)touches withEvent:(nullable UIEvent *)event
{
    NSArray<UITouch*>* allTouches = [touches allObjects];
    
    int fingerid = 0;
    int count = [allTouches count];
    
    TouchInput* touchinputs = new TouchInput[count];
    
    for (UITouch *touch in allTouches) {
        CGPoint point = [touch locationInView: getView()];
        
        TouchInput touchinput;
        touchinput.type = correctTouchTypeForGlist(touch.type);
        touchinput.fingerid = fingerid;
        touchinput.pointerindex = 0;
        touchinput.x = point.x;
        touchinput.y = point.y;
        
        touchinputs[0] = touchinput;
        
        fireEvent<gTouchEvent>(count, touchinputs, 0, ACTIONTYPE_DOWN);
        
        fingerid++;
    }
    
    delete[] touchinputs;
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    NSArray<UITouch*>* allTouches = [touches allObjects];
    
    int fingerid = 0;
    int count = [allTouches count];
    
    TouchInput* touchinputs = new TouchInput[count];
    
    for (UITouch *touch in allTouches) {
        CGPoint point = [touch locationInView: getView()];
        
        TouchInput touchinput;
        touchinput.type = correctTouchTypeForGlist(touch.type);
        touchinput.fingerid = fingerid;
        touchinput.pointerindex = 0;
        touchinput.x = point.x;
        touchinput.y = point.y;
        
        touchinputs[0] = touchinput;
        
        fireEvent<gTouchEvent>(count, touchinputs, 0, ACTIONTYPE_MOVE);
        
        fingerid++;
    }
    
    delete[] touchinputs;
}
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    NSArray<UITouch*>* allTouches = [touches allObjects];
    
    int fingerid = 0;
    int count = [allTouches count];
    
    TouchInput* touchinputs = new TouchInput[count];
    
    for (UITouch *touch in allTouches) {
        CGPoint point = [touch locationInView: getView()];
        
        TouchInput touchinput;
        touchinput.type = correctTouchTypeForGlist(touch.type);
        touchinput.fingerid = fingerid;
        touchinput.pointerindex = 0;
        touchinput.x = point.x;
        touchinput.y = point.y;
        
        touchinputs[0] = touchinput;
        
        fireEvent<gTouchEvent>(count, touchinputs, 0, ACTIONTYPE_UP);
        
        fingerid++;
    }
    
    delete[] touchinputs;
}
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event
{
    NSArray<UITouch*>* allTouches = [touches allObjects];
    
    int fingerid = 0;
    int count = [allTouches count];
    
    TouchInput* touchinputs = new TouchInput[count];
    
    for (UITouch *touch in allTouches) {
        CGPoint point = [touch locationInView: getView()];
        
        TouchInput touchinput;
        touchinput.type = correctTouchTypeForGlist(touch.type);
        touchinput.fingerid = fingerid;
        touchinput.pointerindex = 0;
        touchinput.x = point.x;
        touchinput.y = point.y;
        
        touchinputs[0] = touchinput;
        
        fireEvent<gTouchEvent>(count, touchinputs, 0, ACTIONTYPE_UP);
        
        fingerid++;
    }
    
    delete[] touchinputs;
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
