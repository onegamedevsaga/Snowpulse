#ifndef SNOWPULSE_APPLICATION_METAL_RENDERBATCHDATAMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_RENDERBATCHDATAMETAL_H_

#include "../../defines.h"
#include "../render_batch.h"

#include <memory>
#include <vector>
#include <simd/simd.h>

#include "../../common/blend_mode.h"
#include "../../common/texture_filtering.h"


namespace MTL {
class Texture;
}

namespace snowpulse {
class SNOWPULSEAPI RenderBatchDataMetal : public RenderBatch {
    public:
        RenderBatchDataMetal() : RenderBatch(RenderBatchType::kData) { }
        ~RenderBatchDataMetal() { }

        bool CanMergeWith(const RenderBatchDataMetal* other) const {
            return this->drawMode == other->drawMode &&
                   this->blendMode == other->blendMode &&
                   this->textureFiltering == other->textureFiltering &&
                   this->isPremultiplied == other->isPremultiplied &&
                   this->viewMatrix == other->viewMatrix &&
                   this->projectionMatrix == other->projectionMatrix &&
                   this->shaderProgram == other->shaderProgram &&
                   this->texture == other->texture;
        }

        void MergeWith(const RenderBatchDataMetal& other) {
            // Append vertices, UVs, colors, and transform matrices
            this->vertices.insert(this->vertices.end(), other.vertices.begin(), other.vertices.end());
            this->uvs.insert(this->uvs.end(), other.uvs.begin(), other.uvs.end());
            this->colors.insert(this->colors.end(), other.colors.begin(), other.colors.end());
            this->transformMatrices.insert(this->transformMatrices.end(), other.transformMatrices.begin(), other.transformMatrices.end());

            // Adjust and append indices
            for (const auto& index : other.indices) {
                this->indices.push_back(this->vertexCount + index);
            }

            // Update counts
            this->vertexCount += other.vertexCount;
            this->indexCount += other.indexCount;
        }

        int drawMode;
        std::string shaderProgram;
        BlendMode blendMode;
        TextureFiltering textureFiltering;
        unsigned int vertexCount;
        unsigned int indexCount;

        std::vector<simd::float3> vertices;
        std::vector<simd::float2> uvs;
        std::vector<simd::float4> colors;
        std::vector<uint16_t> indices;
        std::vector<simd::float4x4> transformMatrices;

        unsigned int positionBufferIndex;
        unsigned int uVBufferIndex;
        unsigned int colorBufferIndex;
        unsigned int transformBufferIndex;
        unsigned int uniformsBufferIndex;

        simd::float4x4 viewMatrix;
        simd::float4x4 projectionMatrix;

        bool isPremultiplied;
        MTL::Texture* texture;
};
}   // namespace snowpulse
#endif
