//
// Created by To Ngoc Linh on 8/31/17.
//

#ifndef MJSTORY_MJAPISENTDOWNLOAD_H
#define MJSTORY_MJAPISENTDOWNLOAD_H

#include "MJRequest.h"

namespace mj {
    class SentDownloadAPI : public RequestDelegate {
    public:
        static void call(int story_id, int type, std::string error, std::function<void(bool)> callback = nullptr);

    private:
        void send();
        SentDownloadAPI(int story_id, int type, std::string error);

        Request * _request = nullptr;
        std::function<void(bool)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}


#endif //MJSTORY_MJAPISENTDOWNLOAD_H
