#ifndef SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_

#include "../../common/singleton.h"
#include "../metal/application_metal.h"

#include "directory_macos.h"
#include "../macos/input_macos.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMacOS : public ApplicationMetal, public Singleton<ApplicationMacOS> {
    public:
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* view) override;
        void Close() override;
        void Shutdown() override;

    protected:
        friend class Singleton<ApplicationMacOS>;
        ApplicationMacOS();

        DirectoryMacOS* directory_;
        InputMacOS* input_;
};
}   // namespace snowpulse

#endif
