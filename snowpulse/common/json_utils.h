#ifndef SNOWPULSE_COMMON_JSONUTILS_H_
#define SNOWPULSE_COMMON_JSONUTILS_H_

#include "../defines.h"

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <json.hpp>

namespace snowpulse {

typedef nlohmann::json Json;

class SNOWPULSEAPI JsonUtils {
    public:
        static std::shared_ptr<Json> Create() {
            return std::make_shared<Json>();
        }

        static std::shared_ptr<Json> LoadFile(std::string filename) {
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

        static bool SaveFile(Json* data, std::string filename) {
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


};
}   // namespace snowpulse
#endif
