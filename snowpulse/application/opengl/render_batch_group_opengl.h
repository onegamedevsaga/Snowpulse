#ifndef SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHGROUPOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHGROUPOPENGL_H_

#include "../../defines.h"
#include "../render_batch.h"

#include <memory>
#include <vector>

#include "render_batch_data_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI RenderBatchGroupOpenGL : public RenderBatch {
    public:
        RenderBatchGroupOpenGL() : RenderBatch(RenderBatchType::kGroup) { }
        ~RenderBatchGroupOpenGL() { }

        std::vector<std::shared_ptr<RenderBatch>> GetBatches() const { return batches_; }

        void AddBatch(std::shared_ptr<RenderBatch> batch) {
            batches_.push_back(batch);
        }

    private:
        std::vector<std::shared_ptr<RenderBatch>> batches_;
};
}   // namespace snowpulse
#endif
