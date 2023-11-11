//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//
#pragma once

#include "APBaseAPI.h"

namespace ap 
{
    namespace api 
	{
        class APSendOTPAPI final : public RequestDelegate
		{
        public:

            static void call(
							 const std::string & type,
							 const std::string & phone,
							 const std::string & email,
                             const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            static void call(
                             const std::string & type,
                             const std::string & phone,
                             const std::string & email,
                             const std::string & countryCode,
                             const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
        private:
            void send() const;
            
			APSendOTPAPI(
					const std::string & type,
					const std::string & phone,
					const std::string & email,
					const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
            APSendOTPAPI(
                    const std::string & type,
                    const std::string & phone,
                    const std::string & email,
                    const std::string & countryCode,
                    const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
			std::string m_type;

            std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> m_callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
        };
    }
}
