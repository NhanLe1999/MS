
#ifndef MMFirebaseDatabase_Huawei_hpp
#define MMFirebaseDatabase_Huawei_hpp

#include "APProfile.h"
#include "firebase/database.h"
#include "firebase/database/listener.h"
#include "MJStoryInfoModel.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "json/writer.h"
#include "json/stringbuffer.h"



namespace mm
{
    class RealtimeDatabaseHuawei {
    public:
        static RealtimeDatabaseHuawei getInstance();
        
        void isUserExist(int user_id,std::function<void(bool,std::string)> callback, std::function<void(float)> progressCallback);
        void updateUserSettings();
        void updateUserSettings(int user_id, std::function<void()> i_callback = nullptr);
        void updateProfileInfo(ap::Profile profile, std::function<void()> i_callback = nullptr);
        void updateSequenceProfileData(ap::UserInfo user,std::vector<ap::Profile> profiles,const int id);

        std::string createJSONString(rapidjson::Document& document, std::vector<std::pair<std::string, firebase::Variant>> data, std::vector<int> type);

        void updateTrackingWorksheetsSended(int user_id, int profile_id, const std::string& value, std::function<void()> callback = nullptr);
        void updateTracking(mj::model::LessonInfo lesson,mj::model::ActInfo info,bool update_last_activity, std::function<void()> i_nullptr = nullptr);
		void updateTrackingAndLearnedLessons(mj::model::LessonInfo lesson, mj::model::ActInfo info, bool update_last_activity, int lessonId = 0, std::function<void()> i_nullptr = nullptr);

        void updateProfileKeys(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateLearnedLessons(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateReadStories(int user_id,int profile_id, int story_id = 0, std::function<void()> i_callback = nullptr);
        void updateReadAudios(int user_id,int profile_id, int audio_id = 0, std::function<void()> i_callback = nullptr);
        void updateLoveStories(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void updateLoveAudios(int user_id,int profile_id, std::function<void()> i_callback = nullptr);
        void deleteLoveStories(int user_id,int profile_id,int i_storyID ,std::function<void()> i_callback = nullptr);
        void deleteLoveAudios(int user_id,int profile_id,int i_storyID, std::function<void()> i_callback = nullptr);

        void updateProfilesData();

        void loadProfileData(int profileID, std::function<void()> callback);
        void loadProfileDataForUserWindow(int profileID, std::function<void()> callback);
        
        void saveData();

        void updateActivityLearnOfLessonPhonic(int user_id, int profile_id, int grade, int numberActivity, std::function<void()> i_callback = nullptr);
        void updateActivityLearnOfLessonInReadingComprehension(int profileID, std::string keyActivityLearn, int idActivity, std::function<void()> i_callback = nullptr);
        void updateProfileKeysByKeyAndValue(int profile_id, std::string key, int value, std::function<void()> i_callback = nullptr);
        const int MAXSTORY = 200;

        void loadProfileForUsersUseWindows(rapidjson::Value& value, int profile_id);
        void loadUserSettingsForUsersUseWindows(rapidjson::Value& value);
        void loadSkipCountNumberForUsersUseWindows(rapidjson::Value& value, int profile_id = -1);
        void isUserExistForUsersUseWindows(int user_id,  std::function<void(bool, std::string)>callback , std::string errorMessage);

        std::string createArgStr(std::vector<std::string> keys, std::vector<std::string> values, std::vector<int> types);
        std::function<void()> _callback;

    private:
        RealtimeDatabaseHuawei();
        ::firebase::database::Database *database;
        std::string replaceString(std::string subject, const std::string search, const std::string replace);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        jstring firebaseUrl;

        std::string className = "org.cocos2dx.cpp.MMFirebase";
        std::string updateMethod = "UpdateChild";
        std::string removeMethod = "removeChild";
#endif // 0

        float _timer = 0.0f;

        void
        updateTrackingAndLearnedLessonsAISpeaking(mj::model::LessonInfo lesson,
                                                  mj::model::ActInfo info,
                                                  bool update_last_activity, int lessonId,
                                                  std::function<void()> callback);

        void
        updateLearnedLessonsAISpeaking(int user_id, int profile_id, std::function<void()> callback);
    };
    
}

#endif /* MMFirebaseDatabase_Huawei_hpp */
#endif
