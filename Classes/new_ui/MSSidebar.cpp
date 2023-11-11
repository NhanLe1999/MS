//
//  MSSidebar.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#include "MSSidebar.h"
#include "MSAppDrawer.h"
#include "MSSidebarDelegate.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "HelperManager.h"

#include "GAFWrapper.h"
#include "HSAudioEngine.h"

#include "MJPlatformConfig.h"
#include "StoryConfigManager.h"

#include "HSLib.h"
#include "StoryDataModel.h"
#include "StoryFreeManager.h"
#include "APDatabase.h"

#include "MJCalculator.h"
#include "MMQRScan.h"
#include "NativeBridge.h"
#include "StoryAlert.h"
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "new/APEnterNewPasswordPopupForOldUser.h"
//#else
//#include "APEnterNewPasswordPopupForOldUser.h"
//#endif

#include "MMQRScan.h"
#include "StoryAudioPlayer.h"
#include "MSFirebasePlugin.h"
#include "APProfileSwitch.h"
#include "APLoadUpdateAPI.h"
#include "APEnterEmailPopup.h"
#include "APAccountDefinesV3.h"
#include "AppInbox.h"
#include "APPurchaseManager.h"
#include "SpineWrapper.h"
#include "F2PManager.h"
#include "APLoginScreen.h"
#include "APLoginView.h"
USING_NS_CC;
INITIALIZE_READER(MSSidebar);

std::once_flag ms_sidebar_reader;

MSSidebar * MSSidebar::defaultSidebar = nullptr;

std::vector<std::string> list_name_des =
{
    std::string("sidebar.des.material"),
    std::string("sidebar.des.stories"),
    std::string("sidebar.des.lessons"),
    std::string("sidebar.des.ai_speaking"),
    std::string("sidebar.des.audiobooks"),
    std::string("sidebar.des.free"),
    std::string("sidebar.des.subscriptions"),
    std::string("sidebar.des.settings"),
    std::string("sidebar.des.account"),
    std::string("sidebar.des.favourite"),
    std::string("sidebar.des.parents"),
    std::string("sidebar.des.close"),
    std::string("sidebar.des.license"),
    std::string("sidebar.des.more"),
    std::string("sidebar.des.qr"),
    std::string("sidebar.des.support")
};

std::map<std::string, std::string> button_gaf =
{
    {"button_stories", "stories/stories.gaf"},
    {"button_lessons", "lessons/lessons.gaf"},
    {"button_audiobooks", "audiobooks/audiobooks.gaf"},
    {"button_free", "free/free.gaf"},
    {"button_favourite", "favourite/favourite.gaf"},
    {"button_qr", ""},
};

const std::map<std::string, std::string> button_sound =
{
    {"button_stories", "stories/stories.mp3"},
    {"button_lessons", "lessons/lessons.mp3"},
    {"button_audiobooks", "audiobooks/audiobooks.mp3"},
    {"button_free", "free/free.mp3"},
    {"button_favourite", "favourite/favourite.mp3"},
    {"button_qr", ""},
    {"button_material", "worksheets/worksheets.mp3"},
};

MSSidebar * MSSidebar::createSidebar()
{
    std::call_once(ms_sidebar_reader, [] {
        REGISTER_CSB_READER(MSSidebar);
    });
    std::string csb_name = "";
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        csb_name = mj::helper::getCSBName(CONFIG_MANAGER->isVietnameseStories() ? "csb/new_ui/Sidebar_VN.csb" : "csb/new_ui/Sidebar_Ai_Speaking.csb");
    }
    else {
        csb_name = mj::helper::getCSBName(CONFIG_MANAGER->isVietnameseStories() ? "csb/new_ui/Sidebar_VN.csb" : "csb/new_ui/Sidebar.csb");
    }
    
    auto sidebar = reinterpret_cast<MSSidebar *>(CSLoader::createNode(csb_name));
    
    if (sidebar)
    {
        defaultSidebar = sidebar;
        sidebar->didLoadFromCSB();
        return sidebar;
    }
    
    CC_SAFE_DELETE(sidebar);
    return nullptr;
}

bool MSSidebar::shouldShowQR() {
    return false;
    // Nếu là MS thì ẩn button QR, thay bằng button Worksheet
    if (!CONFIG_MANAGER->isVietnameseStories() && MJDEFAULT->getBoolForKey(key_feature_worksheet)){
        return false;
    }

    if(MJDEFAULT->getBoolForKey(key_is_submit)|| !CONFIG_MANAGER->isCountryVN()){
        return false;
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return false;
#endif
//    if(CONFIG_MANAGER->isVietnameseStories()){
//        return false;
//    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return true;
#endif

    return MJDEFAULT->getBoolForKey(key_app_should_show_qr);
}
void MSSidebar::loadDefaultAvatar(ap::Profile profile)
{
    if (APPURCHASE_MANAGER->isSkipUser())
    {
        auto imagePath = "free2paid/icons/userDefault.png";
        if (!FileUtils::getInstance()->isFileExist(imagePath))
        {
            imagePath = profile.avatar.c_str();
        }
        utils::findChild<ui::ImageView*>(this, "sidebar.profile.avatar")->loadTexture(imagePath);
    }
    else {
        utils::findChild<ui::ImageView*>(this, "sidebar.profile.avatar")->loadTexture(profile.avatar);
    }
}
void MSSidebar::onAISpeaking(cocos2d::Ref* sender)
{
    if (_current_button == sender)
        return;

    if (_is_animating) {
        return;
    }
    auto spine_button_ai_speaking = utils::findChild<SpineWrapper*>(this, "spine");
    if (spine_button_ai_speaking)
    {
        ResetAndCreatSpineForAiSpeakingButton("animation1", false, true);

    }
    MJDEFAULT->setStringForKey("key_sidebar_tab_select", "aiSpeaking");
    auto d = moveBackground(reinterpret_cast<ui::Button*>(sender));
    MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::AI_SPEAKING);
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::AI_SPEAKING, (ms::MovingDirection)d) : (void) nullptr;
}
void MSSidebar::showHidenMaterialButton(bool isShow)
{
    if (isShow) {
        if (auto button_material = utils::findChild(this, "button_material")) {
            auto button_subscriptions = utils::findChild<ui::Button*>(this, "button_subscriptions");
            if (button_subscriptions)
                button_material->setPosition(button_subscriptions->getPosition() + Vec2(0, 120));
            GAFWrapper* gafExits = utils::findChild<GAFWrapper*>(button_material, "gaf");
            if (!gafExits)
            {
                gafExits = GAFWrapper::createGAF("gaf/sidebar/worksheets/worksheets.gaf");
                auto image = button_material->getChildByName("image");
                gafExits->setAnchorPoint(Point::ANCHOR_MIDDLE);
                gafExits->setName("gaf");
                gafExits->setPosition(image->getPosition() + Point(0, 11));
                button_material->addChild(gafExits);
                image->setVisible(false);
            }

            auto materialGafPre = utils::findChild(button_material, "material_new_button_gaf");
            if (!materialGafPre)
            {
                auto materialGaf = GAFWrapper::createGAF("gaf/sidebar/new/new.gaf");
                materialGaf->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                materialGaf->setPosition(Vec2(button_material->getContentSize().width, button_material->getContentSize().height - materialGaf->getContentSize().height));
                materialGaf->playSequence("idle");
                materialGaf->setName("material_new_button_gaf");
                materialGaf->setAnchorPoint(Vec2(0.5, 0.5));
                materialGaf->setScale(1.1);
                button_material->addChild(materialGaf);

            }
            if (gafExits)
                gafExits->playSequence("idle");
            button_material->setVisible(true);
        }
    }
    else
    {
        if (auto button_material = utils::findChild<ui::Button*>(this, "button_material")) {
            button_material->setVisible(false);
        }
    }
}
void MSSidebar::ResetAndCreatSpineForAiSpeakingButton(std::string creatSequenceName, bool loop, bool playAudio)
{
    if (auto button_ai_speaking = utils::findChild<ui::Button*>(this, "button_ai_speaking")) {
        SpineWrapper* spine = nullptr;
        std::string folderPath = "spine/recording";
        bool isFolderExits = FileUtils::getInstance()->isDirectoryExist(folderPath);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        isFolderExits = true;
#endif
        if (isFolderExits) {
            std::string path = "spine/recording/MIC";
            spine = SpineWrapper::createWithPath(path);
            if (auto oldeSpine = utils::findChild<SpineWrapper*>(button_ai_speaking, "spine"))
            {
                oldeSpine->removeFromParent();
            }
            auto button_mic = utils::findChild<ui::ImageView*>(button_ai_speaking, "image");
            if (button_ai_speaking && spine && button_mic) {

                spine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                spine->playSequence(creatSequenceName, loop);
                spine->setContentSize(button_mic->getContentSize());
                spine->setScale(0.6);
                spine->setName("spine");
                Size button_ai_speakingSize = button_ai_speaking->getContentSize();

                // set vi tri theo ti le size cua button ai_speakin, do spine recoding design khong ro rang vi tri
                int scalePointX = button_ai_speakingSize.width / 150;
                int scalePointY = button_ai_speakingSize.width / 120;
                spine->setPosition(Vec2(30 * scalePointX, 18 * scalePointY));
                button_ai_speaking->addChild(spine);
                button_mic->setOpacity(0);
            }
            std::string audioPath = folderPath + "/Sound_Animation_Mic_icon.mp3";
            if (FileUtils::getInstance()->isFileExist(audioPath) && playAudio)
            {
                HSAudioEngine::getInstance()->playEffect(audioPath);
            }
        }
    }
}
void MSSidebar::SetBackGroundButtonPostion(ms::HomeTab tabs)
{
    auto button_background = utils::findChild(this, "button_background");
    button_background->setVisible(true);
    Button* button = nullptr;
    switch (tabs)
    {
        case ms::STORIES:
        {
            button = utils::findChild<Button*>(this, "button_stories");

            break;
        }
        case ms::LESSONS:
        {
            button = utils::findChild<Button*>(this, "button_lessons");
            break;
        }
        case ms::MATERIAL:
        {
            button = utils::findChild<Button*>(this, "button_material");
            break;
        }
        case ms::MATERIAL_STORIES:
        {
            button = utils::findChild<Button*>(this, "button_material");
            break;
        }
        case ms::MATERIAL_PHONICS:
        {
            button = utils::findChild<Button*>(this, "button_material");
            break;
        }
        case ms::SUBSCRIPTIONS:
        {
            button = utils::findChild<Button*>(this, "button_subscriptions");
            break;
        }
        case ms::FREE:
        {
            button = utils::findChild<Button*>(this, "button_free");
            break;
        }
        case ms::SETTINGS:
        {
            button = utils::findChild<Button*>(this, "button_settings");
            break;
        }
        case ms::ACCOUNT:
        {
            button = utils::findChild<Button*>(this, "button_account");
            break;
        }
        case ms::PROMOTION:
        {
            button = utils::findChild<Button*>(this, "button_promo");
            break;
        }
        case ms::AUDIOBOOKS:
        {
            button = utils::findChild<Button*>(this, "button_audiobooks");
            break;
        }
        case ms::FAVOURITE:
        {
            button = utils::findChild<Button*>(this, "button_stories");
            break;
        }
        case ms::LICENSE:
        {
            button = utils::findChild<Button*>(this, "button_license");
            break;
        }
        case ms::LESSONS_PHONIC:
        {
            button = utils::findChild<Button*>(this, "button_lessons");
            break;
        }
        case ms::LESSONS_COMPREHENSION:
        {
            button = utils::findChild<Button*>(this, "button_lessons");
            break;
        }
        case ms::AI_SPEAKING:
        {
            button = utils::findChild<Button*>(this, "button_ai_speaking");
            break;
        }
        default:
            break;
    }
    if (button)
    {
        button_background->setAnchorPoint(button->getAnchorPoint());
        button_background->setPosition(button->getPosition());
        button_background->setContentSize(button->getContentSize());
    }
}

void MSSidebar::showProfileMenu()
{
    if (is_show_profile_menu) return;
    _is_animating = true;
    auto overlay = utils::findChild(this, "overlay_profile");
    overlay->setOpacity(0);
    overlay->setVisible(true);
    overlay->runAction(FadeTo::create(0.5, 220));
    auto panel_choose = utils::findChild(this, "panel_choose");
    is_show_profile_menu = true;
    panel_choose->setVisible(true);
    panel_choose->setScale(0);
    panel_choose->setLocalZOrder(100);
    panel_choose->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.5f, 1)), CallFunc::create([=] {
        _is_animating = false;
    }), NULL));
}

void MSSidebar::hideProfileMenu()
{
    _is_animating = true;
    auto panel_choose = utils::findChild(this, "panel_choose");
    if (!is_show_profile_menu) return;
    auto overlay = utils::findChild(this, "overlay_profile");
    overlay->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([this](Node * n) {
        n->setVisible(false);
    }), nullptr));
    panel_choose->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=] {
        panel_choose->setVisible(false);
        _is_animating = false;
        is_show_profile_menu = false;
    }), NULL));
}

void MSSidebar::didLoadFromCSB() {
    _delegate = nullptr;
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        button_gaf.insert({ "button_ai_speaking", "" });
    }
    else
    {
        button_gaf.insert({ "button_material", "worksheets/worksheets.gaf" });

    }
    auto notch = MJ_PLATFORM_CONFIG.getNotchHeight();
    auto padding = utils::findChild<ui::Layout *>(this, "black_padding");
    padding->setContentSize(Size(notch, padding->getContentSize().height));
    
    auto content = utils::findChild<ui::Layout *>(this, "content");
    content->setPosition(Point(notch, 0));
    
//    if(CONFIG_MANAGER->isVietnameseStories()){
//        utils::findChild<ImageView*>(content, "Image_2")->setClippingEnabled(true) ;
//    }
    
    if (shouldShowQR()) {
        utils::findChild(this, "button_qr")->setVisible(true);
    } else {
        utils::findChild(this, "button_qr")->setVisible(false);
    }

    auto bt = utils::findChild<ui::Button *>(this, "button_free");
    if (CONFIG_MANAGER->isVietnameseStories()) {
        if (MJDEFAULT->getBoolForKey("show_hoc_van",false)){
            auto btn_lessson = utils::findChild(this, "button_lessons");
            btn_lessson->setVisible(true);
            btn_lessson->setPosition(btn_lessson->getPosition() - Point(0, 120));
        }else{
            utils::findChild(this, "button_lessons")->setVisible(false);
            bt->setPosition(bt->getPosition() + Point(0, 120));
        }
        auto bt_audio = utils::findChild(this, "button_audiobooks");
        bt_audio->setPosition(bt_audio->getPosition() + Point(0, 120));
        //bt->setPosition(bt->getPosition() + Point(0, 120));
    }
    
    if (CONFIG_MANAGER->isVietnameseStories() || MJDEFAULT->getBoolForKey(key_feature_worksheet, true) == false) {
        // code cũ
        if (!CONFIG_MANAGER->isAppActive()) {
            if (FREE_STORY_MANAGER->checkFreeTimeAvailable() || CONFIG_MANAGER->isTest7Day()) {
                bt->setName("button_favourite");
                bt->getChildByName("sidebar.des.free")->setName("sidebar.des.favourite");
                bt->addClickEventListener(CC_CALLBACK_1(MSSidebar::onFavourite, this));
            }
        } else {
            bt->setName("button_favourite");
            bt->getChildByName("sidebar.des.free")->setName("sidebar.des.favourite");
            bt->addClickEventListener(CC_CALLBACK_1(MSSidebar::onFavourite, this));
        }
    } else {
        // Nếu là MS thì:
        //      - ẩn button Library(favourite) và button QR đi
        //      - nếu là bản trial thì hiện button Free và button Worksheet ở dưới (vị trí của button QR cũ)
        //      - nếu là bản còn hạn sử dụng thì ẩn button Free đi, thay vào vị trí đó là button Worksheet
        if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        {
            if (MJDEFAULT->getBoolForKey(key_feature_worksheet, true)) {
                if (CONFIG_MANAGER->isAppActive()) {
                    bt->setVisible(false);

                }
                if (auto button_ai_speaking = utils::findChild<ui::Button*>(this, "button_ai_speaking")) {
                    button_ai_speaking->setVisible(true);
                    //button_ai_speaking->setPosition(Vec2(button_ai_speaking->getPosition().x, button_ai_speaking->getPosition().y + 120));
                    if (MJDEFAULT->getBoolForKey(key_feature_ai_speaking, true)) {
                        auto newGaf = GAFWrapper::createGAF("gaf/sidebar/new/new.gaf");
                        newGaf->setPosition(Vec2(button_ai_speaking->getContentSize() / 2) + Vec2(68, 10));
                        newGaf->playSequence("idle");
                        newGaf->setName("new_gaf");
                        newGaf->setAnchorPoint(Vec2(0.5, 0.5));
                        newGaf->setScale(1.1);
                        button_ai_speaking->addChild(newGaf);
                        ResetAndCreatSpineForAiSpeakingButton("idle1", false, false);
                    }
                }
            }
        }
        else {
            if (MJDEFAULT->getBoolForKey(key_feature_worksheet, true)) {
                bt->setVisible(false);
                bt->setOpacity(0);
                if (auto button_material = utils::findChild<ui::Button*>(this, "button_material")) {
                    button_material->setVisible(true);
                    button_material->setPosition(bt->getPosition());
                }
            }
            else {
                if (auto button_material = utils::findChild<ui::Button*>(this, "button_material")) {
                    button_material->setVisible(false);
                }
            }
        }
    }
    
    if (!CONFIG_MANAGER->isLicenceEnable() || MJDEFAULT->getBoolForKey(key_is_submit, false) || !MJDEFAULT->getBoolForKey("is_license_feature_enable", false) || CONFIG_MANAGER->isVietnameseStories()) {
        auto btlicense = utils::findChild(this, "button_license");
        btlicense->removeFromParent();
        auto bt_subscribe = utils::findChild(this, "button_subscriptions");
        bt_subscribe->setPosition(bt_subscribe->getPosition() + Point(0, -120));
    }
    
    // buttons
    auto button_account = utils::findChild<ui::Button *>(this, "button_account");
    auto button_settings = utils::findChild<ui::Button *>(this, "button_settings");
    auto button_subscriptions = utils::findChild<ui::Button *>(this, "button_subscriptions");
    
    button_account->setPosition(button_settings->getPosition());
    button_settings->setPosition(button_subscriptions->getPosition());
    button_subscriptions->setPosition(button_subscriptions->getPosition() + Point(0, 120));
    
    auto button_license = utils::findChild<ui::Button *>(this, "button_license");
    if (button_license)
    {
        button_license->setPosition(button_subscriptions->getPosition());
    }
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        auto button_free = utils::findChild<ui::Button*>(this, "button_free");
        if (button_free)
        {
            button_free->setPosition(button_free->getPosition() + Point(0, -120));
        }
    }
    
    // chưa có profile được chọn
    auto a = MJDEFAULT->getIntegerForKey(key_current_profile_id);
    if (MJDEFAULT->getIntegerForKey(key_current_profile_id, 0) == 0)
    {
        utils::findChild(this, "button_lock")->setVisible(true);
        utils::findChild(this, "button_profile")->setVisible(false);
    }
    else
    {
        utils::findChild(this, "button_lock")->setVisible(false);
        utils::findChild(this, "button_profile")->setVisible(true);
        auto profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id));
        loadDefaultAvatar(profile);
        utils::findChild<ui::Text*>(this, "sidebar.profile.name")->setString(profile.name);
    }

    this->setContentSize(Size(padding->getContentSize().width + content->getContentSize().width, this->getContentSize().height));
    
    for (auto p : button_gaf) {
        auto button = utils::findChild<ui::Button *>(this, p.first);
        if (!button) {
            continue;
        }
        if (!MJDEFAULT->getBoolForKey("show_hoc_van",false)){
            if (CONFIG_MANAGER->isVietnameseStories() && std::string(p.first) == "button_lessons"){
                continue;
            }
        }
        if (p.first == "button_qr") {
            if (shouldShowQR())
                _top_buttons.push_back(button);
        } else if (p.first == "button_free") {
            bool debugEnable = false;
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            debugEnable = true;
#endif
            // hieunt: nếu không phải MS hoặc là bản trial
            if (CONFIG_MANAGER->isVietnameseStories() || !CONFIG_MANAGER->isAppActive()) {
                button->setVisible(false);
                //_top_buttons.push_back(button);
            }
        } else {
            _top_buttons.push_back(button);
        }
        
        button->setPosition(button->getPosition() + Point(0, 120));
        
        if (p.second.empty()) {
            continue;
        }
        
        auto gaf = GAFWrapper::createGAF("gaf/sidebar/" + p.second);
        auto image = button->getChildByName("image");
        gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
        gaf->setName("gaf");
        if (p.first == "button_material") {
            gaf->setPosition(image->getPosition() + Point(0, 11));
        } else {
            gaf->setScale(1.5);
            gaf->setPosition(image->getPosition() + Point(0, 5));
        }
        button->addChild(gaf);
        gaf->playSequence("idle");
        image->removeFromParent();
    }
    
    utils::findChild(this, "button_languages")->setVisible(false);
    
    is_showing_languages = false;
    changeTextDisplayLang();
    for (auto button : _top_buttons) {
        if (!button) {
            continue;
        }
        if (button->getName() == "button_stories") {
            this->onStories(button);
            break;
        }
    }
    
    utils::findChild(this, "noti_img")->setVisible(false);
    utils::findChild(this, "noti_img_lock")->setVisible(false);
    
    auto list_noti = StoryDataModel::getInstance()->getAllMessage();
    auto num_new_noti = 0;
    for (int i = 0; i < (int) list_noti.size(); i++) {
        if (XHSLib->checkMessageUnRead(list_noti[i].asValueMap()["id"].asInt())) {
            num_new_noti++;
        }
    }

    utils::findChild(this, "noti_img_lock")->setVisible(num_new_noti > 0);
    utils::findChild<ui::Text *>(this, "noti_lb_lock")->setString(StringUtils::toString(num_new_noti));
    
    auto show_noti = EventListenerCustom::create("mjstory.shownoti", [=](EventCustom *event_custom) {
        int *edata = (int *) event_custom->getUserData();
        utils::findChild(this, "noti_img")->setVisible(true);
        utils::findChild<ui::Text *>(this, "noti_lb")->setString(StringUtils::toString(*edata));
        utils::findChild(this, "noti_img_lock")->setVisible(true);
        utils::findChild<ui::Text *>(this, "noti_lb_lock")->setString(StringUtils::toString(*edata));
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(show_noti, this);
    
    auto hide_noti = EventListenerCustom::create("mjstory.hidenoti", [=](EventCustom *event_custom) {
        utils::findChild(this, "noti_img")->setVisible(false);
        utils::findChild(this, "noti_img_lock")->setVisible(false);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(hide_noti, this);
    
    auto animate_popup = EventListenerCustom::create("mjstory.animate.popup", [=](EventCustom * event_custom) {
        auto btn_lock = utils::findChild(this, "button_lock");
        auto btn_profile = utils::findChild(this, "button_profile");
        if (btn_lock->isVisible())
        {
            btn_lock->runAction(Sequence::create(ScaleTo::create(0.15, 1.2), EaseElasticOut::create(ScaleTo::create(1, 1)), nullptr));
        }
        if (btn_profile->isVisible())
        {
            btn_profile->runAction(Sequence::create(ScaleTo::create(0.15, 1.2), EaseElasticOut::create(ScaleTo::create(1, 1)), nullptr));
        }
//        utils::findChild(this, "button_lock")->runAction(Sequence::create(TintTo::create(0.5, Color3B::GREEN), DelayTime::create(0.5), TintTo::create(0.5, Color3B::WHITE), NULL));
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(animate_popup, this);
    
    auto listener_welcome = EventListenerCustom::create(key_event_welcome_popup_close, [=](EventCustom * e) {
        auto type = reinterpret_cast<int *>(e->getUserData());
        if (* type == 2) {
            if (FREE_STORY_MANAGER->checkFreeTimeAvailable() || CONFIG_MANAGER->isTest7Day()) {
            } else {
                this->onFree(utils::findChild(this, "button_free"));
            }
        } else {
            CCLOG("go to subscribe page");
            MJDEFAULT->setStringForKey(key_source_subscribe, "welcome");
            MJDEFAULT->setBoolForKey(key_click_shop_type_popup, true);
            this->onSubscriptions(utils::findChild(this, "button_subscriptions"));
        }
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_welcome, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_subscribe_page, [=](EventCustom *) {
        if (MJDEFAULT->getStringForKey(key_source_subscribe) == "")
        {
            std::string source = "";
            auto freeOption = MJDEFAULT->getIntegerForKey(key_free_content_page_option);
            if(APPURCHASE_MANAGER->isSkipUser())
            {
                if(freeOption == (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
                {
                    source = "ob_demo_popup_lessons_free_user_14";
                }
                else
                {
                    source = "ob_demo_popup_lessons_free_user_7";
                }
            }
            else
            {
                source = "ob_discover_10_lessons_free_user_7";
                
                if(freeOption == (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
                {
                    source = "ob_discover_10_lessons_free_user_14";
                }
            }

            MJDEFAULT->setStringForKey(key_source_subscribe, source);
        }
        
        MJDEFAULT->setBoolForKey(key_click_shop_type_popup, true);
        this->onSubscriptions(utils::findChild(this, "button_subscriptions"));
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_lessons_page, [=](EventCustom *) {
        this->onLessons(utils::findChild(this, "button_lessons"));
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_audiobooks_page, [=](EventCustom *) {
        this->onAudiobooks(utils::findChild(this, "button_audiobooks"));
    }), this);
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    auto back_from_fullscreen_popup_listener = EventListenerCustom::create(key_back_from_fullscreenpopup, [=](EventCustom *) {
        if (_saved_button == nullptr)
            return;
        auto button_name = _saved_button->getName();
        if (button_name == "button_stories") {
            this->onStories(_saved_button);
            return;
        }
        
        if (button_name == "button_audiobooks") {
            this->onAudiobooks(_saved_button);
            return;
        }
        
        if (button_name == "button_lessons") {
            this->onLessons(_saved_button);
            return;
        }
        
        if (button_name == "button_free") {
            this->onFree(_saved_button);
            return;
        }
        if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        {
            if (button_name == "button_ai_speaking") {
                this->onAISpeaking(_saved_button);
                return;
            }
        }
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(back_from_fullscreen_popup_listener, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_app_become_purchased, [this](EventCustom * e) {
        // Nếu là MS thì bỏ nút Favourite(Library) đi
        if (CONFIG_MANAGER->isVietnameseStories() || MJDEFAULT->getBoolForKey(key_feature_worksheet, true) == false) {
            //đổi nút free thành nút favourite
            auto bt = utils::findChild<ui::Button *>(this, "button_free");
            if (bt) {
                if (_saved_button && _saved_button->getName() == "button_free") {
                    this->onFavourite(bt);
                }
                bt->setName("button_favourite");
                bt->getChildByName("sidebar.des.free")->setName("sidebar.des.favourite");
                bt->addClickEventListener(CC_CALLBACK_1(MSSidebar::onFavourite, this));
                this->changeTextDisplayLang();
                
                auto gaf = GAFWrapper::createGAF("gaf/sidebar/favourite/favourite.gaf");
                auto image = bt->getChildByName("gaf");
                gaf->setPosition(image->getPosition() + Point(0, 5));
                gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
                gaf->setName("gaf");
                gaf->setScale(1.5);
                bt->removeChildByName("gaf");
                gaf->playSequence("idle");
                bt->addChild(gaf);
            }
        }
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_gradebox_show, [=](EventCustom * e) {
        auto overlay = ui::Layout::create();
        overlay->setContentSize(this->getContentSize());
        overlay->setName("sidebar_overlay");
        overlay->setTouchEnabled(true);
        overlay->setEnabled(true);
        overlay->setBackGroundColor(Color3B::BLACK);
        overlay->setBackGroundColorOpacity(230);
        overlay->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        overlay->setOpacity(0);
        overlay->runAction(FadeTo::create(0.5, 225));
        overlay->addClickEventListener([](Ref * sender) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_gradebox_tap_from_sidebar);
        });
        this->addChild(overlay, 10000);
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_gradebox_hide, [=](EventCustom * e) {
        auto overlay = this->getChildByName("sidebar_overlay");
        if (!overlay) {
            return;
        }
        overlay->runAction(Sequence::create(DelayTime::create(0.2), FadeOut::create(0.25), RemoveSelf::create(), NULL));
    }), this);
    
    // add new icon on phonics
    if(MJDEFAULT->getBoolForKey(key_feature_phonics,true) && !CONFIG_MANAGER->isVietnameseStories())
    {
        if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        {
            auto button_lessons = utils::findChild(this, "button_lessons");
            //auto new_icon = ui::ImageView::create("mjstory/new_icon_3.png");
            //new_icon->setScale(0.4);
            //new_icon->setPosition(Vec2(button_lessons->getContentSize() / 2) + Vec2(20, 32));
            //button_lessons->addChild(new_icon);

        }
        else {
            if (MJDEFAULT->getBoolForKey(key_feature_worksheet, true)) {
                if (auto button_material = utils::findChild(this, "button_material")) {
                    auto materialGaf = GAFWrapper::createGAF("gaf/sidebar/new/new.gaf");
                    materialGaf->setPosition(Vec2(button_material->getContentSize() / 2) + Vec2(68, 10));
                    materialGaf->playSequence("idle");
                    materialGaf->setName("material_new_button_gaf");
                    materialGaf->setAnchorPoint(Vec2(0.5, 0.5));
                    materialGaf->setScale(1.1);
                    button_material->addChild(materialGaf);
                }
            }
            else {
                auto button_lessons = utils::findChild(this, "button_lessons");
                auto new_icon = ui::ImageView::create("mjstory/new_icon_3.png");
                //new_icon->setScale(0.4);
                new_icon->setPosition(Vec2(button_lessons->getContentSize() / 2) + Vec2(20, 32));
                button_lessons->addChild(new_icon);
            }
        }
    }
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        auto button_lessons = utils::findChild(this, "button_lessons");
        auto new_icon = ui::ImageView::create("mjstory/new_icon_vn.png");
        if (LANGUAGE_MANAGER->getDisplayLangId() != LANGUAGE_VN_CODE){
            new_icon = ui::ImageView::create("mjstory/new_icon_vn.png");
        }
        new_icon->setScale(0.5);
        new_icon->setPosition(Vec2(button_lessons->getContentSize()/2) + Vec2(20,32));
        button_lessons->addChild(new_icon);
    }
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("event.change.current.profile", [=](EventCustom * event) {
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            
            auto profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id));
            
            loadDefaultAvatar(profile);
            
            if (auto name =  utils::findChild<ui::Text*>(Director::getInstance()->getRunningScene(), "sidebar.profile.name"))
            {
                name->setString(profile.name);
            }
            
        });
    });
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        showHidenMaterialButton(false);
    }
}

void MSSidebar::enableQR() {
    // hieunt: không rõ vì sao hàm này không gọi shouldShowQR(), bây giờ ở MS nút QR bị ẩn nên sửa lại...
    if (!CONFIG_MANAGER->isVietnameseStories()) {
        if (this->shouldShowQR() == false) {
            return;
        }
    }
    
    auto button_qr = utils::findChild<ui::Button *>(this, "button_qr");
    if (!button_qr->isVisible()) {
        button_qr->setVisible(true);
        _top_buttons.push_back(button_qr);
    }
}

void MSSidebar::onEnter()
{
    ui::Layout::onEnter();
}

void MSSidebar::onEnterTransitionDidFinish() {
    ui::Layout::onEnterTransitionDidFinish();
    this->scheduleOnce([](float) {
        if (MJDEFAULT->getBoolForKey(key_event_go_to_subscribe_page, false)) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_subscribe_page);
            MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, false);
            return;
        }
        
        if (MJDEFAULT->getBoolForKey(key_event_go_to_free_page, false)) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_free_page);
            MJDEFAULT->setBoolForKey(key_event_go_to_free_page, false);
            return;
        }
        
        if (MJDEFAULT->getBoolForKey(key_event_go_to_lessons_page, false)) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_lessons_page);
            MJDEFAULT->setBoolForKey(key_event_go_to_lessons_page, false);
            return;
        }
        
        if (MJDEFAULT->getBoolForKey(key_event_go_to_audiobooks_page, false)) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_audiobooks_page);
            MJDEFAULT->setBoolForKey(key_event_go_to_audiobooks_page, false);
            return;
        }
    }, 0.1, "delay_sidebar_transition_finish");
}

ui::Widget::ccWidgetClickCallback MSSidebar::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSSidebar, onLessons),
        CLICK_MAP(MSSidebar, onLanguages),
        CLICK_MAP(MSSidebar, onSettings),
        CLICK_MAP(MSSidebar, onStories),
        CLICK_MAP(MSSidebar, onSubscriptions),
        CLICK_MAP(MSSidebar, onAudiobooks),
        CLICK_MAP(MSSidebar, onFree),
        CLICK_MAP(MSSidebar, onPromotions),
        CLICK_MAP(MSSidebar, onFavourite),
        CLICK_MAP(MSSidebar, onLock),
        CLICK_MAP(MSSidebar, onClose),
        CLICK_MAP(MSSidebar, onLicense),
        CLICK_MAP(MSSidebar, onQR),
        CLICK_MAP(MSSidebar, onProfile),
        CLICK_MAP(MSSidebar, onAccount),
        CLICK_MAP(MSSidebar, onSwitchProfile),
        CLICK_MAP(MSSidebar, onMaterial),
        CLICK_MAP(MSSidebar, onSupport),

    };
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        function_map.insert(CLICK_MAP(MSSidebar, onAISpeaking));
    }

    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    
    return nullptr;
}

#pragma mark - Force open layouts
void MSSidebar::onPromotionsForce()
{
    this->onPromotions(utils::findChild(this, "button_promo"));
}

void MSSidebar::onStoriesForce() {
    for (auto button : _top_buttons) {
        if (!button) {
            continue;
        }
        if (button->getName() == "button_stories") {
            this->onStories(button);
            break;
        }
    }
}

void MSSidebar::onLessonsForce()
{
    this->onLessons(utils::findChild(this, "button_lessons"));
}

void MSSidebar::onAudiobooksForce()
{
    this->onAudiobooks(utils::findChild(this, "button_audiobooks"));
}

void MSSidebar::onQRForce()
{
    this->onQR(utils::findChild(this, "button_qr"));
}
void MSSidebar::onAISpeakingForce()
{
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        this->onAISpeaking(utils::findChild(this, "button_ai_speaking"));
    }
}
void MSSidebar::onMaterialForce()
{
    this->onMaterial(utils::findChild(this, "button_material"));
}

void MSSidebar::onSubscriptionsForce() {
    MJDEFAULT->setBoolForKey(key_click_shop_type_popup, true);
    this->onSubscriptions(utils::findChild(this, "button_subscriptions"));
}

void MSSidebar::onParentForce()
{
    this->onLock(utils::findChild(this, "button_parent"));
}

void MSSidebar::onSettingsForce()
{
    this->onSettings(utils::findChild(this, "button_settings"));
}

void MSSidebar::onAccountForce(bool showCalculator)
{
    if (showCalculator)
    {
        MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {
            _delegate ? _delegate->onSelectedTab(ms::HomeTab::ACCOUNT, (ms::MovingDirection)0) : (void) nullptr;
                 
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                Close(nullptr);
            }
            else {
                onClose(nullptr);
            }
        });
        
        return;
    }
    
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::ACCOUNT, (ms::MovingDirection)0) : (void) nullptr;
    
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        Close(nullptr);
    }
    else {
        onClose(nullptr);
    }
}

#pragma mark - On click buttons
void MSSidebar::onSignInF2P()
{
    auto loginScreen = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::HomeLogin, ms::Authen::StartRegisterFrom::Default);
    F2P_MANAGER->setCanMergeProfileKey(true);
    APPopupBase::sAnimationMove(loginScreen, APPopupBase::AnimationDirection::LEFT);
    Director::getInstance()->getRunningScene()->addChild(loginScreen, INT_MAX - 1);
}
void MSSidebar::onShowParentTaskBarF2P()
{
    // TODO: kiểm tra điều kiện hiển thị popup Lấy thông tin người dùng
    APAccountBaseV3::checkShowingEnterEmailPopup(this);
    // -- end --

    this->setLocalZOrder(7);
    mj::helper::playButtonFX();
    MJDEFAULT->setBoolForKey(key_show_parent_lock, false);

    auto overlay = utils::findChild(this, "overlay");
    overlay->setOpacity(0);
    overlay->setVisible(true);
    overlay->runAction(FadeTo::create(0.5, 220));
    overlay->removeAllChildren();

    auto lb_arrow = ui::Text::create();
    lb_arrow->setString("←");
    lb_arrow->setFontSize(100);
    lb_arrow->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    lb_arrow->setPosition(Point(Director::getInstance()->getVisibleSize().width / 2 + MJ_PLATFORM_CONFIG.getNotchHeight(), 420));
    lb_arrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Point(25, 0)), MoveBy::create(0.5, Point(-25, 0)), NULL)));
    overlay->addChild(lb_arrow);

    _is_animating = true;
    auto button_close = utils::findChild<ui::Button*>(this, "button_close");
    button_close->setVisible(true);

    auto button_settings = utils::findChild<ui::Button*>(this, "button_settings");
    button_settings->setVisible(true);

    auto button_subscriptions = utils::findChild<ui::Button*>(this, "button_subscriptions");
    button_subscriptions->setVisible(true);

    // ở MS thì hiện button account, VM thì không
    auto button_account = utils::findChild<ui::Button*>(this, "button_account");
    button_account->setVisible(true);
    if (APPURCHASE_MANAGER->isSkipUser())
    {
        if (auto button_account_text = utils::findChild<ui::Text*>(this, "sidebar.des.account")) {
            auto text = LANGUAGE_MANAGER->getDisplayTextByKey("ap_login_button_title");
            button_account_text->setText(text);
        }
        button_subscriptions->setVisible(false);
    }
    else {
        if (auto button_account_text = utils::findChild<ui::Text*>(this, "sidebar.des.account")) {
            auto text = LANGUAGE_MANAGER->getDisplayTextByKey("sidebar.des.account");
            button_account_text->setText(text);
        }
        button_subscriptions->setVisible(true);
    }
    auto button_promo = utils::findChild<ui::Button*>(this, "button_promo");
    auto button_material = utils::findChild<ui::Button*>(this, "button_material");
    auto button_support = utils::findChild<ui::Button*>(this, "button_support");
    button_support->setVisible(true);
    button_promo->setVisible(true);
    button_promo->setPosition(button_subscriptions->getPosition() + Vec2(0, 280));
    auto button_license = utils::findChild<ui::Button*>(this, "button_license");
    if (button_license)
        button_license->setVisible(true);

    if (_bottom_buttons.empty()) {
        _bottom_buttons.push_back(button_close);
        _bottom_buttons.push_back(button_promo);
        _bottom_buttons.push_back(button_settings);
        _bottom_buttons.push_back(button_account);
        if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
            _bottom_buttons.push_back(button_material);
        }
        if (button_license)
            _bottom_buttons.push_back(button_license);
        _bottom_buttons.push_back(button_subscriptions);
        _bottom_buttons.push_back(button_support);
    }
    
    if(APPURCHASE_MANAGER->isLifeTimeUser())
    {
        button_subscriptions->setVisible(false);
    }

    auto button_parent = utils::findChild<ui::Button*>(this, "button_lock");
    if (!button_parent->isVisible())
    {
        button_parent = utils::findChild<ui::Button*>(this, "button_profile");
    }
    button_parent->runAction(Sequence::create(FadeOut::create(0.1), CallFuncN::create([](Node* n) {
        n->setVisible(false);
        }), DelayTime::create(0.5), CallFunc::create([this] {
            this->_is_animating = false;
            }), nullptr));

    for (auto i = 0; i < _bottom_buttons.size(); ++i) {
        auto bt = _bottom_buttons[i];
        bt->setOpacity(0);
        bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, 60))), NULL), NULL));
    }

    for (auto i = 0; i < _top_buttons.size(); ++i) {
        auto bt = _top_buttons[i];
        bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeOut::create(0.25), EaseQuadraticActionOut::create(MoveBy::create(0.25, Point(0, 60))), nullptr), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
            n->setVisible(false);
            }), NULL));
    }

    auto bt_background = utils::findChild(this, "button_background");
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
        if (bt_background)
            bt_background->runAction(Sequence::create(DelayTime::create(0.2), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, -60))), CallFuncN::create([](Node* n) {
            n->setVisible(true);
                }), NULL), NULL));
        showHidenMaterialButton(true);
    }
    else {
        bt_background->runAction(Sequence::create(DelayTime::create(0.2), Spawn::create(FadeOut::create(0.25), EaseQuadraticActionOut::create(MoveBy::create(0.25, Point(0, 60))), nullptr), DelayTime::create(0.25), CallFuncN::create([](Node* n) {
            n->setVisible(false);
            }), NULL));
    }
}
void MSSidebar::onMaterial(cocos2d::Ref *sender) {
    if (!MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        if (_current_button == sender)
            return;
    }
    
    if (_is_animating) {
        return;
    }
    
    MJDEFAULT->setStringForKey("key_sidebar_tab_select", "material");
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)){
    auto overlay = utils::findChild(this, "overlay");
    if (overlay->isVisible())
    {
        overlay->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([this](Node* n) {
            n->setVisible(false);
            this->setLocalZOrder(10);
            }), nullptr));
    }
        
    }
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::MATERIAL, (ms::MovingDirection)d) : (void) nullptr;

}

void MSSidebar::onAccount(cocos2d::Ref *sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
   
    if (APPURCHASE_MANAGER->isSkipUser())
    {
        MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [=]() {
            onSignInF2P();
            });
    }
    else {
        this->onAccountForce();
    }
}

void MSSidebar::onStories(cocos2d::Ref *sender) {
    MJDEFAULT->setStringForKey(key_read_from, "new");
    if (_current_button == sender)
        return;

    if (_is_animating) {
        return;
    }
    
    MJDEFAULT->setStringForKey("key_sidebar_tab_select", "stories");
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::STORIES);
    }
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::STORIES, (ms::MovingDirection)d) : (void) nullptr;
}

void MSSidebar::onSupport(cocos2d::Ref* sender) {
    auto countryCode = MJDEFAULT->getStringForKey(key_country_code, "US");
    countryCode = "TH";
    if (countryCode == "VN") {
        Application::getInstance()->openURL("https://zalo.me/396675150817766153");
    }
    else {
        if (countryCode == "TH" || countryCode == "MY") {
            std::string phoneNumber = countryCode == "TH" ? "6620385919" : "88335350";
            if (MJ_PLATFORM_CONFIG.phoneSupport(phoneNumber))
            {
                ///????
            }
            else
            {
                auto str = LANGUAGE_MANAGER->getTextByKeyContryCode("support.action.error");
                StoryAlert::showAlert("Telephone support: " + phoneNumber, "OK", "");
            }
        }
        else {
            if (MJ_PLATFORM_CONFIG.mailSupport(LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.email"))) {
            }
            else {
                auto str = LANGUAGE_MANAGER->getTextByKeyContryCode("support.action.error");
                StoryAlert::showAlert("Mail support: " + LANGUAGE_MANAGER->getDisplayTextByKey("key.support.info.email"), "OK", "");
            }
        }
    }
}

void MSSidebar::onLessons(cocos2d::Ref *sender) {
//    if (_current_button == sender)
//        return;
    
    if (_is_animating) {
        return;
    }
    
    MJDEFAULT->setStringForKey("key_sidebar_tab_select", "lessons");
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::LESSONS);

    }
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::LESSONS, (ms::MovingDirection)d) : (void) nullptr;
}

void MSSidebar::onSettings(cocos2d::Ref *sender) {

    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        Close(nullptr);
        _delegate ? _delegate->onSelectedTab(ms::HomeTab::SETTINGS, (ms::MovingDirection)0) : (void) nullptr;
    }
    else {
        if (APPURCHASE_MANAGER->isSkipUser()) {
            MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {
                _delegate ? _delegate->onSelectedTab(ms::HomeTab::SETTINGS, (ms::MovingDirection)0) : (void) nullptr;
                onClose(nullptr);
                });
        }
        else {
            _delegate ? _delegate->onSelectedTab(ms::HomeTab::SETTINGS, (ms::MovingDirection)0) : (void) nullptr;
            onClose(nullptr);
        }
    }

}

void MSSidebar::onPromotions(cocos2d::Ref *sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        Close(nullptr);
        _delegate ? _delegate->onSelectedTab(ms::HomeTab::PROMOTION, (ms::MovingDirection)0) : (void) nullptr;
    }
    else {
        _delegate ? _delegate->onSelectedTab(ms::HomeTab::PROMOTION, (ms::MovingDirection)0) : (void) nullptr;

        onClose(nullptr);
    }
}

void MSSidebar::onSubscriptions(cocos2d::Ref *sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::SUBSCRIPTIONS, (ms::MovingDirection)0) : (void) nullptr;
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        Close(nullptr);
    }
    else {
        onClose(nullptr);
    }
}

void MSSidebar::onLicense(cocos2d::Ref *sender) {
    if (_current_button == sender)
        return;    
    if (_is_animating) {
        return;
    }

    _delegate ? _delegate->onSelectedTab(ms::HomeTab::LICENSE, (ms::MovingDirection)0) : (void) nullptr;
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        Close(nullptr);
    }
    else {
        onClose(nullptr);
    }
}

void MSSidebar::onAudiobooks(cocos2d::Ref * sender) {
    MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "new");
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::AUDIOBOOKS);

    }
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::AUDIOBOOKS, (ms::MovingDirection)d) : (void) nullptr;
}

void MSSidebar::onLanguages(cocos2d::Ref *sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    
    moveBackground(reinterpret_cast<ui::Button *>(sender));
    is_showing_languages ? hideLanguages() : showLanguages();
}

void MSSidebar::onFree(Ref * sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::FREE);
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::FREE, (ms::MovingDirection)d) : (void) nullptr;
}

void MSSidebar::onQR(Ref * sender) {

    if (!NativeBridge::networkAvaiable()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        alert->setTag(1122);
        alert->setDelegate(this);
        return;
    }

//    auto pending_qr = MJDEFAULT->getStringForKey(key_qrcode_received, "");
    auto pending_qr = ms::analytics::Firebase::QR_DEEP_LINK;
    if (!pending_qr.empty()) {
        std::map<std::string, std::string> url_parse = mj::helper::url_parse(pending_qr);
        
        if (url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_MS) != std::string::npos)
        {
            std::string urlPart = url_parse[URL_ROOT_PART];
            
            if (urlPart.compare(DEF_DEEPLINK_PART_QRBOOK) == 0)
            {
                auto story_id = mj::helper::parseDynamicLinkQR(pending_qr);
                auto view = MSStoryAudioPlayer::createView(story_id);
                Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                ms::analytics::Firebase::QR_DEEP_LINK = "";
                
                return;
            }
        }
//        MJDEFAULT->setStringForKey(key_qrcode_received, "");
    }

    auto view = MMQRScan::createView();
    view->setName(mmqrscan_name);
    Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
}

void MSSidebar::onProfile(cocos2d::Ref * sender)
{
    showNumberNotification();
    
    if (_is_animating) {
        return;
    }
    mj::helper::playButtonFX();
    if (is_show_profile_menu)
    {
        hideProfileMenu();
        return;
    }
    if (APPURCHASE_MANAGER->isSkipUser())
    {
        onShowParentTaskBarF2P();
    }
    else {
        showProfileMenu();
    }
}

void MSSidebar::onSwitchProfile(cocos2d::Ref * sender)
{
    
    mj::helper::playButtonFX();
    auto profilewitch =	APProfileSwitch::createView();
    this->addChild(profilewitch);

    auto listener = EventListenerCustom::create(key_event_profile_switched, [this](EventCustom *)
    {
        auto profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
        
        loadDefaultAvatar(profile);

        utils::findChild<ui::Text*>(this, "sidebar.profile.name")->setString(profile.name);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(key_event_profile_switched);
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    if (is_show_profile_menu)
    {
        hideProfileMenu();
    }
}

void MSSidebar::onFavourite(Ref * sender) {
    if (_current_button == sender)
        return;
    
    if (_is_animating) {
        return;
    }
    
    auto d = moveBackground(reinterpret_cast<ui::Button *>(sender));
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        MJDEFAULT->setIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::FAVOURITE);

    }
    _delegate ? _delegate->onSelectedTab(ms::HomeTab::FAVOURITE, (ms::MovingDirection)d) : (void) nullptr;
}

void MSSidebar::onSelectLanguage(cocos2d::Ref * sender) {
    _current_language_code = reinterpret_cast<ui::Button *>(sender)->getName();
    auto info = languages_info.at(_current_language_code);
    auto language_button = utils::findChild(this, "button_languages");
    
    std::string flag_path, description_text;
    std::tie(flag_path, description_text) = info;
    
    language_button->getChildByName<ui::ImageView *>("flag")->loadTexture(flag_path);
    language_button->getChildByName<ui::Text *>("description")->setString(description_text);
    
    hideLanguages();
}

#pragma mark - StoryAlertDelegate
void MSSidebar::onAlertOK(cocos2d::Ref * sender)
{
    
}

void MSSidebar::onAlertClose(cocos2d::Ref * sender)
{
    
}

void MSSidebar::onAlertCancel(cocos2d::Ref * sender)
{
    if (auto alert = dynamic_cast<StoryAlert *>(sender))
    {
        if (alert->getTag() == 1122)
        {
            this->onQR(nullptr);
        }
    }
}

#pragma mark - Zzz
int MSSidebar::moveBackground(ui::Button * button) {
    if (button == nullptr) {
        return 0;
    }
    _is_animating = true;
    double distance = 0;
    if (_current_button) {
        auto current_gaf = _current_button->getChildByName<GAFWrapper *>("gaf");
        if (current_gaf)
            current_gaf->playSequence("action2");
        distance = button->getPositionY() - _current_button->getPositionY();

    }
    
    _saved_button = _current_button;
    _current_button = button;
    auto current_gaf = _current_button->getChildByName<GAFWrapper *>("gaf");
    if (current_gaf)
        current_gaf->playSequence("action");
    
    if (button_sound.count(button->getName())) {
        auto fx = button_sound.at(button->getName());
        HSAudioEngine::getInstance()->playEffect("gaf/sidebar/" + fx);
    }
    
    auto background = utils::findChild(this, "button_background");
    auto destination = button->getPosition();
    background->stopAllActions();
    background->runAction(Sequence::create(MoveTo::create(0.25, destination), CallFunc::create([this]() {
        _is_animating = false;
    }), nullptr));


    if (button->getName() == "button_material") {
        auto gaf = button->getChildByName<GAFWrapper*>("material_new_button_gaf");
        if (gaf) {
            gaf->playSequence("action");
            gaf->forceLoop(true);
        }
    }
    else {
        auto gaf = utils::findChild<GAFWrapper*>(this, "material_new_button_gaf");
        if (gaf) {
            gaf->playSequence("idle");
        }
    }
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        if (button->getName() == "button_ai_speaking") {
            auto gaf = button->getChildByName<GAFWrapper*>("new_gaf");
            //if (gaf) {
            //    gaf->playSequence("action");
            //    gaf->forceLoop(true);
            //}
            if (auto button_ai_speaking = utils::findChild<Button*>(this, "button_ai_speaking"))
            {

                if (auto spine_button_ai_speaking = utils::findChild<SpineWrapper*>(button_ai_speaking, "spine"))
                {
                    ResetAndCreatSpineForAiSpeakingButton("animation1", false, false);
                }
            }
        }
        else {
            auto gaf = utils::findChild<GAFWrapper*>(this, "new_gaf");
            if (gaf) {
                gaf->playSequence("idle");
            }
            if (auto button_ai_speaking = utils::findChild<Button*>(this, "button_ai_speaking"))
            {

                if (auto spine_button_ai_speaking = utils::findChild<SpineWrapper*>(button_ai_speaking, "spine"))
                {
                    ResetAndCreatSpineForAiSpeakingButton("idle1", false, false);
                }
            }

        }
    }
    
    // hieunt: căn chỉnh một chút với button Worksheet
    if (!CONFIG_MANAGER->isVietnameseStories()) {
        if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        {
            
        }
        else{
            if (button->getName() == "button_material" && LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                background->setAnchorPoint(Point(0.75f, 0.59f));
                background->setContentSize(Size(background->getContentSize().width, 140));
            }
            else {
                // các giá trị này được lấy theo file sidebar.csd
                background->setAnchorPoint(Point(0.75f, 0.5f));
                background->setContentSize(Size(background->getContentSize().width, 120));
            }
            
        }
        

    }
    
    if (distance == 0) {
        return 0;
    }
    
    if (distance > 0) {
        return 1;
    }
    
    return -1;
}

void MSSidebar::showLanguages() {
//    if (is_showing_languages) {
//        return;
//    }
//    is_showing_languages = true;
//    _language_buttons.clear();
//    auto counter = 0;
//    for (auto button : _buttons) {
////        button->setTouchEnabled(false);
//        button->setEnabled(false);
//        button->runAction(Sequence::create(DelayTime::create(0.1f * counter++), FadeOut::create(0.25), CallFunc::create([button] {
//            button->setVisible(false);
//        }), nullptr));
//    }
//
//    auto button_languages = utils::findChild<ui::Button *>(this, "button_languages");
//
//    counter = 1;
//    for (auto info : languages_info) {
//        auto lang_code = info.first;
//        if (lang_code == _current_language_code) {
//            continue;
//        }
//
//        std::string flag_path, description_text;
//        std::tie(flag_path, description_text) = info.second;
//
//        auto new_button = reinterpret_cast<ui::Button *>(button_languages->clone());
//        new_button->setVisible(false);
//        new_button->setPosition(Point(
//                                      button_languages->getPositionX(),
//                                      button_languages->getPositionY() - counter * button_languages->getContentSize().height));
//        new_button->getChildByName<ui::ImageView *>("flag")->loadTexture(flag_path);
//        new_button->getChildByName<ui::Text *>("description")->setString(description_text);
//        button_languages->getParent()->addChild(new_button);
//        new_button->setOpacity(0);
//        new_button->setVisible(true);
//        new_button->setName(info.first);
//        new_button->setEnabled(false);
//        new_button->runAction(Sequence::create(DelayTime::create(0.2 + 0.1 * counter++), FadeIn::create(0.25f), CallFunc::create([new_button] {
//            new_button->setEnabled(true);
//        }), nullptr));
//        new_button->addClickEventListener(CC_CALLBACK_1(MSSidebar::onSelectLanguage, this));
//        _language_buttons.push_back(new_button);
//    }
//
//    _delegate ? _delegate->onSelectedLanguage(true, _current_language_code) : (void) nullptr;
}

void MSSidebar::hideLanguages() {
//    if (!is_showing_languages) {
//        return;
//    }
//    is_showing_languages = false;
//    auto counter = 0;
//    for (auto button : _language_buttons) {
//        button->setEnabled(false);
//        button->runAction(Sequence::create(DelayTime::create(0.1f * counter++), FadeOut::create(0.25), CallFunc::create([button] {
//            button->removeFromParent();
//        }), nullptr));
//    }
//
//    counter = 0;
//    for (auto button : _buttons) {
//        button->setVisible(true);
//        button->setOpacity(0);
//        button->stopAllActions();
//        button->runAction(Sequence::create(DelayTime::create(0.2 + 0.1 * counter++), FadeIn::create(0.25f), CallFunc::create([button] {
//            button->setEnabled(true);
//        }), nullptr));
//    }
//
//    _delegate ? _delegate->onSelectedLanguage(false, _current_language_code) : (void) nullptr;
}

void MSSidebar::changeTextDisplayLang()
{
    _current_language_code = StoryLanguageManager::getLanguageCodeById(StoryLanguageManager::getCurrentLangId());
    auto lang_code_display = StoryLanguageManager::getLanguageCodeById(StoryLanguageManager::getDisplayLangId());
    languages_info = LANGUAGE_MANAGER->getLanguageStoriesInfo(lang_code_display);
    
    std::tuple<std::string, std::string> temp = languages_info.at(_current_language_code);
    std::string flag_path, description_text;
    std::tie(flag_path, description_text) = temp;
    utils::findChild<ui::ImageView *>(this, "flag")->loadTexture(flag_path);
    
    if(auto descriptionText = utils::findChild<ui::Text *>(this, "description"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  descriptionText->setFontName("fonts/leelawdb.ttf") : descriptionText->setFontName("fonts/Montserrat-Medium.ttf")  ;
        
        descriptionText->setString(description_text);
    }
        
    for (int i = 0; i < (int) list_name_des.size(); i++)
    {
        auto label = utils::findChild<ui::Text *>(this, list_name_des[i]);
        
        if (label)
        {
            (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  label->setFontName("fonts/leelawdb.ttf") : label->setFontName("fonts/Montserrat-Medium.ttf")  ;
            label->setString(LANGUAGE_MANAGER->getTextByKey(lang_code_display, list_name_des[i]));
            if (list_name_des[i] == "sidebar.des.lessons" && CONFIG_MANAGER->isVietnameseStories()){
                label->setString(LANGUAGE_MANAGER->getTextByKey(lang_code_display, "sidebar.des.lessons.vm"));
            }
        }
    }
    
    if(auto switchText = utils::findChild<Text*>(utils::findChild<Button*>(this, "button_switch"), "Text_4"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  switchText->setFontName("fonts/leelawdb.ttf") : switchText->setFontName("fonts/Montserrat-Medium.ttf")  ;

        switchText->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.change.title"));
    }
    
    if(auto parentButtonText = utils::findChild<Text*>(utils::findChild<Button*>(this, "button_parent"), "Text_4"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  parentButtonText->setFontName("fonts/leelawdb.ttf") : parentButtonText->setFontName("fonts/Montserrat-Medium.ttf")  ;

        parentButtonText->setString(LANGUAGE_MANAGER->getDisplayTextByKey("sidebar.des.parents"));
    }
    
    if(auto panel_choose = utils::findChild(this, "panel_choose"))
    {
        if(auto node = dynamic_cast<ui::ImageView*>(panel_choose))
        {
            node->setScale9Enabled(true);
            
            node->setContentSize(Size((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)?370.f:320.0f,panel_choose->getContentSize().height));
        }
    }
    
    // hieunt: căn chỉnh một chút với button Worksheet
    if (!CONFIG_MANAGER->isVietnameseStories()) {
        auto background = utils::findChild(this, "button_background");
        if (!MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
        {
            if (_current_button && _current_button->getName() == "button_material" && LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                background->setAnchorPoint(Point(0.75f, 0.59f));
                background->setContentSize(Size(background->getContentSize().width, 140));
            }
            else {
                // các giá trị này được lấy theo file sidebar.csd
                background->setAnchorPoint(Point(0.75f, 0.5f));
                background->setContentSize(Size(background->getContentSize().width, 120));
            }
        }

    }
}

void MSSidebar::onLock(cocos2d::Ref *sender) {
    if (_is_animating) {
        return;
    }
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        _onSaveUnlockButton = _current_button;
    }
    mj::helper::playButtonFX();
    
    if (is_show_profile_menu)
    {
        hideProfileMenu();
    }

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_parents);
    
    //    ap::api::LoadUpdate::call(true);
    
    MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {
        onShowParentTaskBarF2P();
    });
    
}

void MSSidebar::onClose(cocos2d::Ref * sender) {
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
        sender = _onSaveUnlockButton;
        Close(sender);
        //auto d = moveBackground(reinterpret_cast<ui::Button*>(sender));
        ms::HomeTab preTabUnclock = (ms::HomeTab)MJDEFAULT->getIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::LESSONS);
        _delegate ? _delegate->onSelectedTab(preTabUnclock, (ms::MovingDirection::UP)) : (void) nullptr;
    }
    else {
        if (_is_animating) {
            return;
        }
        mj::helper::playButtonFXClose();

        if (is_show_profile_menu)
        {
            hideProfileMenu();
            //HelperManager::GetInstance()->addSidebarBlack();
            return;
        }

        auto bt_stories = utils::findChild(this, "button_stories");
        if (bt_stories->getOpacity() == 255) {
            return;
        }

        MJDEFAULT->setBoolForKey(key_show_parent_lock, true);
        _is_animating = true;

        auto overlay = utils::findChild(this, "overlay");
        overlay->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([this](Node* n) {
            n->setVisible(false);
            this->setLocalZOrder(10);
            }), nullptr));

        for (auto i = 0; i < _bottom_buttons.size(); ++i) {
            auto bt = _bottom_buttons[i];
            bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeOut::create(0.25), EaseQuadraticActionOut::create(MoveBy::create(0.25, Point(0, -60))), nullptr), CallFuncN::create([](Node* n) {
                n->setVisible(false);
                }), NULL));
        }

        for (auto i = 0; i < _top_buttons.size(); ++i) {
            auto bt = _top_buttons[i];
            bt->setOpacity(0);
            bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, -60))), CallFuncN::create([](Node* n) {
                n->setVisible(true);
                }), NULL), NULL));
        }

        auto bt_background = utils::findChild(this, "button_background");
        bt_background->runAction(Sequence::create(DelayTime::create(0.2), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, -60))), CallFuncN::create([](Node* n) {
            n->setVisible(true);
            }), NULL), NULL));

        // chưa có profile được chọn
        if (MJDEFAULT->getIntegerForKey(key_current_profile_id, 0) == 0)
        {
            auto button_lock = utils::findChild(this, "button_lock");
            button_lock->setVisible(true);
            button_lock->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.1), CallFunc::create([this] {
                this->_is_animating = false;
                }), NULL));
            utils::findChild(this, "button_profile")->setVisible(false);
        }
        else
        {
            utils::findChild(this, "button_lock")->setVisible(false);
            auto button_profile = utils::findChild(this, "button_profile");
            button_profile->setVisible(true);
            button_profile->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.1), CallFunc::create([this] {
                this->_is_animating = false;
                }), NULL));
        }

        //if (auto node = dynamic_cast<Node*>(sender))
        //    {
        //        HelperManager::GetInstance()->addSidebarBlack();
        //    }
    }
}
void MSSidebar::Close(cocos2d::Ref* sender) {
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
        _saved_button = _current_button;
        _current_button = reinterpret_cast<ui::Button*>(sender);
        if (_is_animating) {
            return;
        }
        mj::helper::playButtonFXClose();

        if (is_show_profile_menu)
        {
            hideProfileMenu();
            //HelperManager::GetInstance()->addSidebarBlack();
            return;
        }
        if (auto button_ai_speaking = utils::findChild<ui::Button*>(this, "button_ai_speaking")) {
            button_ai_speaking->setVisible(true);
        }
        if (auto overlay = utils::findChild<ui::Button*>(this, "overlay")) {
            overlay->setVisible(false);
        }

        if (auto button_material = utils::findChild<ui::Button*>(this, "button_material")) {
            button_material->setVisible(false);
        }
        for (auto i = 0; i < _bottom_buttons.size(); ++i) {
            auto bt = _bottom_buttons[i];
            bt->setVisible(false);
            //bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, 60))), NULL), NULL));
        }
        auto bt_stories = utils::findChild(this, "button_stories");
        if (bt_stories->getOpacity() == 255) {
            return;
        }

        MJDEFAULT->setBoolForKey(key_show_parent_lock, true);
        _is_animating = true;

        auto overlay = utils::findChild(this, "overlay");
        overlay->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([this](Node* n) {
            n->setVisible(false);
            this->setLocalZOrder(10);
            }), nullptr));

        for (auto i = 0; i < _bottom_buttons.size(); ++i) {
            auto bt = _bottom_buttons[i];
            bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeOut::create(0.25), EaseQuadraticActionOut::create(MoveBy::create(0.25, Point(0, -60))), nullptr), CallFuncN::create([](Node* n) {
                n->setVisible(false);
                }), NULL));
        }

        for (auto i = 0; i < _top_buttons.size(); ++i) {
            auto bt = _top_buttons[i];
            bt->setOpacity(0);
            bt->runAction(Sequence::create(DelayTime::create(i * 0.1), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, -60))), CallFuncN::create([](Node* n) {
                n->setVisible(true);
                }), NULL), NULL));
        }

        auto bt_background = utils::findChild(this, "button_background");
        if (bt_background)
            bt_background->runAction(Sequence::create(DelayTime::create(0.2), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(MoveBy::create(0.25, Point(0, -60))), CallFuncN::create([](Node* n) {
            n->setVisible(true);
                }), NULL), NULL));
        showHidenMaterialButton(false);
        // chưa có profile được chọn
        if (MJDEFAULT->getIntegerForKey(key_current_profile_id, 0) == 0)
        {
            auto button_lock = utils::findChild(this, "button_lock");
            button_lock->setVisible(true);
            button_lock->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.1), CallFunc::create([this] {
                this->_is_animating = false;
                }), NULL));
            utils::findChild(this, "button_profile")->setVisible(false);
        }
        else
        {
            utils::findChild(this, "button_lock")->setVisible(false);
            auto button_profile = utils::findChild(this, "button_profile");
            button_profile->setVisible(true);
            button_profile->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.1), CallFunc::create([this] {
                this->_is_animating = false;
                }), NULL));
        }

        //if (auto node = dynamic_cast<Node*>(sender))
        //    {
        //        HelperManager::GetInstance()->addSidebarBlack();
        //    }
    }
}

void MSSidebar::onLessionTabAutoSelect()
{
    if (auto button = utils::findChild<ui::Button *>(this, "button_lessons"))
    {
        _is_animating = false;
        onLessons(button);
    }
}
void MSSidebar::onAISpeakingSelect()
{

}

void MSSidebar::onReadTabAutoSelect(){
    if (auto button = utils::findChild<ui::Button *>(this, "button_stories"))
    {
        _is_animating = false;
        onStories(button);
    }
}

void MSSidebar::showNumberNotification()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||  CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "getStringDataAppInbox");
#endif
    auto list_noti = StoryDataModel::getInstance()->getAllMessage();
    auto num_new_noti = 0;
    for (int i = 0; i < (int) list_noti.size(); i++) {
        if (XHSLib->checkMessageUnRead(list_noti[i].asValueMap()["id"].asInt())) {
            num_new_noti++;
        }
    }

    auto message = mj::appinbox::getMessageAppinbox();
    num_new_noti = num_new_noti + message.size();

    utils::findChild(this, "noti_img")->setVisible(num_new_noti > 0);
    utils::findChild<ui::Text *>(this, "noti_lb")->setString(StringUtils::toString(num_new_noti));
#endif
}

void MSSidebar::updateProfileAfterCreateProfile()
{
    if (MJDEFAULT->getIntegerForKey(key_current_profile_id, 0) == 0)
    {
        utils::findChild(this, "button_lock")->setVisible(true);
        utils::findChild(this, "button_profile")->setVisible(false);
    }
    else
    {
        utils::findChild(this, "button_lock")->setVisible(false);
        utils::findChild(this, "button_profile")->setVisible(true);
        auto profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id));
        loadDefaultAvatar(profile);
        utils::findChild<ui::Text*>(this, "sidebar.profile.name")->setString(profile.name);
    }
}
