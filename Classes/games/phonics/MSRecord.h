#ifndef __MSRecord__
#define __MSRecord__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define audio_duration_max 5.0f
using namespace cocos2d;
using namespace std;

class MSRecordDelegate {
public:
    virtual void onFinishRecord(int finish_type, std::string url = "") {};
};

class MSRecord: public cocos2d::ui::Layout, public MSRecordDelegate {
    
public:

    enum class Type {
        AVAudioSessionCategoryOptionMixWithOthers,
        AVAudioSessionCategoryOptionDuckOthers,
        AVAudioSessionCategoryOptionAllowBluetooth,
        AVAudioSessionCategoryOptionDefaultToSpeaker,
        AVAudioSessionCategoryOptionInterruptSpokenAudioAndMixWithOthers,
        AVAudioSessionCategoryOptionAllowBluetoothA2DP,
        AVAudioSessionCategoryOptionAllowAirPlay,
        AVAudioSessionCategoryOptionOverrideMutedMicrophoneInterruption
    };
    
    CREATE_FUNC(MSRecord);
    
    static MSRecord * createRecordWithDuration(float audioDuration,std::string path,bool isAVAudioSessionChange,bool isLowSettingEnable,Type type);
    static MSRecord * createRecord(float timer = audio_duration_max, std::string phrase = "",bool isShowNativeRemainPopup =false);
    bool init(float timer = audio_duration_max,bool isShowNativeRemainPopup =false,bool isStartRecord = true);
    void startRecord();
    float _record_duration = 0;
    void setDelegate(MSRecordDelegate *_delegate);
    
    enum RECORD_TYPE {
        RECORD_DONE = 4,
        PLAY_DONE = 5,
    };
    
    enum PERMISSION_STATUS{
        GRANTED = 1,
        DENIED = 2,
        UNDETERMINED = 3 
    };
    
    void startPlay(bool _is_play_once = true);
    void stopPlay();
    void stopRecord();
    void checkBlow();
    bool isRecording();
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
    void showRemainPermissionPopup();
#endif
    bool is_record_stop = false;
    static void leadUserToGrantPermissionSetting();
    void acceptPermissionCallback();
    void denyPermissioCallback();
    static PERMISSION_STATUS getPermissionStatus();
    static void askPermission(std::function<void()> accepted=nullptr,std::function<void()> denied=nullptr);
private:
    MSRecordDelegate *delegate = nullptr;
    cocos2d::ui::Text *text_alert = nullptr;
    void updateCountTimeRecord(float dt);
    float time_record = 0.0f;
    std::string _phrase;
    bool _isAVAudioLowSettingEnable = false;
    bool _isisAVAudioSessionChange = true;
    bool is_record_permission_granted;
    bool is_show_remainding_permission_popup = false;
    
    Type _type = Type::AVAudioSessionCategoryOptionDefaultToSpeaker;
};

#endif
