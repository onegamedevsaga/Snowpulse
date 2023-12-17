#include "render_queue_opengl.h"

#include <iostream>
#include <algorithm>

#include "render_batch_group_opengl.h"

namespace snowpulse {

std::shared_ptr<RenderQueueOpenGL> RenderQueueOpenGL::Create() {
    auto renderQueue = new RenderQueueOpenGL();
    return std::shared_ptr<RenderQueueOpenGL>(renderQueue);
}

RenderQueueOpenGL::RenderQueueOpenGL() {
}

RenderQueueOpenGL::~RenderQueueOpenGL() {
}

std::vector<std::shared_ptr<RenderBatchDataOpenGL>> RenderQueueOpenGL::PopAllData() {
    SortBatches();
    GroupBatches(batches_);

    std::vector<std::shared_ptr<RenderBatchDataOpenGL>> batches;
    for (const auto& b : batches_) {
        auto type = b->GetType();
        if (type == RenderBatchType::kData) {
            auto data = std::dynamic_pointer_cast<RenderBatchDataOpenGL>(b);
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

void RenderQueueOpenGL::Push(std::shared_ptr<RenderBatch> batch) {
    batches_.push_back(batch);
}

void RenderQueueOpenGL::SortBatches() {
    std::sort(batches_.begin(), batches_.end(), [](const auto& a, const auto& b) {
        return a->sortOrder < b->sortOrder;
    });
}

void RenderQueueOpenGL::GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches) {
    auto duplicate = std::move(batches);
    batches.clear();
    bool addNewBatch = true;
    for (const auto& b : duplicate) {
        if (b->GetType() == RenderBatchType::kGroup) {
            auto group = std::dynamic_pointer_cast<RenderBatchGroupOpenGL>(b);
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
            auto batch = (RenderBatchDataOpenGL*)b.get();
            auto prevBatch = (RenderBatchDataOpenGL*)batches[batches.size() - 1].get();
            if (prevBatch->drawMode == batch->drawMode &&
                prevBatch->blendMode == batch->blendMode &&
                prevBatch->textureFiltering == batch->textureFiltering &&
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
