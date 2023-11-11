#ifndef __StoryFreeManager__
#define __StoryFreeManager__
#define FREE_STORY_MANAGER StoryFreeManager::getInstance()

#include <stdio.h>
#include "cocos2d.h"
#include "MJDefinitions.h"

#define klist_stories_free "klist_stories_free"
#define date_get_storyfree "day_get_freestories"
#define key_storyfree_read "key_storyfree_read"
#define key_storyfree_choose "key_storyfree_choose"
#define key_storyfree_recently "key_storyfree_recently"
#define date_get_audiobookfree "date_get_audiobookfree"

#define key_numstories_rate_bonus "key_numstories_rate_bonus"
#define key_rate_app_won "key_rate_app_won"
#define key_rate_app "key_rate_app"
#define key_rate_list_stories "key_rate_list_stories"

#define key_hailong_app "key_hailong_app"

//FreeTime Read

#define klist_story_freetime "klist_story_freetime"

#define key_numpage_read "key_numpage_read"
#define max_numpage_freeread 10

#define kstoryid_free_recentread "kstoryid_free_recentread"

#define ACTION_STORY "action_story"
#define ACTION_LESSON "action_lesson"
#define ACTION_AUDIOBOOK "action_audiobook"
#define ACTION_TYPE "action_type"
#define AUDIO_BOOK_TIMEREAD "audio_book_timeread"
#define AUDIO_BOOK_MAX_TIMEREAD 5000.0f
#define klist_audiobook_free "klist_audiobook_free"
#define key_audiobook_read "key_audiobook_read"
#define key_audiobook_choose "key_audiobook_choose"

#define knum_lesson_free "knum_lesson_free"
#define key_lesson_read "key_lesson_read"
#define key_lesson_choose "key_lesson_choose"
#define max_active_lesson_play 3
#define rate_time_expire "rate_time_expire"
#define time_bonus_rate 2

#define key_percent_action_played "key_percent_action_played"
#define key_sent_downloaded "key_sent_downloaded"
using namespace cocos2d;
using namespace std;

class StoryFreeManager {
public:
    static StoryFreeManager * getInstance();
    static StoryFreeManager * __instance;
    
    bool isFirst2Days();
    bool isUpdate3Days();
    ValueVector getListFreeStoriesOfDay(int lang_id);
    void setListFreeStoriesOfDay(ValueMap free_stories);
    void setListFreeStoriesOfDay(std::unordered_map<int, std::vector<int>> free_stories);
    bool checkFreeOfDayStoryCanRead(int _id);
    bool checkRateStoryCanRead();
    bool checkCanRead(int _id);
    void setReadFreeStory(int _id);
    bool checkStoryOfListRateBonus(int _id);
    
    void addFreeStoryRecentlyRead(int _id);
    ValueVector getFreeStoryRecentlyRead();
    void addRateStoryRead(int _id);
    
    bool isActionFreeTimeCanPlay(int _id);
    bool checkFreeTimeAvailable();
    bool compareFreeTimeRead(int time_expire, int time_current_day);
    void setActionIDFreeTimePlayed(int _id);
    
    ValueVector getFreeTimeListStoryRead();
    void clearFreeTimeListStoryRead();
    
    int getTotalNumPageRead();
    void setTotalNumPageRead();
    void clearNumPageFreeRead();
    
    int getStoryNumpageRecentRead(int story_id);
    void setStoryIDRecentRead(int story_id, int page_id, int total_page, std::string action_type);
    
    ValueMap map_story_recent;
    
    int total_numpage_read = 0;
    bool checkReadOnlyOneStory(int story_id);
    
    int freetime_fake_count = 0;
    
    bool getFreeActionAvailableWithType(std::string action_type);
    void setFreeActionPlay(std::string action_type);
    
    //Audio_book
    void setFreeAudioBookTimeRead(float time_read);
    void setListFreeAudioBook(std::unordered_map<int, std::vector<int>> free_audio_book);
    ValueVector getListFreeAudioBookOfDay(int lang_id);
    void setFreeAudioChoose(int audio_book_id);
    int getFreeAudioBookChoose();
    void setAudioMapRecent(int audio_book_id, float duration, float total_time);
    float getAudioMapRecent(int audio_book_id);
    ValueMap map_audibook_recent;
    bool isAudioBookFreePlayAvailable(int audio_book_id);

    //lesson
    void setNumFreeActiPlayed();
    int getNumFreeActiCanPlay();
    void setNumActiFreeFromServer(int num_active);
    int getNumActiFreeFromServer();
    void setActiviChoose();
    int getMaxActiFreeCanPlay();
    
    void resetAllFreeKeyOnNewDay();
    
    //rate_with_time_expire
    bool getRateCanReadWithTimeExpire();
    void setRateTimeExpire(int value);
    
    void setPercentActionFreePlayed(float per);
    float getPercentAvaialbleActionFreePlayed(std::string action_type = "");
    
    bool isAudiobookOnListFree(int book_id);
    static int is_sent_freeuse_toserver;
    static int is_freetime_use;
    bool isStoryOnListFree(int _id);
};

#endif
