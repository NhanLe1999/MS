#include "APUpdateProfileAPI.h"
#include "APStoreErrorAPI.h"
#include "APDatabase.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "MMFirebaseDatabase.h"
//#include "DatabaseHelper.h"
#include "MJHelper.h"
#include "APProfileManager.h"

namespace ap {
    namespace api {
        void UpdateProfile::call(const Profile & profile, const std::function<void(bool is_success, std::string msg)> & callback) {
            auto api_call = new UpdateProfile(profile, callback);
            api_call->send();
        }
    
        void UpdateProfile::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        UpdateProfile::UpdateProfile(const Profile & p, const std::function<void(bool, std::string)> & callback) : _profile(p), _callback(callback)
        {
//            auto config_item = buildConfigItem();
			_request = new Base(api::PlatformType::NEW, "account/update-profile", {
                {"age", std::to_string(_profile.age)},
                {"gender", std::to_string(_profile.getGenderCode())},
                {"description", _profile.description},
                {"name", _profile.name},
				{"profile_id", std::to_string(_profile.profile_id)},
                {"config_item", buildConfigItem()},
                {"sync_clevertap", "1"}
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        void UpdateProfile::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {
            //TODO: cập nhật profile vào database
			if (!data.IsNull() && data.IsObject())
			{
				_profile.profile_id = data["profile_id"].GetInt();
			}
			ap::Database::getInstance().updateProfile(_profile);
            
            MJDEFAULT->setIntegerForKey("key_last_profile_updated", _profile.profile_id);
            mm::RealtimeDatabase::getInstance().updateProfileInfo(_profile);
            if (_callback) {
                _callback(true, "");
            }
            delete this;
        }
    
        void UpdateProfile::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "account/update-profile", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
		std::string UpdateProfile::buildConfigItem()
		{
            std::string config_item = "";
            auto key = StringUtils::format("avatar_path_%i", (CONFIG_MANAGER->isVietnameseStories() ? 51 : 40));
            std::map<std::string, std::string> map;
            if(ap::Database::getInstance().getProfileById(_profile.profile_id).description == "" ){
                
            }else{
                //nếu đã có config items
                auto temp = ap::Database::getInstance().getProfileById(_profile.profile_id).description;
                map = JsonStringToDict(temp);
            }
            
            map[key] = _profile.avatar;

            //convert map to Json Object
            rapidjson::Document doc;
            // define document as object
            doc.SetObject();
            // must pass an allocator when the object may need to allocate memory
            rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
            for(auto m: map){
                rapidjson::Value map_key (rapidjson::kStringType);
                map_key.SetString(m.first.c_str() , allocator);
                rapidjson::Value map_value (rapidjson::kStringType);
                map_value.SetString(m.second.c_str(), allocator);
                doc.AddMember(map_key, map_value, allocator);
            }
            
            // convert json to string
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            doc.Accept(writer);
            
            config_item = strbuf.GetString();
            return config_item;
		}
        
        std::map<std::string, std::string> UpdateProfile::JsonStringToDict (std::string const& s){
            auto json_s = mj::helper::splitString(s, ',');
            std::map <std::string, std::string> map;
            std::string key, value;
            for(auto str : json_s){
                auto key_value = mj::helper::splitString(str, ':');
                map[key_value.front()] = key_value.back();
            }
            return map;
        }
    }
}
