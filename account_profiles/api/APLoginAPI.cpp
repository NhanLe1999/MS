//
//  APLoginAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APLoginAPI.h"
#include "APStoreErrorAPI.h"

#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "manager/GettingUserInfoManager.h"
#include "HelperManager.h"

namespace ap {
    namespace api {
        void Login::call(
                         const LoginType login_type,
                         const std::string & user_name,
                         const std::string & phone,
                         const std::string & mail,
                         const std::string & token,
						 const std::string & firebase_id,
						 const std::string & is_upgrade,
                         const std::function<void(int error_code,bool is_success, std::string msg)> & callback) {
            auto refresh_call = new Login(login_type, user_name, phone, mail, token, firebase_id,is_upgrade, callback);
            refresh_call->send();
        }

		void Login::call(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & firebase_id,
			const std::string & is_upgrade,
			const std::string & password,
			const std::function<void(int error_code, bool is_success, std::string msg)> & callback) {
			auto refresh_call = new Login(login_type, user_name, phone, mail, token, firebase_id, is_upgrade, password, callback);
			refresh_call->send();
		}

		void Login::callV3(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & password,
			const std::string & country_code,
			const std::string & user_name_id,
			const std::string & new_access_token,
			const std::function<void(int error_code, bool is_success, std::string msg,std::string tokenToChangePassword, LoginDataInfo info)> & callback)
		{
			auto refresh_call = new Login(login_type, user_name, phone, mail, token, password, country_code, user_name_id, new_access_token,callback);
			refresh_call->send();
		}

        
        Login::Login(
                      const LoginType login_type,
                      const std::string & user_name,
                      const std::string & phone,
                      const std::string & mail,
                      const std::string & token,
					  const std::string & firebase_id,
					  const std::string & is_upgrade,
                      const std::function<void(int error_code,bool is_success, std::string msg)> & callback) :
			_name(user_name),_phone(phone),_mail(mail),_is_upgrade(is_upgrade),_callback(callback) {
            _request = new Base(api::PlatformType::NEWV2, "authen/register", {
                {"type", std::to_string(login_type)},
                {"name", user_name},
                {"email", mail},
                {"phone", phone},
				{"fid", firebase_id},
				{"is_upgrade",  (login_type == LoginType::SKIP || !MJDEFAULT->getBoolForKey("is_skip_login", false)) ? "" : "true"},
                {"access_token", token}
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
        
		Login::Login(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & firebase_id,
			const std::string & is_upgrade,
			const std::string & password,
			const std::function<void(int error_code, bool is_success, std::string msg)> & callback) :
			_name(user_name), _phone(phone), _mail(mail), _is_upgrade(is_upgrade), _callback(callback), _password(password){
			_request = new Base(api::PlatformType::NEWV2, "authen/register", {
				{"type", std::to_string(login_type)},
				{"name", user_name},
				{"email", mail},
				{"phone", phone},
				{"fid", firebase_id},
				{"is_upgrade",  (login_type == LoginType::SKIP || !MJDEFAULT->getBoolForKey("is_skip_login", false)) ? "" : "true"},
				{"access_token", token},
				{"password", password}
				}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}


		//constructor for logic v3,v4

		Login::Login(
			const LoginType login_type,
			const std::string & user_name,
			const std::string & phone,
			const std::string & mail,
			const std::string & token,
			const std::string & password,
			const std::string & country_code,
			const std::string & user_name_id,
			const std::string & new_access_token,
			const std::function<void(int error_code, bool is_success, std::string msg,std::string tokenToChangePassword, LoginDataInfo info)> & callback)
			:
			_loginType(login_type),
			_name(user_name), 
			_phone(phone), 
			_mail(mail),
			_callbackV3(callback),
			_password(password),
			_userNameID(user_name_id)
		{
			_request = new Base(api::PlatformType::NEWV4, "authen/login",
				{
				{"type", std::to_string(login_type)},
				{"user_name", user_name},
				{"email", mail},
				{"phone", phone},
				{"access_token", token},
				{"password", password},
				{"country_code", country_code},
				{"user_name_crm", user_name_id},
				{"new_access_token", new_access_token},
				}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}

        void Login::send() const 
		{
            if (_request) 
			{
                _request->send();
            }
        }
        
        void Login::onRequestSuccess(int /*tag*/, int code, const std::string & /*message*/, const rapidjson::Value & data) 
		{
            int user_id = 0;
            std::string token;
			bool isNewUser = false;
            std::string tokenToChangePassword;

			LoginDataInfo loginDataInfo;

			loginDataInfo.currentUserInfo._id			= -1;
			loginDataInfo.currentUserInfo._name			= _name;
			loginDataInfo.currentUserInfo._phone		= _phone;
			loginDataInfo.currentUserInfo._mail			= _mail;
			loginDataInfo.currentUserInfo._address		= _address;
			loginDataInfo.currentUserInfo._userNameID	= _userNameID;
			
			if (JSON_SAFE_CHECK(data, "new_access_token", String))
			{
				loginDataInfo.newAccessToken = data["new_access_token"].GetString();
			}

			if (JSON_SAFE_CHECK(data, "phone_purchase", String))
			{
				loginDataInfo.phonePurchase = data["phone_purchase"].GetString();
			}

			if (JSON_SAFE_CHECK(data, "old_info", Object))
			{
				auto oldInfo = data["old_info"].GetObject();

				if (JSON_SAFE_CHECK(oldInfo, "list_indentity", Array))
				{
					auto list_indentity = oldInfo["list_indentity"].GetArray();

					for (size_t i = 0; i < list_indentity.Size(); i++)
					{
						auto identity = list_indentity[i].GetObject();

						if (JSON_SAFE_CHECK(identity, "type", Int))
						{
							if (JSON_SAFE_CHECK(identity, "user_id", Int))
							{
								loginDataInfo.oldUserInfo._id = identity["user_id"].GetInt();
							}

							auto type = (LoginType)identity["type"].GetInt();

							switch (type)
							{
							case ap::api::Login::FACEBOOK:
                                {
                                    if (JSON_SAFE_CHECK(identity, "name", String))
                                        loginDataInfo.oldUserInfo._name = loginDataInfo.oldUserInfo._name.empty() ? identity["name"].GetString() : loginDataInfo.oldUserInfo._name;
                                    
                                    if (JSON_SAFE_CHECK(identity, "email", String))
                                        loginDataInfo.oldUserInfo._mail = loginDataInfo.oldUserInfo._mail.empty() ? identity["email"].GetString() : loginDataInfo.oldUserInfo._mail;
                                    
                                    if (JSON_SAFE_CHECK(identity, "phone", String))
                                        loginDataInfo.oldUserInfo._phone = loginDataInfo.oldUserInfo._phone.empty() ? identity["phone"].GetString() : loginDataInfo.oldUserInfo._phone;
                                }
							case ap::api::Login::GOOGLE:
								if (JSON_SAFE_CHECK(identity, "email", String))
									loginDataInfo.oldUserInfo._mail = loginDataInfo.oldUserInfo._mail.empty() ? identity["email"].GetString() : loginDataInfo.oldUserInfo._mail;
								break;
							case ap::api::Login::PHONE:
								if (JSON_SAFE_CHECK(identity, "phone", String))
									loginDataInfo.oldUserInfo._phone = loginDataInfo.oldUserInfo._phone.empty() ? identity["phone"].GetString() : loginDataInfo.oldUserInfo._phone;
								break;
							case ap::api::Login::SKIP:
								break;
							case ap::api::Login::USER_NAME:
								break;
							default:
								break;
							}

							loginDataInfo.oldUserInfo._type.push_back(type);
						}
					}
				}

				if (JSON_SAFE_CHECK(oldInfo, "list_profile", Array))
				{
					auto list_profile = oldInfo["list_profile"].GetArray();
					
					for (size_t i = 0; i < list_profile.Size(); i++)
					{
						auto profieObject = list_profile[i].GetObject();
						
						ap::Profile profile;

						if (JSON_SAFE_CHECK(profieObject, "id", Int))
						{
							profile.profile_id = profieObject["id"].GetInt();
						}

						if (JSON_SAFE_CHECK(profieObject, "name", String))
						{
							profile.name = profieObject["name"].GetString();
						}

						if (JSON_SAFE_CHECK(profieObject, "config_item", String))
						{
							profile.avatar = HelperManager::GetInstance()->getProfileAvatarPathFromConfigJson(profieObject["config_item"].GetString());
						}

						if (JSON_SAFE_CHECK(profieObject, "age", Int))
						{
							profile.age = profieObject["age"].GetInt();
						}

						if (JSON_SAFE_CHECK(profieObject, "gender", Int))
						{
							profile.gender = (ap::Profile::Gender) profieObject["gender"].GetInt();
						}

						loginDataInfo.profiles.push_back(profile);
					}
				}

				if (JSON_SAFE_CHECK(oldInfo, "old_access_token", String))
				{
					loginDataInfo.oldAccessToken =  oldInfo["old_access_token"].GetString();
				}
			}

            if (data.HasMember("user_id") && data["user_id"].IsInt()) 
			{
                user_id = data["user_id"].GetInt();

				loginDataInfo.currentUserInfo._id = user_id;
            }
            
            if (data.HasMember("access_token") && data["access_token"].IsString()) 
			{
                token = data["access_token"].GetString();
            }

			if (JSON_SAFE_CHECK(data,"is_change_pw_default",Bool))
			{
				loginDataInfo.isPasswordChanged  = data["is_change_pw_default"].GetBool();
			}

            if (data.HasMember("token_to_change_pw") && data["token_to_change_pw"].IsString())
            {
                tokenToChangePassword = data["token_to_change_pw"].GetString();
            }
            
            if (data.HasMember("token_to_change_pw") && data["token_to_change_pw"].IsInt())
            {
                tokenToChangePassword = std::to_string(data["token_to_change_pw"].GetInt());
            }
		
			if (loginDataInfo.newAccessToken.empty())
			{
				loginDataInfo.type = JSON_SAFE_CHECK(data, "old_info", Object) ? V4Type::DEFAULT_WITH_OLD_USER_INFO : V4Type::DEFAULT;
			}
			else 
			{
				loginDataInfo.type = JSON_SAFE_CHECK(data, "old_info", Object) ? V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT : V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT;
			}

			if (loginDataInfo.type == V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT ||
				loginDataInfo.type == V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT)
			{
				loginDataInfo.currentUserInfo._type.push_back(LoginType::USER_NAME);
			}
			switch (loginDataInfo.type)
			{
			case ap::api::Login::V4Type::DEFAULT:
				Database::getInstance().saveIdentity(std::to_string(user_id), token);
				Database::getInstance().saveUserInfo(std::to_string(user_id), _name, _mail, _phone, _address, _userNameID);
				break;
			case ap::api::Login::V4Type::DEFAULT_WITH_OLD_USER_INFO:
				Database::getInstance().saveIdentity(std::to_string(user_id), token);
				Database::getInstance().saveUserInfo(std::to_string(user_id), _name, _mail, _phone, _address, _userNameID);
				break;
			case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT:
				break;
			case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT:
				break;
			default:
				break;
			}

            if (_callback) 
			{
                _callback(code, true, "");
            }
            else if (_callbackV3)
            {
                _callbackV3(code, true, "",tokenToChangePassword, loginDataInfo);
            }	
            
            delete this;
        }
        
        void Login::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & /*data*/) 
		{
			StoreError::call(_request->getURL(), message, "authen/login", _request->getParam());
            
			if (_callback) 
			{
                _callback(error_code,false, message);
            }
            else if (_callbackV3)
            {
                _callbackV3(error_code, false, message,"",LoginDataInfo());
            }
            
            delete this;
        }
    }
}
