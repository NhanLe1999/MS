#ifndef MacCppBridge_h
#define MacCppBridge_h
#include "cocos2d.h"
#include <iostream>
#define MAC_CPP_BRIDGE MacCppBridge::getInstance()
class MacCppBridge{
public:
    MacCppBridge();
    enum IDCALLVIDEO {
        INTRO = 11,
        LESSON_FULL = 12,
        LESSON_MIDDLE = 13,
        LESSON_POPUP = 14,
        LESSON_FULL_GAME5 = 15,
    };
    
    static MacCppBridge *getInstance();
    static void playVideo(std::string fileName, int idCallVideo, bool autoLoop = false);
    static void onMacCppBridgeTest();
    static std::string getMACAddress();
    static std::string getOSXSystemInfo();
    static void setWindowWantsLayer();
    static cocos2d::Size getScreenSize();
    static void removeVideoMAC();
    static void startEventKeyBoardWithID(int index = 5);
    static void removeUserDefault();
    static void createContainerDataPath();
    static bool createMacPathFileByString(std::string strPath);
    static std::string getDocumentPath();
    static bool isInternetConnection();
    static void buyPackgeViaIAP(std::string indexPackageStr);
    static void restorePackageIAP();
    static void loadAllItemPackageIAP();
    static void displayIndicator(bool isDisplay);
    static void sendMailSupport(std::string subjectStr, std::string contentStr);
    static bool osXlessthan10_9();
    static int getIDCallVideo();
};
#endif
