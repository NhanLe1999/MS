/**
* API Merge account F2P
* @author DH
* @created 06/30/2022
*/

#ifndef MergeAccountAPI_hpp
#define MergeAccountAPI_hpp

#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
    
        class MergeAccountAPI final : public RequestDelegate {
            enum CALL_TYPE{
                MERGE_PROFILE,
                DELETE_PROFILE,
            };
        public:
            /**
            * Function call merge API
            * @param old_account_token
            * @param new_account_token
            * @param callback
            */
            static void callMerge(std::string old_account_token, std::string new_account_token, const std::function<void(bool is_success, std::string msg, int errorCode)>& callback = nullptr);
            /**
            * Function call delete profile API
            * @param listProfileToDelete
            * @param callback
            */
            static void callDeleteProfile(int user_id, std::string listProfileToDelete, const std::function<void(bool is_success, std::string msg)>& callback = nullptr);

        
        private:
            Profile _profile;
            Base* _request = nullptr;
            std::string _old_account_token;
            std::string _new_account_token;
            std::string _listProfileToDelete;
            CALL_TYPE _callType;
            std::function<void(bool is_success, std::string msg, int errorCode)> _callbackMergeAccount;
            std::function<void(bool is_success, std::string msg)> _callbackDeleteProfile;

        private:
            void send() const;

            MergeAccountAPI(std::string old_account_token, std::string new_account_token, const std::function<void(bool is_success, std::string msg, int errorCode)>& callback = nullptr);

            MergeAccountAPI(int user_id, std::string listProfileToDelete, const std::function<void(bool is_success, std::string msg)>& callback = nullptr);

            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
            
        };
    }
}
#endif /* MergeAccountAPI_hpp */
