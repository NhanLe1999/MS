//
// Created by To Ngoc Linh on 8/31/17.
//

#include "MJAPIRateApp.h"

void mj::RateAppAPI::call(std::function<void(bool)> callback) {
    auto api = new RateAppAPI();
    api->_callback = callback;
    api->send();
}

mj::RateAppAPI::RateAppAPI() {
    _request = new mj::Request(mj::Request::ActionType::RATE_APP);
    _request->setDelegate(this);
}

void mj::RateAppAPI::send() {
    if (_request) {
        _request->send();
    }
}

void mj::RateAppAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    if (_callback) {
        _callback(true);
    }

    delete this;
}

void mj::RateAppAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
