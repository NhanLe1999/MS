//
//  MJInApp-Apple.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#ifndef MJInApp_Apple_hpp
#define MJInApp_Apple_hpp

#include "MJInApp.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
namespace mj {
    class InAppApple : public InApp {
    public:
        void init(const std::vector<std::string> &ids) override;
        void purchase(const std::string product_id) override;
        void refresh(const std::vector<std::string> &ids) override;
        void restore() override;
        std::string getReceipt(const std::string & product_id) override;

//        void payWithOnePay(std::string product_id, std::string coupon) override;
    };
}
#endif

#endif /* MJInApp_Apple_hpp */
