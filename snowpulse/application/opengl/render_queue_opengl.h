#ifndef SNOWPULSE_APPLICATION_OPENGL_RENDERQUEUEOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_RENDERQUEUEOPENGL_H_

#include "../../defines.h"
#include "../../game/drawable.h"

#include <memory>
#include <vector>

#include "../render_batch.h"
#include "render_batch_data_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI RenderQueueOpenGL {
    public:
        static std::shared_ptr<RenderQueueOpenGL> Create();

        virtual ~RenderQueueOpenGL();

        void Push(std::shared_ptr<RenderBatch> batch);
        std::vector<std::shared_ptr<RenderBatchDataOpenGL>> PopAllData();

    private:
        RenderQueueOpenGL();
        void SortBatches();
        void GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches);

        std::vector<std::shared_ptr<RenderBatch>> batches_;
};
}   // namespace snowpulse

#endif
