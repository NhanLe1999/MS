//
//  MSTabSubscribe.cpp
//  MJStory
//
//  Created by Hoi Sung on 12/15/17.
//

#include "MSTabSubscribe.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSTabSubscribe_Item.h"
#include "MJPay.h"
#include "MJPayPopup.h"
#include "MJPaySubPopup.h"
#include "StoryConfigManager.h"
#include "MJPlatformConfig.h"
#include "MJCalculator.h"
#include "StoryFreeManager.h"
#include "MJPlatformConfig.h"
#include "HelperManager.h"
#include "MSEventLogger.h"
#include "IAPManager.h"
#include "NativeBridge.h"
#include "MSIntro.h"
#include "GettingUserInfoManager.h"
#include "RatingManager.h"
#include "MJ_PageLoadResource.h"
#include "APAuth.h"
#include "MSFirebasePlugin.h"
#include "F2PManager.h"
#include"MJSubscriptionGetPaymentInfo.h"

USING_NS_CC;
INITIALIZE_READER(MSTabSubscribe);

std::once_flag ms_tab_subscribe_reader;
MSTabSubscribe * MSTabSubscribe::createTab() {
    std::call_once(ms_tab_subscribe_reader, [] {
        REGISTER_CSB_READER(MSTabSubscribe);
        REGISTER_CSB_READER(MSTabSubscribe_Item);

    });
    auto csb_file = std::string("csb/new_ui/Subscribe.csb");
    if (CONFIG_MANAGER->isVietnameseStories()) {
        csb_file = std::string("csb/new_ui/Subscribe_VN.csb");
    }
    auto node = reinterpret_cast<MSTabSubscribe *>(CSLoader::createNode(csb_file));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

MSTabSubscribe * MSTabSubscribe::createTabVoucher() {
    std::call_once(ms_tab_subscribe_reader, [] {
        REGISTER_CSB_READER(MSTabSubscribe);
        REGISTER_CSB_READER(MSTabSubscribe_Item);
        
    });
    auto csb_file = std::string("csb/new_ui/Subscribe_VN_Install.csb");
    auto node = reinterpret_cast<MSTabSubscribe *>(CSLoader::createNode(csb_file));
    if (node) {
        node->_has_voucher = true;
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSTabSubscribe::didLoadFromCSB(bool dumy)
{
    _source = MJDEFAULT->getStringForKey(key_source_subscribe, "Clevertap");
    auto rich = ui::RichText::create();
    if (!MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
         auto text1 = ui::RichElementText::create(0, mj::helper::colorFromHex(0xa0a0a0), 0xff, "* Try 7 days free when you subscribe for a year. If you love it, no action is needed: iTunes will charge the yearly fee unless you cancel the trial 24 hours before expiration.", "fonts/Montserrat-Italic.ttf", 18);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
         text1 = ui::RichElementText::create(0, mj::helper::colorFromHex(0xa0a0a0), 0xff, "* Try 7 days free when you subscribe for a year. If you love it, no action is needed: Google Play will charge the yearly fee unless you cancel the trial 24 hours before expiration.", "fonts/Montserrat-Italic.ttf", 18);
#endif
        auto text2 = ui::RichElementNewLine::create(1, mj::helper::colorFromHex(0xa0a0a0), 0xff);
        auto node_break = ui::Layout::create(); node_break->setContentSize(Size(50, 10));
         auto p_break = ui::RichElementCustomNode::create(9, mj::helper::colorFromHex(0xa0a0a0), 0xff, node_break);
          auto text3 = ui::RichElementText::create(2, mj::helper::colorFromHex(0xa0a0a0), 0xff, "All plans renew automatically. You can change this by turning off auto-renewal in your iTunes settings. You must do this at least 24 hours before the end of your current subscription period for the change to take effect. To learn more, check our ", "fonts/Montserrat-Italic.ttf", 18);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
         text3 = ui::RichElementText::create(2, mj::helper::colorFromHex(0xa0a0a0), 0xff, "All plans renew automatically. You can change this by turning off auto-renewal in your Google Play settings. You must do this at least 24 hours before the end of your current subscription period for the change to take effect. To learn more, check our ", "fonts/Montserrat-Italic.ttf", 18);
#endif
        auto text4 = ui::RichElementText::create(3, mj::helper::colorFromHex(0x5fa3bb), 0xff, "Term of Use", "fonts/Montserrat-BoldItalic.ttf", 18, ui::RichElementText::UNDERLINE_FLAG, "http://google.com");
        auto text5 = ui::RichElementText::create(2, mj::helper::colorFromHex(0xa0a0a0), 0xff, " and ", "fonts/Montserrat-Italic.ttf", 18);
         auto text6 = ui::RichElementText::create(3, mj::helper::colorFromHex(0x5fa3bb), 0xff, "Privacy Policy", "fonts/Montserrat-BoldItalic.ttf", 18, ui::RichElementText::UNDERLINE_FLAG, "http://google.com");

        rich->pushBackElement(text1);
        rich->pushBackElement(text2);
         rich->pushBackElement(p_break);
        rich->pushBackElement(ui::RichElementNewLine::create(1, mj::helper::colorFromHex(0x2d2d2d), 0xff));
        rich->pushBackElement(text3);
        rich->pushBackElement(text4);
          rich->pushBackElement(text5);
        rich->pushBackElement(text6);
        rich->setPosition(Point(this->getContentSize().width / 2, 200));
        rich->ignoreContentAdaptWithSize(false);
         rich->setContentSize(Size(700, 200));

    }

    _pay_method = mj::PayMethod::MJPAY_NONE;
    this->setClippingEnabled(true);
    win_size = cocos2d::Director::getInstance()->getWinSize();
    setContentSize(win_size);
    layout_check_pos = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_pagekage");

    IAPManager::getInstance()->loadAndUpdatePackageInfoFromAppInfoJson();
    auto packagesAndroid = IAPManager::getInstance()->getPackages();
    auto packages = IAPManager::getInstance()->getPackages();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "US");

    k_countryCode = (k_countryCode == "VN" || k_countryCode == "TH" || k_countryCode == "ID") ? k_countryCode : "US";

    auto currentPackagesOnStoreMap = IAPManager::getInstance()->getCurrentSellingPackagesOnStore();
    auto currentOfflinePackagesOnStoreMap = IAPManager::getInstance()->getCurrentOfflineSellingPackagesOnStore();

    if (currentPackagesOnStoreMap.count(k_countryCode) > 0) { //can't get packages from server => will get offline packages

        std::vector<std::string> packagesOnStore = currentPackagesOnStoreMap.at(k_countryCode);

        for(int i = 0; i < packagesOnStore.size(); i++)
        {
            CCLOG("%s_____current___", packagesOnStore[i].c_str());
        }

        for(auto cc : packages)
        {
            CCLOG("%s_____currentPackges___", cc.first.c_str());
        }

        for (auto packName : packagesOnStore) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if (packName.find("alllanguage") != std::string::npos)
            {
                if (packages.count(packName) > 0) {

                    auto p = packages.at(packName);

                    if (k_countryCode == "TH" || k_countryCode == "ID") {
                        packName = packName + "." +  mj::helper::to_lower(k_countryCode);
                        p._productId = packName;
                        p._priceSale = (packages.count(packName) > 0) ? packages.at(packName)._priceSale : p._priceSale;
                    }

                    _actualPackages.insert({ packName,p });
                }
            }
            else
#endif
            {
                if (packages.count(packName) > 0) {

                    _actualPackages.insert({ packName,packages.at(packName) });
                }
            }
        }
    }
    //offline
    else if (currentOfflinePackagesOnStoreMap.count(k_countryCode) > 0) {

        std::vector<std::string> packagesOnStore = currentOfflinePackagesOnStoreMap.at(k_countryCode);

        for (auto packName : packagesOnStore) {

            if (packages.count(packName) > 0) {

                _actualPackages.insert({ packName,packages.at(packName) });
            }
        }
    }

    mj::Pay::getInstance().setPackages(_actualPackages);
    _packages =  mj::Pay::getInstance().getPackages();

    auto packge = IAPManager::getInstance()->getCurrentSellingPackagesOnStore();

    if (packge.count(k_countryCode))
    {
        auto listIdPackge = packge.at(k_countryCode);

        int numberIdPackge = 0;

        for (int i = 0; i < _packages.size(); i++)
        {
            int count = 0;
            for (int j = 0; j < listIdPackge.size(); j++)
            {
                if (_packages[i].product_id != listIdPackge[j])
                {
                    count++;
                }
            }

            if (count == listIdPackge.size())
            {
                _packages.erase(_packages.begin() + i);
                break;
            }

        }
    }

    _package_items.clear();

    std::vector<double> listPrice = {};

    for (int i = 0; i < _packages.size(); i++)
    {
        listPrice.push_back(_packages[i].price_40);
    }
    mj::ListPackages packagesCopy = {};
    std::sort(listPrice.begin(), listPrice.end());

    for (int i = 0; i < listPrice.size(); i++)
    {
        for (int j = 0; j < _packages.size(); j++)
        {
            if (listPrice[i] == _packages[j].price_40)
            {
                packagesCopy.push_back(_packages[j]);
            }
        }
    }
    _packages.clear();
    _packages = packagesCopy;
    resetPackget(_packages);

    for (auto i = 0; i < _packages.size(); i++) {
        auto p = utils::findChild<MSTabSubscribe_Item*>(this, cocos2d::StringUtils::format("package_%d", i));
        p->setDelegate(this);
        p->setTag(i);
        p->setPackage(_packages[i]);
    }
   
    createLayoutRun();
    changeTextDisplayLang();
    
    _pay_method = mj::PayMethod::MJPAY_NONE;

    std::vector<cocos2d::ui::Text *> textSameFont{
            utils::findChild<ui::Text *>(this, "text.title"),
            utils::findChild<ui::Text *>(this, "text.plan.title"),
            utils::findChild<ui::Text *>(this, "extra_title"),
            utils::findChild<ui::Text *>(this, "text.features.title"),
            utils::findChild<ui::Text *>(this, "text.features.title1"),
            utils::findChild<ui::Text *>(this, "text.features.title2"),
            utils::findChild<ui::Text *>(this, "text.features.title3"),
            utils::findChild<ui::Text *>(this, "text.features.title4"),
            utils::findChild<ui::Text *>(this, "text.features.title5"),
            utils::findChild<ui::Text *>(this, "text.features.title6"),
            utils::findChild<ui::Text *>(this, "text.features.title7"),
            utils::findChild<ui::Text *>(this, "text.features.title8"),
    };

     for(auto text : textSameFont)
     {
         if(!text)
         {
             continue;
         }

         text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
     }
     std::vector<cocos2d::ui::Text*> textSameFonts =
     {
         utils::findChild<ui::Text *>(this, "text.title.detail"),
         utils::findChild<ui::Text *>(this, "text.pay"),
         utils::findChild<ui::Text *>(this, "text.detail1"),
         utils::findChild<ui::Text *>(this, "text.detail2"),
         utils::findChild<ui::Text *>(this, "text.detail3"),
     };

     for(auto text : textSameFonts)
     {
         if(!text)
         {
             continue;
         }

         text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
     }
    
    utils::findChild<ui::Text *>(this, "text_restore")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fontsMontserrat-Italic.ttf");
    
    if (CONFIG_MANAGER->isCountryVN())
    {
        utils::findChild<ui::Text *>(this, "text.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.title.vn"));
    }
    else
    {
        utils::findChild<ui::Text *>(this, "text.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.title"));
    }
    utils::findChild<ui::Text *>(this, "text.title.detail")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.title.detail"));
    
    auto text_sub_detail = utils::findChild<ui::Text *>(this, "text.title.detail_sub");
    text_sub_detail->setString(LANGUAGE_MANAGER->getDisplayTextByKey("text.title.detail_sub"));
    text_sub_detail->setContentSize(text_sub_detail->getVirtualRendererSize());

    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    if (CONFIG_MANAGER->isCountryVN()) {
        if (!_has_voucher) {
            listview->removeChild(utils::findChild(this, "layout_pay_text"));
        } else {
            listview->setTouchEnabled(false);
        }
        auto listv = utils::findChild<ui::ListView *>(this, "listv");
        auto voucher = MJDEFAULT->getStringForKey("key.install.voucher");
        auto text_sub = utils::findChild<ui::Text *>(this, "text.title.detail");
        if (voucher.empty()) {
            text_sub->setString(LANGUAGE_MANAGER->getDisplayTextByKey("voucher.discount.inapp"));
            listv->removeItem(1);
        } else {
            auto str = StringUtils::format("%s%s", LANGUAGE_MANAGER->getDisplayTextByKey("voucher.discount").c_str(), voucher.c_str());
            text_sub->setString(str);
        }
        text_sub->setContentSize(text_sub->getVirtualRendererSize() + Size(2, 0));
        auto w = 0;
        for (auto i : listv->getItems()) {
            w += i->getContentSize().width;
        }
        listv->setContentSize(Size(w, listv->getContentSize().height));
        listv->forceDoLayout();
        listv->setTouchEnabled(false);
    } else {
        auto listv = utils::findChild<ui::ListView *>(this, "listv");
        listv->removeItem(1);
        auto w = 0;
        for (auto i : listv->getItems()) {
            w += i->getContentSize().width;
        }
        listv->setContentSize(Size(w, listv->getContentSize().height));
        listv->forceDoLayout();
        listv->setEnabled(false);
    }

    if (_has_voucher) {
        return;
    }
    
    utils::findChild<ui::Text *>(this, "text.plan.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.plan.title"));
    utils::findChild<ui::Text *>(this, "text.detail1")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.detail1"));
    utils::findChild<ui::Text *>(this, "text.detail2")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.detail2"));
    utils::findChild<ui::Text *>(this, "text.detail3")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), CONFIG_MANAGER->isAppVNStories() ? "sub.text.detail3.vm":"sub.text.detail3"));

    if (LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.detail3").size() > 15) utils::findChild<ui::Text *>(this, "text.detail3")->setFontSize(20);
    
	utils::findChild<ui::Text *>(this, "text.number.detail")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.number.detail"));
    utils::findChild<ui::Text *>(this, "text.features.title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title"));
    utils::findChild<ui::Text *>(this, "text.features.title1")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title1"));
    utils::findChild<ui::Text *>(this, "text.features.title2")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title2"));
    utils::findChild<ui::Text *>(this, "text.features.title3")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title3"));
    utils::findChild<ui::Text *>(this, "text.features.title4")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title4"));
    utils::findChild<ui::Text *>(this, "text.features.title5")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title5"));
    utils::findChild<ui::Text *>(this, "text.features.title6")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title6"));
    utils::findChild<ui::Text *>(this, "text.features.title7")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title7"));
    utils::findChild<ui::Text *>(this, "text.features.title8")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.features.title8"));

    
    std::vector<std::pair<std::string,ui::Text*>> numbers =
    {
        {"1000+",utils::findChild<ui::Text *>(this, "text.number1")},
        {"300+",utils::findChild<ui::Text *>(utils::findChild<ui::Text *>(this, "img_plan3"), "text.number2")},
        {"200+",utils::findChild<ui::Text *>(utils::findChild<ui::Text *>(this, "img_plan2"), "text.number2")},
    };
    
    for(auto s : numbers)
    {
        if(s.second)
        {
            s.second->setString(s.first);
        }
    }

    auto text_restore = utils::findChild<ui::Text *>(this, "text_restore");
    text_restore->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text_restore"));
    
    utils::findChild(this, "panel_restore")->setContentSize(cocos2d::Size(utils::findChild(this, "text_restore")->getContentSize().width, 1));
    
    auto pay_text = utils::findChild(this, "layout_pay_text");
    if (pay_text) {
        pay_text->removeAllChildren();
        pay_text->addChild(rich);
        rich->setContentSize(pay_text->getContentSize() - Size(50, 0));
        rich->setPosition(pay_text->getContentSize() / 2);

        auto sample = ui::Layout::create();
    //    sample->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    //    sample->setBackGroundColor(Color3B::YELLOW);
    //    sample->setOpacity(0xaa);
        sample->setContentSize(Size(125, 25));
        sample->setPosition(Point(350, 25));
        sample->setTouchEnabled(true);
        sample->addClickEventListener([](Ref * sender) {
            Application::getInstance()->openURL(key_terms_of_use_url);
        });
        pay_text->addChild(sample);
        
        auto sample2 = sample->clone();
        sample2->setContentSize(Size(150, 25));
        sample2->setPosition(Point(515, 25));
        sample2->addClickEventListener([] (Ref * sender) {
            Application::getInstance()->openURL(key_privacy_policy_url);
        });
        pay_text->addChild(sample2);
    }
    
    auto layout_plan_title = utils::findChild(this, "layout_plan_title");
    auto text_plane_title = utils::findChild(this, "text.plan.title");
    layout_plan_title->getChildByName("gachngang1")->setPositionX(layout_plan_title->getContentSize().width/2 - text_plane_title->getContentSize().width/2 - 10);
    layout_plan_title->getChildByName("gachngang2")->setPositionX(layout_plan_title->getContentSize().width/2 + text_plane_title->getContentSize().width/2 + 10);
    
    auto text_features = utils::findChild(this, "text.features.title");
    auto layout_features_title = utils::findChild(this, "layout_features_title");
    layout_features_title->getChildByName("gachngang1")->setPositionX(layout_features_title->getContentSize().width/2 - text_features->getContentSize().width/2 - 10);
    layout_features_title->getChildByName("gachngang2")->setPositionX(layout_features_title->getContentSize().width/2 + text_features->getContentSize().width/2 + 10);

    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onClose(nullptr);
        }
    };

    setCallbackOnclickChoosePackge();
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
}


void MSTabSubscribe::onEnter() {
    
    ui::Layout::onEnter();
    
    CRASH_AUTO_LOG;
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    auto event_paypopup_close = cocos2d::EventListenerCustom::create(k_event_pay_popup_close, [=](cocos2d::EventCustom * e) {
        auto pay_method = (mj::PayMethod *)(e->getUserData());
        this->_pay_method = * pay_method;
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_paypopup_close, this);
    
    auto event_coupon_change = cocos2d::EventListenerCustom::create(key_event_coupon_change, [=](cocos2d::EventCustom * e) {
        this->scheduleOnce([=](float dt) {
            this->showPriceInfo();
            this->changeTextDisplayLang();
        }, 0.25, "delay_reload_price");
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_coupon_change, this);
    
    mj::Pay::getInstance().setPriceDelegate(this);
    mj::Pay::getInstance().refresh();
    
    this->schedule(CC_SCHEDULE_SELECTOR(MSTabSubscribe::updateCheckPos));
    
    auto btclose = cocos2d::utils::findChild(this, "btclose");
    if (mj::PlatformConfig::getInstance().getNotchHeight() != 0) {
        btclose->setPosition(btclose->getPosition() + Vec2(-mj::PlatformConfig::getInstance().getNotchHeight(), 0));
    }
    btclose->setPosition(btclose->getPosition()+Vec2(-60,-10)); //fix to fit to oppo screen
    
    if (MJDEFAULT->getBoolForKey(key_show_parent_lock, true) && !_has_voucher) {
        MJCalculator::showCalculator(this, nullptr, [=] {
            this->onClose(nullptr);
        });
    }

}

void MSTabSubscribe::runAnimationAppear() {
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    auto elements = listview->getItems();
    for (auto element : elements) {
        element->setOpacity(0);
    }
    
    this->scheduleOnce([=](float) {
        for (auto i = 0; i < elements.size(); ++i) {
            auto element = elements.at(i);
            auto saved_position = element->getPosition();
            element->setPosition(saved_position + Point(0, -200));
            element->runAction(Sequence::create(DelayTime::create(0.05 * i), Spawn::create(EaseQuadraticActionOut::create(MoveTo::create(0.4, element->getPosition() + Point(0, 200))), FadeIn::create(0.4),nullptr), NULL));
        }
    }, 0.1, "animate_delay_subscribe");
}

void MSTabSubscribe::runAnimationDisappear() {
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    auto elements = listview->getItems();

        for (auto i = 0; i < 6; ++i) {
            if (i >= elements.size())
                break;
            auto element = elements.at(i);
            auto saved_position = element->getPosition();
            element->runAction(Sequence::create(DelayTime::create(0.05 * (6 - i)), Spawn::create(EaseQuadraticActionIn::create(MoveTo::create(0.25, element->getPosition() + Point(0, -200))), FadeOut::create(0.25),nullptr), NULL));
        }
    
    this->runAction(Sequence::create(FadeOut::create(1), CallFunc::create([=]{
        this->removeFromParent();
    }), nullptr));
}

void MSTabSubscribe::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
}

void MSTabSubscribe::createLayoutRun(){
    layout_run = cocos2d::ui::Scale9Sprite::create("mjstory/new_ui/tabsubscribe_package_navi.png");
    layout_run->setPreferredSize(cocos2d::Size(win_size.width, 101));
    layout_run->setPosition(Vec2(win_size.width/2, win_size.height + 200));
    layout_run->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    this->addChild(layout_run, 1900000000);
    auto bt_startweek = cocos2d::ui::Button::create("mjstory/new_ui/tabsubscribe_navibt.png");
    bt_startweek->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    bt_startweek->setPosition(Vec2(layout_run->getContentSize().width - 10, layout_run->getContentSize().height/2 - 2.5));
    layout_run->addChild(bt_startweek);
    bt_startweek->addClickEventListener(CC_CALLBACK_1(MSTabSubscribe::onStartYourFreeWeek, this));
    
    auto bt_title = cocos2d::ui::Text::create(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.startyourweek"),
                                              LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/monkeystories_ultra.ttf", 25);
    bt_title->setPosition(Vec2(bt_startweek->getContentSize().width/2, bt_startweek->getContentSize().height/2 + 2.5));
    bt_startweek->addChild(bt_title);
    
    auto text_layout = cocos2d::ui::Text::create(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.run"),
                                                 LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/monkeystories_ultra.ttf", 25);

    if (CONFIG_MANAGER->isCountryVN())
    {
        text_layout->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.text.run.vn"));
    }
    
    text_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    text_layout->setPosition(Vec2((bt_startweek->getPositionX() - bt_startweek->getContentSize().width)/2, layout_run->getContentSize().height/2));
    layout_run->addChild(text_layout);
}

void MSTabSubscribe::onStartYourFreeWeek(cocos2d::Ref *sender){
    CCLOG("onStartYourFreeWeek");
    mj::helper::playButtonFX();
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    listview->scrollToTop(0.25, true);
}

void MSTabSubscribe::updateCheckPos(float dt){
    auto pos_check = layout_check_pos->getParent()->convertToWorldSpace(layout_check_pos->getPosition());
    if (pos_check.y >= (win_size.height-5) && !isrun_down) {
        isrun_up = false;
        isrun_down = true;
        layout_run->stopAllActions();
        layout_run->runAction(MoveTo::create(0.25f, Vec2(win_size.width/2, win_size.height)));
        return;
    }
    
    if (pos_check.y < (win_size.height-5) && !isrun_up) {
        isrun_up = true;
        isrun_down = false;
        layout_run->stopAllActions();
        layout_run->runAction(MoveTo::create(0.25f, Vec2(win_size.width/2, win_size.height+200)));
    }
}

MSTabSubscribe::~MSTabSubscribe()
{
    pushCleverTapEvent();
}

void MSTabSubscribe::showPriceInfo() {
    _packages = _packagesCopy;

    for (auto i = 0; i < _packages.size(); i++){
        auto p = utils::findChild<MSTabSubscribe_Item *>(this, StringUtils::format("package_%d", i));
        if (!p) {
            continue;
        }
        p->setPackage(_packages[i]);
        p->reloadPrice();
        _package_items.push_back(p);
    }
}

void MSTabSubscribe::resetPackget(mj::ListPackages packages)
{
    _packagesCopy = packages;
}

#include "MSEventLogger.h"
#include "PayInit.h"
#include "PayInit_Win.h"

void MSTabSubscribe::onSelectProduct(mj::InAppPackage product) {
    if (_callbackIsClickChoosePackge)
    {
        _callbackIsClickChoosePackge(product);
    }
}

void MSTabSubscribe::changeTextDisplayLang()
{

}

void MSTabSubscribe::onClose(cocos2d::Ref *sender)
{
    _callbackIsClickChoosePackge = nullptr;
	mj::helper::playButtonFXClose();
    this->runAnimationDisappear();
}

ui::Widget::ccWidgetClickCallback MSTabSubscribe::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSTabSubscribe, onClose),
        CLICK_MAP(MSTabSubscribe, onRestoreClicked),
        CLICK_MAP(MSTabSubscribe, onDetail),
    };
    
    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    return nullptr;
}

void MSTabSubscribe::onGetPriceInfo(mj::ListPackages packages) {
    _packages = packages;
    showPriceInfo();
}

void MSTabSubscribe::onRestoreClicked(cocos2d::Ref *sender)
{
    mj::helper::showWaitView();
    removePaymentListenerEvents();

    registerFailedEvents();
    registerSuccessEvents("",true);
    IAPManager::getInstance()->onIAPRestore();
}

void MSTabSubscribe::onExit() 
{
    ui::Layout::onExit();
    mj::Pay::getInstance().setPriceDelegate(nullptr);
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
        ms::HomeTab preTabUnclock = (ms::HomeTab)MJDEFAULT->getIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::LESSONS);
        _delegate ? _delegate->onSelectedTab(preTabUnclock, ms::MovingDirection::NONE) : (void) nullptr;
    }
}

void MSTabSubscribe::onAlertOK(cocos2d::Ref *sender) 
{
	//tracking.
	click_type = "purchase_now";
    ms::EventLogger::getInstance().logEvent("buy_now", {{"product_id", cocos2d::Value(_product.getProductIDStore(mj::Pay::getInstance().getDiscountPercent()))}});

    mj::InApp::getInstance().purchase(_product.getProductIDStore(mj::Pay::getInstance().getDiscountPercent()));
}

#include "MJPlatformConfig.h"
void MSTabSubscribe::onAlertCancel(cocos2d::Ref * sender) 
{
	//tracking.
	
}

#include "MSVoucherDetail.h"
void MSTabSubscribe::onDetail(Ref * sender) {
    auto detail = MSVoucherDetail::createView();
    this->addChild(detail, 1000);
}

void MSTabSubscribe::pushCleverTapEvent()
{
    MJDEFAULT->deleteValueForKey(key_source_subscribe);
    CleverTapManager::GetInstance()->pushEvent("purchase_screen_view",{
        {"click_type", cocos2d::Value(_isclickBuyNow ? "buy_now" : "close")},
        {"source", cocos2d::Value(_source)}});
}

void MSTabSubscribe::onIAP(std::string k_productID)
{

    MK_RETURN_IF(k_productID.empty());
    
    const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");
    CCLOG("%s___", k_countryCode.c_str());
    onPayInApp(k_productID);
}


void MSTabSubscribe::registerSuccessEvents(std::string k_productID, bool isRestore)
{
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_PURCHASED_FAILURE, [=](EventCustom* eventCustom) {//show popup get user info

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_FAILURE);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            const auto k_countryCode = MJDEFAULT->getStringForKey(key_country_code,"VN");

            if (!MJDEFAULT->getBoolForKey(key_is_submit, false))
            {
                if (k_countryCode == "VN" || k_countryCode == "TH" || k_countryCode == "US")
                {
                    MJSubscriptionGetPaymentInfo::Type typePop = MJDEFAULT->getStringForKey(key_popup_fail_payment, "phone") == "phone" ? MJSubscriptionGetPaymentInfo::Type::PHONE : MJSubscriptionGetPaymentInfo::Type::EMAIL;

                    auto view = MJSubscriptionGetPaymentInfo::createView(typePop, "");

                    this->addChild(view, INT_MAX - 1);
                }
                else
                {
                    StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("pay.purchase.failed"), "OK", "Close");
                }
            }

        });

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS, [=](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJParentPopup::PopupInfo purchaseSuccessInfo;

            std::string title, message, ok;
            StoryAlert* alert = nullptr;

            title       = LANGUAGE_MANAGER->getDisplayTextByKey(isRestore ? "subscription.restore.success":"subscription.purchase.success");
            message     = isRestore ? title : LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.access.all");
            ok          = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.start.learning.now");

            if(!k_productID.empty()||!isRestore) {

                if (IAP_MANAGER->isLifeTimePackage(k_productID))
                {
                 message = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.1profile");
                }
                else if (IAP_MANAGER->isPromotionPackage(k_productID))
                {
                   // purchaseSuccessInfo.message = LANGUAGE_MANAGER->getDisplayTextForKey("subscription.purchase.success.2months");
                    message = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.2months");
                }
            }
                this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom* e) {

                    this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");

                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));

                    });

                }), this);
            alert = StoryAlert::showAlert(message, ok, title, false, 30, nullptr, 0, "", "", 32);
            alert-> setOkButtonCallback([=]() {
                if(F2P_MANAGER->isSkipUser())
                {
                    
                }
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
            });

        });

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS, [=](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS);

        std::string message                                 = "";

        std::unordered_map<std::string,std::string > newMap = std::unordered_map<std::string, std::string >();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)||(CC_TARGET_PLATFORM == CC_PLATFORM_MAC )

        std::unordered_map<std::string, std::string > *map         = static_cast<std::unordered_map<std::string, std::string>*>(eventCustom->getUserData());

         newMap = map != nullptr ?*map:std::unordered_map<std::string, std::string >();


         CC_SAFE_DELETE(map);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string *data         = static_cast<std::string*>(eventCustom->getUserData());

        message     = data != nullptr ? std::string(*data) : "";

        CC_SAFE_DELETE(data);
#endif
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            std::string messageA, title, ok;
           title       = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.changesuccess");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            messageA     =   message;
#else
            std::string packageRenew        =  newMap.count("package_renew") > 0 ? newMap.at("package_renew"):"";

            int timeRenew                   = -1;

            if(newMap.count("time_renew") > 0 ) {

                SAFE_EXECUTE_CODE
                (
                   timeRenew = stoi(newMap.at("time_renew"));
                )
            }

            std::string priceStr        = "<price>";

            std::string packageName     = "<package_name>";

            if(IAP_MANAGER->getPackages().count(packageRenew)>0) {

                auto packInfo      = IAP_MANAGER->getPackages().at(packageRenew);

                priceStr        = packInfo.getSalePriceAsString() + " " + packInfo._currency;

                packageName     = LANGUAGE_MANAGER->getDisplayTextByKey(packInfo._productInfoNameKey);
            }

            std::string timeRenewStr        =  mj::helper::getStringTime(timeRenew,  "%R %d/%m/%G");

            std::string str                 =  LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.success.changesubscription2");

            messageA =  StringUtils::format(str.c_str(),packageName.c_str(),priceStr.c_str(),timeRenewStr.c_str());

#endif
           ok          =   LANGUAGE_MANAGER->getDisplayTextByKey("subscription.purchase.start.learning.now");

            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom* e) {

                this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));

                });

            }), this);

            auto alert = StoryAlert::showAlert(messageA, ok, title, false, 28, nullptr, 0, "", messageA, 30);
            alert->setOkButtonCallback([=]() {

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
                });

            });

        });

    });
}

void MSTabSubscribe::removePaymentListenerEvents() {

    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_FAILURE);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_PURCHASED_SUCCESS);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SWITCH_SUBSCRIPTION_PACKAGE_SUCCESS);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_306);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_307);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_308);

}

void MSTabSubscribe::registerFailedEvents() {

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_306, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_306);

        std::unordered_map<std::string, std::string>* data = static_cast<std::unordered_map<std::string, std::string>*>(eventCustom->getUserData());

        std::unordered_map<std::string, std::string> map = (data != nullptr ? *data : std::unordered_map<std::string, std::string>());

        CC_SAFE_DELETE(data);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJLoadingLayer::hide();

            std::string platform_name = map.count("platform_name") > 0 ? map.at("platform_name") : "--";
            std::string time_purchase = map.count("time_purchase") > 0 ? map.at("time_purchase") : "--";

            std::string ok, message;
           ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
            message = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.purchase.address").c_str(), platform_name.c_str(), time_purchase.c_str());

            StoryAlert ::showAlert(message, ok, "can", false, 20, nullptr, 0, "", message, 30);

        });

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_307, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_307);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

           std::string ok, message;

            ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
            message = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.purchase1");

            auto alert = StoryAlert ::showAlert(message, ok, "title", false, 20);

        });

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_SUBSCRIPTION_308, [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_SUBSCRIPTION_308);

        std::string* data = static_cast<std::string*>(eventCustom->getUserData());

        std::string userName = data != nullptr ? std::string(*data) : "";

        CC_SAFE_DELETE(data);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

            //MJLoadingLayer::hide();

            std::string ok, message;
            ok         = LANGUAGE_MANAGER->getDisplayTextByKey("key.logout");
            message    = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.already.register.message").c_str(), userName.c_str());

            auto alert = StoryAlert::showAlert(message, ok, "can", false, 28, nullptr, 0, "", message, 30);
  
            alert->setOkButtonCallback([=] {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_change_to_landscape", [=](EventCustom*) {
                        MK_RUN_IN_COCOS_THREAD(Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, MSIntro::createScene()));
                                                       this->getEventDispatcher()->removeCustomEventListeners("event_change_to_landscape");)
                    }), this);

                    logOutApp();
                });

            });

        });

    });

}

void MSTabSubscribe::onPayInApp(std::string productID)
{
    if (productID.empty()) {

        CCLOG("%s cannot get product id!", __FUNCTION__);
        return;
    }

    if (NativeBridge::networkAvaiable()) {

        IAPManager::getInstance()->onIAP(productID);
    }
    else
    {
        IAP_MANAGER->showErrorPopupByCode(LANGUAGE_MANAGER->getDisplayTextByKey("subscription.error.network"));
    }
}

void MSTabSubscribe::onIAPInapp(std::string productID)
{
    mj::helper::showWaitView();
    IAPManager::getInstance()->checkIAPConditions([=](){

          Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

              const std::string cannotBuyReason = IAPManager::getInstance()->getFailReason();
              CleverTapManager::GetInstance()->pushEvent("purchase_screen_choose_package",{
                              {"choose_package", cocos2d::Value(mj::helper::getPackageTime(productID))},
                              {"verify", cocos2d::Value(cannotBuyReason.empty() ? "true" : "false")}
                          });
              if(!cannotBuyReason.empty()) {
                  return;
              }

              onIAP(productID);

          });
    });
}

void MSTabSubscribe::setCallbackOnclickChoosePackge()
{
    _callbackIsClickChoosePackge = [=](mj::InAppPackage product) {
        removePaymentListenerEvents();

        registerFailedEvents();
        registerSuccessEvents(product.product_id,false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        bool isSubcribe1Month = (product.product_id.find("1month") != std::string::npos);
        if (MJDEFAULT->getBoolForKey(key_is_submit, false) || isSubcribe1Month)
        {
            onIAPInapp(product.product_id);
            return;
        }
#endif
        
        StoryAlert* alert = nullptr;
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI
        auto str = LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.ios.win");
        if (!MJDEFAULT->getStringForKey("key.install.voucher").empty())
        {
            str = LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.ios.voucher.win");
        }
        if (!MJDEFAULT->getStringForKey("key.install.voucher").empty() || !CONFIG_MANAGER->isAppVNStories())
        {
            alert = StoryAlert::showAlert(str, LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.other"), LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.continue"), false, 20, nullptr, 0, "", "", 25);
        }
        else
        {
            alert = StoryAlert::showAlert(str, LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.other.voucher"), LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.continue"), false, 20, nullptr, 0, "", "", 25);
        }

#else
        auto str = LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.ios");
        if (!MJDEFAULT->getStringForKey("key.install.voucher").empty())
        {
            str = LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.ios.voucher");
        }
        if (!MJDEFAULT->getStringForKey("key.install.voucher").empty() || !CONFIG_MANAGER->isAppVNStories())
        {
            alert = StoryAlert::showAlert2Button(str, LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.continue"), LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.other"), false, 20);
        }
        else
        {
            alert = StoryAlert::showAlert2Button(str, LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.continue"), LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.other.voucher"), false, CONFIG_MANAGER->isAppVNStories() ? 18 : 20);
        }

#endif

        alert->setCacelButtonCallback([=] {
            click_type = "receive_license_key";

#if !(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI)
            if (CONFIG_MANAGER->isAppVNStories()) //VMonkey.
            {
                Application::getInstance()->openURL(MJDEFAULT->getStringForKey("pay_use_url", "https://www.vmonkey.vn/bang-gia.html"));
            }
            else
            {
                auto url = MJDEFAULT->getStringForKey("pay_use_url", "http://truyentranh.monkeystories.vn/lp5-app/?utm_source=App&utm_medium=App_Mai_MS&utm_campaign=BangGiaAppMS");
                Application::getInstance()->openURL(url);
            }

            pushEventPopupConfirm("receive_license_key", true);
#else
            pushEventPopupConfirm("close", false);
#endif
        });
        
#if !(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI)
        alert->setCloseButtonCallback([this] {
            pushEventPopupConfirm("close", false);
        });
#endif

        alert->setOkButtonCallback([=] {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_PLATFORM_HUAWEI)
            auto url = MJDEFAULT->getStringForKey("pay_use_url", "http://truyentranh.monkeystories.vn/lp5-app/?utm_source=App&utm_medium=App_Mai_MS&utm_campaign=BangGiaAppMS");
            Application::getInstance()->openURL(url);

            pushEventPopupConfirm("receive_license_key", true);

            return;
#endif
            pushEventPopupConfirm("buy now", true);
            onIAPInapp(product.product_id);
        });

    };
}

void MSTabSubscribe::logOutApp()
{
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

    auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event)
    {

        ms::EventLogger::getInstance().logEvent("event_signout_done", {}); cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_successfully");
        MJDEFAULT->setBoolForKey(key_onboarding_license, false);
        MJDEFAULT->setBoolForKey(key_logged_in, false);
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
            auto scene = MSIntro::createScene();
            Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
        });
    });

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom * event) {
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

void MSTabSubscribe::pushEventPopupConfirm(std::string typeClick, bool isSucces)
{
    CleverTapManager::GetInstance()->pushEvent("purchase_screen_popup_confirm", {
        {"click_type",cocos2d::Value(typeClick)},
        {"source",cocos2d::Value(_source)},
        {"purchased_successful",cocos2d::Value(isSucces)},
        });
}
void MSTabSubscribe::CallAPILoadUpdate()
{
    mj::helper::showWaitView();
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ap::api::LoadUpdate::call(CC_CALLBACK_2(MSTabSubscribe::APILoadUpdateCallBack, this)); });
            }),
        NULL));

}

void MSTabSubscribe::APILoadUpdateCallBack(bool isSucess, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::REGISTER, StartLoginFrom::Default, StartRegisterFrom::MSNewTabSubscribe);

        loginView->setName("loginView");
        Director::getInstance()->getRunningScene()->addChild(loginView);

        animationMoveToRight(loginView);
        mj::helper::deleteWaitView();
        this->removeFromParent();
    });

}
void MSTabSubscribe::changeDisplayLanguage()
{
    
}
