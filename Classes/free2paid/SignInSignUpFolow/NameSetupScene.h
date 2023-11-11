#pragma once

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"
#include <unordered_set>
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "MSIntro.h"
#include "CleverTapManager.h"
#include "HelperManager.h"
#include "APProfileAgeF2p.h"
#include "MJHelper.h"
#include "BirthdayPicker.h"
#include "MJHelper.h"
#define MS_NAME_SETUP_SCENE_NAME "NameSetupScene"

class NameSetupScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(NameSetupScene);

    static Scene* createScene();

    static NameSetupScene* createView();
private:
    void didLoadFromCSB();

    void onNext(cocos2d::Ref* sender);
    void returnPreviousScreen(cocos2d::Ref* sender);

    virtual void registerKeyboardEvent();
    virtual void scheduleEnter(std::function<void()> _callback);
    virtual void scheduleWindowsTab(std::function<void()> _callback);
    void doCheckNext();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
private:
    bool flag;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::unordered_set<cocos2d::EventKeyboard::KeyCode> _pressedKeys;
#endif
    cocos2d::Vec2 _textFieldSizeBase = cocos2d::Vec2(0,0);
    std::string _childName = "";
    cocos2d::Vec2 _touchArea;
};

CREATE_READER(NameSetupScene);

