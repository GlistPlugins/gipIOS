#import <GLKit/GLKit.h>

@interface gIOSViewController : GLKViewController <GLKViewControllerDelegate>
{
    EAGLContext* m_Context;
    bool fbo_initialized;
    CADisplayLink* display_link;
}
-(void) setupGL;
-(void) drawVulkanFrame:(CADisplayLink*)sender;

@end

UIView* getView();
