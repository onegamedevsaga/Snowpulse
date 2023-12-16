#ifndef SNOWPULSE_APPLICATION_RENDERBATCH_H_
#define SNOWPULSE_APPLICATION_RENDERBATCH_H_

#include "../defines.h"

#include "render_batch_type.h"

namespace snowpulse {
class SNOWPULSEAPI RenderBatch {
    public:
        virtual ~RenderBatch() { }

        RenderBatchType GetType() const { return type_; }

        int sortOrder;

    protected:
        RenderBatch(RenderBatchType type) : sortOrder(0), type_(type) { }

    private:
        RenderBatchType type_;
};
}   // namespace snowpulse
#endif
