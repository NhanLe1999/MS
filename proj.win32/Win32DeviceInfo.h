#ifndef __Win32DeviceInfo__
#define __Win32DeviceInfo__
#define XWin32DeviceInfo Win32DeviceInfo::getInstance()

#include <stdio.h>
#include "cocos2d.h"
#include <rpc.h>
#include <rpcdce.h>
#include <iostream>
#include "stdio.h"
#include <vector>
#pragma comment(lib, "rpcrt4.lib")
#include <string>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
class Win32DeviceInfo {
public:
    static Win32DeviceInfo * getInstance();
    static Win32DeviceInfo * __instance;
	std::string GetMACaddress();
	static std::string getMemorySize();//ram
	static bool GetWinMajorMinorVersion(DWORD& major, DWORD& minor);
	static std::string GetWindowsOSInfo();//os
	static std::string cpuInfo();//chip
	static bool isInternetConection();

	void enableKeyboard(bool isEnable);
	
	void setKeyboardIsEnable();
	bool isEnableKeyBoard = true;
	bool isCallDelay = false;
};

#endif
