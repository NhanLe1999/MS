#pragma once

#include "APBaseAPI.h"

namespace ap {
	namespace api {
		class APRegisterAPI final : public RequestDelegate
		{
		public:

			enum LoginType 
			{
				FACEBOOK = 1,
				GOOGLE = 2,
				PHONE = 3,
				SKIP = 4,
				USER_NAME = 5
			};

			static void call(
				const LoginType login_type,
				const std::string & user_name,
				const std::string & phone,
				const std::string & mail,
				const std::string & token,
				const std::string & countryCode,
				const std::string & password,
				const std::function<void(int error_code, bool is_success, std::string msg)> & callback = nullptr);


		private:
			void send() const;

			APRegisterAPI(
				const LoginType login_type,
				const std::string & user_name,
				const std::string & phone,
				const std::string & mail,
				const std::string & token,
				const std::string & countryCode,
				const std::string & password,
				const std::function<void(int error_code, bool is_success, std::string msg)> & callback = nullptr);

			const std::string _name;
			const std::string _phone;
			const std::string _mail;
			const std::string _token;
			const std::string _countryCode;
			const std::string _password;

			std::function<void(int error_code, bool is_success, std::string msg)> _callback;

			void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;

			void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;

			Base * _request = nullptr;
		};
	}
}
