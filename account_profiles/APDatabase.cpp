//
//  APDatabase.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#include "APDatabase.h"
#include "cocos2d.h"
#include "MJHelper.h"
#include "MJDefault.h"

#define SQL_DROP_AP_TABLE       "DROP TABLE IF EXISTS identity"
#define SQL_CREATE_AP_TABLE     "CREATE TABLE IF NOT EXISTS identity (_id INTEGER PRIMARY KEY, access_token TEXT NOT NULL,name TEXT,email TEXT, phone TEXT, address TEXT, user_name TEXT)"
#define SQL_UPDATE_AP_TABLE     "UPDATE identity SET name = ?, email = ?, phone = ?,address = ?,user_name = ? WHERE _id = ?"

namespace ap
{
    Database::Database()
    {
        auto db_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "ap.sqlite";
        try
        {
            _db = new SQLite::Database(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        }
        catch (std::exception & e)
        {
            CCLOG("%s:", e.what());
        }
    }

    void Database::saveIdentity(const std::string &user_id, const std::string &access_token)
	{
        try 
		{
			_db->exec(SQL_DROP_AP_TABLE);
			_db->exec(SQL_CREATE_AP_TABLE);
			SQLite::Statement stmt(*_db, "REPLACE INTO identity(_id,access_token) VALUES (?, ?)");
			stmt.bind(1, user_id);
			stmt.bind(2, access_token);
			stmt.exec();

        }
        catch (std::exception & e)
        {
            CCLOG("db exception: %s", e.what());
        }
    }

	void Database::saveUserInfo(const std::string & user_id, const std::string & name, const std::string & email, const std::string & phone_number, const std::string & address, const std::string & userNameCrm)
	{
		try
		{
			//only saved user info when it had save identity
			SQLite::Statement stmt(*_db, SQL_UPDATE_AP_TABLE);
			stmt.bind(1, name);
			stmt.bind(2, email);
			stmt.bind(3, phone_number);
			stmt.bind(4, address);
			stmt.bind(5, userNameCrm);
			stmt.bind(6, user_id);
			stmt.exec();
		}
		catch (std::exception & e)
        {
			CCLOG("db exception: %s", e.what());
            
            resetDatabaseIfColumnsChanged(user_id,name,email,phone_number,address,userNameCrm);
		}
	}

	UserInfo Database::getUserInfo()
	{
		try
		{
			SQLite::Statement stmt(*_db, "SELECT _id,name,email,phone,address,user_name FROM identity");
			if (stmt.executeStep())
			{
				UserInfo user_info;
				user_info._id = stmt.getColumn(0).getInt();
				user_info._name = stmt.getColumn(1).getString();
				user_info._mail = stmt.getColumn(2).getString();
				user_info._phone = stmt.getColumn(3).getString();
				user_info._address = stmt.getColumn(4).getString();
				user_info._userName = stmt.getColumn(5).getString();
				return user_info;
			}
		}
		catch(std::exception & e)
		{
			CCLOG("db exception: %s", e.what());
            
            try
            {
                SQLite::Statement stmt(*_db, "SELECT _id,name,email,phone FROM identity");
                
                if (stmt.executeStep())
                {
                    UserInfo user_info;
                    user_info._id       = stmt.getColumn(0).getInt();
                    user_info._name     = stmt.getColumn(1).getString();
                    user_info._mail     = stmt.getColumn(2).getString();
                    user_info._phone    = stmt.getColumn(3).getString();
                    return user_info;
                }
            }
            catch(std::exception & e)
            {
                CCLOG("db exception: %s", e.what());
            }
		}
        
		return UserInfo();
	}

    std::string Database::getAccessToken() {
        try {
            return _db->execAndGet("SELECT access_token FROM identity").getText();
        } catch (std::exception & e) {
            return "";
        }
    }

    void Database::updateProfile(const Profile & p) {
        try {
            _db->exec("CREATE TABLE IF NOT EXISTS profiles (profile_id INTEGER PRIMARY KEY, name TEXT, gender INTEGER, age INTEGER,avatar TEXT, description TEXT)");
            SQLite::Statement stmt(* _db, "REPLACE INTO profiles VALUES (?, ?, ?, ?, ?, ?)");
            stmt.bind(1, p.profile_id);
            stmt.bind(2, p.name);
            stmt.bind(3, p.getGenderCode());
            stmt.bind(4, p.age);
            stmt.bind(5, (p.avatar == "" )? "mjstory/avatar/boy_1.png": p.avatar);
            stmt.bind(6, p.description);
            stmt.exec();
        } catch (std::exception & e) {
            CCLOG("db exception: %s", e.what());
        }
    }

	void Database::updateProfile(const std::vector<Profile> profiles)
	{
		if (profiles.empty()) return;

		try {
			// delete old profile
			_db->exec("DROP TABLE IF EXISTS profiles");
		}
		catch (std::exception & e) {
			CCLOG("db exception: %s", e.what());

		}

		for (Profile profile : profiles)
		{
			updateProfile(profile);
		}
	}

    void Database::deleteProfile(const int profile_id) {
        try {
            SQLite::Statement stmt(* _db, "DELETE FROM profiles WHERE profile_id = ?");
            stmt.bind(1, profile_id);
            stmt.exec();
        } catch (std::exception & e) {
            CCLOG("db exception: %s", e.what());
        }
    }

	bool Database::checkDuplicateName(const std::string & name,int id /*= -1*/)
	{
		auto c_name = mj::helper::to_lower(name);
		auto profiles = getProfiles();
		for (const auto & p : profiles) {
			auto p_name = mj::helper::to_lower(p.name); // check duplicate name ignore case
			if (p_name.compare(c_name) == 0 && p.profile_id != id) {
				return true;
			}
		}
		return false;
	}

	bool Database::isReachMaxProfile()
	{
		int max_profile = MJDEFAULT->getIntegerForKey(key_max_account_profile, 3);
		return getTotalProfile() >= max_profile;
	}

    std::vector<Profile> Database::getProfiles() {
        std::vector<Profile> ps;
        try {
            SQLite::Statement stmt(* _db, "SELECT * FROM profiles");
            while (stmt.executeStep()) {
                Profile p;
                p.profile_id = stmt.getColumn(0);
                p.name = stmt.getColumn(1).getText();
                p.gender = (stmt.getColumn(2).getInt() == 1) ? Profile::Gender::MALE : Profile::Gender::FEMALE;
                p.age = stmt.getColumn(3).getInt();
                p.avatar = stmt.getColumn(4).getText();
                p.description = stmt.getColumn(5).getText();
                if(p.profile_id <= 0){
                    continue;
                }
                ps.emplace_back(p);
            }
        } catch (std::exception & e) {
            CCLOG("db exception: %s", e.what());
        }
        
        return ps;
    }

	int Database::getTotalProfile()
	{
        return getProfiles().size();
	}

	void Database::dropDatabase()
	{
		_db->exec("DROP TABLE IF EXISTS identity");
		_db->exec("DROP TABLE IF EXISTS profiles");
	}
	void Database::dropDatabaseProfiles()
	{
		_db->exec("DROP TABLE IF EXISTS profiles");
	}
    Profile Database::getProfileById(const int profile_id) {
        Profile p; p.profile_id = -1;
        try {
            SQLite::Statement stmt(* _db, "SELECT * FROM profiles WHERE profile_id = ?");
            stmt.bind(1, profile_id);
            if (stmt.executeStep()) {
                p.profile_id = stmt.getColumn(0);
                p.name = stmt.getColumn(1).getText();
                p.gender = (stmt.getColumn(2).getInt() == 1) ? Profile::Gender::MALE : Profile::Gender::FEMALE;
                p.age = stmt.getColumn(3).getInt();
                p.avatar = stmt.getColumn(4).getText();
                p.description = stmt.getColumn(5).getText();
            }
        } catch (std::exception & e) {
            CCLOG("db exception: %s", e.what());
        }
        
        return p;
    }

    void Database::resetDatabaseIfColumnsChanged(const std::string & user_id, const std::string & name, const std::string & email, const std::string & phone_number, const std::string & address, const std::string & userNameCrm)
    {
        std::string accessToken = "";

        //the columns of table has modified -> so we need to get the access_token from the old table.
        try
        {
          SQLite::Statement stmt(*_db, "SELECT access_token FROM identity");
          
          if (stmt.executeStep())
          {
              accessToken = stmt.getColumn(0).getString();
          }
        }
        catch(std::exception & e)
        {
          CCLOG("db exception: %s", e.what());
          
        }

        //add it to
        try
        {
          _db->exec(SQL_DROP_AP_TABLE);
          _db->exec(SQL_CREATE_AP_TABLE);
          SQLite::Statement stmt(*_db, "REPLACE INTO identity(_id,access_token,name,email,phone,address,user_name) VALUES (?, ?, ?, ?, ?, ?, ?)");
          stmt.bind(1, user_id);
          stmt.bind(2, accessToken);
          stmt.bind(3, name);
          stmt.bind(4, email);
          stmt.bind(5, phone_number);
          stmt.bind(6, address);
          stmt.bind(7, userNameCrm);
          stmt.exec();

        }
        catch (std::exception & e)
        {
          CCLOG("db exception: %s", e.what());
        }
    }

}

