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

        int drawMode;
        std::string shaderProgram;
        BlendMode blendMode;
        TextureFiltering textureFiltering;
        unsigned int vertexCount;
        unsigned int indexCount;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        simd::float4x4 transformMatrix;

        unsigned int positionBufferIndex;
        unsigned int uVBufferIndex;
        unsigned int colorBufferIndex;
        unsigned int transformBufferIndex;
        unsigned int uniformsBufferIndex;
        unsigned int projectionMatrixShaderLocation;

        simd::float4x4 viewMatrix;
        simd::float4x4 projectionMatrix;

        bool isPremultiplied;
        MTL::Texture* texture;
};
}   // namespace snowpulse
#endif
