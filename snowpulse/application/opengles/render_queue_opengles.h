#ifndef SNOWPULSE_APPLICATION_OPENGLES_RENDERQUEUEOPENGLES_H_
#define SNOWPULSE_APPLICATION_OPENGLES_RENDERQUEUEOPENGLES_H_

#include "../../defines.h"
#include "../../game/drawable.h"

#include <memory>
#include <vector>

#include "../render_batch.h"
#include "render_batch_data_opengles.h"

namespace snowpulse {
class SNOWPULSEAPI RenderQueueOpenGLES {
    public:
        static std::shared_ptr<RenderQueueOpenGLES> Create();

        virtual ~RenderQueueOpenGLES();

        void Push(std::shared_ptr<RenderBatch> batch);
        std::vector<std::shared_ptr<RenderBatchDataOpenGLES>> PopAllData();

    private:
        RenderQueueOpenGLES();
        void SortBatches();
        void GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches);

        std::vector<std::shared_ptr<RenderBatch>> batches_;

};
}   // namespace snowpulse

#endif
