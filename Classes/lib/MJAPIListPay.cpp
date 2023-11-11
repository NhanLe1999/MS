//
// Created by To Ngoc Linh on 8/31/17.
//

#include "MJAPIListPay.h"
#include "../external/json/writer.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

void mj::ListPayAPI::call(std::function<void(bool)> callback) {
    auto api = new ListPayAPI();
    api->_callback = callback;
    api->send();
}

mj::ListPayAPI::ListPayAPI() {
    _request = new mj::Request(mj::Request::ActionType::INAPP_INFO);
    _request->setDelegate(this);
}

void mj::ListPayAPI::send() {
    if (_request)
        _request->send();
}

void mj::ListPayAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    data.Accept(writer);
    auto json_string = std::string(buffer.GetString());
    MJDEFAULT->setStringForKey(keyPriceInfo, json_string);

    if (_callback) {
        _callback(true);
    }

    delete this;
}

void mj::ListPayAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
