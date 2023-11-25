#include "render_queue_opengles.h"

#include <iostream>

#include "render_batch_group_opengles.h"

namespace snowpulse {

std::shared_ptr<RenderQueueOpenGLES> RenderQueueOpenGLES::Create() {
    auto renderQueue = new RenderQueueOpenGLES();
    return std::shared_ptr<RenderQueueOpenGLES>(renderQueue);
}

RenderQueueOpenGLES::RenderQueueOpenGLES() {
}

RenderQueueOpenGLES::~RenderQueueOpenGLES() {
}

std::vector<std::shared_ptr<RenderBatchDataOpenGLES>> RenderQueueOpenGLES::PopAllData() {
    SortBatches();
    GroupBatches(batches_);

    std::vector<std::shared_ptr<RenderBatchDataOpenGLES>> batches;
    for (const auto& b : batches_) {
        auto type = b->GetType();
        if (type == RenderBatchType::kData) {
            auto data = std::dynamic_pointer_cast<RenderBatchDataOpenGLES>(b);
            if (data) {
                batches.push_back(data);
            }
        }
        else {
#ifdef SPDEBUG
            std::cerr << "Error: There shouldn't be other type of batches in this list." << std::endl;
#endif
        }
    }
    batches_.clear();
    return batches;
}

void RenderQueueOpenGLES::Push(std::shared_ptr<RenderBatch> batch) {
    batches_.push_back(batch);
}

void RenderQueueOpenGLES::SortBatches() {
    std::sort(batches_.begin(), batches_.end(), [](const auto& a, const auto& b) {
        return a->sortOrder < b->sortOrder;
    });
}

void RenderQueueOpenGLES::GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches) {
    auto duplicate = std::move(batches);
    batches.clear();
    bool addNewBatch = true;
    for (const auto& b : duplicate) {
        if (b->GetType() == RenderBatchType::kGroup) {
            auto group = std::dynamic_pointer_cast<RenderBatchGroupOpenGLES>(b);
            auto groupBatches = group->GetBatches();
            GroupBatches(groupBatches);
            for (const auto& b2 : groupBatches) {
                batches.push_back(b2);
            }
            addNewBatch = true;
        }
        else if (addNewBatch) {
            batches.push_back(b);
            addNewBatch = false;
        }
        else if (batches[batches.size() - 1]->GetType() == RenderBatchType::kData) {
            auto batch = (RenderBatchDataOpenGLES*)b.get();
            auto prevBatch = (RenderBatchDataOpenGLES*)batches[batches.size() - 1].get();
            if (prevBatch->drawMode == batch->drawMode &&
                prevBatch->blendMode == batch->blendMode &&
                prevBatch->isPremultiplied == batch->isPremultiplied &&
                prevBatch->viewMatrix == batch->viewMatrix &&
                prevBatch->projectionMatrix == batch->projectionMatrix &&
                prevBatch->shaderProgram == batch->shaderProgram &&
                prevBatch->texture == batch->texture) {

                for (const auto& v : batch->vertices) {
                    prevBatch->vertices.push_back(v);
                }
                for (const auto& i : batch->indices) {
                    prevBatch->indices.push_back(prevBatch->vertexCount + i);
                }
                for (const auto& m : batch->matrices) {
                    prevBatch->matrices.push_back(m);
                }
                prevBatch->vertexCount += batch->vertexCount;
                prevBatch->indexCount += batch->indexCount;
            }
            else {
                batches.push_back(b);
            }
        }
    }
}
}   // namespace snowpulse
