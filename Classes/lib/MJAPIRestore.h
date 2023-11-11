//
// Created by To Ngoc Linh on 9/11/17.
//

#ifndef MJSTORY_MJAPIVERIFYANDROID_H
#define MJSTORY_MJAPIVERIFYANDROID_H

#include "MJRequest.h"

namespace mj {
    class RestoreAPI : public RequestDelegate {

    public:
        static void call(std::vector<std::pair<std::string, std::string> > info, std::function<void(bool,std::string)> callback = nullptr);
        static void callOnce(std::string product_id, std::string token, std::function<void(bool,std::string)> callback = nullptr);

    private:
        void send();
        RestoreAPI(std::string product_id, std::string token);

        Request * _request = nullptr;
        std::function<void(bool,std::string)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}

#endif //MJSTORY_MJAPIVERIFYANDROID_H
