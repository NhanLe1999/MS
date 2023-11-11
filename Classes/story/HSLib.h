#ifndef __HSLib__
#define __HSLib__
#define XHSLib HSLib::getInstance()

#include <stdio.h>
#include "cocos2d.h"
#include "../mjlib/Common.h"
#include "aes256.h"
#include "base64.h"
#include "JsonDump.h"
#include "HSAudioEngine.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"

enum MJHomePage {
    MJ_HOME_PAGE,
    MJ_EXPLORE_PAGE,
    MJ_FAVORITE_PAGE,
    MJ_FREE_PAGE,
    MJ_LESSON_PAGE,
    MJ_SUBSCRIPTION_PAGE,
    MJ_PARENT_PAGE
};

class HSLib {
public:
    static HSLib * getInstance();
    static HSLib * __instance;
    cocos2d::Size getSizeScreen();
    cocos2d::Size getDeviceScreen();
    static cocos2d::Size sizeDisplay;
    
    std::string formatNumber(int soxu);
    std::string gatSo0(int So);
    
    std::string formatCoinForRoom(int soxu);
    
    static int pageIndex;
    static int pageNumAll;
    static std::string storythumb;
    
    static int wbackground;
    static int hbackground;
    static int story_play_type;
    static int story_id, _id;
    static int lang_id;
    
    static string story_fontname;
    static bool isSelectFavoriteEnable;
    static cocos2d::ValueMap _list_bridge_text;
    
    std::string getFilePathStory(std::string inputPath, bool isDocument = true);
    static std::string fontNameStory;
    static MJHomePage home_page;
    
    enum STORY_TYPE {
        SIMPLE_TYPE = 1,
        ADVANCED_TYPE = 2,
    };
    
    enum STORY_PLAY {
        READTOME = 1,
        AUTOPLAY = 2,
        READITMYSELF = 3,
    };
    
    std::string mahoaString(std::string str);
    std::string giaimaString(std::string str);
    
    
    void getFontName();
    
    cocos2d::ValueMap getValueMapForKey(std::string key);
    void setValueMapForKey(cocos2d::ValueMap valuemap, std::string key);

    cocos2d::ValueVector getValueVectorForKey(std::string key);
    void setValueVectorForKey(cocos2d::ValueVector value, std::string key);
    
    std::string getNameLevelWithID(int level_id);
    std::string getNameLangWithID(int lang_id);
    
    void addStoryToRecentlyRead(int _id);
    ValueVector getStoryRecentlyRead();
    
    void setNewStory(ValueVector list_id, int lang_id);
    ValueVector getNewStory();
    
    void setPopularStory(ValueVector list_id, int lang_id);
    ValueVector getPopularStory();
    
    static int pop_scene_index, level_current;
    static bool isEndingPageClick, isPopupSubscribeNow;
    
    static std::vector<std::string> anims;
    
    void addRateStory(int _id, int rate_vl);
    ValueVector getRateStory();
    
    void addFavoritesStory(int _id);
    void removeFavoritesStory(int _id);
    ValueVector getFavoritesStory();
    bool isFavoritesStory(int _id);
    
    
    static ValueVector list_data;
    
    void addMessageRead(ValueVector list_message);
    bool checkMessageUnRead(int id_message);
    static bool isofflinemode_enable, isGetReadingLelvelDone;
    
    static int keyboard_index;
    static bool enable_keyboard;
    static bool show_popup_subscribe;
    
    void addAudioToRecentlyRead(int _id);
    ValueVector getAudioRecentlyRead();
    
};

#endif
