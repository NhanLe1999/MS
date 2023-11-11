#include "SplashScene.h"
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
#include "MsSubscriptionLandscape.h"
#include "ProfileMiniScene.h"
#include "GettingStartedScene.h"
#define desplayTime 3.0f
USING_NS_CC;
INITIALIZE_READER(SplashScene);

#define SPLASH_SCENE_NAME "SplashScene"

std::once_flag ms_f2_splash_scene;//4.

Scene* SplashScene::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = SplashScene::createView();
        view->setName(SPLASH_SCENE_NAME);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

SplashScene* SplashScene::createView()
{
    std::call_once(ms_f2_splash_scene, []
        {
            REGISTER_CSB_READER(SplashScene);
        });

    auto csb_name = mj::helper::getCSBName("csb/free2paid/splash/SplashScene.csb");//5

    auto view = reinterpret_cast<SplashScene*>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (view)
    {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void SplashScene::didLoadFromCSB()
{
    this->setName("SplashScene");
    _timeOnScreen = int(utils::getTimeInMilliseconds() / 1000.0);
    _timeOnPage1 = _timeOnScreen;

    this->scheduleOnce([=] (float dt) {
        pushEventTrackingChooseAcountType();
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        auto userId = ap::Database::getInstance().getUserInfo()._id;
        std::string uId = std::to_string(userId);
        auto text = cocos2d::ui::Text::create("User id : " + uId, "fonts/Montserrat-Bold.ttf", 30);
        text->setTextColor(Color4B::RED);
        text->setAnchorPoint(Vec2(0.5, 0.5));
        auto size = Director::getInstance()->getWinSize();
        text->setPosition(cocos2d::Vec2(size.width * 0.5, size.height * 0.95));
        Director::getInstance()->getRunningScene()->addChild(text, INT_MAX);
#endif
        
    }, 2.0f, "delay_time_push_event_ob_choose_account_type");
    
    if(F2P_MANAGER->isFirstLoginSkip()){ // lan dau user login vao f2p, bat buoc vao man SplashScene
        F2P_MANAGER->setKeyGotoF2PSplash(true); // key nay danh dau user da tung vao man SplashScene
    }
    
    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0

    _pageView = cocos2d::utils::findChild<cocos2d::ui::PageView*>(this, "mainPageView");
    _pageView->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_0(SplashScene::onPageTurn, this));
    _navigation = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "navigation");
    scheduleChangePage();
    // nếu la free2paid thì xoa hết thông tin các profile cũ đi để bát đâu luồng này
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            ap::Database::getInstance().dropDatabaseProfiles();
            ap::ProfileManager::getInstance().setCurrentProfileId(0);
            }),
        NULL));
    onChangeDiscountPriceByCountry();
}

void SplashScene::onPageTurn()
{
    _currentPageIndex = _pageView->getCurPageIndex();
    
    if (_currentPageIndex == 0)
    {
        _navigation->loadTexture("free2paid/splash_scene/splash1/group_1257.png");

        EndPage2();
        EndPage3();
        _timeOnPage1 = int(utils::getTimeInMilliseconds() / 1000.0);
    }

    else if (_currentPageIndex == 1)
    {
        _navigation->loadTexture("free2paid/splash_scene/splash2/group_1257.png");
        _timeOnPage2 = int(utils::getTimeInMilliseconds() / 1000.0);
        EndPage1();
        EndPage3();
    }

    else
    {
        _navigation->loadTexture("free2paid/splash_scene/splash3/group_1257.png");
        EndPage1();
        EndPage2();
        _timeOnPage3 = int(utils::getTimeInMilliseconds() / 1000.0);
    }
    this->unschedule(_scheduleChangePageName);
    scheduleChangePage();
}

void SplashScene::onBtnContinueClicked(cocos2d::Ref* sender)
{
    CCLOG("continue");
    _currentPageIndex =  _pageView->getCurPageIndex();
    if (_currentPageIndex == 0 || _currentPageIndex == 1)
    {
        changePage();
    }
    else
    {
        EndPage3();
        pushEventTracking();
        auto newPayment = MSNewTabSubscribe::createTab(monkey::iap::FROM::FREETOWPAID);
        newPayment->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
        newPayment->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
        Director::getInstance()->getRunningScene()->addChild(newPayment, INT_MAX - 1);
        newPayment->setSourceForPayment("ob_intro_screen");
        newPayment->setCallbackOnClose([=] () {
            auto scene = ProfileMiniScene::createView();
            Director::getInstance()->getRunningScene()->addChild(scene, INT_MAX - 1);
        });
        newPayment->setCallbackOnBack([=]() {
            auto scene = GettingStartedScene::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            });


    }
}

void SplashScene::onBtnPage0Clicked(cocos2d::Ref* sender)
{
    CCLOG("Page0");
    EndPage2();
    EndPage3();
    _timeOnPage1 = int(utils::getTimeInMilliseconds() / 1000.0);
    _pageView->setCurPageIndex(0);
    _navigation->loadTexture("free2paid/splash_scene/splash1/group_1257.png");
    this->unschedule(_scheduleChangePageName);
    scheduleChangePage();
}

void SplashScene::onBtnPage1Clicked(cocos2d::Ref* sender)
{
    CCLOG("Page1");
    EndPage1();
    EndPage3();
    _timeOnPage2 = int(utils::getTimeInMilliseconds() / 1000.0);
    _isViewMs02 = true;
    _pageView->setCurPageIndex(1);
    onChangeDiscountPriceByCountry();
    _navigation->loadTexture("free2paid/splash_scene/splash2/group_1257.png");
    this->unschedule(_scheduleChangePageName);
    scheduleChangePage();
}

void SplashScene::onBtnPage2Clicked(cocos2d::Ref* sender)
{
    CCLOG("Page2");
    EndPage1();
    EndPage2();
    _timeOnPage3 = int(utils::getTimeInMilliseconds() / 1000.0);
    _isViewMs03 = true;
    _pageView->setCurPageIndex(2);
    _navigation->loadTexture("free2paid/splash_scene/splash3/group_1257.png");
}

void SplashScene::onChangeDiscountPriceByCountry()
{
    auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");
    if (auto text = cocos2d::utils::findChild<cocos2d::ui::Text*>(this, "lb.splash.scene.txt.discount.page1"))
    {
        std::string discountValue = k_countryCode == "VN" ? " 50%" : " 40%";
        std::string str = LANGUAGE_MANAGER->getDisplayTextByKey("splash.scene.txt.discount.page1") + discountValue;
        //std::string dissountPrice = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("splash.scene.txt.discount.page1").c_str(), discountValue);
        text->setString(str);

    }

    if (auto text = cocos2d::utils::findChild<cocos2d::ui::Text*>(this, "lb.splash.scene.txt.package.applied.page1"))
    {
        
        std::string str = LANGUAGE_MANAGER->getDisplayTextByKey((k_countryCode == "VN" || k_countryCode == "TH") ? "splash.scene.txt.package.applied.page1" : "splash.scene.txt.package.applied.page2");
        text->setString(str);
    }

    if (auto image = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "Image_11"))
    {
        std::string imageurl = k_countryCode == "VN" ? "free2paid/splash_scene/splash2/k_2.png" : "free2paid/splash_scene/splash2/k_1.png";

        image->loadTexture(imageurl);
    }

}

void SplashScene::changePage()
{
    if (_currentPageIndex == 0)
    {
        _isViewMs02 = true;
        _timeOnPage2 = int(utils::getTimeInMilliseconds() / 1000.0);
        EndPage1();
    }

    if (_currentPageIndex == 1)
    {
        _isViewMs03 = true;
        _timeOnPage3 = int(utils::getTimeInMilliseconds() / 1000.0);
        EndPage2();
    }

    _pageView->setCurPageIndex(_currentPageIndex + 1);

    _navigation->loadTexture("free2paid/splash_scene/splash" + std::to_string(_currentPageIndex + 2) + "/group_1257.png");
    _currentPageIndex++;
}

void SplashScene::scheduleChangePage()
{
    if (_currentPageIndex < 2)
    {
        this->schedule([=](float dt) {
            if (_currentPageIndex < 2)
            {
                changePage();
            }
        }, desplayTime, _scheduleChangePageName);
    }
}

void SplashScene::onExit()
{
    cocos2d::ui::Layout::onExit();
    this->unschedule(_scheduleChangePageName);
}

void SplashScene::pushEventTracking ()
{
    if (isPushEvent)
    {
        return;
    }
    isPushEvent = true;
    _timeOnScreen = int(utils::getTimeInMilliseconds() / 1000.0) - _timeOnScreen;
    CleverTapManager::GetInstance()->pushEvent("ob_intro_screen", {
            {"total_time_on_screen", cocos2d::Value(_timeOnScreen)},
            {"do_view_MS02_or_not", cocos2d::Value(_isViewMs02 ? "true" : "false")},
            {"do_view_MS03_or_not", cocos2d::Value(_isViewMs03 ? "true" : "false")},
        });

    CleverTapManager::GetInstance()->pushEvent("ob_intro_screen_view_1", {
        {"time_on_screen", cocos2d::Value(_totalTimeOnPage1)},
        {"Click_monkey_image_or_not", cocos2d::Value(_isClickImageMonkeyPage1 ? "true" : "false")},
        });

    CleverTapManager::GetInstance()->pushEvent("ob_intro_screen_view_2", {
        {"time_on_screen", cocos2d::Value(_totalTimeOnPage2)},
        {"Click_baby_image_or_not", cocos2d::Value(_isClickImageMonkeyPage2 ? "true" : "false")},
        {"Click_discount40_image_or_not", cocos2d::Value(_isClickImageDiscount ? "true" : "false")},
        });

    CleverTapManager::GetInstance()->pushEvent("ob_intro_screen_view_3", {
        {"time_on_screen", cocos2d::Value(_totalTimeOnPage3)},
        {"Click_baby_image_or_not", cocos2d::Value(_isClickImageBabyPage3 ? "true" : "false")},
        {"Click_course_image_or_not", cocos2d::Value(_isClickImageCourse ? "true" : "false")},
        });
}

void SplashScene::pushEventTrackingChooseAcountType()
{
    auto value = MJDEFAULT->getStringForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE, "");
    if (value != "")
    {
        int timeEnd = MJDEFAULT->getIntegerForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE, 0);
        std::vector<std::pair<std::string, cocos2d::Value>> properties = {
            {"click_type", cocos2d::Value(value)},
            {"time_on_screen", cocos2d::Value(timeEnd)},
        };
        CleverTapManager::GetInstance()->pushEvent("ob_choose_account_type", properties);
        MJDEFAULT->deleteValueForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE);
        MJDEFAULT->deleteValueForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE);
    }
}

void SplashScene::OnClickMonkeyPage1(cocos2d::Ref* sender)
{
    _isClickImageMonkeyPage1 = true;
}

void SplashScene::OnClickChildrenPage2(cocos2d::Ref* sender)
{
    _isClickImageBabyPage2 = true;
}

void SplashScene::OnClickMonkeyPage2(cocos2d::Ref* sender)
{
    _isClickImageMonkeyPage2 = true;
}

void SplashScene::OnClickDiscount50(cocos2d::Ref* sender)
{
    _isClickImageDiscount = true;
}

void SplashScene::OnClickChildrenPage3(cocos2d::Ref* sender)
{
    _isClickImageBabyPage3 = true;
}

void SplashScene::OnClickCoursePage3(cocos2d::Ref* sender)
{
    _isClickImageCourse = true;
}

cocos2d::ui::Widget::ccWidgetClickCallback SplashScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(SplashScene, onBtnContinueClicked),
        CLICK_MAP(SplashScene, onBtnPage0Clicked),
        CLICK_MAP(SplashScene, onBtnPage1Clicked),
        CLICK_MAP(SplashScene, onBtnPage2Clicked),
        CLICK_MAP(SplashScene, OnClickMonkeyPage1),
        CLICK_MAP(SplashScene, OnClickChildrenPage2),
        CLICK_MAP(SplashScene, OnClickMonkeyPage2),
        CLICK_MAP(SplashScene, OnClickDiscount50),
        CLICK_MAP(SplashScene, OnClickChildrenPage3),
        CLICK_MAP(SplashScene, OnClickCoursePage3),

    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void SplashScene::EndPage1()
{
    if (_timeOnPage1 != 0)
    {
        _timeOnPage1 = int(utils::getTimeInMilliseconds() / 1000.0) - _timeOnPage1;
        _totalTimeOnPage1 = _totalTimeOnPage1 + _timeOnPage1;
        _timeOnPage1 = 0;
    }
   
}

void SplashScene::EndPage2()
{
    if (_timeOnPage2 != 0)
    {
        _timeOnPage2 = int(utils::getTimeInMilliseconds() / 1000.0) - _timeOnPage2;
        _totalTimeOnPage2 = _totalTimeOnPage2 + _timeOnPage2;
        _timeOnPage2 = 0;
    }
}

void SplashScene::EndPage3()
{
    if (_timeOnPage3 != 0)
    {
        _timeOnPage3 = int(utils::getTimeInMilliseconds() / 1000.0) - _timeOnPage3;
        _totalTimeOnPage3 = _totalTimeOnPage3 + _timeOnPage3;
        _timeOnPage3 = 0;
    }
}