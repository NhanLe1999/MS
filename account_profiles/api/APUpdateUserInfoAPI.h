#pragma once
//
//  APUpdateUserInfoAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APUpdateUserInfoAPI_hpp
#define APUpdateUserInfoAPI_hpp

#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
	namespace api {
		class UpdateUserInfo final : public RequestDelegate {
		public:
            // AUTHOR: Hieu Nguyen
            static void call(const std::string& name, const std::string& phone, bool is_retry = true, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            // -- end --
            
			static void call(const UserInfo & user_info, const bool is_retry = true, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            static std::string buildConfigItem();
		private:
			void send() const;
            
            // AUTHOR: Hieu Nguyen
            UpdateUserInfo(const std::string& name, const std::string& phone, bool is_retry = true, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            // -- end --

			UpdateUserInfo(const UserInfo & user_info, const bool is_retry = true,
				const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

			UserInfo _info;
			std::function<void(bool is_success, std::string msg)> _callback;

			void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;

			void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;

            
			Base * _request = nullptr;
            bool _is_retry = false;
            int _retry_count = 0;
		};
	}
}
#endif /* APUpdateUserInfoAPI_hpp */
