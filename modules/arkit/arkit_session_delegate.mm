
#include "arkit_session_delegate.h"
#include "arkit_interface.h"

@implementation ARKitSessionDelegate

@synthesize arkit_interface;

- (void)session:(ARSession *)session didAddAnchors:(NSArray<ARAnchor *> *)anchors {
	for (ARAnchor *anchor in anchors) {
		arkit_interface->_add_or_update_anchor(anchor);
	}
}

- (void)session:(ARSession *)session didRemoveAnchors:(NSArray<ARAnchor *> *)anchors {
	for (ARAnchor *anchor in anchors) {
		arkit_interface->_remove_anchor(anchor);
	}
}

- (void)session:(ARSession *)session didUpdateAnchors:(NSArray<ARAnchor *> *)anchors {
	for (ARAnchor *anchor in anchors) {
		arkit_interface->_add_or_update_anchor(anchor);
	}
}

@end
