#pragma once

#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap 
{
	namespace api 
	{
		class APLinkUserCrmToAccount final : public RequestDelegate 
		{
		
		public:
            
			static void call(const std::string& i_newAccessToken, const std::string& i_oldAccessToken, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
			
		private:

			void send(std::string i_accessToken) const;
            
			APLinkUserCrmToAccount(const std::string& i_newAccessToken, const std::string& i_oldAccessToken, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

			std::function<void(bool is_success, std::string msg)> _callback;

			void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;

			void onRequestError(int tag, int errorCode, const std::string & message, const rapidjson::Value & data) override;
            
			Base * _request = nullptr;
            bool _is_retry = false;
            int _retry_count = 0;
		};
	}
}
