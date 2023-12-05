#ifndef SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_

#include "../application.h"

#include <string>
#include <memory>

#include "../../defines.h"
#include "render_queue_metal.h"
#include "graphics_metal.h"

namespace snowpulse {

class ViewDelegateMetal;

class SNOWPULSEAPI ApplicationMetal : public Application {
    public:
        virtual ~ApplicationMetal();
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* view);
        virtual void Shutdown() override;

        virtual void RunFrame();

        Graphics* GetGraphics() const override { return graphics_.get(); }

    protected:
        ApplicationMetal();

        std::shared_ptr<GraphicsMetal> graphics_;
        std::shared_ptr<ViewDelegateMetal> viewDelegate_;
        MTK::View* view_;
};
}   // namespace snowpulse

#endif
