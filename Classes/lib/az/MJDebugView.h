//
// Created by To Ngoc Linh on 9/18/17.
//

#ifndef MJSTORY_MJDEBUGVIEW_H
#define MJSTORY_MJDEBUGVIEW_H

#include "cocostudiomacro2.h"

#define debugkey_full_features "mjdk_full_features"
#define debugkey_country_gb "mjdk_country_global"
#define debugkey_license_disable "mjdk_have_license"
#define debugkey_test_config "mjdk_test_config"
#define debugkey_homescreen_landscape "mjdk_homescreen_landscape"
#define debugkey_quick_play_game "mjdk_quick_game"
#define debugkey_test_7_days "mjdk_test_7days"
#define debugkey_fake_free_time "mjdk_key_fake_free_time"
#define debugkey_domain_daybehoc "mjdk_key_domain_daybehoc"
#define debugkey_domain_testdaybehoc "mjdk_key_domain_testdaybehoc"
#define debugkey_test_free "debugkey_test_free"
#define debugkey_dev_environment "debugkey_dev_environment"
#define debugkey_product_environment "debugkey_product_environment"
#define debugkey_fake_device_id "debugkey_fake_device_id"
#define debugkey_language_vn "debugkey_lang_vn"
#define debugkey_all_lessons "debugkey_all_lessons"
#define debugkey_fake_ipx "debugkey_fake_ipx"
#define debugkey_auto_play "debugkey_auto_play"
#define debugkey_auto_story "key_is_auto_play_stroy"
#define debugkey_skip_story "debugkey_skip_story"
#define debugkey_auto_audiobook "debugkey_auto_audiobook"

#define debugkey_is_test_license "debugkey_is_test_license"

#define debugkey_test_act_vmonkey "debugkey_test_act_vmonkey"


class MJDebugView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJDebugView);
    static MJDebugView * createView();

private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &name) override;
    void onReset(cocos2d::Ref * sender);
};

CREATE_READER(MJDebugView);

#endif //MJSTORY_MJDEBUGVIEW_H
