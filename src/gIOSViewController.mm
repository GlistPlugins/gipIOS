#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <GLKit/GLKView.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/EAGLIOSurface.h>

#import "gIOSViewController.h"
#import "gIOSInterface.h"

#include "gKeyCode.h"

static UIView* mainView;
static ViewBounds viewBounds = {0, 0};

// Hidden zero-size view that owns the soft keyboard. The user never sees or
// touches it; showIOSKeyboard() makes it first responder, which raises the IME,
// and the UIKeyInput callbacks forward everything into the engine event path.
// Same pattern as the Android GlistInputView (a hidden EditText): routing input
// through a dedicated responder instead of the GL view keeps the keyboard from
// resizing or recreating the GL surface.
@interface GlistKeyInputView : UIView <UIKeyInput>
@end

@implementation GlistKeyInputView

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

- (BOOL)hasText
{
    // Pretend there is always text so the keyboard keeps sending deleteBackward.
    return YES;
}

- (UITextAutocorrectionType)autocorrectionType
{
    return UITextAutocorrectionTypeNo;
}

- (UITextSpellCheckingType)spellCheckingType
{
    return UITextSpellCheckingTypeNo;
}

- (UIReturnKeyType)returnKeyType
{
    return UIReturnKeyDone;
}

- (void)insertText:(NSString*)text
{
    // The keyboard commits whole strings; the engine consumes single code
    // points. Return arrives here as "\n" and is forwarded as the enter key,
    // matching what the Android input connection sends.
    NSData* data = [text dataUsingEncoding:NSUTF32LittleEndianStringEncoding];
    const uint32_t* codepoints = (const uint32_t*)data.bytes;
    NSUInteger count = data.length / sizeof(uint32_t);
    for(NSUInteger i = 0; i < count; i++) {
        if(codepoints[i] == '\n') {
            fireEvent<gKeyPressedEvent>(G_KEY_ENTER);
            fireEvent<gKeyReleasedEvent>(G_KEY_ENTER);
        } else {
            fireEvent<gCharTypedEvent>((unsigned int)codepoints[i]);
        }
    }
}

- (void)deleteBackward
{
    fireEvent<gKeyPressedEvent>(G_KEY_BACKSPACE);
    fireEvent<gKeyReleasedEvent>(G_KEY_BACKSPACE);
}

@end

static GlistKeyInputView* keyInputView = nil;

static DeviceOrientation currentGlistOrientation(UIWindow* window, CGSize size);

@implementation gIOSViewController

-(instancetype)init
{
    self = [super init];
    return self;
}

-(void) setupGL
{
    fbo_initialized = false;
    GLKView* view = (GLKView*)self.view;
    m_Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    view.context = m_Context;
    self.delegate = self;
    
    [EAGLContext setCurrentContext:view.context];
    
    viewBounds = {static_cast<float>(view.bounds.size.width), static_cast<float>(view.bounds.size.height)};
    
    mainView = view;
}

-(void) viewDidLoad
{
    [super viewDidLoad];
    [self setupGL];
}

- (void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if(!fbo_initialized)
    {
        GLint defaultFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);

        setDefaultFbo(defaultFBO);

        fbo_initialized = true;

        // The engine is brought up on the first frame rather than in
        // viewDidLoad: only now, after layout, does the drawable report the
        // screen's real pixel size (in viewDidLoad the storyboard still holds
        // its design-time bounds, e.g. 320 wide). The engine works in this
        // pixel space, the same convention as the Android surface.
        viewBounds = {static_cast<float>(view.drawableWidth), static_cast<float>(view.drawableHeight)};
        setup();
    }

    loop();
}

- (void) glkViewControllerUpdate:(GLKViewController *)controller
{
}

- (void) viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    // The new bounds and interface orientation are only final once the rotation
    // animation completes, so the engine is notified from the completion block.
    // Everything here runs on the main thread, which is also the thread the
    // GLKView loop draws on, so the events can be fired directly.
    [coordinator animateAlongsideTransition:nil completion:^(id<UIViewControllerTransitionCoordinatorContext> context) {
        // The drawable has been resized by the time the transition completes;
        // its pixel size is the space the engine works in.
        GLKView* view = (GLKView*)self.view;
        viewBounds = {static_cast<float>(view.drawableWidth), static_cast<float>(view.drawableHeight)};
        gIOSWindow* window = gIOSWindow::getWindow();
        if(!window) {
            return;
        }
        // Orientation first, size second: the resize handler uses the surface
        // shape itself to detect rotation, but the canvas callback fired for
        // the orientation event should already see the final orientation.
        fireEvent<gDeviceOrientationChangedEvent>(currentGlistOrientation(self.view.window, size));
        window->setSize(static_cast<int>(view.drawableWidth), static_cast<int>(view.drawableHeight));
    }];
}

@end

static DeviceOrientation currentGlistOrientation(UIWindow* window, CGSize size)
{
    UIInterfaceOrientation orientation = UIInterfaceOrientationUnknown;
    if(@available(iOS 13.0, *)) {
        orientation = window.windowScene.interfaceOrientation;
    }
    switch(orientation) {
        case UIInterfaceOrientationPortrait: return DEVICEORIENTATION_PORTRAIT;
        case UIInterfaceOrientationPortraitUpsideDown: return DEVICEORIENTATION_REVERSE_PORTRAIT;
        case UIInterfaceOrientationLandscapeRight: return DEVICEORIENTATION_LANDSCAPE;
        case UIInterfaceOrientationLandscapeLeft: return DEVICEORIENTATION_REVERSE_LANDSCAPE;
        default: break;
    }
    // No window scene to ask (or pre-iOS 13): fall back to the surface shape.
    return size.width > size.height ? DEVICEORIENTATION_LANDSCAPE : DEVICEORIENTATION_PORTRAIT;
}

void showIOSKeyboard()
{
    // Called from the engine loop; hop to the main queue for UIKit work. The
    // input view is created lazily because the GL view does not exist yet when
    // the engine starts up.
    dispatch_async(dispatch_get_main_queue(), ^{
        if(!mainView) {
            return;
        }
        if(!keyInputView) {
            keyInputView = [[GlistKeyInputView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
            [mainView addSubview:keyInputView];
        }
        [keyInputView becomeFirstResponder];
    });
}

void hideIOSKeyboard()
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [keyInputView resignFirstResponder];
    });
}

UIView* getView()
{
    return mainView;
}

ViewBounds getViewBounds()
{
    return viewBounds;
}
