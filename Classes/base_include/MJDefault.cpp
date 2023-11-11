#include "MJDefault.h"
#include "UtilitySqlite.h"
#include "GlobalFunc.h"
#include "MJPlatformConfig.h"
#define kTableName "mjdefault"
using namespace std;
USING_NS_CC;
static MJDefault *shareInstant = NULL;
MJDefault* MJDefault::getInstance(){
    if (shareInstant == NULL) {
        shareInstant = new MJDefault();
    }
    return shareInstant;
}
MJDefault::MJDefault(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    UtilitySqlite::shareInstance()->onOpenDatabase();
    isWindows = true;
#endif
}

void MJDefault::setIntegerForKey(std::string key, int value)
{
    if (isWindows)
    {
        auto valueStr = cocos2d::StringUtils::format("%d", value);
        this->setDataBaseStringForKey(key, valueStr);
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), value);
        UserDefault::getInstance()->flush();
    }
}
int MJDefault::getIntegerForKey(std::string key, int default_value){
    if (isWindows) {
        std::string value_string = this->getDataBaseStringForKey(key);
        if (value_string == "")
            return default_value;
        return std::atoi(value_string.c_str());
    }else{
        return UserDefault::getInstance()->getIntegerForKey(key.c_str(), default_value);
    }
}

void MJDefault::setFloatForKey(std::string key, float value){
    if (isWindows) {
        auto valueStr = cocos2d::StringUtils::format("%f", value);
        this->setDataBaseStringForKey(key, valueStr);
    }else{
        UserDefault::getInstance()->setFloatForKey(key.c_str(), value);
        UserDefault::getInstance()->flush();
    }
}
float MJDefault::getFloatForKey(std::string key, float default_value){
    if (isWindows) {
        std::string value_string = this->getDataBaseStringForKey(key);
        if (value_string == "") {
            return default_value;
        }
        return std::atof(value_string.c_str());
    }else{
        return UserDefault::getInstance()->getFloatForKey(key.c_str(), default_value);
    }
}

void MJDefault::setBoolForKey(std::string key, bool value){
    if (isWindows) {
        std::string valueStr = value ? "true" : "false";
        this->setDataBaseStringForKey(key, valueStr);
    }else{
        UserDefault::getInstance()->setBoolForKey(key.c_str(), value);
        UserDefault::getInstance()->flush();
    }

}
bool MJDefault::getBoolForKey(std::string key, bool default_value){
    if (isWindows) {
        std::string value_string = this->getDataBaseStringForKey(key);
        if (value_string == "")
            return default_value;
        return (value_string == "true");
    }else{
        return UserDefault::getInstance()->getBoolForKey(key.c_str(), default_value);
    }
}

void MJDefault::setDateForKey(std::string key, LTDate value){
    std::string value_string = value.toString();
    GlobalFunc::setStringOfDateHanChe(key, value_string);
}
LTDate MJDefault::getDateForKey(std::string key, LTDate default_value){
    std::string value_string = GlobalFunc::getStringOfDateHanChe(key);
    if (value_string == "")
        return default_value;
    return LTDate(value_string);
}

void MJDefault::setStringForKey(std::string key, std::string value){
    if (isWindows) {
        this->setDataBaseStringForKey(key, value);
    }else{
        UserDefault::getInstance()->setStringForKey(key.c_str(), value);
        UserDefault::getInstance()->flush();
    }
}
std::string MJDefault::getStringForKey(std::string key, std::string default_value){
    if (isWindows) {
        auto value = this->getDataBaseStringForKey(key);
        if (value == "") {
            return default_value;
        }else return value;
    }else{
        return UserDefault::getInstance()->getStringForKey(key.c_str(), default_value);
    }
}

void MJDefault::deleteValueForKey(std::string key){
    if (isWindows) {
        UtilitySqlite::shareInstance()->deleteData(kTableName, StringUtils::format("mj_key = '%s'", key.c_str()));
    }else{
        UserDefault::getInstance()->deleteValueForKey(key.c_str());
    }
}

#include "HSLib.h"
void MJDefault::setDataBaseStringForKey(std::string key, std::string value){
    string query = StringUtils::format("REPLACE INTO %s (mj_key, mj_value) VALUES (?,?)", kTableName);
    ValueVector query_vector;
    query_vector.push_back(Value(key));
    query_vector.push_back(Value(XHSLib->mahoaString(value)));
    UtilitySqlite::shareInstance()->executeQuery(query,query_vector);
}

#include <numeric>

void MJDefault::setIntegerForKeyForListValue(std::vector<std::pair<std::string, int>> keyValue)
{
    if (isWindows)
    {
        try {
            if (keyValue.size() > 50) {
                auto first = 0;
                const auto chunk_size = 50;
                while (first < keyValue.size()) {
                    auto last = std::min(first + chunk_size, (int)keyValue.size());
                    setIntegerForKeyForListValue(std::vector<std::pair<std::string, int>>(std::begin(keyValue) + first, std::begin(keyValue) + last));
                    first = last;
                }

                return;
            }
            std::string query = StringUtils::format("REPLACE INTO %s (mj_key, mj_value) VALUES ", kTableName);



            query = std::accumulate(std::begin(keyValue), std::end(keyValue), query, [](std::string q, std::pair<std::string, int>) {
                return q.append("(?,?)" + std::string(","));
                });
            query.resize(query.length() - 1);

            ValueVector query_vector;

            for (auto kv : keyValue)
            {
                auto valueStr = cocos2d::StringUtils::format("%d", kv.second);
                query_vector.push_back(Value(kv.first));
                query_vector.push_back(Value(XHSLib->mahoaString(valueStr)));
            }

            UtilitySqlite::shareInstance()->executeQuery(query, query_vector);

            int count = 0;

        }
        catch (std::exception& e) {
            CCLOG("%s db exception: %s", __FUNCTION__, e.what());
        }
    }
    else {

        for (auto kv : keyValue)
        {

            UserDefault::getInstance()->setIntegerForKey(kv.first.c_str(), kv.second);
            UserDefault::getInstance()->flush();
        }
    }
    
}

std::string MJDefault::getDataBaseStringForKey(std::string key){
    auto value = UtilitySqlite::shareInstance()->getValueFromKey(key);
    value = XHSLib->giaimaString(value);
    return value;
}

void MJDefault::setNumLessonHanche(int value){
    GlobalFunc::setNumLessonHanche(value);
}

int MJDefault::getNumLessonHanche(){
    return GlobalFunc::getNumLessonHanche();
}

std::string MJDefault::getAllDatabase() {
	if (isWindows) {
		return UtilitySqlite::shareInstance()->getAllData();
	}
	/*else {
		UserDefault::getInstance()->deleteValueForKey(key.c_str());
	}*/
}

void MJDefault::setAllDatabase(std::string info) {
	if (isWindows) {
		UtilitySqlite::shareInstance()->onCloseDatabase();
		UtilitySqlite::shareInstance()->setAllData(info);
	}
}

std::vector<int> MJDefault::getArrayIntegerForKey(std::string key, std::vector<int> default_value)
{
    std::string valueStr = getStringForKey(key, "");
    return mj::model::ConvertStringToArrayInterger(valueStr);
}

void MJDefault::setArrayIntegerForKey(std::string key, std::vector<int> value) 
{
    std::string valueStr = mj::model::ConvertArrayIntergerToString(value);
    setStringForKey(key, valueStr);
}

std::vector<std::string> MJDefault::getArrayStringForKey(std::string key, std::vector<std::string> default_value)
{
    std::string valueStr = getStringForKey(key, "");
    return mj::model::ConvertStringToArrayString(valueStr);
}

void MJDefault::setArrayStringForKey(std::string key, std::vector<std::string> value)
{
    std::string valueStr = mj::model::ConvertArrayStringToString(value);
    setStringForKey(key, valueStr);
}

