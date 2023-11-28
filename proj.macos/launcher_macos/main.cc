#include <iostream>
#include <snowpulse.h>
#include <game.h>
//#include <application/metal/app_delegate_metal.h>

int main( int argc, char* argv[] )
{
    /*NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate( &del );
    pSharedApplication->run();

    pAutoreleasePool->release();*/
    
    auto game = game::Game::Create();
    auto app = static_cast<snowpulse::ApplicationMacOS*>(snowpulse::Application::GetInstance());

    if (app->Initialize(snowpulse::Vector2Int(1920, 1080), snowpulse::Vector2Int(1067, 600))) {
        app->SetGame(game.get());
        game->Initialize(app);

        app->Run();
        app->Shutdown();
    }

    return 0;
}



/*#ifndef SNOWPULSE_APPLICATION_METAL_APPDELEGATEMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_APPDELEGATEMETAL_H_

#include <string>
#include <memory>

namespace snowpulse {
class SNOWPULSEAPI AppDelegateMetal {
    public:
        virtual ~AppDelegateMetal();

    protected:
        AppDelegateMetal();
};
}   // namespace snowpulse

#endif*/
