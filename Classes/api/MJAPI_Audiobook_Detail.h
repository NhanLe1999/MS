#pragma once

#include "MJRequest.h"

namespace mj {
    namespace api {
        class MJAPI_Audiobook_Detail : public mj::RequestDelegate {
        public:
            static void call(std::string bookId, std::function<void(bool,std::string)> callback = nullptr);
            
        private:
            void send();
			MJAPI_Audiobook_Detail(std::string bookId);

            int onRequestRaw(int tag, std::string data) override;
            void onRequestSuccess(int tag, rapidjson::Value data) override;
            void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
		private:

			Request *								_request = nullptr;
			std::function<void(bool, std::string)>	_callback;

        };
    }
}

