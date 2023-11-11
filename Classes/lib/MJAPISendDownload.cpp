//
// Created by To Ngoc Linh on 8/31/17.
//

#include "MJAPISendDownload.h"
#include "MJHelper.h"
#include "StoryDataModel.h"
#include "StoryFreeManager.h"
#include "HSLib.h"

void mj::SentDownloadAPI::call(int story_id, int type, std::string error, std::function<void(bool)> callback) {
    auto api = new SentDownloadAPI(story_id, type, error);
    api->_callback = callback;
    api->send();
}

mj::SentDownloadAPI::SentDownloadAPI(int story_id, int type, std::string error) {
    _request = new mj::Request(mj::Request::ActionType::SENT_DOWNLOAD, {
            {"story_lang_id", cocos2d::StringUtils::toString(story_id)},
            {"type", cocos2d::StringUtils::toString(type)},
            {"url_download", StoryDataModel::getInstance()->getLinkDownloadStory(story_id)},
            {"error_message", error},
            {"profile_id", cocos2d::StringUtils::toString(MJDEFAULT->getIntegerForKey(key_current_profile_id))}
    }, mj::Request::RequestType::POST);
    _request->setDelegate(this);
}

void mj::SentDownloadAPI::send() {
    if (_request) {
        _request->send();
    }
}

void mj::SentDownloadAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    auto d = data.GetObject();

    if (JSON_SAFE_CHECK(d, "type", String)) {
        auto type = ATOI(d["type"].GetString());
        if (type == 1) {
            StoryDataModel::getInstance()->addStorySentDownloadSuccess(XHSLib->_id, XHSLib->story_id, type);
        } else if (type == 2) {
            FREE_STORY_MANAGER->clearFreeTimeListStoryRead();
        }
    }

    if (_callback) {
        _callback(true);
    }

    delete this;
}

void mj::SentDownloadAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback(false);
    }

    delete this;
}
