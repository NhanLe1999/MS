//
// Created by To Ngoc Linh on 8/30/17.
//

#ifndef MJSTORY_MJAPIGETLINKDOWNLOAD_H
#define MJSTORY_MJAPIGETLINKDOWNLOAD_H

#include "MJRequest.h"

namespace mj {
    class GetLinkDownloadAPI : public RequestDelegate {
    public:
        static GetLinkDownloadAPI * call(int language, int story_id, bool hd, std::function<void(std::string)> callback = nullptr);
//        static std::vector<int> _server_ids;
//        static int _server_index;
        inline void setCallback(std::function<void(std::string)> callback) { _callback = callback; }
    private:
        void send();
        GetLinkDownloadAPI(int language, int story_id, bool hd);
        
        int _unique_id;

        Request * _request = nullptr;
        std::function<void(std::string)> _callback;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
};


#endif //MJSTORY_MJAPIGETLINKDOWNLOAD_H
