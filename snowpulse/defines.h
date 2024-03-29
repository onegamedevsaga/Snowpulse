#ifndef SNOWPULSE_DEFINES_H_
#define SNOWPULSE_DEFINES_H_

#ifdef SNOWPULSEAPI_EXPORT
	#ifdef SNOWPULSE_PLATFORM_WINDOWS
		#define SNOWPULSEAPI/* __declspec(dllexport)*/
		#define SNOWPULSEAPI_HIDDEN
	#else
		#define SNOWPULSEAPI __attribute__((visibility("default")))
		#define SNOWPULSEAPI_HIDDEN __attribute__((visibility("hidden")))
	#endif
#else
	#ifdef SNOWPULSE_PLATFORM_WINDOWS
		#define SNOWPULSEAPI/* __declspec(dllimport)*/
		#define SNOWPULSEAPI_HIDDEN/* __declspec(dllimport)*/
	#else
		#define SNOWPULSEAPI
		#define SNOWPULSEAPI_HIDDEN
	#endif
#endif


#if defined(DEBUG) || defined(_DEBUG)
	#define SPDEBUG
#endif

#define SPNULL nullptr

#define SPSAFE_DELETE(ptr) if(ptr != 0) { delete ptr; ptr=0; }

// Use this in public access modifier
#define SPSINGLETON_FRIEND(cls) \
static cls* GetInstance() { return cls::GetSingletonInstance(); } \
private: \
friend class snowpulse::Singleton<cls>; \
public:

#define SPPI (3.14159265358979323846f)
#define SPDEGTORAD(degrees) (degrees * 0.017453292519943f)

#endif
