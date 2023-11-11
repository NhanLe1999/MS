//
// Created by To Ngoc Linh on 8/31/17.
//

#ifndef MJSTORY_MJAPILISTPAY_H
#define MJSTORY_MJAPILISTPAY_H

#include "MJRequest.h"

namespace mj {
    class ListPayAPI : public RequestDelegate {
    public:
        static void call(std::function<void(bool)> callback = nullptr);

    private:
        void send();
        ListPayAPI();

        Request * _request = nullptr;
        std::function<void(bool)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}


#endif //MJSTORY_MJAPILISTPAY_H
