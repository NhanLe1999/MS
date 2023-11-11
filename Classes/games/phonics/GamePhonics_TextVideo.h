
#ifndef MJGamePhonics_TextVideo_hpp
#define MJGamePhonics_TextVideo_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "MSRecord.h"
#include "GAFWrapper.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#include "ui/UIVideoPlayer.h"
using namespace cocos2d::experimental::ui;
#endif

class MJGamePhonics_TextVideoDelegate {
public:
    virtual void onFinishAction(int finish_type) {};
    virtual void onRecordClicked() {};
    virtual void onPlayClicked() {};
    virtual void onNextTurnClicked() {};
};

class MJGamePhonics_TextVideo : public cocos2d::ui::Layout, public MJGamePhonics_TextVideoDelegate, public MSRecordDelegate, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGamePhonics_TextVideo);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    enum TEXTVIDEO_FINISH_TYPE {
        VIDEO_DONE = 1,
        HIGH_LIGHT_DONE = 2,
        SHOW_TEXT_DONE = 3,
        RECORD_DONE = 4,
        AUDIO_DONE = 5,
    };
    
    void setDelegate(MJGamePhonics_TextVideoDelegate * delegate);
    void playVideo();
    void showText();
    void hideText();
    void hightlightText();
    void startRecord();
    Layout* micLayout = NULL;
    
    struct HighlightInfo {
        std::string word;
        int index_start;
        int index_end;
        float time_start;
        float time_end;
    };
    
    struct ObjectInfo {
        std::string text;
        std::string audio_path;
       // std::string img_path;
        std::string video_path;
        float audio_duration;//video_duration, 
//        std::vector<HighlightInfo> list_highlight;
    };
    
    static MJGamePhonics_TextVideo * createGame(ObjectInfo _object);
    ObjectInfo _object_info;
    void resetTextVideoData(ObjectInfo _object);
    void removeVideo();
    void deleteLayerLock();
    cocos2d::ui::Layout *btrecord, *btplay, *btnext;
    void runLayoutDisplay(cocos2d::ui::Layout *layout, bool isShow);
    void acceptPermissionCallback();
    void denyPermissioCallback();

private:
    MJGamePhonics_TextVideoDelegate * delegate = nullptr;
    void didLoadFromCSB(ObjectInfo _object);
    void onEnter() override;
    Label *text_highlight = nullptr;
    void addVideoWithPath(std::string video_path);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    void videoEventCallback(Ref* sender, VideoPlayer::EventType eventType);
    VideoPlayer* videoPlayer = nullptr;
#endif
    
    void onFinishRecord(int finish_type,std::string url) override;
    
    int index_start = 0, index_end = 0;
    int sync_index = 0;
    void runHighLightText();
    void resetTextColor();
    GAFWrapper *television_gaf = nullptr;
    float scale_value = 1.0f;
    
    ui::ImageView *_logLayer = nullptr;
    
    
    void onPlay(cocos2d::Ref *sender);
    void onRecord(cocos2d::Ref *sender);
    MSRecord *_record = nullptr;
    MSRecord *_recordCopy = nullptr;
    bool _is_recording = false;
    void onNext(cocos2d::Ref *sender);
    void onExit() override;
};

CREATE_READER(MJGamePhonics_TextVideo);

#endif
