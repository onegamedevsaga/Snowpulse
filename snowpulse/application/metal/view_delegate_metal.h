#ifndef SNOWPULSE_APPLICATION_METAL_VIEWDELEGATEMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_VIEWDELEGATEMETAL_H_

#include "../../defines.h"

#include <string>
#include <memory>
#include <MetalKit/MetalKit.hpp>

#include "application_metal.h"

namespace snowpulse {
class SNOWPULSEAPI ViewDelegateMetal : public MTK::ViewDelegate {
    public:
        ViewDelegateMetal(ApplicationMetal* app);
        virtual ~ViewDelegateMetal();
        void drawInMTKView(MTK::View* view);

    private:
        ApplicationMetal* app_;
};
}   // namespace snowpulse
#endif
