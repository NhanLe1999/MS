//
//  APProfileManager.hpp
//  MonkeyMath
//
//  Created by Minhtzy on 12/08/19.
//

#ifndef AP_PROFILE_MANAGER_HPP
#define AP_PROFILE_MANAGER_HPP

#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "APProfile.h"
#include "MSGradeManager.h"

#define PROFILE_MANAGER ap::ProfileManager::getInstance()

namespace ap {
	class ProfileManager
    {
	public:
        
        enum class BOOK_TYPE
        {
                STORY,
                AUDIOBOOK
        };
        
		static ProfileManager & getInstance();

		void initProfile(int profileID);
		void loadDataFromMJDefault();
		//void restoreFromBackup(const std::string & backup) const;
        std::map<std::string, std::string> getAllDatabase();
        std::map<std::string, std::string> getAllTable(std::string tbl_name);


        void saveReadStories(cocos2d::ValueVector stories);
        void saveReadAudioBooks(cocos2d::ValueVector audios);
        void saveLessonLearned(cocos2d::ValueVector lessons);
        
        void loadReadStories();
        void loadReadAudioBooks();
        void loadLessonLearned();
        
        void saveProcess();
        void loadProcess();
        void clearDataModel();
        
		void switchProfile(int profile_id, bool user_choose = false);
        void clearProfileDefault();
		//void updateProfile(const ap::Profile & p) const;
		//static void updateProfile(int profile_id, const std::string & name, const std::string & avatar, int grade_id);
		//bool removeProfile(int profile_id) const;

		// delete tracking phonics
		void removeTrackingPhonics();

		int getCurrentProfileId() const;
		void setCurrentProfileId(int profile_id);

		/*std::string getProfileName() const;
		std::string getAvatarPath() const;*/
		int getProfileGradeId(ms::GradeManager::Scope scope) const;
		void setProfileGradeId(ms::GradeManager::Scope scope,int grade_id) const;

		void setStringForKey(const std::string & key, const std::string & value) const;
		void setIntegerForKey(const std::string & key, int value) const;

		int getIntegerForKey(const std::string & key, int default_value = 0) const;
		std::string getStringForKey(const std::string & key, std::string default_value = "") const;

		void addStoryToRecentlyRead(int _id);
		cocos2d::ValueVector getStoryRecentlyRead();

		void addAudioToRecentlyRead(int _id);
		cocos2d::ValueVector getAudioRecentlyRead();

		//void addFavoritesStory(int _id);
		//void removeFavoritesStory(int _id);
		//cocos2d::ValueVector getFavoritesStory();
		//bool isFavoritesStory(int _id);

		ProfileManager(ProfileManager const &) = delete;
		void operator =(ProfileManager const &) = delete;
        
        void updateLearnedLesson(int profile_id, int lesson_id, int category_id) const;
        void updateReadStories(int profile_id, int story_id, int time_read) const;
        void updateLoveStories(int profile_id, int story_id) const;
        void updateReadAudios(int profile_id, int story_id, int time_read) const;
        void updateLoveAudios(int profile_id, int story_id) const;
        void updateProfileKeys(int profile_id, std::string key, std::string value) const;
        
        void deleteLoveStories(int profile_id, int story_id) const;
        void deleteLoveAudios(int profile_id, int story_id) const;
        
        void deleteLoveXXXBook(ProfileManager::BOOK_TYPE i_type = BOOK_TYPE::STORY,int profile_id = -1);
        
        std::vector<int> getAllReadStories(int profileID);
        
        std::vector<int> getAllReadAudiobooks(int profileID);

        std::vector<int> getAllLoveStories(int profileID);
        
        std::vector<int> getAllLoveAudiobooks(int profileID);

		std::vector<int> getAllLessonLearned(int profileID);

		void resetFavoriteBooks();
        
    protected:
		explicit ProfileManager();
        void setRow(std::string tbl_name, std::string column_1, std::string column_2, std::string column_3, int value_1, int value_2, int value_3);
		void setDataBaseStringForKey(std::string key, std::string value);
		std::string getDataBaseStringForKey(std::string key, std::string default_value);
	private:
		SQLite::Database * _db;
		int _current_profile;
	};
}

#endif /* AP_PROFILE_MANAGER_HPP */
