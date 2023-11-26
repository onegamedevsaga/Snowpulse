#ifndef SNOWPULSE_APPLICATION_METAL_RENDERQUEUEMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_RENDERQUEUEMETAL_H_

#include "../../defines.h"
#include "../../game/drawable.h"

#include <memory>
#include <vector>

#include "../render_batch.h"
#include "render_batch_data_metal.h"

namespace snowpulse {
class SNOWPULSEAPI RenderQueueMetal {
    public:
        static std::shared_ptr<RenderQueueMetal> Create();

        virtual ~RenderQueueMetal();

        void Push(std::shared_ptr<RenderBatch> batch);
        std::vector<std::shared_ptr<RenderBatchDataMetal>> PopAllData();

    private:
        RenderQueueMetal();
        void SortBatches();
        void GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches);

        std::vector<std::shared_ptr<RenderBatch>> batches_;
};
}   // namespace snowpulse

#endif
