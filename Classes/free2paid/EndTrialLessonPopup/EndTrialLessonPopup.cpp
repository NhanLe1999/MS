#include "EndTrialLessonPopup.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "StoryInfoCover.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "MJHelper.h"
#include "MsSubscriptionLandscape.h"
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "MJDefinitions.h"
USING_NS_CC;
INITIALIZE_READER(EndTrialLessonPopup);

#define MS_END_TRIAL_LESSON_POPUP_NAME "EndTrialLessonPopup"

std::once_flag ms_f2p_popup_end_lesson_trial_reader;

EndTrialLessonPopup* EndTrialLessonPopup::createView()
{
    std::call_once(ms_f2p_popup_end_lesson_trial_reader, []
        {
            REGISTER_CSB_READER(EndTrialLessonPopup);
        });

    auto view = static_cast<EndTrialLessonPopup*>(CSLoader::createNode("csb/free2paid/EndTrialLessonPopup.csb"));

    if (view)
    {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void EndTrialLessonPopup::didLoadFromCSB()
{
    LANGUAGE_MANAGER->changeDisplayLanguage(this);

    auto myScreenContentSize = Director::getInstance()->getVisibleSize();
    auto myScreenOrigin = Director::getInstance()->getVisibleOrigin();

    this->ignoreContentAdaptWithSize(false);
    this->setContentSize(myScreenContentSize);

    if (auto myBackground = utils::findChild<Layout*>(this, "background")) {
        myBackground->setContentSize(myScreenContentSize);
        myBackground->setPosition(myScreenOrigin + myScreenContentSize / 2);
        
        float maxScaleSize = 1.3;
        if (auto popupBackground = utils::findChild<ImageView*>(this, "popup_bg"))
        {
            mj::helper::autoScaleRootLayout(maxScaleSize, this, "popup_bg");
            popupBackground->setPosition(myScreenOrigin + myScreenContentSize / 2);
        }
    }
}

void EndTrialLessonPopup::show(Node* scene, int z_order)
{
    CRASH_AUTO_LOG;
    if (!scene)
    {
        if (auto alert = utils::findChild(DIRECTOR->getRunningScene(), MS_END_TRIAL_LESSON_POPUP_NAME))
            return;
        DIRECTOR->getRunningScene()->addChild(this, z_order);
        this->setName(MS_END_TRIAL_LESSON_POPUP_NAME);
    }
    else
    {
        if (auto alert = utils::findChild(scene, MS_END_TRIAL_LESSON_POPUP_NAME))
            return;
        scene->addChild(this, z_order);
        this->setName(MS_END_TRIAL_LESSON_POPUP_NAME);
    }

}

void EndTrialLessonPopup::onClickLeftButton(cocos2d::Ref* sender)
{
    //Add eventracking 
    registerEventTrackingClickForButton("unlock");
    auto newPayment = MSNewTabSubscribe::createTab(monkey::iap::FROM::FREETOWPAID);
    newPayment->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    newPayment->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
    if ((ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4) == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY) {
        newPayment->setSourceForPayment("ob_finish_demo_to_register_14_lessons");
    }
    else {
        newPayment->setSourceForPayment("ob_finish_demo_to_register_7_lessons");
    }
    newPayment->setCallbackOnBack([=]() {
        this->setVisible(true);
        });
    newPayment->setCallbackOnClose([=]() {
        this->setVisible(true);
        });
    Director::getInstance()->getRunningScene()->addChild(newPayment, INT_MAX - 1);

    this->setVisible(false);
}

void EndTrialLessonPopup::onClickRightButton(cocos2d::Ref* sender)
{
    //Add eventracking 
    registerEventTrackingClickForButton("register");

    auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::REGISTER,StartLoginFrom::Default ,StartRegisterFrom::EndTrialLessonPopup);

    Director::getInstance()->getRunningScene()->addChild(loginView, INT_MAX - 1);

    this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback EndTrialLessonPopup::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(EndTrialLessonPopup, onClickLeftButton),
        CLICK_MAP(EndTrialLessonPopup, onClickRightButton),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void EndTrialLessonPopup::registerEventTrackingClickForButton(std::string type)
{
    CleverTapManager::GetInstance()->pushEvent(
        (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4) == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? 
        "ob_finish_demo_to_register_14_lessons" : "ob_finish_demo_to_register_7_lessons", {
           {"click_type",cocos2d::Value(type)},
        });
}
