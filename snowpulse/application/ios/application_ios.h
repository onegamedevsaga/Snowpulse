#ifndef SNOWPULSE_APPLICATION_IOS_APPLICATIONIOS_H_
#define SNOWPULSE_APPLICATION_IOS_APPLICATIONIOS_H_

#include "../../common/singleton.h"
#include "../metal/application_metal.h"

#include "directory_ios.h"
#include "../ios/input_ios.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationIOS : public ApplicationMetal, public Singleton<ApplicationIOS> {
    public:
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        void Close() override;
        void Shutdown() override;

        void RunFrame();
    

    protected:
        friend class Singleton<ApplicationIOS>;
        ApplicationIOS();

        DirectoryIOS* directory_;
        InputIOS* input_;
};
}   // namespace snowpulse

#endif
