//
//  MJAPITrackLesson.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/5/18.
//

#include "MJAPITrackLesson.h"
#include "MSLessonManager.h"

USING_NS_CC;

namespace mj {
    void TrackingAPI::call(mj::model::ActInfo info, std::function<void(bool)> callback) {
        auto refresh_call = new TrackingAPI(info);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    TrackingAPI::TrackingAPI(mj::model::ActInfo info) {
        auto lesson = ms::LessonManager::getInstance().getLessonById(info.lesson_id);
        _request = new mj::Request(mj::Request::ActionType::TRACK_LESSON, {
            {"activities_id", StringUtils::toString(info.activity_id)},
            {"lesson_id", StringUtils::toString(info.lesson_id)},
            {"categories_id", StringUtils::toString(lesson.category_id)}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void TrackingAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void TrackingAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        if (_callback) {
            _callback(true);
        }
        
        delete this;
    }
    
    void TrackingAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false);
        }
        delete this;
    }
}
