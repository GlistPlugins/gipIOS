#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <GLKit/GLKView.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/EAGLIOSurface.h>

#import "gIOSViewController.h"
#import "gIOSInterface.h"

static UIView* mainView;
static ViewBounds viewBounds = {0, 0};

@implementation gIOSViewController

-(instancetype)init
{
    self = [super init];
}

-(void) setupGL
{
    fbo_initialized = false;
    GLKView* view = (GLKView*)self.view;
    m_Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    view.context = m_Context;
    view.multipleTouchEnabled = YES;
    self.delegate = self;
    self.preferredFramesPerSecond = 60;
    self.paused = NO;
    view.enableSetNeedsDisplay = NO;
    
    [EAGLContext setCurrentContext:view.context];
    CGFloat scale = view.contentScaleFactor;
    viewBounds = {static_cast<float>(view.bounds.size.width * scale), static_cast<float>(view.bounds.size.height * scale)};
    
    mainView = view;
}

-(void) viewDidLoad
{
    [super viewDidLoad];
    [self setupGL];

    setup();
}

- (void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if(!fbo_initialized)
    {
        GLint defaultFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
        
        setDefaultFbo(defaultFBO);
        
        fbo_initialized = true;
    }
    glViewport(0, 0, (GLsizei)view.drawableWidth, (GLsizei)view.drawableHeight);
    loop();
}

- (void) glkViewControllerUpdate:(GLKViewController *)controller
{
}

@end

UIView* getView()
{
    return mainView;
}

ViewBounds getViewBounds()
{
    return viewBounds;
}
