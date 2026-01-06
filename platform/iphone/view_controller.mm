
#import "view_controller.h"

#include "os_iphone.h"

#include "core/project_settings.h"

extern "C" {

int add_path(int, char **);
int add_cmdline(int, char **);

int add_path(int p_argc, char **p_args) {

	NSString *str = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"godot_path"];
	if (!str)
		return p_argc;

	p_args[p_argc++] = "--path";
	[str retain]; // memory leak lol (maybe make it static here and delete it in ViewController destructor? @todo
	p_args[p_argc++] = (char *)[str cString];
	p_args[p_argc] = NULL;

	return p_argc;
};

int add_cmdline(int p_argc, char **p_args) {

	NSArray *arr = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"godot_cmdline"];
	if (!arr)
		return p_argc;

	for (int i = 0; i < [arr count]; i++) {

		NSString *str = [arr objectAtIndex:i];
		if (!str)
			continue;
		[str retain]; // @todo delete these at some point
		p_args[p_argc++] = (char *)[str cString];
	};

	p_args[p_argc] = NULL;

	return p_argc;
};
}; // extern "C"

@interface ViewController ()

@end

@implementation ViewController

- (void)didReceiveMemoryWarning {

	printf("*********** did receive memory warning!\n");
};

- (void)viewDidLoad {
	[super viewDidLoad];

	if (@available(iOS 11.0, *)) {
		[self setNeedsUpdateOfScreenEdgesDeferringSystemGestures];
	}
}

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures {
	return UIRectEdgeAll;
}

- (BOOL)shouldAutorotate {
	switch (OS::get_singleton()->get_screen_orientation()) {
		case OS::SCREEN_SENSOR:
		case OS::SCREEN_SENSOR_LANDSCAPE:
		case OS::SCREEN_SENSOR_PORTRAIT:
			return YES;
		default:
			return NO;
	}
};

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
	switch (OS::get_singleton()->get_screen_orientation()) {
		case OS::SCREEN_PORTRAIT:
			return UIInterfaceOrientationMaskPortrait;
		case OS::SCREEN_REVERSE_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscapeRight;
		case OS::SCREEN_REVERSE_PORTRAIT:
			return UIInterfaceOrientationMaskPortraitUpsideDown;
		case OS::SCREEN_SENSOR_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscape;
		case OS::SCREEN_SENSOR_PORTRAIT:
			return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
		case OS::SCREEN_SENSOR:
			return UIInterfaceOrientationMaskAll;
		case OS::SCREEN_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscapeLeft;
	}
};

- (BOOL)prefersStatusBarHidden {
	return YES;
}

- (BOOL)prefersHomeIndicatorAutoHidden {
	if (GLOBAL_GET("display/window/ios/hide_home_indicator")) {
		return YES;
	} else {
		return NO;
	}
}

#ifdef GAME_CENTER_ENABLED
- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController {
	//[gameCenterViewController dismissViewControllerAnimated:YES completion:^{GameCenter::get_singleton()->game_center_closed();}];//version for signaling when overlay is completely gone
	GameCenter::get_singleton()->game_center_closed();
	[gameCenterViewController dismissViewControllerAnimated:YES completion:nil];
}
#endif

@end
