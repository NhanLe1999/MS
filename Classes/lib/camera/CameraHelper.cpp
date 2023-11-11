////
////  CameraHelper.cpp
////  MJStory
////
////  Created by Thien Phuoc on 09/03/2022.
////
//
//  CameraHelper.cpp
//  MJStory
//
//  Created by Thien Phuoc on 09/03/2022.
//

#include "CameraHelper.h"
#define CAMERA_MANAGER_CLASS "org.cocos2dx.cpp.AppActivity"
#include "cocos2d.h"
using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif


CameraHelper::CameraHelper() {

};
CameraHelper::~CameraHelper() {

};


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

void CameraHelper::startCapture(bool isHighlightEnable, bool isSoundEnable) {

    cocos2d::JniHelper::callStaticVoidMethod(CAMERA_MANAGER_CLASS, "OpenCamera");
}
void CameraHelper::enableBorder(int borderSize, cocos2d::Color4B color) {

}
void CameraHelper::removeCamera(std::function<void()> callback) {

}
void CameraHelper::endCapture(std::string fileName, std::function<void(bool)> callback) {

    cocos2d::JniHelper::callStaticVoidMethod(CAMERA_MANAGER_CLASS, "StopCamera");
    //mergeMultiVideo();

}
void CameraHelper::addViewToCamera(cocos2d::Vec2 postion, cocos2d::Size size)
{

}

void CameraHelper::enableAudioWhenRecordCamera(bool isEnable) {
}
void CameraHelper::stop(std::function<void()> callback) {

}
void CameraHelper::clean(std::function<void()> callback) {
}
void CameraHelper::setVisible(bool isVisible, std::function<void()> callback) {
}
void CameraHelper::saveVideoToFile(std::vector<std::string> videoPaths, std::string imagePath, std::string outputPath, float duration, std::function<void(bool, std::string)> callback)
{
    cocos2d::JniHelper::callStaticVoidMethod(CAMERA_MANAGER_CLASS, "SaveVideo");
}
void CameraHelper::moveFrontCameraToPos(cocos2d::Vec2 position, cocos2d::Size cameraSize) {

    cocos2d::JniHelper::callStaticVoidMethod(CAMERA_MANAGER_CLASS, "moveFrontCameraToPos", position.x, position.y, cameraSize.width, cameraSize.height);
}
cocos2d::Rect CameraHelper::convertCocosPositionToIOSScreenPos(cocos2d::Rect input) {

    cocos2d::JniHelper::callStaticVoidMethod(CAMERA_MANAGER_CLASS, "convertCocosPositionToIOSScreenPos", input.origin.x, input.origin.y, input.size.width, input.size.height);

    return input;
}
void CameraHelper::authorization(bool firstCheckPermisison) {


}
bool CameraHelper::getAuthorizationStatus() {

    return true;
}

void CameraHelper::leadUserToGrantPermissionSetting()
{

}

void CameraHelper::shareVideoAndroid(std::string videoPath, std::function<void()> callback) {

}

std::vector<std::string> CameraHelper::joinMultiVideo(std::vector<std::string> videoPaths, std::string outputPath, std::function<void(bool, std::string)> callback) {
    std::vector<std::string> outputs;
    return outputs;

}
int CameraHelper::getOrientation() {
    return 1;
}
#endif

void CameraHelper::shareVideoWithUrl(std::string videoPath, std::function<void()> callback) {

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    shareVideoIOSWithUrl(videoPath, callback);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    shareVideoAndroid(videoPath, callback);
#endif
}

void CameraHelper::startVideoRecorder() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    startVideoRecorderIOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    startVideoRecorderAndroid();
#endif
}

void CameraHelper::stopVideoRecorder(std::function<void(void)> callback, ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    stopVideoRecorderIOS(callback, aISpeakingSharePopupDelegate);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    stopVideoRecorderAndroid(callback);
#endif
}

void CameraHelper::startVideoRecorderAndroid() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif
}

void CameraHelper::stopVideoRecorderAndroid(std::function<void(void)> callback) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif
}