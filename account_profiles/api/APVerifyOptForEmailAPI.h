//
//  Created by To Ngoc Linh on 6 / 20 / 19.
//  Modified by NguyenHoangThienPhuoc
//

#pragma once

#include "APBaseAPI.h"

namespace ap 
{
    namespace api 
	{
		class CommomVerifyOTP final : public RequestDelegate 
		{
		public:

			enum class VERIFY_TYPE
			{
				EMAIL,
				PHONE
			};
            
            // AUTHOR: Hieu Nguyen
            static void call(
                        const std::string & otp,
                        const std::string & email,
                        const std::function<void(bool is_success, std::string msg, int code)> & callback = nullptr);
            // -- end --
            
            static void call(
						const std::string &otp,
						const VERIFY_TYPE &type,
						const std::string &mailOrPhoneNumber,
                        const std::string &countryCode,
						const std::function<void(bool is_success, std::string msg, int code)> & callback = nullptr);
        
		private://private function

            void send() const;
            
            // AUTHOR: Hieu Nguyen
            CommomVerifyOTP(
                        const std::string &otp,
                        const std::string &email,
                        const std::function<void(bool is_success, std::string msg, int code)> & callback = nullptr);
            // -- end --
            
			CommomVerifyOTP(
						const std::string &otp,
						const VERIFY_TYPE &type,
						const std::string &mailOrPhoneNumber,
                        const std::string &countryCode,
						const std::function<void(bool is_success, std::string msg, int code)> & callback = nullptr);

            std::function<void(bool is_success, std::string msg, int code)> _callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;

		private: //private variables   

			std::string _otp;
			std::string _mailOrPhoneNumber;
			VERIFY_TYPE _type;
            Base		*_request = nullptr;
        };
    }
}
