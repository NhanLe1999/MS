#ifndef __Monkey__NativeBridge__
#define __Monkey__NativeBridge__
#define key_crashlytic "story_page"
#define key_crashlytics_device "device_id"

#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"
using namespace std;
USING_NS_CC;
class NativeBridge: public Layer
{
public:
    static void keepBackLightOn(bool keep);
    /*
     So sanh 2 string co giong nhau hay khong
     */
    static bool utf8StringCompare(string str1, string str2);
    /*
     lay chieu dai cua string
     */
    static int utf8LengthOfString(string str);
    /*
     Lay ky tu cua string
     */
    static string utf8characterAtIndex(string str, int index);
    
    static string string_towupper(string str);
    static string string_towlower(string str);
    static bool networkAvaiable();
    //MARK: push
    static string getDeviceToken();
    static void PushReminder(vector<string> messages, vector<int> listDate);
    //danh cho android
    static void initPush();
    static void initAlarm(string json);
    static float getTimeZoneOffset();
    
    static NativeBridge* getInstance();
    static vector<int> findStringOnString(std::string subject_str, std::string find_str, bool isCheckValid = true);
    static bool checkValidPost(int pos, std::string subject_str, std::string find_str);
    static std::string hsStringSub(std::string wordgr, int indexChar, int length);
    static std::string hsStringToKhongDau(std::string subject_str, int lang_id = 1);
    
    static void rateAppMJStories();
    static bool isTabletDevice();
    static bool isRetinaDevice();
    static void changeDeviceOrientation(bool isPortrait);
    static void setCrashlyticsKeyValue(std::string key, std::string value);
    static bool isRamWarning();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    static std::string hsStringAtIndex(int indexChar, std::string wordgr);
    static std::string hsToLowerString(std::string inputStr);
    static int hsNumberCharactersOfString(std::string inputStr);
    static std::string hsToUpperString(std::string inputStr);
    static std::string chuthuongx;
    static std::string chuhoax;
    static string testtest();
    static std::vector<int> hsFindString(std::string substring, std::string findstring, bool isCheckValid = true);
    //static std::vector<int> hs9FindString(std::string substring, std::string findstring);
    std::vector<int> hsFindString(std::string substring, std::string findstring);
    //vector<int> findStringOnString(std::string subject_str, std::string find_str);
#else
    static void pushDeviceTokenToServer();
#endif

    static void callProscoreAPI(std::vector<std::pair<std::string, std::string>> params, std::function<void(std::string)> onSucceed = nullptr, std::function<void()> onConnectionError = nullptr);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    void* _mailHelper;
    void sendMail(string subject, string content);
    static NativeBridge *instance;
#else
    static void sendMail(string subject, string content, string attach);
    
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static void addSkipBackupAttributeToItemAtPath(string filePathString);
    
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static string removeAccent(string str);
    static float getInchDevice();
    
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //static NativeBridge *instance;
#endif
    
private:
    
};

#endif /* defined(__Monkey__NativeBridge__) */
