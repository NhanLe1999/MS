//
//  MJHelper-Apple.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJHelper_Mac_hpp
#define MJHelper_Mac_hpp

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait);
        void shareStory();
        void sharing(std::string msg,std::string imagePath); // ham nay viet ra danh cho ipad
        void logLanguages();
    }
}

#endif

#endif /* MJHelper_Mac_hpp */
