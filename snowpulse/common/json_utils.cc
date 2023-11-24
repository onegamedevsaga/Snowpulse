#include "json_utils.h"

#include <iostream>
#include <fstream>

#include "../application/application.h"

namespace snowpulse {

std::shared_ptr<Json> JsonUtils::Create() {
    return std::make_shared<Json>();
}

std::shared_ptr<Json> JsonUtils::LoadFile(std::string filename) {
    filename = snowpulse::Application::GetInstance()->GetPlatformPath("assets/" + filename);
    std::ifstream f(filename.c_str());
    if (!f.fail()) {
        auto j = std::make_shared<Json>();
        f >> *j;
        f.close();
        f.clear();
        if (j.get() && !j->is_null()) {
            return j;
        }
    }
    return SPNULL;
}

bool JsonUtils::SaveFile(Json* data, std::string filename) {
    filename = snowpulse::Application::GetInstance()->GetPlatformPath("assets/" + filename);
    std::ofstream f(filename.c_str());
    if (!f.is_open()) {
#ifdef SPDEBUG
        std::cerr << "Unable to open file (" << filename.c_str() << ") for writing." << std::endl;
#endif
        return false;
    }
    if (!data) {
#ifdef SPDEBUG
        std::cerr << "No Json data to save." << std::endl;
#endif
        return false;
    }
    
    f << data->dump(4);
    f.close();
    f.clear();
    return true;
}
} // namespace snowpulse
