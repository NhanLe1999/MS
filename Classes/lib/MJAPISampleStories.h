//
//  MJAPISampleStories.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/12/18.
//

#ifndef MJAPISampleStories_hpp
#define MJAPISampleStories_hpp

#include "MJRequest.h"

namespace mj {
    class SampleStoriesAPI : public RequestDelegate {
    public:
        static void call(std::function<void(bool, std::unordered_map<int, std::pair<int, int>>)> callback = nullptr);
        
    private:
        void send();
        SampleStoriesAPI();
        
        Request * _request = nullptr;
        std::function<void(bool, std::unordered_map<int, std::pair<int, int>>)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPISampleStories_hpp */
