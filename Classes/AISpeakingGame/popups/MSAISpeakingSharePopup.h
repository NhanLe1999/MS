///////////////////////////////////////////////////////////////////////
//  MSAISpeakingSharePopup_hpp
//  Implementation of the Class MSAISpeakingSharePopup_hpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#ifndef MSAISpeakingSharePopup_hpp
#define MSAISpeakingSharePopup_hpp

#include "cocostudiomacro2.h"
#include"MSCollectionAISpeakingLessonItemDelegate.h"
#include "StoryLanguageManager.h"
#include "MKDefinitions.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
namespace ms {namespace delegate {
    class AISpeakingSharePopupDelegate {
    public:
        virtual void onShareButtonClicked(cocos2d::Ref* sender) {};
        virtual void onCloseButtonClick(cocos2d::Ref* sender) {};
        virtual void onFinishButtonClicked(cocos2d::Ref* sender) {};
    };
}}

class MSAISpeakingSharePopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MSAISpeakingSharePopup);
    static MSAISpeakingSharePopup* createItem();
    void onResize(cocos2d::Size size);
//    static MSAISpeakingSharePopup* showPopupFinishLesson(ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate, Node* scene = nullptr, int z_order = INT_MAX - 1);
//    static MSAISpeakingSharePopup* showPopupShare(ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate, Node* scene = nullptr, int z_order = INT_MAX - 1);
    static MSAISpeakingSharePopup* showPopup(ms::delegate::AISpeakingSharePopupDelegate* aISpeakingSharePopupDelegate, Node* scene = nullptr, int z_order = INT_MAX - 1);

    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _closeCallback, CloseCallback, nullptr);
    
    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _shareCallback, ShareCallback, nullptr);
    
    MK_SYNTHESIZE_WITHDEFAULT_VALUE(std::function<void()>, _finishCallback, FinishCallback, nullptr);
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    void onCloseButtonClick(cocos2d::Ref* sender);
    void onShareButtonClicked(cocos2d::Ref* sender);
    void onFinishButtonClicked(cocos2d::Ref* sender);
    void initialView();
    CC_SYNTHESIZE(ms::delegate::AISpeakingSharePopupDelegate*, _delegate, Delegate);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
    Button* _share_button = nullptr, * _finish_button = nullptr, * _close_button = nullptr;
    Text* _lb_share_video_title = nullptr, * _text_share = nullptr, * _text_finish = nullptr;
    Layout * _background_overlay = nullptr, * _background_blue = nullptr, *_background_white = nullptr;
};

CREATE_READER_CLASS(MSAISpeakingSharePopupReader, MSAISpeakingSharePopup);

#endif /* MSAISpeakingSharePopup_hpp */
