//
//  MJAPIRefresh.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/4/17.
//

#ifndef MJAPIRefresh_hpp
#define MJAPIRefresh_hpp

#include "MJRequest.h"

namespace mj {
    class RefreshAPI : public RequestDelegate {
    public:
        static void call(std::function<void(bool)> callback = nullptr);

    private:
        void send();
        RefreshAPI();

        Request * _request = nullptr;
        std::function<void(bool)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        void updateDeviceInfoLicense(rapidjson::Value response_data);
    };
}

#endif /* MJAPIRefresh_hpp */
