#include <snowpulse.h>
#include <application/metal/app_delegate_metal.h>

int main( int argc, char* argv[] )
{
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate( &del );
    pSharedApplication->run();

    pAutoreleasePool->release();

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
