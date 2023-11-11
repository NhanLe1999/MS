//
//  MJHelper-Win32.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJHelper-Win32.h"

#include "platform/CCPlatformConfig.h"
#include "cocos2d.h"
#include "2d/CCLight.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <windows.h>
namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait) {

        }

        void shareStory() {
            //CCLOG("share story not available on win32");
        }

		void bringWindowsToFront() {
			auto hwnd = cocos2d::Director::getInstance()->getOpenGLView()->getWin32Window();
			SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);
			SetActiveWindow(hwnd);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			RedrawWindow(hwnd, nullptr, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
		}

    }
}

#endif
