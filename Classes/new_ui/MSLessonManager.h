//
//  MSLessonManager.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#ifndef MSLessonManager_hpp
#define MSLessonManager_hpp

#include "cocos2d.h"
#include "StoryDataModel.h"

namespace ms {
    class LessonManager {
    public:
        static LessonManager & getInstance() {
            static LessonManager sharedManager;
            return sharedManager;
        };
        
//        void prepareLesson(mj::model::LessonInfo, std::function<void(bool)> callback);
        void prepareLesson(mj::model::LessonInfo, bool retry = false);
        void prepareActivity(mj::model::ActInfo, std::function<void(bool)> callback = nullptr, bool is_retry = false, bool isDownload = false);
        
         bool isActivityReady(mj::model::ActInfo info);
         bool isLessonReady(mj::model::LessonInfo info);
        
        void getLessonState();
        void getLatestActivity();
        void downloadLesson();
        int getActivityIndexOfLesson(mj::model::ActInfo info);
        mj::model::ActInfo getNextActivity(mj::model::ActInfo info);
        mj::model::ActInfo getActivityById(int activity_id);
        mj::model::LessonInfo getNextLesson(mj::model::LessonInfo info);
        mj::model::LessonInfo getLessonById(int lesson_id);
        
        int getLastLessonIdOfCategory(int category_id);
        int getLastActivityIdOfCategory(int category_id);
        int getLastActivityIdOfLesson(int lesson_id);
        
        std::vector<int> getFreeLessonId(int category_id);
        void setCurrentLessonFreeForCategory(int category);
        std::vector<int> getListLessonFree();
        void setListLessonFree(std::vector<int> listLesson);
        void clearDataLessonFree();
        int getCurrentLessonForCategory();
        
        std::string getKeyEventLessonReady(mj::model::LessonInfo info);
        std::string getKeyEventLessonError(mj::model::LessonInfo info);
        std::string getKeyEventLessonDone(mj::model::LessonInfo info);
        
        std::string getKeyEventActivityReady(mj::model::ActInfo info);
        std::string getKeyEventActivityError(mj::model::ActInfo info);
        std::string getKeyEventActivityProgress(mj::model::ActInfo info);

        void updateLastActivityLearned(mj::model::ActInfo info);
        int getLessonShouldLearn(int cat_id);
        int getActivityShouldLearn(int cat_id);
        int getActivityShouldLearn_4notPhonics(int cat_id, int lesson_id);
        
        std::vector<mj::model::LessonInfo> getAllLessonsByGrade(int grade_id);
        
        void setLessonLearned(int lesson_id);
        void setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn);
		void setLessonLearned(std::vector<int> lesson_id);
        std::vector<mj::model::LessonInfo> getDownloadedLessons();
        bool isPhonicLesson(mj::model::LessonInfo lesson);
        bool isPhonicCategory(int cat_id);
        bool isActivityAvail(mj::model::ActInfo);
        void updateAllLesson();
        void ResetCacheAllLesson();
        std::vector<int> getLessonsLearned();

        std::vector<int> getAllLessonsLearned();

        bool checkLessonLearned(int id);

        bool checkLessonReadingAndPhonicLearned(int id);

        mj::model::LessonInfo getCurrentLesson(); // D�ng cho Auto Play;
        void setCurrentLesson(mj::model::LessonInfo lesson); // D�ng cho Auto Play;
        std::vector<mj::model::LessonInfo> getAllLesson();
        void resetIndexDownload();
        void resetListUrlError();
        void setCountClickRetry(int indexClick);
        void setInforCurrenLesson(mj::model::LessonInfo info);
        void onDownLoadActivity(mj::model::LessonInfo info);
        mj::model::ActInfo getActivityBeginDownload(std::vector<mj::model::ActInfo> listActivity, bool isPhonicLesson, int lesssonId, int categoryID);
        void resetLearedLesson();
    protected:
        explicit LessonManager();
    private:
        void updateLessonsState();
        
        void prepareStoryActivity(mj::model::ActInfo info, std::function<void(bool)> callback = nullptr);
        
        void onActivityReady(mj::model::ActInfo info);
        void onActivityError(mj::model::ActInfo info,std::function<void(bool)> callback = nullptr);
        void onActivityProgress(mj::model::ActInfo, double progress);
        void onLessonError(mj::model::LessonInfo info);
        void onLessonReady(mj::model::LessonInfo info);
        void onLessonDone(mj::model::LessonInfo info);

        void deleteDownloadedActivity(mj::model::ActInfo );
       
        
        int compareLesson(mj::model::LessonInfo a, mj::model::LessonInfo b);
        int getDistanceBetweenLessons(mj::model::LessonInfo a, mj::model::LessonInfo b);
        
        
        std::vector<mj::model::LessonInfo> _all_lessons;
        LessonManager(LessonManager const &);
        void operator = (LessonManager const &);
        
        int _prepare_attemp = 0;
        int _preparing_lesson_id = -100;
        bool isStoryUnzipError = false;
        int _catergoryForLessonFree = - 1;
        int _index = 0;
        int _countClickRetry = -1;
        int _maxRetryCount = -1;
        mj::model::LessonInfo _currentLesson;
        std::pair<int, std::string> _valueLinkError = { -1, "" };
        std::string _urlLinkErrorTheSecond = "";
        std::vector<std::string> _listUrlError = {};
        mj::model::LessonInfo _infoLesson;
        
        std::vector<int> _activity_retried;
        std::vector<int> _listLessonFree = {};
        std::vector <std::pair<int, std::vector<mj::model::ActInfo>>> _idLessonAndListActivities = {};
        std::vector<std::string> _valueDomainDownloadStories = {};
        std::vector<std::string> _valueDomainDownloadAudioBookAndActivity = {};
        std::string _typError = "";
        std::vector<int> _listIdOfLessonsLearned = {}, _listIdOfAllLessonsLearned = {};
        int _countRetryDownloadActError = 0;
        int _profileId = 0;
    };
}

#endif /* MSLessonManager_hpp */
