//
//  MSLoggerConstant.h
//  MJStory
//
//  Created by To Ngoc Linh on 12/14/17.
//

#ifndef MSLoggerConstant_h
#define MSLoggerConstant_h

#define kSearchEventTag              "tag"
#define kSearchEventType             "type"
#define kSearchEventBookID           "choose_story_from_searching_box"
#define kSearchEventItemName         "itemName"
#define kSearchEventTypeAudiobook    "audiobook"
#define kSearchEventTypeStory        "story"

#define msAnalyticsKStoryID           "storyId"
#define msAnalyticsKStoryName         "storyName"
#define msAnalyticsKStoryFrom         "from"
#define msAnalyticsKStoryTotalPage    "totalPageInStory"
#define msAnalyticsKProfileAge        "userProfileAge"
#define msAnalyticsKStoryAuthor       "storyAuthor"
#define msAnalyticsKStoryDesigner     "storyDesigner"
#define msAnalyticsKStoryLevel        "storyLevel"
#define msAnalyticsKStoryTopic        "storyTopic"

namespace ms {
    namespace analytics {
        
        //funnel 1: read a story
        static const char * kEventViewStoryInfo = "ms_view_info_of_story";
        static const char * kEventReadStory = "ms_read_a_story";
        static const char * kEventFinishStory = "ms_finish_a_story";
        static const char * kEventPlayGameInStory = "ms_play_game_in_story";
        static const char * kEventFinishGameInStory = "ms_complete_game_in_story";
        static const char * kEventReadRecommendedStory = "ms_read_recommended_story";
        static const char * kEventRateStory = "ms_rate_story";
        
        //funnel 2: checkout
        static const char * kEventViewPurchaseInfo = "ms_view_purchase_page";
        static const char * kEventBeginCheckout = "ms_start_checkout";
        static const char * kEventCheckoutChooseMethod = "ms_choose_checkout_method";
        static const char * kEventFinishCheckout = "ms_finish_checkout";
        static const char * kEventFillInfo = "ms_fill_info";
        static const char * kEventCheckoutStep2 = "ms_checkout_step2";
        
        //funnel 3: listen to audiobook
        static const char * kEventViewAudiobookPage = "ms_view_tab_audiobook";
        static const char * kEventViewAudiobookPlayer = "ms_view_audiobook_player";
        static const char * kEventStartListening = "ms_start_listen_audiobook";
        static const char * kEventFinishListening = "ms_finish_listen_audiobook";
        
        //funnel 4: downloading
        static const char * kEventStartDownloading = "ms_start_downloading";
        static const char * kEventFinishDownloading = "ms_finish_downloading";
        
        //funnel 5: load api
        static const char * kEventAPIAppInfo = "ms_call_api_app_info";
        static const char * kEventAPIListPay = "ms_call_api_list_pay";
        static const char * kEventAPIReadlingLevel = "ms_call_api_reading_level";
        static const char * kEventAPIRefresh = "ms_call_api_refresh";
        static const char * kEventAPIAllStories = "ms_call_api_all_stories";
        static const char * kEventAPIAllGames = "ms_call_api_all_games";
        static const char * kEventAPIAllNewCategories = "ms_call_api_all_new_categories";
        static const char * kEventAPIListSampleStories = "ms_call_api_sample_stories";
        static const char * kEventAPIAllAudiobooks = "ms_call_api_all_audiobooks";
        static const char * kEventAPIAudiobooksSeries = "ms_call_api_audiobook_series";
        static const char * kEventAPIAllLessons = "ms_call_api_all_lessosn";
        static const char * kEventAPIHome = "ms_call_api_home";
        static const char * kEventLoadUpdate = "ms_call_api_load_update";
        static const char * kEventProfileList = "ms_call_api_profile_list";
        
        static const char * kEventReadSampleStories = "ms_read_sample_stories";

        
        //property
        static const char * kPropertyCheckoutOption = "checkout_option";
        static const char * kPropertyPackage = "item_id";
        static const char * kPropertyPrice = "price";

        static const char * kPropertyIndex = "index";
        
        static const char * kPropertyStoryId            = "index";
        static const char * kPropertyStoryName          = "item_name";
        static const char * kPropertyStoryLanguage      = "location";
        static const char * kPropertyStoryReadingType   = "reading_type";
        static const char * kPropertyStoryFavorite      = "favorited";

        static const char * kPropertyGameType = "item_category";

        static const char * kPropertyRateType = "value";

        static const char * kPropertySourceViewPurchase = "source";

        static const char * kPropertyChangePackageFrom = "source";
        static const char * kPropertyChangePackageTo = "item_id";
        
        static const char * kPropertyAudiobookName = "item_name";
        
        static const char * kDownloadItemName = "item_name";
        static const char * kDownloadTime = "value";


        static const char * kStoryLearningProperties    = "story_learning_properties";
        static const char * kSearchEvent                = "search_event";
        static const char * kFinishStoryEvent           = "finish_story";

    }
}


#endif /* MSLoggerConstant_h */
