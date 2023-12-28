#include "string_utils.h"

#include <cctype>

namespace snowpulse {
std::string StringUtils::ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                            return std::tolower(c);
                        }
                   );
    return str;
}
std::string StringUtils::ToUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                            return std::toupper(c);
                        }
                   );
    return str;
}
} // namespace snowpulse
