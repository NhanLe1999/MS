//
//  MJSubscriptionPage.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJSubscriptionPage.h"
#include "MJPayPopup.h"
#include "MJPaySubPopup.h"
#include "MJDefinitions.h"
#include "MJCalculator.h"
#include "NativeBridge.h"
#include "MJPayGuide.h"
#include "StoryFeedback.h"
#include "MJPayDetails.h"
#include "MJDefinitions.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#endif
#include "HSLib.h"


INITIALIZE_READER_INSTANCE(MJSubscriptionPageReader);

MJSubscriptionPage * MJSubscriptionPage::createPage(const int & discount) {
    auto csb_name = CONFIG_MANAGER->isHomeScreenLandscape() ? "csb/pay/MJSubscriptionPage_Landscape.csb" : "csb/pay/MJSubscriptionPage.csb";
    auto p = static_cast<MJSubscriptionPage *>(cocos2d::CSLoader::createNodeWithVisibleSize(csb_name));
    if (p) {
        p->_discount = discount;
        p->_pay_method = mj::PayMethod::MJPAY_NONE;
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJSubscriptionPage::didLoadFromCSB() {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    if(CONFIG_MANAGER->isVietnameseStories()){
        mj::InApp::getInstance().init({
            "mac.new.earlystart.stories.vn.1month.40_off",
            "mac.new.earlystart.stories.vn.6month.40_off",
            "mac.new.earlystart.stories.vn.1year.40_off",
            "mac.new.earlystart.stories.vn.lifetime.40_off",
        });

    }else{
        mj::InApp::getInstance().init({
            "mac.new.earlystart.stories.1month.40_off",
            "mac.new.earlystart.stories.6month.40_off",
            "mac.new.earlystart.stories.1year.40_off",
            "mac.new.earlystart.stories.lifetime.40_off",
            "mac.trial.earlystart.stories.lifetime.40_off",
        });
    }
#else
    if(CONFIG_MANAGER->isVietnameseStories()){
        mj::InApp::getInstance().init({
            "new.earlystart.stories.vn.1month.40_off",
            "new.earlystart.stories.vn.6month.40_off",
            "new.earlystart.stories.vn.1year.40_off",
            "new.earlystart.stories.vn.lifetime.40_off",
        });
        
    }else{
        mj::InApp::getInstance().init({
                "new.earlystart.stories.1month.40_off",
                "new.earlystart.stories.6month.40_off",
                "new.earlystart.stories.1year.40_off",
                "new.earlystart.stories.lifetime.40_off",
                "trial.earlystart.stories.1year.40_off"
        });
    }
#endif
#endif
    classes_name = "subscribepage";
    _packages = mj::Pay::getInstance().getPackages();
    this->setAnchorPoint(cocos2d::Point::ANCHOR_BOTTOM_LEFT);
    this->setPosition(cocos2d::Point::ZERO);
    for (auto i = 0; i < 4; i++) {
        auto p = static_cast<MJSubscriptionItem *>(cocos2d::ui::Helper::seekWidgetByName(this, cocos2d::StringUtils::format("product_%d", i)));// mj::helper::getChild<MJSubscriptionItem *>(this, "content", "products", cocos2d::StringUtils::format("wrapper_%d", i), cocos2d::StringUtils::format("product_%d", i));
        p->setDelegate(this);
        p->setPackage(_packages[i]);
    }

    if (CONFIG_MANAGER->isCountryVN()) {
        if (CONFIG_MANAGER->isHomeScreenLandscape()) {
            utils::findChild(this, "wrapper_0")->setVisible(false);
            auto products_panel = utils::findChild(this, "products");
            products_panel->setPosition(products_panel->getPosition() + Vec2(0, 25));
        }
    }

//    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
//        auto sv = utils::findChild<ui::ScrollView *>(this, "scrollview");
//        sv->setScrollBarEnabled(false);
//        auto content = utils::findChild(this, "content");
//        if (sv->getContentSize().width > content->getContentSize().width) {
//            sv->setInnerContainerSize(Size(sv->getContentSize().width, sv->getContentSize().height));
//            content->setAnchorPoint(Point::ANCHOR_MIDDLE);
//            content->setPosition(content->getParent()->getContentSize() / 2);
//        }
//    }

    showPriceInfo();

    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        if (CONFIG_MANAGER->isCountryVN()) {
            utils::findChild(this, "panel_vi")->setVisible(true);
            utils::findChild(this, "text_en")->setVisible(false);
            auto lv = utils::findChild<ui::ListView *>(this, "listview_detail");
            lv->setEnabled(false);
            lv->setScrollBarEnabled(false);
            lv->setTopPadding(75);
        } else {
            utils::findChild(this, "panel_vi")->setVisible(false);
            utils::findChild(this, "text_en")->setVisible(true);
            utils::findChild<ui::ListView *>(this, "listview_detail")->setEnabled(true);
            utils::findChild<ui::ListView *>(this, "listview_detail")->setScrollBarAutoHideEnabled(true);
            utils::findChild<ui::ListView *>(this, "listview_detail")->setScrollBarAutoHideTime(1000.f);
        }
    } else {
        if (CONFIG_MANAGER->isCountryVN()) {
            auto saved = utils::findChild(this, "text_en")->getContentSize().height;
//            utils::findChild(this, "text_en")->removeFromParent();
//            utils::findChild<ui::ListView *>(this, "listview_detail")->setEnabled(false);
            auto lv = utils::findChild<ui::ListView *>(this, "listview");
            lv->removeItem(6);
            lv->removeItem(0);
            lv->setTopPadding(15);
            lv->setContentSize(Size(768, Director::getInstance()->getVisibleSize().width - 350));
            lv->setPosition(Point(110 + lv->getContentSize().height / 2, 768 / 2));
            lv->setScrollBarEnabled(true);
            lv->setScrollBarAutoHideEnabled(true);
            lv->setScrollBarAutoHideTime(100.f);
            ui::Helper::doLayout(lv);
        } else {
//            auto saved = utils::findChild(this, "left_panel")->getContentSize().height;
//            utils::findChild(this, "left_panel")->removeFromParent();
            auto lv = utils::findChild<ui::ListView *>(this, "listview");
            lv->removeItem(5);
            lv->setTopPadding(10);
            lv->setContentSize(Size(768, Director::getInstance()->getVisibleSize().width - 330));
            lv->setPosition(Point(110 + lv->getContentSize().height / 2, 768 / 2));
            lv->setScrollBarEnabled(true);
            lv->setScrollBarAutoHideEnabled(true);
            lv->setScrollBarAutoHideTime(100.f);
//            lv->setInnerContainerSize(lv->getInnerContainerSize() - Size(0, saved));
            ui::Helper::doLayout(lv);
//            utils::findChild<ui::ListView *>(this, "listview_detail")->setEnabled(true);
        }
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto text_en = utils::findChild<ui::Text *>(this, "text_en");
    if (text_en) {
        auto str = text_en->getString();
        str = mj::helper::replace_string(str, "iTunes", "Google Play");
        str = mj::helper::replace_string(str, "AppStore", "Google Play");
        text_en->setString(str);
    }
#endif
}

void MJSubscriptionPage::onEnter() {
    auto event_paypopup_close = cocos2d::EventListenerCustom::create(k_event_pay_popup_close, [=](cocos2d::EventCustom * e) {
        auto pay_method = (mj::PayMethod *)(e->getUserData());
        this->_pay_method = * pay_method;
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_paypopup_close, this);

    auto event_coupon_change = cocos2d::EventListenerCustom::create(key_event_coupon_change, [=](cocos2d::EventCustom * e) {
        this->scheduleOnce([=](float dt) {
            this->_discount = mj::Pay::getInstance().getDiscountPercent();
            this->showPriceInfo();
            this->changeTextDisplayLang();
        }, 0.25, "delay_reload_price");
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_coupon_change, this);
    ui::Layout::onEnter();
    mj::Pay::getInstance().setPriceDelegate(this);
    mj::Pay::getInstance().refresh();
    
    if (CONFIG_MANAGER->isCountryVN()) {
//        auto label_coupon = Label::createWithTTF("", "fonts/monkeystories_thin.ttf", 20);
//        label_coupon->setName("label_coupon");
//        label_coupon->setColor(Color3B::BLUE);
//        auto text_coupon_csb = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.lbcoupon"));
//        text_coupon_csb->setVisible(false);
//        text_coupon_csb->getParent()->addChild(label_coupon);
//        label_coupon->setPosition(text_coupon_csb->getPosition());
//        if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
//            label_coupon->setRotation(-90);
//        }
    } else static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.lbcoupon"))->setVisible(false);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.lbrestore"))->setVisible(false);
#else
    auto label_restore = Label::createWithTTF("", "fonts/monkeystories_medium.ttf", 20);
    auto text_restore_csb = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.lbrestore"));
    text_restore_csb->setVisible(false);
    text_restore_csb->getParent()->addChild(label_restore);
    label_restore->setPosition(text_restore_csb->getPosition());
    label_restore->setName("label_restore");
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        label_restore->setRotation(-90);
    }
#endif
    
    changeTextDisplayLang();
    
    
    showCalculator();
}

void MJSubscriptionPage::changeTextDisplayLang(){
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
    static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.bubble"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.bubble"));
    if (CONFIG_MANAGER->isCountryVN()) {
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.feature1"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.feature1"));
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.feature2"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.feature2"));
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.feature3"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.feature3"));
    }

    
    //text_item
    for (int i = 0; i < (int) product_item.size(); i++) {
        product_item[i]->changeDisplayText();
    }

    utils::findChild(this, "btinfopagekage")->setVisible(lang_id == 4);
    utils::findChild(this, "btinappdetail")->setVisible(false);

    
    //coupon text
    if (CONFIG_MANAGER->isCountryVN()) {
        string text_coupon;
        auto label_coupon = cocos2d::utils::findChild<ui::Text *>(this, "pay.lbcoupon");
        label_coupon->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.lbcoupon30"));
        label_coupon->setTextColor(Color4B::BLACK);
        label_coupon->setVisible(true);
    }

    if (CONFIG_MANAGER->isAppActive()) {
        auto label_coupon = cocos2d::utils::findChild<ui::Text *>(this, "pay.lbcoupon");
        label_coupon->setVisible(true);
        if (mj::Pay::getInstance().getTimeExpire() == -1) {
            label_coupon->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.info.lifetime"));
        } else {
            if ((MJDEFAULT->getIntegerForKey(key_lang_display, 1)) == 1) {
                label_coupon->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.info.expiry") + LTDate(mj::Pay::getInstance().getTimeExpire()).toPrettyStringGB());
            } else {
                label_coupon->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.info.expiry") + LTDate(mj::Pay::getInstance().getTimeExpire()).toPrettyString());
            }
        }
    }
    
    //label restore
    auto text_restore = LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.lbrestore");
    std::vector<int> arr_find;
    if (lang_id == 1) {
        arr_find = NativeBridge::findStringOnString(text_restore, "Restore");
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    text_restore = LANGUAGE_MANAGER->replaceString(text_restore, "Apple", "Google");
#endif
        arr_find = NativeBridge::findStringOnString(text_restore, "tại");
    }
    
    auto label_restore = cocos2d::utils::findChild<Label *>(this, "label_restore");
    if (label_restore) {
        label_restore->setString("");
        label_restore->setString(text_restore);
        if ((int)arr_find.size() > 0) {
            for (int i = 0; i < NativeBridge::utf8LengthOfString(text_restore); i++) {
                auto letter = label_restore->getLetter(i);
                if (letter != NULL) {
                    if (i < arr_find[0]) {
                        letter->setColor(Color3B::BLACK);
                    }else{
                        letter->setColor(Color3B::ORANGE);
                    }
                }
            }
        }
    }
}

void MJSubscriptionPage::onExit() {
    ui::Layout::onExit();
    mj::Pay::getInstance().setPriceDelegate(nullptr);
}

void MJSubscriptionPage::loadData(){
    _pay_method = mj::PayMethod::MJPAY_NONE;
}

void MJSubscriptionPage::onSelectProduct(mj::InAppPackage product) {
    CCLOG("User did select product: %s", product.product_id.c_str());
    
    if (CONFIG_MANAGER->isCountryVN()) {
        if (_pay_method == mj::PayMethod::MJPAY_NONE) {
            auto pay_popup = MJPayPopup::createPopUp(_packages, product.product_id, _discount);
            cocos2d::Director::getInstance()->getRunningScene()->addChild(pay_popup, 100);
        } else {
            auto pay_popup = MJPaySubPopup::createPopup(_pay_method, _packages, product.product_id, _discount);
            cocos2d::Director::getInstance()->getRunningScene()->addChild(pay_popup, 100);
        }
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        mj::helper::setOrientationPortrait(!CONFIG_MANAGER->isHomeScreenLandscape());
#endif
        mj::InApp::getInstance().purchase(product.getProductIDStore(mj::Pay::getInstance().getDiscountPercent()));
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback MJSubscriptionPage::onLocateClickCallback(const std::string & callback_name) {
    if (callback_name == "onBack") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onBack(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
#endif
        
        return CC_CALLBACK_1(MJSubscriptionPage::onBack, this);
    }

    if (callback_name == "onRestore") {
        return CC_CALLBACK_1(MJSubscriptionPage::onRestore, this);
    }

    
    if (callback_name == "onInfoPageKage") {
        return CC_CALLBACK_1(MJSubscriptionPage::onInfoPageKage, this);
    }

    if (callback_name == "onPayDetails") {
        return CC_CALLBACK_1(MJSubscriptionPage::onPayDetails, this);
    }

    if (callback_name == "onPrivacy") {
        return CC_CALLBACK_1(MJSubscriptionPage::onPrivacy, this);
    }

    if (callback_name == "onTOS") {
        return CC_CALLBACK_1(MJSubscriptionPage::onTOS, this);
    }
    
    return nullptr;
}

void MJSubscriptionPage::onPrivacy(cocos2d::Ref * sender) {
    cocos2d::Application::getInstance()->openURL(key_privacy_policy_url);
}


void MJSubscriptionPage::onTOS(cocos2d::Ref * sender) {
    cocos2d::Application::getInstance()->openURL(key_terms_of_use_url);
}

void MJSubscriptionPage::onBack(cocos2d::Ref *sender) {
    this->removeFromParent();
}

void MJSubscriptionPage::onRestore(cocos2d::Ref * sender) {
    mj::Pay::getInstance().restore();
}


void MJSubscriptionPage::onGetPriceInfo(mj::ListPackages packages) {
    _packages = packages;
    showPriceInfo();
}

void MJSubscriptionPage::showPriceInfo() {
    _packages = mj::Pay::getInstance().getPackages();
    if (_packages.size() < 4) {
        return;
    }

    product_item.clear();

    for (auto i = 0; i < 4; i++){
        auto p = utils::findChild<MJSubscriptionItem *>(this, StringUtils::format("product_%d", i));
        if (!p) {
            continue;
        }
        p->setPackage(_packages[i]);
        p->setDiscount(_discount);
        p->reloadPrice();
        product_item.push_back(p);
    }
}

void MJSubscriptionPage::showCalculator() {
    if (!CONFIG_MANAGER->isCountryVN()) {
        if (MJDEFAULT->getBoolForKey(key_show_parent_lock, true))
            MJCalculator::showCalculator(this);
    }
}

void MJSubscriptionPage::onInfoPageKage(cocos2d::Ref *sender){
//    auto feedback = StoryFeedback::createPage();
//    DIRECTOR->getRunningScene()->addChild(feedback, 99999998);
//    
//    return;
    
    auto mjguide = MJPayGuide::createPage();
    DIRECTOR->getRunningScene()->addChild(mjguide, 99999998);
}

void MJSubscriptionPage::onPayDetails(cocos2d::Ref * sender) {
    mj::InAppPackage iap;
    auto details = MJPayDetails::createPopup(iap);
    Director::getInstance()->getRunningScene()->addChild(details, 999999);
}
