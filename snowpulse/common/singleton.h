#ifndef SNOWPULSE_COMMON_SINGLETON_H_
#define SNOWPULSE_COMMON_SINGLETON_H_

#include "../defines.h"

#include <iostream>

namespace snowpulse {
template <class T>
class Singleton {
    public:
        static T* GetInstance() {
            if (!instance_) {
#ifdef SPDEBUG
                std::cerr << "Singleton's instance_ isn't set.";
#endif
            }
            return instance_;
        }

        virtual ~Singleton() { }
        Singleton(const Singleton&) = delete;           // Disable copy constructor
        Singleton& operator=(const Singleton&) = delete; // Disable assignment operator
        Singleton(Singleton&&) = delete;                // Disable move constructor
        Singleton& operator=(Singleton&&) = delete;     // Disable move assignment operator

    protected:
        static T* instance_;

        Singleton() { }
};

template <class T>
T* Singleton<T>::instance_ = SPNULL;

}   // namespace snowpulse
#endif
