//
//  MJAPISampleStories.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/12/18.
//

#include "MJAPISampleStories.h"
#include "MJHelper.h"

USING_NS_CC;

namespace mj {
    void SampleStoriesAPI::call(std::function<void(bool, std::unordered_map<int, std::pair<int, int>>)> callback) {
        auto call = new SampleStoriesAPI();
        call->_callback = callback;
        call->send();
    }
    
    SampleStoriesAPI::SampleStoriesAPI() {
        _request = new mj::Request(mj::Request::ActionType::SAMPLE_STORIES);
        _request->setDelegate(this);
    }
    
    void SampleStoriesAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void SampleStoriesAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        std::unordered_map<int, std::pair<int, int>> r;
        if (d.IsArray()) {
            auto arr = d.GetArray();
            for (auto i = 0; i < arr.Size(); ++i) {
                if (arr[i].IsObject()) {
                    auto o = arr[i].GetObject();
                    
                    auto lang_id = JSON_SAFE_CHECK(o, "lang_id", Int) ? o["lang_id"].GetInt() : 0;
                    auto story_id = JSON_SAFE_CHECK(o, "sid", Int) ? o["sid"].GetInt() : 0;
                    auto cat_id = JSON_SAFE_CHECK(o, "game_categories_id", Int) ? o["game_categories_id"].GetInt() : 0;
                    
                    if (lang_id != 0 && story_id != 0) {
                        r[cat_id] = std::make_pair(lang_id, story_id);
                    }
                }
            }
        }
        if (_callback) {
            _callback(true, r);
        }
        
        delete this;
    }
    
    void SampleStoriesAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        std::unordered_map<int, std::pair<int, int>> r;
        if (_callback) {
            _callback(false, r);
        }
        delete this;
    }
}
