//
// Created by To Ngoc Linh on 8/31/17.
//

#include "MJAPISendFeedback.h"
#include "MJHelper.h"
#include "StoryDataModel.h"
#include "StoryFreeManager.h"
#include "HSLib.h"

void mj::MJAPISendFeedback::call(string message, std::function<void(bool)> callback) {
    auto api = new MJAPISendFeedback(message);
    api->_callback = callback;
    api->send();
}

mj::MJAPISendFeedback::MJAPISendFeedback(string message) {
    _request = new mj::Request(mj::Request::ActionType::SENT_FEEDBACK, {
            {"message", message},
    }, mj::Request::POST);
    _request->setDelegate(this);
}

void mj::MJAPISendFeedback::send() {
    if (_request) {
        _request->send();
    }
}

void mj::MJAPISendFeedback::onRequestSuccess(int tag, rapidjson::Value data) {
    if (_callback) {
        _callback(true);
    }

    delete this;
}

void mj::MJAPISendFeedback::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
