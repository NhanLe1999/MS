//
//  MJAPIGetAudioSyncData.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/28/18.
//

#ifndef MJAPIGetAudioSyncData_hpp
#define MJAPIGetAudioSyncData_hpp

#include "MJRequest.h"

namespace mj {
    class AudioSyncDataAPI : public RequestDelegate {
    public:
        static AudioSyncDataAPI * call(int audiobook_id, std::function<void(bool)> callback = nullptr);
        ~AudioSyncDataAPI();
        
    private:
        void send();
        AudioSyncDataAPI(int lesson_id);
        
        int _audiobook_id;
        
        Request * _request = nullptr;
        std::function<void(bool)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}

#endif /* MJAPIGetAudioSyncData_hpp */
