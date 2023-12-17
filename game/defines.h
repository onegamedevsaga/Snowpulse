#ifndef GAME_DEFINES_H_
#define GAME_DEFINES_H_

#ifdef GAMEAPI_EXPORT
#ifdef SNOWPULSE_PLATFORM_WINDOWS
#define GAMEAPI/* __declspec(dllexport)*/
#define GAMEAPI_HIDDEN
#else
#define GAMEAPI __attribute__((visibility("default")))
#define GAMEAPI_HIDDEN __attribute__((visibility("hidden")))
#endif
#else
#ifdef SNOWPULSE_PLATFORM_WINDOWS
#define GAMEAPI/* __declspec(dllimport)*/
#define GAMEAPI_HIDDEN/* __declspec(dllimport)*/
#else
#define GAMEAPI
#define GAMEAPI_HIDDEN
#endif
#endif

#endif
