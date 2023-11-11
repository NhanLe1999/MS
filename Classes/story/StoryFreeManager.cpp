#include "StoryFreeManager.h"
#include "JsonDump.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

int StoryFreeManager::is_sent_freeuse_toserver = false;
int StoryFreeManager::is_freetime_use = false;
StoryFreeManager * StoryFreeManager::__instance = NULL;
StoryFreeManager * StoryFreeManager::getInstance() {
    if (!__instance) {
        __instance = new StoryFreeManager();
    }
    return __instance;
}


ValueVector StoryFreeManager::getListFreeStoriesOfDay(int lang_id){
    CCLOG("%s", MJDEFAULT->getStringForKey(StringUtils::format("%s%d", klist_stories_free, lang_id)).c_str());
    return JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(StringUtils::format("%s%d", klist_stories_free, lang_id)));
}
void StoryFreeManager::setListFreeStoriesOfDay(ValueMap free_stories){
    MJDEFAULT->setIntegerForKey(key_storyfree_choose, 0);
    MJDEFAULT->setBoolForKey(key_storyfree_read, false);
    
    for (auto item:free_stories) {
        if (item.second.getType() == Value::Type::VECTOR) {
            CCLOG("%s", StringUtils::format("%s%s", klist_stories_free, item.first.c_str()).c_str());
            MJDEFAULT->setStringForKey(StringUtils::format("%s%s", klist_stories_free, item.first.c_str()), JsonDump::getInstance()->VectorToString(item.second.asValueVector()));
        }
    }
}

void StoryFreeManager::setListFreeStoriesOfDay(std::unordered_map<int, std::vector<int>> free_stories){
    MJDEFAULT->setIntegerForKey(key_storyfree_choose, 0);
    MJDEFAULT->setBoolForKey(key_storyfree_read, false);

    for (auto item : free_stories) {
        CCLOG("%s", StringUtils::format("%s%d", klist_stories_free, item.first).c_str());

        MJDEFAULT->setStringForKey(
                                   StringUtils::format("%s%d", klist_stories_free, item.first),
                                   mj::helper::vectorIntToString(item.second));
    }
}

bool StoryFreeManager::checkFreeOfDayStoryCanRead(int _id){
    auto list_stories_free = getListFreeStoriesOfDay(MJDEFAULT->getIntegerForKey(key_current_lang, 1));
    for (int i = 0; i < (int) list_stories_free.size(); i++) {
        if (_id == list_stories_free[i].asInt()) {
            return true;
        }
    }
    return false;
}

bool StoryFreeManager::checkRateStoryCanRead(){
    CCLOG("check num_rate = %d", MJDEFAULT->getIntegerForKey(key_numstories_rate_bonus));
    if (MJDEFAULT->getBoolForKey(key_rate_app_won) && MJDEFAULT->getIntegerForKey(key_numstories_rate_bonus) > 0) {
        return true;
    }
    return false;
}

bool StoryFreeManager::checkStoryOfListRateBonus(int _id){
    auto list_stories = JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(key_rate_list_stories));
    for (int i = 0; i < (int) list_stories.size(); i++) {
        if (_id == list_stories[i].asInt()) {
            return true;
        }
    }
    return false;
}


bool StoryFreeManager::checkCanRead(int _id){
    if (checkFreeOfDayStoryCanRead(_id)) {
        return true;
    }
    if (checkRateStoryCanRead()) {
        return true;
    }
    return false;
}

void StoryFreeManager::setReadFreeStory(int _id){
    if (MJDEFAULT->getIntegerForKey(key_storyfree_choose) == 0) {
        MJDEFAULT->setIntegerForKey(key_storyfree_read, true);
        MJDEFAULT->setIntegerForKey(key_storyfree_choose, _id);
        FREE_STORY_MANAGER->addFreeStoryRecentlyRead(_id);
    }
    
//    if (checkFreeOfDayStoryCanRead(_id)) {
//        MJDEFAULT->setBoolForKey(key_storyfree_read, true);
//        MJDEFAULT->setIntegerForKey(key_storyfree_read, _id);
//        return;
//    }
//    
//    if (checkRateStoryCanRead()){
//        auto num_rate = MJDEFAULT->getIntegerForKey(key_numstories_rate_bonus);
//        num_rate--;
//        MJDEFAULT->setIntegerForKey(key_numstories_rate_bonus, num_rate);
//    }
}

//theem box chon

void StoryFreeManager::addFreeStoryRecentlyRead(int _id){
    auto list_recently = getFreeStoryRecentlyRead();
    for (int i = 0; i < (int) list_recently.size(); i++) {
        if (_id == list_recently[i].asInt()) {
            list_recently.erase(list_recently.begin() + i);
            break;
        }
    }
    list_recently.insert(list_recently.begin(), Value(_id));
    MJDEFAULT->setStringForKey(key_storyfree_recently, JsonDump::getInstance()->VectorToString(list_recently));
}
ValueVector StoryFreeManager::getFreeStoryRecentlyRead(){
    return JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(key_storyfree_recently));
}

void StoryFreeManager::addRateStoryRead(int _id){
    auto num_rate_bonus = MJDEFAULT->getIntegerForKey(key_numstories_rate_bonus);
    num_rate_bonus--;
    MJDEFAULT->setIntegerForKey(key_numstories_rate_bonus, num_rate_bonus);
}


//FreeTime
bool StoryFreeManager::isActionFreeTimeCanPlay(int _id){
    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
    if (CONFIG_MANAGER->isTestFreeTimeFake()) {
        time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
    }
    auto date_current = LTDate(time_current_day);
    auto key = StringUtils::format("%s_%d_%d_%d", key_freetime_read, date_current.year, date_current.month, date_current.day);
    CCLOG("key = %s", key.c_str());
    if (MJDEFAULT->getIntegerForKey(key, 0) == 0) {// = 0 : ngày này chưa đọc truyện.
        return true;
    }
    return false;
}

bool StoryFreeManager::checkFreeTimeAvailable(){
    auto time_current = MJDEFAULT->getIntegerForKey(key_time_curent);
    time_current = std::max(time_current, int(utils::getTimeInMilliseconds() / 1000));

    auto time = MJDEFAULT->getIntegerForKey(key_freetime_expire);
    
    if (time_current < MJDEFAULT->getIntegerForKey(key_freetime_expire)) {
        return true;
    }else return false;
}

bool StoryFreeManager::compareFreeTimeRead(int time_expire, int time_current_day){
    if (CONFIG_MANAGER->isTestFreeTimeFake()) {
        time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
    }
    auto date_expire = LTDate(time_expire);
    auto date_current = LTDate(time_current_day);
    CCLOG("compareFreeTimeRead: current = %s   --vs--   %s", date_current.toString().c_str(), date_expire.toString().c_str());
    auto value_curent = date_current.year*10000+date_current.month*100+date_current.day;
    auto value_expire = date_expire.year*10000+date_expire.month*100+date_expire.day;
    CCLOG("compareFreeTimeRead: current = %d   --vs--   %d", value_curent, value_expire);
    if (value_curent > value_expire) return false;
    CCLOG("compareFreeTimeRead = Ok");
    return true;
}

void StoryFreeManager::setActionIDFreeTimePlayed(int _id){
    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
    if (CONFIG_MANAGER->isTestFreeTimeFake()) {
        time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
    }
    auto date_current = LTDate(time_current_day);
    auto key = StringUtils::format("%s_%d_%d_%d", key_freetime_read, date_current.year, date_current.month, date_current.day);
    CCLOG("key = %s", key.c_str());
    MJDEFAULT->setIntegerForKey(key, _id);
}
ValueVector StoryFreeManager::getFreeTimeListStoryRead(){
    return JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(klist_story_freetime));
}

void StoryFreeManager::clearFreeTimeListStoryRead(){
    MJDEFAULT->setStringForKey(key_storyfree_recently, "");
}


int StoryFreeManager::getTotalNumPageRead(){
    total_numpage_read = 0;
    for (auto item:map_story_recent) {
        total_numpage_read += item.second.asInt();
    }
    return total_numpage_read;
}

void StoryFreeManager::setTotalNumPageRead(){
    total_numpage_read++;
}

void StoryFreeManager::clearNumPageFreeRead(){
    MJDEFAULT->setIntegerForKey(key_numpage_read, 0);
}

int StoryFreeManager::getStoryNumpageRecentRead(int story_id){
    return map_story_recent[StringUtils::toString(story_id)].asInt();
}
void StoryFreeManager::setStoryIDRecentRead(int story_id, int page_id, int total_page, std::string action_type){
    if (page_id > getStoryNumpageRecentRead(story_id)) {
        map_story_recent[StringUtils::toString(story_id)] = page_id;
        if (action_type == ACTION_STORY){
            CCLOG("page_id = %d | total_page = %d", page_id, total_page);
            if (this->checkFreeTimeAvailable() || this->isStoryOnListFree(story_id)) {
                this->setPercentActionFreePlayed(100.0/total_page);
            }
        }
    }
}

bool StoryFreeManager::checkReadOnlyOneStory(int story_id){
    auto size_map = (int)map_story_recent.size();
    if (size_map < 1) return true;
    if (size_map == 1 && !map_story_recent[StringUtils::toString(story_id)].isNull()) return true;
    return false;
}

#pragma mark - ACTION_FREE_PLAY_CHECK
#include "MJAPISendDownload.h"
void StoryFreeManager::setPercentActionFreePlayed(float per){
    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
    time_current_day = std::max(time_current_day, int(utils::getTimeInMilliseconds() / 1000));
    if (CONFIG_MANAGER->isTestFreeTimeFake()) {
        time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
    }
    auto date_current = LTDate(time_current_day);
    auto key = StringUtils::format("%s_%d_%d_%d", key_percent_action_played, date_current.year, date_current.month, date_current.day);
    auto per_current = MJDEFAULT->getFloatForKey(key);
    if (per_current >= 100.0f) per_current = 100;
    if (per_current < 0) per_current = 0;
    MJDEFAULT->setFloatForKey(key, per+per_current);
    
    //is_freetime_use = (MJDEFAULT->getIntegerForKey(key_time_curent) < MJDEFAULT->getIntegerForKey(key_freetime_expire));
    if (!is_sent_freeuse_toserver && per_current > 90.0f) {
        mj::SentDownloadAPI::call(100, 2, "");
        is_sent_freeuse_toserver = true;
//        auto key_downloaded = StringUtils::format("%s_%d_%d_%d", key_sent_downloaded, date_current.year, date_current.month, date_current.day);
//        MJDEFAULT->setBoolForKey(key_downloaded, is_sent_freeuse_toserver);
    }
    
//    CCLOG("set percent = %f", per+per_current);
}
float StoryFreeManager::getPercentAvaialbleActionFreePlayed(std::string action_type){
    if (action_type == ACTION_LESSON){
        auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
            time_current_day = std::max(time_current_day, int(utils::getTimeInMilliseconds() / 1000));
            if (CONFIG_MANAGER->isTestFreeTimeFake()) {
                time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
            }
            auto date_current = LTDate(time_current_day);
            auto key = StringUtils::format("%s_%d_%d_%d", key_percent_action_played, date_current.year, date_current.month, date_current.day);
            CCLOG("set key = %s", key.c_str());
            float percent_avaiable = 100 - MJDEFAULT->getFloatForKey(key, 0);
        //    percent_avaiable = percent_avaiable;
            if (action_type == ACTION_LESSON){
                int max = getMaxActiFreeCanPlay();
                auto percent_compare = 100.0f/max;
                if (percent_avaiable < percent_compare*0.66) {
                    percent_avaiable = 0;
                }
            }
            return percent_avaiable;
    }else{
        return 100.f;
    }
    
}

bool StoryFreeManager::getFreeActionAvailableWithType(std::string action_type){
    if (action_type == ACTION_STORY){
        if (MJDEFAULT->getBoolForKey(key_audiobook_read)) return false;
        if (MJDEFAULT->getBoolForKey(key_lesson_read)) return false;
    }
    
    if (action_type == ACTION_LESSON){
        if (MJDEFAULT->getBoolForKey(key_storyfree_read)) return false;
        if (MJDEFAULT->getBoolForKey(key_audiobook_read)) return false;
    }
    
    if (action_type == ACTION_AUDIOBOOK){
        if (MJDEFAULT->getBoolForKey(key_storyfree_read)) return false;
        if (MJDEFAULT->getBoolForKey(key_lesson_read)) return false;
    }
    
    return true;
}

void StoryFreeManager::setFreeActionPlay(std::string action_type){
    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
    if (CONFIG_MANAGER->isTestFreeTimeFake()) {
        time_current_day = MJDEFAULT->getIntegerForKey(key_time_fake);
    }
    auto date_current = LTDate(time_current_day);
    auto key = StringUtils::format("%s_%d_%d_%d", ACTION_TYPE, date_current.year, date_current.month, date_current.day);
    CCLOG("set key = %s", key.c_str());
    MJDEFAULT->setStringForKey(key, action_type);
}

//Audio_book
void StoryFreeManager::setAudioMapRecent(int audio_book_id, float duration, float total_time){
    if (getAudioMapRecent(audio_book_id) < duration) {
        auto duration_add = duration - getAudioMapRecent(audio_book_id);
        this->setPercentActionFreePlayed(duration_add*1.0f/total_time*100);
//        CCLOG("audio percent = %f", duration_add*1.0f/total_time*100);
        map_audibook_recent[StringUtils::toString(audio_book_id)] = duration;
    }
}
float StoryFreeManager::getAudioMapRecent(int audio_book_id){
    return map_audibook_recent[StringUtils::toString(audio_book_id)].asFloat();
}

bool StoryFreeManager::isAudioBookFreePlayAvailable(int audio_book_id){
    auto list_audiobook_free = getListFreeStoriesOfDay(MJDEFAULT->getIntegerForKey(key_current_lang, 1));
    auto check = false;
    for (int i = 0; i < (int) list_audiobook_free.size(); i++){
        if (audio_book_id == list_audiobook_free[i].asInt()){
            check = true;
            break;
        }
    }
    return check;
}

bool StoryFreeManager::isStoryOnListFree(int _id){
    auto list_stories_free = getListFreeStoriesOfDay(MJDEFAULT->getIntegerForKey(key_current_lang));
    for (int i = 0; i < (int)list_stories_free.size(); i++) {
        CCLOG("list_stories_free = %d | _id = %d", list_stories_free[i].asInt(), _id);
        if (list_stories_free[i].asInt() == _id) {
            return true;
        }
    }
    return false;
}


void StoryFreeManager::setFreeAudioBookTimeRead(float time_read){
    time_read += MJDEFAULT->getFloatForKey(AUDIO_BOOK_TIMEREAD);
    MJDEFAULT->setFloatForKey(AUDIO_BOOK_TIMEREAD, time_read);
}

ValueVector StoryFreeManager::getListFreeAudioBookOfDay(int lang_id){
    return JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(StringUtils::format("%s%d", klist_audiobook_free, lang_id)));
}

bool StoryFreeManager::isAudiobookOnListFree(int book_id){
    auto list_audio_free = getListFreeAudioBookOfDay(MJDEFAULT->getIntegerForKey(key_current_lang));
    for (int i = 0; i < (int)list_audio_free.size(); i++) {
        if (list_audio_free[i].asInt() == book_id) {
            return true;
        }
    }
    return false;
}

void StoryFreeManager::setListFreeAudioBook(std::unordered_map<int, std::vector<int>> free_audio_book){
    setFreeAudioBookTimeRead(0);
    for (auto item : free_audio_book) {
        MJDEFAULT->setStringForKey(StringUtils::format("%s%d", klist_audiobook_free, item.first),mj::helper::vectorIntToString(item.second));
    }
}

void StoryFreeManager::setFreeAudioChoose(int audio_book_id){
    MJDEFAULT->setBoolForKey(key_audiobook_read, true);
    MJDEFAULT->setIntegerForKey(key_audiobook_choose, audio_book_id);
}

int StoryFreeManager::getFreeAudioBookChoose(){
    return MJDEFAULT->getIntegerForKey(key_audiobook_choose);
}

//lesson
void StoryFreeManager::setNumFreeActiPlayed(){
    auto num_choose = MJDEFAULT->getIntegerForKey(key_lesson_choose)+1;
    MJDEFAULT->setIntegerForKey(key_lesson_choose, num_choose);
}
int StoryFreeManager::getNumFreeActiCanPlay(){//return <= 0 là hết lượt chơi.
    if (!getFreeActionAvailableWithType(ACTION_LESSON)) return 0;
    if (this->checkFreeTimeAvailable()) {// = true : trong khoảng thời gian 7 ngày đầu
        return max_active_lesson_play - MJDEFAULT->getIntegerForKey(key_lesson_choose);
    }else {
        return getNumActiFreeFromServer() - MJDEFAULT->getIntegerForKey(key_lesson_choose);
    }
}
void StoryFreeManager::setNumActiFreeFromServer(int num_active){
    MJDEFAULT->setIntegerForKey(knum_lesson_free, num_active);
}
int StoryFreeManager::getNumActiFreeFromServer(){
    return MJDEFAULT->getIntegerForKey(knum_lesson_free);
}

void StoryFreeManager::setActiviChoose(){
    MJDEFAULT->setBoolForKey(key_lesson_choose, true);
}

void StoryFreeManager::resetAllFreeKeyOnNewDay(){
    //story
    MJDEFAULT->setIntegerForKey(key_storyfree_choose, 0);
    MJDEFAULT->setBoolForKey(key_storyfree_read, false);
    clearNumPageFreeRead();
    
    //audio_book
    MJDEFAULT->setIntegerForKey(key_audiobook_choose, 0);
    MJDEFAULT->setBoolForKey(key_audiobook_read, false);
    MJDEFAULT->setFloatForKey(AUDIO_BOOK_TIMEREAD, 0);
    
    //lesson
    MJDEFAULT->setBoolForKey(key_lesson_choose, false);
    MJDEFAULT->setIntegerForKey(key_lesson_choose, 0);
}

int StoryFreeManager::getMaxActiFreeCanPlay(){
    return max_active_lesson_play;
//    if (this->checkFreeTimeAvailable()) {// = true : trong khoảng thời gian 7 ngày đầu
//
//    }else {
//        return getNumActiFreeFromServer();
//    }
}

//rate_with_time_expire
bool StoryFreeManager::getRateCanReadWithTimeExpire(){
    auto time_current = MJDEFAULT->getIntegerForKey(key_time_curent);
    time_current = std::max(time_current, int(utils::getTimeInMilliseconds() / 1000));
    if (MJDEFAULT->getIntegerForKey(rate_time_expire) > time_current) {
        return true;
    } else return false;
}

void StoryFreeManager::setRateTimeExpire(int value){
    auto rate_expire = MJDEFAULT->getIntegerForKey(key_freetime_expire);
    auto time_server = MJDEFAULT->getIntegerForKey(key_time_curent);
    auto time_local = (int)(utils::getTimeInMilliseconds() / 1000);
    auto current_time = std::max(time_server, time_local);
    auto time_free_left = (rate_expire - current_time) - (24 * 60 * 60 * (7 - 2));
    time_free_left = std::max(time_free_left, 0);
    
    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
    time_current_day = std::max(time_current_day, int(utils::getTimeInMilliseconds() / 1000));
    auto rate_day = LTDate(time_current_day) + value*24*3600;
    auto t = rate_day.toTimeStruct();
    MJDEFAULT->setIntegerForKey(rate_time_expire, (int)mktime(&t) + time_free_left);
}

bool StoryFreeManager::isFirst2Days() {
    auto rate_expire = MJDEFAULT->getIntegerForKey(key_freetime_expire);
    auto time_server = MJDEFAULT->getIntegerForKey(key_time_curent);
    auto time_local = (int)(utils::getTimeInMilliseconds() / 1000);
    auto current_time = std::max(time_server, time_local);
    return (rate_expire - current_time >= 24 * 60 * 60 * (7 - 2));
}

bool StoryFreeManager::isUpdate3Days() {
	auto time_local = (int)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto current = MJDEFAULT->getIntegerForKey(key_time_curent, time_local);
    auto offline = MJDEFAULT->getIntegerForKey(key_time_offline_limit,300);
    auto last_time_update = std::max(current,time_local) - current;
//    return (time_local - last_time_update >= 24 * 60 * 60 * (7 - 3));
	CCLOG("Time local : %d | Time server : %d | Time offile: %d | Time last update : %d",time_local, current, offline, last_time_update);
    return (last_time_update > offline);
}
