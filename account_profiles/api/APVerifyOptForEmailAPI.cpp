//
//  Created by To Ngoc Linh on 6 / 20 / 19.
//  Modified by NguyenHoangThienPhuoc
//

#include "APVerifyOptForEmailAPI.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "APStoreErrorAPI.h"
#include "MJHelper.h"

namespace ap 
{
    namespace api 
	{
        // AUTHOR: Hieu Nguyen
        void CommomVerifyOTP::call(
                    const std::string & otp,
                    const std::string & email,
                    const std::function<void(bool is_success, std::string msg, int code)> & callback)
        {
            api::APIConfig config = api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new CommomVerifyOTP(otp, email, callback);
            refresh_call->send();
        }
        // -- end --
    
        void CommomVerifyOTP::call(
					const std::string & otp,
					const VERIFY_TYPE &type,
					const std::string & email,
                    const std::string &countryCode,
					const std::function<void(bool is_success, std::string msg, int code)> & callback) 
		{
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new CommomVerifyOTP(otp, type,email,countryCode, callback);
            refresh_call->send();
        }
    
        // AUTHOR: Hieu Nguyen
        CommomVerifyOTP::CommomVerifyOTP(
                    const std::string & otp,
                    const std::string & email,
                    const std::function<void(bool is_success, std::string msg, int code)> & callback) :
            _otp(otp), _mailOrPhoneNumber(email), _callback(callback)
        {
            // chú ý là dùng NEWV2 chứ không phải NEW đâu
            _request = new Base(api::PlatformType::NEWV2,"account/verify-opt-for-email",
            {
                {"code",    _otp},
                {"email",   _mailOrPhoneNumber}
            }, api::RequestType::POST);
            
            _request->setDelegate(this);
            _request->enableLog();
        }
        // -- end --
        
		CommomVerifyOTP::CommomVerifyOTP(
					const std::string & otp,
					const VERIFY_TYPE & type,
					const std::string & email,
                    const std::string & countryCode,
					const std::function<void(bool is_success, std::string msg, int code)> & callback) :
			_otp(otp), _type(type),_mailOrPhoneNumber(email),_callback(callback)
		{
            if(type == VERIFY_TYPE::PHONE)
            {
                _request = new Base(api::PlatformType::NEW, "account/verify-opt-for-phone",
                {
                    {"code", _otp},
                    {"country_code", countryCode},
                    {"phone",_mailOrPhoneNumber}
                }, api::RequestType::POST);
            }
            else
            {
                _request = new Base(api::PlatformType::NEW,"account/verify-opt-for-email",
                {
                    {"code", _otp},
                    {"email",_mailOrPhoneNumber}
                }, api::RequestType::POST);
            }

            _request->setDelegate(this);
            _request->enableLog();
        }
        
        void CommomVerifyOTP::send() const
		{
            if (_request) 
			{
                _request->send();
            }
        }
        
		void CommomVerifyOTP::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data)
		{
            std::string tokenToChangePassword= "";
            
//            if (data.HasMember("token_to_change_pw") && data["token_to_change_pw"].IsString())
            if (data.IsObject() && data.HasMember("token_to_change_pw") && data["token_to_change_pw"].IsString())
            {
                tokenToChangePassword = data["token_to_change_pw"].GetString();
            }
            
			if (_callback) 
			{
                _callback(true, tokenToChangePassword, code);
            }

            delete this;
        }

        void CommomVerifyOTP::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data)
		{
			StoreError::call(_request->getURL(), message, cocos2d::StringUtils::format("account/verify-opt-for-%s", _type == VERIFY_TYPE::EMAIL ? "email" : "phone"), _request->getParam());
            
			if (_callback) 
			{
                _callback(false, message, error_code);
            }

            delete this;
        }
    }
}
