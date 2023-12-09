#import "GameViewController.h"

#include <iostream>
#include <map>
#include <snowpulse.h>
#include <game.h>

@implementation GameViewController {
    bool hasInitializedApplication;
    MTKView* view_;
    std::shared_ptr<game::Game> game_;
    snowpulse::ApplicationMacOS* app_;
    snowpulse::InputMacOS* input_;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    hasInitializedApplication = false;
}

- (void)viewDidAppear {
    [super viewDidAppear];
    NSWindow *window = [self.view window];
    //[window toggleFullScreen:nil];

    if (self.view.window) {
        [window addObserver:self
                forKeyPath:@"frame"
                options:NSKeyValueObservingOptionNew
                context:NULL];
    }

    if (hasInitializedApplication) {
        return;
    }

    view_ = (MTKView *)self.view;
    view_.device = MTLCreateSystemDefaultDevice();
    if(!view_.device) {
        NSLog(@"Metal is not supported on this device");
        self.view = [[NSView alloc] initWithFrame:self.view.frame];
        return;
    }

    [window makeFirstResponder:self.view];
    NSRect frame = [window frame];
    NSRect contentRect = [NSWindow contentRectForFrameRect:frame styleMask:[window styleMask]];
    CGSize screenSize = contentRect.size;

    game_ = game::Game::Create();
    app_ = static_cast<snowpulse::ApplicationMacOS*>(snowpulse::Application::GetInstance());
    if (app_->Initialize(snowpulse::Vector2Int(1920, 1080), snowpulse::Vector2Int((int)screenSize.width, (int)screenSize.height), (__bridge void*)view_)) {
        app_->SetAssetFolderPath("assets/");
        app_->SetGame(game_.get());
        game_->Initialize(app_);
        input_ = static_cast<snowpulse::InputMacOS*>(snowpulse::Input::GetInstance());
    }
    hasInitializedApplication = true;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context {
    if ([keyPath isEqualToString:@"frame"]) {
        auto screenSize = self.view.frame.size;
        app_->SetScreenSize(snowpulse::Vector2Int((int)screenSize.width, (int)screenSize.height));
    }
}

- (void)viewWillDisappear {
    [super viewWillDisappear];
    if (self.view.window) {
        [self.view.window removeObserver:self forKeyPath:@"frame"];
    }
}

- (void)dealloc {
    app_->Shutdown();
}

- (void)keyDown:(NSEvent *)event {
    std::string key = [[event characters] UTF8String];
    input_->ProcessInputs(key, true);
}

- (void)keyUp:(NSEvent *)event {
    std::string key = [[event characters] UTF8String];
    input_->ProcessInputs(key, false);
}

- (void)mouseDown:(NSEvent *)event {
    NSPoint locationInView = [self.view convertPoint:[event locationInWindow] fromView:nil];
    NSInteger mouseButton = [event buttonNumber];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y), 0, true);
}

- (void)rightMouseDown:(NSEvent *)event {
    NSPoint locationInView = [self.view convertPoint:[event locationInWindow] fromView:nil];
    NSInteger mouseButton = [event buttonNumber];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y), 1, true);
}

- (void)otherMouseDown:(NSEvent *)event {
    NSPoint locationInView = [self.view convertPoint:[event locationInWindow] fromView:nil];
    input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), snowpulse::Vector2(locationInView.x, locationInView.y), event.buttonNumber , true);
}

- (void)mouseUp:(NSEvent *)event {
    // Handle mouse up event
}
@end
