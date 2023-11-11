#include "APLinkUserCrmToAccount.h"
#include "APStoreErrorAPI.h"

namespace ap 
{
namespace api 
{
	void APLinkUserCrmToAccount::call(const std::string & i_newAccessToken, const std::string & i_oldAccessToken, const std::function<void(bool is_success, std::string msg)>& callback)
	{
		auto apiCall = new APLinkUserCrmToAccount(i_newAccessToken, i_oldAccessToken, callback);
		apiCall->send(i_oldAccessToken);
	}

	void APLinkUserCrmToAccount::send(std::string i_accessToken) const
	{
		if (_request) 
		{
			_request->send(i_accessToken);
		}
	}

	APLinkUserCrmToAccount::APLinkUserCrmToAccount(const std::string & i_newAccessToken, const std::string & i_oldAccessToken, const std::function<void(bool is_success, std::string msg)>& callback)
		:_callback(callback)
	{
		_request = new Base(api::PlatformType::NEW, "link-user-crm-to-account", {
			{"new_access_token", i_newAccessToken},
			{"old_access_token", i_oldAccessToken},
			}, api::RequestType::POST);
		_request->setDelegate(this);
		_request->enableLog();
	}

	void APLinkUserCrmToAccount::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) {
		
		if (_callback) 
		{
			_callback(true, message);
		}

		delete this;
	}

	void APLinkUserCrmToAccount::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) 
	{
		StoreError::call(_request->getURL(), message, "authen/logout", _request->getParam());

		if (_callback) 
		{
			_callback(false, message);
		}

		delete this;
	}
}
}
