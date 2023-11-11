//
//  MJAPIStoryDetails.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#include "MJAPIStoryDetails.h"

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "NativeBridge.h"
#include "MSServerManager.h"
#include "ParseJSONHelper.hpp"
#include "MJAPI_DecodeObjectJsonInfo.h"

USING_NS_CC;

namespace mj {
    void StoryDetailsAPI::call(int story_id, std::function<void(bool)> callback) {
        auto refresh_call = new StoryDetailsAPI(story_id);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    StoryDetailsAPI::StoryDetailsAPI(int story_id) {
        _story_id = story_id;
        _request = new mj::Request(mj::Request::ActionType::STORY_DETAIL, {
            {"stories_id", StringUtils::toString(story_id)},
            {"sv", StringUtils::toString(ms::ServerManager::getInstance().getCurrentServerId())}
        }, mj::Request::RequestType::GET, REQUEST_TIME_OUT);
        _request->setDelegate(this);
    }
    
    void StoryDetailsAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    
#define JSON_EXTRACT(obj, key, type, default) JSON_SAFE_CHECK(obj, key, type) ? obj[key].Get##type() : default
    void StoryDetailsAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        auto data = d.GetArray();
        
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        rapidjson::StringBuffer rj_buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
        d.Accept(writer);
        CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryLessonInteractiveData(tag, data);

        if (_callback) {
            _callback(true);
        }

        delete this;
    }
    
    void StoryDetailsAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_reason_error, message);
        if (_callback) {
            _callback(false);
        }
        delete this;
    }
}
