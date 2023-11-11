//
// Created by To Ngoc Linh on 9/16/17.
//

#ifndef MJSTORY_MJAPIVERIFYAPPLE_H
#define MJSTORY_MJAPIVERIFYAPPLE_H

#include "MJRequest.h"

namespace mj {
    class VerifyAppleAPI : public RequestDelegate {
    public:
        static void call(std::function<void(bool)> callback = nullptr);

    private:
        void send();
        VerifyAppleAPI(std::function<void(bool)> callback = nullptr);

        Request * _request = nullptr;
        std::function<void(bool)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}

#endif //MJSTORY_MJAPIVERIFYAPPLE_H
