
/**
 * @class ViewController
 * @brief Main view controller for the iOS platform implementation.
 * 
 * Manages the game engine's presentation on iOS devices and handles
 * Game Center functionality. This controller manages autorotation,
 * system UI preferences, and memory warnings.
 */

/**
 * @brief Determines if the view controller should autorotate to a given interface orientation.
 * @param p_orientation The target interface orientation.
 * @return YES if the controller should rotate to the specified orientation, NO otherwise.
 */

/**
 * @brief Handles low memory warnings from the system.
 * 
 * Called when the system notifies about insufficient available memory.
 * Used to release non-essential resources.
 */

/**
 * @brief Called when the view hierarchy is loaded into memory.
 * 
 * Performs initial setup and configuration of the view controller
 * after the view hierarchy is created.
 */

/**
 * @brief Specifies which screen edges should defer system gesture recognition.
 * @return A bitmask of UIRectEdge values indicating edges that should defer gestures.
 */

/**
 * @brief Determines whether the status bar should be hidden.
 * @return YES if the status bar should be hidden, NO otherwise.
 */

/**
 * @brief Determines whether the home indicator should be automatically hidden.
 * @return YES if the home indicator should be auto-hidden, NO otherwise.
 */
#import <GameKit/GameKit.h>
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <GKGameCenterControllerDelegate> {
};

- (BOOL)shouldAutorotateToInterfaceOrientation:
		(UIInterfaceOrientation)p_orientation;

- (void)didReceiveMemoryWarning;

- (void)viewDidLoad;

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures;

- (BOOL)prefersStatusBarHidden;

- (BOOL)prefersHomeIndicatorAutoHidden;

@end
