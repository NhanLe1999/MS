//
//  CCNativeAlert_Android.cpp
//
//  Created by Justin Hawkwood on 2015/02/24.
//
//
#include "NativeBridge.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JsonDump.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include "AppDelegate.h"
#define  CLASS_NAME "org/cocos2dx/lib/EduNativeBridge"
#define  CLASS_NAME_GCM "org/cocos2dx/gcm/EduGcm"
#include "ProScore.h"
#include "MsNewDownloadFile.h"
#include "MsNewDownloadFileThumb.h"

#include "MsNewRequest.h"

#define TG3_GRAVITY_EARTH                    (9.80665f)

bool NativeBridge::utf8StringCompare(string str1, string str2) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "utf8StringCompare", "(Ljava/lang/String;Ljava/lang/String;)Z")) {
        jstring param1 = t.env->NewStringUTF(str1.c_str());
        jstring param2 = t.env->NewStringUTF(str2.c_str());
        jboolean retV = t.env->CallStaticBooleanMethod(t.classID,t.methodID,param1, param2);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(param2);
        t.env->DeleteLocalRef(t.classID);
        return retV;
    }
    return false;
}

int NativeBridge::utf8LengthOfString(string str) {
    int length = 0;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "utf8LengthOfString", "(Ljava/lang/String;)I")) {
        jstring param1 = t.env->NewStringUTF(str.c_str());
        length = (jint)t.env->CallStaticIntMethod(t.classID,t.methodID,param1);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
    }
    return length;
}

string NativeBridge::utf8characterAtIndex(string str, int index) {
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "utf8characterAtIndex", "(Ljava/lang/String;I)Ljava/lang/String;")) {
        jstring param1 = t.env->NewStringUTF(str.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1, index);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        return str_return;
    }
    return str_return;
}

string NativeBridge::string_towupper(string str) {
    if (str == "") {
        return str;
    }
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "toUpperCaseString", "(Ljava/lang/String;)Ljava/lang/String;")) {
        jstring param1 = t.env->NewStringUTF(str.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        return str_return;
    }
    return str_return;
}

string NativeBridge::string_towlower(string str) {
    if (str == "") {
        return str;
    }
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "toLowerCaseString", "(Ljava/lang/String;)Ljava/lang/String;")) {
        jstring param1 = t.env->NewStringUTF(str.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        return str_return;
    }
    return str_return;
}
bool NativeBridge::networkAvaiable() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "networkAvaiable", "()Z")) {
        jboolean retV = t.env->CallStaticBooleanMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return retV;
    }
    return true;
}

void NativeBridge::sendMail(string subject, string content, string attach) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "sendEmail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jsubject = t.env->NewStringUTF(subject.c_str());
        jstring jcontent = t.env->NewStringUTF(content.c_str());
        jstring jattach = t.env->NewStringUTF(attach.c_str());
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jsubject,jcontent,jattach);
        t.env->DeleteLocalRef(jsubject);
        t.env->DeleteLocalRef(jcontent);
        t.env->DeleteLocalRef(jattach);
        t.env->DeleteLocalRef(t.classID);
    }
}

float NativeBridge::getTimeZoneOffset() {
    float offset = 0;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getTimeZoneOffset", "()F")) {
        offset = (jfloat)t.env->CallStaticFloatMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return offset;
}


void NativeBridge::PushReminder(vector<string> messages, vector<int> listDate) {
    
}

void NativeBridge::initPush() {
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_GCM, "initPushNotificationGCM", "()V")) {
//        t.env->CallStaticVoidMethod(t.classID,t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
}


void NativeBridge::initAlarm(string json) {
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_GCM, "initAlarm", "(Ljava/lang/String;)V")) {
//        jstring j_json = t.env->NewStringUTF(json.c_str());
//        t.env->CallStaticVoidMethod(t.classID,t.methodID, j_json);
//        t.env->DeleteLocalRef(j_json);
//        t.env->DeleteLocalRef(t.classID);
//    }
}

string NativeBridge::getDeviceToken() {
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME_GCM, "getDeviceToken", "()Ljava/lang/String;")) {
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        return str_return;
    }
    return str_return;
}

void NativeBridge::keepBackLightOn(bool keep)
{
    return;
}

vector<int> NativeBridge::findStringOnString(std::string subject_str, std::string find_str, bool isCheckValid){
    std::vector<int> _results;
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hsFindString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")) {

        jstring param1 = t.env->NewStringUTF(subject_str.c_str());
        jstring param2 = t.env->NewStringUTF(find_str.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1,param2);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(param2);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        str_return = str_return.substr(0, str_return.length()-1);
        str_return = std::string("[") + str_return;
        str_return = str_return + std::string("]");
        cocos2d::ValueVector _arr = JsonDump::getInstance()->dumpValueVectorFromString(str_return);
        for (int i = 0; i < (int)_arr.size(); i ++) {
            int pos = _arr[i].asInt();
            if (!isCheckValid || checkValidPost(pos, subject_str, find_str)) {
                _results.push_back(pos);
                CCLOG("add pos");
            }
        }
        return _results;
    }
    return _results;
}
std::string NativeBridge::hsStringSub(std::string wordgr, int indexChar, int length){
    string str_return = "";
    string _index = cocos2d::StringUtils::toString(indexChar);
    string _length = cocos2d::StringUtils::toString(length);
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hsSubString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")) {
        jstring param1 = t.env->NewStringUTF(wordgr.c_str());
        jstring param2 = t.env->NewStringUTF(_index.c_str());
        jstring param3 = t.env->NewStringUTF(_length.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1,param2,param3);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        CCLOG("str_return = %s", str_return.c_str());
        return str_return;
    }
    return str_return;
}

bool NativeBridge::checkValidPost(int pos, std::string subject_str, std::string find_str){
    auto length_subject = utf8LengthOfString(subject_str);
    auto length_find = utf8LengthOfString(find_str);
    if (length_subject == length_find) {
        return true;
    }else{
        auto daudacbiet = std::string("\n“:.!-?,;\"' ");
        if (pos == 0) {
            auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
            auto daudacbiet_end = daudacbiet;
            
            for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                if (str_check_end == daudacbiet_end.substr(j, 1)) {
                    return true;
                }
            }
            
        }else if (pos + length_find == length_subject){
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    return true;
                }
            }
            
        }else{
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            auto check_start = false;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    check_start = true;
                    break;
                }
            }
            
            if (check_start) {
                auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
                auto daudacbiet_end = daudacbiet;
                
                for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                    if (str_check_end == daudacbiet_end.substr(j, 1)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

string NativeBridge::removeAccent(string str){
    string str_return = "";
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "removeAccent", "(Ljava/lang/String;)Ljava/lang/String;")) {
        jstring param1 = t.env->NewStringUTF(str.c_str());
        jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,param1);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(t.classID);
        str_return = JniHelper::jstring2string(s);
        return str_return;
    }
    return str_return;
}

string NativeBridge::hsStringToKhongDau(std::string subject_str, int lang_id){
    string re_str;
    //subject_str = string_towlower(subject_str);
    
    if (lang_id == 1){
        return subject_str;
    }else{
        subject_str = removeAccent(subject_str);
//        for (int i = 0; i < utf8LengthOfString(subject_str); i++) {
//            auto sub_str = utf8characterAtIndex(subject_str, i);//utf8characterAtIndex(subject_str, i);
//            if (sub_str == string("đ")) {
//                re_str += string("d");
//            }else{
//                re_str += sub_str;
//            }
//        }
        return subject_str;//re_str;
    }
}

void NativeBridge::changeDeviceOrientation(bool isPortrait){}

void NativeBridge::setCrashlyticsKeyValue(std::string key, std::string value) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setCrashlyticsKeyValue", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring param1 = t.env->NewStringUTF(key.c_str());
        jstring param2 = t.env->NewStringUTF(value.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, param1, param2);
        t.env->DeleteLocalRef(param1);
        t.env->DeleteLocalRef(param2);
        t.env->DeleteLocalRef(t.classID);
    }
}

float NativeBridge::getInchDevice(){
    double inch_device = 0;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getInchDevice", "()D")) {
        inch_device = (jdouble)t.env->CallStaticDoubleMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return (float)inch_device;
}

#include "StoryConfigManager.h"
void NativeBridge::rateAppMJStories(){
    if (CONFIG_MANAGER->isVietnameseStories()) {
        Application::getInstance()->openURL("https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior.story.vn");
    }else Application::getInstance()->openURL("https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior.story");
}

void NativeBridge::pushDeviceTokenToServer(){
    AppDelegate *app = (AppDelegate*)Application::getInstance();
    app->pushTokenToServer();
    app = nullptr;
}

bool NativeBridge::isTabletDevice(){
    if (getInchDevice() > 6.5) {
        return true;
    }
    return false;
}

bool NativeBridge::isRetinaDevice(){
    if (getInchDevice() > 7.5) {
        return true;
    }
    return true;
}

bool NativeBridge::isRamWarning(){
    float memory_size = 0;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxIMAC", "getRamSize", "()F")) {
        memory_size = (jfloat)t.env->CallStaticFloatMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    CCLOG("android memory_size = %f", memory_size);
    if (memory_size <= 0.0f) memory_size = 1.0f;
    return memory_size < 0.3f;
}

void NativeBridge::callProscoreAPI(std::vector<std::pair<std::string,std::string>> params,std::function<void(std::string)> onSucceed,std::function<void()> onConnectionError ){
    std::string data = "";
    for(int i=0;i<params.size();i++){
        data+=params[i].first+"="+params[i].second+"&";
    }
    data.pop_back();//remove last &
    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.ProScoreAPI","sendPostRequest",data);
}

/*
void NativeBridge::callProscoreAPI_sec(std::function<void(std::string)> onSucceed,std::function<void()> onConnectionError){

    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.Speech","getSpeechInput");
}
 */
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_AppActivity_onReciveDeppLink(JNIEnv* env, jclass clazz, jstring url)
{
    // TODO: implement onReciveDeppLink()
    const jclass stringClass = env->GetObjectClass(url);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(url, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string uri = std::string((char*)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    MK_RUN_IN_COCOS_THREAD(GlobalFunc::onDeepLink(uri);)
}

extern "C"
{
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ProScoreAPI_onRequestSuccess(JNIEnv * env, jobject thiz,
                                                                          jstring dsa)
{
    const jclass stringClass = env->GetObjectClass(dsa);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(dsa, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);

    //
    if(auto node = utils::findChild(Director::getInstance()->getRunningScene(),"ProScoreInstance")){

        if(auto game = dynamic_cast<ProScore*>(node->getParent())) {

            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                game->onRequestSucceedElsa(ret);
                //game->onRequestSucces_test(ret);
            });
        }

    }

}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ProScoreAPI_onRequestFailure(JNIEnv *env, jclass clazz) {
    // TODO: implement onRequestFailure()
    if(auto game = utils::findChild<ProScore*>(Director::getInstance()->getRunningScene(),"ProScoreInstance")){

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            game->onConnectionError();
        });

    }
}

JNIEXPORT jboolean JNICALL
Java_org_cocos2dx_cpp_ProScoreAPI_isDevEnviroment(JNIEnv *env, jclass clazz) {
    // TODO: implement isDevEnviroment()
    return false;
}


}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFile_processEvent(JNIEnv *env, jclass clazz, jint sizeFileReceive, jlong totalFileSize) {
    // TODO: implement processEvent()
    double pr = (double) sizeFileReceive / (double) (totalFileSize);

    ms::NewDownload::getInstance().processDownload(pr);
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFile_onSucessEvent(JNIEnv *env, jclass clazz) {
    // TODO: implement onSucessEvent()
    ms::NewDownload::InfoFileDownload  infoFileDownload;
    ms::NewDownload::getInstance().downloadSucess(infoFileDownload);
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFile_onErrorEvent(JNIEnv *env, jclass clazz, jobjectArray value) {
    // TODO: implement onErrorEvent()

    vector<string> stringValueEvent;

    if(value != nullptr)
    {
        int size = env->GetArrayLength(value);

        for (int i=0; i < size; ++i)
        {
            jstring stringValue = static_cast<jstring>(env->GetObjectArrayElement(value, i));

            const jsize strLen = env->GetStringUTFLength(stringValue);
            const char* mayarray = env->GetStringUTFChars(stringValue, 0);
            if(mayarray != nullptr)
            {
                string str(mayarray, strLen);
                stringValueEvent.push_back(str);
                env->ReleaseStringUTFChars(stringValue, mayarray);
            }
            env->DeleteLocalRef(stringValue);
        }
    }
    ms::NewDownload::getInstance().downloadError(stringValueEvent);
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFile_getDataBy(JNIEnv *env, jclass clazz) {
    // TODO: implement getDataBy()
    vector<unsigned char> buf;
    //ms::NewDownload::getInstance().onSucessDownloadThumbQueue(/*&buf*/);
}
extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFileThumb_processEvent(JNIEnv *env, jclass clazz, jint sizeFileReceive, jlong totalFileSize) {
    // TODO: implement processEvent()
    double pr = (double) sizeFileReceive / (double) (totalFileSize);

    ms::MsNewDownloadFileThumb::getInstance().processDownload(pr);
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFileThumb_onSucessEvent(JNIEnv *env, jclass clazz) {
    // TODO: implement onSucessEvent()
    ms::MsNewDownloadFileThumb::getInstance().downloadSucess();
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFileThumb_onErrorEvent(JNIEnv *env, jclass clazz, jobjectArray value) {
    // TODO: implement onErrorEvent()

    vector<string> stringValueEvent;

    if(value != nullptr)
    {
        int size = env->GetArrayLength(value);

        for (int i=0; i < size; ++i)
        {
            jstring stringValue = static_cast<jstring>(env->GetObjectArrayElement(value, i));

            const jsize strLen = env->GetStringUTFLength(stringValue);
            const char* mayarray = env->GetStringUTFChars(stringValue, 0);
            if(mayarray != nullptr)
            {
                string str(mayarray, strLen);
                stringValueEvent.push_back(str);
                env->ReleaseStringUTFChars(stringValue, mayarray);
            }
            env->DeleteLocalRef(stringValue);
        }
    }
    ms::MsNewDownloadFileThumb::getInstance().downloadError(stringValueEvent);
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DownloadFileThumb_getDataBy(JNIEnv *env, jclass clazz) {
    // TODO: implement getDataBy()
    vector<unsigned char> buf;
    //ms::NewDownload::getInstance().onSucessDownloadThumbQueue(/*&buf*/);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_AppActivity_onRequestSucceed_1sec(JNIEnv *env, jclass clazz,
                                                        jstring message) {
    // TODO: implement onRequestSucceed_sec()
    const jsize strLen = env->GetStringUTFLength(message);
    const char* mayarray = env->GetStringUTFChars(message, 0);
    if(mayarray != nullptr)
    {
        std::string str(mayarray, strLen);
        env->ReleaseStringUTFChars(message, mayarray);
        env->DeleteLocalRef(message);
        
        if(auto node = utils::findChild(Director::getInstance()->getRunningScene(),"ProScoreInstance")){
            
            if(auto game = dynamic_cast<ProScore*>(node->getParent())) {
                game->onFinishRecordGoogle(4, "", str);
            }
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ReadRequest_onRequestSucess(JNIEnv *env, jclass clazz, jstring respone, jobject is_sucess) {

    // TODO: implement onRequestSucess()
    const jsize strLen = env->GetStringUTFLength(respone);
    const char* mayarray = env->GetStringUTFChars(respone, 0);
    if(mayarray != nullptr)
    {
        string str(mayarray, strLen);
        env->ReleaseStringUTFChars(respone, mayarray);
        env->DeleteLocalRef(respone);
        ms::newApi::Request::getInstance().onRequestSucess(str, is_sucess);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_NewDownloadFile_processEvent(JNIEnv *env, jclass clazz, jint process) {
    // TODO: implement processEvent()

    int p = process;
    double p1 = p / 100.0f;

    ms::NewDownload::getInstance().processDownload(p1);

}
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_NewDownloadFile_onSucessEvent(JNIEnv *env, jclass clazz) {
    // TODO: implement onSucessEvent()
    ms::NewDownload::InfoFileDownload  infoFileDownload;
    ms::NewDownload::getInstance().downloadSucess(infoFileDownload);
}


extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BinaryFileDownloader_processEvent(JNIEnv *env, jclass clazz,
                                                        jint sizeFileReceive,
                                                        jlong totalFileSize) {
    // TODO: implement processEvent()

    double pr = (double) sizeFileReceive / (double) (totalFileSize);

    ms::NewDownload::getInstance().processDownload(pr);
}
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BinaryFileDownloader_onSucessDownload(JNIEnv *env, jclass clazz) {
    // TODO: implement onSucessDownload()
    ms::NewDownload::InfoFileDownload  infoFileDownload;
    ms::NewDownload::getInstance().downloadSucess(infoFileDownload);
}
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BinaryFileDownloader_onErrorDownload(JNIEnv *env, jclass clazz,
                                                           jobjectArray value) {
    // TODO: implement onErrorDownload()

    vector<string> stringValueEvent;

    if(value != nullptr)
    {
        int size = env->GetArrayLength(value);

        for (int i=0; i < size; ++i)
        {
            jstring stringValue = static_cast<jstring>(env->GetObjectArrayElement(value, i));

            const jsize strLen = env->GetStringUTFLength(stringValue);
            const char* mayarray = env->GetStringUTFChars(stringValue, 0);
            if(mayarray != nullptr)
            {
                string str(mayarray, strLen);
                stringValueEvent.push_back(str);
                env->ReleaseStringUTFChars(stringValue, mayarray);
            }
            env->DeleteLocalRef(stringValue);
        }
    }
    ms::NewDownload::getInstance().downloadError(stringValueEvent);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_ReadRequest_onRequestError(JNIEnv *env, jclass clazz, jint code,
                                                 jstring message, jstring respone) {
    // TODO: implement onRequestError()

    if(code == -1)
    {
        return;
    }
    const jsize strLen = env->GetStringUTFLength(respone);
    const char* mayarray = env->GetStringUTFChars(respone, 0);
    std::string responestr = "none";
    if(mayarray != nullptr)
    {
        string strRespone(mayarray, strLen);
        env->ReleaseStringUTFChars(respone, mayarray);
        env->DeleteLocalRef(respone);
        responestr = strRespone;
    }


    const jsize strLen1 = env->GetStringUTFLength(message);
    const char* mayarray1 = env->GetStringUTFChars(message, 0);
    if(mayarray1 != nullptr)
    {
        string strMessage(mayarray1, strLen1);
        env->ReleaseStringUTFChars(message, mayarray1);
        env->DeleteLocalRef(message);
        ms::newApi::Request::getInstance().onRequestError(code, strMessage, responestr);
    }

}

#endif