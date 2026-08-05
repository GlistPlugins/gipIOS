#import "gIOSAppDelegate.h"

#import <UIKit/UIStoryboard.h>

#import "gIOSViewController.h"
#import "gIOSInterface.h"
#import "gIOSWindow.h"

#include <unordered_map>

static bool g_IsTerminating = false;
static std::unordered_map<const void*, int> activeTouchIds;


InputType correctTouchTypeForGlist(UITouchType type) {
    switch(type) {
        case UITouchTypeDirect:
            return INPUTTYPE_FINGER;

        case UITouchTypePencil:
            return INPUTTYPE_STYLUS;

        default:
            return INPUTTYPE_UNKNOWN;
    }
}


static const void* getTouchKey(UITouch* touch) {
    return (__bridge const void*)touch;
}


static int findTouchId(UITouch* touch) {
    const void* key = getTouchKey(touch);
    const auto it = activeTouchIds.find(key);

    if(it == activeTouchIds.end()) {
        return -1;
    }

    return it->second;
}


static int createTouchId(UITouch* touch) {
    const int existingId = findTouchId(touch);

    if(existingId >= 0) {
        return existingId;
    }

    for(int candidateId = 0; candidateId < 16; ++candidateId) {
        bool idAlreadyUsed = false;

        for(const auto& entry : activeTouchIds) {
            if(entry.second == candidateId) {
                idAlreadyUsed = true;
                break;
            }
        }

        if(!idAlreadyUsed) {
            activeTouchIds[getTouchKey(touch)] = candidateId;
            return candidateId;
        }
    }

    return -1;
}


static void removeTouchId(UITouch* touch) {
    activeTouchIds.erase(getTouchKey(touch));
}

static void sendTouchEvent(UITouch* touch, int fingerId, ActionType action) {
    UIView* view = getView();

    if(view == nil || touch == nil || fingerId < 0) {
        return;
    }

    const CGPoint point = [touch locationInView:view];
    const CGFloat scale = view.contentScaleFactor;

    TouchInput input{};
    input.type = correctTouchTypeForGlist(touch.type);
    input.fingerid = fingerId;
    input.pointerindex = 0;
    input.x = static_cast<int>(point.x * scale);
    input.y = static_cast<int>(point.y * scale);

    fireEvent<gTouchEvent>(1, &input, 0, action);
}


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
    activeTouchIds.clear();
    fireEvent<gAppPauseEvent>();
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    activeTouchIds.clear();
    g_IsTerminating = true;
}

- (void)touchesBegan:(NSSet<UITouch*> *)touches withEvent:(nullable UIEvent *)event
{
    for(UITouch* touch in touches) {
        const int fingerId = createTouchId(touch);

        if(fingerId < 0) {
            continue;
        }

        sendTouchEvent(touch, fingerId, ACTIONTYPE_DOWN);
    }
}


- (void)touchesMoved:(NSSet<UITouch*>*)touches withEvent:(nullable UIEvent*)event {
    for(UITouch* touch in touches) {
        int fingerId = findTouchId(touch);

        /*
         * Normally the touch already has an ID. This fallback
         * prevents an unusual iOS event from losing the touch.
         */
        if(fingerId < 0) {
            fingerId = createTouchId(touch);
        }

        if(fingerId < 0) {
            continue;
        }

        sendTouchEvent(touch, fingerId, ACTIONTYPE_MOVE);
    }
}


- (void)touchesEnded:(NSSet<UITouch*>*)touches withEvent:(nullable UIEvent*)event {
    for(UITouch* touch in touches) {
        const int fingerId = findTouchId(touch);

        if(fingerId < 0) {
            continue;
        }

        sendTouchEvent(touch, fingerId, ACTIONTYPE_UP);
        removeTouchId(touch);
    }
}


- (void)touchesCancelled:(NSSet<UITouch*>*)touches withEvent:(nullable UIEvent*)event {
    for(UITouch* touch in touches) {
        const int fingerId = findTouchId(touch);

        if(fingerId < 0) {
            continue;
        }

        /*
         * Treat a cancelled touch as released so joystick,
         * firing or camera controls cannot remain stuck.
         */
        sendTouchEvent(touch, fingerId, ACTIONTYPE_UP);

        removeTouchId(touch);
    }
}

@end

bool getIsTerminating()
{
    return g_IsTerminating;
}
