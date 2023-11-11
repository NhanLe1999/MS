//
//  MSLessonManager.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/17.
//

#include "MSLessonManager.h"
#include "StoryLanguageManager.h"
#include "MJAPILessonDetails.h"
#include "MSDownloader.h"
#include "MJAPITrackLesson.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"

#include "MSServerManager.h"
#include "APProfileManager.h"
#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "MMFirebaseDatabase.h"

#include "UnzipErrorPopup.h"
#include "MsNewDownloadFile.h"
#include "F2PManager.h"
namespace ms {
    LessonManager::LessonManager() {
//#if (defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0))
        _all_lessons.clear();
        _listIdOfAllLessonsLearned.clear();
        updateLessonsState();
//#endif
    }

    void LessonManager::updateAllLesson(){
        _all_lessons.clear();
        _listIdOfAllLessonsLearned.clear();
        
        updateLessonsState();
    }

    void LessonManager::ResetCacheAllLesson() {
        _all_lessons.clear();
        _listIdOfAllLessonsLearned.clear();
        updateLessonsState();

    }
    std::vector<int> LessonManager::getLessonsLearned() {
        if(_listIdOfLessonsLearned.empty())
            _listIdOfLessonsLearned = PROFILE_MANAGER.getAllLessonLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
        return _listIdOfLessonsLearned;
    }
    std::vector<int> LessonManager::getAllLessonsLearned() {
        // bao gom ca lesson phonic va reading 
        if (_listIdOfAllLessonsLearned.empty())
        {
            _listIdOfAllLessonsLearned = PROFILE_MANAGER.getAllLessonLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));
            if (_all_lessons.empty())
            {
                _all_lessons = StoryDataModel::getInstance()->getAllLessonsByLanguage(StoryLanguageManager::getCurrentLangId());
            }
            for (auto lesson : _all_lessons) {
                if (lesson.is_learned)
                {
                    _listIdOfAllLessonsLearned.push_back(lesson.lesson_id);
                }
            }
        }

        return _listIdOfAllLessonsLearned;
    }
    bool LessonManager::checkLessonLearned(int id)
    {
        std::vector<int> listIdOfLessonsLearned = getLessonsLearned();
        for (int i = 0; i < listIdOfLessonsLearned.size(); i++)
        {
            if (id == listIdOfLessonsLearned[i])
            {
                return true;
            }
        }
        return false;
    }
    bool LessonManager::checkLessonReadingAndPhonicLearned(int id)
    {
        std::vector<int> listIdOfLessonsLearned = getAllLessonsLearned();
        for (int i = 0; i < listIdOfLessonsLearned.size(); i++)
        {
            if (id == listIdOfLessonsLearned[i])
            {
                return true;
            }
        }
        return false;
    }
    void LessonManager::updateLessonsState() {
        _profileId = ap::ProfileManager::getInstance().getCurrentProfileId();

        _all_lessons = StoryDataModel::getInstance()->getAllLessonsByLanguage(StoryLanguageManager::getCurrentLangId());

        auto grades = StoryDataModel::getInstance()->getAllNewGrades(1);
        
        if (CONFIG_MANAGER->isVietnameseStories()){
            grades = StoryDataModel::getInstance()->getAllNewGrades(StoryLanguageManager::getCurrentLangId());
        }
        if (CONFIG_MANAGER->isLearnAllLessons()) {
            for (auto &lesson : _all_lessons) {
                lesson.state = mj::model::LessonInfo::State::UNLOCKED;
            }
            return;
        }
        
        std::vector<mj::model::LessonInfo> last_lessons;
        for (auto grade : grades) {
            auto idx = getLastLessonIdOfCategory(grade.grade_id);
            auto last = getLessonById(idx);
            if (last.is_learned) {
                last = getNextLesson(last);
            }
            last_lessons.push_back(last);
            
            if (idx == -1) {
                for (auto &l : _all_lessons) {
                    if (l.category_id == grade.grade_id) {
                        l.state = mj::model::LessonInfo::State::UNLOCKED;
                        break;
                    }
                }
            }
        }
        
        for (auto &lesson : _all_lessons) {
            // nếu là Lessons:Comprehension của MS thì unlock luôn
            if (!CONFIG_MANAGER->isVietnameseStories() && !isPhonicLesson(lesson)) {
                if (lesson.state == mj::model::LessonInfo::State::LOCKED) {
                    lesson.state = mj::model::LessonInfo::State::UNLOCKED;
                }
                
                if (lesson.is_learned) {
                    int last_act_id = getLastActivityIdOfLesson(lesson.lesson_id);
                    
                    if (last_act_id == 1) {         /* giá trị 1 là của user phiên bản cũ đã chơi hết lesson mới được push lên firebase */
                        lesson.is_learned = true;
                    } else if (last_act_id == -1) { /* giá trị -1 khi lesson chưa có activity nào được chơi xong */
                        lesson.is_learned = false;
                    } else {                        /* các giá trị còn lại, là activity_id đã chơi xong của lesson */
                        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
                        
                        if (activities.size() && activities.back().activity_id == last_act_id) {
                            lesson.is_learned = true;
                        } else {
                            lesson.is_learned = false;
                        }
                    }
                    
                    // đồng bộ is_learned và state
                    if (lesson.is_learned) {
                        lesson.state = mj::model::LessonInfo::State::LEARNED;
                    }
                }
                
                continue;
            }
            
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
			// FIXME: unlocked all lessons phonic. REMOVEME next release
   /*         if(lesson.category_id == 479 || lesson.category_id == 480 || lesson.category_id == 481 || lesson.category_id == 469) {
                lesson.state = mj::model::LessonInfo::State::UNLOCKED;
                continue;
            }*/
#endif
            for (auto anchor : last_lessons) {
                auto c = compareLesson(lesson, anchor);
                if (c == 0) {
                    continue;
                }
                
                if (c == 1) {
                    if (lesson.is_learned)
                        lesson.state = mj::model::LessonInfo::State::LEARNED;
                    else
                        lesson.state = mj::model::LessonInfo::State::UNLOCKED;
                }
                
                if (c == 2) {
                    lesson.state = mj::model::LessonInfo::State::LOCKED;
                }
                
                if (c == 3) {
                    lesson.state = mj::model::LessonInfo::State::UNLOCKED;
                }
            }
        }
        
        for (auto lesson : last_lessons) {
            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
            if (activities.empty()) {
                continue;
            }
            auto last_activity_id = getLastActivityIdOfCategory(lesson.category_id);
            if (last_activity_id == activities.rbegin()->activity_id) {
                //unlock next activity
                auto next_lesson = getNextLesson(lesson);
                for (auto &l : _all_lessons) {
                    if (l.lesson_id == next_lesson.lesson_id) {
                        l.state = mj::model::LessonInfo::State::UNLOCKED;
                    }
                    if (l.lesson_id == lesson.lesson_id) {
                        l.state = mj::model::LessonInfo::State::LEARNED;
                    }
                }
            }
            
        }
    }
    
    std::vector<mj::model::LessonInfo> LessonManager::getAllLessonsByGrade(int grade_id) {
        std::vector<mj::model::LessonInfo> t;
        std::copy_if(_all_lessons.begin(), _all_lessons.end(), std::back_inserter(t), [grade_id](mj::model::LessonInfo l) {
            return l.category_id == grade_id;
        });
        
        return t;
    }
    
    int LessonManager::compareLesson(mj::model::LessonInfo a, mj::model::LessonInfo b) {
        if (a.category_id != b.category_id) {
            return -1;
        }
        
        if (a.lesson_order < b.lesson_order) {
            return 1;
        }
        
        if (a.lesson_order > b.lesson_order) {
            return 2;
        }
        
        return 3;
    }
    
    int LessonManager::getDistanceBetweenLessons(mj::model::LessonInfo a, mj::model::LessonInfo b) {
        if (a.category_id != b.category_id) {
            return INT32_MAX;
        }
        
        auto all = getAllLessonsByGrade(a.category_id);
        auto a_it = std::find_if(std::begin(all), std::end(all), [a](mj::model::LessonInfo l) {
            return l.lesson_id == a.lesson_id;
        });
        
        auto b_it = std::find_if(std::begin(all), std::end(all), [b](mj::model::LessonInfo l) {
            return l.lesson_id == b.lesson_id;
        });
        
        if (a_it == std::end(all)) {
            return INT32_MAX;
        }
        
        if (b_it == std::end(all)) {
            return INT32_MAX;
        }
        
        auto distance = std::distance(a_it, b_it);
        if (distance < 0) {
            return INT32_MAX;
        }
        
        return distance;
    }
    
    mj::model::ActInfo LessonManager::getActivityById(int activity_id) {
        return StoryDataModel::getInstance()->getActivityById(activity_id);
    }
    
    mj::model::ActInfo LessonManager::getNextActivity(mj::model::ActInfo info) {
        mj::model::ActInfo r;
        r.activity_id = -1;
        r.lesson_id = -1;
        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::LessonInfo l) {
            return l.lesson_id == info.lesson_id;
        });
        
        if (it == std::end(_all_lessons)) {
            return r;
        }
        
        auto lesson_info = * it;
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_info.lesson_id);
        auto a_it = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActInfo a) {
            return a.activity_id == info.activity_id;
        });
        
        if (a_it == std::end(activities)) {
            return r;
        }
        
        a_it++;
        if (a_it == std::end(activities)) {
            return r;
        }
        
        if (a_it != std::end(activities))
            r = * a_it;
        return r;
    }
    
    mj::model::LessonInfo LessonManager::getLessonById(int lesson_id) {
        if (_all_lessons.empty())
        {
            _all_lessons = StoryDataModel::getInstance()->getAllLessonsByLanguage(StoryLanguageManager::getCurrentLangId());
        }

        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lesson_id](mj::model::LessonInfo l) {
            return l.lesson_id == lesson_id;
        });
        
        mj::model::LessonInfo r;
        r.lesson_id = -1;
        
        if (it != std::end(_all_lessons)) {
            r = * it;
        }
        
        return r;
    }
    
    mj::model::LessonInfo LessonManager::getNextLesson(mj::model::LessonInfo info) {
        if (info.lesson_id == -1) {
            return info;
        }
        
        std::vector<mj::model::LessonInfo> t;
        std::copy_if(_all_lessons.begin(), _all_lessons.end(), std::back_inserter(t), [info](mj::model::LessonInfo l) {
            return l.category_id == info.category_id;
        });

		if (info.lesson_id == t.rbegin()->lesson_id) {
			return info;
		}
        
        mj::model::LessonInfo r;
        r.lesson_id = -1;
        auto it = std::find_if(std::begin(t), std::end(t), [info](mj::model::LessonInfo l) {
            return l.lesson_id == info.lesson_id;
        });
        
        if (it != std::end(t)) {
            ++it;
            if (it != std::end(t)) {
                r = * it;
                return r;
            }
        }
        
        return r;
    }

    mj::model::ActInfo LessonManager::getActivityBeginDownload(std::vector<mj::model::ActInfo> listActivity, bool isPhonicLesson, int lesssonId, int categoryId)
    {
        mj::model::ActInfo infoAct;
        
        if(listActivity.empty())
        {
            return infoAct;
        }
        
        infoAct = listActivity[0];
        
        int actID = 0;
        if(!isPhonicLesson)
        {
            std::string keyActivityLearn = StringUtils::format(key_last_activity_x_y, _profileId, lesssonId);
            actID = MJDEFAULT->getIntegerForKey(keyActivityLearn, 0);
        }else{
            std::string key =  StringUtils::format(key_last_activity_x, categoryId);
            actID = MJDEFAULT->getIntegerForKey(key, 0);;
        }
        
        if(actID == -1)
        {
            actID = 0;
        }
        
        for(int i = 0; i < listActivity.size(); i++)
        {
            if(listActivity[i].activity_id == actID)
            {
                infoAct = listActivity[i];
                break;
            }
        }
        
        return infoAct;
        
       // return ;
    }

    void LessonManager::resetLearedLesson()
    {
        if(!_listIdOfLessonsLearned.empty())
            _listIdOfLessonsLearned.clear();
        if (!_listIdOfAllLessonsLearned.empty())
            _listIdOfAllLessonsLearned.clear();
    }
    
    void LessonManager::updateLastActivityLearned(mj::model::ActInfo info) {
        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::LessonInfo i) {
            return i.lesson_id == info.lesson_id;
        });

        auto lesson_info = * it;
        auto lesson_index = std::distance(std::begin(_all_lessons), it);
        
        bool is_4notPhonics = !CONFIG_MANAGER->isVietnameseStories() && !isPhonicLesson(lesson_info); // MS và không phải phonics
        
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_info.lesson_id);
        auto it2 = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActInfo i) {
            return i.activity_id == info.activity_id;
        });
        
        auto activity_index = std::distance(std::begin(activities), it2);
        
        auto current_lesson_id = getLastLessonIdOfCategory(lesson_info.category_id);// MJDEFAULT->getIntegerForKey("key_last_lesson", -1);
        it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [current_lesson_id](mj::model::LessonInfo i) {
            return i.lesson_id == current_lesson_id;
        });
        auto current_lesson_index = (it == std::end(_all_lessons) ? -1 : std::distance(std::begin(_all_lessons), it));
        
        auto current_activity_id = is_4notPhonics ? getLastActivityIdOfLesson(lesson_info.lesson_id) : getLastActivityIdOfCategory(lesson_info.category_id);// MJDEFAULT->getIntegerForKey("key_last_activity", -1);
        it2 = std::find_if(std::begin(activities), std::end(activities), [current_activity_id](mj::model::ActInfo i) {
            return i.activity_id == current_activity_id;
        });
        auto current_activity_index = (it2 == std::end(activities) ? -1 : std::distance(std::begin(activities), it2));
        
        if (is_4notPhonics || lesson_index >= current_lesson_index){
            //||F2P_MANAGER->isFreeContentPage()) {
            //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_lesson_x, lesson_info.category_id), lesson_info.lesson_id);
            
            std::string key = StringUtils::format(key_last_lesson_x, lesson_info.category_id);
            PROFILE_MANAGER.setIntegerForKey(key, lesson_info.lesson_id);
            if (activity_index >= current_activity_index) {
                //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_activity_x, lesson_info.category_id), info.activity_id);
                if (is_4notPhonics) {
                    std::string keyActivityLearn = StringUtils::format(key_last_activity_x_y, _profileId, lesson_info.lesson_id);
                    MJDEFAULT->setIntegerForKey(keyActivityLearn, info.activity_id);
                    mm::RealtimeDatabase::getInstance().updateActivityLearnOfLessonInReadingComprehension(_profileId, keyActivityLearn, info.activity_id);
                } else {
                    PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_activity_x, lesson_info.category_id), info.activity_id);
                }
            }

			auto user_info = ap::Database::getInstance().getUserInfo();

            auto valueLessonLearn = StoryDataModel::getInstance()->getLessonLearnByID(lesson_info.lesson_id);

            if (valueLessonLearn.size() > 0)
            {
                PROFILE_MANAGER.insertValueToTableLessonLearn(valueLessonLearn);
                mm::RealtimeDatabase::getInstance().updateTrackingAndLearnedLessons(lesson_info, info, activity_index >= current_activity_index, lesson_info.lesson_id);
            }
			MJDEFAULT->setBoolForKey("key_remove_phonics_tracking", false);

            
            
            updateLessonsState();
        }
        if(F2P_MANAGER->isFreeContentPage())
        {
            auto valueLessonLearn = StoryDataModel::getInstance()->getLessonLearnByID(lesson_info.lesson_id);

            if (valueLessonLearn.size() > 0)
            {
                PROFILE_MANAGER.insertValueToTableLessonLearn(valueLessonLearn);
                mm::RealtimeDatabase::getInstance().updateTrackingAndLearnedLessons(lesson_info, info, true, lesson_info.lesson_id);
            }
        }
        //AddEventTracking 
        ms::f2p::FreeContentOption freeContentOption = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option);
        if (info.activity_id == activities.at(activities.size() - 1).activity_id
            && (freeContentOption == ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM || freeContentOption == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY))
        {
            F2P_MANAGER->pushEventTrackingDoneItemFree(info.lesson_id);
        }
        
        //remove hand guide of previous lessons
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(key_event_play_locked_lesson);
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_reload_lesson_state);
    }
    
    int LessonManager::getActivityShouldLearn_4notPhonics(int cat_id, int lesson_id) {
        auto act_id = getLastActivityIdOfLesson(lesson_id);
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);
        
        if (act_id == -1) {
            return activities.begin()->activity_id;
        } else {
            for (auto i=0; i<activities.size(); i++) {
                auto act = activities.at(i);
                if (act.activity_id == act_id) {
                    return i == activities.size()-1 ? activities.at(i).activity_id : activities.at(i+1).activity_id;
                }
            }
        }
        
        return activities.begin()->activity_id;
    }
    
    int LessonManager::getActivityShouldLearn(int cat_id) {
        auto lesson_id = getLastLessonIdOfCategory(cat_id);
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);
        if (activities.empty()) {
            return 0;
        }
        
        auto act_id = getLastActivityIdOfCategory(cat_id);
        if (act_id == 0) {
            return 0;
        }
        if (act_id == activities.rbegin()->activity_id) {
            return 0;
        }
        return getNextActivity(getActivityById(act_id)).activity_id;
    }
    
    int LessonManager::getLessonShouldLearn(int cat_id) {
        auto lesson_id = getLastLessonIdOfCategory(cat_id);
        auto l = getLessonById(lesson_id);
        if (l.is_learned) {
            lesson_id = getNextLesson(l).lesson_id;
            return lesson_id;
        }
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);
        if (activities.empty()) {
            return lesson_id;
        }
        
        auto act_id = getLastActivityIdOfCategory(cat_id);
        if (act_id == 0) {
            return lesson_id;
        }
        if (act_id == activities.rbegin()->activity_id) {
            return getNextLesson(getLessonById(lesson_id)).lesson_id;
        }
        return lesson_id;
    }
    
    int LessonManager::getLastLessonIdOfCategory(int category_id) {
        // nếu là MS và Lessons-Comprehension
        if (!CONFIG_MANAGER->isVietnameseStories() && !isPhonicCategory(category_id)) {
            auto all = getAllLessonsByGrade(category_id);
            auto lastPlayedLesson_id = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_lesson_x, category_id), -1);
            
            int preLast = -1; // index
            int inxLast = -1; // index
            int nexLast = -1; // index
            
            for (auto i=0; i<all.size(); i++) {
                auto lesson = all.at(i);
                
                if (lesson.lesson_id == lastPlayedLesson_id) {
                    if (!lesson.is_learned) {
                        return lastPlayedLesson_id;
                    } else {
                        inxLast = i;
                        continue;
                    }
                }
                
                if (!lesson.is_learned) {
                    if (inxLast == -1) {
                        preLast = i;
                    } else if (nexLast == -1) {
                        nexLast = i;
                    }
                }
            }
            
            /*
             Với MS và Lessons-Comprehension thì trả về bài học user học gần đây nhất nếu chưa học xong, còn nếu user đã học xong bài học đó thi:
             - trả về bài tiếp theo gần nhất mà chưa học xong hoặc
             - trả về bài ở dưới gần nhất mà chưa học xong
             - user lần đầu vào Lessons-Comprehension thì trả về bài học đầu tiên
             */
            
            if (nexLast != -1) {
                return all.at(nexLast).lesson_id;
            } else if (preLast != -1) {
                if (inxLast == -1) {
                    return all.begin()->lesson_id;
                } else {
                    return all.at(preLast).lesson_id;
                }
            }
            
            return 0; // đã học tất cả các bài trong category
        }
        
        // các trường hợp còn lại
        auto all = getAllLessonsByGrade(category_id);
        auto lesson_zero = -1;
        auto order_zero = 0;
        if (!all.empty()) {
            lesson_zero = all.front().lesson_id;
			for (auto i = 0; i < (int)all.size(); ++i) {
				auto l = all[i];
                if ((l.is_learned || l.state == mj::model::LessonInfo::UNLOCKED) && l.lesson_order > order_zero) {
                    lesson_zero = l.lesson_id;
                    order_zero = l.lesson_order;
                }
            }
        }
        
        return lesson_zero;
//        auto lesson_zero = getAllLessonsByGrade(category_id)[0].lesson_id;
//        return MJDEFAULT->getIntegerForKey(StringUtils::format("key_last_lesson_%d", category_id), lesson_zero);
//        return PROFILE_MANAGER.getIntegerForKey(key_last_lesson_x, category_id);
    }
    
    int LessonManager::getLastActivityIdOfCategory(int category_id) {
        //return MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_activity_x, category_id), -1);
        return PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_activity_x, category_id), -1);
    }
    
    int LessonManager::getLastActivityIdOfLesson(int lesson_id) {
        return MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_activity_x_y, ap::ProfileManager::getInstance().getCurrentProfileId(), lesson_id), -1);
    }
    
    void LessonManager::prepareLesson(mj::model::LessonInfo info, bool retry) {
        if (retry) {
            /*
            * If retry, increase the variable which save how many times this lesson has been redownload.
            */
            _prepare_attemp++;
            if (_prepare_attemp >= 2) {
                onLessonError(info);
                return;
            }
        } else {
            _prepare_attemp = 0;
        }

        /*
        * prepare_attemp is a variable which is used to count how many times a lesson was redownload
        * If prepare >= 2 ( == 2) means that lessons has been downloaded two times, so client does not download it again,
        * and show an Error popup.
        */
        auto current_lesson = getLessonById(getLessonShouldLearn(info.category_id));
        auto nextLesson = getNextLesson(current_lesson);
        auto distance = getDistanceBetweenLessons(current_lesson, info);

        if (CONFIG_MANAGER->isLearnAllLessons()) {
        } else if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonManager::getInstance().isPhonicLesson(info)) {

        } else if (distance > 5 && distance != INT32_MAX && !F2P_MANAGER->isFreeContentPage()) {
            // ????
            CCLOG("STOP CACHING");
            Director::getInstance()->getScheduler()->schedule([info](float dt) {
                LessonManager::getInstance().onLessonError(info);
            }, Application::getInstance(), 0, 0, 0.1, false, "delay_dispatch_lesson_error");
            return;
        }
        
       // if(_typError.length() == 0)
        {
            _typError = MJDEFAULT->getStringForKey(key_error_url_stories, "None");
        }

        if (_valueDomainDownloadStories.empty())
        {
            _valueDomainDownloadStories = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::STORY);
        }

        if (_valueDomainDownloadAudioBookAndActivity.empty())
        {
            _valueDomainDownloadAudioBookAndActivity = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
        }
        
        auto acts = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id, StoryDataModel::ALL, _valueLinkError, _countClickRetry, _valueDomainDownloadStories, _valueDomainDownloadAudioBookAndActivity, _typError);
        _valueLinkError = {-1, ""};
        if (!acts.empty()) {
            /*
            * First time select a lesson, client does not have any information about this lesson: acts's size = 0
            * So if the amount of acvitities of a lesson greater than 0 means that lesson has been downloaded before,
            *  and client don't have to download again.
            */
            auto id = getActivityBeginDownload(acts, isPhonicLesson(info), info.lesson_id, info.category_id);
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(StringUtils::format("%s_%d", key_event_got_lesson_details, info.lesson_id));
            LessonManager::getInstance().prepareActivity(id, [this, info](bool activity_success) {
                // Get lesson info success
                if (activity_success) {
                    if (MJDEFAULT->getStringForKey(key_read_from) == "Library" && _index == 1)
                    {
                        CleverTapManager::GetInstance()->pushEvent("redownload_item", {
                            {"item_name",cocos2d::Value(info.name)},
                            {"item_id",cocos2d::Value(info.lesson_id)},
                            {"item_type",cocos2d::Value("lesson")},
                            {"disk_space_available",cocos2d::Value(StringUtils::format("%0.2fG", mj::helper::getDiskSpace()))},
                            {"total_downloaded_cache",cocos2d::Value(StringUtils::format("%0.2fMB", mj::helper::getTotalSizeFileDownload()))},
                            });
                    }
                    LessonManager::getInstance().onLessonReady(info);
                } else {
                    LessonManager::getInstance().onLessonError(info);
                }
            });
            return;
        }
        
        
            /*
            * If this lesson has not been download, client has to request this lesson's information to perform other actions.
            */
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                /*
                * End of download activity
                * Push to firebase crashlytics and clevertap
                */

                bool isSuccess = false;             // success or not
                std::string errorType;      // which type of error
                int lessonID;               // lesson ID
                std::string lessonName;     // lessonName;
                std::string lessonType;
                lessonID = info.lesson_id;
                auto lesson = ms::LessonManager::getInstance().getLessonById(lessonID);
                lessonName = lesson.name;
                auto isPhonic = isPhonicLesson(lesson);
                lessonType = isPhonic ? "PHONICS" : "STORIES";

				
                    isSuccess = true;
                    errorType = "none";
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(StringUtils::format("%s_%d", key_event_got_lesson_details, info.lesson_id));
					auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
					if (activities.empty()) {
                        /*
                        * This lesson has no activities.
                        */
                        //errorType = "No activity";
						LessonManager::getInstance().onLessonError(info);
						return;
					}
                    auto id = getActivityBeginDownload(activities, isPhonicLesson(info), info.lesson_id, info.category_id);
					LessonManager::getInstance().prepareActivity(id, [this, info](bool activity_success) {
                        /*
                        * Prepare the first activity.
                        */
						if (activity_success) {
							LessonManager::getInstance().onLessonReady(info);
						}
						else {
							LessonManager::getInstance().onLessonError(info);
						}
					});
				
				

                ms::EventLogger::getInstance().logEvent("learn_lesson_download", {
                    {"is_success", cocos2d::Value(isSuccess)},
                    {"failed_reason", cocos2d::Value(errorType)},
                    {"lesson_id", cocos2d::Value(lessonID)},
                    {"lesson_name", cocos2d::Value(lessonName)},
                    {"lesson_type", cocos2d::Value(lessonType)}
                    }, true);
			});
    }

    
    void LessonManager::prepareActivity(mj::model::ActInfo info, std::function<void(bool)> callback, bool is_retry, bool isDownload) {
        /*
        * Prepare activity:
        * * If this activity is a game, then do (??)
        * * Else if this activity is a story, then upzip it
        * Prepare the next activity.
        */
        if (isActivityReady(info)) {
            /*
            * This activity has already been in this device
            */
            callback ? callback(true) : (void) nullptr;
            onActivityReady(info);
            return;
        }

        auto success_event = StringUtils::format("download_success_%d_%d", info.lesson_id, info.activity_id);
        auto error_event = StringUtils::format("download_error_%d_%d", info.lesson_id, info.activity_id);
        auto progress_event = StringUtils::format("download_progress_%d_%d", info.lesson_id, info.activity_id);

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
        
        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActInfo::Type::STORY) {
            relative_directory = "mjstory/";
        }

        std::sort(info.zip_urls.begin(), info.zip_urls.end());
        info.zip_urls.erase(unique(info.zip_urls.begin(), info.zip_urls.end()), info.zip_urls.end());

        for (auto url : info.zip_urls) {
            ms::utils::DownloadUnit unit;
            unit.url = url;
            unit.download_id = mj::helper::getFilenameFromURL(unit.url);
            unit.forderPath = FileUtils::getInstance()->getWritablePath() + relative_directory;
            unit.storage_path = unit.forderPath + unit.download_id;
            unit.success_event = success_event;
            unit.failed_event = error_event;
            unit.progress_event = progress_event;
            unit.typeUrl = StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME;
            
            if (is_retry) {
                if (!MJDEFAULT->getStringForKey(key_alternative_domain_lesson).empty())
                    unit.url = mj::helper::replaceDomain(unit.url, MJDEFAULT->getStringForKey(key_alternative_domain_lesson));
            }
            
            CCLOG("activity: %s", info.name.c_str());
            CCLOG("activity url: %s", url.c_str());
            if (!FileUtils::getInstance()->isFileExist(unit.storage_path))
            {
                if(ms::NewDownload::getInstance().isNewDownload())
                {
                    ms::NewDownload::getInstance().initDownload(unit,"lesson_list", "url_activity",ms::NewDownload::TYPE_LINK_URL::LESSON);
                }else{
                    ms::utils::Downloader::getInstance().enqueue(unit);
                }
            }
        }
        CCLOG("remove listener %s", success_event.c_str());
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
        
        CCLOG("add listener %s", success_event.c_str());
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(success_event, [this, callback, info, success_event, error_event, progress_event](EventCustom * e) {
            _listUrlError.clear();
            if (LessonManager::isActivityReady(info)) {
                
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
                
                if (info.type == mj::model::ActInfo::Type::STORY) {
                    /*
                    * If this story is a story then upzip it right after success downloaded.
                    */
                    auto filename = mj::helper::getFilenameFromURL(info.data_zip);
                    auto directory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
                    auto full_path = FileUtils::getInstance()->getWritablePath() + "mjstory/" + filename;
                    
                    std::string successEvent = "";

                    for (auto url : info.zip_urls) {
                        ms::utils::DownloadUnit unit;
                        unit.url = url;
                        unit.download_id = mj::helper::getFilenameFromURL(unit.url);
                        successEvent = "download_story_success" + unit.download_id;
                        EventCustom e(successEvent);
                        if (Director::getInstance()->getEventDispatcher()->hasEventListener(e.getEventName())) {

                            Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                        }
                    }

                    mj::helper::unzipAsync(full_path, directory, [=](bool success) {
                        if (success) {
                            // Unzip success
                            auto story_info = StoryDataModel::getInstance()->getStoryById(info.sub_id);
                            StoryDataModel::getInstance()->downloadSuccess(story_info.unique_id, story_info.story_id, story_info.lang_id);
                            callback ? callback(true) : (void) nullptr;
                            LessonManager::getInstance().onActivityReady(info);
                        } else {
                            /*
                             * Unzip error
                             * Delete this story's zip file so we can determine if we can play this story.
                             */
                            _countRetryDownloadActError++;
                            if(_countRetryDownloadActError < 2)
                            {
                                FileUtils::getInstance()->removeFile(full_path);
                                ms::NewDownload::getInstance().clearListDownLoadStory();
                                prepareActivity(info, callback);
                                return;
                            }else{
                                _countRetryDownloadActError = 0;
                            }
                            ms::LessonManager::getInstance().deleteDownloadedActivity(info);
                            
                            
                            
                            
                            //LessonManager::getInstance().onActivityError(info,callback);
                            CCLOG("unzip failed. activity ID = %d", info.activity_id);
                        }
                    });
                } else {
                    callback ? callback(true) : (void) nullptr;
                    LessonManager::getInstance().onActivityReady(info);
                }
            }

            auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
            CCLOG("Download success: %s", unit->storage_path.c_str());
        });
        
        CCLOG("add listener %s", error_event.c_str());
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(error_event, [this, info, callback, error_event, success_event, progress_event](EventCustom * e) {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
            //callback ? callback(false) : (void) nullptr;
            auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
            if(unit)
            {
                _valueLinkError = { info.activity_id, unit->url };

                auto count = std::count(_listUrlError.begin(), _listUrlError.end(), unit->download_id);

                _countClickRetry = count++;

                if (_maxRetryCount <= _countClickRetry)
                {
                    MJDEFAULT->setBoolForKey(key_show_pop_lost_connection, true);
                }

                _listUrlError.push_back(unit->download_id);

                LessonManager::getInstance().onActivityError(info, callback);
                CCLOG("Download error: %s", unit->url.c_str());

                auto u = (ms::utils::DownloadUnit *) e->getUserData();
            }
        });
        
        auto acts = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        if (acts.empty()) {
            return;
        }
        auto id = getActivityBeginDownload(acts, isPhonicLesson(_infoLesson), _infoLesson.lesson_id, _infoLesson.category_id);
        if (id.activity_id == info.activity_id) {
            Director::getInstance()->getEventDispatcher()->addCustomEventListener(progress_event, [info, callback](EventCustom * e) {
                auto progress = reinterpret_cast<double *>(e->getUserData());
                LessonManager::getInstance().onActivityProgress(info, * progress);
            });
        }
        if(ms::NewDownload::getInstance().isNewDownload())
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ms::NewDownload::getInstance().beginDownload();
            });
        }

    }

    bool LessonManager::isActivityAvail(mj::model::ActInfo info) {
        /*
        * Check if an Activity is ready for learning.
        * Just check if this activity has been downloaded.
        */
        std::string relative_directory = "cached/zip";
        if (info.type == mj::model::ActInfo::Type::STORY) {
            return true;

            if (isPhonicLesson(ms::LessonManager::getLessonById(info.lesson_id))) {
                return true;
            }

            relative_directory = "mjstory/";
        }
        std::string prePath = FileUtils::getInstance()->getWritablePath() + relative_directory;
        for (auto url : info.zip_urls) {
            std::string fileName = mj::helper::getFilenameFromURL(url);
            std::string fullPath = prePath + "/" + fileName;
            if (!FileUtils::getInstance()->isFileExist(fullPath)) {
                return false;
            }
        }

        return true;
    }

    void LessonManager::deleteDownloadedActivity(mj::model::ActInfo info) {
        /*
        * Delete zip file of this activity (story only)
        * This function can be update for deleting activities too.
        */
        if (info.type == mj::model::ActInfo::STORY) {
            StoryDataModel::getInstance()->deleteDownloadStory(info.sub_id);
            auto relative_directory = "mjstory/";
            for (auto url : info.zip_urls) {
                auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);

                if (FileUtils::getInstance()->isFileExist(path)) {
                    FileUtils::getInstance()->removeFile(path);
                }
            }
        }else{
            auto relative_directory = "cached/zip/";
            for (auto url : info.zip_urls) {
                auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);

                if (FileUtils::getInstance()->isFileExist(path)) {
                    FileUtils::getInstance()->removeFile(path);
                }
            }
        }
    }

    bool LessonManager::isActivityReady(mj::model::ActInfo info) {
        
        MS_LOGGER.logEvent("isActivityReady",{
            {"type",cocos2d::Value("info.type == mj::model::ActInfo::STORY")},
            {"name",cocos2d::Value(info.name)},
            {"activity_id",cocos2d::Value(StringUtils::format("%d",info.activity_id))}

        });

        if (info.type == mj::model::ActInfo::STORY) {
            if (StoryDataModel::getInstance()->getStoryIsDownloadedWith(info.sub_id, 0, 0)) {
                return true;
            }
        }

        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActInfo::Type::STORY) {
            relative_directory = "mjstory/";
        }
        
        for (auto url : info.zip_urls) {
            auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);
            
            if (!FileUtils::getInstance()->isFileExist(path)) {
                return false;
            }
        }
        
        return true;

    }
    
    int LessonManager::getActivityIndexOfLesson(mj::model::ActInfo info) {
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        auto it = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActInfo a) {
            return a.activity_id == info.activity_id;
        });
        
        if (it != std::end(activities)) {
            return std::distance(std::begin(activities), it);
        }
        
        return -1;
    }
    
    bool LessonManager::isLessonReady(mj::model::LessonInfo info) {

       //_idLessonAndListActivities.f

        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        
        auto id = getActivityBeginDownload(activities, isPhonicLesson(info), info.lesson_id, info.category_id);
        
        if (!activities.empty()) {
            if (isActivityReady(id)) {
                return true;
            }
        }
        
        return false;
    }

    void LessonManager::onDownLoadActivity(mj::model::LessonInfo info)
    {

    }
    
    void LessonManager::onLessonDone(mj::model::LessonInfo info) {
        CCLOG("Lesson Done: %s", info.name.c_str());
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonDone(info));
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonReady(info));
        if (CONFIG_MANAGER->isLearnAllLessons()) {
            return;
        }
    }
    
    void LessonManager::onLessonError(mj::model::LessonInfo info) {
        mj::helper::deleteWaitView();
        CCLOG("Lesson Error: %s", info.name.c_str());
        if (_prepare_attemp < 2) {
            
            ms::ServerManager::getInstance().moveToNextServer();
//            _prepare_attemp++;
            prepareLesson(info, true);
        } else {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonError(info));
        }
    }
    
    void LessonManager::onLessonReady(mj::model::LessonInfo info) {
        mj::helper::deleteWaitView();
        CCLOG("Lesson Ready: %s", info.name.c_str());
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonReady(info));
    }
    
    void LessonManager::onActivityError(mj::model::ActInfo info,std::function<void(bool)> callback) {
        CCLOG("Activity Error: %s", info.name.c_str());
//        if (_prepare_attemp < 5) {

        if (_maxRetryCount > _countClickRetry)
        {
            _index = 0;
            prepareLesson(_infoLesson);
        }
        else {
            callback ? callback(false) : (void) nullptr;
        }
    }
    
    void LessonManager::onActivityProgress(mj::model::ActInfo info, double progress) {
        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActInfo::Type::STORY) {
            relative_directory = "mjstory/";
        }
        
        auto counter = 0;
        
        for (auto url : info.zip_urls) {
            auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);
            
            if (FileUtils::getInstance()->isFileExist(path)) {
                counter++;
            }
        }
        
        progress = counter * 100.f / info.zip_urls.size() + progress * (1.f / info.zip_urls.size());
        
        EventCustom e(getKeyEventActivityProgress(info));
        e.setUserData(&progress);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    }
    
    void LessonManager::onActivityReady(mj::model::ActInfo info) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventActivityReady(info));
        
        auto nextActivity = getNextActivity(info);
        if (nextActivity.activity_id != -1) {
            prepareActivity(getNextActivity(info), nullptr, false);
        } else {
            auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::LessonInfo l) {
                return l.lesson_id == info.lesson_id;
            });
            onLessonDone(* it);
        }
    }
    
    std::string LessonManager::getKeyEventLessonDone(mj::model::LessonInfo info) {
        return StringUtils::format("event_lesson_done_%d", info.lesson_id);
    }
    
    std::string LessonManager::getKeyEventLessonError(mj::model::LessonInfo info) {
        return StringUtils::format("event_lesson_error_%d", info.lesson_id);
    }
    
    std::string LessonManager::getKeyEventLessonReady(mj::model::LessonInfo info) {
        return StringUtils::format("event_lesson_ready_%d", info.lesson_id);
    }
    
    std::string LessonManager::getKeyEventActivityError(mj::model::ActInfo info) {
        return StringUtils::format("event_activity_error_%d", info.activity_id);
    }
    
    std::string LessonManager::getKeyEventActivityReady(mj::model::ActInfo info) {
        return StringUtils::format("event_activity_ready_%d", info.activity_id);
    }
    
    std::string LessonManager::getKeyEventActivityProgress(mj::model::ActInfo info) {
        return StringUtils::format("event_activity_progress_%d", info.activity_id);
    }
    
    std::vector<int> LessonManager::getFreeLessonId(int category_id) {
        auto free_id = getLessonShouldLearn(category_id);
        auto first_lesson = getLessonById(free_id);
        auto second_lesson = getNextLesson(first_lesson);
        auto third_lesson = getNextLesson(second_lesson);
        
        std::vector<int> r;
        r.push_back(first_lesson.lesson_id);
        if (second_lesson.lesson_id != -1) {
            bool flag = true;
            for (auto elem : r) {
                if (second_lesson.lesson_id == elem) {
                    flag = false;
                    break;
                }
            }
            if (flag) r.push_back(second_lesson.lesson_id);
        }
        if (third_lesson.lesson_id != -1) {
            bool flag = true;
            for (auto elem : r) {
                if (third_lesson.lesson_id == elem) {
                    flag = false;
                    break;
                }
            }
            if (flag) r.push_back(third_lesson.lesson_id);
        }

        return r;
    }

    void LessonManager::setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn)
    {

        std::vector<int> listLesson = {};

        for (auto lesson : listLessonLearn)
        {
            int lessonID = std::stoi(lesson.first);

            bool isSuccess = true;
            //auto lesson = ms::LessonManager::getLessonById(lessonID);
            std::string crashActivity = "";
            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lessonID);
            for (int i = 0; i < activities.size(); i++) {
                if (activities[i].isUnzipError) {
                    crashActivity = std::to_string(activities[i].activity_id) + " ";
                    isSuccess = false;
                }
            }

            auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lessonID](mj::model::LessonInfo l) {
                return l.lesson_id == lessonID;
                });
            if (it == std::end(_all_lessons)) {
                return;
            }
            it->is_learned = true;
            it->state = mj::model::LessonInfo::State::LEARNED;
            listLesson.push_back(lessonID);
        }
        StoryDataModel::getInstance()->setLessonLearned(listLesson);
        updateAllLesson();
    }

    std::vector<mj::model::LessonInfo> LessonManager::getAllLesson()
    {
        return _all_lessons;
    }
    
    void LessonManager::setLessonLearned(int lesson_id) {
        bool isSuccess = true;
        auto lesson = ms::LessonManager::getLessonById(lesson_id);
        std::string crashActivity = "";
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);
        for (int i = 0; i < activities.size(); i++) {
            if (activities[i].isUnzipError) {
                crashActivity = std::to_string(activities[i].activity_id) + " ";
                isSuccess = false;
            }
        }

        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lesson_id](mj::model::LessonInfo l) {
            return l.lesson_id == lesson_id;
        });
        if (it == std::end(_all_lessons)) {
            return;
        }
        it->is_learned = true;
        it->state = mj::model::LessonInfo::State::LEARNED;
        StoryDataModel::getInstance()->setLessonLearned(std::vector<int>({lesson_id}));
        updateAllLesson();

    }
    
    std::vector<mj::model::LessonInfo> LessonManager::getDownloadedLessons() {
        auto downloaded_id = PROFILE_MANAGER.getAllLessonLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));//StoryDataModel::getInstance()->getDownloadedLessons();
        std::vector<mj::model::LessonInfo> lessons;
        for (auto idx : downloaded_id) {
            auto l = getLessonById(idx);
            if (l.state == mj::model::LessonInfo::LOCKED) {
                continue;
            }
            if (getLessonShouldLearn(l.category_id) == l.lesson_id) {
                continue;
            }
            lessons.push_back(getLessonById(idx));
        }
        
        return lessons;
    }
    
    bool LessonManager::isPhonicLesson(mj::model::LessonInfo lesson){
        std::vector<int> const phonic_grade_id = {
            469, 479, 480, 481
        };
        for (auto i : phonic_grade_id) {
            if (lesson.category_id == i) {
                return true;
            }
        }
        return false;
    }
    
    bool LessonManager::isPhonicCategory(int cat_id){
        // danh sách này copy ở hàm isPhonicLesson(...)
        std::vector<int> const phonic_grade_id = {
            469, 479, 480, 481
        };
        
        for (auto vl : phonic_grade_id) {
            if (vl == cat_id) {
                return true;
            }
        }
        
        return false;
    }
    mj::model::LessonInfo LessonManager::getCurrentLesson() {
        return _currentLesson;
    }
    void LessonManager::setCurrentLesson(mj::model::LessonInfo lesson) {
        _currentLesson = lesson;
    }
    
    void LessonManager::setCurrentLessonFreeForCategory(int category)
    {
        _catergoryForLessonFree = category;
    }

    std::vector<int> LessonManager::getListLessonFree()
    {
        if (_listLessonFree.empty() && MJDEFAULT->getIntegerForKey(key_free_content_page_option) == 3)
        {
            ms::f2p::FreeContentOption freeContentOption1 = (ms::f2p::FreeContentOption)MJDEFAULT->getIntegerForKey(key_free_content_page_option, 4);
            std::vector<mj::model::FreeItemInfo> freeItemInfos = StoryDataModel::getInstance()->getFreeTrialInfosByOption(freeContentOption1);
            for (auto items : freeItemInfos)
            {
                _listLessonFree.push_back(items._id);
            }
        }
        return _listLessonFree;
    }

    void LessonManager::clearDataLessonFree()
    {
        _listLessonFree.clear();
    }

    void LessonManager::setListLessonFree(std::vector<int> listLesson)
    {
        _listLessonFree = listLesson;
    }

    int LessonManager::getCurrentLessonForCategory()
    {
        return (_catergoryForLessonFree == - 1) ? 84 : _catergoryForLessonFree;
    }

    void LessonManager::resetIndexDownload()
    {
        auto acts = StoryDataModel::getInstance()->getActivitiesOfLesson(_infoLesson.lesson_id);

        for (auto act : acts)
        {
            for (auto url : act.zip_urls)
            {
               auto pathCache = FileUtils::getInstance()->getWritablePath() + "cached/zip/" + mj::helper::getFilenameFromURL(url) + ".tmp";

               if (FileUtils::getInstance()->isFileExist(pathCache))
               {
                   FileUtils::getInstance()->removeFile(pathCache);
               }
            }

            
        }

        _index = 0;
        _valueLinkError = { -1, "" };
        _countClickRetry = -1;
        _listUrlError.clear();
    }

    void LessonManager::resetListUrlError()
    {
        if (_maxRetryCount == -1)
        {
            _maxRetryCount = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME).size();
        }

        MJDEFAULT->deleteValueForKey(key_error_url_stories);
        _valueLinkError = { -1, "" };
        _index == 0;
        _countClickRetry = -1;
        _listUrlError.clear();
    }

    void LessonManager::setInforCurrenLesson(mj::model::LessonInfo info)
    {
        _infoLesson = info;
    }

    void LessonManager::setCountClickRetry(int indexClick)
    {
        if (_countClickRetry > indexClick)
        {
            return;
        }
        _countClickRetry = indexClick;
    }


}
