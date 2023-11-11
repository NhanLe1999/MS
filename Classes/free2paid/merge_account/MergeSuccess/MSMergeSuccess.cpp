///////////////////////////////////////////////////////////
//  AISpeakingTopic.cpp
//  Implementation of the Class AISpeakingTopic.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MSMergeSuccess.h"
#include "APChooseProfileView.h"
#include "MSFreeContentPage.h"
#include "MMFirebaseDatabase.h"
#include "NameSetupScene.h"


INITIALIZE_READER(MSMergeSuccess);

/*
 * 1. New, Popular
 * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
 */

std::once_flag _MSMergeSuccess_reader;

MSMergeSuccess::MSMergeSuccess(){
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::MSMergeSuccess");
}

MSMergeSuccess::~MSMergeSuccess(){
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::MSMergeSuccess");

}
Scene* MSMergeSuccess::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        scene->setName("MSMergeSuccesscene");

        auto view = MSMergeSuccess::createTab();
        view->setName("MSMergeSuccess");
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}
MSMergeSuccess* MSMergeSuccess::createTab()
{
    CRASH_AUTO_LOG;
    CCLOG("MsGroupFreeItems::createTab");

	std::call_once(_MSMergeSuccess_reader, []
		{
			REGISTER_CSB_READER(MSMergeSuccess);
		});

	auto csbName = mj::helper::getCSBName("csb/free2paid/MSMergeSuccess.csb");

	auto node = reinterpret_cast<MSMergeSuccess*>(CSLoader::createNode(csbName));

	if (node)
	{
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}



void MSMergeSuccess::didLoadFromCSB()
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::didLoadFromCSB");
    mj::helper::showWaitView();
    if (auto rootLayout = (utils::findChild<Layout*>(this, "rootLayout")))
    {
        rootLayout->setOpacity(0);
    }
    //F2P_MANAGER->setKeyGotoF2PSplash(true); // key nay danh dau user da tung vao man SplashScene, use da merge xong, thi se khong vao lai Splash nua, maf vao luong cheat
    auto myScreenContentSize = Director::getInstance()->getVisibleSize();
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setPosition(myScreenContentSize / 2);
    initViewContent();
    F2P_MANAGER->setMergingAccount(false);
    F2P_MANAGER->setMegredAccount(true);
}

void MSMergeSuccess::initViewContent()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::initViewContent");

    LANGUAGE_MANAGER->changeDisplayLanguage(this);
    if (auto list = (utils::findChild<ListView*>(this, "profile_items_list")))
    {
        setProfileItemsListView(list);
    }
    if (auto button = (utils::findChild<Button*>(this, "back_button")))
    {
        button->setVisible(false);
        button->setTouchEnabled(true);
    }
    F2P_MANAGER->setSkipUser(false);
    APPURCHASE_MANAGER->setSkipUser(false);
    F2P_MANAGER->clearDataFreeToPaid();
}
void MSMergeSuccess::mouseScroll(std::string listviewName, ListView* listView, float percent, float timeInSec, bool attenuated)
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::mouseScroll");

    ListView* list;
    if(listviewName!="")
        list = utils::findChild<ListView*>(this, listviewName);
    else
    {
        list = listView;
    }
    if (!list)
        return;

        int _percent = list->getScrolledPercentHorizontal();
        _percent += percent;
        list->scrollToPercentHorizontal(_percent, timeInSec, attenuated);

}

void MSMergeSuccess::onExit()
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::onExit");

    Layout::onExit();
}

void MSMergeSuccess::onEnter()
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::onEnter");
    Layout::onEnter();
    _profileList.clear();
    CallAPILoadUpdate();
}

void MSMergeSuccess::onSelectedProfile(int profileId)
{
    setSelectedProfileId(profileId);
    CallAPIUpdateProfile();
}

void MSMergeSuccess::onDeleteSelectedProfile(int profileId)
{
}


void MSMergeSuccess::initListView()
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::initListView");
    if (!getProfileItemsListView())
        return;
    getProfileItemsListView()->setDirection(ScrollView::Direction::HORIZONTAL);
    getProfileItemsListView()->setItemsMargin(20.0f);
    getProfileItemsListView()->setScrollBarEnabled(false);
    getProfileItemsListView()->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(MSMergeSuccess::selectedEventScrollView, this));
    getProfileItemsListView()->setBounceEnabled(true);
    getProfileItemsListView()->setVisible(true);
    getProfileItemsListView()->setTouchEnabled(false);
    getProfileItemsListView()->setClippingEnabled(false);
    if (getProfileInfoList().size() >= 5) {
        getProfileItemsListView()->setTouchEnabled(true);
    }
    getProfileItemsListView()->setMagneticType(ListView::MagneticType::CENTER);
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        getProfileItemsListView()->removeAllChildren();
        Size itemsSize;
        for (int i = 0; i < getProfileInfoList().size(); i++)
        {
            MsProfileItem* msProfileItem = initProfileItem(getProfileInfoList().at(i));
            itemsSize = msProfileItem->getContentSize();
            msProfileItem->setVisible(true);
            msProfileItem->setTouchEnabled(true);
            msProfileItem->setZOrder(INT_MAX - 1);
            if (!msProfileItem)
                continue;
            getProfileItemsListView()->pushBackCustomItem(msProfileItem);
            
        }
        if (itemsSize.width > 0)
        {
            getProfileItemsListView()->setContentSize(Size((itemsSize.width + 20.0f) * getProfileInfoList().size() -20.0f, itemsSize.height*1.1));
        }
        if (getProfileItemsListView()->getContentSize().width >= getProfileItemsListView()->getParent()->getContentSize().width)
        {
            getProfileItemsListView()->setContentSize(Size(getProfileItemsListView()->getParent()->getContentSize().width, getProfileItemsListView()->getContentSize().height));
            getProfileItemsListView()->setClippingEnabled(true);
        }
        getProfileItemsListView()->requestDoLayout();
        getProfileItemsListView()->jumpToPercentHorizontal(50.0f);
        this->runAction(Sequence::create(DelayTime::create(0.2f),
            CallFunc::create([=] {
                }),
            NULL));
        });
}

MsProfileItem* MSMergeSuccess::initProfileItem(ap::Profile profileInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("MsGroupFreeItems::initProfileItem");
    MsProfileItem* msProfileItem = MsProfileItem::createView(profileInfo, this);

    return msProfileItem;
}

void MSMergeSuccess::APICallBack(bool callAPISuccess)
{
}

void MSMergeSuccess::updateProfileData(bool callAPISuccess)
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::updateProfileData");
}
void MSMergeSuccess::CallAPILoadUpdate()
{ 
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ap::api::LoadUpdate::call(CC_CALLBACK_2(MSMergeSuccess::APILoadUpdateCallBack, this)); });
            }),
        NULL));

}

void MSMergeSuccess::APILoadUpdateCallBack(bool isSucess, std::string msg)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
        mj::helper::deleteWaitView();
        if (isSucess) {
            CallAPIProfileList();
        }
        else
        {
            // show thong bao loi
        }
            
    }),NULL));

}

void MSMergeSuccess::CallAPIProfileList()
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            mj::helper::showWaitView();
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                std::string userId = std::to_string(ap::Database::getInstance().getUserInfo()._id);
                ap::api::ProfilesList::callV2(userId, CC_CALLBACK_2(MSMergeSuccess::APIProfileListCallBack, this));

                });
            }),
        NULL));

}

void MSMergeSuccess::APIProfileListCallBack(bool isSucess, std::string msg)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
        mj::helper::deleteWaitView();
        if (isSucess)
        {
            getAllProfilefromDB();
            
            if(getProfileInfoList().size()>0)
            {
                if (auto rootLayout = (utils::findChild<Layout*>(this, "rootLayout")))
                {
                    rootLayout->setOpacity(255);
                }
                initListView();
            }
            else
            {
                ShowCreateProfile();
            }
        }
        else
        {
            // show thong bao loi
        }
            
    }),NULL));
    
}
void MSMergeSuccess::ShowCreateProfile()
{

    auto choose_profile = APChooseProfileView::createView();
    choose_profile->setName("choose_profile_view");
    choose_profile->hideCreateNewProfilesButton();
    Director::getInstance()->getRunningScene()->addChild(choose_profile, INT_MAX - 1);

}
void MSMergeSuccess::CallAPIUpdateProfile()
{
    mj::helper::showWaitView();
    ap::Profile profile = ap::Database::getInstance().getProfileById(getSelectedProfileId());
    PROFILE_MANAGER.setCurrentProfileId(profile.profile_id);
    mm::RealtimeDatabase::getInstance().saveData();
    auto userInfo = ap::Database::getInstance().getUserInfo();
    if(MJDEFAULT->getBoolForKey("key_merge_profile_clevertap", false))
    {
        CleverTapManager::GetInstance()->pushProfileByID(profile.profile_id, CleverTapManager::PUSH_TYPE::UPDATE, [=]() {});
    }
    else
    {
        if(MJDEFAULT->getIntegerForKey("key_profile_skip_id_merge_clevertap") == profile.profile_id)
        {
            CleverTapManager::GetInstance()->pushProfileByID(profile.profile_id, CleverTapManager::PUSH_TYPE::UPDATE, [=]() {});
        }
        else
        {
            CleverTapManager::GetInstance()->pushProfileByID(profile.profile_id, CleverTapManager::PUSH_TYPE::LOGIN, [=]() {});
        }
    }
    
    
    mm::RealtimeDatabase::getInstance().loadProfileData(profile.profile_id, nullptr);
    
    ap::api::UpdateProfile::callV2(profile, [this, profile](bool is_success, std::string msg)
        {
            this->runAction(Sequence::create(DelayTime::create(0.2f),
                CallFunc::create([=] {
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        if (is_success)
                        {
                            CRASH_AUTO_LOG;
                            ap::api::LoadUpdate::call([=](bool isSuccess, std::string msg) {
                                this->runAction(Sequence::create(DelayTime::create(0.2f),
                                    CallFunc::create([=] {
                                        mj::helper::deleteWaitView();
                                        if (profile.isMissingInfo())
                                        {
                                            auto view = NameSetupScene::createView();
                                            Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                                        }
                                        else
                                        {
                                            if (APPURCHASE_MANAGER->isFreeUser() && !APPURCHASE_MANAGER->isSkipUser())
                                            {
                                                MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::TEN_ITEM_FREE);
                                                auto scene = MSFreeContentPage::createScene();
                                                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                                            }
                                            else
                                            {
                                                gotoMsHome();
                                            }
                                        }
                                        }),
                                    NULL));
                                });
                        }
                        else
                        {
                            this->runAction(Sequence::create(DelayTime::create(0.2f),
                                CallFunc::create([=] {
                                    mj::helper::deleteWaitView();
                                    }),
                                NULL));
                            bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
                            //TODO: show notification and do something
                            if (msg == "") {
                                StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                            }
                            else {
                                //TODO: show notification
                                StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
                            }
                        }
                        }
                    );

                    }
                ),
                NULL));


        });

   
}

void MSMergeSuccess::onBackButtonClicked(cocos2d::Ref* sender)
{

    CRASH_AUTO_LOG;
    gotoMsHome();
}


void MSMergeSuccess::getAllProfilefromDB()
{
    setProfileInfoList(ap::Database::getInstance().getProfiles());
}


void MSMergeSuccess::selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::selectedEventScrollView");

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
}


void MSMergeSuccess::gotoMsHome()
{
    CRASH_AUTO_LOG;
    CCLOG("MSMergeSuccess::gotoMsHome");
    auto home = MSHome::createScene();
                        if (!CONFIG_MANAGER->isAppActive())
                        {
                            if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false))
                            {
                                if (MSInstallPromotion::shouldShowPopup())
                                {
                                    auto popup = MSInstallPromotion::createView(true);
                                    home->addChild(popup);
                                }
                                else
                                {
                                    if (!MJDEFAULT->getBoolForKey("is_user_skip", false) && !MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
                                    {
                                        if (APPURCHASE_MANAGER->getTypeUser() == IAP::user::TYPE_USERS::USER_EXPRIED)
                                        {
                                            //auto popup = StoryWelcomePopUp::createPage();
                                            //popup->setName("StoryWelcomePopUp");
                                            //home->addChild(popup);
                                        }
                                    }
                                }
                            }
                        }
                        if (MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
                        {
                            MJDEFAULT->setBoolForKey("key_parent_in_choose_view_button_clicked", false);
                            if (auto homeView = (MSHome*)home->getChildByName(MSHOME_NAME))
                            {
                                auto waitView = APWaitView::createView(std::string(""), false, true);
                                waitView->setName("waiting_view");
                                home->addChild(waitView, INT_MAX - 1);
                                homeView->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]() {
                                    MJDEFAULT->setBoolForKey(key_show_parent_lock, false);
                                    waitView->removeFromParent();
                                    homeView->clearLayouts();
                                    homeView->goToParent();
                                    })));
                            }
                        }
                        // Update phần setting ngôn ngữ ở đây
                        StoryLanguageManager::getInstance()->changeMapLangDisplay(MJDEFAULT->getIntegerForKey(key_lang_display, 1));
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
}


cocos2d::ui::Widget::ccWidgetClickCallback MSMergeSuccess::onLocateClickCallback(const std::string& callback_name)
{
    CRASH_AUTO_LOG;
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSMergeSuccess, onBackButtonClicked),
        
        
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

