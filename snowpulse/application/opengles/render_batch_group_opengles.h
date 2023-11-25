#ifndef SNOWPULSE_APPLICATION_OPENGLES_RENDERBATCHGROUPOPENGLES_H_
#define SNOWPULSE_APPLICATION_OPENGLES_RENDERBATCHGROUPOPENGLES_H_

#include "../../defines.h"
#include "../render_batch.h"

#include <memory>
#include <vector>

#include "render_batch_data_opengles.h"

namespace snowpulse {
class SNOWPULSEAPI RenderBatchGroupOpenGLES : public RenderBatch {
    public:
        RenderBatchGroupOpenGLES() : RenderBatch(RenderBatchType::kGroup) { }
        ~RenderBatchGroupOpenGLES() { }

        std::vector<std::shared_ptr<RenderBatch>> GetBatches() const { return batches_; }

        void AddBatch(std::shared_ptr<RenderBatch> batch) {
            batches_.push_back(batch);
        }

    private:
        std::vector<std::shared_ptr<RenderBatch>> batches_;
};
}   // namespace snowpulse
#endif
