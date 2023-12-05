//
//  GameViewController.m
//  launcher_ios
//
//  Created by Marc Jamero on 11/28/23.
//

#import "GameViewController.h"
#import "Renderer.h"

#include <iostream>
#include <map>
#include <snowpulse.h>
#include <game.h>

@implementation GameViewController
{
    MTKView* view_;

    //Renderer *_renderer;
    std::shared_ptr<game::Game> game_;
    snowpulse::ApplicationIOS* app_;
    snowpulse::InputIOS* input_;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    NSLog(@"Screen Width: %f, Screen Height: %f", screenSize.width, screenSize.height);
    
    view_ = (MTKView *)self.view;
    view_.device = MTLCreateSystemDefaultDevice();
    view_.backgroundColor = UIColor.greenColor;

    if(!view_.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
    
    game_ = game::Game::Create();
    app_ = static_cast<snowpulse::ApplicationIOS*>(snowpulse::Application::GetInstance());

    if (app_->Initialize(snowpulse::Vector2Int(2688, 1242), snowpulse::Vector2Int((int)screenSize.width, (int)screenSize.height), (__bridge void*)view_)) {
        app_->SetGame(game_.get());
        game_->Initialize(app_);
        input_ = static_cast<snowpulse::InputIOS*>(snowpulse::InputIOS::GetInstance());
    }
    
    
    //_view.device
    //_renderer = [[Renderer alloc] initWithMetalKitView:_view];

    //[_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];

    //_view.delegate = _renderer;
}


- (void)dealloc {
    app_->Shutdown();
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        //void* t = (__bridge void*)[NSValue valueWithNonretainedObject:touch];
        void* t = (__bridge void*)touch;
        CGPoint touchLocation = [touch locationInView:self.view];
        input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), t, snowpulse::TouchPhase::kBegan, snowpulse::Vector2Int(touchLocation.x, touchLocation.y));
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        //void* t = (__bridge void*)[NSValue valueWithNonretainedObject:touch];
        void* t = (__bridge void*)touch;
        CGPoint touchLocation = [touch locationInView:self.view];
        input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), t, snowpulse::TouchPhase::kMoved, snowpulse::Vector2Int(touchLocation.x, touchLocation.y));
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        //void* t = (__bridge void*)[NSValue valueWithNonretainedObject:touch];
        void* t = (__bridge void*)touch;
        CGPoint touchLocation = [touch locationInView:self.view];
        input_->ProcessInputs(app_->GetResolutionSize(), app_->GetScreenSize(), t, snowpulse::TouchPhase::kEnded, snowpulse::Vector2Int(touchLocation.x, touchLocation.y));
    }
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    // Handle touch cancel event
}

@end
