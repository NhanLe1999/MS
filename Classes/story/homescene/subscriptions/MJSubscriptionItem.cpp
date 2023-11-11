//
//  MJSubscriptionItem.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJSubscriptionItem.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "StoryConfigManager.h"
#include "MJPay.h"
#include "MJPayDetails.h"

INITIALIZE_READER_INSTANCE(MJSubscriptionItemReader);

cocos2d::ui::Widget::ccWidgetClickCallback MJSubscriptionItem::onLocateClickCallback(const std::string &call_back) {
    if (call_back == "onClick")
        return CC_CALLBACK_1(MJSubscriptionItem::onClick, this);

    if (call_back == "onDetails") {
        return CC_CALLBACK_1(MJSubscriptionItem::onDetails, this);
    }

    return nullptr;
}

void MJSubscriptionItem::onClick(cocos2d::Ref *sender) {

    if (_delegate) {
        _delegate->onSelectProduct(this->getPackage());
    }
}

void MJSubscriptionItem::reloadPrice() {
    auto packages = mj::Pay::getInstance().getPackages();
    auto it = std::find_if(std::begin(packages), std::end(packages), [=](mj::InAppPackage iap) {
        return iap.product_id == _package.product_id;
    });

    if (it != std::end(packages)) {
        _package = *it;
    }

    auto is_vn = CONFIG_MANAGER->isCountryVN();

    utils::findChild(this, "panel_price_vi")->setVisible(is_vn);
    utils::findChild(this, "panel_price_en")->setVisible(!is_vn);

    changeDisplayText();

    double actual_price = 0;
    std::string currency_code = std::string("");
    if (is_vn) {
        actual_price = _package.getActualPrice(_discount);
        currency_code = "₫";
    } else {
        actual_price = _package.getStorePrice(_discount);
        currency_code = _package.store_currency_code;
    }
    auto text_price = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "price"));
    if (actual_price == 0) {
        text_price->setString("---");
    } else {
        if (is_vn)
            text_price->setString(cocos2d::StringUtils::format("%s %s", mj::helper::prettyInteger((int)actual_price).c_str(), currency_code.c_str()));
        else {
            if (actual_price == (int) actual_price && actual_price >= 1000) {
                text_price->setString(cocos2d::StringUtils::format("%s %s", currency_code.c_str(), mj::helper::prettyInteger((int)actual_price).c_str()));
            } else {
                text_price->setString(cocos2d::StringUtils::format("%s %0.2f", currency_code.c_str(), actual_price));
            }
        }
    }

    auto text_price_average = utils::findChild<ui::Text *>(this, "price_average");
    int n_month = 1;
    if (_package.product_id == "com.earlystart.stories.1month") {
        n_month = 1;
    } else if (_package.product_id == "com.earlystart.stories.6month") {
        n_month = 1;
    } else if (_package.product_id == "com.earlystart.stories.1year") {
        n_month = 1;
    }

    if (actual_price == 0) {
        text_price_average->setString("---");
    } else {
        if (is_vn)
            text_price_average->setString(StringUtils::format("%s %s", mj::helper::prettyInteger((int)(actual_price / n_month)).c_str(), currency_code.c_str()));
        else {
            if (actual_price == (int) actual_price && actual_price >= 1000) {
                text_price_average->setString(cocos2d::StringUtils::format("%s %s", currency_code.c_str(), mj::helper::prettyInteger((int)actual_price).c_str()));
            } else {
                text_price_average->setString(cocos2d::StringUtils::format("%s %0.2f", currency_code.c_str(), actual_price));
            }
        }
    }

    auto text_name = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "name"));
    text_name->setString(_package.product_name);
    cocos2d::ui::Helper::doLayout(this);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    utils::findChild(this, "panel_price_vi")->setVisible(is_vn);
    utils::findChild(this, "panel_price_en")->setVisible(!is_vn);
#endif
}

void MJSubscriptionItem::changeDisplayText(){
    CCLOG("name item = %s", this->getName().c_str());

    utils::findChild<ui::Text *>(this, "name")->setString(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), std::string("product.name.") + _package.product_id));

    auto name_item = this->getName();
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    auto action_label = utils::findChild<ui::Text *>(this, "action");
    if (_package.product_id.find("lifetime") != std::string::npos) {
        action_label->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.action.buy"));
    } else {
        action_label->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.action.subscribe"));
    }

    //set text
    if (name_item == "product_0") {
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.price_detail"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.price_detail1"));
    }else if (name_item == "product_1"){
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.price_detail"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.price_detail3"));
    }else if (name_item == "product_2") {
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.price_detail"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.price_detail2"));
    }else {
        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.price_detail"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.price_detail4"));
    }

    //set best seller
    if (name_item == "product_2" && !CONFIG_MANAGER->isCountryVN()) {
        cocos2d::utils::findChild(this, "img_bestseller")->setVisible(true);
//        static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "pay.lbbestseller"))->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "pay.lbbestseller"));
    }
}

void MJSubscriptionItem::onDetails(cocos2d::Ref *sender) {
    auto details = MJPayDetails::createPopup(_package);
    Director::getInstance()->getRunningScene()->addChild(details, 999999);
}

void MJSubscriptionItem::onEnter() {
    ui::Layout::onEnter();
    if (CONFIG_MANAGER->isCountryVN()) {
        utils::findChild(this, "button_details")->setVisible(false);
        //centered label
        utils::findChild(this, "name")->setPositionX(100.f);
    }
}
