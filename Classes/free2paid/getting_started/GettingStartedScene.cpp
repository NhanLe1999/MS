/*
*Author: Dat Hoang
*Created: 06/15/2022
*/

#include "GettingStartedScene.h"
#include "MSFirebasePlugin.h"


USING_NS_CC;
INITIALIZE_READER(GettingStartedScene);

#define F2P_GettingStartedScene_NAME "GettingStartedScene"

std::once_flag ms_f2p_getting_started_scene_reader;//4.

Scene* GettingStartedScene::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = GettingStartedScene::createView();
        view->setName(F2P_GettingStartedScene_NAME);
        scene->addChild(view, INT_MAX-2);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

GettingStartedScene* GettingStartedScene::createView()
{
    std::call_once(ms_f2p_getting_started_scene_reader, []
        {
            REGISTER_CSB_READER(GettingStartedScene);
        });

    auto csb_name = mj::helper::getCSBName("csb/free2paid/gettingStarted/gettingStartedScene.csb");//5

    auto view = reinterpret_cast<GettingStartedScene*>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (view)
    {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void GettingStartedScene::didLoadFromCSB()
{
    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0

    _timeStartScreen = int(utils::getTimeInMilliseconds() / 1000.0);
    
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([=]() {

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        CheatMenu::show(this);
#endif

    })));

    //adjust size option box
    auto winSize = Director::getInstance()->getWinSize();
    float ratio = (float)1366 / 768;
    if ((float)winSize.width / winSize.height > ratio)
    {
        float standardWidth = ratio * 768 * 0.3875;
        float delta = 0;
        if (auto panelHadAccount = utils::findChild(this, "panel.had.account"))
        {
            delta = standardWidth - panelHadAccount->getContentSize().width;
            panelHadAccount->setContentSize(Size(standardWidth, panelHadAccount->getContentSize().height));
            if (auto img = panelHadAccount->getChildByName<ImageView*>("img.bg.had.account"))
            {
                img->setContentSize(Size(img->getContentSize().width + delta, img->getContentSize().height));
            }
            auto nodes = panelHadAccount->getChildren();
            for (auto node: nodes)
            {
                node->setPositionX(panelHadAccount->getContentSize().width/2);
            }
        }
        if (auto panelHaveNoAccount = utils::findChild(this, "panel.have.no.account"))
        {
            panelHaveNoAccount->setContentSize(Size(standardWidth, panelHaveNoAccount->getContentSize().height));
            if (auto img = panelHaveNoAccount->getChildByName<ImageView*>("img.bg.had.account"))
            {
                img->setContentSize(Size(img->getContentSize().width + delta, img->getContentSize().height));
            }
            auto nodes = panelHaveNoAccount->getChildren();
            for (auto node: nodes)
            {
                node->setPositionX(panelHaveNoAccount->getContentSize().width/2);
            }
        }
    }
    
    
}

cocos2d::ui::Widget::ccWidgetClickCallback GettingStartedScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(GettingStartedScene, onBackBtnClicked),
        CLICK_MAP(GettingStartedScene, onSignInBtnClicked),
        CLICK_MAP(GettingStartedScene, onStartNowBtnClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void GettingStartedScene::onSignInBtnClicked(cocos2d::Ref* sender)
{
    CCLOG("Sign in");
    MJDEFAULT->setStringForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE, "login");
    int timeEnd = int(utils::getTimeInMilliseconds() / 1000.0)- _timeStartScreen;
    MJDEFAULT->setIntegerForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE, timeEnd);

    std::vector<std::pair<std::string, cocos2d::Value>> properties = {
    {"click_type", cocos2d::Value("login")},
    {"time_on_screen", cocos2d::Value(timeEnd)},
    };
    CleverTapManager::GetInstance()->pushEvent("ob_choose_account_type", properties);
    APLoginViewPage(APLoginView::SCREEN_TYPE::LOGIN);
}

void GettingStartedScene::onStartNowBtnClicked(cocos2d::Ref* sender)
{
    CCLOG("Start now");
    MJDEFAULT->setStringForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE, "start_now");
    int timeEnd = int(utils::getTimeInMilliseconds() / 1000.0) - _timeStartScreen;
    MJDEFAULT->setIntegerForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE, timeEnd);

    std::vector<std::pair<std::string, cocos2d::Value>> properties = {
    {"click_type", cocos2d::Value("start_now")},
    {"time_on_screen", cocos2d::Value(timeEnd)},
    };
    CleverTapManager::GetInstance()->pushEvent("ob_choose_account_type", properties);
    F2P_MANAGER->setKeyGotoF2PGetStarted(true);
    CallAPILogin();
}

void GettingStartedScene::onBackBtnClicked(cocos2d::Ref* sender)
{
    CCLOG("Back");
    F2P_MANAGER->setKeyGotoF2PGetStarted(false);
    auto scene = MSLanguageSelection::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
    
}
void GettingStartedScene::CallAPILogin()
{
    mj::helper::showWaitView();
    this->runAction(Sequence::create(DelayTime::create(1.0f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                std::function<void(std::string)> callback = [=] (std::string classification) {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        ap::api::Login::callV2(api::Login::LoginType::SKIP,false, false, "0", CC_CALLBACK_3(GettingStartedScene::APILoginCallBack, this), classification);
                    });
                };
                
                if(CONFIG_MANAGER->isTurnOnABTesting())
                {
                    ms::remoteConfig::Firebase::getInstance().fetDataRemoteConfigABTesting(callback);
                }else{
                    if(callback)
                    {
                        callback("");
                    }
                }
                
            });
            }),
        NULL));

}

void GettingStartedScene::APILoginCallBack(int error_code, bool isSucess, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        if (isSucess) {
            MJDEFAULT->deleteValueForKey(key_f2p_account_paid_info_for_merge);
            MJDEFAULT->setBoolForKey("is_f2p_login_called", true);
            MJDEFAULT->setStringForKey("key_old_account_token", ap::Database::getInstance().getAccessToken());
            CallAPILoadUpdate();
        }
        else
        {
            if(error_code == error_code_for_merged_success)
            {
                CCLOG("REGISTER");
                APLoginViewPage(APLoginView::SCREEN_TYPE::REGISTER);
            }
            else
            {
                mj::helper::deleteWaitView();
                showErrorAlert(msg);
                CCLOG("%s\n", msg.c_str());
            }

        }
     });

}

void GettingStartedScene::CallAPILoadUpdate()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        ap::api::LoadUpdate::call(CC_CALLBACK_2(GettingStartedScene::APILoadUpdateCallBack, this)); 
        });

}

void GettingStartedScene::APILoadUpdateCallBack(bool isSucess, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        if (isSucess) {
            CallAPIProfileList();
        }
        else
        {
            /*luu thong tin user da dang nhap*/
            if(!F2P_MANAGER->isMegredAccount() && F2P_MANAGER->isSkipUser())
                F2P_MANAGER->f2PSaveUserInfo(true);
            /*---------------------*/
            if (F2P_MANAGER->isNeedGotoCheat()) // kiem tra xem user co phai lan dau tao user skip khong, neu da tung tao thi se vao luong chong cheat, neu la lan dau tao thi se vao luong f2p
                // key KeyGotoF2PSplash chỉ set true khi lan dau vao vao page Splash sau khi dang nhap, key nay = false, nghĩa là user  chưa vào f2p bao giờ. Lúc này có 2 trường hợp.
                // TH1: User vào lần đầu: khi đó key isFirstLoginSkip = true và key KeyGotoF2PSplash = false
                // TH2:user đã vào -> gỡ app ra -> cài lại: Khi đó isFirstLoginSkip = false và KeyGotoF2PSplash = false
            {
                // day la luong cheat
                CCLOG("REGISTER");
                APLoginViewPage(APLoginView::SCREEN_TYPE::REGISTER);
            }
            else
            {
                
                gotoSplashPage();
            }
        }
    });
}

void GettingStartedScene::CallAPIProfileList()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        
        int id = ap::Database::getInstance().getUserInfo()._id;
        
        std::string userId = std::to_string(id);
        ap::api::ProfilesList::callV2(userId, CC_CALLBACK_2(GettingStartedScene::APIProfileListCallBack, this));

        });

}

void GettingStartedScene::APIProfileListCallBack(bool isSucess, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

        if (isSucess)
        {
        }
        else
        {
        }
        CCLOG("APIProfileListCallBack");
        
        /*luu thong tin user da dang nhap*/
        if(!F2P_MANAGER->isMegredAccount()&& F2P_MANAGER->isSkipUser())
            F2P_MANAGER->f2PSaveUserInfo(true);
        /*---------------------*/
        
        if (F2P_MANAGER->isNeedGotoCheat()) // kiem tra xem user co phai lan dau tao user skip khong, neu da tung tao thi se vao luong chong cheat, neu la lan dau tao thi se vao luong f2p
        {// luong chong cheat
            CCLOG("REGISTER");
            APLoginViewPage(APLoginView::SCREEN_TYPE::REGISTER);
        }
        else
        {
            gotoSplashPage();
        }
        });
}
void GettingStartedScene::showErrorAlert(std::string msg){
    bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, is_vn] {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
        if(msg == ""){
            StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
        }else{
            //TODO: show notification
            StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
        }
    });
}
void GettingStartedScene::showAlertHavePaidAccount()
{
    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.f2p.lifetime.purchased.account");
    auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
    auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
    auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");
    auto alert = ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, this, true, false);
    alert->setSizeTextMessage(Size(550, 115));
    alert->MoveTextMessage(textMessage, false, 10);
    alert->setOnOkCallback([=] {
        onSignInBtnClicked(nullptr);
        });
}

void GettingStartedScene::onExit()
{
    this->stopAllActions();
    Layout::onExit();
}

void GettingStartedScene::gotoSplashPage()
{
    CCLOG("gotoSplashPage");
    
    this->runAction(Sequence::create(DelayTime::create(0.1f),
        CallFunc::create([=] {
            CCLOG("gotoSplashPage after runaction");
            auto scene = SplashScene::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            }),
        NULL));

}

void GettingStartedScene::APLoginViewPage(APLoginView::SCREEN_TYPE SCREEN_TYPE)
{
    mj::helper::deleteWaitView();
    APLoginView* loginScreen = nullptr; //dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView"));
    // if (loginScreen)
    //     loginScreen->removeFromParent();
    if(SCREEN_TYPE == APLoginView::SCREEN_TYPE::REGISTER)
        loginScreen = APLoginView::createView(APLoginView::SCREEN_TYPE::REGISTER, ms::Authen::StartLoginFrom::Default, ms::Authen::StartRegisterFrom::F2PGettingStartedScene);
    else
    {
        loginScreen = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::F2PGettingStartedScene, ms::Authen::StartRegisterFrom::Default);
    }

    APPopupBase::sAnimationMove(loginScreen, APPopupBase::AnimationDirection::RIGHT);
    Director::getInstance()->getRunningScene()->addChild(loginScreen, INT_MAX - 1);
    F2P_MANAGER->setKeyGotoF2PGetStarted(true);
}
