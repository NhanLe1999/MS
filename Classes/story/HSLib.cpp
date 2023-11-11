#include "HSLib.h"
#include "MJ_PageLoadResource.h"
#include "GlobalConstants.h"
#include "NativeBridge.h"
#include "StoryDataModel.h"
int HSLib::pageIndex = 1;
int HSLib::pageNumAll = 1;
int HSLib::wbackground = 1;
int HSLib::hbackground = 1;
int HSLib::story_play_type = 1;
int HSLib::story_id = 0;
int HSLib::lang_id = 1;
string HSLib::story_fontname = "";
bool HSLib::isGetReadingLelvelDone = false;
bool HSLib::isSelectFavoriteEnable = false;
MJHomePage HSLib::home_page = MJ_HOME_PAGE;
int HSLib::pop_scene_index = 0;
int HSLib::level_current = 0;
int HSLib::_id = 0;
bool HSLib::isEndingPageClick = false;
bool HSLib::isofflinemode_enable = false;
bool HSLib::isPopupSubscribeNow = false;
int HSLib::keyboard_index = 0;
bool HSLib::enable_keyboard = true;//
bool HSLib::show_popup_subscribe = false;
ValueVector HSLib::list_data;
cocos2d::ValueMap HSLib::_list_bridge_text;
cocos2d::Size HSLib::sizeDisplay = cocos2d::Size(1024, 768);
std::string HSLib::storythumb = "storythumb";
std::string HSLib::fontNameStory = "";

HSLib * HSLib::__instance = NULL;

std::vector<std::string> HSLib::anims = {};


HSLib * HSLib::getInstance() {
    if (!__instance) {
        __instance = new HSLib();
        sizeDisplay = cocos2d::Director::getInstance()->getVisibleSize();
    }
    return __instance;
}

cocos2d::Size HSLib::getSizeScreen() {
    return cocos2d::Director::getInstance()->getVisibleSize();
}

cocos2d::Size HSLib::getDeviceScreen(){
    return cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
}

#pragma mark - Format Number
std::string HSLib::formatNumber(int soXu) {
    std::string str;
    if (soXu > 1000000) {
        int soXuTrieu = soXu/1000000;
        int soXu2 = soXu - soXuTrieu*1000000;
        int soXuNghin = soXu2/1000;
        int soXuDonvi = soXu2 - soXuNghin*1000;
        std::string strSoNghin =  gatSo0(soXuNghin);
        std::string strSoDV = gatSo0(soXuDonvi);
        str =  cocos2d::StringUtils::format("%d,%s,%s", soXuTrieu, strSoNghin.c_str(), strSoDV.c_str());
    }
    else if (soXu > 1000){
        int soXuNghin = soXu/1000;
        int soXuDonvi = soXu - soXuNghin*1000;
        std::string strSoDV = gatSo0(soXuDonvi);
        str =  cocos2d::StringUtils::format("%d,%s", soXuNghin,strSoDV.c_str());
    }
    else {
        str = cocos2d::StringUtils::format("%d", soXu);
    }
    return str;
}

std::string HSLib::gatSo0(int So) {
    std::string hs9;
    if (So < 10) {
        hs9 = cocos2d::StringUtils::format("00%d", So);
    }
    else if (So < 100) {
        hs9 = cocos2d::StringUtils::format("0%d", So);
    }
    else {
        hs9 = cocos2d::StringUtils::format("%d", So);
    }
    return hs9;
}


std::string HSLib::formatCoinForRoom(int soxu){
    std::string str;
    if (soxu >= 10000) {
        int soK = soxu/1000;
        if (soK > 1000) {
            str =  cocos2d::StringUtils::format("%sK", this->formatNumber(soK).c_str());
        }
        else {
            str =  cocos2d::StringUtils::format("%dK", soK);
        }
    }
    else {
       str = this->formatNumber(soxu);
    }
    return str;
}


std::string HSLib::getFilePathStory(std::string inputPath, bool isDocument){
    if (inputPath == std::string("")) return inputPath;
    if (isDocument) {
        return FileUtils::getInstance()->getWritablePath() + std::string("mjstory/") + inputPath;
    }else{
        return std::string("mjstory/") + inputPath;
    }
}

std::string HSLib::mahoaString(std::string str){
    std::string key = "B0-10-41-3E-5A-91";
    std::vector<unsigned char> key_data(key.begin(), key.end());
    std::vector<unsigned char> value_data(str.begin(), str.end());
    std::vector<unsigned char> encrypted_data;
    Aes256::encrypt(key_data, value_data, encrypted_data);
    std::string encrypted_string(encrypted_data.begin(), encrypted_data.end());
    str = base64::encode(encrypted_string);
    return str;
}

std::string HSLib::giaimaString(std::string str){
    std::string key = "B0-10-41-3E-5A-91";
    str = base64::decode(str);
    std::vector<unsigned char> key_data(key.begin(), key.end());
    std::vector<unsigned char> encrypted_data(str.begin(), str.end());
    std::vector<unsigned char> decrypted_data;
    Aes256::decrypt(key_data, encrypted_data, decrypted_data);
    std::string decrypted_string(decrypted_data.begin(), decrypted_data.end());
    return decrypted_string;
}

void HSLib::setValueMapForKey(cocos2d::ValueMap valuemap, std::string key){
    string packageStr = JsonDump::getInstance()->MapToString(valuemap);
    MJDEFAULT->setStringForKey(key.c_str(), packageStr);
}

cocos2d::ValueMap HSLib::getValueMapForKey(std::string key){
    string packageStr = MJDEFAULT->getStringForKey(key.c_str());
    return JsonDump::getInstance()->dumpValueMapFromString(packageStr);
}

void HSLib::setValueVectorForKey(cocos2d::ValueVector value, std::string key){
    string packageStr = JsonDump::getInstance()->VectorToString(value);
    MJDEFAULT->setStringForKey(key.c_str(), packageStr);
}

cocos2d::ValueVector HSLib::getValueVectorForKey(std::string key){
    string packageStr = MJDEFAULT->getStringForKey(key.c_str());
    return JsonDump::getInstance()->dumpValueVectorFromString(packageStr);
    
}

void HSLib::getFontName(){
    fontNameStory = std::string("monkeystories_medium.ttf");
    CCLOG("get_font_name = %s", fontNameStory.c_str());
}

std::string HSLib::getNameLevelWithID(int level_id){
    auto str_level = string("All level");
    switch (level_id) {
        case 1:
            str_level = string("A");
            break;
        case 2:
            str_level = string("B");
            break;
        case 3:
            str_level = string("C");
            break;
        case 4:
            str_level = string("D");
            break;
        case 5:
            str_level = string("E");
            break;
        case 6:
            str_level = string("F");
            break;
        case 7:
            str_level = string("G");
            break;
        case 8:
            str_level = string("H");
            break;
        case 9:
            str_level = string("I");
            break;
        case 10:
            str_level = string("J");
            break;
        case 11:
            str_level = string("K");
            break;
        case 12:
            str_level = string("L");
            break;
        case 13:
            str_level = string("M");
            break;
        case 14:
            str_level = string("N");
            break;
        case 15:
            str_level = string("O");
            break;
            
        default:
            break;
    }
    return str_level;
}

std::string HSLib::getNameLangWithID(int lang_id){
    auto str_lang = string("English US");
    if (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 1) {
        switch (lang_id) {
            case 1:
                str_lang = string("English US");
                break;
            case 4:
                str_lang = string("Vietnamese");
                break;
            case 12:
                str_lang = string("English UK");
                break;
            default:
                break;
        }
    }else if (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4){
        str_lang = string("Tiếng Anh");
        switch (lang_id) {
            case 1:
                str_lang = string("Tiếng Anh");
                break;
            case 4:
                str_lang = string("Tiếng Việt");
                break;
            default:
                break;
        }
    }
        return str_lang;
}



void HSLib::addStoryToRecentlyRead(int _id){
    auto maxItem = 15;//chỉ lưu 15 truyện đọc gần nhất.
    auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    auto list_recentlyread = getStoryRecentlyRead();
    
    for (int i = 0; i < (int)list_recentlyread.size(); i++) {
        if (_id == list_recentlyread[i].asInt() ) {
            list_recentlyread.erase(list_recentlyread.begin()+i);
            break;
        }
    }
    
    if ((int)list_recentlyread.size() >= maxItem) {
        list_recentlyread.erase(list_recentlyread.begin()+maxItem-1);
    }
    
    list_recentlyread.insert(list_recentlyread.begin(), Value(_id));
    MJDEFAULT->setStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang), JsonDump::getInstance()->VectorToString(list_recentlyread));
    
    
    //add vào list - lưu lại tất cả truyện đã đọc.
    ValueVector list_dadoc;
    auto str_list_dadoc = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang));
    if (str_list_dadoc != "") {
        list_dadoc = JsonDump::getInstance()->dumpValueVectorFromString(str_list_dadoc);
    }
    
    for (int i = 0; (int)i < list_dadoc.size(); i++) {
        if (_id == list_dadoc[i].asInt()) {
            list_dadoc.erase(list_dadoc.begin()+i);
            break;
        }
    }
    list_dadoc.push_back(Value(_id));
    MJDEFAULT->setStringForKey(StringUtils::format("%s%d", key_liststories_docroi, current_lang), JsonDump::getInstance()->VectorToString(list_dadoc));
}

ValueVector HSLib::getStoryRecentlyRead(){
    ValueVector re_list;
    auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    auto str_recentlyread = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_stories_recentlyread, current_lang));
    if (str_recentlyread != "") {
        re_list = JsonDump::getInstance()->dumpValueVectorFromString(str_recentlyread);
    }
    return re_list;
}


void HSLib::addAudioToRecentlyRead(int _id){
    auto maxItem = 15;//chỉ lưu 15 truyện đọc gần nhất.
    auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    auto list_recentlyread = getAudioRecentlyRead();
    
    for (int i = 0; i < (int)list_recentlyread.size(); i++) {
        if (_id == list_recentlyread[i].asInt() ) {
            list_recentlyread.erase(list_recentlyread.begin()+i);
            break;
        }
    }
    
    if ((int)list_recentlyread.size() >= maxItem) {
        list_recentlyread.erase(list_recentlyread.begin()+maxItem-1);
    }
    
    list_recentlyread.insert(list_recentlyread.begin(), Value(_id));
    MJDEFAULT->setStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang), JsonDump::getInstance()->VectorToString(list_recentlyread));
    
    
    //add vào list - lưu lại tất cả audio đã đọc.
    ValueVector list_dadoc;
    auto str_list_dadoc = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang));
    if (str_list_dadoc != "") {
        list_dadoc = JsonDump::getInstance()->dumpValueVectorFromString(str_list_dadoc);
    }
    
    for (int i = 0; (int)i < list_dadoc.size(); i++) {
        if (_id == list_dadoc[i].asInt()) {
            list_dadoc.erase(list_dadoc.begin()+i);
            break;
        }
    }
    list_dadoc.push_back(Value(_id));
    MJDEFAULT->setStringForKey(StringUtils::format("%s%d", key_listaudiobook_docroi, current_lang), JsonDump::getInstance()->VectorToString(list_dadoc));
}
ValueVector HSLib::getAudioRecentlyRead(){
    ValueVector re_list;
    auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
    auto str_recentlyread = MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_audio_recentlyread, current_lang));
    if (str_recentlyread != "") {
        re_list = JsonDump::getInstance()->dumpValueVectorFromString(str_recentlyread);
    }
    return re_list;
}

void HSLib::setNewStory(ValueVector list_id, int lang_id){
    CCLOG("setNewStory%d %s", lang_id, JsonDump::getInstance()->VectorToString(list_id).c_str());
    MJDEFAULT->setStringForKey(StringUtils::format("%s_%d", key_new_stories, lang_id), JsonDump::getInstance()->VectorToString(list_id));
}
ValueVector HSLib::getNewStory(){
    auto str_list = MJDEFAULT->getStringForKey(StringUtils::format("%s_%d", key_new_stories, MJDEFAULT->getIntegerForKey(key_current_lang)));
    return JsonDump::getInstance()->dumpValueVectorFromString(str_list);
}

void HSLib::setPopularStory(ValueVector list_id, int lang_id){
    CCLOG("setPopularStory%d %s", lang_id, JsonDump::getInstance()->VectorToString(list_id).c_str());
    MJDEFAULT->setStringForKey(StringUtils::format("%s_%d", key_popular_stories, lang_id), JsonDump::getInstance()->VectorToString(list_id));
}

ValueVector HSLib::getPopularStory(){
    auto str_list = MJDEFAULT->getStringForKey(StringUtils::format("%s_%d", key_popular_stories, MJDEFAULT->getIntegerForKey(key_current_lang)));
    return JsonDump::getInstance()->dumpValueVectorFromString(str_list);
}

void HSLib::addRateStory(int _id, int rate_vl){
    auto list_rate = getRateStory();
    ValueMap map;
    map["rate"] = rate_vl;
    map["id"] = _id;
    list_rate.push_back(Value(map));
    MJDEFAULT->setStringForKey(key_ratestories, JsonDump::getInstance()->VectorToString(list_rate));
}
ValueVector HSLib::getRateStory(){
    auto str_list = MJDEFAULT->getStringForKey(key_ratestories);
    return JsonDump::getInstance()->dumpValueVectorFromString(str_list);
}

void HSLib::addFavoritesStory(int _id){
    int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
    auto maxItem = max_number_favorite;
    auto list_favorites = getFavoritesStory();
    bool is_find = false;

    for (int i = 0; i < (int) list_favorites.size(); i++) {
        if (list_favorites[i].asInt() == _id) {
            is_find = true;
        }
    }
    
    if (!is_find) {
        if ((int)list_favorites.size() >= maxItem) {
            list_favorites.erase(list_favorites.begin()+maxItem-1);
        }
        list_favorites.insert(list_favorites.begin(), Value(_id));
        
        auto key = key_favorites + StringUtils::toString(lang_id);
        MJDEFAULT->setStringForKey(key, JsonDump::getInstance()->VectorToString(list_favorites));
    }
}

void HSLib::removeFavoritesStory(int _id){
    int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
    auto list_favorites = getFavoritesStory();
    for (int i = 0; i < (int) list_favorites.size(); i++) {
        if (_id == list_favorites[i].asInt()) {
            list_favorites.erase(list_favorites.begin()+i);
//            StoryDataModel::getInstance()->setStoryRead(_id, 1, 1);
//            mj::PageLoadResource::getInstance().deleteCacheOfStory(_id);
            break;
        }
    }
    auto key = key_favorites +  StringUtils::toString(lang_id);
    MJDEFAULT->setStringForKey(key, JsonDump::getInstance()->VectorToString(list_favorites));
}

ValueVector HSLib::getFavoritesStory(){
    int lang_id = MJDEFAULT->getIntegerForKey(key_current_lang);
    auto key = key_favorites + StringUtils::toString(lang_id);
    auto str_list = MJDEFAULT->getStringForKey(key);
    if (lang_id == 1 && str_list == "") {
        str_list = MJDEFAULT->getStringForKey(key);
    }
    return JsonDump::getInstance()->dumpValueVectorFromString(str_list);
}

bool HSLib::isFavoritesStory(int _id){
    auto list_favorites = getFavoritesStory();
    for (int i = 0; i < (int) list_favorites.size(); i++) {
        if (_id == list_favorites[i].asInt()) {
            return true;
        }
    }
    return false;
}

#pragma mark - MESSAGE
void HSLib::addMessageRead(ValueVector list_message){
    MJDEFAULT->setStringForKey(key_listmessageread, JsonDump::getInstance()->VectorToString(list_message));
}

bool HSLib::checkMessageUnRead(int id_message){
    auto list_message_read = JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(key_listmessageread));
    for (int i = 0; i < (int)list_message_read.size(); i++) {
        if (id_message == list_message_read[i].asInt()) {
            return false;
        }
    }
    return true;
}
