#pragma once
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "SpineWrapper.h"
#include "MSAISpeakingSharePopup.h"

struct MergedVideoInfo {
  
    std::string  _actorQuesVideoPath;

    std::string  _userNormalVideoPath;

    std::string  _actorListeningVideoPath;

    std::string  _userSpeakVideoPath;
    
    std::string  _imagePath;

    float       _actorVideoQuestDurationTime = -1.0f;
};
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

struct AISpeakingReviewScreenVideoInfo {
    
    int         _vID = -1;
    std::string _url;
    float       _duration   = -1.0f;
    float       _timeStart  = -1.0f;
    float       _timeEnd    = -1.0f;
    
    AISpeakingReviewScreenVideoInfo() {}
    
    AISpeakingReviewScreenVideoInfo(int vID, std::string url) {
        _vID = vID;
        _url = url;
    }
};

struct AISpeakingReviewScreenTurnInfo {
    
    AISpeakingReviewScreenVideoInfo _leftVideo;
    
    AISpeakingReviewScreenVideoInfo _rightVideo;

    std::string _imagePath = "";

    bool _isNextTurnBasedOnRightVideo = true;

    AISpeakingReviewScreenTurnInfo() {}
    
    AISpeakingReviewScreenTurnInfo(AISpeakingReviewScreenVideoInfo leftVideo,AISpeakingReviewScreenVideoInfo rightVideo,std::string imagePath , bool isNextTurnBasedOnRightVideo){
        
        _leftVideo = leftVideo;
        _rightVideo = rightVideo;
        _isNextTurnBasedOnRightVideo = isNextTurnBasedOnRightVideo;
        _imagePath = imagePath;

    };
};

class AISpeakingReviewScreen : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::AISpeakingSharePopupDelegate {

public:

    CREATE_FUNC(AISpeakingReviewScreen);

    static Scene* createScene(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo _lessonInfo = mj::model::AISpeakingLessonInfo());
    
    static AISpeakingReviewScreen* createView(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo _lessonInfo = mj::model::AISpeakingLessonInfo());

private:
    
    void didLoadFromCSB(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo _lessonInfo = mj::model::AISpeakingLessonInfo());
    void playPreviewStart(std::function<void()> callback = nullptr);
    void playPreviewContent(std::vector<AISpeakingReviewScreenTurnInfo> infoList);
    void onTryAgain(Ref* sender);

    void onLetTalk(Ref* sender);

    void onReplay(Ref* sender);

    void onBackButtonClicked(Ref* sender);
    
    void playSequenceVideos();
    void onCloseButtonClick(cocos2d::Ref* sender) override;
    void onShareButtonClicked(cocos2d::Ref* sender) override ;
    void onFinishButtonClicked(cocos2d::Ref* sender) override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
private:
    CC_SYNTHESIZE(std::string, _outputVideoPath, OutputVideoPath);
    CC_SYNTHESIZE(mj::model::AISpeakingLessonInfo, _lessonInfo, LessonInfo);

    void showPopupShare();
    void hidePopupShare();
    void setOpacitySharePopup(int opacity);
    void pushEventAIspeakingToCleaverTap(bool isShare);
	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
    
    std::vector<AISpeakingReviewScreenTurnInfo> _turnInfoList;
    
    int     _currentID = 0;
    
    float   _currentPercent = 0;
    
    cocos2d::experimental::ui::VideoPlayer* _leftVideoPlayer = nullptr;

    cocos2d::experimental::ui::VideoPlayer* _rightVideoPlayer = nullptr;

    AISpeakingReviewScreenTurnInfo _currentTurnInfo;
    
    cocos2d::Node* _leftVideoArea = nullptr;
    cocos2d::Node* _rightVideoArea = nullptr;
    bool _isMerged = false, _isFinishReviewed = false;
    MSAISpeakingSharePopup* _MSAISpeakingSharePopup = nullptr;
    float _sharePopupScale = 0.8;
    ProgressTimer* _progcess = nullptr;
    std::function<void()> _callback = nullptr;
};

CREATE_READER(AISpeakingReviewScreen);

#endif
