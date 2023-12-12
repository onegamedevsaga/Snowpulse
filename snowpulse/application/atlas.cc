#include "atlas.h"

#include <iostream>

namespace snowpulse {

std::shared_ptr<Atlas> Atlas::Create() {
    auto atlas = new Atlas();
    return std::shared_ptr<Atlas>(atlas);
}

Atlas::Atlas() {
}

Atlas::~Atlas() {
}
}   // namespace snowpulse
