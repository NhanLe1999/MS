//
//  MSInstallPromotion.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/21/19.
//

#include "MSInstallPromotion.h"
#include "MJHelper.h"

USING_NS_CC;

INITIALIZE_READER(MSInstallPromotion);

#define kRC_promotion "install_promotion"
#define kRC_promotionContent "install_promotion_content"
#define kRC_promotionAction "install_promotion_action"

std::once_flag ms_install_promotion_reader_flag;

#include "MJPay.h"
#include "MJDefault.h"
bool MSInstallPromotion::shouldShowPopup() {
    return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	return false;
#endif
    if (!CONFIG_MANAGER->isCountryVN()) {
        return false;
    }

    if (!CONFIG_MANAGER->isAppVNStories()) {
        return false;
    }

    auto voucher = MJDEFAULT->getStringForKey("key.install.voucher");
    if (!voucher.empty()) {
        return false;
    }
    auto seed = random(1, 3);
    if (seed == 1 && !mj::Pay::getInstance().isActive()) {
        return true;
    }
    
    if (MJDEFAULT->getBoolForKey("key.force.show.install.promotion", false)) {
        MJDEFAULT->setBoolForKey("key.force.show.install.promotion", false);
        return true;
    }
    
    return false;
}

bool MSInstallPromotion::shouldShowNotification() {
    return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return false;
#endif
    if (!CONFIG_MANAGER->isCountryVN()) {
        return false;
    }

    if (!CONFIG_MANAGER->isAppVNStories()) {
        return false;
    }

    auto voucher = MJDEFAULT->getStringForKey("key.install.voucher");
    if (!voucher.empty()) {
        return false;
    }

    if (!mj::Pay::getInstance().isActive()) {
        return true;
    }
    
    if (MJDEFAULT->getBoolForKey("key.force.show.install.promotion", false)) {
        MJDEFAULT->setBoolForKey("key.force.show.install.promotion", false);
        return true;
    }
    
    return false;
}

MSInstallPromotion * MSInstallPromotion::createView(bool is_popup) {
    std::call_once(ms_install_promotion_reader_flag, [] {
        REGISTER_CSB_READER(MSInstallPromotion);
    });
    
    auto node = reinterpret_cast<MSInstallPromotion *>(CSLoader::createNode("csb/Install_Promotion.csb"));
    if (node) {
        node->didLoadFromCSB(is_popup);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "firebase/remote_config.h"
#endif
void MSInstallPromotion::didLoadFromCSB(bool is_popup) {
    this->_is_popup = is_popup;
    utils::findChild(this, "btfree")->setCascadeColorEnabled(false);
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    std::string content = firebase::remote_config::GetString(kRC_promotionContent);
//    content = mj::helper::replace_string(content, "<br>", "\n");
//    utils::findChild<ui::Text *>(this, "content")->setString(content);
//    std::string promotion = firebase::remote_config::GetString(kRC_promotion);
//    promotion = mj::helper::replace_string(promotion, "<br>", "\n");
//    utils::findChild<ui::Text *>(this, "title")->setString(promotion);
//    utils::findChild<ui::Text *>(this, "titlebtn")->setString(firebase::remote_config::GetString(kRC_promotionAction));
//#else
//    utils::findChild<ui::Text *>(this, "content")->setString("Đăng ký ngay để nhận được mã giảm giá lên đến 50% và bộ 5 quyển truyện tranh trị giá 150.000đ");
//    utils::findChild<ui::Text *>(this, "title")->setString("Giảm giá\nđến 50%");
//    utils::findChild<ui::Text *>(this, "titlebtn")->setString("Đăng ký");
//#endif
    
    if (is_popup) {
        auto background = ui::Layout::create();
        background->setContentSize(Director::getInstance()->getVisibleSize());
        background->setBackGroundColor(Color3B::BLACK);
        background->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        background->setBackGroundColorOpacity(0xbb);
        background->setAnchorPoint(Point::ANCHOR_MIDDLE);
        background->setPosition(this->getContentSize() / 2);
        background->setTouchEnabled(true);
        this->addChild(background, -1);
        this->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    } else {
        utils::findChild(this, "btclose")->setVisible(false);
    }
}

void MSInstallPromotion::onEnter() {
    ui::Layout::onEnter();
}

void MSInstallPromotion::onExit() {
    ui::Layout::onExit();
}

void MSInstallPromotion::onClose(Ref * sender) {
    this->removeFromParent();
}

#include "MJMailIntro.h"
void MSInstallPromotion::onReceive(Ref * sender) {
    auto intro = MJMailIntro::createView(true);
    Director::getInstance()->getRunningScene()->addChild(intro, 1000000);
    if (_is_popup) {
        this->removeFromParent();
    }
}

ui::Widget::ccWidgetClickCallback MSInstallPromotion::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSInstallPromotion, onClose),
        CLICK_MAP(MSInstallPromotion, onReceive)
    };

    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

