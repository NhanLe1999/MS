//
//  MJDefinitions.h
//  MJStory
//
//  Created by To Ngoc Linh on 8/4/17.
//

#ifndef MJDefinitions_h
#define MJDefinitions_h

#pragma mark - URL

#define key_appstore_app_url        "https://itunes.apple.com/app/id1236664909"
#define key_appstore_app_vn_url     "https://itunes.apple.com/app/id1389646429"

#define key_appstore_rate_url       ""
#define key_terms_of_use_url        "http://www.monkeystories.net/terms-and-conditions.html"
#define key_privacy_policy_url      "http://www.monkeystories.net/privacy-policy.html"


#define key_app_store_app_android   "https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior.story"
#define key_app_store_app_vn_android   "https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior.story"

#define key_app_store_app_mac       "https://itunes.apple.com/us/app/monkey-stories-books-games/id1282232882?ls=1&mt=12"
#define key_app_store_app_vn_mac       "https://itunes.apple.com/us/app/monkey-stories-books-games/id1389646429?ls=1&mt=12"

#define key_app_store_app_win32     "http://bit.ly/caidat02"
#define key_app_store_app_vn_win32     "http://bit.ly/caidat02"

#pragma mark - Key use in UserDefault
#define key_freetime_expire     "key.default.freetime.expire"
#define key_freetime_read       "key.default.freetime.read"
#define key_first_api_called    "key.default.first.api.call.done"
#define key_device_identifier   "key.default.device.identifier"
#define key_api_version         "key.default.api.version"
#define key_api_appinfo_called  "key.appinfo.called.time"

#define key_time_offline_limit     "key_time_offline_limit"
#define key_data_converted      "key_data_converted"
#define key_image_cache_subfolders_converted      "key_image_cache_subfolders_converted"
#define key_update_story_ids      "key_update_story_ids"
#define key_update_lesson_ids      "key_update_lesson_ids"
#define key_update_lesson_ai_speking_ids      "key_update_lesson_ai_speking_ids"

#define key_api_version_all_games_local "key.api.all.games"
#define key_api_all_lessons_version_local "key.api.all.lessons.version"
#define key_version_api_audiobook "key.version.audiobook.api"
#define key_api_version_story_local "key_api_ver_audio_book"
#define key_api_version_ai_speaking_topic_local "key_api_version_ai_speaking_topic_local"
#define key_api_version_ai_speaking_lesson_list_local "key_api_version_ai_speaking_lesson_list_local"
#define key_api_version_ai_speaking_common_list_local "key_api_version_ai_speaking_common_list_local"

#define key_api_version_audio_local "key_api_ver_story"
#define key_api_version_material_local "key_api_ver_material"
#define key_api_version_categories_local "key_api_ver_categories_local"
#define key_api_version_app_info_local "key_api_version_app_info_local"
#define key_api_version_version_trial_items_local "key_api_version_version_trial_items_local"

#define key_api_version_story_fisrt_install "key_api_ver_audio_book_fisrt_install"
#define key_api_version_ai_speaking_topic_fisrt_install "key_api_version_ai_speaking_topic_fisrt_install"
#define key_api_version_ai_speaking_lesson_list_fisrt_install "key_api_version_ai_speaking_lesson_list_fisrt_install"
#define key_api_version_ai_speaking_common_list_fisrt_install "key_api_version_ai_speaking_common_list_fisrt_install"

#define key_api_version_audio_fisrt_install "key_api_ver_story_fisrt_install"
#define key_api_all_lessons_version_fisrt_install "key.api.all.lessons.version_fisrt_install"

#define key_api_version_story_v2_fromJsonServer "key_api_version_story_v2_fromJsonServer"
#define key_api_version_ai_speaking_topic_fromJsonServer "key_api_version_ai_speaking_topic_fromJsonServer"
#define key_api_version_ai_speaking_lesson_list_fromJsonServer "key_api_version_ai_speaking_lesson_list_fromJsonServer"
#define key_api_version_ai_speaking_common_list_fromJsonServer "key_api_version_ai_speaking_common_list_fromJsonServer"

#define key_api_version_audio_v2_fromServer "key_api_version_audio_v2_fromServer"
#define key_api_version_material_fromServer "key_api_version_material_fromServer"
#define key_api_version_all_games_fromServer "key_api_version_all_games_fromServer"
#define key_api_all_lessons_version_fromServer "key_api_all_lessons_version_fromServer"
#define key_api_version_categories_fromServer "key_api_ver_categories_fromServer"
#define key_api_version_app_info_from_server "key_api_version_app_info_from_server"

#define key_api_version_version_trial_items_from_server "key_api_version_version_trial_items_from_server"
#define key_api_version_free_trial "key_api_version_free_trial"
#define key_api_age_free_trial "key_api_age_free_trial"

#define keypay_inapp      "key_pay_inapp"
#define key_time_purchased     "key_time_purchased"
#define key_time_activated      "key_time_activated"
#define key_time_expire      "key_time_expire"
#define key_product_list      "key_product_list"
#define key_is_active      "key_is_active"
#define key_is_free      "key_is_free"
#define key_free_days      "key_free_days"
#define key_is_purchased      "key_is_purchased"
#define key_new_version      "key_new_version"
#define key_pay_use_url      "key_pay_use_url"
#define key_is_free_story     "key_is_free_story"


//#define key_api_version_material_local "key_api_version_material_appinfo"
#define key_ver_newest_version "key_ver_newest_version" // QuanPD: Key cho app version mới nhất

#define key_material_max_items_sent         "key_material_max_items_sent"       // số worksheet tối đa được chọn
#define key_material_max_items_sent_free    "key_material_max_items_sent_free"  // số worksheet tối đa được chọn cho người dùng FREE

#define key_alternative_domain_story "key.alternative.domain.story"
#define key_alternative_domain_audio "key.alternative.domain.audio"
#define key_alternative_domain_lesson "key.alternative.domain.lesson"

#define keyPriceInfo        "mj_price_info"
#define keyReceipt          "mj_receipt"
#define keyTimeExpire       "mj_subscription_expire"
#define keyLicense          "mj_license_key"
#define keyCoupon           "mj_coupon"
#define keyDiscount         "mj_discount_percent"
#define key_server_id_download "mj_server_id_download"
#define key_server_id_temp_download "mj_server_id_temp_download"
#define key_popup_id_to_show "key_popup_id_to_show"
#define key_popup_showed "key_popup_showed_"
#define key_Actived         "is_Actived"
#define key_isFreeUse       "is_FreeUse"
#define key_isPurchased       "is_purchased"

#define key_is_submit       "key_is_submit"
#define key_skip			"is_skip"
#define key_feature_phonics "key_feature_phonics"
#define key_feature_worksheet "key_feature_worksheet"
#define key_feature_ai_speaking "key_feature_ai_speaking"

#define key_onboarding_part_1 "mj_onboarding_part1"
#define key_onboarding_part_2 "mj_onboarding_part2"
#define key_onboarding_part_3 "mj_onboarding_part3"
#define key_onboarding_license "mj_onboarding_license"
#define key_onboarding_lessons	"mj_onboarding_lessons"

#define key_lesson_learn "key_is_learned_lesson_phonic"

#define key_show_parent_lock "mj_show_parent_lock"
#define key_show_parent_profile_menu "mj_show_parent_profile_menu"

#define key_current_grade_id "key.default.grade.id"
#define key_current_grade_id_new "key.default.grade.id.new"

#pragma mark - Key use in custom event dispatcher
#define key_event_coupon_change     "event.coupon.change"
#define key_event_search_page_close "event.search.close"
#define key_event_welcome_popup_close "event.welcome.close"
#define key_event_process_refresh_api_done "event.process.refresh.api"
#define key_event_choose_level "event.choose.level"
#define key_even_quit_app_with_esc_keyboard "even.quitapp.with.esc.keyboard"
#define key_event_language_changed "event.language.changed"
#define key_event_grade_changed "event.grade.changed"
#define key_change_displaylang "mjstory.displaylang"
#define key_back_from_fullscreenpopup "event.fullscreen.popup.close"
#define key_reload_lesson_state "event.reload.lesson.state"
#define key_lesson_ended "event.lesson.ended"
#define key_event_got_lesson_details "event.got.lesson.details"
#define key_event_activity_wrapper_ready "event.activity.wrapper.ready"
#define key_event_go_to_subscribe_page "event.go.to.subscribe.page"
#define key_event_go_to_license "event.go.to.license.page"
#define key_event_go_to_free_page "event.go.to.free.page"
#define key_event_go_to_lessons_page "event.go.to.lessons.page"
#define key_event_go_to_audiobooks_page "event.go.to.audiobooks.page"
#define key_event_app_active_again "event.app.enter.foreground"

#define key_event_update_recently_audiobook "event.update.recenly.audiobook"
#define key_event_device_registered "event.device.registered"
#define key_event_intro_completed "event.intro.completed"
#define key_event_go_to_parents "event.go.to.parents"
#define key_event_refresh_downloaded_favourite "event.refresh.downloaded.favourite"
#define key_event_gradebox_show "event.gradebox.show"
#define key_event_gradebox_hide "event.gradebox.hide"
#define key_event_gradebox_tap_from_sidebar "event.gradebox.sidebar.tap"
#define key_event_play_locked_lesson "event.play.locked.lesson"
#define key_event_app_become_purchased "event.app.become.purchased"
#define key_event_profile_switched "event.profile.switched"
#define key_event_profile_first_created "event.profile.first.created" // lần đầu tiên tạo profile, push lên Clever Tap khi vào home
#define key_event_profile_choose "event.profile.choose" // chọn profile, ở màn hình chọn profile khi vào app

#define key_click_shop_type_popup "key.click.shop.type.popup" // kiểu của event click_shop là popup hay organic

#define key_count_guide_story "key_count_guide_story"
#define key_current_guide_showed "key_current_guide_showed"
#define key_first_time_created_profile "key_first_time_created_profile"
#define key_number_activity_of_grade_phonic "Grade_Lesson_numberLearn_activity_%d"
#define key_user_id "user.id"
#define key_user_mail "user.mail"
#define key_user_phone "user.phone"
#define key_user_name "user.name"
//not use anymore
#define key_show_viral "key_show_viral"
#define key_user_birthday "user.birthday"
//

#define key_logged_in "key_logged_in"
#define key_first_logged_out "key_first_logged_out"

#define profile_version_control "profile_version_control"

#define url_ios_vn        "https://itunes.apple.com/app/id1388216940"
#define url_android_vn   "https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior.story"
#define url_mac_vn       "https://itunes.apple.com/us/app/monkey-stories-books-games/id1389646429?ls=1&mt=12"
#define url_win5_vn     "http://www.monkeystories.net"

#pragma mark - HieuNT
#define DEF_DOMAIN_CORRECT_MS                       "monkeystories.vn"
#define DEF_DOMAIN_CORRECT_MS_NEW                    "monkeystories.page.link"
#define DEF_DOMAIN_CORRECT_VM                       "vmonkey.vn"
#define DEF_DEEPLINK_PART_APP                       "app"
#define DEF_DEEPLINK_PART_QRBOOK                    "qrbook"
#define DEF_DEEPLINK_PART_QRSTORY                   "qrstory"
#define DEF_DEEPLINK_PART_QRAUBOOK                  "qraubook"
#define DEF_DEEPLINK_PART_QRPHONICS                 "qrphonics"
#define DEF_DEEPLINK_PART_QRREADING_COMPREHENSION   "qrrc"

#define DEF_DEEPLINK_KEYNAME_TASK   "task"

#define ms_scene_name_intro         "ms_scene_name_intro"   // tên của scene lúc intro, add trong hàm createScene(), đừng set lại nhé
#define ms_scene_name_home          "ms_scene_name_home"    // tên của scene ở home, add trong hàm createScene(), đừng set lại nhé

#define key_email_material_x        "key_email_material_%d"         // email của người dùng để nhận học liệu (material), tham số là user_id
#define key_mat_recent_searches     "key_mat_recent_searches"       // những từ khoá tìm gần nhất
#define key_mat_worksheet_sent_x_y  "key_mat_worksheet_sent_%d_%d"  // danh sách học liệu đã được gửi, tham số là user_id và profile_id

/*quân thêm ở đây*/
#define key_select_see_more_story	"key_select_see_more_stroy"		// Trong tab stories, người dùng có chọn "Want to see more ?" hay không 

/*Nhân thêm*/
#define key_time_start_learn_lesson	"key_time_start_learn_lesson" // thời gian lúc bắt đầu học một lesson
#define key_time_user_exit_app	"key_time_user_exit_app" // thời gian lúc bắt đầu học một lesson
#define key_sum_time_user_exit_app	"key_sum_time_user_exit_app" // thời gian lúc bắt đầu học một lesson
#define key_read_from	"key_read_from" 
#define key_read_from_of_audiobook	"key_read_from_of_audiobook" 
#define key_start_activiy_of_lesson	"key_start_activiy_of_lesson" 
#define key_download_lesson	"key_download_lesson" 
#define key_turn_previous_page		"key_turn_previous_page" 
#define key_click_confirm_pop_up	"key_click_confirm_pop_up" 
#define key_number_profile			"key_number_profile" 
#define key_name_last_learn_activity  "key_name_last_learn_activity" 
#define key_click_on_delete_stories	"key_click_on_delete_stories" 
#define key_live_in_screen	"key_live_in_screen" 
#define key_link_url_download	"key_link_url_download" 
#define key_type_error	"key_type_error" 
#define key_reason_error	"key_reason_error" 
#define key_source_subscribe	"key_source_subscribe"
#define key_check_is_first_install_is_finished	"key_check_is_first_install"  //if firstInstall is finished, set to true, else set to false
#define key_checkIsLoadUpdate "key_check_is_load_update"

#define key_push_event_crash_to_fibase	"key_push_event_crash_to_fibase"
#define key_count_retry "key_count_retry"
#define key_speed_internet "key_speed_internet"


#define key_property_price	"key_property_price" 
#define key_choose_package	"key_choose_package" 
#define key_package_length	"key_package_length"
#define REQUEST_TIME_OUT 30.0f
#define   CHILDREN_AGED   4
#define key_last_update_id "key_last_update_id"

#define KEY_RESTORE_CLICKED						"key.restore.clicked"
#define KEY_SUBSCRIPTION_IS_BACK				"key.subscription.is.back"
#define KEY_PURCHASE_SOURCE						"key.purchase.source"
#define KEY_TITLE_PURCHASE_SOURCE				"key.title.purchase.source"
#define KEY_SUBSCRIPTION_TRIAL_TITLE			"key.subscription.trial.title"

#define key_register_phone_number "key_register_phone_number"
#define key_register_phone_contry_code "key_register_phone_contry_code"

#define	KEY_PHONECODE_ID	"phonecode_id"
#define	KEY_PHONECODE_CODE	"phonecode_code"

#define	COUNTRY_NAME_KEY	"key_country_name"
#define	COUNTRY_CODE_HERE_KEY   "key_country_code_here_name"
#define	IP_NAME_KEY	        "key_ip_name"
#define	REGION_NAME_KEY		"key_region_name"
#define	CITY_NAME_KEY		"key_city_name"
#define	LATITUDE_NAME_KEY	"key_latitude_name"
#define	LONGITUDE_NAME_KEY	"key_longitude_name"
#define	IS_VN_KEY	        "key_is_vn"
#define	PATH_INSTALL_KEY	"key_path_install"
#define	DOMAIN_STORY_VN_KEY	"key_download_url_story_vn"
#define	DOMAIN_STORY_GLOBAL_KEY	"key_download_url_story_global"
#define	DOMAIN_STORY_USING_KEY	"key_download_url_story_using"

#define	DOMAIN_GAME_VN_KEY	"key_download_url_game_vn"
#define	DOMAIN_GAME_GLOABL_KEY	"key_download_url_game_global"
#define	DOMAIN_GAME_USING_KEY	"key_download_url_game_using"

#define	DOMAIN_AUDIO_VN_KEY	"key_download_url_audio_vn"
#define	DOMAIN_AUDIO_GLOBAL_KEY	"key_download_url_audio_global"
#define	DOMAIN_AUDIO_USING_KEY	"key_download_url_audio_using"

#define	DOMAIN_ACTIVITIES_VN_KEY	"key_download_url_activities_vn"
#define	DOMAIN_ACTIVITIES_GLOBAL_KEY	"key_download_url_activities_global"
#define	DOMAIN_ACTIVITIES_USING_KEY	"key_download_url_activities_using"

#define	DOMAIN_MEDIA_KEY	"key_download_url_media"
#define	DOMAIN_MEDIA_USING_KEY	"key_download_url_media_using"
#define	DOMAIN_CATEGORY_URL_USING_KEY	"key_download_url_ai_speaking_category_icon_using"

#define	PATH_FIRST_INSTALL_KEY	"key_path_install"
#define	IS_SKIP_KEY	        "key_is_skip"
#define	THUMB_STORY_URL_KEY	"key_download_url_thumb_story"
#define	THUMB_LESSON_URL_KEY	"key_download_url_thumb_lesson"
#define	THUMB_AUDIO_URL_KEY	"key_download_url_thumb_audio"

#define	THUMB_AI_SPEAKING_URL_KEY	"key_download_url_thumb_ai_speaking"
#define	THUMB_AUDIO_URL_KEY	"key_download_url_thumb_audio"
#define	THUMB_OTHER_URL_KEY	"download_thumb_other"
#define IMAGE_CACHE_PATH "imgcache/"
#define IMAGE_CACHE_PATH_FOR_AISPEAKING "imgcache/aispeaking/"
#define CACHE_ZIP "cached/zip/"
#define VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING "videocache/aispeaking/zips/"
#define VIDEO_CACHE_PATH_FOR_AISPEAKING "videocache/aispeaking/videos/"
#define CURRENT_ACTOR_AISPEAKING "current_actor_aispeaking"
#define FIRST_CHECK_AISPEKING_PERMISSION "first_check_ai_speaking_permission"
#define PRE_TAB_UNCLOCK_KEY "pre_tab_unclock_key"
#define PLATFORM_SUPPORT_FOR_AISPEAKING  CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define SHOW_MONKEY_TALKING_KEY "show_monkey_talking"
#define key_error_url_stories "key_error_url_stories"
#define key_error_url_game_audio_book "key_error_url_game_audio_book"
#define key_Limit_download_thumb "key_Limit_download_thumb"

#define key_popup_fail_payment "key_popup_fail_payment"

#define key_time_talking_call_end "key_time_call_end"
#define key_is_relearn_talking_lesson "key_is_relearn_talking_lesson"
#define key_show_pop_lost_connection "key_show_pop_lost_connection"
#define key_turn_on_new_download "key_turn_on_new_download"
#define TURN_ON_NEW_DOMAIN true

#define NEW_AUDIO_BOOK true

/*---------------------key for freeTo paid begin------------------------*/
#define key_f2p_show_trial_alert "key_f2p_show_trial_alert"  // //key danh dau popup thong bao bat dau hoc thu duoc show 1 lan
#define key_f2p_show_notify_permisison_alert "key_f2p_show_notify_permisison_alert" //key danh dau popup xin cap quyen thong bao duoc show 1 lan
#define key_f2p_save_free_items_learned_on_day "key_f2p_save_free_items_learned_on_day"  // luu id cua cac items da hoc trong 1 ngay unlock
#define key_f2p_save_unlock_day_number "key_f2p_save_unlock_day_number" // luu danh sach ngay da unlock cua user, co toi da 7 item, moi item gia tri tu 1-7
#define key_f2p_current_learning_item_id "key_f2p_current_learning_item_id" // save item learning in progress

#define key_f2p_save_learned_items_last_day "key_f2p_save_learned_items_last_day" // đánh dấu thứ tự item đã học
#define key_f2p_save_last_date_of_learned_items "key_f2p_save_last_date_of_learned_items" //đánh dấu ngày học items đã học cuối cùng
#define key_f2p_save_list_of_learned_items "key_f2p_save_list_of_learned_items" //luu lai list id cac items đã học
#define key_f2p_save_items_learned_avata_url "key_f2p_save_items_learned_avata_url" //luu lai list avata url cua cac items đã học
#define key_f2p_save_unlocked_items "key_f2p_save_unlocked_items" //luu lai trang thai unlock cua item ngay sau item da hoc, neu chua unlock, thi thuc hien animation, neu da tung unlock, thi khong thu hien animation nua
#define key_free_content_page_option "key_free_content_page_option"
#define key_f2p_need_merge_profiles "key_f2p_need_merge_profiles"

#define key_f2p_skip_profile_id "key_f2p_skip_profile_id"
#define key_f2p_merge_status_response "key_f2p_merge_status_response"
#define key_f2p_account_free_info_for_merge "key_f2p_account_free_info_for_merge"
#define key_f2p_account_paid_info_for_merge "key_f2p_account_paid_info_for_merge"


#define key_f2p_acount_name "acount_name_f2p"
#define key_f2p_birth_day "birth_of_date_f2p"
#define key_list_profile_to_delete  "key_list_profile_to_delete"
#define key_userId_merged "key_userId_merged"
#define key_on_free_content_page "key_on_free_content_page"

#define key_source_go_to_screen_payment "key_source_go_to_screen_payment"

#define key_f2p_paid "key_f2p_paid"
#define key_f2p_free "key_f2p_free"

#define key_f2p_option_7_or_14_free_items "key_f2p_option_7_or_14_free_items"
#define SHOULD_LOGIN_DUMMY_CT "should_login_dummy_CT" //only should login one time after install
#define key_old_access_token_change_pw "old_access_token_change_pw"
#define error_code_for_merged_success 1000101
#define USER_ID_CONFIG 5704679
#define key_audio_bool_type "key_audio_bool_type"
/*---------------------key for freeTo paid end------------------------*/
/**/

/*-----------key for applyer----------*/
#define DISABLE_APPSFLYER   0
#define KEY_APPSFLYER_UID                   	"key_appsflyer_uid"
#define KEY_APPSFLYER_PRICE                   	"key_appsflyer_price"
#define KEY_APPSFLYER_CURRENCY                   "key_appsflyer_currency"
/*-----------------------------------*/

#define key_audio_bool_pause                   "key_audio_bool_pause"

#define key_debugkey_dev_environment false

#define ENABLE_API_FIRESTORE 1

#endif /* MJDefinitions_h */
	
