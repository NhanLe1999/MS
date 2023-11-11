//
//  MSEventLogger.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/13/17.
//

#ifndef MSEventLogger_hpp
#define MSEventLogger_hpp

#include "cocos2d.h"
#include "MSLoggerConstant.h"
#include "MJStoryInfoModel.h"
#include "manager/CleverTapManager.h"

#define MS_LOGGER ms::EventLogger::getInstance()
#define CRASH_AUTO_LOG  MS_LOGGER.logEvent(StringUtils::format("%s", __FUNCTION__),{})

namespace ms {

    struct StoryInfoTracker
    {
        int m_currentPage;
        float m_totalTimeOnPage;
        float m_standardTime;
        int m_repeatCount;
        int m_imgClickedCount;
        int m_gafClickedCount;

        StoryInfoTracker()
        {

        }

        StoryInfoTracker(int i_currentPage, float i_totalTimeOnPage, float i_standardTime, int i_repeatCount, int i_imgClickedCount, int i_gafClickedCount)
        {
             m_currentPage = i_currentPage ;
             m_totalTimeOnPage = i_totalTimeOnPage;
             m_standardTime = i_standardTime;
             m_repeatCount = i_repeatCount;
             m_imgClickedCount = i_imgClickedCount;
             m_gafClickedCount = i_gafClickedCount;
        }
    };

    struct StoryLearningTracker
    {
        enum class E_START_STORY_FROM
        {
            FAVOURITE_TAB,
            DOWNLOADED_TAB,
            STORIES_TAB
        };

		int m_commonStoryID;
        int m_storyID;
        std::string m_storyName;
        std::string m_from;
        int m_totalPage;
        int m_userAge;
        std::string m_storyAuthor;
        std::string m_storyDesigner;
        int m_storyLevel;
        std::string m_storyTopic;
        std::vector<StoryInfoTracker> m_storyInfors;
		int m_lessonID;
		std::string m_lessonName;
    };

    struct FinishStoryTracker
    {
        std::string m_storyName;
        int m_storyID;
        bool m_favorited =  false;
    };

    class AnalyticsPlugin;
    
    class EventLogger {
    public:
        static EventLogger & getInstance() {
            static EventLogger sharedLogger;
            return sharedLogger;
        };
        
        void logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> properties, bool doPushCelvertapEVent = false);
        void setIdentifier(std::string name);
        void setUserProperties(std::string name, std::string value);

        //event khi user hoàn thành các slide onboard đầu tiên
        void logEventExplore();
        
        //event khi user nhập license khi được hỏi lần đầu tiên
        void logEventFirstLicenseSubmit();
        
        //event user nhập license lần đầu tiên thành công
        void logEventFirstLicenseSuccess();
        
        //event user nhập license lần đầu tiên thất bại
        void logEventFirstLicenseFailed();
        
        //event khi user yêu cầu quên license
        void logEventFirstLicenseForgot();
        
        //event khi user skip license
        void logEventFirstLicenseSkip();
        
        //event khi user hoàn thành tutorial
        void logEventTutorialCompleted();
        
        //event khi user skip tutorial
        void logEventTutorialSkip();
        
        //event khi user bắt đầu checkout
        void logEventInitPayment();
        
        //event khi user chọn package
        void logEventSelectPackage(const std::string & product_id);
        
        //event khi user chọn phương thức thanh toán
        void logEventSelectMethod(const std::string & product_id, const std::string & method);
        
        //event khi user submit thông tin cá nhân bank
        void logEventBankInfo();
        
        void logEventCODInfo();
        
        //event khi user thanh toán (chỉ áp dụng với onepay và in app
        void logEventPurchased();
        
        void logEventStartLearning(std::string lesson_type,mj::model::LessonInfo info);
        void logEventStartActivity(std::string lesson_type,mj::model::ActInfo info);
        void logEventFinishActivity(std::string lesson_type,mj::model::ActInfo info);
        void logEventFinishLesson(std::string lesson_type,mj::model::LessonInfo info);
        
        void logEventOnboardInfoSubmit();
        void logEventOnboardInfoSkip();

        //viral marketing
        void logEventShareButton(const std::string & position);
        void logEventGetSharedLink();
        
    protected:
        explicit EventLogger();
        
    private:
        EventLogger(EventLogger const &);
        void operator = (EventLogger const &);
        
        std::vector<AnalyticsPlugin *> plugins;
    };
}

#endif /* MSEventLogger_hpp */
