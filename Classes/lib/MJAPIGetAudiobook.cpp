//
//  MJAPIGetAudiobook.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/18/17.
//

#include "MJAPIGetAudiobook.h"
#include "../external/json/writer.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

USING_NS_CC;

namespace mj {
    void GetAudiobookAPI::call(int audiobook_id, std::function<void(bool, std::string saved_path, std::string audio_url)> callback) {
        auto refresh_call = new GetAudiobookAPI(audiobook_id);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    GetAudiobookAPI::GetAudiobookAPI(int audiobook_id) {
        _request = new mj::Request(mj::Request::ActionType::GET_AUDIOBOOK, {
            {"id", StringUtils::toString(audiobook_id)}
        }, mj::Request::RequestType::GET, 60);
        _request->setDelegate(this);
    }
    
    void GetAudiobookAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void GetAudiobookAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        auto data = d.GetObject();
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        
        auto saved_path = std::string("");
        auto audio_url = std::string("");
        
        if (JSON_SAFE_CHECK(data, "audio_file", String)) {
            audio_url = data["audio_file"].GetString();
            saved_path = mj::helper::getFilenameFromURL(audio_url);
            saved_path = mj::helper::replace_string(saved_path, FileUtils::getInstance()->getFileExtension(saved_path), ".json");
            saved_path = std::string("audiobooks/content/") + saved_path;
            
            FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "audiobooks");
            FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "audiobooks/content");
            FileUtils::getInstance()->writeStringToFile(buffer.GetString(), FileUtils::getInstance()->getWritablePath() + saved_path);
        }
        
        if (_callback) {
            _callback(true, saved_path, audio_url);
        }
        
        delete this;
    }
    
    void GetAudiobookAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_reason_error, message);
        GetAudiobookResponse r;
        r.content = "";
        r.audio_path = "";
        r.sync.clear();
        if (_callback) {
            _callback(false, "", "");
        }
        delete this;
    }
}
