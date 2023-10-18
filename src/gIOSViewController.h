#import <GLKit/GLKit.h>

@interface gIOSViewController : GLKViewController <GLKViewControllerDelegate>
{
    EAGLContext* m_Context;
    bool fbo_initialized;
}
-(void) setupGL;

@end

UIView* getView();
