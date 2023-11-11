//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//

#include "APSendOTPAPI.h"
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
		//1 = phone
		//2 = email
        void APSendOTPAPI::call(
				const std::string & type,
				const std::string & phone,
				const std::string & email,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
		{
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new APSendOTPAPI(type, phone, email, callback);
            refresh_call->send();
        }
    
    void APSendOTPAPI::call(
            const std::string & type,
            const std::string & phone,
            const std::string & email,
            const std::string & countryCode,
            const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
    {
        api::APIConfig config = api::APIConfig();
        ap::api::Base::setGlobalConfiguration(config);
        auto refresh_call = new APSendOTPAPI(type, phone, email,countryCode, callback);
        refresh_call->send();
    }
        
		APSendOTPAPI::APSendOTPAPI(
				const std::string & type,
				const std::string & phone,
				const std::string & email,
				const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
			m_type(type), m_callback(callback)
		{
			//1 = phone
			//2 = email
            _request = new Base(api::PlatformType::NEW, "account/send-opt-verify-pw", {
				{"type", type},
				{"phone", phone},
				{"email", email},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        APSendOTPAPI::APSendOTPAPI(
                const std::string & type,
                const std::string & phone,
                const std::string & email,
                const std::string & countryCode,
                const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
            m_type(type), m_callback(callback)
        {
            //1 = phone
            //2 = email
            _request = new Base(api::PlatformType::NEW, "account/send-opt-verify-pw", {
                {"type", type},
                {"phone", phone},
                {"email", email},
                {"country_code", countryCode},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
        
        void APSendOTPAPI::send() const
		{
            if (_request) 
			{
                _request->send();
            }
        }
        
        void APSendOTPAPI::onRequestSuccess(int /*tag*/, int /*code*/, const std::string & message, const rapidjson::Value & data)
		{
			int user_id = 0;

			if (m_callback) 
			{
                m_callback(true, message, data);
            }

            delete this;
        }

        void APSendOTPAPI::onRequestError(int /*tag*/, int /*error_code*/, const std::string & message, const rapidjson::Value & data)
		{
			StoreError::call(_request->getURL(), message, "account/send-opt-verify-pw", _request->getParam());
            
			if (m_callback)
			{
				m_callback(false, message, data);
            }

            delete this;
        }
    }
}
