//
// Created by To Ngoc Linh on 8/30/17.
//

#include "MJAPIGetLinkDownload.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MSServerManager.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"

mj::GetLinkDownloadAPI * mj::GetLinkDownloadAPI::call(int language, int story_id, bool hd, std::function<void(std::string)> callback) {
    auto api = new GetLinkDownloadAPI(language, story_id, hd);
    api->_callback = callback;
    api->send();
    
    return api;
}

mj::GetLinkDownloadAPI::GetLinkDownloadAPI(int language, int story_id, bool hd) {
    _unique_id = StoryDataModel::getInstance()->getStoryByStoryIdAndLang(story_id, LANGUAGE_MANAGER->getCurrentLangId()).unique_id;
    auto server_id = ms::ServerManager::getInstance().getCurrentServerId();
    if (server_id == -1) {
        _request = nullptr;
    } else {
        _request = new mj::Request(mj::Request::ActionType::GET_STORY, {
                {"sid", cocos2d::StringUtils::toString(story_id)},
                {"is_hdr", hd ? "1" : "0"},
            {"sv", cocos2d::StringUtils::toString(ms::ServerManager::getInstance().getCurrentServerId())}
        });
        _request->setDelegate(this);
    }
}

void mj::GetLinkDownloadAPI::send() {
    if (_request) {
        _request->send();
    } else {
        if (_callback) {
            _callback("");
        }
        delete this;
    }
}

void mj::GetLinkDownloadAPI::onRequestSuccess(int tag, rapidjson::Value data) {
    auto d = data.GetObject();
    auto url = std::string("");
    if (JSON_SAFE_CHECK(d, "story", String)) {
        url = d["story"].GetString();
    }

    std::tuple<int, std::string, std::string> t;
    if (StoryData::getInstance()->isHdr) {
        t = std::make_tuple(_unique_id, "", url);
    } else {
        t = std::make_tuple(_unique_id, url, "");
    }
    StoryDataModel::getInstance()->setLinkDownloadStory({t});
    
    if (_callback) {
        _callback(url);
    }
    delete this;
}

void mj::GetLinkDownloadAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    if (_callback) {
        _callback("");
    }
    
    delete this;
}

