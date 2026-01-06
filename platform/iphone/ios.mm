
#include "ios.h"
#include <sys/sysctl.h>

#import <UIKit/UIKit.h>

void iOS::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_rate_url", "app_id"), &iOS::get_rate_url);
};

void iOS::alert(const char *p_alert, const char *p_title) {
	UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:p_title] message:[NSString stringWithUTF8String:p_alert] delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] autorelease];
	[alert show];
}

String iOS::get_model() const {
	// [[UIDevice currentDevice] model] only returns "iPad" or "iPhone".
	size_t size;
	sysctlbyname("hw.machine", NULL, &size, NULL, 0);
	char *model = (char *)malloc(size);
	if (model == NULL) {
		return "";
	}
	sysctlbyname("hw.machine", model, &size, NULL, 0);
	NSString *platform = [NSString stringWithCString:model encoding:NSUTF8StringEncoding];
	free(model);
	const char *str = [platform UTF8String];
	return String(str != NULL ? str : "");
}

String iOS::get_rate_url(int p_app_id) const {
	String templ = "itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=APP_ID";
	String templ_iOS7 = "itms-apps://itunes.apple.com/app/idAPP_ID";
	String templ_iOS8 = "itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=APP_ID&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software";

	//ios7 before
	String ret = templ;

	if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0 && [[[UIDevice currentDevice] systemVersion] floatValue] < 7.1) {
		// iOS 7 needs a different templateReviewURL @see https://github.com/arashpayan/appirater/issues/131
		ret = templ_iOS7;
	} else if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
		// iOS 8 needs a different templateReviewURL also @see https://github.com/arashpayan/appirater/issues/182
		ret = templ_iOS8;
	}

	// ios7 for everything?
	ret = templ_iOS7.replace("APP_ID", String::num(p_app_id));

	printf("returning rate url %ls\n", ret.c_str());
	return ret;
};

iOS::iOS(){};
