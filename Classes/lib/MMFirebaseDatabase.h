//
//  MMFirebaseDatabase.hpp
//  MonkeyMath
//
//  Created by MacOne on 10/7/19.
//

#ifndef MMFirebaseDatabase_hpp
#define MMFirebaseDatabase_hpp

#include "APProfile.h"
#include "firebase/database.h"
#include "firebase/database/listener.h"

#include "MJStoryInfoModel.h"

namespace mm
{
    class UserValueListener : public firebase::database::ValueListener
    {
    public:
        void OnValueChanged(const firebase::database::DataSnapshot& snapshot) override;
        void OnCancelled(const firebase::database::Error& error_code,
                         const char* error_message) override;
    };

    class ProfilesValueListener : public firebase::database::ValueListener
    {
        
    private:
        std::string _value_ref;
        int _profile_id;
    public:
        ProfilesValueListener();
        ProfilesValueListener(const std::string & value_ref, int profile_id);
        void OnValueChanged(const firebase::database::DataSnapshot& snapshot) override;
        void OnCancelled(const firebase::database::Error& error_code,
                         const char* error_message) override;
    };

    class ProfilesChildListener : public firebase::database::ChildListener
    {
        public:
        void OnChildAdded(const firebase::database::DataSnapshot& snapshot,
                          const char* previous_sibling) override ;
        void OnChildChanged(const firebase::database::DataSnapshot& snapshot,
                            const char* previous_sibling) override ;
        void OnChildRemoved(
                            const firebase::database::DataSnapshot& snapshot) override ;
        void OnChildMoved(const firebase::database::DataSnapshot& snapshot,
                          const char* previous_sibling) override ;
        void OnCancelled(const firebase::database::Error& error_code,
                         const char* error_message) override ;
    };

    class RealtimeDatabase {
    public:
        static RealtimeDatabase getInstance();
        
        void isUserExist(int user_id,std::function<void(bool,std::string)> callback, std::function<void(float)> progressCallback);
        void updateUserSettings();
        void updateUserSettings(int user_id, std::function<void()> i_callback = nullptr);
        void updateProfileInfo(ap::Profile profile, std::function<void()> i_callback = nullptr);
        void updateSequenceProfileData(ap::UserInfo user,std::vector<ap::Profile> profiles,const int id);
        void updateInformationEventNetworkError(int user_id, std::string link, std::string screen, std::string error, std::string errorcode, std::string statusCode, std::string remoteAdress, std::function<void()> i_callback = nullptr, bool isPhusEventF2p = false);
        
//        void updateLastLearnedSkill(int user_id,int profile_id,int grade_id,int lesson_id);
        
        void updateTrackingWorksheetsSended(int user_id, int profile_id, const std::string& value, std::function<void()> callback = nullptr);
        void updateTracking(mj::model::LessonInfo lesson,mj::model::ActInfo info,bool update_last_activity, std::function<void()> i_nullptr = nullptr);
		void updateTrackingAndLearnedLessons(mj::model::LessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId = 0, std::function<void()> i_nullptr = nullptr);
        void updateTrackingAndLearnedLessonsAISpeaking(mj::model::AISpeakingLessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId = 0, std::function<void()> i_nullptr = nullptr);

        void updateProfileKeys(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateProfileKeysByKeyAndValue(int profile_id, std::string key, int value, std::function<void()> i_callback = nullptr);

        void updateLearnedLessons(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateLearnedLessonsAISpeaking(int user_id, int profile_id, std::function<void()> i_callback = nullptr);

        void updateReadStories(int user_id,int profile_id, int story_id = 0, std::function<void()> i_callback = nullptr, bool isPushFireStore = true);
        void pushDataFirestore(int user_id,int profile_id, int story_id = 0);
        void updateReadAudios(int user_id,int profile_id, int audio_id = 0, std::function<void()> i_callback = nullptr);
        void updateLoveStories(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateStoriesActivity(int user_id, int profile_id, std::function<void()> i_callback = nullptr);
        void updateLoveAudios(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateActivityLearnOfLessonInReadingComprehension(int profileID, std::string keyActivityLearn, int idActivity, std::function<void()> i_callback = nullptr);
        void deleteLoveStories(int user_id,int profile_id,int i_storyID ,std::function<void()> i_callback = nullptr);
        void dropDataUserFreeFromFireBase(int user_id);
        void deleteLoveAudios(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback = nullptr);

        void updateProfilesData();
        
        void loadUserSettings(const firebase::database::DataSnapshot& snapshot);
        
        void loadProfileInfo(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadLessonLearned(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadReadStories(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadLoveStories(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadStoriesActivity(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadReadAudios(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadLoveAudios(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadProfileKeys(const firebase::database::DataSnapshot& snapshot, int profile_id);

        void loadActivityLearnOfLessonPhonic(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadActivityLearnOfLessonInReadingComprehension(const firebase::database::DataSnapshot& snapshot, int profile_id);
        void loadProfileData(int profileID, std::function<void()> callback);
        void loadProfileDataForUserWindow(int profileID, std::function<void()> callback);
//        void loadLastLearnedSkill(const firebase::database::DataSnapshot& snapshot);
        
        void saveData();
        void retrieveData(std::function<void(bool)> callback = NULL);
        
        void addListener(int user_id);
        void removeListener(int user_id);
		
		void retrieveCustomData(std::function<void(bool success)> callback);
		void addRemindingUpdateInfoPopupSkipVariableListener();
		void updateRemindingUpdateInfoPopupSkipNumber(const int i_num);
		void loadSkipCountNumber(const firebase::database::DataSnapshot& snapshot, int profile_id = -1);

		void multipathUpdates(std::function<void()> i_nullptr = nullptr);


        void updateUserSettingsTest(std::function<void()> i_nullptr = nullptr);
        void updateProfilesTest(std::function<void()> i_nullptr = nullptr);
        void updateStories(std::function<void()> i_nullptr = nullptr);
        void updateSequelize(std::vector<std::map<std::string, firebase::Variant>>  v, int index,int length, firebase::database::Database* database);

        void updateActivityLearnOfLessonPhonic(int user_id, int profile_id, int grade, int numberActivity, std::function<void()> i_callback = nullptr);
        const int MAXSTORY = 200;

        void loadProfileForUsersUseWindows(rapidjson::Value& value, int profile_id);
        void loadUserSettingsForUsersUseWindows(rapidjson::Value& value);
        void loadInfomationForPfofile(rapidjson::Value& value, int profile_id);
        void loadSkipCountNumberForUsersUseWindows(rapidjson::Value& value, int profile_id = -1);
        void isUserExistForUsersUseWindows(int user_id,  std::function<void(bool, std::string)>callback , std::string errorMessage);
        void loadkeySubmitAndroid();

        void loadValueUserLandingpage(int userId, std::function<void()> callbackFunction);
        void updatevalueUserLandingPage(std::string value, std::function<void()> callback);
        void updatevalueProfileKeyPushDataStories(int profileId, int userID, std::function<void()> callback);

        void loadKeyNewLibApp(int profileId, std::function<void()> callback = nullptr);

    private:
        RealtimeDatabase();
        ::firebase::database::Database *database;
        std::string replaceString(std::string subject, const std::string search, const std::string replace);

        float _timer = 0.0f;
    };
    
}

#endif /* MMFirebaseDatabase_hpp */
