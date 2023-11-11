//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//

#include "APPasswordAPI.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "APStoreErrorAPI.h"
#include "MJHelper.h"
#include "MJDebugView.h"
#include "StoryDataModel.h"
#include "APAuth.h"

namespace ap 
{
    namespace api 
	{
		void APPasswordAPI::changePasswordCall(
			const std::string & email,
			const std::string & phone,
			const std::string & newPassword,
			const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
		{
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
			auto refresh_call = new APPasswordAPI(email, phone, newPassword, callback);
			refresh_call->send();
		}
    
        void APPasswordAPI::changePasswordV2Call(
             const std::string & email,
             const std::string & phone,
             const std::string & newPassword,
             const std::string & tokenToChangePassword,
             const std::string & countryCode,
             const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
        {
            api::APIConfig config = api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new APPasswordAPI(email, phone, newPassword,tokenToChangePassword,countryCode, callback);
            refresh_call->send();
        }

		void APPasswordAPI::changePasswordV3Call(const std::string & password, 
			const std::function<void(bool is_success, std::string msg, const rapidjson::Value&data)>& callback)
		{
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
			auto refresh_call = new APPasswordAPI(password, callback);
			refresh_call->send();
		}

        void APPasswordAPI::forgotPasswordCall(
				const std::string & email,
				const std::string & phone,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
		{
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new APPasswordAPI(email, phone, callback);
            refresh_call->send();
        }
        
		//change passwrod constructor
		APPasswordAPI::APPasswordAPI(
				const std::string & email,
				const std::string & phone,
				const std::string & newPassword,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
			m_email(email),m_phone(phone), m_callback(callback)
		{
			m_isChangPasswordCalled = true;

            _request = new Base(api::PlatformType::NEW, "account/change-pw", 
			{
				{"email", email},
				{"phone", phone},
				{"password", newPassword},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
		//change passwrod v2 constructor

		APPasswordAPI::APPasswordAPI(
				const std::string & email,
				const std::string & phone,
				const std::string & newPassword,
				const std::string & tokenToChangePassword,
				const std::string & countryCode,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
			m_email(email),m_phone(phone), m_callback(callback)
		{
			m_isChangPasswordCalled = true;

			_request = new Base(api::PlatformType::NEWV2, "account/change-pw",
			{
				{"email", email},
				{"phone", phone},
				{"password", newPassword},
				{"token_to_change_pw", tokenToChangePassword},
				{"country_code",countryCode}
			}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}

		//change passwrod v2 constructor

		APPasswordAPI::APPasswordAPI(
			const std::string & password,
			const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
			m_callback(callback)
		{
			m_isChangPasswordCalled = true;

			_request = new Base(api::PlatformType::NEWV3, "authen/change-pw",
				{
					{"password", password},
				}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}
		//forgot passwrod constructor
		APPasswordAPI::APPasswordAPI(
			const std::string & email,
			const std::string & phone,
			const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
			m_email(email), m_phone(phone), m_callback(callback)
		{
			m_isChangPasswordCalled = false;

			_request = new Base(api::PlatformType::NEW, "authen/list-info-forgot-pw", 
			{
				{"email", email},
				{"phone", phone}
			}, api::RequestType::GET);
			_request->setDelegate(this);
			_request->enableLog();
		}
        
        void APPasswordAPI::send() const 
		{
            if (_request) {
                _request->send();
            }
        }
        
        void APPasswordAPI::onRequestSuccess(int /*tag*/, int /*code*/, const std::string & message, const rapidjson::Value & data) 
		{
			int user_id = 0;

			if (m_callback) 
			{
                m_callback(true, message, data);
            }

            delete this;
        }

        void APPasswordAPI::onRequestError(int /*tag*/, int /*error_code*/, const std::string & message, const rapidjson::Value & data) 
		{
			StoreError::call(_request->getURL(), message, m_isChangPasswordCalled?"account/change-pw" :"authen/list-info-forgot-pw", _request->getParam());
            
			if (m_callback)
			{
				m_callback(false, message, data);
            }

            delete this;
        }
    }
}
