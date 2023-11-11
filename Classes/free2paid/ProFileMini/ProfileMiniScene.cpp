#include "ProfileMiniScene.h"
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

USING_NS_CC;
INITIALIZE_READER(ProfileMiniScene);

#define MS_PROFILE_MINI_SCENE_NAME "ProfileMiniScene"

std::once_flag ms_f2p_profile_mini_reader;//4.

ProfileMiniScene* ProfileMiniScene::createView()
{
    std::call_once(ms_f2p_profile_mini_reader, []
        {
            REGISTER_CSB_READER(ProfileMiniScene);
        });

    auto view = static_cast<ProfileMiniScene*>(CSLoader::createNodeWithVisibleSize("csb/free2paid/ProfileMini.csb"));

    if (view)
    {
        view->didLoadFromCSB();

        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void ProfileMiniScene::didLoadFromCSB()
{
    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0

    float maxScaleSize = 1.5;
    mj::helper::autoScaleRootLayout(maxScaleSize, this, "background");

    this->schedule([=](float) {
        if (!_isTouchActive)
        {
            _countTime += _loopTime;
            if (_countTime >= 2) {
                gotoNextScreen();
            }
        }
        }, _loopTime, _countdownScheduleName);

    this->setTouchEnabled(true);
    this->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType event) {
        switch (event) {
        case ui::Widget::TouchEventType::BEGAN:
        {
            _isTouchActive = true;
            this->onTouchBegan();
            break;
        }
            
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
        {
            _isTouchActive = false;
            this->onTouchEnded();
            break;
        }
        default:
            break;
        }
        });

}

void ProfileMiniScene::gotoNextScreen(float dt)
{
    this->unschedule(_countdownScheduleName);

    auto profileAgeScreen = APProfileAgeF2p::createView();
    
    Director::getInstance()->getRunningScene()->addChild(profileAgeScreen, INT_MAX - 1);
    
    this->removeFromParent();
}

void ProfileMiniScene::onTouchEnded() {
    _countTime = 0;
    auto touch_end = this->getTouchEndPosition();
    auto touch_pos = this->getTouchBeganPosition();
    if (touch_end.y <= XHSLib->getSizeScreen().width) {
        if (touch_end.x - touch_pos.x < -250) {
            gotoNextScreen();
            return;
        }
    }
}

void ProfileMiniScene::onTouchBegan()
{
    CCLOG("Began");
}

cocos2d::ui::Widget::ccWidgetClickCallback ProfileMiniScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
       // CLICK_MAP(ProfileMiniScene, onLanguageSelected),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}
