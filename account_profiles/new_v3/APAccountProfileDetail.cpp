//
//  APAccountProfileDetail.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 8/8/20.
//

#include "APAccountProfileDetail.h"
#include "APDatabase.h"
#include "StoryParentEditProfile_Landscape.h"
#include "StoryParentAccountProfile_Landscape.h"
#include "StoryLanguageManager.h"

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"

#include "APProfileNameView.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"
#include "StoryConfigManager.h"
#include "APAccountUserInfo.h"

USING_NS_CC;

INITIALIZE_READER(APAccountProfileDetail);

std::once_flag v3_ap_account_profile_detail_reader_flag;

APAccountProfileDetail::~APAccountProfileDetail()
{
    try
    {
        CleverTapManager::GetInstance()->pushEvent("edit_profile",
        {
            {"edit_profile_clicked",cocos2d::Value(m_isEditProfileClicked)},
            {"edit_profile_successfully",cocos2d::Value(m_isProfileEditedSuccessfully)},
            {"detail_profile",cocos2d::Value(true)},
        });
    }
    catch (const std::exception&)
    {
        
    }
}

APAccountProfileDetail * APAccountProfileDetail::createPage()
{
    std::call_once(v3_ap_account_profile_detail_reader_flag, [] {
        REGISTER_CSB_READER(APAccountProfileDetail);
    });
    auto node = reinterpret_cast<APAccountProfileDetail *>(CSLoader::createNodeWithVisibleSize("csb/account/new_v3/APAccountProfileDetail.csb"));
    if (node) {
        node->setName(name_account_profile_detail);
        node->didLoadFromCSB();
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}

void APAccountProfileDetail::didLoadFromCSB()
{
    _edit_enabled = false;
    m_lv_profile = cocos2d::utils::findChild<ui::ListView*>(this, "lvProfile");
    m_lv_profile->setScrollBarColor(Color3B(255, 255, 255));
    m_lv_profile->setScrollBarAutoHideEnabled(false);
    m_lv_profile->setScrollBarEnabled(true);
    m_lv_profile->removeAllItems();
    
    // co giãn một chút với màn hình dài
    if (Director::getInstance()->getWinSize().width > 1024.f) {
        float sss = 0.2f; // vì max profile là 4 nên giá trị này nên nhỏ hơn 0.25
        float adding = sss * (Director::getInstance()->getWinSize().width - 1024.f) / 2.f;
        m_lv_profile->setItemsMargin(m_lv_profile->getItemsMargin() + adding);
    }

    auto lv_pos = m_lv_profile->getPosition();
    auto list_width = 0.0f;
    auto list_profile = ap::Database::getInstance().getProfiles();
    auto current_it = 0;
    int i = 0;
    for (auto profile : list_profile)
    {
        auto card_profile = APAccountProfileCard::createCard(profile);
        card_profile->setDelegate(this);
        float rate_scale = 1.0f;
        card_profile->setScale(rate_scale);
        m_lv_profile->pushBackCustomItem(card_profile);
        list_width += card_profile->getContentSize().width*rate_scale + m_lv_profile->getItemsMargin();
        
        if(profile.profile_id == MJDEFAULT->getIntegerForKey(key_current_profile_id)){
            current_it = i;
        }
        i++;
    }
    if (list_width > 0)
    {
        list_width -= m_lv_profile->getItemsMargin();
    }

    m_lv_profile->setContentSize(Size (list_width , m_lv_profile->getContentSize().height));
    
    if (list_profile.size() < 4)
    {
        m_lv_profile->setContentSize(cocos2d::Size(list_width, m_lv_profile->getContentSize().height));
        m_lv_profile->setPositionX(this->getContentSize().width * 0.5);
        
    } else {
        m_lv_profile->setMagneticType(ui::ListView::MagneticType::CENTER);
        
        m_lv_profile->setCurSelectedIndex(current_it);
    }
    
    if (list_width > this->getContentSize().width)
    {
        // nothing here
    }
    // hide edit button if don't have profile
    if (list_profile.empty())
    {
        utils::findChild(this,"btn_edit")->setVisible(false);
        auto btn_create = utils::findChild<ui::Button*>(this, "btn_create");
        btn_create->setPositionType(PositionType::PERCENT);
        btn_create->setPositionPercent(Vec2(0.5f,0.5f));
    }
    
    // hide create button if user has enough profile
    if(list_profile.size() >= MJDEFAULT->getIntegerForKey(key_max_account_profile)){
        utils::findChild(this,"btn_create")->setVisible(false);
        auto btn_edit = utils::findChild<ui::Button*>(this, "btn_edit");
        btn_edit->setPositionType(PositionType::PERCENT);
        btn_edit->setPositionPercent(Vec2(0.5f,0.5f));
    }
    
    // title
    if (auto title = cocos2d::utils::findChild<ui::Text *>(this, "title"))
    {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Bold.ttf");
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.accountv3.screen.account.manageprofiles"));
    }
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        if (auto title = cocos2d::utils::findChild<ui::Text *>(this, "title"))
        {
            title->setTextColor(Color4B::WHITE);
        }
        
        if (auto background = utils::findChild<ui::Layout *>(this, "background"))
        {
            background->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
        }
        
        if (auto button_back = cocos2d::utils::findChild<ui::Button *>(this, "btn_back"))
        {
            button_back->loadTextureNormal("account/new_v3_vn/button_back_left.png");
            button_back->loadTexturePressed("account/new_v3_vn/button_back_left.png");
            button_back->resetNormalRender();
            button_back->resetPressedRender();
            button_back->resetDisabledRender();
            button_back->getRendererNormal()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererClicked()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererDisabled()->setPosition(button_back->getContentSize()/2);
        }
    }
    
    // showing effect
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.25f));
}

void APAccountProfileDetail::onEnter()
{
    ui::Layout::onEnter();
    
    ms::EventLogger::getInstance().logEvent("view_settings_profile", {});
    
    std::vector<cocos2d::ui::Text*> textSameFonts =
    {
        utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_edit"), "Text_1_0_0"),
        utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_create"), "Text_1_0_0"),
    };
    
    for(auto text : textSameFonts)
    {
        if(!text)
        {
            continue;
        }
        
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }

    utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_edit"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("favorite.edit"));
    utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_create"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.new.profile"));
}

void APAccountProfileDetail::onExit()
{
    ui::Layout::onExit();
    
    if (auto accountLayer = dynamic_cast<APAccountUserInfo *>(m_delegate))
     {
         accountLayer->resetProfileInfo();
     }
}

void APAccountProfileDetail::onNewProfile(Ref * sender)
{
    if (!ap::Database::getInstance().isReachMaxProfile()) // nếu chưa max số profile
    {
        ms::EventLogger::getInstance().logEvent("click_new_profile", {});
        auto profile_name = APProfileNameView::createView();
        profile_name->setDelegate(this);
//        Director::getInstance()->getRunningScene()->addChild(profile_name);
        this->addChild(profile_name, INT_MAX-1);
    }
    else
    {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.full.profiles"), "OK",LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
    }
}

void APAccountProfileDetail::onEditProfile(cocos2d::Ref * sender)
{
    m_isEditProfileClicked = true;

    if (m_delegate != nullptr)
    {
        if (auto delegate = dynamic_cast<StoryParentAccountProfile_Landscape*>(m_delegate))
        {
            delegate->onEditProfile();
        }
    }

    ms::EventLogger::getInstance().logEvent("click_edit_profile", {});
    _edit_enabled = true;
    utils::findChild(this, "btn_edit")->setVisible(false);
    utils::findChild(this, "btn_create")->setVisible(false);
    for (auto profile : m_lv_profile->getItems())
    {
        static_cast<APAccountProfileCard*>(profile)->setCardMode(APAccountProfileCard::APCARD_MODE::EDITABLE);
    }
}

void APAccountProfileDetail::onBack(cocos2d::Ref * sender)
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void APAccountProfileDetail::onClose(cocos2d::Ref * sender)
{
    if (_edit_enabled)
    {
        _edit_enabled = false;
        utils::findChild(this, "btn_edit")->setVisible(true);
        
       // hide create button if user has enough profile
        if(ap::Database::getInstance().getProfiles().size() < MJDEFAULT->getIntegerForKey(key_max_account_profile)){
            utils::findChild(this, "btn_create")->setVisible(true);
        }
        
        for (auto profile : m_lv_profile->getItems())
        {
            static_cast<APAccountProfileCard*>(profile)->setCardMode(APAccountProfileCard::APCARD_MODE::NORMAL);
        }
        
    }
    else
    {
        if (m_delegate != nullptr)
        {
            if (auto delegate = dynamic_cast<StoryParentAccountProfile_Landscape*>(m_delegate))
            {
                delegate->setVisible(true);
            }
        }
        
        this->removeFromParent();
    }
}

void APAccountProfileDetail::onClickProfileIcon(cocos2d::Ref * sender)
{
    auto card = static_cast<APAccountProfileCard*>(sender);
    if (card->getCardMode() == APAccountProfileCard::EDITABLE)
    {
        auto edit_view = StoryParentEditProfile_Landscape::createView(card->getProfile());
//        Director::getInstance()->getRunningScene()->addChild(edit_view, INT_MAX - 1);
        this->addChild(edit_view, INT_MAX - 1);
        auto listener = cocos2d::EventListenerCustom::create("key.event.account.profile.edit.success", [this, card](EventCustom * event) {
            auto profile = (ap::Profile *) event->getUserData();
            auto profile_cpy = *profile; // copy value profile get from edit view
            if (profile)
            {
                this->scheduleOnce([profile_cpy, card](float dt) {
                    card->setProfile(profile_cpy);
                }, 0.1f,"delay exit");
            }
                     
            m_isProfileEditedSuccessfully = true;
            
            // TODO: Show lại các button edit/create và ẩn nút edit ở profileCard ở đây
            utils::findChild(this, "btn_edit")->setVisible(true);
            
            if (!ap::Database::getInstance().isReachMaxProfile()) {
                utils::findChild(this, "btn_create")->setVisible(true);
            }
            
            for (auto profile : m_lv_profile->getItems())
            {
                static_cast<APAccountProfileCard*>(profile)->setCardMode(APAccountProfileCard::APCARD_MODE::NORMAL);
            }
            
            cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("key.event.account.profile.edit.success");
        });

        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
}

void APAccountProfileDetail::onProfileCreated(int profile_id)
{
    this->didLoadFromCSB();

    CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN,[=](){
        
        const int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

        CleverTapManager::GetInstance()->pushProfileByID(k_currentProfileID,CleverTapManager::PUSH_TYPE::LOGIN,nullptr);
        
    });
}

ui::Widget::ccWidgetClickCallback APAccountProfileDetail::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APAccountProfileDetail, onNewProfile),
        CLICK_MAP(APAccountProfileDetail, onEditProfile),
        CLICK_MAP(APAccountProfileDetail, onBack)
    };

    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }

    return nullptr;
}

void APAccountProfileDetail::pushCleverTapTrackingData(int profileID)
{
    try
    {
        auto profile = ap::Database::getInstance().getProfileById(profileID);

        std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =
        {
          {"view_screen", cocos2d::Value(true)},
          {"profile_name", cocos2d::Value(profile.name)},
          {"profile_gender", cocos2d::Value(profile.gender)},
          {"profile_age", cocos2d::Value(profile.age)},
        };

        CleverTapManager::GetInstance()->pushEvent("create_new_profile", eventTrackingProperties);
    }
    catch (const char * i_errorCode)
    {
        CCLOG("Error: cannot push story tracking data to server !");
    }
}
