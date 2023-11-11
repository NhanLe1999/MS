/**
* @author DH
* @created 06/29/2022
*/

#include "Merge1Scene.h"
#include "F2PCommon.h"
#include "APPurchaseManager.h"
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
#include "Merge2Scene.h"
#include "MS_F2P_API_MergeAccount.h"
#include "APUpdateProfileAPI.h"
#include "APProfilesListAPI.h"
#include "APChooseProfileView.h"
#include "MSMergeSuccess.h"
#include "ShowNewAlert.h"
#include "NativeBridge.h"
#include "MJ_PageLoadResource.h"

USING_NS_CC;
INITIALIZE_READER(Merge1Scene);

#define MERGE_1_SCENE_NAME "Merge1Scene"
#define ITEM_SCALED_SIZE 1.2f

std::once_flag ms_f2p_merge_1_scene_reader;

Scene* Merge1Scene::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = Merge1Scene::createView();
        view->setName(MERGE_1_SCENE_NAME);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

Merge1Scene* Merge1Scene::createView()
{
    std::call_once(ms_f2p_merge_1_scene_reader, []
        {
            REGISTER_CSB_READER(Merge1Scene);
        });

    auto csb_name = mj::helper::getCSBName("csb/free2paid/merge/Merge1.csb");//5

    auto view = reinterpret_cast<Merge1Scene*>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (view)
    {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}
MsProfileItem* Merge1Scene::initProfileItem(ListView* listview, ap::Profile profileInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("Merge1Scene::initProfileItem");
    MsProfileItem* msProfileItem = MsProfileItem::createView(profileInfo, this, false);
    auto m_scaledSize = listview->getContentSize().height / msProfileItem->getContentSize().height;
    msProfileItem->scalingSize(m_scaledSize);
    return msProfileItem;
}
void Merge1Scene::initListViewProfile(ListView * listview, std::vector<ap::Profile> profile)
{
    CRASH_AUTO_LOG;


    CCLOG("Merge1Scene::initListView");

    // add listview _profilePaidAccountListView
    if (!listview)
        return;
    auto parentSize = listview->getParent()->getContentSize();
    listview->setDirection(ScrollView::Direction::HORIZONTAL);
    listview->setItemsMargin(10.0f);
    listview->setScrollBarEnabled(false);
    listview->setTouchEnabled(false);
    listview->setClippingEnabled(true);
    if (profile.size() >= 4) {
        //listview->setScrollBarEnabled(true);
        //listview->setScrollBarWidth(0);
        listview->setTouchEnabled(true);
    }
    listview->setBounceEnabled(true);
    listview->setVisible(true);
    listview->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Merge1Scene::selectedEventScrollView, this));
    listview->setGravity(ListView::Gravity::LEFT);
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        listview->removeAllItems();
        float listviewWidth = 0.f;
        for (int i = 0; i < profile.size(); i++)
        {
            MsProfileItem* msProfileItem = initProfileItem(listview, profile.at(i));
            msProfileItem->setVisible(true);
            msProfileItem->setTouchEnabled(true);
            msProfileItem->setZOrder(INT_MAX - 1);
            if (!msProfileItem)
                continue;
            listview->pushBackCustomItem(msProfileItem);
            listviewWidth += (msProfileItem->getContentSize().width + listview->getItemsMargin());

        }
        listview->setInnerContainerSize(Size(listviewWidth + 100, listview->getContentSize().height));
        listview->requestDoLayout();
        });
}

void Merge1Scene::showProductPackages()
{
    if (auto text = cocos2d::utils::findChild<Text*>(this, "lb.merge1.scene.txt.paid.pack"))
    {
        //(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-ExtraBold.ttf");
        std::string productId = "";
        if (_f2POldAccountInfoForMerge._inAppPackageModel.size() > 0)
        {
            monkey::iap::PRODUCT_PACKAGES  productPackageMax = monkey::iap::PRODUCT_PACKAGES::FREE;
            int packageMaxIndex = 0;
            // can tim goi package cos gia tri gia han lon nhat
            for(int i = 0;i<  _f2POldAccountInfoForMerge._inAppPackageModel.size(); i++)
            {
                auto inAppPackageModel = _f2POldAccountInfoForMerge._inAppPackageModel.at(i);
                monkey::iap::PRODUCT_PACKAGES productPackage =  inAppPackageModel.getProductPackageType();
                if((int)productPackage> (int)productPackageMax)
                {
                    productPackageMax = productPackage;
                    packageMaxIndex = i;
                }
            }
            productId = _f2POldAccountInfoForMerge._inAppPackageModel.at(packageMaxIndex)._productId;
        }
        std::string packageName = getPackageName(productId);
        text->setString(packageName);
    }


    if (auto text = cocos2d::utils::findChild<Text*>(this, "lb.merge1.scene.txt.free.pack"))
    {
        std::string productId = "";
        if (_f2PNewAccountInfoForMerge._inAppPackageModel.size() > 0)
        {
            monkey::iap::PRODUCT_PACKAGES  productPackageMax = monkey::iap::PRODUCT_PACKAGES::FREE;
            int packageMaxIndex = 0;
            // can tim goi package cos gia tri gia han lon nhat
            for (int i = 0; i < _f2PNewAccountInfoForMerge._inAppPackageModel.size(); i++)
            {
                auto inAppPackageModel = _f2PNewAccountInfoForMerge._inAppPackageModel.at(i);
                monkey::iap::PRODUCT_PACKAGES productPackage = inAppPackageModel.getProductPackageType();
                if ((int)productPackage > (int)productPackageMax)
                {
                    productPackageMax = productPackage;
                    packageMaxIndex = i;
                }
            }
            productId = _f2PNewAccountInfoForMerge._inAppPackageModel.at(packageMaxIndex)._productId;
        }
        std::string packageName = getPackageName(productId);
        text->setString(packageName);
    }

}

std::string Merge1Scene::getPackageName(std::string productId)
{
    std::string packageName = "";
    std::string packagePre =  LANGUAGE_MANAGER->getDisplayTextByKey("product.name.com.earlystart.stories.package");
    std::string packTime = LANGUAGE_MANAGER->getDisplayTextByKey("notifree.title");
    monkey::iap::PRODUCT_PACKAGES  productPackage = monkey::iap::PRODUCT_PACKAGES::FREE;
    if(!productId.empty())    
        productPackage = IAP_MANAGER->getProductPackage(productId);
    switch (productPackage)
    {
        case monkey::iap::FREE:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("notifree.title");
            break; 
        }
        case monkey::iap::ONE_MONTH:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("product.name.com.earlystart.stories.1month");
            break;
        }
        case monkey::iap::SIX_MONTH:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("product.name.com.earlystart.stories.6month");
            break;
        }
        case monkey::iap::ONE_YEAR:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("product.name.com.earlystart.stories.1year");
            break;
        }
        case monkey::iap::LIFT_TIME:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("sub.item.lifetime");
            break;
        }
        default:
        {
            packTime = LANGUAGE_MANAGER->getDisplayTextByKey("notifree.title");
            break;
        }
    }
    if ((LANGUAGE_MANAGER->getDisplayLangId() != LANGUAGE_EN_CODE))
    {
       packageName = StringUtils::format("%s %s", packagePre.c_str(), packTime.c_str());
    }
    else
    {
        packageName = StringUtils::format("%s %s", packTime.c_str(), packagePre.c_str());
    }
    return packageName;

}

void Merge1Scene::onExit()
{
    deleteWaitViewCustom();

    Layout::onExit();
}

void Merge1Scene::showAlertMergeLiftTimeAccountFalse()
{
    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.f2p.lifetime.purchased.account");
    auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
    auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
    auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");
    auto alert = ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, this, true, false);
    alert->setSizeTextMessage(Size(550, 115));
    alert->MoveTextMessage(textMessage, false, 10);
}

void Merge1Scene::loadProfileInfos()
{


    _profilePaidAccountListView = utils::findChild<ListView*>(this, "merge1.scene.list.view_paid_account");
    _profileFreeAccountListView = utils::findChild<ListView*>(this, "merge1.scene.list.view_free_account");



    if (_profilePaidAccountListView == nullptr || _profileFreeAccountListView == nullptr)
    {
        return;
    }
    int j = 1;

    if (!_f2PNewAccountInfoForMerge._profileList.empty())
    {
        initListViewProfile(_profileFreeAccountListView, _f2PNewAccountInfoForMerge._profileList);
    }

    if (!_f2POldAccountInfoForMerge._profileList.empty())
    {
        initListViewProfile(_profilePaidAccountListView, _f2POldAccountInfoForMerge._profileList);
    }
    //load profile skip
    auto m_profileSkip = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_f2p_skip_profile_id));
    if (auto m_imgProfileSkip = cocos2d::utils::findChild<ImageView*>(this, "merge1.scene.img.profile.skip"))
    {
        m_imgProfileSkip->loadTexture(m_profileSkip.avatar);
    }
    if (auto m_txtNameProfileSkip = cocos2d::utils::findChild<Text*>(this, "merge1.scene.txt.user.name.profile.skip"))
    {
        m_txtNameProfileSkip->setString(m_profileSkip.name);
    }

    //load user in4
    auto m_userIn4 = _f2POldAccountInfoForMerge._userInfo;
    if (auto m_txtFullName = cocos2d::utils::findChild<Text*>(this, "merge1.scene.txt.full.name"))
    {
        m_txtFullName->setString(m_userIn4._name);
    }
    if (auto m_txtPhone = cocos2d::utils::findChild<Text*>(this, "merge1.scene.txt.phone"))
    {
        m_txtPhone->setString(m_userIn4._phone);
    }
    if (auto m_txtEmail = cocos2d::utils::findChild<Text*>(this, "merge1.scene.txt.email"))
    {
        m_txtEmail->setString(m_userIn4._mail);
    }
    if (auto m_txtDeviceID = cocos2d::utils::findChild<Text*>(this, "merge1.scene.txt.device.id"))
    {
        std::string id = MJ_PLATFORM_CONFIG.getDeviceId();
        m_txtDeviceID->setString(m_txtDeviceID->getString() +" "+ id);
    }
    showProductPackages();


}

void Merge1Scene::onSelectedProfile(int profileId)
{
}

void Merge1Scene::onDeleteSelectedProfile(int profileId)
{
}

Merge1Scene::~Merge1Scene()
{
}

void Merge1Scene::didLoadFromCSB()
{
    MJDEFAULT->setBoolForKey(key_f2p_need_merge_profiles, false);
    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0
    loadAuthenInfoForCallAPI();
    F2P_MANAGER->setMergingAccount(true);
    //fakeData();

}

void Merge1Scene::loadAuthenInfoForCallAPI()
{
    std::vector<std::string> accountFreeInfo;
    std::vector<std::string> accountPaidInfo;
    std::vector<std::string> accountInfoTemp =F2P_MANAGER->f2PGetUserInfo(true);
    bool isUserSkip = false;
    // kiem tra xem dau la tai khoan skip
    if (F2P_MANAGER->f2PGetUserInfo(true).size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE || F2P_MANAGER->f2PGetUserInfo(false).size() < SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE){
        return;
    }
    isUserSkip = accountInfoTemp.at(3) != "0" ? true : false;
    if(isUserSkip)
    {
        accountFreeInfo = F2P_MANAGER->f2PGetUserInfo(true);
        accountPaidInfo = F2P_MANAGER->f2PGetUserInfo(false);
    }
    else
    {
        accountFreeInfo = F2P_MANAGER->f2PGetUserInfo(false);
        accountPaidInfo = F2P_MANAGER->f2PGetUserInfo(true);
    }
    
    if (accountFreeInfo.size() >= SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE && accountPaidInfo.size() >= SIZE_VECTOR_SAVE_USER_INFO_FOR_MERGE)
    {
        _f2PNewAccountInfoForMerge = ms::f2p::F2PAccountInfoForMerge(accountFreeInfo.at(0), accountFreeInfo.at(1));
        _f2POldAccountInfoForMerge = ms::f2p::F2PAccountInfoForMerge(accountPaidInfo.at(0), accountPaidInfo.at(1));
    }
    else
    {
        return;
    }

    handlerForFreeAccount(CC_CALLBACK_1(Merge1Scene::handlerForFreeAccountCallback, this));
}

void Merge1Scene::handlerForFreeAccount(const std::function<void(bool)> callback)
{
    showWaitViewCustom();
    ap::Database::getInstance().dropDatabase();
    APPURCHASE_MANAGER->reset();
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            _handlerForFreeAccountCallback = callback;
            setUserIdAPICalling(_f2PNewAccountInfoForMerge._userId);
            
            Database::getInstance().saveIdentity(std::to_string(_f2PNewAccountInfoForMerge._userId), _f2PNewAccountInfoForMerge._token);
            CallAPILoadUpdate();
            }),
        NULL));
}

void Merge1Scene::handlerForFreeAccountCallback(bool success)
{
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {           
            vector<ap::Profile> profile = ap::Database::getInstance().getProfiles();
            _f2PNewAccountInfoForMerge._profileList = profile;
            _f2PNewAccountInfoForMerge._inAppPackageModel = APPURCHASE_MANAGER->getPurchasedPackages();
            _f2PNewAccountInfoForMerge._userInfo = ap::Database::getInstance().getUserInfo();
            _f2PNewAccountInfoForMerge._maxProfile = MJDEFAULT->getIntegerForKey(key_max_account_profile, 3);
            F2P_MANAGER->f2PSaveUserInfo(true);
            handlerForPaidAccount(CC_CALLBACK_1(Merge1Scene::handlerForPaidAccountCallback, this));
            }),
        NULL));


}

void Merge1Scene::handlerForPaidAccount(const std::function<void(bool)> callback)
{
    ap::Database::getInstance().dropDatabase();
    _handlerForPaidAccountCallback = callback;
    setUserIdAPICalling(_f2PNewAccountInfoForMerge._userId);
    Database::getInstance().saveIdentity(std::to_string(_f2POldAccountInfoForMerge._userId), _f2POldAccountInfoForMerge._token);
    CallAPILoadUpdate();
}

void Merge1Scene::handlerForPaidAccountCallback(bool success)
{
    deleteWaitViewCustom();
    vector<ap::Profile> profile = ap::Database::getInstance().getProfiles();
    _f2POldAccountInfoForMerge._profileList = profile;
    _f2POldAccountInfoForMerge._inAppPackageModel = APPURCHASE_MANAGER->getPurchasedPackages();
    _f2POldAccountInfoForMerge._userInfo = ap::Database::getInstance().getUserInfo(); 
    _f2POldAccountInfoForMerge._maxProfile = MJDEFAULT->getIntegerForKey(key_max_account_profile, 3);
    F2P_MANAGER->f2PSaveUserInfo(false);
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            loadProfileInfos();
            }),
        NULL));


}

void Merge1Scene::CallAPIProfileList()
{
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                std::string userId = std::to_string(ap::Database::getInstance().getUserInfo()._id);
                ap::api::ProfilesList::callV2(userId, CC_CALLBACK_2(Merge1Scene::APIProfileListCallBack, this));
                
            });
            }),
        NULL));
  
}

void Merge1Scene::APIProfileListCallBack(bool isSucess, std::string msg)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
        if (isSucess)
        {
            if (ap::Database::getInstance().getUserInfo()._id == _f2PNewAccountInfoForMerge._userId)
            {
                if(_handlerForFreeAccountCallback)
                    _handlerForFreeAccountCallback(isSucess);
            }
            else
            {
                if (_handlerForPaidAccountCallback)
                 _handlerForPaidAccountCallback(isSucess);
            }
        }
        else
        {
            // show thong bao loi
        }
            
    }),NULL));



}
void Merge1Scene::onSignOut(cocos2d::Ref* sender)
{
    if (!NativeBridge::networkAvaiable())
    {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        alert->setDelegate(this);

        return;
    }

    ms::EventLogger::getInstance().logEvent("click_signout", {});
    auto waiting_layout = Layout::create();
    waiting_layout->setContentSize(Director::getInstance()->getVisibleSize());
    waiting_layout->setBackGroundColor(Color3B::BLACK);
    waiting_layout->setBackGroundColorType(BackGroundColorType::SOLID);
    waiting_layout->setBackGroundColorOpacity(200);
    waiting_layout->setTouchEnabled(true);
    waiting_layout->setName("waiting_logout");
    Director::getInstance()->getRunningScene()->addChild(waiting_layout);

    auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
    waiting_layout->addChild(indicator);
    indicator->setPosition(waiting_layout->getContentSize() / 2);
    indicator->setVisible(true);
    indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));

    ap::Auth::signOut(this);
    //reload data

    auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom* event)
        {

            ms::EventLogger::getInstance().logEvent("event_signout_done", {}); cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_successfully");
            MJDEFAULT->setBoolForKey(key_onboarding_license, false);
            MJDEFAULT->setBoolForKey(key_logged_in, false);
            //        waiting_layout->removeFromParent();
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                auto scene = MSIntro::createScene();
                Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
                });
        });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom* event) {
        ms::EventLogger::getInstance().logEvent("event_signout_fail", {});
        cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
        waiting_layout->removeFromParent();

        });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);

    //delete unused data key.
    GettingUserInfoManager::GetInstance()->deleteData();
    RatingManager::GetInstance()->deleteData();

    mj::PageLoadResource::getInstance().deleteCacheAllStory();
    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
    FileUtils::getInstance()->purgeCachedEntries();
}

void Merge1Scene::CallAPIMergeProfile()
{
    if (!F2P_MANAGER->isCanMergeProfile())
    {
        showAlertMergeLiftTimeAccountFalse();
        return;
    }
    mj::helper::showWaitView();
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ap::api::MergeAccountAPI::callMerge(_f2POldAccountInfoForMerge._token,_f2PNewAccountInfoForMerge._token,  CC_CALLBACK_3(Merge1Scene::APIMergeProfileCallBack, this));


                });
            }),
        NULL));

}

void Merge1Scene::APIMergeProfileCallBack(bool isSucess, std::string msg, int errorCode)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            mj::helper::deleteWaitView();
            std::vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges;
            f2PAccountInfoForMerges.push_back(_f2PNewAccountInfoForMerge);
            f2PAccountInfoForMerges.push_back(_f2POldAccountInfoForMerge);
            if (isSucess) {
                CCLOG("Merge request success received respond");
                MJDEFAULT->setBoolForKey(key_f2p_merge_status_response, true);

                F2P_MANAGER->saveMergedUserInfo(f2PAccountInfoForMerges);

                this->runAction(Sequence::create(DelayTime::create(0.5f),
                    CallFunc::create([=] {
                        auto scene = MSMergeSuccess::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                        }),
                    NULL));
            }
            else
            {
                if (errorCode == 402) // loi do merge tai khoan tron doi voi tai khoan da mua, xu ly show popup
                {
                    mergeLifeTimeError = true;
                    showAlertMergeLiftTimeAccountFalse();
                }
                else
                {
                    F2P_MANAGER->saveMergedUserInfo(f2PAccountInfoForMerges);
                    MJDEFAULT->setBoolForKey(key_f2p_merge_status_response, false);
                    this->runAction(Sequence::create(DelayTime::create(0.5f),
                        CallFunc::create([=] {
                            auto view = Merge2Scene::createView(f2PAccountInfoForMerges);
                            APPopupBase::sAnimationMove(view, APPopupBase::AnimationDirection::LEFT);
                            this->addChild(view, INT_MAX - 1);
                            }),
                        NULL));
                }
                CCLOG("%s\n", msg.c_str());

            }
            }),
        NULL));



}

void Merge1Scene::onGoToMerge2Page()
{


}

void Merge1Scene::CallAPILoadUpdate()
{
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ap::api::LoadUpdate::call(CC_CALLBACK_2(Merge1Scene::APILoadUpdateCallBack, this)); });
            }),
        NULL));

}

void Merge1Scene::APILoadUpdateCallBack(bool isSucess, std::string msg)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
        if(isSucess){
                CallAPIProfileList();
        }
        else
        {
            // show thong bao loi
        }
            
    }),NULL));

}

cocos2d::ui::Widget::ccWidgetClickCallback Merge1Scene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(Merge1Scene, onBtnLinkClicked),
        CLICK_MAP(Merge1Scene, onBtnSkipClicked)
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void Merge1Scene::onBtnSkipClicked(cocos2d::Ref* sender)
{
    CCLOG("Merge 1 Scen skip");
    F2P_MANAGER->setMegreSkipAccount(true);
    if(!mergeLifeTimeError) // neu merge api tra ve loi 402- merge tai khoan tron doi, thi sex cho phep user merge lai khi dang nhap tai khoan khac. neu khong, thi se khong cho vao luong merge nua
    {
        F2P_MANAGER->setSkipUser(false);
        APPURCHASE_MANAGER->setSkipUser(false);
    }
    else
    {
        F2P_MANAGER->setSkipUser(true);
        APPURCHASE_MANAGER->setSkipUser(true);
    }
    MJDEFAULT->deleteValueForKey(key_f2p_account_paid_info_for_merge);
    Database::getInstance().saveIdentity(std::to_string(_f2PNewAccountInfoForMerge._userId), _f2PNewAccountInfoForMerge._token);
    Database::getInstance().saveUserInfo(std::to_string(_f2PNewAccountInfoForMerge._userId), _f2PNewAccountInfoForMerge._userInfo._name, _f2PNewAccountInfoForMerge._userInfo._mail
        , _f2PNewAccountInfoForMerge._userInfo._phone, _f2PNewAccountInfoForMerge._userInfo._address, _f2PNewAccountInfoForMerge._userInfo._userName);
    this->scheduleOnce([=](float dt)
       {

           onSignOut(nullptr);

       }, 0.25f, "purge_cached");
}

void Merge1Scene::onBtnLinkClicked(cocos2d::Ref* sender)
{
    /*kiem tra co the merge hay khong*/

    CallAPIMergeProfile();
}

void Merge1Scene::showWaitViewCustom()
{
    if (auto waitingView = utils::findChild(Director::getInstance()->getRunningScene(), "waiting_view_merge1"))
    {
        waitingView->setZOrder(INT_MAX);
        waitingView->setVisible(true);
        return;
    }

    auto waitView = APWaitView::createView(std::string(""), false, true);
    waitView->setName("waiting_view_merge1");
    this->addChild(waitView, INT_MAX - 1);

    this->schedule([=](float) {
        int countTime = MJDEFAULT->getIntegerForKey("countTimeCheckWaitingView", 0);
        countTime++;
        MJDEFAULT->setIntegerForKey("countTimeCheckWaitingView", countTime);
        if (countTime >= 60) {
            this->unschedule("checkWaitingViewCustom");
            deleteWaitViewCustom();
        }
        }, 1, CC_REPEAT_FOREVER, 0, "checkWaitingViewCustom");
}
void Merge1Scene::deleteWaitViewCustom()
{

    if (auto waitingView = utils::findChild(this, "waiting_view_merge1"))
    {
        waitingView->removeFromParent();
        if (this->isScheduled("checkWaitingViewCustom"))
        {
            MJDEFAULT->setIntegerForKey("countTimeCheckWaitingView", 0);
            this->unschedule("checkWaitingViewCustom");
        }
    }
}
void Merge1Scene::selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    CRASH_AUTO_LOG;
    CCLOG("Merge1Scene::selectedEventScrollView");
    ListView* listView = static_cast<ListView*>(pSender);
    switch (type) {

    case ui::ScrollView::EventType::SCROLLING_ENDED:
    {
        int percent = listView->getScrolledPercentHorizontal();

        if (percent <= 0)
        {
            listView->jumpToLeft();
        }

        if (percent >= 100)
        {
            listView->jumpToRight();
        }
        break;
    }

    default:
        break;
    }

    auto ctn = listView->getInnerContainer();
}
