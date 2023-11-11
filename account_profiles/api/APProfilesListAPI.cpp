#include "APProfilesListAPI.h"
#include "APStoreErrorAPI.h"

#include "APProfile.h"
#include "APDatabase.h"

#include "MJHelper.h"
//#include "DatabaseHelper.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace ap {
    namespace api {
        void ProfilesList::call(const std::function<void(bool, std::string)> & callback) {
            auto api_call = new ProfilesList(callback);
            api_call->send();
        }
    
        ProfilesList::ProfilesList(const std::function<void(bool, std::string)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/profile-list", {}, api::RequestType::GET);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        void ProfilesList::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        void ProfilesList::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "account/profile-list", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            
            delete this;
        }
    
        void ProfilesList::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {
            //TODO: cập nhập danh sách profile vào database
			if (!data.IsNull() && data.IsArray())
			{
				std::vector<ap::Profile> profiles;
				for (rapidjson::SizeType i = 0; i < data.Size(); ++i)
				{
					auto profile_data = data[i].GetObject();
					ap::Profile profile;
					profile.profile_id = profile_data["id"].GetInt();
					profile.name = profile_data["name"].GetString();
					profile.description = profile_data["description"].GetString();
					profile.age = profile_data["age"].GetInt();
                    profile.gender = (profile_data["gender"].GetInt() == 1)? ap::Profile::MALE : ap::Profile::FEMALE;

                    auto config_item = profile_data["config_item"].GetString();
                    //lưu tạm ver cũ của config item.
                    profile.description = config_item;
                    auto db_path = StringUtils::format("profile_%d", profile.profile_id);
                    CCLOG("db_path: %s", db_path.c_str());

                    // load profile config item
                    rapidjson::Document doc;
                    doc.Parse<rapidjson::kParseDefaultFlags>(config_item);
                    if (!doc.HasParseError())
                    {
                        if(CONFIG_MANAGER->isVietnameseStories()){
                            if(doc.HasMember("avatar_path_51") && doc["avatar_path_51"].IsString()){
                                profile.avatar = doc["avatar_path_51"].GetString();
                            }
                        }else{
                            if(doc.HasMember("avatar_path_40") && doc["avatar_path_40"].IsString()){
                                profile.avatar = doc["avatar_path_40"].GetString();
                            }
                        }
                    }
					profiles.push_back(profile);
//                    ap::Database::getInstance().updateProfile(profile);
				}
                ap::Database::getInstance().updateProfile(profiles);
				// add key_max_account_profile
                MJDEFAULT->setIntegerForKey(key_max_account_profile, std::max( MJDEFAULT->getIntegerForKey(key_max_account_profile,3), (int)profiles.size()));
			}
			if (_callback) {
                _callback(true, "");
            }
            
            delete this;
        }
        
        std::vector<std::pair<std::string, std::string>> ProfilesList::JsonStringToDict (std::string const& s){
            auto json_s = mj::helper::splitString(s, ',');
            std::vector<std::pair<std::string, std::string>> map;
            std::string key, value;
            for(auto str : json_s){
                auto key_value = mj::helper::splitString(str, ':');
                std::pair<std::string, std::string> pair;
                pair.first = key_value.front();
                pair.second = key_value.back();
                map.push_back(pair);
            }
            return map;
        }
    }
}
