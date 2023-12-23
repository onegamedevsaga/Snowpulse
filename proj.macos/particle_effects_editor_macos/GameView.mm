#import "GameView.h"

#include <snowpulse.h>

@implementation GameView
{
    snowpulse::ApplicationMacOS* app_;
    snowpulse::InputMacOS* input_;
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];
    for (NSTrackingArea *trackingArea in self.trackingAreas) {
        [self removeTrackingArea:trackingArea];
    }
    NSTrackingAreaOptions trackingOptions = NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp | NSTrackingMouseMoved;
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                                options:trackingOptions
                                                                  owner:self
                                                               userInfo:nil];
    [self addTrackingArea:trackingArea];
}

// Implement mouseEntered: and mouseExited: as needed
- (void)mouseEntered:(NSEvent *)event {
    // Your code for mouse entered
}

- (void)mouseExited:(NSEvent *)event {
    // Your code for mouse exited
}

- (void)mouseMoved:(NSEvent *)event {
    if (!app_) {
        app_ = static_cast<snowpulse::ApplicationMacOS*>(snowpulse::Application::GetInstance());
    }
    if (!input_) {
        input_ = static_cast<snowpulse::InputMacOS*>(snowpulse::Input::GetInstance());
    }
    NSPoint locationInView = [self convertPoint:[event locationInWindow] fromView:nil];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y));
}

- (void)mouseDragged:(NSEvent *)event {
    [super mouseDragged:event];
    NSPoint locationInView = [self convertPoint:event.locationInWindow fromView:nil];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y));
}

- (void)righMouseDragged:(NSEvent *)event {
    [super rightMouseDragged:event];
    NSPoint locationInView = [self convertPoint:event.locationInWindow fromView:nil];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y));
}

- (void)otherMouseDragged:(NSEvent *)event {
    [super otherMouseDragged:event];
    NSPoint locationInView = [self convertPoint:event.locationInWindow fromView:nil];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y));
}

- (void)scrollWheel:(NSEvent *)event {
    [super scrollWheel:event];

    // Get the scrolling delta
    CGFloat deltaX = [event scrollingDeltaX];
    CGFloat deltaY = [event scrollingDeltaY];
    input_->ProcessInputs(snowpulse::Vector2(deltaX, deltaY));
}
@end
