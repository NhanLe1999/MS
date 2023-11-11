//
//  MJInApp-Android.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/27/17.
//

#ifndef MJInApp_Android_hpp
#define MJInApp_Android_hpp

#include "platform/CCPlatformConfig.h"
#include "MJInApp.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

namespace mj {
    class InAppAndroid : public InApp {
    public:
        void init(const std::vector<std::string> & ids) override;
        void purchase(const std::string product_id) override;
        void refresh(const std::vector<std::string> & ids) override;
        void restore() override;
        std::string getReceipt(const std::string & product_id) override;
    };
}

#endif /*CC_TARGET_PLATFORM */

#endif /* MJInApp_Android_hpp */
