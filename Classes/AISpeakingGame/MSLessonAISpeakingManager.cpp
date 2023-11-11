///////////////////////////////////////////////////////////
//  MSLessonAISpeakingManager.cpp
//  Implementation of the Class MSLessonAISpeakingManager.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MSLessonAISpeakingManager.h"
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
#include "MJAPI_Download_MultiFile.h"
#include "UnzipErrorPopup.h"

namespace ms {
    void LessonAISpeakingManager::onSavelessonLearnAfterLearned(mj::model::AISpeakingLessonInfo lessonInfo)
    {
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lessonInfo.lesson_id);
        if (activities.size() > 0)
        {
            ms::LessonAISpeakingManager::getInstance().setLessonLearned(activities.at(0).lesson_id);
            ms::LessonAISpeakingManager::getInstance().updateLastActivityLearned(activities.at(0));
        }

    }
    LessonAISpeakingManager::LessonAISpeakingManager() {
        updateLessonsState();
    }

    void LessonAISpeakingManager::updateAllLesson(){
        updateLessonsState();
    }
    
    void LessonAISpeakingManager::updateLessonsState() {
         _all_lessons = StoryDataModel::getInstance()->getAllLessonsAISpeakingByLanguage(StoryLanguageManager::getCurrentLangId());
        
        auto allAISpeakingTopic = StoryDataModel::getInstance()->getAllAiSpeakingTopic();
        
        if (CONFIG_MANAGER->isLearnAllLessons()) {
            for (auto &lesson : _all_lessons) {
                lesson.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
            }
            return;
        }
        
        std::vector<mj::model::AISpeakingLessonInfo> last_lessons;
        for (auto aiSpekingTopicInfo : allAISpeakingTopic) {
            auto idx = getLastLessonIdOfCategory(aiSpekingTopicInfo.topic_id);
            auto last = getLessonById(idx);
            if (last.is_learned) {
                last = getNextLesson(last);
            }
            last_lessons.push_back(last);
            
            if (idx == -1) {
                for (auto &l : _all_lessons) {
                    if (l.category_id == aiSpekingTopicInfo.topic_id) {
                        l.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
                        break;
                    }
                }
            }
        }
        
        for (auto &lesson : _all_lessons) {
            // nếu là Lessons:Comprehension của MS thì unlock luôn
                if (lesson.state == mj::model::AISpeakingLessonInfo::State::LOCKED) {
                    lesson.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
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
                        lesson.state = mj::model::AISpeakingLessonInfo::State::LEARNED;
                    }
                }
                
                continue;
            
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
			// FIXME: unlocked all lessons phonic. REMOVEME next release
   /*         if(lesson.category_id == 479 || lesson.category_id == 480 || lesson.category_id == 481 || lesson.category_id == 469) {
                lesson.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
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
                        lesson.state = mj::model::AISpeakingLessonInfo::State::LEARNED;
                    else
                        lesson.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
                }
                
                if (c == 2) {
                    lesson.state = mj::model::AISpeakingLessonInfo::State::LOCKED;
                }
                
                if (c == 3) {
                    lesson.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
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
                        l.state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
                    }
                    if (l.lesson_id == lesson.lesson_id) {
                        l.state = mj::model::AISpeakingLessonInfo::State::LEARNED;
                    }
                }
            }
            
        }
    }
    
    std::vector<mj::model::AISpeakingLessonInfo> LessonAISpeakingManager::getAllLessonsByGrade(int grade_id) {
        std::vector<mj::model::AISpeakingLessonInfo> t;
        std::copy_if(_all_lessons.begin(), _all_lessons.end(), std::back_inserter(t), [grade_id](mj::model::AISpeakingLessonInfo l) {
            return l.category_id == grade_id;
        });
        
        return t;
    }


    int LessonAISpeakingManager::compareLesson(mj::model::AISpeakingLessonInfo a, mj::model::AISpeakingLessonInfo b) {
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
    
    int LessonAISpeakingManager::getDistanceBetweenLessons(mj::model::AISpeakingLessonInfo a, mj::model::AISpeakingLessonInfo b) {
        if (a.category_id != b.category_id) {
            return INT32_MAX;
        }
        
        auto all = getAllLessonsByGrade(a.category_id);
        auto a_it = std::find_if(std::begin(all), std::end(all), [a](mj::model::AISpeakingLessonInfo l) {
            return l.lesson_id == a.lesson_id;
        });
        
        auto b_it = std::find_if(std::begin(all), std::end(all), [b](mj::model::AISpeakingLessonInfo l) {
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
        
//        if (a.lesson_order < b.lesson_order) {
//            return 1;
//        }
//
//        if (a.lesson_order > b.lesson_order) {
//            return 2;
//        }
//
//        return 3;
    }

    void LessonAISpeakingManager::onUpdateLessonAISpeakingStateToDB(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo)
    {
        StoryDataModel::getInstance()->UpdateLessonStateAISpeaking(aISpeakingLessonInfo);
    }
    
    mj::model::ActAISpeakingInfo LessonAISpeakingManager::getActivityById(int activity_id) {
        return StoryDataModel::getInstance()->getActivityById(activity_id);
    }
    
    mj::model::ActAISpeakingInfo LessonAISpeakingManager::getNextActivity(mj::model::ActAISpeakingInfo info) {
        mj::model::ActAISpeakingInfo r;
        r.activity_id = -1;
        r.lesson_id = -1;
        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::AISpeakingLessonInfo l) {
            return l.lesson_id == info.lesson_id;
        });
        
        if (it == std::end(_all_lessons)) {
            return r;
        }
        
        auto lesson_info = * it;
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_info.lesson_id);
        auto a_it = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActAISpeakingInfo a) {
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
    
    mj::model::AISpeakingLessonInfo LessonAISpeakingManager::getLessonById(int lesson_id) {
        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lesson_id](mj::model::AISpeakingLessonInfo l) {
            return l.lesson_id == lesson_id;
        });
        
        mj::model::AISpeakingLessonInfo r;
        r.lesson_id = -1;
        
        if (it != std::end(_all_lessons)) {
            r = * it;
        }
        
        return r;
    }
    
    mj::model::AISpeakingLessonInfo LessonAISpeakingManager::getNextLesson(mj::model::AISpeakingLessonInfo info) {
        if (info.lesson_id == -1) {
            return info;
        }
        
        std::vector<mj::model::AISpeakingLessonInfo> t;
        std::copy_if(_all_lessons.begin(), _all_lessons.end(), std::back_inserter(t), [info](mj::model::AISpeakingLessonInfo l) {
            return l.category_id == info.category_id;
        });

		if (info.lesson_id == t.rbegin()->lesson_id) {
			return info;
		}
        
        mj::model::AISpeakingLessonInfo r;
        r.lesson_id = -1;
        auto it = std::find_if(std::begin(t), std::end(t), [info](mj::model::AISpeakingLessonInfo l) {
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
    
    void LessonAISpeakingManager::updateLastActivityLearned(mj::model::ActAISpeakingInfo info) {
        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::AISpeakingLessonInfo i) {
            return i.lesson_id == info.lesson_id;
        });

        auto lesson_info = * it;
        auto lesson_index = std::distance(std::begin(_all_lessons), it);
        
        bool is_4notPhonics = !CONFIG_MANAGER->isVietnameseStories() && !isPhonicLesson(lesson_info); // MS và không phải phonics
        
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_info.lesson_id);
        auto it2 = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActAISpeakingInfo i) {
            return i.activity_id == info.activity_id;
        });
        
        auto activity_index = std::distance(std::begin(activities), it2);
        
        auto current_lesson_id = getLastLessonIdOfCategory(lesson_info.category_id);// MJDEFAULT->getIntegerForKey("key_last_lesson", -1);
        it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [current_lesson_id](mj::model::AISpeakingLessonInfo i) {
            return i.lesson_id == current_lesson_id;
        });
        auto current_lesson_index = (it == std::end(_all_lessons) ? -1 : std::distance(std::begin(_all_lessons), it));
        
        auto current_activity_id = is_4notPhonics ? getLastActivityIdOfLesson(lesson_info.lesson_id) : getLastActivityIdOfCategory(lesson_info.category_id);// MJDEFAULT->getIntegerForKey("key_last_activity", -1);
        it2 = std::find_if(std::begin(activities), std::end(activities), [current_activity_id](mj::model::ActAISpeakingInfo i) {
            return i.activity_id == current_activity_id;
        });
        auto current_activity_index = (it2 == std::end(activities) ? -1 : std::distance(std::begin(activities), it2));
        
        if (is_4notPhonics || lesson_index >= current_lesson_index) {
            //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_lesson_x, lesson_info.category_id), lesson_info.lesson_id);
            
            std::string key = StringUtils::format(key_last_lesson_x, lesson_info.category_id);
            PROFILE_MANAGER.setIntegerForKey(key, lesson_info.lesson_id);
            if (activity_index >= current_activity_index) {
                //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_activity_x, lesson_info.category_id), info.activity_id);
                if (is_4notPhonics) {
                    int profileID = ap::ProfileManager::getInstance().getCurrentProfileId();
                    std::string keyActivityLearn = StringUtils::format(key_last_activity_x_y, profileID, lesson_info.lesson_id);
                    MJDEFAULT->setIntegerForKey(keyActivityLearn, info.activity_id);
                    mm::RealtimeDatabase::getInstance().updateActivityLearnOfLessonInReadingComprehension(profileID, keyActivityLearn, info.activity_id);
                } else {
                    PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_activity_x, lesson_info.category_id), info.activity_id);
                }
            }

			auto user_info = ap::Database::getInstance().getUserInfo();

            auto valueLessonLearn = StoryDataModel::getInstance()->getLessonLearnAISpeakingByID(lesson_info.lesson_id);

            if (valueLessonLearn.size() > 0)
            {
                PROFILE_MANAGER.insertValueToTableLessonAISpeakingLearn(valueLessonLearn);
                mm::RealtimeDatabase::getInstance().updateTrackingAndLearnedLessonsAISpeaking(lesson_info, info, activity_index >= current_activity_index, lesson_info.lesson_id);
            }
			MJDEFAULT->setBoolForKey("key_remove_phonics_tracking", false);

            
            updateLessonsState();
        }
        
        // call tracking activity
        //mj::TrackingAPI::call(info);
        
        //remove hand guide of previous lessons
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(key_event_play_locked_lesson);
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_reload_lesson_state);
    }
    
    int LessonAISpeakingManager::getActivityShouldLearn_4notPhonics(int cat_id, int lesson_id) {
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
    
    int LessonAISpeakingManager::getActivityShouldLearn(int cat_id) {
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
    
    int LessonAISpeakingManager::getLessonShouldLearn(int cat_id) {
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
    
    int LessonAISpeakingManager::getLastLessonIdOfCategory(int category_id) {
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
                if ((l.is_learned || l.state == mj::model::AISpeakingLessonInfo::UNLOCKED) && l.lesson_order > order_zero) {
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
    
    int LessonAISpeakingManager::getLastActivityIdOfCategory(int category_id) {
        //return MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_activity_x, category_id), -1);
        return PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_activity_x, category_id), -1);
    }
    
    int LessonAISpeakingManager::getLastActivityIdOfLesson(int lesson_id) {
        return MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_activity_x_y, ap::ProfileManager::getInstance().getCurrentProfileId(), lesson_id), -1);
    }
    
    void LessonAISpeakingManager::prepareLesson(mj::model::AISpeakingLessonInfo info, bool retry) {
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
        _index++;
        auto current_lesson = getLessonById(getLessonShouldLearn(info.category_id));
        auto nextLesson = getNextLesson(current_lesson);
        auto distance = getDistanceBetweenLessons(current_lesson, info);

        if (_index == 3)
        {
            _index = 0;
            return;
        }

        if (CONFIG_MANAGER->isLearnAllLessons()) {
            
        } else if (!CONFIG_MANAGER->isVietnameseStories() && !ms::LessonAISpeakingManager::getInstance().isPhonicLesson(info)) { 

        } else if (distance > 5 && distance != INT32_MAX) {
            // ????
            CCLOG("STOP CACHING");
            Director::getInstance()->getScheduler()->schedule([info](float dt) {
                LessonAISpeakingManager::getInstance().onLessonError(info);
            }, Application::getInstance(), 0, 0, 0.1, false, "delay_dispatch_lesson_error");
            return;
        }
        
        auto acts = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        if (!acts.empty()) {
            /*
            * First time select a lesson, client does not have any information about this lesson: acts's size = 0
            * So if the amount of acvitities of a lesson greater than 0 means that lesson has been downloaded before,
            *  and client don't have to download again.
            */
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(StringUtils::format("%s_%d", key_event_got_lesson_details, info.lesson_id));
            LessonAISpeakingManager::getInstance().prepareActivity(acts[0], [this, info](bool activity_success) {
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
                    LessonAISpeakingManager::getInstance().onLessonReady(info);
                } else {
                    LessonAISpeakingManager::getInstance().onLessonError(info);
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
                auto lesson = ms::LessonAISpeakingManager::getInstance().getLessonById(lessonID);
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
						LessonAISpeakingManager::getInstance().onLessonError(info);
						return;
					}
					LessonAISpeakingManager::getInstance().prepareActivity(activities[0], [this, info](bool activity_success) {
                        /*
                        * Prepare the first activity.
                        */
						if (activity_success) {
							LessonAISpeakingManager::getInstance().onLessonReady(info);
						}
						else {
							LessonAISpeakingManager::getInstance().onLessonError(info);
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

    
    void LessonAISpeakingManager::prepareActivity(mj::model::ActAISpeakingInfo info, std::function<void(bool)> callback, bool is_retry) {
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
        
        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActAISpeakingInfo::Type::STORY) {
            relative_directory = "mjstory/";
        }
        
        for (auto url : info.zip_urls) {
            ms::utils::DownloadUnit unit;
            unit.url = url;
            unit.download_id = mj::helper::getFilenameFromURL(unit.url);
            unit.storage_path = FileUtils::getInstance()->getWritablePath() + relative_directory + unit.download_id;
            unit.success_event = success_event;
            unit.failed_event = error_event;
            unit.progress_event = progress_event;
            
            if (is_retry) {
                if (!MJDEFAULT->getStringForKey(key_alternative_domain_lesson).empty())
                    unit.url = mj::helper::replaceDomain(unit.url, MJDEFAULT->getStringForKey(key_alternative_domain_lesson));
            }
            
            CCLOG("activity: %s", info.name.c_str());
            CCLOG("activity url: %s", url.c_str());
            
            if (!FileUtils::getInstance()->isFileExist(unit.storage_path)) {
                ms::utils::Downloader::getInstance().enqueue(unit);
            }
        }
        
        CCLOG("remove listener %s", success_event.c_str());
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
        
        CCLOG("add listener %s", success_event.c_str());
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(success_event, [callback, info, success_event, error_event, progress_event](EventCustom * e) {
            if (LessonAISpeakingManager::isActivityReady(info)) {
                
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
                
                if (info.type == mj::model::ActAISpeakingInfo::Type::STORY) {
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

                    mj::helper::unzipAsync(full_path, directory, [info, callback, filename, full_path](bool success) {
                        if (success) {
                            // Unzip success
                            auto story_info = StoryDataModel::getInstance()->getStoryById(info.sub_id);
                            StoryDataModel::getInstance()->downloadSuccess(story_info.unique_id, story_info.story_id, story_info.lang_id);
                            callback ? callback(true) : (void) nullptr;
                            LessonAISpeakingManager::getInstance().onActivityReady(info);
                        } else {
                            /*
                             * Unzip error
                             * Delete this story's zip file so we can determine if we can play this story.
                             */
                            ms::LessonAISpeakingManager::getInstance().deleteDownloadedActivity(info);
                            //LessonAISpeakingManager::getInstance().onActivityError(info,callback);
                            CCLOG("unzip failed. activity ID = %d", info.activity_id);
                        }
                    });
                } else {
                    callback ? callback(true) : (void) nullptr;
                    LessonAISpeakingManager::getInstance().onActivityReady(info);
                }
            }

            auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
            CCLOG("Download success: %s", unit->storage_path.c_str());
        });
        
        CCLOG("add listener %s", error_event.c_str());
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(error_event, [info, callback, error_event, success_event, progress_event](EventCustom * e) {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
            //callback ? callback(false) : (void) nullptr;
            LessonAISpeakingManager::getInstance().onActivityError(info,callback);
            auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
            CCLOG("Download error: %s", unit->url.c_str());
            
            auto u = (ms::utils::DownloadUnit *) e->getUserData();
        });
        
        auto acts = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        if (acts.empty()) {
            return;
        }
        if (acts[0].activity_id == info.activity_id) {
            Director::getInstance()->getEventDispatcher()->addCustomEventListener(progress_event, [info, callback](EventCustom * e) {
                auto progress = reinterpret_cast<double *>(e->getUserData());
                LessonAISpeakingManager::getInstance().onActivityProgress(info, * progress);
            });
        }
    }

    bool LessonAISpeakingManager::isActivityAvail(mj::model::ActAISpeakingInfo info) {
        /*
        * Check if an Activity is ready for learning.
        * Just check if this activity has been downloaded.
        */
        std::string relative_directory = "cached/zip";
        if (info.type == mj::model::ActAISpeakingInfo::Type::STORY) {

            if (isPhonicLesson(ms::LessonAISpeakingManager::getLessonById(info.lesson_id))) {
                return true;
            }

            relative_directory = "mjstory/";
        }
        std:string prePath = FileUtils::getInstance()->getWritablePath() + relative_directory;
        for (auto url : info.zip_urls) {
            auto fileName = mj::helper::getFilenameFromURL(url);
            auto fullPath = prePath + "/" + fileName;
            if (!FileUtils::getInstance()->isFileExist(fullPath)) {
                return false;
            }
        }

        return true;
    }

    void LessonAISpeakingManager::deleteDownloadedActivity(mj::model::ActAISpeakingInfo info) {
        /*
        * Delete zip file of this activity (story only)
        * This function can be update for deleting activities too.
        */
        if (info.type == mj::model::ActAISpeakingInfo::STORY) {
            StoryDataModel::getInstance()->deleteDownloadStory(info.sub_id);
            auto relative_directory = "mjstory/";
            for (auto url : info.zip_urls) {
                auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);

                if (FileUtils::getInstance()->isFileExist(path)) {
                    FileUtils::getInstance()->removeFile(path);
                }
            }
        }
    }

    bool LessonAISpeakingManager::isActivityReady(mj::model::ActAISpeakingInfo info) {
        
        MS_LOGGER.logEvent("isActivityReady",{
            {"type",cocos2d::Value("info.type == mj::model::ActAISpeakingInfo::STORY")},
            {"name",cocos2d::Value(info.name)},
            {"activity_id",cocos2d::Value(StringUtils::format("%d",info.activity_id))}

        });

        if (info.type == mj::model::ActAISpeakingInfo::STORY) {
            if (StoryDataModel::getInstance()->getStoryIsDownloadedWith(info.sub_id, 0, 0)) {
                return true;
            }
        }

        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActAISpeakingInfo::Type::STORY) {
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
    
    int LessonAISpeakingManager::getActivityIndexOfLesson(mj::model::ActAISpeakingInfo info) {
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        auto it = std::find_if(std::begin(activities), std::end(activities), [info](mj::model::ActAISpeakingInfo a) {
            return a.activity_id == info.activity_id;
        });
        
        if (it != std::end(activities)) {
            return std::distance(std::begin(activities), it);
        }
        
        return -1;
    }
    
    bool LessonAISpeakingManager::isLessonReady(mj::model::AISpeakingLessonInfo info) {
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
        if (!activities.empty()) {
            if (isActivityReady(activities[0])) {
                return true;
            }
            
            //đã có thông tin về các activity
//            auto lesson_done = std::accumulate(std::begin(activities), std::end(activities), true, [](bool r, mj::model::ActAISpeakingInfo activity) {
//                return r && isActivityReady(activity);
//            });
//            if (lesson_done) {
//                return true;
//            } else {
//                return true;
//            }
        }
        
        return false;
    }
    
    void LessonAISpeakingManager::onLessonDone(mj::model::AISpeakingLessonInfo info) {
        CCLOG("Lesson Done: %s", info.name.c_str());
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonDone(info));
        //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonReady(info));
        if (CONFIG_MANAGER->isLearnAllLessons()) {
            return;
        }
//        prepare next lesson
        auto nextLesson = getNextLesson(info);
        if (nextLesson.lesson_id != -1) {
            auto current_lesson = info;
            if (_index == 3) {
                _index = 0;
                CCLOG("STOP CACHING");
//                Director::getInstance()->getScheduler()->schedule([info](float dt) {
//                    LessonAISpeakingManager::getInstance().onLessonError(info);
//                }, Application::getInstance(), 0, 0, 0.1, false, "delay_dispatch_lesson_error");
                return;
            }
            
            if (nextLesson.lesson_id != info.lesson_id)
                prepareLesson(nextLesson, false);
        }
    }
    
    void LessonAISpeakingManager::onLessonError(mj::model::AISpeakingLessonInfo info) {
        CCLOG("Lesson Error: %s", info.name.c_str());
        if (_prepare_attemp < 2) {
            
            ms::ServerManager::getInstance().moveToNextServer();
//            _prepare_attemp++;
            prepareLesson(info, true);
        } else {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonError(info));
            _prepare_attemp = 0;
        }
    }

    bool ms::LessonAISpeakingManager::onDeleteDataExperted(mj::model::AISpeakingLessonInfo info)
    {
        _resultDeleteDataExpected = true;
        if (info.is_learned != true)
            return true;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
            if (activities.size() <= 0)
                return true;
            for (auto activity : activities) {
                for (std::string zipPath : activity.zip_urls)
                {
                    zipPath = FileUtils::getInstance()->getWritablePath() + CACHE_ZIP + mj::helper::getFilenameFromURL(zipPath);
                    if (FileUtils::getInstance()->isFileExist(zipPath))
                    {
                        bool resultRemoveOneFile = FileUtils::getInstance()->removeFile(zipPath);
                        _resultDeleteDataExpected = _resultDeleteDataExpected && resultRemoveOneFile;
                    }
                }
            }
            });

        return _resultDeleteDataExpected;
    }
    
    void LessonAISpeakingManager::onLessonReady(mj::model::AISpeakingLessonInfo info) {
        CCLOG("Lesson Ready: %s", info.name.c_str());
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventLessonReady(info));
    }
    
    void LessonAISpeakingManager::onActivityError(mj::model::ActAISpeakingInfo info,std::function<void(bool)> callback) {
        CCLOG("Activity Error: %s", info.name.c_str());
//        if (_prepare_attemp < 5) {
            if (!mj::helper::isContain(_activity_retried, info.activity_id)) {
                _activity_retried.push_back(info.activity_id);
                prepareActivity(info, callback, true);
//            }
//            auto lesson = getLessonById(info.lesson_id);
//            ms::ServerManager::getInstance().moveToNextServer();
//            _prepare_attemp++;
//            prepareLesson(lesson, true);
        } else {
            _activity_retried.erase(std::remove(_activity_retried.begin(), _activity_retried.end(), info.activity_id), _activity_retried.end());
            callback ? callback(false) : (void) nullptr;
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventActivityError(info));
//            _prepare_attemp = 0;
        }
    }
    
    void LessonAISpeakingManager::onActivityProgress(mj::model::ActAISpeakingInfo info, double progress) {
        auto relative_directory = std::string("cached/zip/");
        if (info.type == mj::model::ActAISpeakingInfo::Type::STORY) {
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
    
    void LessonAISpeakingManager::onActivityReady(mj::model::ActAISpeakingInfo info) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getKeyEventActivityReady(info));
        
        auto nextActivity = getNextActivity(info);
        if (nextActivity.activity_id != -1) {
            prepareActivity(getNextActivity(info));
        } else {
            auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [info](mj::model::AISpeakingLessonInfo l) {
                return l.lesson_id == info.lesson_id;
            });
            onLessonDone(* it);
        }
    }
    
    std::string LessonAISpeakingManager::getKeyEventLessonDone(mj::model::AISpeakingLessonInfo info) {
        return StringUtils::format("event_lesson_done_%d", info.lesson_id);
    }
    
    std::string LessonAISpeakingManager::getKeyEventLessonError(mj::model::AISpeakingLessonInfo info) {
        return StringUtils::format("event_lesson_error_%d", info.lesson_id);
    }
    
    std::string LessonAISpeakingManager::getKeyEventLessonReady(mj::model::AISpeakingLessonInfo info) {
        return StringUtils::format("event_lesson_ready_%d", info.lesson_id);
    }
    
    std::string LessonAISpeakingManager::getKeyEventActivityError(mj::model::ActAISpeakingInfo info) {
        return StringUtils::format("event_activity_error_%d", info.activity_id);
    }
    
    std::string LessonAISpeakingManager::getKeyEventActivityReady(mj::model::ActAISpeakingInfo info) {
        return StringUtils::format("event_activity_ready_%d", info.activity_id);
    }
    
    std::string LessonAISpeakingManager::getKeyEventActivityProgress(mj::model::ActAISpeakingInfo info) {
        return StringUtils::format("event_activity_progress_%d", info.activity_id);
    }
    std::string LessonAISpeakingManager::getKeyEventCommonProgress(mj::model::ActAISpeakingInfo info) {
        return StringUtils::format("event_common_progress_%d", info.activity_id);
    }
    std::vector<int> LessonAISpeakingManager::getFreeLessonId(int category_id) {
        auto free_id = getLessonShouldLearn(category_id);
//        auto index = random(0, (int)_all_lessons.size() - 1);
//        auto first_lesson = _all_lessons[index];
//        auto second_lesson = getNextLesson(first_lesson);
//        auto third_lesson = getNextLesson(second_lesson);
        
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

    void LessonAISpeakingManager::setLessonLearned(std::vector<std::pair<std::string, int>> listLessonLearn)
    {

        std::vector<int> listLesson = {};

        for (auto lesson : listLessonLearn)
        {
            int lessonID = std::stoi(lesson.first);

            bool isSuccess = true;
            //auto lesson = ms::LessonAISpeakingManager::getLessonById(lessonID);
            std::string crashActivity = "";
            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lessonID);
            for (int i = 0; i < activities.size(); i++) {
                if (activities[i].isUnzipError) {
                    crashActivity = std::to_string(activities[i].activity_id) + " ";
                    isSuccess = false;
                }
            }

            auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lessonID](mj::model::AISpeakingLessonInfo l) {
                return l.lesson_id == lessonID;
                });
            if (it == std::end(_all_lessons)) {
                return;
            }
            it->is_learned = true;
            it->state = mj::model::AISpeakingLessonInfo::State::LEARNED;
            listLesson.push_back(lessonID);
        }
        StoryDataModel::getInstance()->setLessonAISpeakingLearned(listLesson);
        
    }

    std::vector<mj::model::AISpeakingLessonInfo> LessonAISpeakingManager::getAllLesson()
    {
        if (_all_lessons.size() <= 0)
        {
            _all_lessons = StoryDataModel::getInstance()->getAllLessonsAISpeakingByLanguage(StoryLanguageManager::getCurrentLangId());
        }
        return _all_lessons;
    }

    mj::model::AISpeakingLessonInfo ms::LessonAISpeakingManager::onSaveLessonAISpeakingState(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo)
    {

        mj::model::AISpeakingLessonInfo aISpeakingLessonInfoBeforeUpdate = getLessonAISpeakingStateFromDBByID(aISpeakingLessonInfo);
        if (aISpeakingLessonInfoBeforeUpdate.state == mj::model::AISpeakingLessonInfo::State::LEARNED)
            return aISpeakingLessonInfoBeforeUpdate;
        onUpdateLessonAISpeakingStateToDB(aISpeakingLessonInfo);
        mj::model::AISpeakingLessonInfo aISpeakingLessonInfoAfterUpdate = getLessonAISpeakingStateFromDBByID(aISpeakingLessonInfo);

        return aISpeakingLessonInfoAfterUpdate;
    }

    std::vector<mj::model::AISpeakingLessonInfo> ms::LessonAISpeakingManager::getAllLessonAISpeakingStateFromDB(std::vector<mj::model::AISpeakingLessonInfo> aISpeakingLessonInfos)
    {
        std::vector<mj::model::AISpeakingLessonInfo> returnAISpeakingLessonInfos;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            // handler get all lesson ai speaking state from DB and firebase

            });
        return aISpeakingLessonInfos;
    }

    mj::model::AISpeakingLessonInfo ms::LessonAISpeakingManager::getLessonAISpeakingStateFromDBByID(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo)
    {
        return StoryDataModel::getInstance()->getLessonAISpeakingByID(aISpeakingLessonInfo.lesson_id);
    }
    
    void LessonAISpeakingManager::setLessonLearned(int lesson_id) {
        bool isSuccess = true;
        auto lesson = ms::LessonAISpeakingManager::getLessonById(lesson_id);
        std::string crashActivity = "";
        auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson_id);
        for (int i = 0; i < activities.size(); i++) {
            if (activities[i].isUnzipError) {
                crashActivity = std::to_string(activities[i].activity_id) + " ";
                isSuccess = false;
            }
        }

        auto it = std::find_if(std::begin(_all_lessons), std::end(_all_lessons), [lesson_id](mj::model::AISpeakingLessonInfo l) {
            return l.lesson_id == lesson_id;
        });
        if (it == std::end(_all_lessons)) {
            return;
        }
        it->is_learned = true;
        it->state = mj::model::AISpeakingLessonInfo::State::LEARNED;
        StoryDataModel::getInstance()->setLessonAISpeakingLearned(std::vector<int>({lesson_id}));
    }
    
    std::vector<mj::model::AISpeakingLessonInfo> LessonAISpeakingManager::getDownloadedLessons() {
        auto downloaded_id = PROFILE_MANAGER.getAllLessonAISpeakingLearned(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));//StoryDataModel::getInstance()->getDownloadedLessons();
        std::vector<mj::model::AISpeakingLessonInfo> lessons;
        for (auto idx : downloaded_id) {
            auto l = getLessonById(idx);
            if (l.state == mj::model::AISpeakingLessonInfo::LOCKED) {
                continue;
            }
            if (getLessonShouldLearn(l.category_id) == l.lesson_id) {
                continue;
            }
            lessons.push_back(getLessonById(idx));
        }
        
        return lessons;
    }
    
    bool LessonAISpeakingManager::isPhonicLesson(mj::model::AISpeakingLessonInfo lesson){
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
    
    bool LessonAISpeakingManager::isPhonicCategory(int cat_id){
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
    mj::model::AISpeakingLessonInfo LessonAISpeakingManager::getCurrentLesson() {
        return _currentLesson;
    }
    void LessonAISpeakingManager::setCurrentLesson(mj::model::AISpeakingLessonInfo lesson) {
        _currentLesson = lesson;
    }
    bool LessonAISpeakingManager::onDownloadVideoCommons(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos, std::function<void(bool)> callback, std::function<void(float)> callbackPercent, CommonDownloadType type)
    {
        bool _isNeedDownload = false;
        if (talkingCommonInfos.size() <= 0)
        {
            if (callback)
            {
                callbackPercent(100.0f);
                callback(true);
            }
            return _isNeedDownload;
        }
        std::vector <mj::api::ThumbDownloadInfo*> thumbDownloadInfos;
        std::vector <std::string> download_url_video_zips;
        if(type == ms::CommonDownloadType::ON_LEARN){  // in learn mode, if common video is exits, don't need download. else download new video common
            for (mj::model::TalkingCommonInfo talkingCommonInfo : talkingCommonInfos)
            {
                std::string filePathVideoZipPath = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING + mj::helper::getFilenameFromURL(talkingCommonInfo.zipPath);
                if (FileUtils::getInstance()->isFileExist(filePathVideoZipPath))
                {
                    if (callback)
                    {
                        callback(true);
                    }
                    return _isNeedDownload;
                }
            }
            
            if (callback)
            {
                callback(false);
            }
            return _isNeedDownload;
        }
        for (mj::model::TalkingCommonInfo talkingCommonInfo : talkingCommonInfos)
        {
            std::string filePathVideoZipPath = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING + mj::helper::getFilenameFromURL(talkingCommonInfo.zipPath);
            if (!FileUtils::getInstance()->isFileExist(filePathVideoZipPath))
            {
                download_url_video_zips.push_back(talkingCommonInfo.zipPath);
                break;
            }
        }
        
        if (download_url_video_zips.size() > 0)
        {
            for (std::string download_url_thumb : download_url_video_zips)
            {
                if (mj::helper::isValidURL(download_url_thumb)) {
                    mj::api::ThumbDownloadInfo* thumbDownloadInfo = new mj::api::ThumbDownloadInfo{};
                    thumbDownloadInfo->downloadUrl = download_url_thumb;
                    thumbDownloadInfo->countRetryDownload = 0;
                    thumbDownloadInfos.push_back(thumbDownloadInfo);
                }
            }
//            mj::api::MJAPI_Download_MultiFile::call(thumbDownloadInfos,
//                [=](bool isSuccess, int, int, int) {
//                if (callback)
//                    callback(isSuccess);
//                }, 
//                [=](float percent) {
//                    callbackPercent(percent);
//                } , RemoteImageView::Type::AI_SPEAKING_VIDEO_ZIP);
            _isNeedDownload = true;
            return _isNeedDownload;
        }
        else
        {
            if (callback)
            {
                callbackPercent(100.0f);
                callback(true);
            }
            return _isNeedDownload;
        }
        
    }
    std::string LessonAISpeakingManager::getActorNameFromConfig(std::string configPath)
    {
        std::string actorName = "";
        auto configJsonData = FileUtils::getInstance()->getStringFromFile(configPath);//("config.json");

        if (!configJsonData.empty()) {

            rapidjson::Document valueData;

            valueData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(configJsonData.c_str());

            if (!valueData.IsNull() && !valueData.HasParseError() && valueData.IsObject()) {

                rapidjson::StringBuffer rj_buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
                valueData.Accept(writer);
                CCLOG("AI Speaking JSON: \n%s", rj_buffer.GetString());

                actorName = JSON_SAFE_EXTRACT(valueData, "actor_name", String, "");
            }
        }
        return actorName;
    }
}
