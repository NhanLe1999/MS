//
//  MJPay.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#include "MJPay.h"
#include "../external/json/writer.h"
#include "MJDefault.h"
#include "MJInApp.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"
#include "MJAPIListPay.h"
#include "MJDefinitions.h"
#include <numeric>
#include "platform/CCPlatformConfig.h"
#include "MSIntroGrade.h"
#include "MJDefinitions.h"
#include "MJHelper.h"

#include "IAPManager.h"
#include "APPurchaseManager.h"

#pragma mark - Define Constants

#define tagRequestPriceInfo 1000000
#define tagRequestLicense   1000001
#define tagRequestCoupon    1000002
#define tagRequestVerify    1000003
#define tagPopupPaySuccess  2000001

#pragma mark - Payment Functions

#include "MJDebugView.h"
#include "MJMailIntro.h"
#include "MSEventLogger.h"
#include "manager/CleverTapManager.h"

namespace mj {
    Pay::Pay() {
        _request_price_info = nullptr;
        _request_coupon = nullptr;
        _request_verify = nullptr;
        _request_price_info = nullptr;
        _should_verification_notify_ui = false;
        auto json_string = MJDEFAULT->getStringForKey(keyPriceInfo);

        _coupon = MJDEFAULT->getStringForKey(keyCoupon);
        _discount_percent = MJDEFAULT->getIntegerForKey(keyDiscount, 40);
        if (_coupon.empty() || _coupon == "40OFF") {
            _coupon = "";
            _discount_percent = 40;
            MJDEFAULT->setStringForKey(keyCoupon, _coupon);
            MJDEFAULT->setIntegerForKey(keyDiscount, _discount_percent);
        }

        _expire_time = MJDEFAULT->getIntegerForKey(keyTimeExpire);
        if (_expire_time == -1) {
            _is_active = true;
        } else if (_expire_time == 0) {
            _is_active = false;
        } else {
            auto current_time = time(nullptr);
            _is_active = current_time < _expire_time;
        }

        _license = MJDEFAULT->getStringForKey(keyLicense);

        if (json_string.empty()) {
//            cocos2d::MessageBox("Không lấy được giá. Nên luôn luôn có giá mặc định", "MJ");
            return;
        }

        rapidjson::Document d;
        d.Parse(json_string.c_str());
        parsePackagesFromData(d.GetArray());
    }

    void Pay::updateActiveStatus(bool is_online, bool is_active) {
        if (!is_online) {
            _expire_time = MJDEFAULT->getIntegerForKey(keyTimeExpire);
            if (_expire_time == -1) {
                _is_active = true;
            } else {
                auto current_time = time(nullptr);
                _is_active = current_time < _expire_time;
            }
        } else {
            _is_active = is_active;
        }
    }

    bool Pay::isActive() {
        if (MJDEFAULT->getBoolForKey(debugkey_full_features, false)) {
            return true;
        }

        auto expire_time = mj::Pay::getInstance().getTimeExpire();
        auto time_current = MJDEFAULT->getIntegerForKey(key_time_curent);

        if (APPURCHASE_MANAGER->isLifeTimeUser() || (APPURCHASE_MANAGER->isActive() && expire_time > time_current) || APPURCHASE_MANAGER->isUserTrial())
        {
            return true;
        }

        return false;
    }

    void Pay::refresh(std::string json_string) {
		if (json_string.empty()) {
			json_string = MJDEFAULT->getStringForKey(keyPriceInfo);
		}

        rapidjson::Document d;
        d.Parse(json_string.c_str());
        if(!d.IsNull() && d.IsArray())
        {
            parsePackagesFromData(d.GetArray());
        }

        if (_price_delegate) {
            _price_delegate->onGetPriceInfo(_packages);
        }

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
           // mj::InApp::getInstance().refresh({});
        });
    }

    mj::ListPackages Pay::getPackages() {
        return _packages;
    }
    void Pay::setPackages(std::unordered_map<std::string, InAppPackageModel> packages) {

        std::vector<std::string> time = {"1month", "6month", "1year", "lifetime"};

        std::vector<std::string> listProductID = {"1month", "6month", "1year", "lifetime"};

        std::vector<std::string> listKey = {};

        //packages = IAPManager::getInstance()->getPackages();




        _packages.clear();

        for(auto pk : packages)
        {
            auto key = pk.first;
            auto purchasePackageNew = pk.second;

            mj::InAppPackage p;

            p.product_id = purchasePackageNew._productId;
            p.currency = purchasePackageNew._currency;
            p.product_id_store = purchasePackageNew._productId;
            p.price_30 = purchasePackageNew._priceOrigin;
            p.price_store_30 = purchasePackageNew._priceOrigin;
            p.price_40 = purchasePackageNew._priceSale;
            p.price_store_40 = purchasePackageNew._priceSale;
            p.product_id_store = purchasePackageNew._productId;
            p.price_original = purchasePackageNew._priceOrigin;
            p.store_currency_code = purchasePackageNew._currency;

            _packages.push_back(p);
        }

    }

    void Pay::verify(std::string package_id, std::string receipt) {
        auto re = mj::InApp::getInstance().getReceipt("");
        std::cout << "receipt: " << re << std::endl;
        _request_verify = new mj::Request(mj::Request::ActionType::VERIFY_RECEIPT, {
            {"receipt", re},
            {"package_id", package_id},
            {"purchases_token", receipt},
            {"package_name", MJ_PLATFORM_CONFIG.getPackageName()}
        }, mj::Request::RequestType::POST);
        _request_verify->setTag(tagRequestVerify);
        _request_verify->setDelegate(this);
        _request_verify->send();
    }

    void Pay::enterLicense(const std::string &license, bool is_confirm) {
        _temp_license = license;
        if (!is_confirm) {
            _request_license = new mj::Request(mj::Request::ACTIVE_LICENSE_V2, {
                {"licence", license}
            }, mj::Request::RequestType::POST);
        } else {
            _request_license = new mj::Request(mj::Request::ACTIVE_LICENSE_V2, {
                {"licence", license},
                {"is_confirm", "1"}
            }, mj::Request::RequestType::POST);
        }
        _request_license->setTag(tagRequestLicense);
        _request_license->setDelegate(this);
        _request_license->send();
    }

    void Pay::setCoupon(const std::string &coupon) {
        _temp_coupon = coupon;
        _request_coupon = new mj::Request(mj::Request::VALIDATE_COUPON, {
            { "coupon", coupon }
        });
        _request_coupon->setTag(tagRequestCoupon);
        _request_coupon->setDelegate(this);
        _request_coupon->send();
    }

    void Pay::payWithStore(const std::string & product_id, const int & discount) {
        auto it = std::find_if(_packages.begin(), _packages.end(), [product_id](mj::InAppPackage iap) {
            return iap.product_id == product_id;
        });
        if (it == _packages.end()) {
            cocos2d::MessageBox("Package not found.", mj::PlatformConfig::getInstance().getAppName().c_str());
            return;
        }
        auto package = *it;
        mj::InApp::getInstance().purchase(package.getProductIDStore(_discount_percent));
    }

    void Pay::forceOverrideLicense(const std::string &license, const int &time_expire) {
        _license = license;
        _expire_time = time_expire;
        MJDEFAULT->setStringForKey(keyLicense, _license);
        MJDEFAULT->setIntegerForKey(keyTimeExpire, _expire_time);
    }

#pragma mark - Request Delegate
    void Pay::onRequestSuccess(int tag, rapidjson::Value data) {
        switch (tag) {
            case tagRequestPriceInfo:
//                onRequestPriceInfoSuccess(data.GetArray());
                return;

            case tagRequestVerify:
                onVerifySuccess(data.GetObject());
                return;

            case tagRequestLicense:
                onActiveLicenseSuccess(data.GetObject());
                return;

            case tagRequestCoupon:
                onValidateCouponSuccess(data.GetObject());
                return;

            default:
                break;
        }
    }

    void Pay::onRequestFailed(int tag, int error_code, std::string error_string, rapidjson::Value data) {
        switch (tag) {
            case tagRequestPriceInfo:
//                onRequestPriceInfoFailed();
                break;

            case tagRequestVerify:
                onVerifyFailed(error_string);
                break;

            case tagRequestLicense:
                if (data.IsObject()) {
                    onActiveLicenseFailed(error_code, error_string, data.GetObject());
                }else {
                    onActiveLicenseFailed(error_code, error_string, rapidjson::Value(0));
                }

                break;

            case tagRequestCoupon:
                onValidateCouponFailed(error_string);
                break;

            default:
                break;
        }
    }

#pragma mark - Validate Coupon Delegate
    void Pay::onValidateCouponSuccess(rapidjson::Value data) {
        _coupon = _temp_coupon;
        _discount_percent = 40;
        MJDEFAULT->setStringForKey(keyCoupon, _coupon);
        MJDEFAULT->setIntegerForKey(keyDiscount, 40);
        if (_coupon_delegate) {
            _coupon_delegate->onValidateCouponSuccess(_temp_coupon, 40);
        }
    }

    void Pay::onValidateCouponFailed(const std::string & error) {
        if (_coupon_delegate) {
            _coupon_delegate->onValidateCouponFailed(_temp_coupon, error);
        }
    }

#pragma mark - Active License Delegate
    void Pay::onActiveLicenseSuccess(rapidjson::Value data) {
        _is_active = true;
        _expire_time = data["data"]["time_expire"].GetInt();
        if (_expire_time == 0) {
            _expire_time = -1;
        }
        MJDEFAULT->setIntegerForKey(keyTimeExpire, _expire_time);
        MJDEFAULT->setStringForKey(keyLicense, _temp_license);

        std::string msg = "";
        if (data.HasMember("message") && data["message"].IsString()) {
            msg = data["message"].GetString();
        }

        _license = _temp_license;
        if (_license_delegate) {
            if (data.IsObject()) {
                _license_delegate->onActiveLicenseSuccess(_temp_license, _expire_time, msg, data.GetObject());
            }else {
                _license_delegate->onActiveLicenseSuccess(_temp_license, _expire_time, msg, rapidjson::Value(0));
            }
        }
    }

    void Pay::onActiveLicenseFailed(const int & error_code, const std::string & error, rapidjson::Value data) {
        if (_license_delegate) {
            if (data.IsObject()) {
                _license_delegate->onActiveLicenseFailed(_temp_license, error_code, error, data.GetObject());
            }else {
                _license_delegate->onActiveLicenseFailed(_temp_license, error_code, error, rapidjson::Value(0));
            }
        }
    }

#pragma mark - Verify Delegate
    void Pay::onVerifySuccess(rapidjson::Value data) {
        CCLOG("Verify success");
        MJDEFAULT->setStringForKey(keyReceipt, mj::InApp::getInstance().getReceipt(""));
        auto obj = data.GetObject();
        _is_active = data["is_active"].GetBool();
        _expire_time = data["time_expire"].GetInt();
        if (_expire_time == 0) {
            _expire_time = -1;
        }
        MJDEFAULT->setIntegerForKey(keyTimeExpire, _expire_time);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_app_become_purchased);
        });

        if (_should_verification_notify_ui) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "pay.verify.success"), "OK", "", false, 24, this, tagPopupPaySuccess);
            });
        }
        _should_verification_notify_ui = false;
    }

    void Pay::onVerifyFailed(std::string message) {
        CCLOG("Verify failed");
        MJDEFAULT->setStringForKey(keyReceipt, mj::InApp::getInstance().getReceipt(""));

        CCLOG("Show alert");
        if (_should_verification_notify_ui) {
            if (message.empty()) {
                message = LANGUAGE_MANAGER->getDisplayTextByKey("pay.verify.failed");
            }
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([message, this]
            {
                StoryAlert::showAlert(message, "OK", "", false, 24, this);
            });
        }
        CCLOG("Restore state");
        _should_verification_notify_ui = false;
    }

#pragma mark - In App Purchase Listener
    void Pay::onProductRequestSuccess(const std::vector<InAppProduct> &products)
    {
        CCLOG("Pay::onProductRequestSuccess");

        for (auto product : products)
        {
            auto it = std::find_if(_packages.begin(), _packages.end(), [product](mj::InAppPackage iap)
            {
                const std::vector<std::string> duration = {"1month", "1year", "6month", "lifetime"};

                for (auto d : duration)
                {
                    if (iap.product_id.find(d) != std::string::npos && product.product_id.find(d) != std::string::npos)
                    {
                        return true;
                    }
                }

                return false;
                //                auto temp = iap.product_id_store + std::string(".30_off");
                //                return (temp == product.product_id);
            });

            if (it != _packages.end())
            {
                it->price_store_30      = product.price;
                it->store_currency_code = product.currency_symbol;
            }

            CCLOG("Pay::onProductRequestSuccess|find1");

            it = std::find_if(_packages.begin(), _packages.end(), [product](mj::InAppPackage iap)
            {
                const std::vector<std::string> duration = {"1month", "1year", "6month", "lifetime"};
                for (auto d : duration)
                {
                    if (iap.product_id.find(d) != std::string::npos && product.product_id.find(d) != std::string::npos)
                    {
                        return true;
                    }
                }
                return false;
            });

            CCLOG("Pay::onProductRequestSuccess|find2");

            if (it != _packages.end())
            {
                it->price_store_40 = product.price;
                it->store_currency_code = product.currency_symbol;
#if !CC_PLATFORM_HUAWEI
                it->store_currency_code = product.currency_code;
#endif
            }

        }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (_price_delegate && !CC_PLATFORM_HUAWEI)
        {
            _price_delegate->onGetPriceInfo(_packages);
        }
#else
        CCLOG("Pay::onProductRequestSuccess end, dispatchCustomEvent key_event_coupon_change");

        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_coupon_change);
#endif
    }

    void Pay::onSuccess(const InAppProduct & product)
    {
        _should_verification_notify_ui = true;
        verify(product.product_id, product.receipt);
        ms::EventLogger::getInstance().logEvent("pay_inapp_successful", {{"product_id", cocos2d::Value(product.product_id) }});

        CCLOG("Pay::onSuccess|%s",product.product_id.c_str());

        try
        {
            std::string source = MJDEFAULT->getStringForKey(key_source_subscribe, "Clevertap");

            ms::EventLogger::getInstance().logEvent("Charged", {
            {"property_price", cocos2d::Value (MJDEFAULT->getStringForKey(key_property_price))},
            {"choose_package", cocos2d::Value (MJDEFAULT->getStringForKey(key_choose_package))},
            {"package_length", cocos2d::Value (MJDEFAULT->getStringForKey(key_package_length))},
            {"purchased_successful", cocos2d::Value ("true")}
                }, true);

            CleverTapManager::GetInstance()->pushEvent("purchase_screen_popup_confirm",
            {
                {"click_type",cocos2d::Value("buy now")},
                {"source",cocos2d::Value(source)},
                {"purchased_successful",cocos2d::Value("true")},
            });
        }
        catch (const std::exception&)
        {

        }
    }

    void Pay::onFailed(const InAppProduct &product)
    {
        CCLOG("Pay::onFailed|%s",product.product_id.c_str());

        ms::EventLogger::getInstance().logEvent("pay_inapp_fail", {{"product_id", cocos2d::Value(product.product_id) }});
        Director::getInstance()->getRunningScene()->scheduleOnce([](float) {
            StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "pay.purchase.failed"), "OK", "");
        }, 0.25, "delay_failed_alert");

        try
        {
            CleverTapManager::GetInstance()->pushEvent("Charged", {
                {"property_price", cocos2d::Value(MJDEFAULT->getStringForKey(key_property_price))},
                {"choose_package", cocos2d::Value(MJDEFAULT->getStringForKey(key_choose_package))},
                {"package_length", cocos2d::Value(MJDEFAULT->getStringForKey(key_package_length))},
                {"purchased_successful", cocos2d::Value("false")},
           });
        }
        catch (const std::exception&)
        {

        }

//        _should_verification_notify_ui = true;
//        verify(product.product_id, product.receipt);
    }

#pragma mark - Request Price Info
    void Pay::parsePackagesFromData(rapidjson::Value data) {

        /*auto cc = IAPManager::getInstance()->getPackages();

*//*#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && !CC_PLATFORM_HUAWEI))
    sort(_packages.begin(), _packages.end(), [](InAppPackage p1, InAppPackage p2) {
        return p1.price_40 < p2.price_40;
    });
#endif*//*
        auto currentPackagesOnStoreMap			= IAPManager::getInstance()->getCurrentSellingPackagesOnStore();
        auto currentOfflinePackagesOnStoreMap	= IAPManager::getInstance()->getCurrentOfflineSellingPackagesOnStore();

        std::vector<std::string> arr_ids;
        arr_ids = std::accumulate(_packages.begin(), _packages.end(), arr_ids, [=](std::vector<std::string> v, mj::InAppPackage iap) {
            v.push_back(iap.getProductIDStore(_discount_percent));
            return v;
        });

        mj::InApp::getInstance().init(arr_ids);
        mj::InApp::getInstance().setListener(this);*/
    }

    void Pay::payWithOnePay(std::string product_id) {
        mj::InApp::getInstance().payWithOnePay(product_id, _coupon);
    }

    void Pay::restore() {
        _should_verification_notify_ui = true;
        ms::EventLogger::getInstance().logEvent("restore_inapp", {{"product_id", cocos2d::Value("") }});
        mj::InApp::getInstance().restore();
    }

    void Pay::onRestoreComplete(bool ok, const std::string &message) {
        CCLOG("restore_inapp_successful");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ms::EventLogger::getInstance().logEvent("restore_inapp_successful", {{"product_id", cocos2d::Value("") }});
        CleverTapManager::GetInstance()->pushEvent("restore", {{"success",cocos2d::Value(true)} });
        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "pay.restore.complete"), "OK", "", false, 24, this);
        return;
#else
        if (ok)
        {
            CleverTapManager::GetInstance()->pushEvent("restore", {{"success",cocos2d::Value(true)} });
            ms::EventLogger::getInstance().logEvent("restore_inapp_successful", {{"product_id", cocos2d::Value("") }});
            verify("", "");
        }
        else
        {
            CleverTapManager::GetInstance()->pushEvent("restore", {{"success",cocos2d::Value(false)} });
            ms::EventLogger::getInstance().logEvent("restore_inapp_fail", {{"product_id", cocos2d::Value("") }});
            Director::getInstance()->getRunningScene()->scheduleOnce([] (float) {
                StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "pay.restore.failed"), "OK", "");
            }, 0.1f, "delay_alert_tach_hs9");
        }
#endif
    }

    void Pay::onAlertOK(cocos2d::Ref *sender) {
        auto tag = reinterpret_cast<Node *>(sender)->getTag();
        if (tag == tagPopupPaySuccess && CONFIG_MANAGER->isCountryVN() && CONFIG_MANAGER->isAppVNStories()) {
            auto intro = MJMailIntro::createGiftView();
            Director::getInstance()->getRunningScene()->addChild(intro, 1000000);
            intro->setCallback([] {
                MJDEFAULT->setBoolForKey(key_first_api_called, true);
                Director::getInstance()->replaceScene(MSIntroGrade::createScene());
            });
            return;
        }
        MJDEFAULT->setBoolForKey(key_first_api_called, true);
        Director::getInstance()->replaceScene(MSIntroGrade::createScene());
    }

    void Pay::onAlertCancel(cocos2d::Ref *sender) {
        MJDEFAULT->setBoolForKey(key_first_api_called, true);
        Director::getInstance()->replaceScene(MSIntroGrade::createScene());
    }
}
