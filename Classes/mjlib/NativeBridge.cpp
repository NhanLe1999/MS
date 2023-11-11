#include "NativeBridge.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <clocale>
#include "utf8.h"//

std::string NativeBridge::chuthuongx = "áàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵæçëîïœûüÿñ";//
std::string NativeBridge::chuhoax = "ÁÀẢÃẠĂẮẰẲẴẶÂẤẦẨẪẬĐÉÈẺẼẸÊẾỀỂỄỆÍÌỈĨỊÓÒỎÕỌÔỐỒỔỖỘƠỚỜỞỠỢÚÙỦŨỤƯỨỪỬỮỰÝỲỶỸỴÆÇËÎÏŒÛÜŸÑ";//
//std::string NativeBridge::khongdau = "aaaaaaaaaaaaaaaaadeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyæçëîïœûüÿñ";//
#define numSoSanhString 77
//std::string chuthuong = "abcefg";
//std::string chuhoa = "ABCEFG";

void NativeBridge::keepBackLightOn(bool keep)
{
}

bool NativeBridge::utf8StringCompare(string str1, string str2) {
    str1 = string_towlower(str1);
    str2 = string_towlower(str2);
    if (!str1.compare(str2))
    {
        return true;
    }
    return false;
}

int NativeBridge::utf8LengthOfString(string str) {
    return hsNumberCharactersOfString(str);
}

string NativeBridge::utf8characterAtIndex(string str, int index) {
    return hsStringAtIndex(index, str);
}

string NativeBridge::string_towupper(string str) {
    if (str == "") {
        return str;
    }
    return hsToUpperString(str);
}

string NativeBridge::string_towlower(string str) {
    CCLOG("HSLower1: %s", str.c_str());
    if (str == "") {
        return str;
    }
    str = hsToLowerString(str);
    CCLOG("HSLower2: %s", str.c_str());
    return str;
    //std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool NativeBridge::networkAvaiable() {
    return true;
}

NativeBridge* NativeBridge::instance;

NativeBridge* NativeBridge::getInstance()
{
    if (instance == nullptr) {
        instance = new NativeBridge();
    }
    return instance;
}

void NativeBridge::sendMail(string subject, string content) {
    
}

void NativeBridge::PushReminder(vector<string> messages, vector<int> listDate) {
}

void NativeBridge::initPush() {
}
void NativeBridge::initAlarm(string json) {
}
float NativeBridge::getTimeZoneOffset() {
    return 7;
}
string NativeBridge::getDeviceToken() {
    return "";
}

int NativeBridge::hsNumberCharactersOfString(std::string inputStr){
    std::string::iterator end_it = utf8::find_invalid(inputStr.begin(), inputStr.end());
    //CCLOG("hsCount = %d", (int)utf8::distance(inputStr.begin(), end_it));
    return (int)utf8::distance(inputStr.begin(), end_it);
}

string NativeBridge::testtest(){
    return hsToLowerString("ÀÂÆÇÉÈÊËÎÏÔŒÙÛÜŸ");
}

std::string NativeBridge::hsStringAtIndex(int indexChar, std::string wordgr){
    std::string::iterator curr_it = wordgr.begin();
    std::string::iterator next_it = curr_it;
    auto count = 0;
    auto index = indexChar;
    utf8::next(next_it, wordgr.end());
    while (curr_it != wordgr.end()) {
        if (count == index) {
            return std::string(curr_it, next_it);
        }
        curr_it = next_it;
        if (next_it != wordgr.end()) {
            utf8::next(next_it, wordgr.end());
        }
        count++;
        ;
    }
    return "";
    
}

std::string NativeBridge::hsStringSub(std::string wordgr, int indexChar, int length){
    std::string::iterator curr_it = wordgr.begin();
    std::string::iterator next_it = curr_it;
    auto count = 0;
    auto index = indexChar;
    std::string _returnStr = "";
    utf8::next(next_it, wordgr.end());
    while (curr_it != wordgr.end()) {
        if (count == index) {
            if (index >= indexChar+length) {
                return _returnStr;
            }else{
                _returnStr += std::string(curr_it, next_it);
                index++;
            }
            //return std::string(curr_it, next_it);
        }
        curr_it = next_it;
        if (next_it != wordgr.end()) {
            utf8::next(next_it, wordgr.end());
        }
        count++;
        ;
    }
    return _returnStr;
}


std::string NativeBridge::hsToLowerString(std::string inputStr) {
    std::string::iterator curr_it = inputStr.begin();
    std::string::iterator next_it = curr_it;
    std::string outputStr = "";
    utf8::next(next_it, inputStr.end());
    while (curr_it != inputStr.end()) {
        std::string cstr = std::string(curr_it, next_it);
        std::string temp = "";
        for (int i = 0; i < numSoSanhString; i++) {
            if (!cstr.compare(hsStringAtIndex(i, chuhoax))) {
                temp = hsStringAtIndex(i, chuthuongx);
                break;
            }
            else{
                temp = cstr;
                std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            }
        }
        
        outputStr += temp;
        curr_it = next_it;
        if (next_it != inputStr.end()) {
            utf8::next(next_it, inputStr.end());
        }
    }
    return outputStr;
}
std::string NativeBridge::hsToUpperString(std::string inputStr){
    std::string::iterator curr_it = inputStr.begin();
    std::string::iterator next_it = curr_it;
    std::string outputStr = "";
    utf8::next(next_it, inputStr.end());
    while (curr_it != inputStr.end()) {
        std::string cstr = std::string(curr_it, next_it);
        std::string temp = "";
        for (int i = 0; i < numSoSanhString; i++) {
            if (!cstr.compare(hsStringAtIndex(i, chuthuongx))) {
                temp = hsStringAtIndex(i, chuhoax);
                break;
            }
            else{
                temp = cstr;
                std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
            }
        }
        
        outputStr += temp;
        curr_it = next_it;
        if (next_it != inputStr.end()) {
            utf8::next(next_it, inputStr.end());
        }
    }
    return outputStr;
    return "";
}

std::vector<int> NativeBridge::hsFindString(std::string substring, std::string findstring, bool isCheckValid){
    std::vector<int> results;
    auto length_find = hsNumberCharactersOfString(findstring);
    for (int i = 0; i <= ((int)hsNumberCharactersOfString(substring) - length_find); i++) {
        auto _str_cut = hsStringSub(substring, i, length_find);
        //CCLOG("str cut %i = | = %s", i, _str_cut.c_str());
        if (_str_cut.compare(findstring) == 0) {//tìm thấy
            //CCLOG("find result = %d", i);
            if (!isCheckValid || checkValidPost(i, substring, findstring)) {
                results.push_back(i);
            }
            i = i + length_find - 1;
        }
    }
    return results;
}
vector<int> NativeBridge::findStringOnString(std::string subject_str, std::string find_str, bool isCheckValid){
    return hsFindString(subject_str, find_str, isCheckValid);
}

bool NativeBridge::checkValidPost(int pos, std::string subject_str, std::string find_str){
    auto length_subject = utf8LengthOfString(subject_str);
    auto length_find = utf8LengthOfString(find_str);
    if (length_subject == length_find) {
        return true;
    }else{
        auto daudacbiet = std::string("\n“:.!-?,;\"' ");
        if (pos == 0) {
            auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
            auto daudacbiet_end = daudacbiet;
            
            for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                if (str_check_end == daudacbiet_end.substr(j, 1)) {
                    return true;
                }
            }
            
        }else if (pos + length_find == length_subject){
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    return true;
                }
            }
            
        }else{
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            auto check_start = false;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    check_start = true;
                    break;
                }
            }
            
            if (check_start) {
                auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
                auto daudacbiet_end = daudacbiet;
                
                for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                    if (str_check_end == daudacbiet_end.substr(j, 1)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

string NativeBridge::hsStringToKhongDau(std::string subject_str, int lang_id){
    return subject_str;
    std::string str_chuthuongx = "áàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵ";
    std::string str_khongdau = "aaaaaaaaaaaaaaaaadeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyy";
    int size_str_template = 67;
    string re_str;
    subject_str = string_towlower(subject_str);
    for (int i = 0; i < utf8LengthOfString(subject_str); i++) {
        auto sub_str = utf8characterAtIndex(subject_str, i);
        auto check = false;
        for (int j = 0; j < size_str_template; j++) {
            auto compare_str = utf8characterAtIndex(str_chuthuongx, j);
            if (sub_str == compare_str) {
                check = true;
                re_str += utf8characterAtIndex(str_khongdau, j);
                break;
            }
        }
        if (!check) {
            re_str += sub_str;
        }
    }
    return re_str;
}

void NativeBridge::changeDeviceOrientation(bool isPortrait) {
}

void NativeBridge::setCrashlyticsKeyValue(std::string key, std::string value){
}

void NativeBridge::rateAppMJStories(){
    string abc = "";
}

bool NativeBridge::isTabletDevice(){
    return true;
}

bool NativeBridge::isRetinaDevice(){
    return true;
}

bool NativeBridge::isRamWarning() {
    return false;
}

void NativeBridge::callProscoreAPI(std::vector<std::pair<std::string, std::string>> params, std::function<void(std::string)> onSucceed, std::function<void()> onConnectionError) {

}


#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include <clocale>
#include "utf8.h"
#include "MacCppBridge.h"
std::string NativeBridge::chuthuongx = "áàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵæçëîïœûüÿñ";//
std::string NativeBridge::chuhoax = "ÁÀẢÃẠĂẮẰẲẴẶÂẤẦẨẪẬĐÉÈẺẼẸÊẾỀỂỄỆÍÌỈĨỊÓÒỎÕỌÔỐỒỔỖỘƠỚỜỞỠỢÚÙỦŨỤƯỨỪỬỮỰÝỲỶỸỴÆÇËÎÏŒÛÜŸÑ";//
#define numSoSanhString 77
//std::string chuthuong = "abcefg";
//std::string chuhoa = "ABCEFG";

void NativeBridge::keepBackLightOn(bool keep)
{
}

bool NativeBridge::utf8StringCompare(string str1, string str2) {
    str1 = string_towlower(str1);
    str2 = string_towlower(str2);
    if (!str1.compare(str2))
    {
        return true;
    }
    return false;
}

int NativeBridge::utf8LengthOfString(string str) {
    return hsNumberCharactersOfString(str);
}

string NativeBridge::utf8characterAtIndex(string str, int index) {
    return hsStringAtIndex(index, str);
}

string NativeBridge::string_towupper(string str) {
    if (str == "") {
        return str;
    }
    return hsToUpperString(str);
}
string NativeBridge::string_towlower(string str) {
    CCLOG("HSLower1: %s", str.c_str());
    if (str == "") {
        return str;
    }
    str = hsToLowerString(str);
    CCLOG("HSLower2: %s", str.c_str());
    return str;
    //std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool NativeBridge::networkAvaiable() {
    return MAC_CPP_BRIDGE->isInternetConnection();
}

NativeBridge* NativeBridge::instance;

NativeBridge* NativeBridge::getInstance()
{
    if (instance == nullptr) {
        instance = new NativeBridge();
    }
    return instance;
}

void NativeBridge::sendMail(string subject, string content) {
    //MAC_CPP_BRIDGE->sendMailSupport(subject, content);
}

void NativeBridge::PushReminder(vector<string> messages, vector<int> listDate) {
}

void NativeBridge::initPush() {
}
void NativeBridge::initAlarm(string json) {
}
float NativeBridge::getTimeZoneOffset() {
    return 7;
}
string NativeBridge::getDeviceToken() {
    return "";
}

int NativeBridge::hsNumberCharactersOfString(std::string inputStr){
    std::string::iterator end_it = utf8::find_invalid(inputStr.begin(), inputStr.end());
    //CCLOG("hsCount = %d", (int)utf8::distance(inputStr.begin(), end_it));
    return (int)utf8::distance(inputStr.begin(), end_it);
}

string NativeBridge::testtest(){
    return hsToLowerString("ÀÂÆÇÉÈÊËÎÏÔŒÙÛÜŸ");
}

std::string NativeBridge::hsStringAtIndex(int indexChar, std::string wordgr){
    std::string::iterator curr_it = wordgr.begin();
    std::string::iterator next_it = curr_it;
    auto count = 0;
    auto index = indexChar;
    utf8::next(next_it, wordgr.end());
    while (curr_it != wordgr.end()) {
        if (count == index) {
            return std::string(curr_it, next_it);
        }
        curr_it = next_it;
        if (next_it != wordgr.end()) {
            utf8::next(next_it, wordgr.end());
        }
        count++;
        ;
    }
    return "";
    
}

std::string NativeBridge::hsToLowerString(std::string inputStr) {
    std::string::iterator curr_it = inputStr.begin();
    std::string::iterator next_it = curr_it;
    std::string outputStr = "";
    utf8::next(next_it, inputStr.end());
    while (curr_it != inputStr.end()) {
        std::string cstr = std::string(curr_it, next_it);
        std::string temp = "";
        for (int i = 0; i < numSoSanhString; i++) {
            if (!cstr.compare(hsStringAtIndex(i, chuhoax))) {
                temp = hsStringAtIndex(i, chuthuongx);
                break;
            }
            else{
                temp = cstr;
                std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            }
        }
        
        outputStr += temp;
        curr_it = next_it;
        if (next_it != inputStr.end()) {
            utf8::next(next_it, inputStr.end());
        }
    }
    return outputStr;
}
std::string NativeBridge::hsToUpperString(std::string inputStr){
    std::string::iterator curr_it = inputStr.begin();
    std::string::iterator next_it = curr_it;
    std::string outputStr = "";
    utf8::next(next_it, inputStr.end());
    while (curr_it != inputStr.end()) {
        std::string cstr = std::string(curr_it, next_it);
        std::string temp = "";
        for (int i = 0; i < numSoSanhString; i++) {
            if (!cstr.compare(hsStringAtIndex(i, chuthuongx))) {
                temp = hsStringAtIndex(i, chuhoax);
                break;
            }
            else{
                temp = cstr;
                std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
            }
        }
        
        outputStr += temp;
        curr_it = next_it;
        if (next_it != inputStr.end()) {
            utf8::next(next_it, inputStr.end());
        }
    }
    return outputStr;
    return "";
}

std::string NativeBridge::hsStringSub(std::string wordgr, int indexChar, int length){
    std::string::iterator curr_it = wordgr.begin();
    std::string::iterator next_it = curr_it;
    auto count = 0;
    auto index = indexChar;
    std::string _returnStr = "";
    utf8::next(next_it, wordgr.end());
    while (curr_it != wordgr.end()) {
        if (count == index) {
            if (index >= indexChar+length) {
                return _returnStr;
            }else{
                _returnStr += std::string(curr_it, next_it);
                index++;
            }
            //return std::string(curr_it, next_it);
        }
        curr_it = next_it;
        if (next_it != wordgr.end()) {
            utf8::next(next_it, wordgr.end());
        }
        count++;
        ;
    }
    return _returnStr;
}


std::vector<int> NativeBridge::hsFindString(std::string substring, std::string findstring){
    std::vector<int> results;
    auto length_find = hsNumberCharactersOfString(findstring);
    for (int i = 0; i <= ((int) hsNumberCharactersOfString(substring) - length_find); i++) {
        auto _str_cut = hsStringSub(substring, i, length_find);
        CCLOG("str cut %i = | = %s", i, _str_cut.c_str());
        if (_str_cut.compare(findstring) == 0) {//tìm thấy
            //CCLOG("find result = %d", i);
            if (checkValidPost(i, substring, findstring)) {
                results.push_back(i);
            }
            i = i + length_find - 1;
        }
    }
    return results;
}
 /*
vector<int> NativeBridge::findStringOnString(std::string subject_str, std::string find_str){
    return hsFindString(subject_str, find_str);
}
*/
bool NativeBridge::checkValidPost(int pos, std::string subject_str, std::string find_str){
    auto length_subject = utf8LengthOfString(subject_str);
    auto length_find = utf8LengthOfString(find_str);
    if (length_subject == length_find) {
        return true;
    }else{
        auto daudacbiet = std::string("\n“:.!-?,;\"' ");
        if (pos == 0) {
            auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
            auto daudacbiet_end = daudacbiet;
            
            for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                if (str_check_end == daudacbiet_end.substr(j, 1)) {
                    return true;
                }
            }
            
        }else if (pos + length_find == length_subject){
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    return true;
                }
            }
            
        }else{
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            auto check_start = false;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    check_start = true;
                    break;
                }
            }
            
            if (check_start) {
                auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
                auto daudacbiet_end = daudacbiet;
                
                for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                    if (str_check_end == daudacbiet_end.substr(j, 1)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

string NativeBridge::hsStringToKhongDau(std::string subject_str, int lang_id){
    std::string str_chuthuongx = "áàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵæçëîïœûüÿñ";
    std::string str_khongdau = "aaaaaaaaaaaaaaaaadeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyæçëîïœûüÿñ";
    string re_str;
    subject_str = string_towlower(subject_str);
    for (int i = 0; i < utf8LengthOfString(subject_str); i++) {
        auto sub_str = utf8characterAtIndex(subject_str, i);
        auto check = false;
        for (int j = 0; j < numSoSanhString; j++) {
            auto compare_str = utf8characterAtIndex(str_chuthuongx, j);
            if (sub_str == compare_str) {
                check = true;
                re_str += utf8characterAtIndex(str_khongdau, j);
                break;
            }
        }
        if (!check) {
            re_str += sub_str;
        }
    }
    
    return re_str;
}

vector<int> NativeBridge::findStringOnString(std::string subject_str, std::string find_str, bool isCheckValid){
    return hsFindString(subject_str, find_str, isCheckValid);
}


std::vector<int> NativeBridge::hsFindString(std::string substring, std::string findstring, bool isCheckValid){
    std::vector<int> results;
    auto length_find = hsNumberCharactersOfString(findstring);
    for (int i = 0; i <= ((int)hsNumberCharactersOfString(substring) - length_find); i++) {
        auto _str_cut = hsStringSub(substring, i, length_find);
        //CCLOG("str cut %i = | = %s", i, _str_cut.c_str());
        if (_str_cut.compare(findstring) == 0) {//tìm thấy
            //CCLOG("find result = %d", i);
            if (!isCheckValid || checkValidPost(i, substring, findstring)) {
                results.push_back(i);
            }
            i = i + length_find - 1;
        }
    }
    return results;
}
#include "StoryConfigManager.h"
void NativeBridge::rateAppMJStories(){
    if (CONFIG_MANAGER->isVietnameseStories()) {
        Application::getInstance()->openURL("macappstore://itunes.apple.com/app/id1389646429?mt=12");
    }else Application::getInstance()->openURL("macappstore://itunes.apple.com/app/id1282232882?mt=12");
}

bool NativeBridge::isTabletDevice(){
    return true;
}

bool NativeBridge::isRetinaDevice(){
    return true;
}

void NativeBridge::changeDeviceOrientation(bool isPortrait) {
}

void NativeBridge::setCrashlyticsKeyValue(std::string key, std::string value){
    MAC_CPP_BRIDGE->setCrashlyticsKeyValue(key, value);
}

void NativeBridge::callProscoreAPI(std::vector<std::pair<std::string, std::string>> params, std::function<void(std::string)> onSucceed, std::function<void()> onConnectionError) {

}


bool NativeBridge::isRamWarning() {
    return false;
}

#endif
