///////////////////////////////////////////////////////////
//  MSLessonAISpeakingManager.h
//  Implementation of the Class MSLessonAISpeakingManager.h
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#ifndef MSLessonAISpeakingManager_hpp
#define MSLessonAISpeakingManager_hpp

#include "cocos2d.h"
#include "StoryDataModel.h"

namespace ms {
enum CommonDownloadType{
    ON_DOWNLOAD = 0,
    ON_LEARN = 1,
    DEFAULT = 6
};
    class LessonAISpeakingManager {
    public:
        static LessonAISpeakingManager & getInstance() {
            static LessonAISpeakingManager sharedManager;
            return sharedManager;
        };
        
//        void prepareLesson(mj::model::AISpeakingLessonInfo, std::function<void(bool)> callback);
        void prepareLesson(mj::model::AISpeakingLessonInfo, bool retry = false);
        void prepareActivity(mj::model::ActAISpeakingInfo, std::function<void(bool)> callback = nullptr, bool is_retry = false);
        
        static bool isActivityReady(mj::model::ActAISpeakingInfo info);
        static bool isLessonReady(mj::model::AISpeakingLessonInfo info);
        
        int getActivityIndexOfLesson(mj::model::ActAISpeakingInfo info);
        mj::model::ActAISpeakingInfo getNextActivity(mj::model::ActAISpeakingInfo info);
        mj::model::ActAISpeakingInfo getActivityById(int activity_id);
        mj::model::AISpeakingLessonInfo getNextLesson(mj::model::AISpeakingLessonInfo info);
        mj::model::AISpeakingLessonInfo getLessonById(int lesson_id);
        
        int getLastLessonIdOfCategory(int category_id);
        int getLastActivityIdOfCategory(int category_id);
        int getLastActivityIdOfLesson(int lesson_id);
        
        std::vector<int> getFreeLessonId(int category_id);
        
        std::string getKeyEventLessonReady(mj::model::AISpeakingLessonInfo info);
        std::string getKeyEventLessonError(mj::model::AISpeakingLessonInfo info);
        std::string getKeyEventLessonDone(mj::model::AISpeakingLessonInfo info);
        
        std::string getKeyEventActivityReady(mj::model::ActAISpeakingInfo info);
        std::string getKeyEventActivityError(mj::model::ActAISpeakingInfo info);
        std::string getKeyEventActivityProgress(mj::model::ActAISpeakingInfo info);
        std::string getKeyEventCommonProgress(mj::model::ActAISpeakingInfo info);

        void updateLastActivityLearned(mj::model::ActAISpeakingInfo info);
        int getLessonShouldLearn(int cat_id);

        int getActivityShouldLearn(int cat_id);
        int getActivityShouldLearn_4notPhonics(int cat_id, int lesson_id);
        
        std::vector<mj::model::AISpeakingLessonInfo> getAllLessonsByGrade(int grade_id);

        void setLessonLearned(int lesson_id);
        void setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn);
		void setLessonLearned(std::vector<int> lesson_id);
        std::vector<mj::model::AISpeakingLessonInfo> getDownloadedLessons();
        bool isPhonicLesson(mj::model::AISpeakingLessonInfo lesson);
        bool isPhonicCategory(int cat_id);
        bool isActivityAvail(mj::model::ActAISpeakingInfo);
        void updateAllLesson();
        mj::model::AISpeakingLessonInfo getCurrentLesson(); // Dùng cho Auto Play;
        void setCurrentLesson(mj::model::AISpeakingLessonInfo lesson); // Dùng cho Auto Play;
        std::vector<mj::model::AISpeakingLessonInfo> getAllLesson();
        mj::model::AISpeakingLessonInfo onSaveLessonAISpeakingState(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo);
        std::vector<mj::model::AISpeakingLessonInfo> getAllLessonAISpeakingStateFromDB(std::vector<mj::model::AISpeakingLessonInfo> aISpeakingLessonInfos);
        mj::model::AISpeakingLessonInfo getLessonAISpeakingStateFromDBByID(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo);
        bool onDownloadVideoCommons(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos, std::function<void(bool)> callback = nullptr, std::function<void(float)> callbackPercent = nullptr, ms::CommonDownloadType type = ms::CommonDownloadType::ON_DOWNLOAD);
        std::string getActorNameFromConfig(std::string configPath);
        void onLessonError(mj::model::AISpeakingLessonInfo info);
        bool onDeleteDataExperted(mj::model::AISpeakingLessonInfo info);
        void resetIndex(int index) {
            _index = index;
        }
        void onSavelessonLearnAfterLearned(mj::model::AISpeakingLessonInfo lessonInfo);
    protected:
        explicit LessonAISpeakingManager();
    private:

        void updateLessonsState();
                
        void onActivityReady(mj::model::ActAISpeakingInfo info);
        void onActivityError(mj::model::ActAISpeakingInfo info,std::function<void(bool)> callback = nullptr);
        void onActivityProgress(mj::model::ActAISpeakingInfo, double progress);
        void onLessonReady(mj::model::AISpeakingLessonInfo info);
        void onLessonDone(mj::model::AISpeakingLessonInfo info);
        void deleteDownloadedActivity(mj::model::ActAISpeakingInfo );
        int compareLesson(mj::model::AISpeakingLessonInfo a, mj::model::AISpeakingLessonInfo b);
        int getDistanceBetweenLessons(mj::model::AISpeakingLessonInfo a, mj::model::AISpeakingLessonInfo b);
        LessonAISpeakingManager(LessonAISpeakingManager const &);

        void onUpdateLessonAISpeakingStateToDB(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo);
        
        void operator = (LessonAISpeakingManager const &);
        std::vector<mj::model::AISpeakingLessonInfo> _all_lessons;
        int _prepare_attemp = 0;
        int _preparing_lesson_id = -100;
        bool isStoryUnzipError = false;
        int _index = 0;
        bool _resultDeleteDataExpected = false;
        mj::model::AISpeakingLessonInfo _currentLesson;
        
        std::vector<int> _activity_retried;
    };
}

#endif /* MSLessonAISpeakingManager_hpp */
