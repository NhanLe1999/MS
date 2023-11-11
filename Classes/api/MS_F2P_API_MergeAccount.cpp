
#include "MS_F2P_API_MergeAccount.h"
#include "APStoreErrorAPI.h"
#include "F2PManager.h"

void ap::api::MergeAccountAPI::callMerge(std::string old_account_token, std::string new_account_token, const std::function<void(bool is_success, std::string msg, int errorCode)>& callback)
{
    auto m_api = new MergeAccountAPI(old_account_token, new_account_token, callback);
    m_api->_callType = CALL_TYPE::MERGE_PROFILE;
    m_api->send();
    
}

void ap::api::MergeAccountAPI::callDeleteProfile(int user_id, std::string listProfileToDelete, const std::function<void(bool is_success, std::string msg)>& callback)
{
	auto m_api = new MergeAccountAPI(user_id, listProfileToDelete, callback);
    m_api->_callType = CALL_TYPE::DELETE_PROFILE;
	m_api->send();

}

void ap::api::MergeAccountAPI::send() const
{
	if (_request)
	{
		_request->send();
	}
}

void ap::api::MergeAccountAPI::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
{
    if(this->_callType == CALL_TYPE::MERGE_PROFILE)
    {
        if (JSON_SAFE_CHECK(data, "maxProfile", Int))
        {
            int m_maxProfile = data["maxProfile"].GetInt();
            F2PManager::GetInstance()->setProfileMergeMaxProfile(m_maxProfile);
        }
        if (JSON_SAFE_CHECK(data, "user_id", Int))
        {
            int user_id = data["user_id"].GetInt();
            F2PManager::GetInstance()->setUserIdMerged(user_id);
        }
        
        if (_callbackMergeAccount)
        {
            _callbackMergeAccount(true, "", 200);
        }
    }
    else
    {
        if (_callbackDeleteProfile)
        {
            _callbackDeleteProfile(true, "");
        }
    }

	delete(this);

}

void ap::api::MergeAccountAPI::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
{
    if(this->_callType == CALL_TYPE::MERGE_PROFILE)
    {
        if(!data.IsNull() && data.IsObject())
        {
            if (JSON_SAFE_CHECK(data, "data", Object)) {
                auto dataError = data["data"].GetObject();
                if (JSON_SAFE_CHECK(dataError, "maxProfile", Int))
                {
                    int m_maxProfile = dataError["maxProfile"].GetInt();
                    F2PManager::GetInstance()->setProfileMergeMaxProfile(m_maxProfile);
                }
                if (JSON_SAFE_CHECK(dataError, "user_id", Int))
                {
                    int user_id = dataError["user_id"].GetInt();
                    F2PManager::GetInstance()->setUserIdMerged(user_id);
                }
            }
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/account/merge-account-skip", _request->getParam());
            if (_callbackMergeAccount)
            {
                _callbackMergeAccount(false, message, error_code);
            }
            
        }
    }
    else
    {
        ap::api::StoreError::call(_request->getURL(), message, "/api/v1/account/delete-profile", _request->getParam());
        
        if (_callbackDeleteProfile)
        {
            _callbackDeleteProfile(false, message);
        }
    }


	delete(this);
}

ap::api::MergeAccountAPI::MergeAccountAPI(std::string old_account_token, std::string new_account_token, const std::function<void(bool is_success, std::string msg, int errorCode)>& callback):
	_old_account_token(old_account_token), _new_account_token(new_account_token), _callbackMergeAccount(callback)
{

	_request = new Base(api::PlatformType::EDU_APP, "/api/v1/account/merge-account-skip", {
		{"old_account_token", old_account_token},
		{"new_account_token", new_account_token}
		}, api::RequestType::POST);
	_request->setDelegate(this);
	_request->enableLog();
}

ap::api::MergeAccountAPI::MergeAccountAPI(int user_id, std::string listProfileToDelete, const std::function<void(bool is_success, std::string msg)>& callback):
_listProfileToDelete(listProfileToDelete), _callbackDeleteProfile(callback)
{
	_request = new Base(api::PlatformType::EDU_APP, "/api/v1/account/delete-profile", {
		{"user_id", std::to_string(user_id)},
		{"profile_id", listProfileToDelete}
		}, api::RequestType::POST);
	_request->setDelegate(this);
	_request->enableLog();
}
