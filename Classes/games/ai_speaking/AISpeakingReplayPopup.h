#pragma once
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "SpineWrapper.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

class AISpeakingReplayPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:

    CREATE_FUNC(AISpeakingReplayPopup);

    static AISpeakingReplayPopup* createView();
	
    void didLoadFromCSB();

private:
    
    void onTryAgain(Ref* sender);

    void onLetTalk(Ref* sender);

    void onReplay(Ref* sender);

    void onClose(Ref* sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	//cocos2d::Node* _introLayer = nullptr;
	//cocos2d::Node* _gameLayer = nullptr;
	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
	//SpineWrapper* _recordButton;
	//SpineWrapper* _stopRecordButton;

	//int                 _currentTurn = 0;

	//MSRecord* _record = nullptr;
 //   int _idAudioQuestion = -1;
 //   float _timeCount = 0.0f;
 //   
 //   cocos2d::Node* _videoIntroArea = nullptr;
};

CREATE_READER(AISpeakingReplayPopup);

#endif