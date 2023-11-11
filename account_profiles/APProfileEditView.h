#pragma once
#ifndef APProfileEditView_hpp
#define APProfileEditView_hpp
#include "cocostudiomacro2.h"
#include "APProfile.h"

class APProfileEditView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(APProfileEditView);
	static APProfileEditView * createView(const ap::Profile& profile);
private:
	void didLoadFromCSB(const ap::Profile& profile);
	void onEnter() override;
	void onExit() override;

    void changeDisplayLanguage();
	void onLoadListAvatar();
	void updateFX(float dt);

	void onSelectBoy(cocos2d::Ref * sender);
	void onSelectGirl(cocos2d::Ref * sender);
	void onSelectAge(cocos2d::Ref * sender);
	void onSave(cocos2d::Ref * sender);
	void onClickPhoto(cocos2d::Ref * sender);
	//void onDelete(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref * sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callBackName) override;

	ap::Profile _profile;
	int _age;
	cocos2d::ui::ListView* _listview;
};

CREATE_READER(APProfileEditView);

#endif // !APProfileEditView_hpp
