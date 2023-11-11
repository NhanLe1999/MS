//
//  MSAnalyticsPlugin.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/14/17.
//

#include "MSAnalyticsPlugin.h"

namespace ms {
    //event khi user hoàn thành các slide onboard đầu tiên
    void AnalyticsPlugin::logEventExplore() {
        logEvent("ms2_start_explore", {});
    }

    //event khi user nhập license khi được hỏi lần đầu tiên
    void AnalyticsPlugin::logEventFirstLicenseSubmit() {
        logEvent("ms2_first_license_submit", {});
    }

    //event user nhập license lần đầu tiên thành công
    void AnalyticsPlugin::logEventFirstLicenseSuccess() {
        logEvent("ms2_first_license_success", {});
    }

    //event user nhập license lần đầu tiên thất bại
    void AnalyticsPlugin::logEventFirstLicenseFailed() {
        logEvent("ms2_first_license_failed", {});
    }

    //event khi user yêu cầu quên license
    void AnalyticsPlugin::logEventFirstLicenseForgot() {
        logEvent("ms2_first_license_forgot", {});
    }

    //event khi user skip license
    void AnalyticsPlugin::logEventFirstLicenseSkip() {
        logEvent("ms2_first_license_skip", {});
    }

    //event khi user hoàn thành tutorial
    void AnalyticsPlugin::logEventTutorialCompleted() {
        logEvent("ms2_tutorial_completed", {});
    }

    //event khi user skip tutorial
    void AnalyticsPlugin::logEventTutorialSkip() {
        logEvent("ms2_tutorial_skipped", {});
    }

    //event khi user bắt đầu checkout
    void AnalyticsPlugin::logEventInitPayment() {
        logEvent("ms2_init_payment", {});
    }

    //event khi user chọn package
    void AnalyticsPlugin::logEventSelectPackage(const std::string & product_id) {
        logEvent("ms2_select_package", {
            {"product_id", cocos2d::Value(product_id)}
        });
    }

    //event khi user chọn phương thức thanh toán
    void AnalyticsPlugin::logEventSelectMethod(const std::string & product_id, const std::string & method) {
        logEvent("ms2_select_method", {
            {"product_id", cocos2d::Value(product_id)},
            {"method", cocos2d::Value(method)}
        });
    }

    //event khi user thanh toán (chỉ áp dụng với onepay và in app
    void AnalyticsPlugin::logEventPurchased() {
        logEvent("ms2_purchased", {});
    }

    void AnalyticsPlugin::logEventBankInfo() {
        logEvent("ms2_submit_info_bank", {});
    }
    
    void AnalyticsPlugin::logEventCODInfo() {
        logEvent("ms2_submit_info_cod", {});
    }
    
    void AnalyticsPlugin::logEventStartLearning(std::string lesson_type, mj::model::LessonInfo info) {
        logEvent("start_learn_lesson", {
            {"lesson_type", cocos2d::Value(lesson_type)},
            {"lesson_id",cocos2d::Value(info.lesson_id)},
            {"lesson_name",cocos2d::Value(info.name)}
        });
    }
    
    void AnalyticsPlugin::logEventStartActivity(std::string lesson_type, mj::model::ActInfo info) {
        logEvent("start_learn_activity", {
            {"lesson_type", cocos2d::Value(lesson_type)},
            {"activity_id",cocos2d::Value(info.activity_id)},
            {"activity_name",cocos2d::Value(info.name)},
            {"lesson_id",cocos2d::Value(info.lesson_id)},
            {"activity_type",cocos2d::Value(info.type)}
        });
    }
    
    void AnalyticsPlugin::logEventFinishActivity(std::string lesson_type, mj::model::ActInfo info) {
        logEvent("finnish_learn_activity", {
            {"lesson_type", cocos2d::Value(lesson_type)},
            {"activity_id",cocos2d::Value(info.activity_id)},
            {"activity_name",cocos2d::Value(info.name)},
            {"lesson_id",cocos2d::Value(info.lesson_id)},
            {"activity_type",cocos2d::Value(info.type)}
        });
    }
    
    void AnalyticsPlugin::logEventFinishLesson(std::string lesson_type, mj::model::LessonInfo info) {
        logEvent("finnish_learn_lesson", {
            {"lesson_type", cocos2d::Value(lesson_type)},
            {"lesson_id",cocos2d::Value(info.lesson_id)},
            {"lesson_name",cocos2d::Value(info.name)}
        });
    }
    
    void AnalyticsPlugin::logEventOnboardInfoSkip() {
        logEvent("ms2_onboard_skip_info", {});
    }
    
    void AnalyticsPlugin::logEventOnboardInfoSubmit() {
        logEvent("ms2_onboard_submit_info", {});
    }
    
    void AnalyticsPlugin::logEventShareButton(const std::string & position) {
        logEvent("ms2_viral_sharebtn_clicked", {
            {"popup_position", cocos2d::Value(position)}
        });
    }
    
    void AnalyticsPlugin::logEventGetSharedLink() {
        logEvent("ms2_viral_shared_link", {});
    }
}
