#include "APUpdatePhoneNumberAPI.h"
#include "APStoreErrorAPI.h"
#include "MJHelper.h"

namespace ap 
{
namespace api 
{
	void APUpdatePhoneNumberAPI::call(Login::LoginType i_type,
		const std::string & i_newAccessToken,
		const std::string & i_phone,
		const std::string &i_countryCode, 
		const bool& i_isWeb, 
		const std::string& i_oldAccessToken,
		const bool& i_isOldTokenUse,
		const bool& i_isAppUsed,
		const std::function<void(bool is_success, int code, std::string msg, std::string token, int userID)>& callback)
	{
		auto apiCall = new APUpdatePhoneNumberAPI(i_type,i_newAccessToken, i_phone, i_countryCode , i_isWeb, i_oldAccessToken, i_isAppUsed, callback);
		apiCall->send(i_isOldTokenUse ? i_oldAccessToken: i_newAccessToken);
	}

	void APUpdatePhoneNumberAPI::send(const std::string & i_oldAccessToken) const
	{
		if (_request) 
		{
			_request->send(i_oldAccessToken);
		}
	}

	APUpdatePhoneNumberAPI::APUpdatePhoneNumberAPI(Login::LoginType i_type,
		const std::string & i_newAccessToken,
		const std::string & i_phone,
		const std::string& i_countryCode,
		const bool& i_isWeb,
		const std::string& i_oldAccessToken,
		const bool& i_isAppUsed,
		const std::function<void(bool is_success, int code, std::string msg, std::string token, int userID)>& callback)
		:_callback(callback)
	{
		_request = new Base(api::PlatformType::NEW, "update-phone-user-crm", {
			{"new_access_token", i_newAccessToken},
			{"phone", i_phone},
			{"country_code", i_countryCode},
			{"is_web",  std::to_string(i_isWeb)},
			{"type", std::to_string(i_type)},
			{"is_app_used",  std::to_string(i_isAppUsed)},
			{"old_access_token", i_oldAccessToken},
			}, api::RequestType::POST);
		_request->setDelegate(this);
		_request->enableLog();
	}

	void APUpdatePhoneNumberAPI::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data)
	{
		std::string token = "";

		int userID = 0;

		if (JSON_SAFE_CHECK(data, "token", String))
		{
			token = data["token"].GetString();
		}

		if (JSON_SAFE_CHECK(data, "user_id", Int))
		{
			userID = data["user_id"].GetInt();
		}
		
		if (_callback) 
		{
			_callback(true, code, message, token, userID);
		}

		delete this;
	}

	void APUpdatePhoneNumberAPI::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) 
	{
		StoreError::call(_request->getURL(), message, "update-phone-user-crm", _request->getParam());

		if (_callback) 
		{
			_callback(false, error_code, message,"",0);
		}

		delete this;
	}
}
}
