#include "render_queue_metal.h"

#include <iostream>

#include "render_batch_group_metal.h"

namespace snowpulse {

std::shared_ptr<RenderQueueMetal> RenderQueueMetal::Create() {
    auto renderQueue = new RenderQueueMetal();
    return std::shared_ptr<RenderQueueMetal>(renderQueue);
}

RenderQueueMetal::RenderQueueMetal() {
}

RenderQueueMetal::~RenderQueueMetal() {
}

std::vector<std::shared_ptr<RenderBatchDataMetal>> RenderQueueMetal::PopAllData() {
    SortBatches();
    GroupBatches(batches_);

    std::vector<std::shared_ptr<RenderBatchDataMetal>> batches;
    batches.reserve(batches_.size());
    for (const auto& b : batches_) {
        auto type = b->GetType();
        if (type == RenderBatchType::kData) {
            auto data = std::dynamic_pointer_cast<RenderBatchDataMetal>(b);
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

void RenderQueueMetal::Push(std::shared_ptr<RenderBatch> batch) {
    batches_.push_back(batch);
}

void RenderQueueMetal::SortBatches() {
    std::sort(batches_.begin(), batches_.end(), [](const auto& a, const auto& b) {
        return a->sortOrder < b->sortOrder;
    });
}

void RenderQueueMetal::GroupBatches(std::vector<std::shared_ptr<RenderBatch>>& batches) {
    auto duplicate = std::move(batches);
    batches.clear();
    bool addNewBatch = true;
    for (const auto& b : duplicate) {
        if (b->GetType() == RenderBatchType::kGroup) {
            auto group = std::dynamic_pointer_cast<RenderBatchGroupMetal>(b);
            auto groupBatches = group->GetBatches();
            GroupBatches(groupBatches);
            for (const auto& b2 : groupBatches) {
                batches.push_back(b2);
            }
            addNewBatch = true;
        }
        else if (addNewBatch) {
            /*auto batch = (RenderBatchDataMetal*)b.get();
            batch->vertices.reserve(2000);
            batch->uvs.reserve(2000);
            batch->colors.reserve(2000);
            batch->transformMatrices.reserve(2000);
            batch->indices.reserve(2000);*/
            batches.push_back(b);
            addNewBatch = false;
        }
        else if (batches[batches.size() - 1]->GetType() == RenderBatchType::kData) {
            auto batch = (RenderBatchDataMetal*)b.get();
            auto prevBatch = (RenderBatchDataMetal*)batches[batches.size() - 1].get();
            if (prevBatch->CanMergeWith(batch)) {

                /*prevBatch->vertices.insert(prevBatch->vertices.end(), batch->vertices.begin(), batch->vertices.end());
                prevBatch->uvs.insert(prevBatch->uvs.end(), batch->uvs.begin(), batch->uvs.end());
                prevBatch->colors.insert(prevBatch->colors.end(), batch->colors.begin(), batch->colors.end());
                prevBatch->transformMatrices.insert(prevBatch->transformMatrices.end(), batch->transformMatrices.begin(), batch->transformMatrices.end());*/

                for (const auto& v : batch->vertices) {
                    prevBatch->vertices.push_back(v);
                }

                for (const auto& u : batch->uvs) {
                    prevBatch->uvs.push_back(u);
                }

                for (const auto& c : batch->colors) {
                    prevBatch->colors.push_back(c);
                }
                
                for (const auto& t : batch->transformMatrices) {
                    prevBatch->transformMatrices.push_back(t);
                }

                for (const auto& i : batch->indices) {
                    prevBatch->indices.push_back(prevBatch->vertexCount + i);
                }

                prevBatch->vertexCount += batch->vertexCount;
                prevBatch->indexCount += batch->indexCount;
            }
            else {
                batches.push_back(b);
            }
        }
    }

    /*std::vector<std::shared_ptr<RenderBatch>> newBatches;
    newBatches.reserve(batches.size()); // Reserve capacity to avoid reallocations

    for (const auto& b : batches) {
        if (b->GetType() == RenderBatchType::kGroup) {
            auto group = std::dynamic_pointer_cast<RenderBatchGroupMetal>(b);
            auto groupBatches = group->GetBatches();
            GroupBatches(groupBatches);
            newBatches.insert(newBatches.end(), groupBatches.begin(), groupBatches.end());
        }
        else {
            if (newBatches.empty() || newBatches.back()->GetType() != RenderBatchType::kData) {
                newBatches.push_back(b);
            }
            else {
                auto batch = static_cast<RenderBatchDataMetal*>(b.get());
                auto prevBatch = static_cast<RenderBatchDataMetal*>(newBatches.back().get());

                if (prevBatch->CanMergeWith(*batch)) {
                    prevBatch->MergeWith(*batch);
                }
                else {
                    newBatches.push_back(b);
                }
            }
        }
    }

    batches.swap(newBatches);*/
}
}   // namespace snowpulse
