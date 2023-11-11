//
//  MJAPILessonDetails.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#ifndef MJAPILessonDetails_hpp
#define MJAPILessonDetails_hpp

#include "MJRequest.h"

namespace mj {
    class LessonDetailsAPI : public RequestDelegate {
    public:
        static void call(int lesson_id, std::function<void(bool)> callback = nullptr);
        
    private:
        void send();
        LessonDetailsAPI(int lesson_id);
        
        int _lesson_id;
        
        Request * _request = nullptr;
        std::function<void(bool)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPILessonDetails_hpp */
