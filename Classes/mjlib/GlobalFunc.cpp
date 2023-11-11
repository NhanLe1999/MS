//
//  GlobalFunc.cpp
//  MonkeyJunior
//
//  Created by developer on 3/10/16.
//
//

#include "NativeBridge.h"
#include "GlobalFunc.h"
#include "AppDelegate.h"
#include "JsonDump.h"
#include "StoryData.h"
#include "GlobalConstants.h"
#include "MJDefault.h"

GlobalFunc* GlobalFunc::globalFunc;

GlobalFunc* GlobalFunc::getInstance()
{
    if (globalFunc == nullptr) {
        globalFunc = new GlobalFunc();
    }
    return globalFunc;
}

int GlobalFunc::_currentIndexListenerKeyboard = 0;

bool GlobalFunc::testUSDevice() {
    return false;
}
bool GlobalFunc::testPhonic() {
    return false;
}
bool GlobalFunc::checkDevice() {
    return false;
}

string GlobalFunc::urlEncode(string &s)
{
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(int i=0, ix = s.length(); i < ix; i++) {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
            (65 <= c && c <= 90) ||//abc...xyz
            (97 <= c && c <= 122) || //ABC...XYZ
            (c=='-' || c=='_' || c=='.' || c=='~')
            ) {
            e << c;
        } else {
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    s = e.str();
    return s;
}

/*
 chia mang ra thanh cac mang con
 */
ValueVector GlobalFunc::chunkValueVector(ValueVector array, int offset) {
    ValueVector arrayData;
    ValueVector rows;
    for(auto g : array) {
        if(rows.size() == offset) {
            arrayData.push_back(Value(rows));
            rows.clear();
        }
        ValueMap item = g.asValueMap();
        rows.push_back(Value(item));
    }
    if(rows.size() > 0) {
        arrayData.push_back(Value(rows));
        rows.clear();
    }
    return arrayData;
}

std::string GlobalFunc::trim(std::string& str, string character)
{
    str.erase(0, str.find_first_not_of(character));       //prefixing spaces
    str.erase(str.find_last_not_of(character)+1);         //surfixing spaces
    return str;
}

void GlobalFunc::insertDatabase() {
}

string GlobalFunc::buildImagePath(string img, string ext) {
    int device_type = StoryData::getInstance()->deviceType;
    string image_weight = "medium";
    if(device_type == 4) {
        image_weight = "large";
    }
    string strImg = StringUtils::format("%s_%s.%s",img.c_str(),image_weight.c_str(),ext.c_str());
    return strImg;
}

//dao lung tung du lieu cua 1 vector
void GlobalFunc::shuffleArray(ValueVector &vector) {
    ValueVector temp;
    while (vector.size() > 0) {
        int total = vector.size();
        int index = random<int>(0, total-1);
        temp.push_back(Value(vector.at(index)));
        vector.erase(vector.begin() + index);
    }
    vector = temp;
}

void GlobalFunc::reverseArray(ValueVector &vector) {
    int total = vector.size();
    if(total == 0) {
        return;
    }
    ValueVector temp;
    for (int i = total - 1; i >= 0; i--) {
        temp.push_back(Value(vector.at(i)));
    }
    vector = temp;
}


/*
 neu la hd => tra ve duong dan tai thu muc hd
 neu la hdr => tra ve duong dan tai thu muc hdr
 */
std::string GlobalFunc::fileInResourse(string fileName) {
    if(StoryData::getInstance()->isHdr) {
        return "hdr/" + fileName;
    }
    return "hd/" + fileName;
}

string GlobalFunc::getCurrentLessonId() {
    return "";
}

//MARK: cau hinh theo user
//bai hoc
bool GlobalFunc::configLessonAuto() {
    return true;
}
bool GlobalFunc::configLessonChildlock() {
    return true;
}
//ngu am
bool GlobalFunc::configPhonicVisible() {
    return true;
    
}
bool GlobalFunc::configPhonicHighlight() {
    return true;
    
}
bool GlobalFunc::configPhonicFast() {
    return true;
}
//trao the
float GlobalFunc::configFastCardDelayTime() {
    return 1;
}

int GlobalFunc::configFastCardType() {
    return 1;
}

bool GlobalFunc::configFastCardAudio() {
    return true;
}
//cap do kho
bool GlobalFunc::configSentenceImageVisible() {
    return true;
}
//nhac hoc
bool GlobalFunc::configPointerDisplay() {
    return true;
}

//MARK: Seter
//bai hoc
void GlobalFunc::configSetLessonAuto(bool vl) {
    
}
void GlobalFunc::configSetLessonChildlock(bool vl) {
}
//ngu am
void GlobalFunc::configSetPhonicVisible(bool vl) {
}
void GlobalFunc::configSetPhonicFast(bool vl) {
}

void GlobalFunc::configSetPhonicHighlight(bool vl) {
}

//trao the
void GlobalFunc::configSetFastCardDelayTime(float vl) {
    
}

void GlobalFunc::configSetFastCardType(int vl) {
    //neu khac gia tri hien tai thi reset lesson index
    
}

void GlobalFunc::configSetFastCardAudio(bool vl) {
    
}
//cap do kho
void GlobalFunc::configSetSentenceImageVisible(bool vl) {
}
//nhac hoc
void GlobalFunc::configSetPointerDisplay(bool vl) {
}

//MARK: ngon ngu

vector<int> GlobalFunc::getComingSoonLanguage() {
    vector<int> arrLanguage = GlobalFunc::getLanguageAvaiable();
    vector<int> comingsoon;
    string str = MJDEFAULT->getStringForKey("language_coming_soon");
    if(str.length() > 0) {
        ValueVector arr = JsonDump::getInstance()->dumpValueVectorFromString(str);
        for(auto g : arr) {
            int lang_id = g.asInt();
            if(std::find(arrLanguage.begin(), arrLanguage.end(), lang_id) != arrLanguage.end()) {
                continue;
            }
            comingsoon.push_back(lang_id);
        }
    }
    return comingsoon;
}

ValueMap GlobalFunc::getMapLanguageName() {
    ValueMap map;
    bool vn = StoryData::getInstance()->isVN;
    if(vn) {
        map["1"] = "Tiếng Anh";
        map["4"] = "Tiếng Việt";
    } else {
        map["1"] = "English US";
        map["4"] = "Vietnamese";
    }
    return map;
}

string GlobalFunc::getLanguageName(int lang_id) {
    ValueMap map = GlobalFunc::getMapLanguageName();
    auto lang_name = map[StringUtils::format("%i", lang_id)].asString();
    if (lang_name == "") {
        lang_name = getLanguageName(1);
    }
    return lang_name;
}

ValueMap GlobalFunc::getMapLanguageCode() {
    ValueMap map;
    map["1"] = "en-US";
    map["4"] = "vi-VN";
    return map;
}

int GlobalFunc::getLangidByCode(string code) {
    int lang_id = 1;
    ValueMap map = GlobalFunc::getMapLanguageCode();
    for(auto g : map) {
        if(g.second.asString() == code) {
            lang_id = atoi(g.first.c_str());
            break;
        }
    }
    return lang_id;
}

string GlobalFunc::getLanguageCode(int lang_id) {
    ValueMap map = GlobalFunc::getMapLanguageCode();
    if(lang_id == 0) {
        lang_id = GlobalFunc::getCurrentLanguage();
    }
    return map[StringUtils::format("%i", lang_id)].asString();
}

int GlobalFunc::getCurrentLanguage() {
    int lang_id = MJDEFAULT->getIntegerForKey("languageCode", 1);
    if(lang_id <= 0) {
        lang_id = 1;
    }
    return lang_id;
}

void GlobalFunc::setCurrentLanguage(int lang_id) {
    return MJDEFAULT->setIntegerForKey("languageCode", lang_id);
}

vector<int> GlobalFunc::getLanguageAvaiable() {
    vector<int> langs;
    string languageKey = MJDEFAULT->getStringForKey("language");
    if(languageKey.length() > 0) {
        ValueVector vector_langid = JsonDump::getInstance()->dumpValueVectorFromString(languageKey);
        for(auto g : vector_langid) {
            langs.push_back(g.asInt());
        }
    }
    return langs;
}

int GlobalFunc::detectLanguageBuyPackage(string package) {
    vector<string> arr = GlobalFunc::explode(package, '.');
    string code = arr.at(2);
    if(code == "alllanguage") {
        return 1;
    }
    ValueMap mapLanguage = GlobalFunc::getMapLanguageCode();
    for(auto item : mapLanguage) {
        if(item.second.asString() == code) {
            return atoi(item.first.c_str());
        }
    }
    return 1;
}

bool GlobalFunc::languageIsNew(int lang_id) {
    string StoryData_path = FileUtils::getInstance()->getWritablePath() + StringUtils::format("cards/StoryData_%d.plist", lang_id);
    return !FileUtils::getInstance()->isFileExist(StoryData_path);
}

//MARK: level
string GlobalFunc::getLevelType(int level_type, bool upper, bool hard) {
    if(level_type > 3) {
        level_type-=3;
    }
    string lv = "";
    if(level_type == 1) {
        lv = upper ? "Easy" : "easy";
    }
    if(level_type == 2) {
        lv = upper ? "Medium" : "medium";
    }
    if(level_type == 3) {
        if(hard) {
            lv = upper ? "Hard" : "hard";
        } else {
            lv = upper ? "Advanced" : "advanced";
        }
    }
    return lv;
}

std::string GlobalFunc::getLanguageFont() {
    return "";
}

void GlobalFunc::phonicColor(Color3B &hl, Color3B &nm, int index) {
    if(index == -1) {
        index = random<int>(1, 4);
    }
    if(index == 1) {
        hl = Color3B(0,202,182);
        nm = Color3B(3,21,24);
    } else if(index == 2) {
        hl = Color3B(255,2,31);
        nm = Color3B(64,32,21);
    } else if(index == 3) {
        hl = Color3B(253,87,55);
        nm = Color3B(8,98,130);
    } else {
        hl = Color3B(224,70,169);
        nm = Color3B(51,14,36);
    }
}

vector<string> GlobalFunc::explode(const string& str, const char& ch) {
    string next;
    vector<string> result;
    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}


bool GlobalFunc::vectorExits(vector<string> vector, string str) {
    for(string str2 : vector) {
        if(GlobalFunc::icompare(str2, str)) {
            return true;
        }
    }
    return false;
}


string GlobalFunc::dauVN(int kieudau, bool sound) {
    string dau_path = "";
    if(!sound) {
        if(kieudau == 1) {
            dau_path = "lesson/dau/huyen.png";
        } else if(kieudau == 2) {
            dau_path = "lesson/dau/sac.png";
        } else if(kieudau == 3) {
            dau_path = "lesson/dau/nang.png";
        } else if(kieudau == 4) {
            dau_path = "lesson/dau/nga.png";
        } else if(kieudau == 5) {
            dau_path = "lesson/dau/hoi.png";
        }
    } else {
        if(kieudau == 1) {
            dau_path = "dau_huyen";
        } else if(kieudau == 2) {
            dau_path = "dau_sac";
        } else if(kieudau == 3) {
            dau_path = "dau_nang";
        } else if(kieudau == 4) {
            dau_path = "dau_nga";
        } else if(kieudau == 5) {
            dau_path = "dau_hoi";
        }
    }
    return dau_path;
}




//MARK: xu ly string
std::string GlobalFunc::joinIntVector(std::vector<int> vv,std::string delimiter) {
    std::stringstream s;
    copy(vv.begin(),vv.end(), std::ostream_iterator<int>(s,delimiter.c_str()));
    std::string str = s.str();
    str = str.substr (0,str.length()-(delimiter.length()));
    return str;
}

std::string GlobalFunc::joinStringVector(std::vector<string> vv,std::string delimiter) {
    std::stringstream s;
    copy(vv.begin(),vv.end(), std::ostream_iterator<string>(s,delimiter.c_str()));
    std::string str = s.str();
    str = str.substr (0,str.length()-(delimiter.length()));
    return str;
}


int GlobalFunc::getTotalLetter(string str) {
    return NativeBridge::utf8LengthOfString(str);
}
/*
 ha van hoi
 van
 nguyen
 */
void GlobalFunc::ReplaceString(std::string &subject, const std::string& search,
                               const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

string GlobalFunc::utf8characterAtIndex(string str, int index) {
    return NativeBridge::utf8characterAtIndex(str, index);
}

bool GlobalFunc::icompare(string a, string b)
{
    return NativeBridge::utf8StringCompare(a,b);
}

string GlobalFunc::string_towlower(string str) {
    return NativeBridge::string_towlower(str);
}

string GlobalFunc::string_towupper(string str) {
    return NativeBridge::string_towupper(str);
}

void GlobalFunc::loadSpriteSheets(string path)
{
    CCLOG("SpriteSheets: %s.png", path.c_str());
    string namePlistFile = StringUtils::format("%s.plist", path.c_str());
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(globalFunc->fileInResourse(namePlistFile));
}

bool GlobalFunc::networkAvaiable() {
    return NativeBridge::networkAvaiable();
}

int GlobalFunc::getDeviceID() {
    return MJDEFAULT->getIntegerForKey("edu_device_id", 0);
}

vector<string> GlobalFunc::getStickerNeedPerchase(bool new_pk) {
    vector<string> stickers;
    string sticker_save = MJDEFAULT->getStringForKey("sticker_paid");
    if(sticker_save.length() > 0) {
        ValueVector vector_sticker = JsonDump::getInstance()->dumpValueVectorFromString(sticker_save);
        for(auto g : vector_sticker) {
            string pid = g.asString();
            if(new_pk){
                GlobalFunc::ReplaceString(pid, "com.", "new.");
            }
            stickers.push_back(pid);
        }
    }
    return stickers;
}

ValueVector GlobalFunc::getStickerPerchased() {
    return {};
}

void GlobalFunc::stickerPerchased(string package_name) {
}

void GlobalFunc::setCouponCode(string coupon) {
    MJDEFAULT->setStringForKey("edu_coupon_code", coupon);
}

//mac dinh cho huong 30% tat luon
string GlobalFunc::getCouponCode() {
    return "";
}

//replace _sale => ''
//replace _original => ''
//replace .upgrade => '.full'

void GlobalFunc::replacePackage(string &package) {
    GlobalFunc::ReplaceString(package, "_sale", "");
    GlobalFunc::ReplaceString(package, "_original", "");
    GlobalFunc::ReplaceString(package, ".upgrade", ".full");
    GlobalFunc::ReplaceString(package, "com.", "new.");
}


void GlobalFunc::createLicenceMessage(string licence, string product_name, int total_device){
    return;
    if(licence.length() == 0) {
        return;
    }
    licence = GlobalFunc::string_towupper(licence);
    int created =  time(nullptr);
    string msg = StringUtils::format("License key %s (%s) is successfully activated. This key can be used in one more device. Thank you.", licence.c_str(), product_name.c_str());
    if(StoryData::getInstance()->isVN) {
        msg = StringUtils::format("Mã kích hoạt %s (%s) đã được nhập thành công. Bạn có thể dùng mã trên ở một thiết bị khác.",licence.c_str(), product_name.c_str());
    }
    
    if(total_device != 0) {
        msg = StringUtils::format("Mã kích hoạt %s (%s) đã được nhập thành công.", licence.c_str(), product_name.c_str());
        if(StoryData::getInstance()->isVN) {
            msg = StringUtils::format("License key %s (%s) is successfully activated.", licence.c_str(), product_name.c_str());
        }
    }
    
    ValueVector arrayMessage;
    ValueMap mapMessage;
    mapMessage["subject"] = "Licence key";
    mapMessage["content"] = msg;
    mapMessage["url"] = "";
    mapMessage["created"] = created;
    mapMessage["type"] = 1;
    arrayMessage.push_back(Value(mapMessage));
}

void GlobalFunc::processUpdateInfo(ValueMap map_response) {
    
}

void GlobalFunc::playLanguageAudio(string config_name) {
}

//MARK: user
void GlobalFunc::changeUserID(int uid) {
    MJDEFAULT->setIntegerForKey("edu_current_user", uid);
}


int GlobalFunc::userID() {
    int uid = MJDEFAULT->getIntegerForKey("edu_current_user", 1);
    if(uid <= 0) {
        uid = 1;
    }
    return uid;
}

ValueMap GlobalFunc::userLevelInfo() {
    ValueMap user;
    return user;
}

void GlobalFunc::userSaveLevelInfo(ValueMap save_map) {    
    
}


void GlobalFunc::userSaveLevelInfoWithSync(int level_id, int lesson_index, bool action_now) {
    
}

void GlobalFunc::poolSprite(Node *node, string str) {
    vector<string> arr = GlobalFunc::explode(str, ',');
    if(arr.size() == 0) {
        return;
    }
    for(string sp : arr) {
        auto tmp = Sprite::createWithSpriteFrameName(sp);
        tmp->setVisible(false);
        node->addChild(tmp);
    }
}

string GlobalFunc::getRateAppUrl() {
    string url = rate_url_ios;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(os_amazon == true) {
        url = rate_url_amazon;
    } else {
        url = rate_url_android;
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    url = std::string("macappstore://itunes.apple.com/app/id1133076609?mt=12");
    //id1133076609
#endif
    
    return url;
}

void GlobalFunc::sendEmailSupport() {
}

int GlobalFunc::totalSampleLesson() {
    ValueMap userLevel = GlobalFunc::userLevelInfo();
    int level_id = userLevel["current_level"].asInt();
    ValueMap levelInfo;
    int total_lesson = (int)levelInfo["jsondata"].asValueMap()["listlesson"].asValueVector().size();
    if(total_lesson > 20) {
        total_lesson = 6;
    }
    int lvid = levelInfo["id"].asInt();
    if(lvid == 74 || lvid == 75) {
        total_lesson = 4;
    }
    total_lesson -= 2;
    return total_lesson;
}

//codemoi
/*
 int current_language_play = 0;
 int current_level_play = 0;
 */
ValueMap GlobalFunc::loadGroupAudioByLevel(ValueMap dict) {
    int weight = dict["weight"].asInt();
    int level_id = dict["id"].asInt();
    int sample_level = dict["sample_level"].asInt();
    if(sample_level == 1) {
        ValueMap jsondata = dict["jsondata"].asValueMap();
        return jsondata["groupaudio"].asValueMap();
    }
    
    int level_type = dict["level_type"].asInt();
    int lang_id = dict["lang_id"].asInt();
    
    if(level_id == current_level_id_play && level_type == current_level_play && lang_id == current_language_play) {
        //lay danh sach audio cua course
        return listGroupAudio;
    }
    
    listGroupAudio.clear();
    string condition = StringUtils::format(" AND weight <= %i", weight);
    ValueVector vector;
    for(auto g : vector) {
        ValueMap lv = g.asValueMap();
        ValueMap jsondata = lv["jsondata"].asValueMap();
        ValueMap groupaudio = jsondata["groupaudio"].asValueMap();
        for(auto item : groupaudio) {
            listGroupAudio[item.first] = item.second;
        }
    }
    current_level_play = level_type;
    current_language_play = lang_id;
    current_level_id_play = level_id;
    return listGroupAudio;
}
//end

void GlobalFunc::syncDataProcess(ValueMap dict_response) {
    
}

int GlobalFunc::getSoBaiDuocHoc(){
    return 1;
}

void GlobalFunc::setSoBaiDuocHoc(int soBaihoc){
    
}

int GlobalFunc::getThoigianGiuaHaiLanHoc(){
    return 1;
}

void GlobalFunc::setThoigianGiuaHaiLanHoc(int soGiay){
    
}

std::string GlobalFunc::getStringOfDateHanChe(std::string keyStr){
    return "";
}

void GlobalFunc::setStringOfDateHanChe(std::string keyStr, std::string dateStr){
}

int GlobalFunc::getNumLessonHanche(){
    return 1;
}

void GlobalFunc::setNumLessonHanche(int value){
    
}
void GlobalFunc::onDeepLink(std::string url) {
    CCLOG("onDeepLink: %s", url.c_str());
    //HandleDeepLink* handleDeepLink = new HandleDeepLink(url);
    //globalFunc->setCurrentDeepLink(handleDeepLink);
    //handleDeepLink->onDeepLink();
}
