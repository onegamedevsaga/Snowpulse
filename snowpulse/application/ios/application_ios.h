#ifndef SNOWPULSE_APPLICATION_IOS_APPLICATIONIOS_H_
#define SNOWPULSE_APPLICATION_IOS_APPLICATIONIOS_H_

#include "../../common/singleton.h"
#include "../opengles/application_opengles.h"

#include "directory_ios.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationIOS : public ApplicationOpenGLES, public Singleton<ApplicationIOS> {
    public:
        virtual bool Initialize() override;

    protected:
        friend class Singleton<ApplicationIOS>;
        ApplicationIOS();

        DirectoryIOS* directory_;
};
}   // namespace snowpulse

#endif
