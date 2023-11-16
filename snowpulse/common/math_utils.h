#ifndef SNOWPULSE_COMMON_MATHUTILS_H_
#define SNOWPULSE_COMMON_MATHUTILS_H_

#include "../defines.h"

#include <cmath>

#include "easing.h"
#include "vector2.h"

namespace snowpulse {
class SNOWPULSEAPI MathUtils {
    public:
        static float Min(float a, float b) { return a < b ? a : b; }
        static float Max(float a, float b) { return a > b ? a : b; }
        static float Clamp(float min, float max, float val) { return val > max ? max : (val < min ? min : val); }

        static Vector2 Ease(Vector2 start, Vector2 end, float percentage, Easing easing = Easing::kLinear) {
            return Vector2(Ease(start.x, end.x, percentage, easing), Ease(start.y, end.y, percentage, easing));
        }

        static float Ease(float start, float end, float percentage, Easing easing = Easing::kLinear ) {
            switch (easing) {
                case Easing::kSineIn:
                    percentage = SineInEasing(percentage);
                    break;
                case Easing::kSineOut:
                    percentage = SineOutEasing(percentage);
                    break;
                case Easing::kSineInOut:
                    percentage = SineInOutEasing(percentage);
                    break;
                case Easing::kQuarticIn:
                    percentage = QuarticInEasing(percentage);
                    break;
                case Easing::kQuarticOut:
                    percentage = QuarticOutEasing(percentage);
                    break;
                case Easing::kQuarticInOut:
                    percentage = QuarticInOutEasing(percentage);
                    break;
                case Easing::kElasticIn:
                    percentage = ElasticInEasing(percentage);
                    break;
                case Easing::kElasticOut:
                    percentage = ElasticOutEasing(percentage);
                    break;
                case Easing::kElasticInOut:
                    percentage = ElasticInOutEasing(percentage);
                    break;
                case Easing::kBounceIn:
                    percentage = BounceInEasing(percentage);
                    break;
                case Easing::kBounceOut:
                    percentage = BounceOutEasing(percentage);
                    break;
                case Easing::kBounceInOut:
                    percentage = BounceInOutEasing(percentage);
                    break;
                case Easing::kLinear:
                default:
                    break;
            }
            return start + (end - start) * percentage;
        }

    private:
        static float SineInEasing(float percentage) {
            return 1 - cos(percentage * 1.57079632679489661923f);
        }

        static float SineOutEasing(float percentage) {
            return sin(percentage * 1.57079632679489661923f);
        }

        static float SineInOutEasing(float percentage) {
            return 0.5 * (1 - cos(3.14159265358979323846f * percentage));
        }

        static float QuarticInEasing(float percentage) {
            return percentage * percentage * percentage * percentage;
        }

        static float QuarticOutEasing(float percentage) {
            float p = 1.0f - percentage;
            return 1.0f - p * p * p * p;
        }

        static float QuarticInOutEasing(float percentage) {
            if (percentage < 0.5) {
                return 8 * percentage * percentage * percentage * percentage;
            } else {
                float f = (percentage - 1);
                return 1 - 8 * f * f * f * f;
            }
        }

        static float ElasticInEasing(float percentage) {
            if (percentage == 0) return 0;
            if (percentage == 1) return 1;
            float p = 0.3;
            float s = p / 4;
            float postFix = pow(2, 10 * (percentage -= 1));
            return -(postFix * sin((percentage - s) * (2 * 3.14159265358979323846f) / p));
        }

        static float ElasticOutEasing(float percentage) {
            if (percentage == 0) return 0;
            if (percentage == 1) return 1;
            float p = 0.3;
            float s = p / 4;
            return pow(2, -10 * percentage) * sin((percentage - s) * (2 * 3.14159265358979323846f) / p) + 1;
        }

        static float ElasticInOutEasing(float percentage) {
            if (percentage == 0) return 0;
            if ((percentage /= 0.5) == 2) return 1;
            float p = (0.3 * 1.5);
            float s = p / 4;
            if (percentage < 1) {
                float postFix = pow(2, 10 * (percentage -= 1));
                return -0.5 * (postFix * sin((percentage - s) * (2 * 3.14159265358979323846f) / p));
            }
            float postFix = pow(2, -10 * (percentage -= 1));
            return postFix * sin((percentage - s) * (2 * 3.14159265358979323846f) / p) * 0.5 + 1;
        }

        static float BounceInEasing(float percentage) {
            return 1 - BounceOutEasing(1 - percentage);
        }

        static float BounceOutEasing(float percentage) {
            if (percentage < 1 / 2.75) {
                return 7.5625 * percentage * percentage;
            } else if (percentage < 2 / 2.75) {
                percentage -= 1.5 / 2.75;
                return 7.5625 * percentage * percentage + 0.75;
            } else if (percentage < 2.5 / 2.75) {
                percentage -= 2.25 / 2.75;
                return 7.5625 * percentage * percentage + 0.9375;
            } else {
                percentage -= 2.625 / 2.75;
                return 7.5625 * percentage * percentage + 0.984375;
            }
        }

        static float BounceInOutEasing(float percentage) {
            if (percentage < 0.5) {
                return BounceInEasing(percentage * 2) * 0.5;
            }
            return BounceOutEasing(percentage * 2 - 1) * 0.5 + 0.5;
        }
};
}   // namespace snowpulse
#endif
