//
//  MJAPITrackLesson.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/5/18.
//

#ifndef MJAPITrackLesson_hpp
#define MJAPITrackLesson_hpp

#include "MJRequest.h"
#include "MJStoryInfoModel.h"

namespace mj {
    class TrackingAPI : public RequestDelegate {
    public:
        static void call(mj::model::ActInfo info, std::function<void(bool)> callback = nullptr);
        
    private:
        void send();
        TrackingAPI(mj::model::ActInfo info);
        
        Request * _request = nullptr;
        std::function<void(bool)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPITrackLesson_hpp */
