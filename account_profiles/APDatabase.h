//
//  APDatabase.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#ifndef APDatabase_hpp
#define APDatabase_hpp

#include <vector>
#include "SQLiteCpp/SQLiteCpp.h"
#include "APProfile.h"

namespace ap {
    class Database {
    public:
        static Database & getInstance() {
            static Database __instance;
            return __instance;
        }
        
        Database();
        
        void saveIdentity(const std::string & user_id, const std::string & access_token);
		void saveUserInfo(const std::string & user_id, const std::string & name, const std::string & email, const std::string & phone_number, const std::string & address, const std::string & userNameCrm);

		UserInfo getUserInfo();
		std::string getAccessToken();
        void updateProfile(const Profile & p);
		void updateProfile(const std::vector<Profile> profiles);
        void deleteProfile(const int profile_id);
		bool checkDuplicateName(const std::string & name,int id = -1);
		bool isReachMaxProfile();
        Profile getProfileById(const int profile_id);
        std::vector<Profile> getProfiles();
		int getTotalProfile();
		void dropDatabase();
		void dropDatabaseProfiles();
        void resetDatabaseIfColumnsChanged(const std::string & user_id, const std::string & name, const std::string & email, const std::string & phone_number, const std::string & address, const std::string & userNameCrm);
    private:
        SQLite::Database * _db = nullptr;
    };
}

#endif /* APDatabase_hpp */
