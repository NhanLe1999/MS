#ifndef MJDefault_h
#define MJDefault_h
#include "cocos2d.h"
#include <iostream>
#include "LTDate.h"
#include "Key_Default_Model.h"
#include "MJStoryInfoModel.h"

#define MJDEFAULT MJDefault::getInstance()

class MJDefault{
public:
    MJDefault();
    static MJDefault *getInstance();
    bool isWindows = false;
    bool isSettingJumpBt = false;
    void setIntegerForKey(std::string key, int value);
    void setIntegerForKeyForListValue(std::vector<std::pair<std::string, int>> keyValue);
    int getIntegerForKey(std::string key, int default_value = 0);
    
    void setFloatForKey(std::string key, float value);
    float getFloatForKey(std::string key, float default_value = 0);
    
    void setBoolForKey(std::string key, bool value);
    bool getBoolForKey(std::string key, bool default_value = false);
    
    void setDateForKey(std::string key, LTDate value);
    LTDate getDateForKey(std::string key, LTDate default_value = LTDate::getCurrentDate());
    
    
    void setStringForKey(std::string key, std::string value);
    std::string getStringForKey(std::string key, std::string default_value = "");
    
    void deleteValueForKey(std::string key);
    
    void setDataBaseStringForKey(std::string key, std::string value);
    std::string getDataBaseStringForKey(std::string key);
    
    void setNumLessonHanche(int value);
    int getNumLessonHanche();

	std::string getAllDatabase();
	void setAllDatabase(std::string info);

    std::vector<int> getArrayIntegerForKey(std::string key, std::vector<int> default_value);
    void setArrayIntegerForKey(std::string key, std::vector<int> value);

    std::vector<std::string> getArrayStringForKey(std::string key, std::vector<std::string> default_value);
    void setArrayStringForKey(std::string key, std::vector<std::string> value);
};
#endif
