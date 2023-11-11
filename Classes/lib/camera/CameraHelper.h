#pragma once

#include "BaseSingleton.h"
#include "IAPDefinitions.h"
#include "MSAISpeakingSharePopup.h"

USING_NS_CC;

class CameraHelper : public BaseSingleton<CameraHelper> {

private: //private functions


public:
    CameraHelper();
    ~CameraHelper();
    void startCapture(bool isHighlightEnable, bool isSoundEnable);
    void endCapture(std::string fileName, std::function<void(bool)> callback);
    void setVisible(bool isVisible, std::function<void()> callback = nullptr);

    void clean(std::function<void()> callback = nullptr);
    void stop(std::function<void()> callback = nullptr);
    void saveVideoToFile(std::vector<std::string> videoPaths, std::string imagePath, std::string outputPath = "output.mp4", float duration = -1.f, std::function<void(bool, std::string)> callback = nullptr);
    std::vector<std::string> joinMultiVideo(std::vector<std::string> videoPaths, std::string outputPath = "output.mp4", std::function<void(bool, std::string)> callback = nullptr);
    void addViewToCamera(cocos2d::Vec2 postion, cocos2d::Size cameraSize);
    void enableBorder(int borderSize, cocos2d::Color4B color);
    void removeCamera(std::function<void()> callback);
    void moveFrontCameraToPos(cocos2d::Vec2 position, cocos2d::Size cameraSize);
    cocos2d::Rect convertCocosPositionToIOSScreenPos(cocos2d::Rect input);
    void authorization(bool firstCheckPermisison);
    bool getAuthorizationStatus();
    void leadUserToGrantPermissionSetting();
    void shareVideoIOSWithUrl(std::string videoPath, std::function<void()> callback = nullptr);
    void shareVideoAndroid(std::string videoPath, std::function<void()> callback = nullptr);
    void shareVideoWithUrl(std::string videoPath, std::function<void()> callback = nullptr);
    void startVideoRecorder();
    void stopVideoRecorder(std::function<void(void)> callback = nullptr, ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate = nullptr);
    void startVideoRecorderAndroid();
    void stopVideoRecorderAndroid(std::function<void(void)> callback = nullptr);

    void startVideoRecorderIOS();
    void stopVideoRecorderIOS(std::function<void(void)> callback = nullptr, ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate = nullptr);

    void enableAudioWhenRecordCamera(bool isEnable);
    int getOrientation();
    bool getVideoOrientationLockStatus();
    void setVideoOrientationLockStatus(bool status);
    bool getSupportIOS15();
    void requestNotifyPermision();
    bool checkNotifyPermision();
    void setIconApp();
private:


};
