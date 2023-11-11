//
//  MJAPIStoryDetails.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#ifndef MJAPIStoryDetails_hpp
#define MJAPIStoryDetails_hpp

#include "MJRequest.h"

namespace mj {
    class StoryDetailsAPI : public RequestDelegate {
    public:
        static void call(int story_id, std::function<void(bool)> callback = nullptr);
        
    private:
        void send();
        StoryDetailsAPI(int story_id);
        
        int _story_id;
        
        Request * _request = nullptr;
        std::function<void(bool)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPIStoryDetails_hpp */
