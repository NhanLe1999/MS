#pragma once

#include "APBaseAPI.h"
#include "APProfile.h"
#include "APLoginAPI.h"

namespace ap 
{
	namespace api 
	{
		class APUpdatePhoneNumberAPI final : public RequestDelegate 
		{
		
		public:
            
			static void call(Login::LoginType i_type,
				const std::string& i_newAccessToken,
				const std::string& i_phone,
				const std::string& i_countryCode,
				const bool& i_isWeb,
				const std::string& i_oldAccessToken,
				const bool& i_isOldTokenUse,
				const bool& i_isAppUsed,
				const std::function<void(bool is_success, int code, std::string msg, std::string token, int userID)> & callback = nullptr);
			
		private:

			void send(const std::string& i_oldAccessToken = "") const;
            
			APUpdatePhoneNumberAPI(Login::LoginType i_type,
				const std::string& i_newAccessToken,
				const std::string& i_phone,
				const std::string& i_countryCode,
				const bool& i_isWeb,
				const std::string& i_oldAccessToken,
				const bool& i_isAppUsed,
				const std::function<void(bool is_success, int code, std::string msg, std::string token, int userID)> & callback = nullptr);

			std::function<void(bool is_success, int code, std::string msg, std::string token, int userID)> _callback;

			void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;

			void onRequestError(int tag, int errorCode, const std::string & message, const rapidjson::Value & data) override;
            
			Base * _request = nullptr;
            bool _is_retry = false;
            int _retry_count = 0;
		};
	}
}
