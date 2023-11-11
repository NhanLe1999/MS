#ifndef Key_Default_Model_h
#define Key_Default_Model_h
#include"MJDefinitions.h"
#define kTimeEndLanHoc 5
#define key_ver_api_home "key_ver_api_home"
#define key_ver_api_level "key_ver_api_level"
#define key_level_choose "key_level_choose"
#define key_level_listlevellang "key_level_listlevellang"
#define key_current_lang "key_current_lang"
#define key_list_lang "key_list_lang"
#define image_cache_path IMAGE_CACHE_PATH
#define key_new_stories "key_new_stories"
#define key_popular_stories "key_popular_stories"
#define key_recent_stories "key_recent_stories"
#define key_stories_recentlyread "key_stories_recentlyread"
#define key_audio_recentlyread "key_audio_recentlyread"
#define key_liststories_docroi "key_liststories_docroi"
#define key_listaudiobook_docroi "key_listaudiobook_docroi"
#define key_ratestories "key_ratestories"
#define key_favorites "key_favorites"
#define key_subscribe_type "key_subscribe_type"
#define kRequestTimeout 60.0f
#define max_index_pageview 5

#define key_listmessageread "key_listmessageread"
#define key_token_sent "key_token_sent"
#define key_token_device "key_token_device"

#define key_call_welcome_popup "key_call_welcome_popup"
#define key_display_levelguide "key_display_levelguide"
#define key_readinglevel_next "key_readinglevel_next"
#define key_lang_display "key_lang_display"
#define key_serverVN "key_serverVN"

#define kstoryversion "kstoryversion"
#define kNumStoryCanDown "kNumStoryCanDown"
#define kListStoryDown "kListStoryDown"

#define key_is_licence "key_is_licence"
#define key_country_code "key_country_code"

//update_app
#define key_update_show "key_update_show"
#define key_update_content "key_update_content"
#define key_update_index "key_update_index"

//load Data một lần
#define key_loadhome_done "key_loadhome_done"
#define key_loadexplore_done "key_loadexplore_done"

#define key_app_firstdate "key_app_firstdate"
#define key_time_curent "key_time_curent"

#define key_time_fake "key_time_fake"

#define key_time_mark_new_available "key_time_mark_new_available"
#define key_time_mark_new_AUDIO_available "key_time_mark_new_AUDIO_available"
#define time_mark_new_STORY 1505736000 //date = 18/09/2017
#define time_mark_new_AUDIO 1523324370 //date = 10/04/2018

#define key_time_mark_new "key_time_mark_new"
#define key_time_mark_new_AUDIO "key_time_mark_new_AUDIO"

#define key_version_api_app "key_version_api_app"
#define key_url_story "key_url_story"

#define key_time_update_skip "key_time_update_skip"

#define key_ver_update_server "key_time_update_server"
#define key_ver_update_client "key_time_update_client"
#define key_ver_update_app "key_ver_update_app"
#define key_version_required "key_version_required"

#define key_time_update_app "key_time_update_app"

#define key_common_res_version_current  "key.common.res.version.curernt"
#define key_common_res_version_request "key.common.res.version.request"
#define key_common_res_path "key.common.res.path"

#define key_thumb_res_version_current  "key.thumb.res.version.curernt"
#define key_thumb_res_version_request "key.thumb.res.version.request"
#define key_thumb_res_path "key.thumb.res.path"

#define key_lesson_thumb_res_version_current  "key.lesson_thumb.res.version.curernt"
#define key_lesson_thumb_res_version_request "key.lesson_thumb.res.version.request"
#define key_lesson_thumb_res_path "key.lesson_thumb.res.path"

#define key_story_thumb_res_version_current  "key.story_thumb.res.version.curernt"
#define key_story_thumb_res_version_request "key.story_thumb.res.version.request"
#define key_story_thumb_res_path "key.story_thumb.res.path"

#define key_audio_thumb_res_version_current  "key.audio_thumb.res.version.curernt"
#define key_audio_thumb_res_version_request "key.audio_thumb.res.version.request"
#define key_audio_thumb_res_path "key.audio_thumb.res.path"

//background music
#define key_background_music "key_bg_music"
#define key_is_phonic_lesson "is.phonic.lesson"

#define time_show_update_again 7

//
#define key_current_hometab "key_current_hometab"
// account profile

#define key_current_profile_id "key_current_profile_id"
#define key_max_account_profile "key_max_account_profile"
// MS Grade Manager
#define key_grade_stories "key.default.grade.story"
#define key_grade_audiobooks "key.default.grade.audiobook"
#define key_grade_lessons "key.default.grade.lesson"
#define key_grade_lessons_phonic "key.default.grade.lesson.phonic"
#define key_grade_lessons_f2p "key.default.grade.lesson.f2p"

// MS Lesson Manager
#define key_last_lesson_x "key_last_lesson_%d"
#define key_last_activity_x "key_last_activity_%d"
#define key_last_activity_x_y "key_last_activity_%d_%d" // lưu activity_id đã chơi xong của một lesson, tham số x profile_id, y là lesson_id
// MS Chapter
#define key_last_time_played_chapter_x "key_last_time_played_chapter_%d"
#define key_last_time_played_x "key_last_time_played_%d"

// Entering info thread
#define key_last_time_enter_info "key_last_time_enter_info" // nhớ là key này đi kèm với <user_info>._id ở cuối

// GROUP PROMOTION URL
#include "platform/CCPlatformConfig.h"

#define GROUP_MS_URL_ANDROID "http://bit.ly/AndroiMS"
#define GROUP_MS_URL_IOS "http://bit.ly/iOSMS"
#define GROUP_MS_URL_WINDOW "http://bit.ly/WindowMS"

#define GROUP_VM_URL_ANDROID "http://bit.ly/AndroiVM"
#define GROUP_VM_URL_IOS "http://bit.ly/iOSVMon"
#define GROUP_VM_URL_WINDOW "http://bit.ly/WindowVM"
#if CC_TARGET_PLATFORM == CC_FLATFORM_ANDROID
#define GROUP_MS_URL GROUP_MS_URL_ANDROID
#define GROUP_VM_URL GROUP_VM_URL_ANDROID
#elseif CC_TARGET_PLATFORM == CC_FLATFORM_IOS
#define GROUP_MS_URL GROUP_MS_URL_IOS
#define GROUP_VM_URL GROUP_VM_URL_IOS
#else
#define GROUP_MS_URL GROUP_MS_URL_WINDOW
#define GROUP_VM_URL GROUP_VM_URL_WINDOW
#endif

#endif
