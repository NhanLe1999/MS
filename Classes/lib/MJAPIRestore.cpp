//
// Created by To Ngoc Linh on 9/11/17.
//

#include "MJAPIRestore.h"
#include "MJPlatformConfig.h"

void mj::RestoreAPI::call(std::vector<std::pair<std::string, std::string> > info, std::function<void(bool,std::string)> callback) {
    if (info.empty()) {
        if (callback) {
            callback(true,"");
        }

        return;
    }
    auto product_id = info[0].first;
    auto token = info[0].second;
    info.erase(info.begin());
    mj::RestoreAPI::callOnce(product_id, token, [info, callback](bool is_success,std::string message) {
        mj::RestoreAPI::call(info, callback);
        return;
    });
}

void mj::RestoreAPI::callOnce(std::string product_id, std::string token, std::function<void(bool,std::string)> callback) {
    auto api = new RestoreAPI(product_id, token);
    api->_callback = callback;
    api->send();
}

mj::RestoreAPI::RestoreAPI(std::string product_id, std::string token) {
    _request = new mj::Request(mj::Request::ActionType::VERIFY_RECEIPT, {
            {"receipt", ""},
            {"package_id", product_id},
            {"purchases_token", token},
            {"package_name", MJ_PLATFORM_CONFIG.getPackageName()}
    }, mj::Request::POST);
    _request->setDelegate(this);
}

void mj::RestoreAPI::send() {
    if (_request) {
        _request->send();
    }
}

void mj::RestoreAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    if (_callback) {
        _callback(true,"");
    }

    delete this;
}

void mj::RestoreAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false,message);
    }

    delete this;
}
