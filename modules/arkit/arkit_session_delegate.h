
#ifndef ARKIT_SESSION_DELEGATE_H
#define ARKIT_SESSION_DELEGATE_H

#import <ARKit/ARKit.h>
#import <UIKit/UIKit.h>

class ARKitInterface;

@interface ARKitSessionDelegate : NSObject <ARSessionDelegate> {
  ARKitInterface *arkit_interface;
}

@property(nonatomic) ARKitInterface *arkit_interface;

- (void)session:(ARSession *)session
    didAddAnchors:(NSArray<ARAnchor *> *)anchors;
- (void)session:(ARSession *)session
    didRemoveAnchors:(NSArray<ARAnchor *> *)anchors;
- (void)session:(ARSession *)session
    didUpdateAnchors:(NSArray<ARAnchor *> *)anchors;
@end

#endif /* !ARKIT_SESSION_DELEGATE_H */
