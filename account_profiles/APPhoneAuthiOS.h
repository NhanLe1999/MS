//
//  APPhoneAuthiOS.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APPhoneAuthiOS_hpp
#define APPhoneAuthiOS_hpp

#include "APPhoneAuth.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import <AccountKit/AKFAccountKit.h>
namespace ap {
    class PhoneAuthiOS : public PhoneAuth {
    public:
        void signIn(const std::string & phone_number = "") override;
        using PhoneAuth::PhoneAuth;
        
    private:
    };
}
#endif
#endif /* APPhoneAuthiOS_hpp */
