//
//  MJPayPopup_BankTransfer.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPayPopup_BankTransfer.h"
#include "MJPayPopup.h"
#include "MJPayPopup_BankInfo.h"
INITIALIZE_READER_INSTANCE(MJPayPopup_BankTransferReader);

MJPayPopup_BankTransfer * MJPayPopup_BankTransfer::createPopup(const mj::ListPackages & packages, const std::string &product_id, const int & discount) {
    auto p = static_cast<MJPayPopup_BankTransfer *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/pay/MJPayPopup_BankTransfer.csb"));
    if (p) {
        p->didLoadFromCSB(packages, product_id, discount);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayPopup_BankTransfer::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJPayPopup_BankTransfer, onNext),
        CLICK_MAP(MJPayPopup_BankTransfer, onBack),
        CLICK_MAP(MJPayPopup_BankTransfer, onSwitch)
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}

void MJPayPopup_BankTransfer::onNext(cocos2d::Ref *sender) {
    auto bank_info = MJPaySubPopup::createPopup(mj::PayMethod::MJPAY_BANK_INFO, _packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(bank_info, INT_MAX - 1);
    this->removeFromParent();
}

void MJPayPopup_BankTransfer::didLoadFromCSB(mj::ListPackages packages, std::string product_id, int _discount){
    MJPaySubPopup::didLoadFromCSB(packages, product_id, _discount);
    auto bank_content_lb = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "bank_content_lb"));
    auto str_content = bank_content_lb->getString();
    std::string price_pagekage;
    for (int i = 0; i < (int) packages.size(); i++) {
        if (packages[i].product_id == product_id) {
            price_pagekage = mj::helper::prettyInteger(packages[i].getActualPrice(_discount));
            break;
        }
    }
    
    str_content = mj::helper::replace_string(str_content, "$(price)", price_pagekage);
    bank_content_lb->setString(str_content);

}
