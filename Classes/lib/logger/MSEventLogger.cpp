//
//  MSEventLogger.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/13/17.
//

#include "MSEventLogger.h"
#include "MSFirebasePlugin.h"
//#include "MSMixpanelPlugin.h"
//#include "MSFacebookPlugin.h"

namespace ms {
    EventLogger::EventLogger() {
        plugins.push_back(&ms::analytics::Firebase::getInstance());
//        plugins.push_back(&ms::analytics::MixPanel::getInstance());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
        //plugins.push_back(&ms::analytics::Facebook::getInstance());
#endif//win32
    }
    
    void EventLogger::logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> property, bool doPushCelvertapEVent) {
        for (auto plugin : plugins) {
            plugin->logEvent(name, property, doPushCelvertapEVent);
        }
    }
    
    void EventLogger::setIdentifier(std::string identifier) {
        for (auto plugin : plugins) {
            plugin->setIdentifier(identifier);
        }
    }
    
    void EventLogger::setUserProperties(std::string name, std::string value) {
        for (auto plugin : plugins) {
            plugin->setUserProperties({{name, cocos2d::Value(value)}});
        }
    }
    
    //event khi user hoàn thành các slide onboard đầu tiên
    void EventLogger::logEventExplore() {
        for (auto plugin : plugins) {
            plugin->logEventExplore();
        }
    }
    
    //event khi user nhập license khi được hỏi lần đầu tiên
    void EventLogger::logEventFirstLicenseSubmit() {
        for (auto plugin : plugins) {
            plugin->logEventFirstLicenseSubmit();
        }
    }
    
    //event user nhập license lần đầu tiên thành công
    void EventLogger::logEventFirstLicenseSuccess() {
        for (auto plugin : plugins) {
            plugin->logEventFirstLicenseSuccess();
        }
    }
    
    //event user nhập license lần đầu tiên thất bại
    void EventLogger::logEventFirstLicenseFailed() {
        for (auto plugin : plugins) {
            plugin->logEventFirstLicenseFailed();
        }
    }
    
    //event khi user yêu cầu quên license
    void EventLogger::logEventFirstLicenseForgot() {
        for (auto plugin : plugins) {
            plugin->logEventFirstLicenseForgot();
        }
    }
    
    //event khi user skip license
    void EventLogger::logEventFirstLicenseSkip() {
        for (auto plugin : plugins) {
            plugin->logEventFirstLicenseSkip();
        }
    }
    
    //event khi user hoàn thành tutorial
    void EventLogger::logEventTutorialCompleted() {
        for (auto plugin : plugins) {
            plugin->logEventTutorialCompleted();
        }
    }
    
    //event khi user skip tutorial
    void EventLogger::logEventTutorialSkip() {
        for (auto plugin : plugins) {
            plugin->logEventTutorialSkip();
        }
    }
    
    //event khi user bắt đầu checkout
    void EventLogger::logEventInitPayment() {
        for (auto plugin : plugins) {
            plugin->logEventInitPayment();
        }
    }
    
    //event khi user chọn package
    void EventLogger::logEventSelectPackage(const std::string & product_id) {
        for (auto plugin : plugins) {
            plugin->logEventSelectPackage(product_id);
        }
    }
    
    //event khi user chọn phương thức thanh toán
    void EventLogger::logEventSelectMethod(const std::string & product_id, const std::string & method) {
        for (auto plugin : plugins) {
            plugin->logEventSelectMethod(product_id, method);
        }
    }
    
    void EventLogger::logEventCODInfo() {
        for (auto plugin : plugins) {
            plugin->logEventCODInfo();
        }
    }
    
    void EventLogger::logEventBankInfo() {
        for (auto plugin : plugins) {
            plugin->logEventBankInfo();
        }
    }
    
    //event khi user thanh toán (chỉ áp dụng với onepay và in app
    void EventLogger::logEventPurchased() {
        for (auto plugin : plugins) {
            plugin->logEventPurchased();
        }
    }
    
    void EventLogger::logEventStartLearning(std::string lesson_type, mj::model::LessonInfo info) {
        for (auto plugin : plugins) {
            plugin->logEventStartLearning(lesson_type,info);
        }
    }
    
    void EventLogger::logEventStartActivity(std::string lesson_type, mj::model::ActInfo info) {
        for (auto plugin : plugins) {
            plugin->logEventStartActivity(lesson_type,info);
        }
    }
    
    void EventLogger::logEventFinishActivity(std::string lesson_type, mj::model::ActInfo info) {
        for (auto plugin : plugins) {
            plugin->logEventFinishActivity(lesson_type,info);
        }
    }
    
    void EventLogger::logEventFinishLesson(std::string lesson_type, mj::model::LessonInfo info) {
        for (auto plugin : plugins) {
            plugin->logEventFinishLesson(lesson_type,info);
        }
    }
    
    void EventLogger::logEventOnboardInfoSubmit() {
        for (auto plugin : plugins) {
            plugin->logEventOnboardInfoSubmit();
        }
    }
    
    void EventLogger::logEventOnboardInfoSkip() {
        for (auto plugin : plugins) {
            plugin->logEventOnboardInfoSubmit();
        }
    }
    
    void EventLogger::logEventShareButton(const std::string & position) {
        for (auto plugin : plugins) {
            plugin->logEventShareButton(position);
        }
    }
    
    void EventLogger::logEventGetSharedLink() {
        for (auto plugin : plugins) {
            plugin->logEventGetSharedLink();
        }
    }
}
