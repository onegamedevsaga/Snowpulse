#include "render_queue_opengl.h"

#include <algorithm>

namespace snowpulse {

std::shared_ptr<RenderQueueOpenGL> RenderQueueOpenGL::Create() {
    auto renderQueue = new RenderQueueOpenGL();
    return std::shared_ptr<RenderQueueOpenGL>(renderQueue);
}

RenderQueueOpenGL::RenderQueueOpenGL() {
}

RenderQueueOpenGL::~RenderQueueOpenGL() {
}

std::vector<RenderBatchOpenGL> RenderQueueOpenGL::PopAll() {
    SortBatches();
    GroupBatches();
    std::vector<RenderBatchOpenGL> batches = std::move(batches_);
    batches_.clear();
    return batches;
}

void RenderQueueOpenGL::Push(RenderBatchOpenGL batch) {
    batches_.push_back(batch);
}

void RenderQueueOpenGL::SortBatches() {
    std::sort(batches_.begin(), batches_.end(), [](const auto& a, const auto& b) {
        return a.sortOrder < b.sortOrder;
    });
}

void RenderQueueOpenGL::GroupBatches() {
    std::vector<RenderBatchOpenGL> batches = std::move(batches_);
    batches_.clear();
    for (const auto& batch : batches) {
        if (batches_.size() <= 0) {
            batches_.push_back(batch);
        }
        else {
            auto& prevBatch = batches_[batches_.size() - 1];
            if (prevBatch.drawMode == batch.drawMode &&
                prevBatch.viewMatrix == batch.viewMatrix &&
                prevBatch.projectionMatrix == batch.projectionMatrix &&
                prevBatch.shaderProgram == batch.shaderProgram &&
                prevBatch.texture == batch.texture) {
     
                for (const auto& v : batch.vertices) {
                    prevBatch.vertices.push_back(v);
                }
                for (const auto& i : batch.indices) {
                    prevBatch.indices.push_back(prevBatch.vertexCount + i);
                }
                for (const auto& m : batch.matrices) {
                    prevBatch.matrices.push_back(m);
                }
                prevBatch.vertexCount += batch.vertexCount;
                prevBatch.indexCount += batch.indexCount;
            }
            else {
                batches_.push_back(batch);
            }
        }
    }
}
}   // namespace snowpulse
