//
//  APLoginAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//
//#pragma once

#include "APRegisterAPI.h"
#include "APStoreErrorAPI.h"

//#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "manager/GettingUserInfoManager.h"

USING_NS_CC;

namespace ap
{
	namespace api
	{
		void APRegisterAPI::call(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & countryCode,
			const std::string & password,
			const std::function<void(int error_code, bool is_success, std::string msg)> & callback)
		{
			auto refresh_call = new APRegisterAPI(login_type, user_name, phone, mail, token, countryCode, password, callback);
			refresh_call->send();
		}

		APRegisterAPI::APRegisterAPI(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & countryCode,
			const std::string & password,
			const std::function<void(int error_code, bool is_success, std::string msg)> & callback) :
			_name(user_name),
			_phone(phone),
			_mail(mail),
			_token(token),
			_countryCode(countryCode),
			_password(password),
			_callback(callback)
		{
			_request = new Base(api::PlatformType::NEWV3, "authen/register",
				{
				{"access_token", token},
				{"email", mail},
				{"phone", phone},
				{"user_name", user_name},
				{"type", std::to_string(login_type)},
				{"country_code",countryCode},
				{"password",password},
				{"is_upgrade",(login_type == LoginType::SKIP || !MJDEFAULT->getBoolForKey("is_skip_login", false)) ? "" : "true"},
				}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}

		void APRegisterAPI::send() const
		{
			if (_request)
			{
				_request->send();
			}
		}

		void APRegisterAPI::onRequestSuccess(int /*tag*/, int code, const std::string & message, const rapidjson::Value & data)
		{
			int user_id = 0;
			std::string token;
			bool isNewUser = false;

			if (data.HasMember("user_id") && data["user_id"].IsInt())
			{
				user_id = data["user_id"].GetInt();
			}

			if (data.HasMember("access_token") && data["access_token"].IsString())
			{
				token = data["access_token"].GetString();
			}

			if (data.HasMember("is_new_register") && data["is_new_register"].GetBool())
			{
				isNewUser = true;
			}

			Database::getInstance().saveIdentity(std::to_string(user_id), token);
			Database::getInstance().saveUserInfo(std::to_string(user_id), _name, _mail, _phone, "address","");

			if (_callback)
			{
				_callback(code, true, message);
			}

			delete this;
		}

		void APRegisterAPI::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & /*data*/)
		{
			StoreError::call(_request->getURL(), message, "authen/register", _request->getParam());

			if (_callback)
			{
				_callback(error_code, false, message);
			}

			delete this;
		}
	}
}
