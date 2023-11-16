#include "application_macos.h"

namespace snowpulse {

std::shared_ptr<ApplicationMacOS> ApplicationMacOS::Create() {
    auto app = std::shared_ptr<ApplicationMacOS>(new ApplicationMacOS());
    if (!app->Initialize()) {
        return SPNULL;
    }
    app->instance_ = app.get();
    return app;
}

ApplicationMacOS::ApplicationMacOS() {
    platform_ = Platform::kMacOS;
    platformString_ = "macOS";
}
}   // namespace snowpulse
