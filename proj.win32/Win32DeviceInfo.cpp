#include "Win32DeviceInfo.h"
#include "Wininet.h"
#include "HSLib.h"
#include "MJDefinitions.h"
Win32DeviceInfo * Win32DeviceInfo::__instance = NULL;
HHOOK KeyboardHook = NULL;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
//KEY_BOARD
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam){
	CCLOG("Key WM_KEYDOWN");
	if (wParam == WM_KEYUP)
	{	
		//Win32DeviceInfo::getInstance()->isEnableKeyBoard = true;
		KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		
		if (kbdStruct.vkCode == VK_RIGHT)
		{	
			CCLOG("Key VK_RIGHT");
			
			//XHSLib->keyboard_index = 1;
			//cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");
			

		}
		if (kbdStruct.vkCode == VK_LEFT)
		{	
			CCLOG("Key VK_LEFT");
			//XHSLib->keyboard_index = 2;
			//cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");

		}

		if ((kbdStruct.vkCode == VK_DOWN || kbdStruct.vkCode == VK_UP))
		{	
			CCLOG("Key VK_DOWN");
			//XHSLib->keyboard_index = 3;
			//cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.keyboard_page");

		}
		
	}
	return CallNextHookEx(KeyboardHook, WH_KEYBOARD_LL, wParam, lParam);
}
Win32DeviceInfo * Win32DeviceInfo::getInstance() {
    if (!__instance) {
        __instance = new Win32DeviceInfo();
    }
    return __instance;
}

std::string Win32DeviceInfo::GetMACaddress(){
	unsigned char MACData[6];
	UUID uuid;
	UuidCreateSequential(&uuid);
	for (int i = 2; i<8; i++)
		MACData[i - 2] = uuid.Data4[i];

	char str2[80] = { 0 };
	sprintf(str2, "%02X:%02X:%02X:%02X:%02X:%02X", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
	return std::string(str2);
}

std::string Win32DeviceInfo::getMemorySize(){
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);// I misunderstand that
	GlobalMemoryStatusEx(&statex);
	auto ramSize = (float)statex.ullTotalPhys / (1024 * 1024 * 1024);
	return cocos2d::StringUtils::format("%f Gb", ramSize);
}
bool Win32DeviceInfo::GetWinMajorMinorVersion(DWORD& major, DWORD& minor){
	bool bRetCode = false;
	LPBYTE pinfoRawData = 0;
	if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pinfoRawData))
	{
		WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100*)pinfoRawData;
		major = pworkstationInfo->wki100_ver_major;
		minor = pworkstationInfo->wki100_ver_minor;
		::NetApiBufferFree(pinfoRawData);
		bRetCode = true;
	}
	return bRetCode;
}
std::string Win32DeviceInfo::GetWindowsOSInfo(){
	std::string     winver;
	OSVERSIONINFOEX osver;
	SYSTEM_INFO     sysInfo;
	typedef void(__stdcall *GETSYSTEMINFO) (LPSYSTEM_INFO);

	__pragma(warning(push))
		__pragma(warning(disable:4996))
		memset(&osver, 0, sizeof(osver));
	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx((LPOSVERSIONINFO)&osver);
	__pragma(warning(pop))
		DWORD major = 0;
	DWORD minor = 0;
	if (GetWinMajorMinorVersion(major, minor))
	{
		osver.dwMajorVersion = major;
		osver.dwMinorVersion = minor;
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
	{
		OSVERSIONINFOEXW osvi;
		ULONGLONG cm = 0;
		cm = VerSetConditionMask(cm, VER_MINORVERSION, VER_EQUAL);
		ZeroMemory(&osvi, sizeof(osvi));
		osvi.dwOSVersionInfoSize = sizeof(osvi);
		osvi.dwMinorVersion = 3;
		if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, cm))
		{
			osver.dwMinorVersion = 3;
		}
	}

	GETSYSTEMINFO getSysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "GetNativeSystemInfo");
	if (getSysInfo == NULL)  getSysInfo = ::GetSystemInfo;
	getSysInfo(&sysInfo);

	if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows 10 Server";
	if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 10";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012 R2";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8.1";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008 R2";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 7";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008";
	if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows Vista";
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION
		&&  sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)  winver = "Windows XP x64";
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)   winver = "Windows Server 2003";
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)   winver = "Windows XP";
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)   winver = "Windows 2000";
	if (osver.dwMajorVersion < 5)   winver = "unknown";

	if (osver.wServicePackMajor != 0)
	{
		std::string sp;
		char buf[128] = { 0 };
		sp = " Service Pack ";
		sprintf_s(buf, sizeof(buf), "%hd", osver.wServicePackMajor);
		sp.append(buf);
		winver += sp;
	}
	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		winver = winver + " - 64 bit";
	}
	else{
		winver = winver + " - 32 bit";
	}


	auto screenStr = cocos2d::StringUtils::format("%d x %d", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	auto osInfoStr = cocos2d::StringUtils::format("OS:%s,Ram:%s,Chip:%s,Screen:%s", winver.c_str(), getMemorySize().c_str(), cpuInfo().c_str(), screenStr.c_str());
	return osInfoStr;
}
std::string Win32DeviceInfo::cpuInfo(){
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	//string includes manufacturer, model and clockspeed
	auto stringCPU = CPUBrandString;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	stringCPU += sysInfo.dwNumberOfProcessors;
	return stringCPU;
}
bool Win32DeviceInfo::isInternetConection()
{
	return 1;
}

void Win32DeviceInfo::enableKeyboard(bool isEnable){
	
	if (isEnable)
	{	
		if (KeyboardHook)
		{
			CCLOG("remove keyboard");
			UnhookWindowsHookEx(KeyboardHook);
			KeyboardHook = NULL;
		}

		if (!KeyboardHook)
		{
			KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
		}
	}
	else
	{
		if (KeyboardHook)
		{
			CCLOG("remove keyboard");
			UnhookWindowsHookEx(KeyboardHook);
			KeyboardHook = NULL;
		}
	}
}

void Win32DeviceInfo::setKeyboardIsEnable(){
}