//
// Created by To Ngoc Linh on 9/16/17.
//

#include "MJAPIVerifyApple.h"
#include "MJInApp.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

void mj::VerifyAppleAPI::call(std::function<void(bool)> callback) {
    auto api = new VerifyAppleAPI(callback);
    api->send();
}

void mj::VerifyAppleAPI::send() {
    if (_request)
        _request->send();
}

#include "MJPlatformConfig.h"
mj::VerifyAppleAPI::VerifyAppleAPI(std::function<void(bool)> callback) {
    _callback = callback;
    
//    if (!MJDEFAULT->getBoolForKey("receipt_need_verified", false)) {
//        _callback ? _callback(false) : (void) nullptr;
//        return;
//    }
    
    auto receipt = mj::InApp::getInstance().getReceipt("");
    
    _request = new mj::Request(mj::Request::ActionType::VERIFY_RECEIPT, {
            {"receipt", receipt},
            {"package_id", ""},
            {"purchases_token", ""},
            {"package_name", MJ_PLATFORM_CONFIG.getPackageName()}
    }, mj::Request::RequestType::POST);
    _request->setDelegate(this);
}

void mj::VerifyAppleAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    CCLOG("verify OK");
    if (_callback) {
        _callback(true);
    }
    
    MJDefault::getInstance()->setBoolForKey("receipt_need_verified", false);
    delete this;
}

void mj::VerifyAppleAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
