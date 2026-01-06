
/**
 * @brief AppDelegate for iOS application lifecycle management
 * 
 * Handles the main application delegate responsibilities for iOS, including
 * application initialization, lifecycle events, and view controller management.
 * 
 * @note Currently configured for Vulkan rendering. Future implementation should
 *       support both OpenGL ES 2 and Vulkan through runtime selection rather than
 *       compile-time branching to improve flexibility.
 * 
 * @property window The main UIWindow for the application
 * @property view_controller The primary view controller managing the app's UI
 * @property is_focus_out Flag indicating whether the application has lost focus
 * 
 * @remarks Uses CoreMotion framework for device motion tracking capabilities
 * 
 * @see GLViewDelegate, ViewController
 */
#if defined(OPENGL_ENABLED)
#import "gl_view.h"
#endif
#import "view_controller.h"
#import <UIKit/UIKit.h>

#import <CoreMotion/CoreMotion.h>

// FIXME: Add support for both GLES2 and Vulkan when GLES2 is implemented again,
// so it can't be done with compilation time branching.
//#if defined(OPENGL_ENABLED)
//@interface AppDelegate : NSObject <UIApplicationDelegate, GLViewDelegate> {
//#endif
#if defined(VULKAN_ENABLED)
@interface AppDelegate : NSObject <UIApplicationDelegate> {
#endif
	//@property (strong, nonatomic) UIWindow *window;
	ViewController *view_controller;
	bool is_focus_out;
};

@property(strong, nonatomic) UIWindow *window;

+ (ViewController *)getViewController;

@end
