#include "Merge2Scene.h"



USING_NS_CC;
INITIALIZE_READER(Merge2Scene);

#define MERGE_2_SCENE_NAME "Merge2Scene"

std::once_flag ms_f2p_merge_2_scene_reader;

Scene* Merge2Scene::createScene(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges)
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = Merge2Scene::createView(f2PAccountInfoForMerges);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

Merge2Scene* Merge2Scene::createView(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges)
{
    std::call_once(ms_f2p_merge_2_scene_reader, []
        {
            REGISTER_CSB_READER(Merge2Scene);
        });

    auto csb_name = mj::helper::getCSBName("csb/free2paid/merge/Merge2.csb");//5

    auto view = reinterpret_cast<Merge2Scene*>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (view)
    {
        view->didLoadFromCSB(f2PAccountInfoForMerges);
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void Merge2Scene::didLoadFromCSB(vector<ms::f2p::F2PAccountInfoForMerge> f2PAccountInfoForMerges)
{
    this->setName(MERGE_2_SCENE_NAME);
    LANGUAGE_MANAGER->changeDisplayLanguage(this);
    _f2PAccountInfoForMerges = f2PAccountInfoForMerges;
    MJDEFAULT->deleteValueForKey(key_list_profile_to_delete);
   setMergeSuccessStatus (MJDEFAULT->getBoolForKey(key_f2p_merge_status_response));
    setMaxProfile(F2PManager::GetInstance()->getProfileMergeMaxProfile());
   // set lai so max profile co the giu
    if(getMaxProfile()<=0)
    {
        for (ms::f2p::F2PAccountInfoForMerge f2PAccountInfoForMerge : _f2PAccountInfoForMerges)
        {
            if (getMaxProfile() < f2PAccountInfoForMerge._maxProfile)
                setMaxProfile(f2PAccountInfoForMerge._maxProfile);
        }
    }
    if (getMergeSuccessStatus())
    {
        if (auto m_btnSubmit = cocos2d::utils::findChild<Button*>(this, "btnSubmit"))
        {
            m_btnSubmit->setEnabled(true);
        }
        _listProfile = ap::Database::getInstance().getProfiles();
    }
    else
    {
        for (ms::f2p::F2PAccountInfoForMerge f2PAccountInfoForMerge : f2PAccountInfoForMerges)
        {
            for (auto profile : f2PAccountInfoForMerge._profileList)
            {
                _listProfile.push_back(profile);
            }
        }
        if (auto m_btnSubmit = cocos2d::utils::findChild<Button*>(this, "btnSubmit"))
        {
            m_btnSubmit->setEnabled(false);
        }
    }
    _listProfileForView = _listProfile;


    _profileListView = cocos2d::utils::findChild<ListView*>(this, "merge2.scene.list.view");

    initListViewProfile(_profileListView, _listProfileForView);
    showProductPackages();
    //load user in4
    auto m_userIn4 = ap::Database::getInstance().getUserInfo();
    if (auto m_txtFullName = cocos2d::utils::findChild<Text*>(this, "merge2.scene.txt.full.name"))
    {
        m_txtFullName->setString(m_userIn4._name);
    }
    if (auto m_txtPhone = cocos2d::utils::findChild<Text*>(this, "merge2.scene.txt.phone"))
    {
        m_txtPhone->setString(m_userIn4._phone);
    }
    if (auto m_txtEmail = cocos2d::utils::findChild<Text*>(this, "merge2.scene.txt.email"))
    {
        m_txtEmail->setString(m_userIn4._mail);
    }
    updatePage();
}
MsProfileItem* Merge2Scene::initProfileItem(ListView* listview, ap::Profile profileInfo)
{
    CRASH_AUTO_LOG;
    CCLOG("Merge1Scene::initProfileItem");
    MsProfileItem* msProfileItem = MsProfileItem::createView(profileInfo, this, !getMergeSuccessStatus());
    auto m_scaledSize = listview->getContentSize().height / msProfileItem->getContentSize().height;
    msProfileItem->scalingSize(m_scaledSize*0.8);
    return msProfileItem;
}
void Merge2Scene::initListViewProfile(ListView* listview, std::vector<ap::Profile> profile)
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
    listview->setClippingEnabled(false);
    if (profile.size() >= 4) {
        //listview->setScrollBarEnabled(true);
        listview->setTouchEnabled(true);
    }
    listview->setBounceEnabled(true);
    listview->setVisible(true);
    listview->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(Merge2Scene::selectedEventScrollView, this));
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        listview->removeAllChildren();
        float m_listViewWidth = 0.f;
        Size itemsSize;
        for (int i = 0; i < profile.size(); i++)
        {
            MsProfileItem* msProfileItem = initProfileItem(listview, profile.at(i));
            msProfileItem->setVisible(true);
            msProfileItem->setTouchEnabled(true);
            msProfileItem->setZOrder(INT_MAX - 1);
            itemsSize = msProfileItem->getContentSize();
            if (!msProfileItem)
                continue;
            listview->pushBackCustomItem(msProfileItem);
        }
        if (itemsSize.width > 0)
        {
            listview->setContentSize(Size((itemsSize.width + 10.0f) * profile.size() - 10.0f, itemsSize.height * 1.1));
        }
        if (listview->getContentSize().width >= listview->getParent()->getContentSize().width)
        {
            listview->setContentSize(Size(listview->getParent()->getContentSize().width, listview->getContentSize().height));
        }
        if (listview->getContentSize().width >= listview->getParent()->getContentSize().width*0.8)
        {
            listview->setClippingEnabled(true);
        }
        listview->requestDoLayout();
        });
}

void Merge2Scene::showProductPackages()
{
    if (_f2PAccountInfoForMerges.size() >= 2)
    {

        std::string  _productId = "";
        monkey::iap::PRODUCT_PACKAGES  productPackageTemp = monkey::iap::PRODUCT_PACKAGES::FREE;
        auto m_userIn4 = ap::Database::getInstance().getUserInfo();
        std::vector<InAppPackageModel> inAppPackageModel = APPURCHASE_MANAGER->getPurchasedPackages();

        if (inAppPackageModel.size() <= 0)
        {
            for (ms::f2p::F2PAccountInfoForMerge f2PAccountInfoForMerge : _f2PAccountInfoForMerges)
            {

                if (f2PAccountInfoForMerge._inAppPackageModel.empty())
                    continue;
                monkey::iap::PRODUCT_PACKAGES  productPackage = IAP_MANAGER->getProductPackage(f2PAccountInfoForMerge._inAppPackageModel.at(0)._productId);
                if (productPackage >= productPackageTemp)
                {
                    productPackageTemp = productPackage;
                    _productId = f2PAccountInfoForMerge._inAppPackageModel.at(0)._productId;
                }
            }
        }
        else
        {
            _productId = inAppPackageModel.at(0)._productId;
        }

        if (auto text = cocos2d::utils::findChild<Text*>(this, "lb.merge2.scene.txt.free.pack"))
        {
            (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-ExtraBold.ttf");
            std::string packageName = getPackageName(_productId);
            text->setString(packageName);
        }
    }

}

void Merge2Scene::onDisableDeleteProFile()
{
    for (auto item : _profileListView->getItems())
    {

        MsProfileItem* msProfileItem = static_cast<MsProfileItem*>(item);
        msProfileItem->setHidenDeleteButton(false);
    }
    _profileListView->forceDoLayout();
    _profileListView->jumpToLeft();
    updatePage();
}

std::string Merge2Scene::getPackageName(std::string productId)
{
    std::string packageName = "";
    std::string packagePre = LANGUAGE_MANAGER->getDisplayTextByKey("product.name.com.earlystart.stories.package").c_str();
    std::string packTime = LANGUAGE_MANAGER->getDisplayTextByKey("notifree.title").c_str();
    monkey::iap::PRODUCT_PACKAGES  productPackage = IAP_MANAGER->getProductPackage(productId);
    switch (productPackage)
    {
    case monkey::iap::FREE:
    {
        packTime = LANGUAGE_MANAGER->getDisplayTextByKey("notifree.title").c_str();
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

cocos2d::ui::Widget::ccWidgetClickCallback Merge2Scene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        //CLICK_MAP(Merge1, onLanguageSelected),
        CLICK_MAP(Merge2Scene, onBtnBackToMerge1Clicked),
        CLICK_MAP(Merge2Scene, onBtnSubmitClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void Merge2Scene::onBtnBackToMerge1Clicked(cocos2d::Ref* sender)
{
    CCLOG("Merge 2 back");
    this->removeFromParent();
   // auto scene = Merge1Scene::createScene();
   // Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
}

void Merge2Scene::onBtnSubmitClicked(cocos2d::Ref* sender)
{
    CCLOG("Merge 2 submuit");
    mj::helper::showWaitView();
    auto m_listProfileToDelete = MJDEFAULT->getStringForKey(key_list_profile_to_delete, "");
    auto m_listProfileToDeleteInt =MJDEFAULT->getArrayIntegerForKey(key_list_profile_to_delete, {});
    auto m_userID = ap::Database::getInstance().getUserInfo()._id;

    if (_listProfileForView.size() >= getMaxProfile())
    {
        ap::api::MergeAccountAPI::callDeleteProfile(m_userID, m_listProfileToDelete, [=](bool is_success, std::string message) {
            this->runAction(Sequence::create(DelayTime::create(0.2f),
                CallFunc::create([=] {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        mj::helper::deleteWaitView();
                        if (is_success) {
                            for(int profileId : m_listProfileToDeleteInt)
                            {
                                if(MJDEFAULT->getIntegerForKey("key_profile_skip_id_merge_clevertap") == profileId)
                                {
                                    MJDEFAULT->setBoolForKey("key_merge_profile_clevertap", true);
                                }
                                for(auto f2PAccountInfoForMerge: _f2PAccountInfoForMerges)
                                {
                                    for(auto profile: f2PAccountInfoForMerge._profileList)
                                    {
                                        if(profile.profile_id == profileId)
                                        {
//                                            CleverTapManager::GetInstance()->setUserInfoForDelete(f2PAccountInfoForMerge._userInfo);
//                                            CleverTapManager::GetInstance()->setProfileInfoForDelete(profile);
//                                            CleverTapManager::GetInstance()->disableProfileByID(profileId,CleverTapManager::PUSH_TYPE::LOGIN, nullptr, false);
//                                            break;
                                        }
                                    }
                                }
                            }
                            CCLOG("Delete request success received respond");

                            callMergAgain();

                        }
                        else
                        {
                            CCLOG("%s\n", message.c_str());
                        }


                        });

                    }),
                NULL));

            });
    }
    else
    {
        switchToMergeSuccess();
    }
    
}

void Merge2Scene::onSelectedProfile(int profileId)
{

}

void Merge2Scene::onDeleteSelectedProfile(int profileId)
{
    _listProfileToDelete = MJDEFAULT->getArrayIntegerForKey(key_list_profile_to_delete,{});
    _listProfileToDelete.push_back(profileId);

    MJDEFAULT->setArrayIntegerForKey(key_list_profile_to_delete, _listProfileToDelete);
    
    _profileListView->requestDoLayout();
    _profileListView->jumpToLeft();
    _listProfileForView;
    for (auto it = _listProfileForView.begin(); it != _listProfileForView.end(); ++it) {
        if (it->profile_id == profileId) {
            _listProfileForView.erase(it);
            break;
        }
    }
    for (auto item : _profileListView->getItems())
    {

        MsProfileItem* msProfileItem = static_cast<MsProfileItem*>(item);
        if (msProfileItem->getProfileInfo().profile_id == profileId)
        {
            _profileListView->setContentSize(Size(_profileListView->getContentSize().width - item->getContentSize().width, _profileListView->getContentSize().height));
            _profileListView->removeItem(_profileListView->getIndex(item));
            break;
        }
    }
    
    _profileListView->requestDoLayout();
    _profileListView->jumpToLeft();

    if (_listProfileForView.size()<= getMaxProfile())
    {
        onDisableDeleteProFile();
    }
}

void Merge2Scene::switchToMergeSuccess()
{
    mj::helper::showWaitView();
    auto uid = MJDEFAULT->getStringForKey("key_new_account_uid");
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            auto scene = MSMergeSuccess::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            }),
        NULL));
    
}

void Merge2Scene::callMergAgain()
{
    if (_f2PAccountInfoForMerges.size() < 2)
        return;
    auto newAccountToken = _f2PAccountInfoForMerges.at(0)._token;
    auto oldAccountToken = _f2PAccountInfoForMerges.at(1)._token;
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            mj::helper::showWaitView();

            ap::api::MergeAccountAPI::callMerge(oldAccountToken, newAccountToken, [=](bool is_success, std::string message, int errorCode) {
                this->runAction(Sequence::create(DelayTime::create(0.2f),
                    CallFunc::create([=] {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

                            if (is_success) {
                                CCLOG("Merge request success received respond");

                                MJDEFAULT->setBoolForKey(key_f2p_merge_status_response, true);
                                F2P_MANAGER->saveMergedUserInfo(_f2PAccountInfoForMerges);
                                switchToMergeSuccess();
                            }
                            else
                            {
                                CCLOG("%s\n", message.c_str());
                                MJDEFAULT->setBoolForKey(key_f2p_merge_status_response, false);
                                if (errorCode == 402) // loi do merge tai khoan tron doi voi tai khoan da mua, xu ly show popup
                                {
                                    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.f2p.lifetime.purchased.account");
                                    auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
                                    auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
                                    auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");
                                    auto alert = ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, this, true, false);
                                    alert->setSizeTextMessage(Size(550, 115));
                                    alert->MoveTextMessage(textMessage, false, 10);
                                }
                            }

                            mj::helper::deleteWaitView();
                            });

                        }),
                    NULL));
             

                });
            }),
        NULL));

}

void Merge2Scene::updatePage()
{
    int m_maxProfile = getMaxProfile();
    if (m_maxProfile <= 0)
        m_maxProfile = 3;
    if ((_listProfileForView.size() <= m_maxProfile) || getMergeSuccessStatus())
    {
        if (auto m_btnSubmit = cocos2d::utils::findChild<Button*>(this, "btn_submit"))
        {
            m_btnSubmit->setEnabled(true);
        }
        if (auto m_txtWarning = cocos2d::utils::findChild<Text*>(this, "lb.merge2.scene.txt.warning"))
        {
            m_txtWarning->setVisible(false);
        }
        
    }
    else
    {
        if (auto m_btnSubmit = cocos2d::utils::findChild<Button*>(this, "btn_submit"))
        {
            m_btnSubmit->setEnabled(false);
        }
        if (auto m_txtWarning = cocos2d::utils::findChild<Text*>(this, "lb.merge2.scene.txt.warning"))
        {
            m_txtWarning->setVisible(true);
            std::string txtWarning = m_txtWarning->getString();
            int numProfileToDelete = _listProfileForView.size() - m_maxProfile; 
            int posOccur = txtWarning.find("1");
            if (posOccur != std::string::npos)
            {
                std::string newTxtWarning = txtWarning.substr(0, posOccur) 
                    + std::to_string(numProfileToDelete) + txtWarning.substr(posOccur + 1, txtWarning.length() - posOccur - 1);
                m_txtWarning->setString(newTxtWarning);
            }
        }
    }

    
}
void Merge2Scene::selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
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
}
void Merge2Scene::onExit()
{
    Layout::onExit();
}



