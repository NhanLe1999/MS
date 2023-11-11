//
//  APLoginAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APLoginAPI_hpp
#define APLoginAPI_hpp

#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
        class Login final : public RequestDelegate {
        
		public:
		
			enum LoginType
			{
				FACEBOOK = 1,
				GOOGLE = 2,
				PHONE = 3,
				SKIP = 4,
				USER_NAME = 5
			};

			enum class V4Type
			{
				DEFAULT,
				DEFAULT_WITH_OLD_USER_INFO,
				LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT,
				LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT,
			};

			struct NewUserInfo : public ap::UserInfo
			{
				std::vector<LoginType> _type;
				std::string _userNameID;
			};

			struct LoginDataInfo
			{
				V4Type type;
				NewUserInfo oldUserInfo;
				NewUserInfo currentUserInfo;
				std::vector<ap::Profile> profiles;
				std::string newAccessToken = "";
				std::string oldAccessToken = "";
				std::string phonePurchase = "";
				bool		isPasswordChanged = false;

				LoginDataInfo() {};

				LoginDataInfo(const LoginDataInfo &i_info) {
					
					type = i_info.type;

					oldUserInfo = i_info.oldUserInfo;

					currentUserInfo = i_info.currentUserInfo;

					for (auto p : i_info.profiles)
					{
						profiles.push_back(p);
					}

					newAccessToken	= i_info.newAccessToken;

					oldAccessToken	= i_info.oldAccessToken;

					phonePurchase	= i_info.phonePurchase;

					isPasswordChanged = i_info.isPasswordChanged;
				};
			};
            
            static void call(
                             const LoginType login_type,
                             const std::string & user_name,
                             const std::string & phone,
                             const std::string & mail,
                             const std::string & token,
							 const std::string & firebase_id,
							 const std::string & is_upgrade,
                             const std::function<void(int error_code,bool is_success, std::string msg)> & callback = nullptr);

			static void call(
							const LoginType login_type,
							const std::string & user_name,
							const std::string & phone,
							const std::string & mail,
							const std::string & token,
							const std::string & firebase_id,
							const std::string & is_upgrade,
							const std::string & password,
							const std::function<void(int error_code, bool is_success, std::string msg)> & callback = nullptr);

			static void callV3(
				const LoginType login_type,
				const std::string & user_name,
				const std::string & phone,
				const std::string & mail,
				const std::string & token,
				const std::string & password,
				const std::string & country_code,
				const std::string & user_name_id,
				const std::string & new_access_token,
				const std::function<void(int error_code, bool is_success, std::string msg,std::string tokenToChangePassword, LoginDataInfo info)> & callback = nullptr);

        private:
            void send() const;
            
            Login(
                  const LoginType login_type,
                  const std::string & user_name,
                  const std::string & phone,
                  const std::string & mail,
                  const std::string & token,
				  const std::string & firebase_id,
				  const std::string & is_upgrade,
                  const std::function<void(int error_code,bool is_success, std::string msg)> & callback = nullptr);

			Login(
				const LoginType login_type,
				const std::string & user_name,
				const std::string & phone,
				const std::string & mail,
				const std::string & token,
				const std::string & firebase_id,
				const std::string & is_upgrade,
				const std::string & password,
				const std::function<void(int error_code, bool is_success, std::string msg)> & callback = nullptr);
            
			//api V3
			Login(
				const LoginType login_type,
				const std::string & user_name,
				const std::string & phone,
				const std::string & mail,
				const std::string & token,
				const std::string & password,
				const std::string & country_code,
				const std::string & user_name_id,
				const std::string & new_access_token,
				const std::function<void(int error_code, bool is_success, std::string msg,std::string tokenToChangePassword, LoginDataInfo info)> & callback = nullptr);

			const std::string _name = "";
			const std::string _phone = ""; 
			const std::string _mail = "" ;
			const std::string _address = "";
			const std::string _is_upgrade = "";
			const std::string _password = "";
			const std::string _userNameID = "";
			
			LoginType _loginType;

            std::function<void(int error_code,bool is_success, std::string msg)> _callback;
            std::function<void(int error_code,bool is_success, std::string msg,std::string tokenToChangePassword, LoginDataInfo info)> _callbackV3;

            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
        };
    }
}
#endif /* APLoginAPI_hpp */
