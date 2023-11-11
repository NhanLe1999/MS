//
//  MJPayGuide.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 3/29/17.
//
//

#include "HSLib.h"
#include "ui/UIHelper.h"

#include "MJPayGuide.h"
#include "StoryDataModel.h"
#include "NativeBridge.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "StoryFreeManager.h"
#include "MJPay.h"
#include "MJHelper.h"

INITIALIZE_READER_INSTANCE(MJPayGuideReader);

USING_NS_CC;
using namespace std;
using namespace ui;

MJPayGuide * MJPayGuide::createPage() {
    MJPayGuide *p;
    p = static_cast<MJPayGuide *>(CSLoader::createNodeWithVisibleSize("csb/pay/MJPayGuide.csb"));
    if (p) {
        p->initPage();
    }
    return p;
}

void MJPayGuide::initPage()
{
    didLoadFromCSB();
    
}

#pragma mark - onEnter
void MJPayGuide::onEnter()
{
    Layout::onEnter();
}

void MJPayGuide::onEnterTransitionDidFinish()
{
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void MJPayGuide::onExitTransitionDidStart()
{
    Layout::onExitTransitionDidStart();
}

void MJPayGuide::didLoadFromCSB()
{
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    
    std::string bg_name;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        bg_name = "bg_ngang";
    }else bg_name = "bg_doc";
    auto background = this->getChildByName(bg_name);
    background->setVisible(true);

    std::vector<int> pid = {3, 1, 2};

    for (int i = 1; i <= 3; i++) {
        background->getChildByName<Text *>(StringUtils::format("lb_price%d", i))->setString(StringUtils::format("%sđ", mj::helper::prettyInteger((int)mj::Pay::getInstance().getPackages()[pid[i - 1]].getActualPrice(mj::Pay::getInstance().getDiscountPercent())).c_str()));
    }
}

void MJPayGuide::onClose(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback MJPayGuide::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJPayGuide, onClose),
    };
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (callback_name == "onClose") {
        
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onClose(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    }
#endif
    
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}

