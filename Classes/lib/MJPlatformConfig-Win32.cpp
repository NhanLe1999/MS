//
//  MJPlatformConfig-Win32.cpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#include "MJPlatformConfig-Win32.h"
#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define GROUD_IDENTIFIER @"com.earlystart"
#define UUID_KEYCHAIN @"monkey_junior_uuid"
#include "../proj.win32/Win32DeviceInfo.h"
#include <time.h>
#include <string>
#include "WebService.h"
#include "StoryConfigManager.h"
#include "windows.h"

#include "MJDefault.h"

#include <fstream>


#define XWin32DeviceInfo Win32DeviceInfo::getInstance()
namespace mj {

    /** get unique device id
     *  @return a std::string contains MAC address, identifier for vendor and uuid saved in keychain with delimiter "|"
     */
    std::string PlatformConfigWin32::getPlatformDeviceID() {

        std::string macaddress = XWin32DeviceInfo->GetMACaddress(); //SystemInfo::SystemInfo::GetMACaddress();
        std::string uuid = "null";
        std::string device_id = "null" ;

        return macaddress + "|" + device_id + "|" + uuid;
    }

    std::string PlatformConfigWin32::getOS() {
        return "win32";
    }

    std::string PlatformConfigWin32::getAppVersion() {
        auto v = Application::getInstance()->getVersion();
		v.resize(v.length() - 2);
		return v;
    }

    std::string PlatformConfigWin32::getReceipt() {
        return "";
    }

	std::string PlatformConfigWin32::getDeviceInfo() {
		return XWin32DeviceInfo->GetWindowsOSInfo();
	}

	std::string PlatformConfigWin32::getDeviceInfoShort() {
		return XWin32DeviceInfo->GetWindowsOSInfo();
	}

    bool PlatformConfigWin32::phoneSupport(std::string) {
        return false;
    }

    bool PlatformConfigWin32::mailSupport(std::string) {
        return false;
    }

	std::string PlatformConfigWin32::getUserDefaults() {
		std::string user_default = MJDEFAULT->getAllDatabase();;
		return user_default;
	}

	void PlatformConfigWin32::setUserDefaults(std::string info) {
		MJDEFAULT->setAllDatabase(info);
	}

    void PlatformConfigWin32::showVMonkeyPage() {
        Application::getInstance()->openURL("https://vmonkey.vn/");
    }
	
    void PlatformConfigWin32::showLearnToReadStorePage() {
        Application::getInstance()->openURL("https://monkeyjunior.vn");
    }

    void PlatformConfigWin32::showStoriesStorePage() {
        Application::getInstance()->openURL("https://monkeystories.vn");
    }

    void PlatformConfigWin32::showMathStorePage() {
        Application::getInstance()->openURL("https://monkeymath.vn");
    }

    std::string PlatformConfigWin32::getAppName() {
        if (CONFIG_MANAGER->isVietnameseStories()) {
            return "VMonkey";
        } else {
            return "Monkey Stories";
        }
    }

    bool PlatformConfigWin32::canOpenLearnToRead() {
        return false;
    }

    void PlatformConfigWin32::openLearnToRead() {

    }

    void PlatformConfigWin32::shareViral(std::string url){
        HWND hwnd = GetDesktopWindow();
		OpenClipboard(hwnd);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, url.size() + 1);
		if (!hg){
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), url.c_str(), url.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
    }

    void PlatformConfigWin32::saveScreenShot(std::string url, std::string file_name){
//        int hdcSrc = User32.GetWindowDC(User32.GetDesktopWindow()),
//        hdcDest = GDI32.CreateCompatibleDC(hdcSrc),
//        hBitmap = GDI32.CreateCompatibleBitmap(hdcSrc,
//                                               GDI32.GetDeviceCaps(hdcSrc,8),GDI32.GetDeviceCaps(hdcSrc,10));                 GDI32.SelectObject(hdcDest,hBitmap);
//        GDI32.BitBlt(hdcDest,0,0,GDI32.GetDeviceCaps(hdcSrc,8),
//                     GDI32.GetDeviceCaps(hdcSrc,10),hdcSrc,0,0,0x00CC0020);
//        SaveImageAs(hBitmap,url,imageFormat);
//        Cleanup(hBitmap,hdcSrc,hdcDest);

        //WCHAR *buffer = new WCHAR[260];
        //const WCHAR name[12] = L"USERPROFILE";
        //DWORD result = GetEnvironmentVariable(name, buffer, 260);
        //if (result > 260) {
        //    delete[] buffer; buffer = new WCHAR[result];
        //    GetEnvironmentVariable(name, buffer, result);
        //}
        //std::wstring s(L"C:/Users/");
        //s += buffer;
        //s += L"/Desktop/";
        //s += std::wstring(file_name.begin(),file_name.end());
        //std::ofstream myfile(s.c_str());
        //// do things here
        //delete[] buffer;
        //return 0;
		//dispatch_async(dispatch_get_main_queue(), ^{
			cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("save picture");
		//	});

		WCHAR *buffer = new WCHAR[260];
		const WCHAR name[12] = L"USERPROFILE";
		DWORD result = GetEnvironmentVariable(name, buffer, 260);
		if (result > 260) {
		    delete[] buffer; buffer = new WCHAR[result];
		    GetEnvironmentVariable(name, buffer, result);
		}
		std::wstring s(buffer);
		s += L"\\Desktop\\";
		s += std::wstring(file_name.begin(), file_name.end());
		CCLOG(string(s.begin(),s.end()).c_str());

		std::string path = string(s.begin(), s.end());
		if (!std::rename(url.c_str(), path.c_str())) {
			CCLOG("Moved Screenshoot to Desktop");
		}

    }
}

#endif //CC_TARGET_PLATFORM
