#pragma once
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "SpineWrapper.h"
#include "AISpeakingReviewScreen.h"
#include "AISpeakingScene.h"
#include "MKDefinitions.h"

//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

class AISpeakingClosingPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:

    enum class AISpeakingClosingPopupType {
        FIRST_CALL,
        RELEARN,
        RELEARN_LAST_TURN
    };
    
    CREATE_FUNC(AISpeakingClosingPopup);
    
    
    static AISpeakingClosingPopup* createView(AISpeakingClosingPopupType type);
	
    void didLoadFromCSB(AISpeakingClosingPopupType type);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _closePopupCallback,ClosePopupCallback, nullptr);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _continueCallback,ContinueCallback, nullptr);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _hangUpCallback,HangUpCallback, nullptr);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _skipCallback,SkipCallback, nullptr);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _exitCallback, ExitCallback, nullptr);

private:
    
    void onContinueButtonClicked(Ref* sender);
    void initView();
    
    void onHangUpClicked(Ref* sender);

    void onClosePopupClicked(Ref* sender);
    
    void onExitButtonClicked(Ref* sender);

    void onSkipButtonClicked(Ref* sender);
    float getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
    
    AISpeakingClosingPopupType _type;
    
    cocos2d::Node* _rootLayout = nullptr;
    Button* _firstcall_continue_button = nullptr, * _firstcall_hang_up_button = nullptr,
        * _relearn_back_button = nullptr, * _relearn_exit_button = nullptr, * _relearn_skip_button = nullptr,
        * _relearn_last_turn_back_button = nullptr, * _relearn_last_turn_exit_button = nullptr;

    Text* _firstcall_lb_sure_to_hang_up = nullptr, * _firstcall_lb_continue = nullptr, * _firstcall_lb_hang_up = nullptr,
        *_relearn_lb_conversation_paused_title = nullptr, * _relearn_lb_exit = nullptr, * _relearn_lb_skip = nullptr, * _relearn_lb_conversation_paused_content = nullptr,
        * _relearn_last_turn_lb_conversation_paused_title = nullptr, * _relearn_last_turn_lb_exit = nullptr;
    Layout* _first_call_layout = nullptr, *_relearn_layout = nullptr, *_relearn_last_turn_layout = nullptr;
    
};

CREATE_READER(AISpeakingClosingPopup);

//#endif
