//
//  MJHelper-Win32.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJHelper_Win32_hpp
#define MJHelper_Win32_hpp

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait);
        void shareStory();
		void bringWindowsToFront();
    }
}

#endif
#endif /* MJHelper_Win32_hpp */
