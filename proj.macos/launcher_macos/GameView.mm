//
//  GameView.m
//  launcher_macos
//
//  Created by Marc Jamero on 12/7/23.
//

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
@end
