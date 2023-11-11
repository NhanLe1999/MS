#pragma once

#define LANGUAGE_MANAGER StoryLanguageManager::getInstance()

#include <stdio.h>
#include <array>
#include "cocos2d.h"
#include "cocostudiomacro2.h"

using namespace cocos2d;
using namespace std;

#define LANGUAGE_EN_CODE    1
#define LANGUAGE_THAI_CODE  2
#define LANGUAGE_ID_CODE	3	//indo
#define LANGUAGE_VN_CODE    4

const std::map<int, std::string> LANG_ID_CODE = 
{
    {LANGUAGE_EN_CODE, "en_US"},
    {LANGUAGE_VN_CODE, "vi_VN"},
	{LANGUAGE_THAI_CODE, "th_TH"},
	{LANGUAGE_ID_CODE, "id_ID"} //indo
};

const std::map<std::string, int> LANG_CODE_ID = 
{
    {"en_US", LANGUAGE_EN_CODE},
    {"vi_VN", LANGUAGE_VN_CODE},
    {"th_TH", LANGUAGE_THAI_CODE},
	{"id_ID", LANGUAGE_ID_CODE}, //indo
};

const std::unordered_map<std::string, std::tuple<std::string, std::string>> LANGUAGES_INFO_EN = {
    {"en_US", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_us.png", "English (US)")},
    {"vi_VN", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Vietnamese")},
	{"th_TH", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Thailand")},
	{"id_ID", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Indonesia")},
};

const std::unordered_map<std::string, std::tuple<std::string, std::string>> LANGUAGES_INFO_VN = 
{
    {"en_US", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_us.png", "Tiếng Anh Mỹ")},
    {"vi_VN", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Tiếng Việt")},
	{"th_TH", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Tiếng Thái")},
	{"id_ID", std::make_tuple("mjstory/new_ui/icon_sidebar_flag_vn.png", "Tiếng Indo")},
};

class StoryLanguageManager 
{
public:
    static std::string getLanguageCodeById(int index);
    static int getLanguageIdByCode(std::string code);
    static int getCurrentLangId();
    static int getDisplayLangId();
    static bool setCurrentLanguage(int index);
    static bool setCurrentLanguage(std::string code);
    
    static std::unordered_map<std::string, std::tuple<std::string, std::string>> getLanguageStoriesInfo(std::string key);
    
    static StoryLanguageManager * getInstance();
    static StoryLanguageManager * __instance;
    
    void updateLanguageWithID(int lang_id);
    static ValueMap map_language_display;
    
    string getTextObjectWithKeyOfClass(string key_classes, string key_object, bool is_replace_linedown = false);
    string getTextObjectWithKeyObject(string key_object, bool is_replace_linedown = false);
    string replaceString(std::string subject, const std::string search, const std::string replace);

    std::string getTextByKey(int language_code, std::string key);
    std::string getTextByKey(std::string language_code, std::string key);
    std::string getTextByKeyContryCode(std::string key);
    std::string getDisplayTextByKey(std::string key, bool isSetTextByContryCode = false);
    void changeMapLangDisplay(int langID);
    void addTextByKey(int lang_id, std::string key, std::string text);

	enum class TextType {
		BOLD,
		ITALIC,
		REGULAR
	};

	std::string getFontByLang(TextType type, bool isSetTextByContryCode = false);

	void setFontByLang(Node* i_node, TextType type, bool isSetTextByContryCode = false);

	void setFontByLang(std::vector<Node*> i_nodes, TextType type, bool isSetTextByContryCode = false);

	void setTextAndFontForNodes(std::vector<std::tuple<Node*, std::string, TextType>> i_nodes, bool isSetTextByContryCode = false);

    void changeDisplayLanguage(Node * n);

    void setStringForNode(std::string content, Node* node);

    std::string getLine(std::string languageKeyContent, cocos2d::ui::Text* boundedNode);

private:
    cocos2d::ValueMap _lang_en_US;
    cocos2d::ValueMap _lang_vi_VN;
    cocos2d::ValueMap _lang_th_TH;
	cocos2d::ValueMap _lang_id_ID;

	std::string getKeyFromFile(cocos2d::ValueMap i_langFile, std::string i_key);
};

