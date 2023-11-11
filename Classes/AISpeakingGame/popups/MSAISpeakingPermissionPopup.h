///////////////////////////////////////////////////////////////////////
//  MSAISpeakingPermissionPopup_hpp
//  Implementation of the Class MSAISpeakingPermissionPopup_hpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#ifndef MSAISpeakingPermissionPopup_hpp
#define MSAISpeakingPermissionPopup_hpp

#include "cocostudiomacro2.h"
#include"MSCollectionAISpeakingLessonItemDelegate.h"
#include "StoryLanguageManager.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
namespace ms {namespace delegate {
    class AISpeakingPermissionPopupDelegate {
    public:
        virtual void onAccept(cocos2d::Ref* sender) {};
        virtual void onClose(cocos2d::Ref* sender) {};
        virtual void onChangeView() {};
    };
}}

class MSAISpeakingPermissionPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MSAISpeakingPermissionPopup);
    static MSAISpeakingPermissionPopup* createItem(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate);
    void onResize(cocos2d::Size size); 
    static MSAISpeakingPermissionPopup* show(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate = nullptr, Node* scene = nullptr, int z_order = INT_MAX - 1);
    virtual void onClose(cocos2d::Ref* sender);
    virtual void onAccept(cocos2d::Ref* sender) ;
    virtual void onChangeView();

private:
    void didLoadFromCSB(ms::delegate::AISpeakingPermissionPopupDelegate* aISpeakingPermissionPopupDelegate);
    void onEnter() override;
    void onExit() override;
    void initialView();
    CC_SYNTHESIZE(ms::delegate::AISpeakingPermissionPopupDelegate*, _delegate, Delegate);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
    Button* _btn_close = nullptr, * _btn_accept = nullptr;
    Text* _text_title = nullptr, * _text_description = nullptr, * _text_accept = nullptr;
    Layout * _background_overlay = nullptr, * _blue_background = nullptr, *_white_image = nullptr;
};

CREATE_READER( MSAISpeakingPermissionPopup);

#endif /* MSAISpeakingPermissionPopup_hpp */
