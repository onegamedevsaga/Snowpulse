#ifndef SNOWPULSE_APPLICATION_OPENGL_RENDERQUEUEOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_RENDERQUEUEOPENGL_H_

#include "../../defines.h"
#include "../../game/drawable.h"

#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render_batch_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI RenderQueueOpenGL {
    public:
        static std::shared_ptr<RenderQueueOpenGL> Create();

        virtual ~RenderQueueOpenGL();

        void Push(RenderBatchOpenGL batch);
        std::vector<RenderBatchOpenGL> PopAll();

    private:
        RenderQueueOpenGL();
        void SortBatches();
        void GroupBatches();

        std::vector<RenderBatchOpenGL> batches_;

};
}   // namespace snowpulse

#endif
