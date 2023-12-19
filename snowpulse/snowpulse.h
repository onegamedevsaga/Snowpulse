#ifndef SNOWPULSE_SNOWPULSE_H_
#define SNOWPULSE_SNOWPULSE_H_

#define GLM_FORCE_PURE

#include "defines.h"

#include "../thirdparty/nlohmann-json/json.hpp"

#include "common/platform.h"
#include "common/space.h"
#include "common/blend_mode.h"
#include "common/path_type.h"
#include "common/easing.h"
#include "common/json_utils.h"
#include "common/math_utils.h"
#include "common/atlas_sprite.h"
#include "common/singleton.h"
#include "common/vector2.h"
#include "common/vector2int.h"
#include "common/vector3.h"
#include "common/matrix4x4.h"
#include "common/color.h"
#include "common/rect.h"
#include "common/rect_stb.h"
#include "common/texture_filtering.h"
#include "common/vertex.h"
#include "common/touch.h"
#include "common/touch_phase.h"

#include "game/updatable.h"
#include "game/drawable.h"
#include "game/node.h"
#include "game/game_object.h"
#include "game/scene.h"
#include "game/game.h"
#include "game/component.h"
#include "game/action_manager.h"

#include "game/components/transform.h"
#include "game/components/sprite_renderer.h"
#include "game/components/spine_renderer.h"
#include "game/components/font_renderer.h"
#include "game/components/tiled_renderer.h"

#include "game/actions/action_interpolate.h"
#include "game/actions/action_wait.h"
#include "game/actions/action_move.h"
#include "game/actions/action_rotate.h"

#include "application/graphics.h"
#include "application/atlas_manager.h"
#include "application/font_manager.h"
#include "application/timer.h"
#include "application/camera.h"
#include "application/input.h"
#include "application/directory.h"

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "application/windows/application_windows.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include <imgui_impl_metal.h>
#include "application/macos/application_macos.h"
#elif SNOWPULSE_PLATFORM_ANDROID
#include "application/android/application_android.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "application/ios/application_ios.h"
#elif SNOWPULSE_PLATFORM_WEBGL
#include "application/webgl/application_webgl.h"
#endif

#endif
