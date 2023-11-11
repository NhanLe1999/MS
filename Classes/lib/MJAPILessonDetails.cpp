//
//  MJAPILessonDetails.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#include "MJAPILessonDetails.h"

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "NativeBridge.h"
#include "MSServerManager.h"
#include "MJAPI_DecodeObjectJsonInfo.h"

USING_NS_CC;

namespace mj {
    void LessonDetailsAPI::call(int lesson_id, std::function<void(bool)> callback) {
        auto refresh_call = new LessonDetailsAPI(lesson_id);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    LessonDetailsAPI::LessonDetailsAPI(int lesson_id) {
        _lesson_id = lesson_id;
        _request = new mj::Request(mj::Request::ActionType::LESSON_DETAILS, {
            {"lesson_id", StringUtils::toString(lesson_id)},
            {"sv", StringUtils::toString(ms::ServerManager::getInstance().getCurrentServerId())}
        }, mj::Request::RequestType::GET, REQUEST_TIME_OUT);
        _request->setDelegate(this);
    }
    
    void LessonDetailsAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    
#define JSON_EXTRACT(obj, key, type, default) JSON_SAFE_CHECK(obj, key, type) ? obj[key].Get##type() : default
    void LessonDetailsAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        auto data = d.GetArray();
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryLessonInteractiveData(tag, data);

        if (_callback) {
            _callback(true);
        }

        delete this;
    }
    
    void LessonDetailsAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_reason_error, message);
        if (_callback) {
            _callback(false);
        }
        delete this;
    }
}
