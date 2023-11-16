#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include "defines.h"

#include <snowpulse.h>

#include <memory>

namespace game {
class GAMEAPI Game : public snowpulse::Game {
    public:
        static std::shared_ptr<Game> Create();

        virtual ~Game();
        void Initialize(snowpulse::Application* application);
        void Shutdown();

    private:
        Game();
};
}

#endif
