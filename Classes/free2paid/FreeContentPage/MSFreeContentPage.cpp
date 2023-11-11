///////////////////////////////////////////////////////////
//  AISpeakingTopic.cpp
//  Implementation of the Class AISpeakingTopic.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MSFreeContentPage.h"
#include "GettingStartedScene.h"

#define url_landing_VN "https://truyentranh.monkeystories.vn/rts-app?app_id=40&utm_source=App&utm_medium=c3_type&utm_campaign=form_landing&utm_content=vn_landing_purchase_screen_ms&device_id=%s&profile_id=%d&users_id=%d&user_type=%s&source=%s&clevertap_id=%s"

INITIALIZE_READER(MSFreeContentPage);

/*
 * 1. New, Popular
 * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
 */

std::once_flag _MSFreeContentPage_reader;

MSFreeContentPage::MSFreeContentPage(){
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::MSFreeContentPage");
}

MSFreeContentPage::~MSFreeContentPage(){
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::MSFreeContentPage");
    F2P_MANAGER->setIsFreeContentPage(false);
    mj::helper::purgeTextureData();
}

Scene* MSFreeContentPage::createScene(ms::f2p::FreeContentOption freeContentOption)
{
    auto scene = Scene::create();

    if (scene)
    {
        scene->setName("MSFreeContentPageScene");
        auto view = MSFreeContentPage::createTab(freeContentOption);

        view->setName("MSFreeContentPage");
        scene->addChild(view);
        auto myScreenContentSize = Director::getInstance()->getVisibleSize();
        view->setContentSize(myScreenContentSize);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}
MSFreeContentPage* MSFreeContentPage::createTab(ms::f2p::FreeContentOption freeContentOption)
{
    CRASH_AUTO_LOG;
    CCLOG("MsGroupFreeItems::createTab");

	std::call_once(_MSFreeContentPage_reader, []
		{
			REGISTER_CSB_READER(MSFreeContentPage);
		});

	auto csbName = mj::helper::getCSBName("csb/free2paid/MSFreeContentPage.csb");

	auto node = reinterpret_cast<MSFreeContentPage*>(CSLoader::createNode(csbName));

	if (node)
	{
		node->didLoadFromCSB(freeContentOption);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}


void MSFreeContentPage::initViewContent()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::initViewContent");

    auto myScreenContentSize = Director::getInstance()->getVisibleSize();
    LANGUAGE_MANAGER->changeDisplayLanguage(this);
    Layout* root = nullptr;
    if (auto rootLayout = (utils::findChild<Layout*>(this, "rootLayout")))
    {
        root = rootLayout;
        root->setContentSize(myScreenContentSize);
        root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        root->setPosition(myScreenContentSize/2);

    }

    if (auto button_group = (utils::findChild<Layout*>(this, "button_group")))
    {
        button_group->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button_group->setPositionX(myScreenContentSize.width / 2);
        if (Button* button_LearnFull = (utils::findChild<Button*>(this, "button_learn")))
        {
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            button_LearnFull->setVisible(true);
    #else
            button_LearnFull->setVisible(false);
    #endif

        }
    }
    
    if (auto textTitle = (utils::findChild<Text*>(this, "lb.MSFreeContentPage.title")))
    {
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        int age = MJDEFAULT->getIntegerForKey(key_api_age_free_trial, 0);
        textTitle->setText(textTitle->getString() + "(" + std::to_string(age) + ")");
    #endif
        textTitle->setPositionX(myScreenContentSize.width / 2);
    }

    if (auto list = (utils::findChild<ListView*>(this, "free_group_items_list")))
    {

        if (root)
        {
            list->setContentSize(Size(myScreenContentSize.width * 0.9, list->getContentSize().height));
            list->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            list->setPosition(root->getContentSize() / 2);
            
        }
        setFreeItemsListView(list);
    }

    if (auto button_back = (utils::findChild<Button*>(this, "back_button")))
    {
        button_back->setPosition(Vec2(myScreenContentSize.width * 0.02, myScreenContentSize.height * 0.95));
        button_back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    }

    if (auto layout = (utils::findChild<Layout*>(this, "layout_percent_page")))
    {
        std::vector<Layout*> radiusPageList;
        for (int i = 1; i <= 4; i++)
        {
            std::string layout_radius_name = StringUtils::format("layout_radius_%d", i);
            if (auto layout_radius = (utils::findChild<Layout*>(layout, layout_radius_name)))
            {
                radiusPageList.push_back(layout_radius);
            }
        }
        setRadiusPageList(radiusPageList);
    }

}

void MSFreeContentPage::didLoadFromCSB(ms::f2p::FreeContentOption freeContentOption)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::didLoadFromCSB");
    F2P_MANAGER->setKeyF2P_Free(true);
    auto myScreenContentSize = Director::getInstance()->getVisibleSize();
   // this->setContentSize(myScreenContentSize);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setPosition(myScreenContentSize / 2);
    if (freeContentOption == ms::f2p::FreeContentOption::DEFAULT)
    {
        ms::f2p::FreeContentOption freeContentOption1 = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4);
        if (freeContentOption1 == ms::f2p::FreeContentOption::DEFAULT) {
            freeContentOption1 = ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY;
            MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY);
        }
        setFreeContentOption(freeContentOption1);
    }
    else
    {
        setFreeContentOption(freeContentOption);
    }
    initViewContent();

    if (auto button_sigin = (utils::findChild<Button*>(this, "button_signin")))
    {
        button_sigin->setPosition(Vec2(myScreenContentSize.width * 0.93, myScreenContentSize.height * 0.9));
        button_sigin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button_sigin->setVisible(F2P_MANAGER->isSkipUser());
    }

    auto option = getFreeContentOption();

    if ( !_isStartDemoPopup )
    {
        if (option != ms::f2p::FreeContentOption::TEN_ITEM_FREE)
        {
            _sourcePayment = "unlock_discover_"
                             + std::to_string(MJDEFAULT->getIntegerForKey("key_f2p_learned_items")) + "_"
                             + (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? "14" : "7");
        }
        else {
            _sourcePayment = "unlock_discover_10_lessons";
        }
    }
    else {
        _isStartDemoPopup = false;
    }

    _fromPayment = (getFreeContentOption() == ms::f2p::FreeContentOption::TEN_ITEM_FREE) ? monkey::iap::FROM::DEFAULT : monkey::iap::FROM::FREETOWPAID;
    std::string userTypeText = APPURCHASE_MANAGER->getStringTypeUser(_fromPayment);
    _pathLanding = cocos2d::StringUtils::format(url_landing_VN, MJ_PLATFORM_CONFIG.getDeviceId().c_str(), MJDEFAULT->getIntegerForKey(key_current_profile_id), ap::Database::getInstance().getUserInfo()._id, userTypeText.c_str(), _sourcePayment.c_str(), CleverTapManager::GetInstance()->getCleverTapIdentify().c_str());
}

void MSFreeContentPage::onSignInF2P()
{
    auto loginScreen = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::FreeContentPage, ms::Authen::StartRegisterFrom::Default);
    F2P_MANAGER->setCanMergeProfileKey(true);
    APPopupBase::sAnimationMove(loginScreen, APPopupBase::AnimationDirection::LEFT);
    Director::getInstance()->getRunningScene()->addChild(loginScreen, INT_MAX - 1);
}

void MSFreeContentPage::mouseScroll(std::string listviewName, ListView* listView, float percent, float timeInSec, bool attenuated)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::mouseScroll");

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

void MSFreeContentPage::onExit()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onExit");

    Layout::onExit();
}

void MSFreeContentPage::onEnter()
{
    mj::helper::SetFpsByNumberFrames(30);
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onEnter");
    this->scheduleOnce([](float dt)
        {

            mj::helper::purgeTextureData();

        }, 1.5, "purge_cached");
    auto value = MJDEFAULT->getStringForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE, "");
    if (value != "")
    {
        int timeEnd = MJDEFAULT->getIntegerForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE, 0);
        std::vector<std::pair<std::string, cocos2d::Value>> properties = {
            {"click_type", cocos2d::Value(value)},
            {"time_on_screen", cocos2d::Value(timeEnd)},
        };
        //CleverTapManager::GetInstance()->pushEvent("ob_choose_account_type", properties);
        MJDEFAULT->deleteValueForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE);
        MJDEFAULT->deleteValueForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE);
    }
    Layout::onEnter();
   F2P_MANAGER->setIsFreeContentPage(true);
    CallAPIFreeContentPage();
}


void MSFreeContentPage::onShowNotifyPermissonPopup()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onShowNotifyPermissonPopup");

}

void MSFreeContentPage::initListView()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::initListView");

    if (!getFreeItemsListView())
        return;
    mj::helper::showWaitView();
    getFreeItemsListView()->setDirection(ScrollView::Direction::HORIZONTAL);
    getFreeItemsListView()->setItemsMargin(20.0f);    
    getFreeItemsListView()->setScrollBarOpacity(0);
    //getFreeItemsListView()->setScrollBarEnabled(false);
    getFreeItemsListView()->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(MSFreeContentPage::selectedEventScrollView, this));
    getFreeItemsListView()->setBounceEnabled(true);
    getFreeItemsListView()->setInertiaScrollEnabled(true);
    //getFreeItemsListView()->setClippingEnabled(true);
    //getFreeItemsListView()->setClippingType(ClippingType::STENCIL);

    getFreeItemsListView()->setMagneticType(ListView::MagneticType::CENTER);
    getFreeItemsListView()->removeAllChildren();
    _msGroupFreeItems.clear();
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        
        MJDEFAULT->setBoolForKey(key_f2p_save_free_items_learned_on_day, false);
        for (int i = 0; i < getAllFreeItemInfos().size(); i++)
        {
            mj::model::FreeItemInfo freeItemInfo;
            MsGroupFreeItems* msGroupFreeItems = initFreeGroupView(i);
            if (!msGroupFreeItems)
                continue;
            msGroupFreeItems->setVisible(false);
            _msGroupFreeItems.push_back(msGroupFreeItems);
            
        }
        for (MsGroupFreeItems* msGroupFreeItems : getGroupFreeItems())
        {
            msGroupFreeItems->setVisible(true);
            getFreeItemsListView()->pushBackCustomItem(msGroupFreeItems);
        }
        int day = MJDEFAULT->getIntegerForKey(key_f2p_save_learned_items_last_day, 0);
        getFreeItemsListView()->requestDoLayout();
        //getFreeItemsListView()->jumpToPercentHorizontal(100 * day / getAllFreeItemInfos().size());
        //getFreeItemsListView()->setOpacity(0);
        this->runAction(Sequence::create(DelayTime::create(0.2f),
            CallFunc::create([=] {
                getFreeItemsListView()->setOpacity(255);
                mj::helper::deleteWaitView();
                getFreeItemsListView()->scrollToPercentHorizontal(100 * day / getAllFreeItemInfos().size(), 1.0, true);
                }),
            NULL));
        });

    MSFree2PaidPermissionPopup* mSFree2PaidPermissionPopup = new MSFree2PaidPermissionPopup();
    _permissionNotify = mSFree2PaidPermissionPopup->checkPermissionNotifications();
}

MsGroupFreeItems* MSFreeContentPage::initFreeGroupView(int day)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::initFreeGroupView");

    int dayInVector = day <= 0 ? 0 : day;
    if (getAllFreeItemInfos().size() <= dayInVector)
        return nullptr;
    vector<mj::model::FreeItemInfo> freeItemInfosByDay = getAllFreeItemInfos().at(dayInVector);
    MsGroupFreeItems* msGroupFreeItems = MsGroupFreeItems::createView(freeItemInfosByDay,getFreeContentOption(), this);
    msGroupFreeItems->setDelegate(this);

    return msGroupFreeItems;
}
void MSFreeContentPage::APICallBack(bool callAPISuccess) {
    CCLOG("MSFreeContentPage::APICallBack");
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
    mj::helper::deleteWaitView();
    initFreeData(callAPISuccess);
    });
}
void MSFreeContentPage::initFreeData(bool callAPISuccess)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::initFreeData");

    if (callAPISuccess)
    {
        int new_version = MJDEFAULT->getIntegerForKey(key_api_version_version_trial_items_from_server, 0);

        MJDEFAULT->setIntegerForKey(key_api_version_version_trial_items_local, new_version);
    }
    vector<vector<mj::model::FreeItemInfo>> allFreeItemInfosAllday;
    ms::f2p::FreeContentOption freeContentOption = getFreeContentOption();

    std::vector<mj::model::FreeItemInfo> freeItemInfos = StoryDataModel::getInstance()->getFreeTrialInfosByOption(freeContentOption);
    if (freeItemInfos.size() <= 0)
        return;
    switch (freeContentOption)
    {
    case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY: // only get story
    {
        std::vector<mj::model::FreeItemInfo> freeItemInfosOnlyStory;

        for (auto freeItemInfo : freeItemInfos)
        {
            if (freeItemInfo._type == ms::MsTypes::DocumentsType::STORY)
                freeItemInfosOnlyStory.push_back(freeItemInfo);
        }
        if (freeItemInfosOnlyStory.size()<=0)
            return;
        for (auto freeItemInfoOnlyStory : freeItemInfosOnlyStory)
        {
            vector<mj::model::FreeItemInfo> freeItemInfosOnday;
            freeItemInfosOnday.push_back(freeItemInfoOnlyStory);
            allFreeItemInfosAllday.push_back(freeItemInfosOnday);
        }
        break;
    }
    case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM:
    {
        for (auto freeItemInfo : freeItemInfos)
        {
            vector<mj::model::FreeItemInfo> freeItemInfosOnday;
            freeItemInfosOnday.push_back(freeItemInfo);
            allFreeItemInfosAllday.push_back(freeItemInfosOnday);
        }
        break;
    }
    case ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY:
    {
        for (int i = 0; i < freeItemInfos.size(); i+=2)
        {
            vector<mj::model::FreeItemInfo> freeItemInfosOnday;
            for (int j = i; j <= i+1; j++)
            {
                if (freeItemInfos.at(j)._type == ms::MsTypes::DocumentsType::STORY)
                {
                    freeItemInfosOnday.push_back(freeItemInfos.at(j));
                    break;
                }
                
            }

            for (int j = i; j <= i + 1; j++)
            {
                if (freeItemInfos.at(j)._type != ms::MsTypes::DocumentsType::STORY)

                {
                    freeItemInfosOnday.push_back(freeItemInfos.at(j));
                    break;
                }

            }
            allFreeItemInfosAllday.push_back(freeItemInfosOnday);
        }
        break;
    }
    case ms::f2p::FreeContentOption::TEN_ITEM_FREE:
    {
        for (auto freeItemInfo : freeItemInfos)
        {
            vector<mj::model::FreeItemInfo> freeItemInfosOnday;
            freeItemInfosOnday.push_back(freeItemInfo);
            allFreeItemInfosAllday.push_back(freeItemInfosOnday);
        }
        break;
    }
    case ms::f2p::FreeContentOption::DEFAULT:
    {
        break;
    }
    default:
        break;
    }
    setAllFreeItemInfos(allFreeItemInfosAllday);
    if (getFreeItemsListView()) {
        getFreeItemsListView()->cleanup();
        getFreeItemsListView()->setVisible(true);
        onSaveFreeItemsLearned([=](bool isSuccess) {
            initListView();
            });
    }
}

std::vector<int> MSFreeContentPage::getAllStoriesLearn()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::getAllStoriesLearn");

    if(_listOfStoryLearned.empty())
    {
        int profileID = PROFILE_MANAGER.getCurrentProfileId();
        _listOfStoryLearned = PROFILE_MANAGER.getAllReadStories(profileID);
        
    }
    return _listOfStoryLearned;
}

bool MSFreeContentPage::checkItemStoryLearned(mj::model::FreeItemInfo freeItemInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::checkItemStoryLearned");

    std::vector<int> _listIdOfStoryLearned = getAllStoriesLearn();
    if (_listIdOfStoryLearned.size() <= 0)
        return false;
    for (int id : _listIdOfStoryLearned)
    {
        if (id == freeItemInfo._id)
            return true;
    }
    return false;
}
void MSFreeContentPage::caculateCountDown()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::caculateCountDown");

}

void MSFreeContentPage::getCountDown()
{
    CRASH_AUTO_LOG;

}


void MSFreeContentPage::onSaveFreeItemsLearned(std::function<void(bool)> callback)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onSaveFreeItemsLearned");

    onChangePageList();

    ap::Profile profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id, -1));
    //int currentDay = mj::helper::getTimeInDays();

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    int currentDay = mj::helper::getTimeInMinutes();
#else
    int currentDay = mj::helper::getTimeInDays();
#endif
    
    vector<int> freeItemsLearnedArray = {};
    // kiem tra lai so luong item da hoc, va item cuoi cung da hoc trong list o vi tri nao

    int lastItemsLearnedOder = 0;
    vector<int> learnedItemsIdsFromFireBase = {};
    bool endOfTrialLearn = false;  // key check user da hoc xong tat ca cac bai hoc hay chua
    int allNumberItemsTrial = 0;
    _listOfStoryLearned.clear();
    ms::LessonManager::getInstance().resetLearedLesson();
    for (int i = 0; i < _allFreeItemInfos.size(); i++)
    {
        std::vector<mj::model::FreeItemInfo> freeItemInfosOnCurrentDay = _allFreeItemInfos.at(i);
        for (int j = 0; j < freeItemInfosOnCurrentDay.size(); j++)
        {

            mj::model::FreeItemInfo itemInfo = freeItemInfosOnCurrentDay.at(j);
            if (checkItemLearned(itemInfo))
            {
                learnedItemsIdsFromFireBase.push_back(itemInfo._id);
                if (j == freeItemInfosOnCurrentDay.size() - 1)
                    lastItemsLearnedOder = itemInfo._day;

                MJDEFAULT->setIntegerForKey("key_f2p_learned_items", learnedItemsIdsFromFireBase.size());
            }
        }
    }

    if (MJDEFAULT->getIntegerForKey(key_f2p_save_last_date_of_learned_items, 0) == 0)
    {// first time open app, khoi tao cac gia tri, hien thi popup bat dau hoc thu

        MJDEFAULT->setIntegerForKey(key_f2p_save_learned_items_last_day, 1);
        MJDEFAULT->setIntegerForKey(key_f2p_save_last_date_of_learned_items, currentDay);
        MJDEFAULT->setArrayIntegerForKey(key_f2p_save_list_of_learned_items, {});
        if (learnedItemsIdsFromFireBase.size() <= 0)
        {
            showNotifyBeginTrialProgramsAlert([=] {
                if (ms::f2p::FreeContentOption::TEN_ITEM_FREE == getFreeContentOption())
                {
                    CleverTapManager::GetInstance()->pushEvent("ob_start_demo_popup_10_lessons_free_user", {
                    {"click_type",cocos2d::Value(_clickTypeOpenOptinTenFree)},
                        });
                }
                else if (ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY == getFreeContentOption())
                {
                    CleverTapManager::GetInstance()->pushEvent("ob_start_demo_popup_14_lessons", {
                    {"click_type",cocos2d::Value(_clickType14OptionFree)},
                        });
                }
                else
                {
                    CleverTapManager::GetInstance()->pushEvent("ob_start_demo_popup_7_lessons", {
                    {"click_type",cocos2d::Value(_clickType7OptionFree)},
                        });
                }
                });
            return;
        }
        callback(true);
        return;
    }
    // dem so luong items duoc phep hoc thu
    for (int i = 0; i < _allFreeItemInfos.size(); i++)
    {
        std::vector<mj::model::FreeItemInfo> freeItemInfosOnCurrentDay = _allFreeItemInfos.at(i);
        for (mj::model::FreeItemInfo itemInfo : freeItemInfosOnCurrentDay)
        {
            allNumberItemsTrial++;
        }
    }

    // luu lai key thu tu item da hoc cuoi cung 
    MJDEFAULT->setIntegerForKey(key_f2p_save_learned_items_last_day, lastItemsLearnedOder);
    // get ra ngay hoc cua items da hoc moi nhat
   int lastItemsLearnedDate =  MJDEFAULT->getIntegerForKey(key_f2p_save_last_date_of_learned_items, currentDay);

   // neu so items da hoc tu firebase va tu MJDefault bang nhau, --> user chua hoc xong bai ngay hom do
   // neu so items da hoc tu firebase > tu MJDefault, user vua hoc xong bai cua ngay hom do. se khong cho hoc bai cua ngay tiep theo
   vector<int> learnedItemsIdsFromMJDefult = MJDEFAULT->getArrayIntegerForKey(key_f2p_save_list_of_learned_items, {});
   if(learnedItemsIdsFromFireBase.size()>= learnedItemsIdsFromMJDefult.size() + getAllFreeItemInfos().at(0).size())
   {

       // ghi lai so item da hoc trong ngay moi nhat
       vector<int> learnedItemsIdsFromFireBaseInLastday = {};
       for (int id : learnedItemsIdsFromFireBase)
       {

           if (std::find(learnedItemsIdsFromMJDefult.begin(), learnedItemsIdsFromMJDefult.end(), id) == learnedItemsIdsFromMJDefult.end())
           {
               learnedItemsIdsFromFireBaseInLastday.push_back(id);
           }
       }
       
       MJDEFAULT->setIntegerForKey(key_f2p_save_last_date_of_learned_items, currentDay);
       MJDEFAULT->setArrayIntegerForKey(key_f2p_save_list_of_learned_items, learnedItemsIdsFromFireBase);
       if (ms::f2p::FreeContentOption::TEN_ITEM_FREE == getFreeContentOption())
       {
           callback(true);
           if (allNumberItemsTrial <= learnedItemsIdsFromFireBase.size())
           {
               // hiển thị kết thúc chương trình học tại đây
               this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                   showPopupEndOfTheProgramTenFreeLessons([=] {
                       CleverTapManager::GetInstance()->pushEvent("ob_finish_demo_to_register_10_lessons_free_user", {
                        {"click_type",cocos2d::Value(_clickTypeFinishOptionTenFree)},
                           });
                       });
                   }), NULL));
           }
       }
       else
       {
           gotoEndLessonScene([=] {
               callback(true);
               onUnlockAll(nullptr);
               },
               [=] {
                   callback(true);
                   // neu user a hoc het so item hoc thu, show  popup ket thu tai day
                   if (allNumberItemsTrial <= learnedItemsIdsFromFireBase.size())
                   {
                       //hien thi popup ket thuc hoc thu tai day 
                       showNotifyEndTrialProgramsAlert();
                   }
                   else
                   {
                       // neu user moi hoc xong 1 bai dau tien, hien thi popup hoi quyen thong bao

                       if (learnedItemsIdsFromMJDefult.size() == 0 && learnedItemsIdsFromFireBase.size() == 1)
                       {
                           // hien thi popup hoi quyen thong bao 1 lan duy nhat tai day
                           showNotifyPermissionRequestAlert();
                       }
                   }
               }
           );
       }
       return;
   }
   else
   {
       if (allNumberItemsTrial <= learnedItemsIdsFromFireBase.size())
       {
           //hien thi popup ket thuc hoc thu tai day
           showNotifyEndTrialProgramsAlert();
       }
       else if(learnedItemsIdsFromMJDefult.size()> learnedItemsIdsFromFireBase.size())
       {
           learnedItemsIdsFromMJDefult.clear();
           MJDEFAULT->setArrayIntegerForKey(key_f2p_save_list_of_learned_items, learnedItemsIdsFromFireBase);
       }
   }
   callback(true);
}
void MSFreeContentPage::CallAPIFreeContentPage()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::CallAPIFreeContentPage");

    int old_version = MJDEFAULT->getIntegerForKey(key_api_version_version_trial_items_local, 0);
    int new_version = MJDEFAULT->getIntegerForKey(key_api_version_version_trial_items_from_server, 0);

    int profileId = MJDEFAULT->getIntegerForKey("key_last_profile_updated", 0);
    if (profileId == 0)
    {
        profileId = PROFILE_MANAGER.getCurrentProfileId();
    }
    else
    {
        PROFILE_MANAGER.setCurrentProfileId(profileId);
    }
    ap::Profile profile = ap::Database::getInstance().getProfileById(profileId);
    ms::f2p::FreeContentOption freeContentOption = getFreeContentOption();
    std::vector<mj::model::FreeItemInfo> freeItemInfos = StoryDataModel::getInstance()->getFreeTrialInfosByOption(freeContentOption);
    if (old_version < new_version || freeItemInfos.size()<=0)
    {
        CCLOG("old_version< new_version");
        mj::helper::showWaitView();
        mj::api::MJAPI_F2P_TrialContent::call(profile, CC_CALLBACK_1(MSFreeContentPage::APICallBack, this));
    }
    else
    {
        initFreeData(false);
    }
}

void MSFreeContentPage::onBackButtonClicked(cocos2d::Ref* sender)
{

    CRASH_AUTO_LOG;
    gotoMsHome();
}

void MSFreeContentPage::onSignInButton(cocos2d::Ref* sender)
{
    CRASH_AUTO_LOG;
    onSignInF2P();
}

void MSFreeContentPage::onUnlockAll(cocos2d::Ref* sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI
    std::string k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "US");
    if (k_countryCode == "VN" && MJDEFAULT->getStringForKey("key_is_landingPage", "") == "landing")
    {
        cocos2d::Application::getInstance()->openURL(_pathLanding);
        return;
    }
#endif

    CRASH_AUTO_LOG;
    auto newPayment = MSNewTabSubscribe::createTab(_fromPayment);
    newPayment->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    newPayment->setPosition(this->getContentSize() / 2);
    
    newPayment->setSourceForPayment(_sourcePayment);
    
    newPayment->setCallbackOnClose([=]() {
        this->setVisible(true);
        });


    newPayment->setCallbackOnBack([=]() {
        this->setVisible(true);
        });

    pushEventDiscoverUnlock("unlock");

    Director::getInstance()->getRunningScene()->addChild(newPayment, INT_MAX - 1);
    this->setVisible(false);
}

void MSFreeContentPage::pushEventDiscoverUnlock(std::string clickType)
{
    auto option = getFreeContentOption();
    
    if (option != ms::f2p::FreeContentOption::TEN_ITEM_FREE)
    {
        int total_learned = MJDEFAULT->getIntegerForKey("key_f2p_learned_items");
        if (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
        {
            total_learned = total_learned % 2 == 1 ? (total_learned / 2 + 1) : (total_learned / 2);
        }

        string learning_day_package = "day_" + std::to_string(total_learned);

        string total_learned_lesson_counts = std::to_string(MJDEFAULT->getIntegerForKey("key_f2p_learned_items")) + "_"
            + (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? "14" : "7");

        CleverTapManager::GetInstance()->pushEvent(option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY
            ? "unlock_discover_14_lessons" : "unlock_discover_7_lessons", {
                    {"total_learned_lesson_counts",cocos2d::Value(total_learned_lesson_counts)},
                    {"click_type",cocos2d::Value(clickType)},
                    {"learning_day_package",cocos2d::Value(learning_day_package)},
            });
    }
    else {
        CleverTapManager::GetInstance()->pushEvent("ob_10_lessons_free_user", {
                    {"click_type",cocos2d::Value(clickType)},
            });
    }
}

void MSFreeContentPage::onDiscover(cocos2d::Ref* sender)
{
    CRASH_AUTO_LOG;
    gotoMsHome();
    pushEventDiscoverUnlock("discover");
}

void MSFreeContentPage::onChangePageList()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onChangePageList");
    
    _percent = getFreeItemsListView()->getScrolledPercentHorizontal();

    if (_percent < -100)
    {
        getFreeItemsListView()->jumpToLeft();
    }
    else
    {
        if (_percent > 0 && _percent < 100)
        {
            if (_percent > _currentPercentage)
            {
                moveScrollBar(_percent, _percent < 80 ? _percent + 20 : 100, true);
            }
            else
            {
                moveScrollBar(_percent, _percent > 20 ? _percent - 20 : 0, false);
            }
        }

        if (_percent <= 0)
        {
            mj::helper::inertiaScrollLeftOrRight(this, getFreeItemsListView(), true);
            _currentPercentage = 0;
        }

        if (_percent >= 100)
        {
            mj::helper::inertiaScrollLeftOrRight(this, getFreeItemsListView(), false);
            _currentPercentage = 100;
        }
    }

    float indexInPageList = (float)((float)((float)_percent / (float)100) * (float)getRadiusPageList().size());
    if (indexInPageList <= 0.1)
        indexInPageList = 0.1;
    if (indexInPageList >= getRadiusPageList().size())
        indexInPageList = getRadiusPageList().size();
    int i = 0;
    for (auto layoutRadius : getRadiusPageList()) {

        if(i < indexInPageList &&  indexInPageList <= i+1)
            layoutRadius->setBackGroundImage("free2paid/freeItems/blue_radius.png");
        else
        {
            layoutRadius->setBackGroundImage("free2paid/freeItems/white_radius.png");
        }
        i++;
    }
    
}

void MSFreeContentPage::selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::selectedEventScrollView");

    switch (type) {

    case ui::ScrollView::EventType::SCROLLING_ENDED:
    {
        onChangePageList();
        break;
    }

    default:
        break;
    }
}

void MSFreeContentPage::showNotifyBeginTrialProgramsAlert(std::function<void()> callback)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::showNotifyBeginTrialProgramsAlert");
    _isStartDemoPopup = true;

    if (MJDEFAULT->getBoolForKey(key_f2p_show_trial_alert, false))
        return;
    std::string textMessage = "";
    if (ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY == getFreeContentOption()) {
         textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_14_lesson");
         MJDEFAULT->setBoolForKey(key_f2p_option_7_or_14_free_items, true);
    }
    else if (ms::f2p::FreeContentOption::TEN_ITEM_FREE == getFreeContentOption())
    {
        textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_10_lesson_free_unlock");
    }
    else
    {
        textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_7_lesson");
        MJDEFAULT->setBoolForKey(key_f2p_option_7_or_14_free_items, true);
    }
    auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_study_stories");
    auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
    auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("key_try_learn_now");

    ShowNewAlert* alert = nullptr;
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE && textMessage == LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_7_lesson"))
    {
        alert = ShowNewAlert::createAlert(texttitle, "", textok, cancel, true, this);
        alert->MoveTextMessage(textMessage, true, 20);
    }
    else
    {
        alert = ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel, true, this);
    }
    alert->setName("BeginTrialProgramsAlert");
    alert->setOnOkCallback([=] {
        _clickTypeOpenOptinTenFree = "unlock";
        _clickType7OptionFree = "unlock";
        _clickType14OptionFree = "unlock";
        if (callback)
        {
            callback();
        }
        initListView();
        _sourcePayment = setKeyUnlockFromToPayment(true);
        onUnlockAll(nullptr);
        });
    alert->setOnCloseCallback([=] {
        _clickTypeOpenOptinTenFree = "close";
        _clickType7OptionFree = "close";
        _clickType14OptionFree = "close";
        if (callback)
        {
            callback();
        }
        initListView();
        });
    alert->setOnCancelCallback([=] {
        _clickTypeOpenOptinTenFree = "learn_demo_lessons";
        _clickType7OptionFree = "start_now";
        _clickType14OptionFree = "start_now";
        if (callback)
        {
            callback();
        }
        initListView();
        //onDiscover(nullptr);
        });
    MJDEFAULT->setBoolForKey(key_f2p_show_trial_alert, true);// pupup nay chi duoc show 1 lan duy nhat khi lan dau vao app

}

void MSFreeContentPage::showNotifyPermissionRequestAlert()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::showNotifyPermissionRequestAlert");


        MSFree2PaidPermissionPopup* mSFree2PaidPermissionPopup = new MSFree2PaidPermissionPopup();
    if (!_permissionNotify && !MJDEFAULT->getBoolForKey(key_f2p_show_notify_permisison_alert,false))
    {
        mSFree2PaidPermissionPopup->show();
        mSFree2PaidPermissionPopup->onChangeView();
        MJDEFAULT->setBoolForKey(key_f2p_show_notify_permisison_alert, true);
    }
    else
    {
        CC_SAFE_DELETE(mSFree2PaidPermissionPopup);
    }
}

void MSFreeContentPage::showNotifyLessonNotReadyAlertWhenLearned()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::showNotifyLessonNotReadyAlertWhenLearned");

    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.v1.of.popup.notice.completed.lesson");
    auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("title.of.popup.notice.completed.lesson");
    auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.top.button.of.end.lesson.scene");
    auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.bottom.button.of.end.lesson.scene");

    ShowNewAlert* alert = ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel, false, this);
    alert->setOnOkCallback([=]() {
        auto subscribeTab = MSNewTabSubscribe::createTab();
        subscribeTab->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
        subscribeTab->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);

        subscribeTab->setName(mshome_name_tabsubscribe);
        this->removeChildByName(mshome_name_tabsubscribe);
        this->addChild(subscribeTab, kMSHomeZOrder_parentSetting);
        subscribeTab->runActionSequenAnimation();
        subscribeTab->setOpacity(0);
        subscribeTab->runAction(FadeIn::create(1));

        auto option = getFreeContentOption();
        string source = "ob_no_or_partly_done_" + std::to_string(MJDEFAULT->getIntegerForKey("key_f2p_learned_items")) + "_"
            + (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? "14" : "7");

        subscribeTab->setSourceForPayment(source);

        pushEvenPartlyDone7Or14Lessons("true", "unlock");
        });

    alert->setOnCancelCallback([=]() {
        pushEvenPartlyDone7Or14Lessons("true", "continue");
    });

    alert->setName("showNotifyLessonNotReadyAlertWhenLearned");
}

void MSFreeContentPage::showNotifyLessonNotReadyAlertWhenHaveNotLearned()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::showNotifyLessonNotReadyAlertWhenHaveNotLearned");

    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.v2.of.popup.notice.completed.lesson");
    auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("title.of.popup.notice.completed.lesson");
    auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.top.button.of.end.lesson.scene");
    auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("text.continue.learn");

    ShowNewAlert* alert = ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel, false, this);
    alert->setName("showNotifyLessonNotReadyAlertWhenLearned");
    alert->setOnOkCallback([=] () {
        auto subscribeTab = MSNewTabSubscribe::createTab();
        subscribeTab->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
        subscribeTab->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
 
        subscribeTab->setName(mshome_name_tabsubscribe);
        this->removeChildByName(mshome_name_tabsubscribe);
        this->addChild(subscribeTab, kMSHomeZOrder_parentSetting);
        subscribeTab->runActionSequenAnimation();
        subscribeTab->setOpacity(0);
        subscribeTab->runAction(FadeIn::create(1));

        auto option = getFreeContentOption();
        string source = "ob_no_or_partly_done_" + std::to_string(MJDEFAULT->getIntegerForKey("key_f2p_learned_items")) + "_"
            + (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? "14" : "7");

        subscribeTab->setSourceForPayment(source);

        pushEvenPartlyDone7Or14Lessons("false", "unlock");
    });

    alert->setOnCancelCallback([=]() {
        pushEvenPartlyDone7Or14Lessons("false", "continue");
    });
}

void MSFreeContentPage::showNotifyEndTrialProgramsAlert()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::showNotifyEndTrialProgramsAlert");

    if (getFreeContentOption() == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
    {
        showPopupEndOfTheProgramTenFreeLessons(nullptr);
    }
    else
    {
        EndTrialLessonPopup* alert = EndTrialLessonPopup::createView();
        alert->show(this, this->getZOrder() + INT_MAX);
    }

}

void MSFreeContentPage::showPopupEndOfTheProgramTenFreeLessons(std::function<void()> callback)
{
    //hiển thị popup học hết tất cả các bài học tại đây
    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key.content.end_trial_lesson");
    auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
    auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
    auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");

    auto alert =  ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, this, true, false);
    alert->setSizeTextMessage(Size(550, 115));

    alert->setOnOkCallback([=] {
        _clickTypeFinishOptionTenFree = "unlock";
        if (callback)
        {
            callback();
        }
        initListView();
        _sourcePayment = setKeyUnlockFromToPayment(false);
        onUnlockAll(nullptr);
        });
    alert->setOnCloseCallback([=] {
        _clickTypeFinishOptionTenFree = "close";
        if (callback)
        {
            callback();
        }
        initListView();
        });
}

void MSFreeContentPage::gotoMsHome()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::gotoMsHome");

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

void MSFreeContentPage::gotoEndLessonScene(std::function<void()> onUnlockCallback, std::function<void()> onComeBackLaterCallback)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::gotoEndLessonScene");

    auto view = EndLessonScene::createView(getFreeContentOption());
    view->setOnComeBackLaterCallback([=] {

        if(onComeBackLaterCallback)
            onComeBackLaterCallback();
        });
    view->setUnlockAllCallback([=] {
        if(onUnlockCallback)
            onUnlockCallback();
        });
    this->addChild(view, INT_MAX);
}

bool MSFreeContentPage::checkPermissionNotifications()
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::checkPermissionNotifications");

#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
       return false;//CameraHelper::GetInstance()->checkNotifyPermision();
#endif
       return false;
}

void MSFreeContentPage::onTouchToGroupItem(bool isTouchToUnLock, int index)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onTouchToGroupItem");
    onResetLayoutListView(index);
}

bool MSFreeContentPage::onItemsLearn(int day)
{
    CRASH_AUTO_LOG;

    return false;
}

void MSFreeContentPage::onLearnStory(mj::model::StoryInfo storyItemInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onLearnStory");

    MSCollectionStoryItem* story_item = MSCollectionStoryItem::createItem(storyItemInfo);
    //auto story_item = dynamic_cast<MSCollectionStoryItem*>(item);
    if (story_item) {

        MJDEFAULT->setStringForKey("STORY_START_FROM", "MSFreeContentPage ");

        mj::helper::playTransitionStoryCover(story_item, this, 0, INT_MAX, mj::FROMSTORY::FREE_TWO_PAID);

        return;
    }
}


void MSFreeContentPage::onLearnLessonFree(mj::model::LessonInfo lessonItemInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onLearnLessonFree");

    if (auto MSCollectionLessonItem = utils::findChild(this, "MSCollectionLessonItem"));
        this->removeChildByName("MSCollectionLessonItem");
    auto ui = MSCollectionLessonItem::createItem(lessonItemInfo);
    ui->setName("MSCollectionLessonItem");
    ui->setVisible(false);
    this->addChild(ui);
    ui->setDelegate(this);
    ui->downloadingLesson(true);
}
void MSFreeContentPage::onLearnLesson(mj::model::LessonInfo lesson)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::onLearnLesson");
    if(F2P_MANAGER->isFreeContentPage())
        PROFILE_MANAGER.setIntegerForKey(key_grade_lessons_f2p, lesson.lesson_id);


    auto preview = MSLessonPreview::createScene(lesson);
    if (preview)
    {
        preview->setContentSize(this->getContentSize());
        ui::Helper::doLayout(preview);
        preview->setName("lesson_preview");
        Director::getInstance()->pushScene(preview);
        preview->setOpacity(0);
        preview->runAction(FadeIn::create(0.25));
        MJDEFAULT->setIntegerForKey(key_time_start_learn_lesson, int(utils::getTimeInMilliseconds() / 1000));
    }
}

void MSFreeContentPage::onContinueLearn(mj::model::ActInfo info) {

    if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        tmp->removeFromParent();
    }

    if (auto wrapper = MSActivityWrapper::createWithInfo(info)) {
        wrapper->setName("wrapper");
        Director::getInstance()->getRunningScene()->addChild(wrapper, INT_MAX - 1);
    }

    Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
};

bool MSFreeContentPage::checkItemLearned(mj::model::FreeItemInfo freeItemInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("MSFreeContentPage::checkItemLearned");

    bool isLearned = false;

    ms::MsTypes::DocumentsType type = freeItemInfo._type;

    switch (type)
    {
    case ms::MsTypes::DocumentsType::STORY:
    {
        isLearned = checkItemStoryLearned(freeItemInfo);
        break;
    }
    case ms::MsTypes::DocumentsType::PHONICS:
    {
        isLearned = ms::LessonManager::getInstance().checkLessonReadingAndPhonicLearned(freeItemInfo._id);
    }
    case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
    {
        isLearned = ms::LessonManager::getInstance().checkLessonReadingAndPhonicLearned(freeItemInfo._id);

        break;
    }

    default:
        break;
    }
    // dumy dkien de test
    if (freeItemInfo._day <= _countLessonForTest)
        isLearned = true;

    return isLearned;
}
void MSFreeContentPage::onResetLayoutListView(int index)
{
    getFreeItemsListView()->requestDoLayout();
    getFreeItemsListView()->setItemsMargin(20);
    if (ms::f2p::FreeContentOption::TEN_ITEM_FREE != getFreeContentOption())
    {

        if (ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY == getFreeContentOption())
        {
            if (index <= 1)
                getFreeItemsListView()->scrollToLeft(1, true);
            else
            {
                getFreeItemsListView()->jumpToPercentHorizontal(100 * (index + 1) / getAllFreeItemInfos().size());
            }
        }
        else
        {
            if (index <= 2)
                getFreeItemsListView()->jumpToLeft();
            else if(index>=getAllFreeItemInfos().size()-1)
            {
                getFreeItemsListView()->jumpToRight();
            }
            else
            {
                getFreeItemsListView()->jumpToPercentHorizontal(100 * (index) / getAllFreeItemInfos().size());
            }
        } 

    }
}
cocos2d::ui::Widget::ccWidgetClickCallback MSFreeContentPage::onLocateClickCallback(const std::string& callback_name)
{
    CRASH_AUTO_LOG;
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSFreeContentPage, onBackButtonClicked),
        CLICK_MAP(MSFreeContentPage, onUnlockAll),
        CLICK_MAP(MSFreeContentPage, onDiscover),
        CLICK_MAP(MSFreeContentPage, setLearnLesson),
        CLICK_MAP(MSFreeContentPage, onSignInButton),
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void MSFreeContentPage::moveScrollBar(int percent, int movePercent, bool flag)
{
    this->runAction(Sequence::create(DelayTime::create(0.0000001f), CallFunc::create([=] {
        getFreeItemsListView()->jumpToPercentHorizontal(flag ? percent + 1 : percent - 1);
        }), DelayTime::create(0.0000001f), CallFunc::create([=] {
            if (flag)
            {
                if ((percent + 1) < movePercent)
                {
                    moveScrollBar(percent + 1, movePercent, flag);
                }
                else
                {
                    _currentPercentage = getFreeItemsListView()->getScrolledPercentHorizontal();
                    return;
                }
            }
            else
            {
                if ((percent - 1) > movePercent)
                {
                    moveScrollBar(percent - 1, movePercent, flag);
                }
                else
                {
                    _currentPercentage = getFreeItemsListView()->getScrolledPercentHorizontal();
                    return;
                }
            }
            }), NULL));
}


void MSFreeContentPage::setLearnLesson(cocos2d::Ref* sender) // ham nay dung de dubug hoc full bai free
{
    CCLOG("hoc full lesson free");
    int numberOfLesson = getAllFreeItemInfos().size() * getAllFreeItemInfos().at(0).size();
    if (_countLessonForTest < numberOfLesson)
        _countLessonForTest++;
    if (getFreeItemsListView()) {
        getFreeItemsListView()->setVisible(false);
        getFreeItemsListView()->cleanup();
        mj::helper::showWaitView();
        this->runAction(Sequence::create(DelayTime::create(0.5f),
            CallFunc::create([=] {
                getFreeItemsListView()->setVisible(true);
                mj::helper::deleteWaitView();
                onSaveFreeItemsLearned([=](bool isSuccess) {
                    initListView();
                    });
                }),
            NULL));
    }
}

void MSFreeContentPage::setKeyDiscoverToPaymentInMSHome()
{
    std::string str = "";
    ms::f2p::FreeContentOption freeContentOption = getFreeContentOption();
    switch (freeContentOption)
    {
        case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY:
        {
            str = "ob_discover_10_lessons_free_user_7";
            break;
        }
        case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM:
        {
            str = "ob_discover_10_lessons_free_user_7";
            break;
        }
        case ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY:
        {

            str = "ob_discover_10_lessons_free_user_14" ;
            break;
        }
        case ms::f2p::FreeContentOption::TEN_ITEM_FREE:
        {
            str = "ob_discover_10_lessons_free_user";
            break;
        }
    }
    MJDEFAULT->setStringForKey(key_source_subscribe, str);
}
std::string MSFreeContentPage::setKeyUnlockFromToPayment(bool isStart)
{
    std::string str = "";
    ms::f2p::FreeContentOption freeContentOption = getFreeContentOption();
    switch (freeContentOption)
    {
    case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY:
    {
        
        break;
    }
    case ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM:
    {
        str = isStart ? "ob_start_demo_popup_7_lessons" : "ob_finish_demo_to_register_7_lessons";
        break;
    }
    case ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY:
    {
        str = isStart ? "ob_start_demo_popup_14_lessons" : "ob_finish_demo_to_register_14_lessons";
        break;
    }
    case ms::f2p::FreeContentOption::TEN_ITEM_FREE:
    {
        if (MJDEFAULT->getBoolForKey(key_f2p_option_7_or_14_free_items))
        {
            if (getFreeContentOption() == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM)
            {
                str = isStart ? "ob_start_demo_popup_10_lessons_free_user_7" : "ob_finish_demo_to_register_10_lessons_free_user_7";
            }
            else
            {
                str = isStart ? "ob_start_demo_popup_10_lessons_free_user_14" : "ob_finish_demo_to_register_10_lessons_free_user_14";
            }
        }
        else
        {
            str = isStart ? "ob_start_demo_popup_10_lessons_free_user" : "ob_finish_demo_to_register_10_lessons_free_user";
        }
        break;
    }
    case ms::f2p::FreeContentOption::DEFAULT:
    {
        break;
    }
    default:
        break;
    }
    return str;
}

void MSFreeContentPage::pushEvenPartlyDone7Or14Lessons(std::string done, std::string clickType)
{
    auto option = getFreeContentOption();

    int total_learned = MJDEFAULT->getIntegerForKey("key_f2p_learned_items");
    if (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
    {
        total_learned = total_learned % 2 == 1 ? (total_learned / 2 + 1) : (total_learned / 2);
    }

    string learning_day_package = "day_" + std::to_string(total_learned);

    string total_learned_lesson_counts = std::to_string(MJDEFAULT->getIntegerForKey("key_f2p_learned_items")) + "_"
        + (option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY ? "14" : "7");

    CleverTapManager::GetInstance()->pushEvent(option == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY
        ? "ob_no_or_partly_done_14_lessons" : "ob_no_or_partly_done_7_lessons", {
            {"done",cocos2d::Value(done)},
            {"total_learned_lesson_counts",cocos2d::Value(total_learned_lesson_counts)},
            {"click_type",cocos2d::Value(clickType)},
            {"learning_day_package",cocos2d::Value(learning_day_package)},
        });
}
