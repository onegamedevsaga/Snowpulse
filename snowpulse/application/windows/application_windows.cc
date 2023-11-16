#include "application_windows.h"

namespace snowpulse {

std::shared_ptr<ApplicationWindows> ApplicationWindows::Create() {
    auto app = std::shared_ptr<ApplicationWindows>(new ApplicationWindows());
    if (!app->Initialize()) {
        return SPNULL;
    }
    app->instance_ = app.get();
    return app;
}

ApplicationWindows::ApplicationWindows() {
    platform_ = Platform::kWindows;
    platformString_ = "Windows";
}
}   // namespace snowpulse
