#include "view_delegate_metal.h"

namespace snowpulse {
ViewDelegateMetal::ViewDelegateMetal(ApplicationMetal* app) : app_(app) {
    
}

ViewDelegateMetal::~ViewDelegateMetal() {
    
}

void ViewDelegateMetal::drawInMTKView(MTK::View* view) {
    app_->RunFrame();
}
}   // namespace snowpulse
