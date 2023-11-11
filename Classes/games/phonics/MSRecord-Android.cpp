#include "MSRecord.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/JniHelper.h"
#include <jni.h>
#include "cocos2d.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MJDefault.h"

#define CLASS_NAME "org.cocos2dx.cpp.MSRecord"
USING_NS_CC;
#pragma mark - PLAY ANI OBJECT - One layer
#ifndef __MSRecordBridge__
#define __MSRecordBridge__
class MSRecordBridge {
public:
    static void startRecord(){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "startRecording", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
    static void stopRecord(){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopRecording", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    static void playAudioRecord(){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "startPlaying", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    static void stopAudioRecord(){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "stopPlaying", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    static string getFilePath() {
        string str_return = "";
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getFilePath", "()Ljava/lang/String;")) {
            jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
            t.env->DeleteLocalRef(t.classID);
            str_return = JniHelper::jstring2string(s);
            return str_return;
        }
        return str_return;
    }

    static MSRecord::PERMISSION_STATUS getPermissionStatus(){
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getPermissionStatus", "()I")) {
            jint b = t.env->CallStaticIntMethod(t.classID,t.methodID);
            t.env->DeleteLocalRef(t.classID);
            switch (b) {
                case 1: return MSRecord::PERMISSION_STATUS::GRANTED;
                case 2: return MSRecord::PERMISSION_STATUS::DENIED;
                case 3: return MSRecord::PERMISSION_STATUS::UNDETERMINED;
            }
        }
        return MSRecord::PERMISSION_STATUS::DENIED;
    }

    static void leadUserToGrantPermissionSetting(){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "leadUserToGrantPermissionSetting", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

    static void askPermission(std::function<void()> accepted = nullptr,std::function<void()> denied = nullptr){
        cocos2d::JniMethodInfo t;
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "askPermission", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }

};
#endif
MSRecord * MSRecord::createRecord(float timer, std::string phrase,bool isShowNativeRemainPopup) {
    auto p_food = new MSRecord();
    if (p_food && p_food->init(timer)) {
        p_food->autorelease();
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}
bool MSRecord::init(float timer,bool isShowNativeRemainPopup, bool isStartRecord) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    is_record_stop = false;
    time_record = 0.0f;
    _record_duration = timer;
    text_alert = cocos2d::ui::Text::create("recording...", "fonts/monkeystories_ultra.ttf", 25);
    this->addChild(text_alert);
    text_alert->setPositionY(-120);
    text_alert->setColor(Color3B::GRAY);
    text_alert->runAction(Repeat::create(Sequence::create(ScaleTo::create(0.5f, 1.15f), ScaleTo::create(0.5f, 1.0f), nullptr), 1000));
    text_alert->setVisible(false);
    this->schedule(schedule_selector(MSRecord::updateCountTimeRecord));
    MSRecordBridge::startRecord();
    if(MSRecordBridge::getPermissionStatus() != MSRecord::PERMISSION_STATUS::GRANTED){
        return false;
    }
    this->runAction(Sequence::create(DelayTime::create(_record_duration), CallFunc::create([=]{
        if (!is_record_stop) {
            this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
            is_record_stop = true;
            MSRecordBridge::stopRecord();
            text_alert->setString("stopRecord");
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                if (delegate) {
                    delegate->onFinishRecord(RECORD_DONE, MSRecordBridge::getFilePath());
                }
            }), nullptr));
        }
    }), nullptr));
    return true;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
MSRecord * MSRecord::createRecordWithDuration(float audioDuration,std::string path,bool isAVAudioSessionChange,bool isLowSettingEnable, Type type) {
    auto p_food = new MSRecord();
    if (p_food ) {
        p_food->_isAVAudioLowSettingEnable = isLowSettingEnable;
        p_food->_isisAVAudioSessionChange = isAVAudioSessionChange;
        p_food->_phrase = path;
        p_food->init(audioDuration,false,true);
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}
void MSRecord::checkBlow(){
    if (!is_record_stop) {
    }
}
bool MSRecord::isRecording(){
    return false;
}
void MSRecord::startRecord(){
}
#endif
void MSRecord::startPlay(bool _is_play_once){
    text_alert->setString("playing...");
    CCLOG("hoisunglog - onPlay2");
    MSRecordBridge::playAudioRecord();
    this->runAction(Sequence::create(DelayTime::create(time_record + 0.1f), CallFunc::create([=]{
        text_alert->setString("done...");
    }), DelayTime::create(0.5f), CallFunc::create([=]{
        if (delegate) {
            delegate->onFinishRecord(PLAY_DONE, MSRecordBridge::getFilePath());
        }
//        this->removeFromParent();
    }),nullptr));
}

void MSRecord::stopPlay() {

}

void MSRecord::stopRecord(){
    if (!is_record_stop) {
        this->unschedule(schedule_selector(MSRecord::updateCountTimeRecord));
        is_record_stop = true;
        MSRecordBridge::stopRecord();
        text_alert->setString("");
        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
            if (delegate) {
                delegate->onFinishRecord(RECORD_DONE, MSRecordBridge::getFilePath());
            }
        }), nullptr));
    }
}

void MSRecord::updateCountTimeRecord(float dt){
    time_record += dt;
}

void MSRecord::setDelegate(MSRecordDelegate *_delegate){
    delegate = _delegate;
}

void MSRecord::leadUserToGrantPermissionSetting(){
    MSRecordBridge::leadUserToGrantPermissionSetting();
}
void MSRecord::askPermission(std::function<void()> accepted,std::function<void()> denied){
    if(MSRecord::getPermissionStatus() == MSRecord::DENIED){
        MSRecord::leadUserToGrantPermissionSetting();
        return;
    }
    MSRecordBridge::askPermission(accepted,denied);
}

MSRecord::PERMISSION_STATUS MSRecord::getPermissionStatus(){
    return MSRecordBridge::getPermissionStatus();
}

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getStringLanguageForKey(JNIEnv *env, jclass clazz,jstring key ) {
        std::string str = LANGUAGE_MANAGER->getDisplayTextByKey(JniHelper::jstring2string(key));
        char *cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        delete [] cstr;
        jstring rtstr = env->NewStringUTF(cstr);
        return rtstr;

    }
}

#endif
