#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "PopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class AskPermissionPopup : public ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {

public:
	enum PERMISSION_KIND {
		RECORD = 1,
		SHOW_NOTI = 2 
	};
    
    static void showNotificationPopup(std::function<void(bool success)> callback,cocos2d::Node* i_parent = nullptr);
    
	static void showPopup(PERMISSION_KIND what_permission = RECORD,std::function<void()> nativePopupOk = nullptr, std::function<void()> nativePopupCancel = nullptr, cocos2d::Node* i_parent = nullptr,bool isShow = false/**/,std::function<void()> cocosOk = nullptr, std::function<void()> callbackGameSpeaking = nullptr);

    CREATE_FUNC(AskPermissionPopup);

    static AskPermissionPopup* createPopup(PERMISSION_KIND type);

	static void loadCancelButton(std::function<void()> cancelCallback);
	

private:
   
	void didLoadFromCSB();

    void onEnter() override;

	void onOkButtonClicked(cocos2d::Ref * sender);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	PERMISSION_KIND _permissionKind;
	std::function<void()> _onPermissionGranted; //callback for native popup
	std::function<void()> _onPermissionDenied;  //callback for native popup
	std::function<void()> _callbackGameSpeaking = nullptr;
    
    std::function<void()> _onOkCallback; //callback for shownoti

	static AskPermissionPopup* instance;
	bool isisExitPath();


};

CREATE_READER(AskPermissionPopup);

