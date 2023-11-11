#pragma once

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"
#include <unordered_set>
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "HelperManager.h"
#include "MJHelper.h"
#include "APProfileDelegate.h"
#include "APUpdateProfileAPI.h"
#include "MSHome.h"
#include "BirthdayPicker.h"
#include "APPurchaseManager.h"
#include "MSFreeContentPage.h"
#include "MSIntroLoad.h"
#include "F2PManager.h"
#include "APLoadUpdateAPI.h"
#include "APProfilesListAPI.h"
class ChooseAvatarScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(ChooseAvatarScene);

    static ChooseAvatarScene* createView();//1.

private:
    void didLoadFromCSB();//2.

    void returnPreviousScreen(cocos2d::Ref* sender);
    void gotoMsHome();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.
    void onCallUpdateProfileAPI(ap::Profile profile);
    void onNext(cocos2d::Ref* sender);
    void onEnter() override;
    void updateFX(float dt);
    void CallAPIProfileList();
    void APIProfileListCallBack(bool isSucess, std::string msg);
private:
    cocos2d::ui::ListView* _listview;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::unordered_set<cocos2d::EventKeyboard::KeyCode> _pressedKeys;
#endif
    bool flag;
    std::string _avatar = "";
    std::string _acountName = "";
    std::vector<int> _birthOfDate;
};

CREATE_READER(ChooseAvatarScene);

