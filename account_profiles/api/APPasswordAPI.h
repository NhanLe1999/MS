//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//
#pragma once

#include "APBaseAPI.h"

namespace ap 
{
    namespace api 
	{
        class APPasswordAPI final : public RequestDelegate 
		{
        public:

			static void changePasswordCall(
				const std::string & email,
				const std::string & phone,
				const std::string & newPassword,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);

            static void changePasswordV2Call(
                const std::string & email,
                const std::string & phone,
                const std::string & newPassword,
                const std::string & tokenToChangePassword,
                const std::string & countryCode,
                const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
			static void changePasswordV3Call(
				const std::string & password,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);

            static void forgotPasswordCall(
							 const std::string & email,
							 const std::string & phone,
                             const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
        private:
            void send() const;
            
			//forgot password constructor
			APPasswordAPI(
					const std::string & email,
					const std::string & phone,
					const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);

			APPasswordAPI(
				const std::string & email,
				const std::string & phone,
				const std::string & newPassword,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
            //v2
            APPasswordAPI(
                const std::string & email,
                const std::string & phone,
                const std::string & newPassword,
                const std::string & tokenToChangePassword,
                const std::string & countryCode,
                const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);

			//v3
			APPasswordAPI(
				const std::string & password,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
			std::string m_email;
			
			std::string m_phone;

			bool m_isChangPasswordCalled = false;

            std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> m_callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
        };
    }
}
