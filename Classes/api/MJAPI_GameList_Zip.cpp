//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_GameList_Zip.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "MJStoryInfoModel.h"
#include "StoryFreeManager.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>
#include "MJAPI_DecodeObjectJsonInfo.h"
#include"APStoreErrorAPI.h"

namespace mj {
    namespace api {
        using namespace ap;
        void GameListZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new GameListZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        GameListZip::GameListZip() {
            int api_version_all_games_local = MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, 0);
            _request = new ap::api::Base(ap::api::PlatformType::EDU_APP, "/api/v1/game/list-game",
                { 
                    {"lang_id", "1"}, 
                    {"version_game_number", std::to_string(api_version_all_games_local)},
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool GameListZip::onReceivedRawData(int tag, const std::string& data)
        {
            auto unzip_json = mj::helper::unzip_json(data);
            if (!unzip_json) {
                return false;
            }

            CCLOG("GameListZip::onReceivedRawData, response: %s", unzip_json);

            rapidjson::Document doc;
            doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);

            if (doc.HasParseError()) {
                doc.GetParseError();
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            if (!doc.IsObject()) {
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            auto map_response = doc.GetObject();
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(tag, map_response);
            MJDEFAULT->setIntegerForKey(key_api_version_all_games_local, MJDEFAULT->getIntegerForKey(key_api_version_all_games_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, -1)));

            if (_callback) {
                _callback(true);
            }
            return true;
        }

        void GameListZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            delete this;
        }

        void GameListZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/game/list-game", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void GameListZip::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
