//
//  MJPayPopup_Store.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPayPopup_Store.h"
#include "MJHelper.h"
#include "platform/CCPlatformConfig.h"
#include "MSEventLogger.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "MJPay.h"
#endif
INITIALIZE_READER_INSTANCE(MJPayPopup_StoreReader);

MJPayPopup_Store * MJPayPopup_Store::createPopup(const mj::ListPackages & packages, const std::string &product_id, const int & discount) {
    auto p = static_cast<MJPayPopup_Store *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/pay/MJPayPopup_Store.csb"));
    if (p) {
        p->didLoadFromCSB(packages, product_id, discount);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayPopup_Store::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJPayPopup_Store, onNext),
        CLICK_MAP(MJPayPopup_Store, onBack),
        CLICK_MAP(MJPayPopup_Store, onSwitch)
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}


void MJPayPopup_Store::onNext(cocos2d::Ref *sender) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutStep2, {
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)},
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("store")}
    });
    mj::Pay::getInstance().payWithStore(_product_id, _discount);
#endif
    
}

void MJPayPopup_Store::didLoadFromCSB(mj::ListPackages packages, std::string product_id, int _discount) {
    MJPaySubPopup::didLoadFromCSB(packages, product_id, _discount);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    utils::findChild<ui::Text *>(this, "title")->setString("Thanh toán qua\ntài khoản Google");
    utils::findChild<ui::Text *>(this, "description")->setString("Bạn sẽ không nhận được mã kích hoạt. Thiết bị này sẽ được tự động kích hoạt. Bạn có thể khôi phục gói mua trên các thiết bị cùng hệ điều hành Android khác. Chúng tôi sẽ kiểm tra gói mua và tổng số lượng thiết bị được kích hoạt.");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    utils::findChild<ui::Text *>(this, "description")->setString("Bạn sẽ không nhận được mã kích hoạt. Thiết bị này sẽ được tự động kích hoạt. Bạn có thể khôi phục gói mua trên các thiết bị cùng hệ điều hành OS X (macOS) khác. Chúng tôi sẽ kiểm tra gói mua và tổng số lượng thiết bị được kích hoạt.");
#endif
}
