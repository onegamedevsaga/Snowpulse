#ifndef SNOWPULSE_COMMON_SINGLETON_H_
#define SNOWPULSE_COMMON_SINGLETON_H_

#include "../defines.h"

#include <iostream>

namespace snowpulse {
template <class T>
class Singleton {
    public:
        static T* GetInstance2() {
            static T instance;
            return &instance;
        }

        virtual ~Singleton() { }
        Singleton(const Singleton&) = delete;           // Disable copy constructor
        Singleton& operator=(const Singleton&) = delete; // Disable assignment operator
        Singleton(Singleton&&) = delete;                // Disable move constructor
        Singleton& operator=(Singleton&&) = delete;     // Disable move assignment operator

    protected:
        Singleton() { }
};

}   // namespace snowpulse
#endif
