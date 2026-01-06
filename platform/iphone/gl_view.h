
/**
 * @class GLView
 * @brief A UIView subclass for rendering OpenGL ES graphics on iOS devices.
 * 
 * GLView provides a complete OpenGL ES rendering surface with support for:
 * - OpenGL ES 1.0 rendering with framebuffer and renderbuffer management
 * - Animation control via CADisplayLink (iOS 3.1+) or NSTimer fallback
 * - Text input through UIKeyInput protocol implementation
 * - AVPlayer integration for video playback
 * - Keyboard input handling with delegate callback support
 * - Audio route change detection
 * 
 * @properties
 *   delegate: GLViewDelegate - The delegate responsible for drawing operations
 *   avAsset: AVAsset - The media asset for playback
 *   avPlayerItem: AVPlayerItem - The current player item
 *   avPlayer: AVPlayer - The video player instance
 *   avPlayerLayer: AVPlayerLayer - The layer for video rendering
 *   backgroundWindow: UIWindow - Background window for video rendering
 *   autocorrectionType: UITextAutocorrectionType - Text input autocorrection setting
 *   animationInterval: NSTimeInterval - Time interval between animation frames
 *   useCADisplayLink: BOOL - Whether to use CADisplayLink for animation synchronization
 * 
 * @methods
 *   startAnimation - Begin the animation loop
 *   stopAnimation - Stop the animation loop
 *   drawView - Manually trigger a drawing update
 *   open_keyboard - Display the on-screen keyboard
 *   hide_keyboard - Hide the on-screen keyboard
 *   deleteBackward - Handle backspace key input
 *   hasText - Check if text input field has content
 *   insertText: - Insert text from keyboard input
 *   initGLES - Initialize OpenGL ES context
 *   createFramebuffer - Create the renderbuffer and framebuffer objects
 *   destroyFramebuffer - Release renderbuffer and framebuffer resources
 *   audioRouteChangeListenerCallback: - Handle audio route changes
 *   keyboardOnScreen: - Handle keyboard appearance notification
 *   keyboardHidden: - Handle keyboard disappearance notification
 */
#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <UIKit/UIKit.h>

@protocol GLViewDelegate;

@interface GLView : UIView <UIKeyInput> {
@private
	// The pixel dimensions of the backbuffer
	GLint backingWidth;
	GLint backingHeight;

	EAGLContext *context;

	// OpenGL names for the renderbuffer and framebuffers used to render to this view
	GLuint viewRenderbuffer, viewFramebuffer;

	// OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist)
	GLuint depthRenderbuffer;

	BOOL useCADisplayLink;
	// CADisplayLink available on 3.1+ synchronizes the animation timer & drawing with the refresh rate of the display, only supports animation intervals of 1/60 1/30 & 1/15
	CADisplayLink *displayLink;

	// An animation timer that, when animation is started, will periodically call -drawView at the given rate.
	// Only used if CADisplayLink is not
	NSTimer *animationTimer;

	NSTimeInterval animationInterval;

	// Delegate to do our drawing, called by -drawView, which can be called manually or via the animation timer.
	id<GLViewDelegate> delegate;

	// Flag to denote that the -setupView method of a delegate has been called.
	// Resets to NO whenever the delegate changes.
	BOOL delegateSetup;
	BOOL active;
	float screen_scale;
}

@property(nonatomic, assign) id<GLViewDelegate> delegate;

// AVPlayer-related properties
@property(strong, nonatomic) AVAsset *avAsset;
@property(strong, nonatomic) AVPlayerItem *avPlayerItem;
@property(strong, nonatomic) AVPlayer *avPlayer;
@property(strong, nonatomic) AVPlayerLayer *avPlayerLayer;

@property(strong, nonatomic) UIWindow *backgroundWindow;

@property(nonatomic) UITextAutocorrectionType autocorrectionType;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;

- (BOOL)canBecomeFirstResponder;

- (void)open_keyboard;
- (void)hide_keyboard;
- (void)deleteBackward;
- (BOOL)hasText;
- (void)insertText:(NSString *)p_text;

- (id)initGLES;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;

- (void)audioRouteChangeListenerCallback:(NSNotification *)notification;
- (void)keyboardOnScreen:(NSNotification *)notification;
- (void)keyboardHidden:(NSNotification *)notification;

@property NSTimeInterval animationInterval;
@property(nonatomic, assign) BOOL useCADisplayLink;

@end

@protocol GLViewDelegate <NSObject>

@required

// Draw with OpenGL ES
- (void)drawView:(GLView *)view;

@optional

// Called whenever you need to do some initialization before rendering.
- (void)setupView:(GLView *)view;

@end
