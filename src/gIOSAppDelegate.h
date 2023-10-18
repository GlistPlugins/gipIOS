#import <UIKit/UIKit.h>
#import "gWindowEvents.h"

@interface gIOSAppDelegate : UIResponder<UIApplicationDelegate>
@property (strong, nonatomic) UIWindow* window;
@end

InputType correctTouchTypeForGlist(UITouchType type);
