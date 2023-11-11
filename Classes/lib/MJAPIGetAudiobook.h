//
//  MJAPIGetAudiobook.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/18/17.
//

#ifndef MJAPIGetAudiobook_hpp
#define MJAPIGetAudiobook_hpp

#include "MJRequest.h"
#include "MJStoryInfoModel.h"

namespace mj {
    struct GetAudiobookResponse {
        std::string content;
        std::vector<mj::model::SyncBlock> sync;
        std::string audio_path;
    };
    
    class GetAudiobookAPI : public RequestDelegate {
    public:
        static void call(int audiobook_id, std::function<void(bool, std::string saved_path, std::string audio_url)> callback = nullptr);
        
    private:
        void send();
        GetAudiobookAPI(int audiobook_id);
        
        Request * _request = nullptr;
        std::function<void(bool, std::string saved_path, std::string audio_url)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}

#endif /* MJAPIGetAudiobook_hpp */
