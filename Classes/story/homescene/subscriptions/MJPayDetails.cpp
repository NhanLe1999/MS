//
// Created by To Ngoc Linh on 9/12/17.
//

#include "MJPayDetails.h"
#include "StoryConfigManager.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "MJPay.h"
#include "platform/CCPlatformConfig.h"
#include "MJPlatformConfig.h"
USING_NS_CC;
INITIALIZE_READER(MJPayDetails);

std::once_flag mj_pay_details_reader;

MJPayDetails * MJPayDetails::createPopup(mj::InAppPackage package) {
    std::call_once(mj_pay_details_reader, [] {
        REGISTER_CSB_READER(MJPayDetails);
    });

//    auto csb = CONFIG_MANAGER->isHomeScreenLandscape() ? "csb/pay/MJPayDetails.csb" : "csb/pay/MJPayDetails_Portrait.csb";

    auto csb = "csb/pay/MJPayDetails.csb";
    auto popup = static_cast<MJPayDetails *>(CSLoader::createNodeWithVisibleSize(csb));
    if (popup) {
        popup->didLoadFromCSB(package);
        return popup;
    }

    CC_SAFE_DELETE(popup);

    return nullptr;
}

void MJPayDetails::didLoadFromCSB(mj::InAppPackage package) {
    _package = package;
    auto str_details = std::string("");
    auto str_price = std::string("");

    auto price = StringUtils::format("%0.2f %s", package.price_store_40, package.store_currency_code.c_str());
    if (package.price_store_40 == 0) {
        price = "---";
    }

    auto period = std::string("");
    if (_package.product_id.find("year") != std::string::npos) {
        period = "yearly";
    }
    if (_package.product_id.find("month") != std::string::npos) {
        period = "monthly";
    }
    if (_package.product_id.find("lifetime") != std::string::npos) {
        period = "lifetime";
    }

    if (period == "lifetime") {
        str_price = "$(price) one time, lifetime unlimited access to all stories, games";
        str_details = "Payment will be charged to iTunes Account at confirmation of purchase \n\n"
                "You can restore your purchase on other iOS devices by tapping on our restore button on this purchase section\n\n"
                "Privacy Policy\n"
                "          $(privacy_policy_url)\n\n"
                "Terms of Service\n"
                "          $(tos_url)";
    } else if (period == "yearly") {
        str_price = "$(price) a year, unlimited access to all stories, games";
        str_details = "Payment will be charged to iTunes Account at confirmation of purchase \n\n"
                "Subscription automatically renews unless auto-renew is turned off at least 24-hours before the end of the current period \n\n"
                "Account will be charged for renewal within 24-hours prior to the end of the current period, and identify the cost of the renewal \n\n"
                "Subscriptions may be managed by the user and auto-renewal may be turned off by going to the user’s Account Settings after purchase\n\n"
                "No cancellation of the current subscription is allowed during active subscription period\n\n"
                "Any unused portion of a free trial period will be forfeited when the user purchases a subscription to $(appname)\n\n"
                "Privacy Policy\n"
                "          $(privacy_policy_url)\n\n"
                "Terms of Service\n"
                "          $(tos_url)";
        
    } else if (period == "monthly") {
        str_price = "$(price) a month, unlimited access to all stories, games";
        str_details = "Payment will be charged to iTunes Account at confirmation of purchase \n\n"
                "Subscription automatically renews unless auto-renew is turned off at least 24-hours before the end of the current period \n\n"
                "Account will be charged for renewal within 24-hours prior to the end of the current period, and identify the cost of the renewal \n\n"
                "Subscriptions may be managed by the user and auto-renewal may be turned off by going to the user’s Account Settings after purchase\n\n"
                "No cancellation of the current subscription is allowed during active subscription period\n\n"
                "Any unused portion of a free trial period will be forfeited when the user purchases a subscription to $(appname)\n\n"
                "Privacy Policy\n"
                "          $(privacy_policy_url)\n\n"
                "Terms of Service\n"
                "          $(tos_url)";
    } else {
        str_details = "Payment will be charged to iTunes Account at confirmation of purchase \n\n"
                "Subscription automatically renews unless auto-renew is turned off at least 24-hours before the end of the current period \n\n"
                "Account will be charged for renewal within 24-hours prior to the end of the current period, and identify the cost of the renewal \n\n"
                "Subscriptions may be managed by the user and auto-renewal may be turned off by going to the user’s Account Settings after purchase\n\n"
                "No cancellation of the current subscription is allowed during active subscription period\n\n"
                "Any unused portion of a free trial period will be forfeited when the user purchases a subscription to $(appname)\n\n"
                "Privacy Policy\n"
                "          $(privacy_policy_url)\n\n"
                "Terms of Service\n"
                "          $(tos_url)";
    }

    str_price = mj::helper::replace_string(str_price, "$(price)", price);
    str_details = mj::helper::replace_string(str_details, "$(privacy_policy_url)", key_privacy_policy_url);
    str_details = mj::helper::replace_string(str_details, "$(tos_url)", key_terms_of_use_url);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    str_details = mj::helper::replace_string(str_details, "iTunes", "Google Play");
    str_details = mj::helper::replace_string(str_details, "iOS", "Android");
#endif
    str_details = mj::helper::replace_string(str_details, "$(appname)", mj::PlatformConfig::getInstance().getAppName());
    auto text = utils::findChild<ui::Text *>(this, "text_details");
    text->setString(str_details);
    text->ignoreContentAdaptWithSize(false);
    text->setTextAreaSize(cocos2d::Size(text->getContentSize().width, 0));
    text->setContentSize(text->getVirtualRendererSize());

    auto text_price = utils::findChild<ui::Text *>(this, "text_price");
    text_price->setString(str_price);

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        utils::findChild(this, "img")->setRotation(-90);
    }

//    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
//        utils::findChild(this, "img")->setContentSize(text->getContentSize() + Size(100, 150));
//    } else {
//        auto size = Size(text->getContentSize().height, text->getContentSize().width);
//        utils::findChild(this, "img")->setContentSize(size + Size(150, 50));
//    }
    ui::Helper::doLayout(this);

    auto content = utils::findChild(this, "content");
    content->setCascadeOpacityEnabled(false);
//    auto sv = utils::findChild<ui::ScrollView *>(this, "scrollview");
//    sv->setScrollBarEnabled(false);
//    sv->setEnabled(false);

    auto inner_size = text->getContentSize();

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        inner_size = Size(inner_size.height, inner_size.width);
    }

//    sv->setInnerContainerSize(inner_size);
//    text->setPosition(sv->getInnerContainerSize() / 2);

    if (package.product_id.empty()) {
        Vec2 translate_vector;
        if (CONFIG_MANAGER->isHomeScreenLandscape()) {
            translate_vector = Vec2(0, -75);
        } else {
            translate_vector = Vec2(50, 0);
        }
        utils::findChild(this, "button_continue")->setVisible(false);
        utils::findChild(this, "text_title")->setVisible(true);
//        sv->setPosition(sv->getPosition() + translate_vector);
    }
}

void MJPayDetails::onEnter() {
    ui::Layout::onEnter();
    auto csb = CONFIG_MANAGER->isHomeScreenLandscape() ? "csb/pay/MJPayDetails.csb" : "csb/pay/MJPayDetails_Portrait.csb";

    auto timeline = CSLoader::createTimeline(csb);
    timeline->play("show", false);
    this->runAction(timeline);
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayDetails::onLocateClickCallback(const std::string &name) {
    if (name == "onBack") {
        return CC_CALLBACK_1(MJPayDetails::onBack, this);
    }

    if (name == "onContinue") {
        return CC_CALLBACK_1(MJPayDetails::onContinue, this);
    }

    return nullptr;
}

void MJPayDetails::onBack(Ref * sender) {
    auto csb = CONFIG_MANAGER->isHomeScreenLandscape() ? "csb/pay/MJPayDetails.csb" : "csb/pay/MJPayDetails_Portrait.csb";

    auto timeline = CSLoader::createTimeline(csb);
    timeline->play("hide", false);
    timeline->setLastFrameCallFunc([=] {
        this->removeFromParent();
    });
    this->runAction(timeline);
}

void MJPayDetails::onContinue(Ref * sender) {
    this->onBack(nullptr);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mj::helper::setOrientationPortrait(!CONFIG_MANAGER->isHomeScreenLandscape());
#endif
    mj::InApp::getInstance().purchase(_package.getProductIDStore(mj::Pay::getInstance().getDiscountPercent()));
}
