#pragma once
#ifndef StoryParentEditProfile_Landscape_hpp
#define StoryParentEditProfile_Landscape_hpp
#include "cocostudiomacro2.h"
#include "APProfile.h"
#include <unordered_set>
class StoryParentEditProfile_Landscape : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(StoryParentEditProfile_Landscape);
	static StoryParentEditProfile_Landscape * createView(const ap::Profile& profile);
private:
	void didLoadFromCSB(const ap::Profile& profile);
	void onEnter() override;
    void onChangeDisplayLanguage(int lang_id);
	void onLoadListAvatar();
	void updateFX(float dt);

	void onSelectBoy(cocos2d::Ref * sender);
	void onSelectGirl(cocos2d::Ref * sender);
	void onSelectAge(cocos2d::Ref * sender);
	void onSave(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref * sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callBackName) override;
	bool flag;
	ap::Profile _profile;
	int _age;
	cocos2d::ui::ListView* _listview;
	void hideError(cocos2d::Node * sender);
	void showError(cocos2d::Node * sender, std::string message,bool update_pos = false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	std::unordered_set<cocos2d::EventKeyboard::KeyCode> _pressedKeys;
#endif
	virtual void registerKeyboardEvent();

	virtual void scheduleEnter(std::function<void()> _callback);

	virtual void scheduleWindowsTab(std::function<void()> _callback);
};

CREATE_READER(StoryParentEditProfile_Landscape);

#endif // !StoryParentEditProfile_Landscape_hpp
