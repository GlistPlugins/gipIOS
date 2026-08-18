#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <GLKit/GLKView.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/EAGLIOSurface.h>
#import <QuartzCore/CAMetalLayer.h>

#import "gIOSViewController.h"
#import "gIOSInterface.h"

static UIView* mainView;
static CAMetalLayer* mainMetalLayer;
static ViewBounds viewBounds = {0, 0};

@implementation gIOSViewController

-(instancetype)init
{
    self = [super init];
    if(self) display_link = nil;
    return self;
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
    if(isIOSVulkanRequested()) {
        UIView* view = self.view;
        mainMetalLayer = [CAMetalLayer layer];
        mainMetalLayer.frame = view.bounds;
        mainMetalLayer.contentsScale = UIScreen.mainScreen.nativeScale;
        [view.layer addSublayer:mainMetalLayer];
        mainView = view;
        viewBounds = {static_cast<float>(view.bounds.size.width * mainMetalLayer.contentsScale),
                static_cast<float>(view.bounds.size.height * mainMetalLayer.contentsScale)};
        setup();
        display_link = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawVulkanFrame:)];
        display_link.preferredFramesPerSecond = UIScreen.mainScreen.maximumFramesPerSecond;
        [display_link addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
    } else {
        [self setupGL];
        setup();
    }
}

-(void) viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    if(!mainMetalLayer) return;
    mainMetalLayer.frame = self.view.bounds;
    const int width = static_cast<int>(self.view.bounds.size.width * mainMetalLayer.contentsScale);
    const int height = static_cast<int>(self.view.bounds.size.height * mainMetalLayer.contentsScale);
    viewBounds = {static_cast<float>(width), static_cast<float>(height)};
    gIOSWindow* window = gIOSWindow::getWindow();
    if(window && width > 0 && height > 0) window->setSize(width, height);
}

-(void) drawVulkanFrame:(CADisplayLink*)sender
{
    (void)sender;
    loop();
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

void* getIOSMetalLayer()
{
    return (__bridge void*)mainMetalLayer;
}

ViewBounds getViewBounds()
{
    return viewBounds;
}
