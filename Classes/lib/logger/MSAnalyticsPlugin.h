//
//  MSAnalyticsPlugin.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/14/17.
//

#ifndef MSAnalyticsPlugin_hpp
#define MSAnalyticsPlugin_hpp

#include "cocos2d.h"

#include "MJStoryInfoModel.h"

namespace ms {
    class AnalyticsPlugin {
    public:
        virtual void logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>>, bool doPushClevertap = false) = 0;
        virtual void setIdentifier(std::string identifier) = 0;
        virtual void setUserProperties(std::map<std::string, cocos2d::Value> props) = 0;
        
        //event khi user hoàn thành các slide onboard đầu tiên
        virtual void logEventExplore();
        
        //event khi user nhập license khi được hỏi lần đầu tiên
        virtual void logEventFirstLicenseSubmit();
        
        //event user nhập license lần đầu tiên thành công
        virtual void logEventFirstLicenseSuccess();
        
        //event user nhập license lần đầu tiên thất bại
        virtual void logEventFirstLicenseFailed();
        
        //event khi user yêu cầu quên license
        virtual void logEventFirstLicenseForgot();
        
        //event khi user skip license
        virtual void logEventFirstLicenseSkip();
        
        //event khi user hoàn thành tutorial
        virtual void logEventTutorialCompleted();
        
        //event khi user skip tutorial
        virtual void logEventTutorialSkip();
        
        //event khi user bắt đầu checkout
        virtual void logEventInitPayment();
        
        //event khi user chọn package
        virtual void logEventSelectPackage(const std::string & package_id);
        
        //event khi user chọn phương thức thanh toán
        virtual void logEventSelectMethod(const std::string & product_id, const std::string & method);
        
        virtual void logEventBankInfo();
        
        virtual void logEventCODInfo();
        
        //event khi user thanh toán (chỉ áp dụng với onepay và in app
        virtual void logEventPurchased();
        
        void logEventStartLearning(std::string lesson_type, mj::model::LessonInfo info);
        void logEventStartActivity(std::string lesson_type, mj::model::ActInfo info);
        void logEventFinishActivity(std::string lesson_type, mj::model::ActInfo info);
        void logEventFinishLesson(std::string lesson_type, mj::model::LessonInfo info);
        
        void logEventOnboardInfoSubmit();
        void logEventOnboardInfoSkip();

        //viral marketing
        void logEventShareButton(const std::string & position);
        void logEventGetSharedLink();
    protected:
    private:
    };
}
#endif /* MSAnalyticsPlugin_hpp */
