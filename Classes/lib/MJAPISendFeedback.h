//
// Created by To Ngoc Linh on 8/31/17.
//

#ifndef MJSTORY_MJAPISendFeedback_H
#define MJSTORY_MJAPISendFeedback_H

#include "MJRequest.h"

namespace mj {
    class MJAPISendFeedback : public RequestDelegate {
    public:
        static void call(std::string message, std::function<void(bool)> callback = nullptr);

    private:
        void send();
        MJAPISendFeedback(std::string message);

        Request * _request = nullptr;
        std::function<void(bool)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}


#endif //MJSTORY_MJAPISENTDOWNLOAD_H
