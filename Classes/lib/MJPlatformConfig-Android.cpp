//
//  MJPlatformConfig-Android.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#include "MJPlatformConfig-Android.h"
#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "jni/JniHelper.h"
#include <jni.h>
#include "cocos2d.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#define HELPER_CLASS "org.cocos2dx.cpp.MJHelper"

#define CLASS_NAME_PLATFORM_CONFIG "org.cocos2dx.cpp.PlatformConfig"

namespace mj {

    /** get unique device id
     *  @return a std::string contains MAC address, identifier for vendor and uuid saved in keychain with delimiter "|"
     */
    std::string PlatformConfigAndroid::getPlatformDeviceID() {
        auto fake_id = MJDEFAULT->getStringForKey("fake_id", "");
        if (!fake_id.empty()) {
            return fake_id;
        }

        std::string str_return = "||";
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME_PLATFORM_CONFIG, "getDeviceId", "()Ljava/lang/String;")) {
            jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            str_return = cocos2d::JniHelper::jstring2string(s);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(s);
            return str_return;
        }
        return str_return;
    }

    std::string PlatformConfigAndroid::getOS() {
        return "android";
    }

    std::string PlatformConfigAndroid::getAppVersion() {
        std::string str_return = "";
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME_PLATFORM_CONFIG, "getAppVersion", "()Ljava/lang/String;")) {
            jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            str_return = cocos2d::JniHelper::jstring2string(s);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(s);
            return str_return;
        }
        return str_return;

    }

    std::string PlatformConfigAndroid::getAccessToken() {
        std::string str_return = "";
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME_PLATFORM_CONFIG, "getAccessToken", "()Ljava/lang/String;")) {
            jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            str_return = cocos2d::JniHelper::jstring2string(s);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(s);
            return str_return;
        }
        return str_return;
    }

    bool PlatformConfigAndroid::phoneSupport(std::string phoneNumber) {
        cocos2d::JniMethodInfo t;
        cocos2d::JniHelper::callStaticVoidMethod(CLASS_NAME_PLATFORM_CONFIG,"phoneSupport", phoneNumber);
//        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME_PLATFORM_CONFIG, "phoneSupport", phoneNumber)) {
//            t.env->CallStaticVoidMethod(t.classID, t.methodID);
//            t.env->DeleteLocalRef(t.classID);
//        }
        return true;
    }

    bool PlatformConfigAndroid::mailSupport(std::string mail)
    {
       std::string content = LANGUAGE_MANAGER->getDisplayTextByKey("SupportView_Mail") ;
              content += getDeviceInfo();

        std::string subject = LANGUAGE_MANAGER->getDisplayTextByKey("SupportView_Mail_title");
               subject += getAppName() + " v" + getAppVersion();
        
        cocos2d::JniHelper::callStaticVoidMethod(CLASS_NAME_PLATFORM_CONFIG, "mailSupport", subject, content,LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.email"));

        return true;
    }


    std::string PlatformConfigAndroid::getDeviceInfo() {
        return cocos2d::JniHelper::callStaticStringMethod(CLASS_NAME_PLATFORM_CONFIG, "getDeviceInfo");
    }
 
    std::string PlatformConfigAndroid::getDeviceInfoShort() {
        if (_device_info.empty()) {
            _device_info = cocos2d::JniHelper::callStaticStringMethod(CLASS_NAME_PLATFORM_CONFIG, "getDeviceInfoShort");
        }
        return _device_info;
    }

    void PlatformConfigAndroid::clearUserDefaults() {
//        JniHelper::callStaticVoidMethod("org/cocos2dx/lib/Cocos2dxHelper", "resetPrefs");
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.lib.Cocos2dxHelper", "resetPrefs");
    }
    
    std::string PlatformConfigAndroid::getUserDefaults() {
        return JniHelper::callStaticStringMethod("org/cocos2dx/lib/Cocos2dxHelper", "getPrefs");
    }
    
    void PlatformConfigAndroid::setUserDefaults(std::string info){
    
        JniHelper::callStaticVoidMethod("org/cocos2dx/lib/Cocos2dxHelper", "setPrefs", info);
    }

    int PlatformConfigAndroid::getDeviceYearClass() {
        return cocos2d::JniHelper::callStaticIntMethod(CLASS_NAME_PLATFORM_CONFIG, "getYearClass");
    }
    
    std::string PlatformConfigAndroid::getPackageName() {
        return cocos2d::JniHelper::callStaticStringMethod(CLASS_NAME_PLATFORM_CONFIG, "getPackageName");
    }

    void PlatformConfigAndroid::showVMonkeyPage() {
        cocos2d::Application::getInstance()->openURL( CC_PLATFORM_HUAWEI ? "https://appgallery.huawei.com/#/app/C104102279" : "market://details?id=com.earlystart.android.monkeyjunior.story.vn");
    }
	
    void PlatformConfigAndroid::showLearnToReadStorePage() {
        cocos2d::Application::getInstance()->openURL(CC_PLATFORM_HUAWEI ? "https://appgallery.huawei.com/#/app/C103598155" : "market://details?id=com.earlystart.android.monkeyjunior");
    }

    void PlatformConfigAndroid::showStoriesStorePage() {
        cocos2d::Application::getInstance()->openURL(CC_PLATFORM_HUAWEI ? "https://appgallery.huawei.com/#/app/C103189517" : "market://details?id=com.earlystart.android.monkeyjunior.story");
    }

    void PlatformConfigAndroid::showMathStorePage() {
        cocos2d::Application::getInstance()->openURL(CC_PLATFORM_HUAWEI ? "https://www.monkeystories.vn/tai-tu-dong.html?app_id=50" : "market://details?id=com.earlystart.monkeymath");
    }

    std::string PlatformConfigAndroid::getAppName() {
        return cocos2d::JniHelper::callStaticStringMethod(CLASS_NAME_PLATFORM_CONFIG, "getAppName");
    }

    bool PlatformConfigAndroid::canOpenLearnToRead() {
        return false;
    }

    void PlatformConfigAndroid::openLearnToRead() {

    }

    void PlatformConfigAndroid::shareViral(std::string url){
        if (cocos2d::JniHelper::callStaticBooleanMethod(CLASS_NAME_PLATFORM_CONFIG,"isAppInstalled", "com.facebook.katana")) {
            cocos2d::JniHelper::callStaticVoidMethod(CLASS_NAME_PLATFORM_CONFIG,"shareFacebook", url);
        }else{
            auto content = "Con mình đang học tiếng Anh trên Monkey Stories và mình thấy rất hữu ích. Cả nhà có thể cho con trải nghiệm miễn phí, tại đây nhé: ";
            url = StringUtils::format("https://www.facebook.com/dialog/share?app_id=520137134992261&display=popup&href=%s&quote=%s", mj::helper::urlencode(url).c_str(), mj::helper::urlencode(content).c_str());
            cocos2d::Application::getInstance()->openURL(url);
        }
    }

    void PlatformConfigAndroid::shareScreenShot(std::string file_url){
        if (cocos2d::JniHelper::callStaticBooleanMethod(CLASS_NAME_PLATFORM_CONFIG,"isAppInstalled", "com.facebook.katana")) {
            cocos2d::JniHelper::callStaticVoidMethod(CLASS_NAME_PLATFORM_CONFIG,"shareScreenShot", file_url);
        }
    }
    
    void PlatformConfigAndroid::saveScreenShot(std::string file_url, std::string file_name){
        if(cocos2d::JniHelper::callStaticBooleanMethod(CLASS_NAME_PLATFORM_CONFIG, "saveScreenShot", file_url)){
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("save picture");
        }
    }
}

#endif
