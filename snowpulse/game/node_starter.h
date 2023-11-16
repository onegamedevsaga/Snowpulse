#ifndef SNOWPULSE_GAME_NODESTARTER_H_
#define SNOWPULSE_GAME_NODESTARTER_H_

#include "../defines.h"

#include <memory>
#include <queue>

#include "node.h"

namespace snowpulse {

class SNOWPULSEAPI NodeStarter {
    public:
        NodeStarter();
        virtual ~NodeStarter();

        void AddNode(std::shared_ptr<Node> node);
        void StartNodes();

    private:
        std::queue<std::shared_ptr<Node>> nodes_;
};
}   // namespace snowpulse

#endif
