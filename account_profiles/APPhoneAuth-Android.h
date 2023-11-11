//
//  APPhoneAuth-Android.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APPhoneAuthAndroid_hpp
#define APPhoneAuthAndroid_hpp

#include "APPhoneAuth.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace ap {
    class PhoneAuthAndroid : public PhoneAuth {
    public:
        void signIn(const std::string & phone_number = "") override;
        using PhoneAuth::PhoneAuth;
        
    private:
    };
}
#endif
#endif /* APPhoneAuth-Android_hpp */
