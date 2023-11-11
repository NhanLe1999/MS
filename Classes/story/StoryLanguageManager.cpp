#include "StoryLanguageManager.h"
#include "JsonDump.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "MJDefinitions.h"
#define path_lang_plist "plist/lang_display_"

ValueMap StoryLanguageManager::map_language_display;

StoryLanguageManager * StoryLanguageManager::__instance = NULL;

StoryLanguageManager * StoryLanguageManager::getInstance()
{
    if (!__instance)
    {
		std::string path = StringUtils::format("%s%d.plist", path_lang_plist, MJDEFAULT->getIntegerForKey(key_lang_display, 1));

		map_language_display = FileUtils::getInstance()->getValueMapFromFile(path);

        __instance = new StoryLanguageManager();

		__instance->_lang_en_US = FileUtils::getInstance()->getValueMapFromFile("plist/lang/en_US.plist");

		__instance->_lang_vi_VN = FileUtils::getInstance()->getValueMapFromFile("plist/lang/vi_VN.plist");

		__instance->_lang_th_TH = FileUtils::getInstance()->getValueMapFromFile("plist/lang/th_TH.plist");

		__instance->_lang_id_ID = FileUtils::getInstance()->getValueMapFromFile("plist/lang/id_ID.plist");
    }

    return __instance;
}

void StoryLanguageManager::changeMapLangDisplay(int langID) {
	std::string path = StringUtils::format("%s%d.plist", path_lang_plist, langID);
	map_language_display = FileUtils::getInstance()->getValueMapFromFile(path);
}

std::string StoryLanguageManager::getTextByKey(int language_code, std::string key)
{
    if (language_code == LANGUAGE_EN_CODE)
    {
        return getTextByKey("en_US", key);
    }

    if (language_code == LANGUAGE_VN_CODE)
    {
        return getTextByKey("vi_VN", key);
    }

    if (language_code == LANGUAGE_THAI_CODE)
    {
        return getTextByKey("th_TH", key);
    }
    
	if (language_code == LANGUAGE_ID_CODE)
	{
		return getTextByKey("id_ID", key);
	}

    return key;
}

std::string StoryLanguageManager::getDisplayTextByKey(std::string key, bool isSetTextByContryCode)
{
    return isSetTextByContryCode ? getTextByKeyContryCode(key) : getTextByKey(getDisplayLangId(), key);
}

std::string StoryLanguageManager::getTextByKeyContryCode(std::string key)
{
	std::string contryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");
    
    if (contryCode == "VN")
	{
		return getKeyFromFile(_lang_vi_VN, key);
	}else if (contryCode == "TH")
	{
		return getKeyFromFile(_lang_th_TH, key);
	}else if (contryCode == "ID")
	{
		return getKeyFromFile(_lang_id_ID, key);
    }else{
        return getTextByKey(getDisplayLangId(), key);
    }

	return key;
}

std::string StoryLanguageManager::getTextByKey(std::string language_code, std::string key)
{
    if (language_code == "en_US")
    {
		return getKeyFromFile(_lang_en_US, key);
    }

    if (language_code == "vi_VN")
    {
		return getKeyFromFile(_lang_vi_VN, key);
    }
    
    if (language_code == "th_TH")
    {
		return getKeyFromFile(_lang_th_TH, key);
    }

	if (language_code == "id_ID")
	{
		return getKeyFromFile(_lang_id_ID, key);
	}

    return key;
}

void StoryLanguageManager::updateLanguageWithID(int lang_id)
{
	std::string path = StringUtils::format("%s%d.plist", path_lang_plist, lang_id);

    map_language_display = FileUtils::getInstance()->getValueMapFromFile(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + path) ? FileUtils::getInstance()->getWritablePath() + path : path);
    
	if ((int)map_language_display.size() == 0) 
	{
		path = StringUtils::format("%s%d.plist", path_lang_plist, LANGUAGE_EN_CODE);

		map_language_display = FileUtils::getInstance()->getValueMapFromFile(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + path) ? FileUtils::getInstance()->getWritablePath() + path : path);
    }
}

string StoryLanguageManager::getTextObjectWithKeyOfClass(string key_classes, string key_object, bool is_replace_linedown)
{
    string re_textobject;
    
    if (!map_language_display[key_classes].isNull() && map_language_display[key_classes].getType() == Value::Type::MAP)
    {
        re_textobject = map_language_display[key_classes].asValueMap()[key_object].asString();
    }
    
    if (is_replace_linedown)
    {
        re_textobject = replaceString(re_textobject, "*", "\n");
    }
    
    re_textobject = mj::helper::replace_string(re_textobject, "$(appname)", mj::PlatformConfig::getInstance().getAppName());
    
    return re_textobject;
}

string StoryLanguageManager::getTextObjectWithKeyObject(string key_object, bool is_replace_linedown)
{
    string re_textobject = map_language_display[key_object].asString();
    
    if (is_replace_linedown)
    {
        re_textobject = replaceString(re_textobject, "*", "\n");
    }
    
    re_textobject = mj::helper::replace_string(re_textobject, "$(appname)", mj::PlatformConfig::getInstance().getAppName());
    
    return re_textobject;
}

std::string StoryLanguageManager::replaceString(std::string subject, const std::string search,
                                          const std::string replace)
{
    size_t pos = 0;
    
    while((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    
    return subject;
}

int StoryLanguageManager::getCurrentLangId()
{
    return MJDEFAULT->getIntegerForKey(key_current_lang, LANGUAGE_EN_CODE);
}

int StoryLanguageManager::getDisplayLangId()
{
    return MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);
}

int StoryLanguageManager::getLanguageIdByCode(std::string code) {

    auto it = LANG_CODE_ID.find(code);
    if (it != LANG_CODE_ID.end())
        return it->second;
    else
        return LANG_CODE_ID.begin()->second;
}

std::string StoryLanguageManager::getLanguageCodeById(int index) {

    auto it = LANG_ID_CODE.find(index);
    if (it != LANG_ID_CODE.end())
        return it->second;
    else
        return LANG_ID_CODE.begin()->second;
}

bool StoryLanguageManager::setCurrentLanguage(int index) {
    
    if (index == getCurrentLangId())
    {
        return false;
    }
    MJDEFAULT->setIntegerForKey(key_current_lang, index);
    
    return true;
}

bool StoryLanguageManager::setCurrentLanguage(std::string code)
{
    if (code == getLanguageCodeById(getCurrentLangId()))
    {
        return false;
    }
    
    MJDEFAULT->setIntegerForKey(key_current_lang, LANG_CODE_ID.at(code));
    
    return true;
}

std::unordered_map<std::string, std::tuple<std::string, std::string>> StoryLanguageManager::getLanguageStoriesInfo(std::string key)
{
   return (key == "vi_VN") ? LANGUAGES_INFO_VN : LANGUAGES_INFO_EN;
}

void StoryLanguageManager::addTextByKey(int lang_id, std::string key, std::string text)
{
    if (lang_id == LANGUAGE_EN_CODE)
    {
        _lang_en_US.emplace(key, cocos2d::Value(text));
    }
    else
    {
        _lang_vi_VN.emplace(key, cocos2d::Value(text));
    }
}

std::string StoryLanguageManager::getKeyFromFile(cocos2d::ValueMap i_langFile, std::string i_key)
{
	if (i_langFile.count(i_key) > 0)
	{
		auto result = i_langFile.at(i_key).asString();
		result = mj::helper::replace_string(result, "$(appname)", mj::PlatformConfig::getInstance().getAppName());
		return result;
	}

	return i_key;
}

std::string StoryLanguageManager::getFontByLang(TextType type, bool isSetTextByContryCode)
{
	if (MJDEFAULT->getIntegerForKey("key_lang_display", LANGUAGE_EN_CODE) == LANGUAGE_THAI_CODE || (isSetTextByContryCode && MJDEFAULT->getStringForKey(key_country_code, "VN") == "TH"))
	{
		switch (type)
		{
		case StoryLanguageManager::TextType::BOLD:
			return "fonts/leelawdb.ttf";
			break;
		case StoryLanguageManager::TextType::ITALIC:
			return "fonts/leelawad.ttf";

			break;
		case StoryLanguageManager::TextType::REGULAR:
			return "fonts/leelawad.ttf";
			break;
		default:
			return "fonts/leelawdb.ttf";
			break;
		}
	}

	return "";
}

void StoryLanguageManager::setFontByLang(Node* i_node, TextType type, bool isSetTextByContryCode)
{
	if (!i_node)
	{
		return;
	}

	if (isSetTextByContryCode && MJDEFAULT->getStringForKey(key_country_code, "VN") != "TH")
	{
		return;
	}

	if (MJDEFAULT->getIntegerForKey("key_lang_display", LANGUAGE_EN_CODE) == LANGUAGE_THAI_CODE || (isSetTextByContryCode && MJDEFAULT->getStringForKey(key_country_code, "VN") == "TH"))
	{
		if (auto node = dynamic_cast<ui::Text*>(i_node))
		{
			node->setFontName(getFontByLang(type, isSetTextByContryCode));
		}

		if (auto node = dynamic_cast<ui::Button*>(i_node))
		{
			node->setTitleFontName(getFontByLang(type, isSetTextByContryCode));
		}

		if (auto node = dynamic_cast<ui::TextField*>(i_node))
		{
			node->setFontName(getFontByLang(type, isSetTextByContryCode));
		}

		if (auto node = dynamic_cast<cocos2d::Label*>(i_node))
		{
			auto config = node->getTTFConfig();
			config.fontFilePath = getFontByLang(type, isSetTextByContryCode);
			node->setTTFConfig(config);
		}
	}
}

void StoryLanguageManager::setFontByLang(std::vector<Node*> i_nodes, TextType type , bool isSetTextByContryCode)
{
	for (auto node : i_nodes)
	{
		if (!node)
		{
			continue;
		}

		setFontByLang(node, type, isSetTextByContryCode);
	}
}

void StoryLanguageManager::setTextAndFontForNodes(std::vector<std::tuple<Node*, std::string, TextType>> i_nodes, bool isSetTextByContryCode)
{
	for (auto nodeInfo : i_nodes)
	{
		auto node = std::get<0>(nodeInfo);
		auto textKey = std::get<1>(nodeInfo);
		auto fontType = std::get<2>(nodeInfo);

		if (!node)
		{
			continue;
		}

		setFontByLang(node, fontType, isSetTextByContryCode);

		if (textKey.empty())
		{
			continue;
		}

		if (auto castedNode = dynamic_cast<ui::Text*>(node))
		{
			castedNode->setText(LANGUAGE_MANAGER->getDisplayTextByKey(textKey, isSetTextByContryCode));
		}

		if (auto castedNode = dynamic_cast<ui::Button*>(node))
		{
			castedNode->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey(textKey, isSetTextByContryCode));
		}

		if (auto castedNode = dynamic_cast<ui::TextField*>(node))
		{
			castedNode->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey(textKey, isSetTextByContryCode));
		}

		if (auto castedNode = dynamic_cast<cocos2d::Label*>(node))
		{
			castedNode->setString(LANGUAGE_MANAGER->getDisplayTextByKey(textKey, isSetTextByContryCode));
		}
	}
}

void StoryLanguageManager::changeDisplayLanguage(Node * node) {

    static const std::string ks_prefix = "lb.";

    node->enumerateChildren(StringUtils::format("//%s.*", ks_prefix.c_str()), [](Node * child) {

        auto key = child->getName();

        if (key.size() >= ks_prefix.size() && key.substr(0, ks_prefix.size()) == ks_prefix)
        {
            key = key.substr(ks_prefix.size(), key.length());
            
            LANGUAGE_MANAGER->setStringForNode(LANGUAGE_MANAGER->getDisplayTextByKey(key), child);
        }

        return false;
    });

}


std::string StoryLanguageManager::getLine(std::string languageKeyContent, cocos2d::ui::Text* boundedNode)
{
    if (languageKeyContent.empty()) {
        return "";
    }

    string::size_type pos = languageKeyContent.find_last_of("|");

    if (pos != string::npos)
    {
        std::string templanguageKeyContent = languageKeyContent.substr(0, pos);

        std::string preTemplanguageKeyContent = templanguageKeyContent;

        templanguageKeyContent.erase(remove_if(templanguageKeyContent.begin(), templanguageKeyContent.end(), [](char c) { return c == '|'; }), templanguageKeyContent.end());

        auto text = ui::Text::create(templanguageKeyContent, boundedNode->getFontName(), boundedNode->getFontSize());

        cocos2d::Size textSize = text->getContentSize();

        if (textSize.width > boundedNode->getContentSize().width)
        {
            return getLine(preTemplanguageKeyContent, boundedNode);
        }
        else
        {
            return preTemplanguageKeyContent.empty() ? languageKeyContent : preTemplanguageKeyContent;
        }
    }
    else
    {
        return languageKeyContent;
    }

    return "";
}

void StoryLanguageManager::setStringForNode(std::string languageKeyContent, Node* node) {

    if (auto castedNode = dynamic_cast<cocos2d::ui::Text*>(node)) {

        if (languageKeyContent.find("|") != std::string::npos && LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE && castedNode->isCustomText()) {

            LANGUAGE_MANAGER->setFontByLang(castedNode, StoryLanguageManager::TextType::BOLD);

            std::string templanguageKeyContent = languageKeyContent;

            templanguageKeyContent.erase(remove_if(templanguageKeyContent.begin(), templanguageKeyContent.end(), [](char c) { return c == '|'; }), templanguageKeyContent.end());

            castedNode->setString(templanguageKeyContent);

            auto text = ui::Text::create(templanguageKeyContent, castedNode->getFontName(), castedNode->getFontSize());

            text->setString(templanguageKeyContent);

            cocos2d::Size textSize = text->getContentSize();

            if (textSize.width > castedNode->getContentSize().width)
            {
                std::string contentList;

                auto bestText = getLine(languageKeyContent, castedNode);

                std::string prelanguageKeyContent = languageKeyContent;

                while (!bestText.empty())
                {
                    std::string preBestText = bestText;

                    bestText.erase(remove_if(bestText.begin(), bestText.end(), [](char c) { return c == '|'; }), bestText.end());

                    contentList += bestText;

                    CCLOG("XXXXXXXXXXXX:preBT:%s", preBestText.c_str());
                    CCLOG("XXXXXXXXXXXX:BT:%s", bestText.c_str());

                    auto found = prelanguageKeyContent.find(preBestText);

                    if (found != std::string::npos) {

                        prelanguageKeyContent.erase(found, preBestText.length());

                        if (prelanguageKeyContent.find("|") != std::string::npos) {

                            contentList += "\n";

                            //if (prelanguageKeyContent.front() == '|') {
                            //    prelanguageKeyContent.erase(0, 1);
                            //}

                            bestText = getLine(prelanguageKeyContent, castedNode);
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }

                }
                castedNode->setString(contentList);

            }
        }
        else
        {
            if (languageKeyContent.find("|") != std::string::npos) {

                languageKeyContent.erase(std::remove_if(languageKeyContent.begin(), languageKeyContent.end(), [](const char& c) {
                    return c == '|';
                }), languageKeyContent.end());
            }

            castedNode->setString(languageKeyContent);
            LANGUAGE_MANAGER->setFontByLang(castedNode, StoryLanguageManager::TextType::BOLD);
        }
    }
    else if (auto castedNode = dynamic_cast<ui::TextField*>(node)) {

        if (languageKeyContent.find("|") != std::string::npos) {

            languageKeyContent.erase(std::remove_if(languageKeyContent.begin(), languageKeyContent.end(), [](const char& c) {
                return c == '|';
            }), languageKeyContent.end());
        }

        castedNode->setPlaceHolder(languageKeyContent);
        LANGUAGE_MANAGER->setFontByLang(castedNode, StoryLanguageManager::TextType::BOLD);
    }
    else if (auto castedNode = dynamic_cast<cocos2d::Label*>(node)) {

        if (languageKeyContent.find("|") != std::string::npos) {

            languageKeyContent.erase(std::remove_if(languageKeyContent.begin(), languageKeyContent.end(), [](const char& c) {
                return c == '|';
            }), languageKeyContent.end());
        }

        castedNode->setString(languageKeyContent);
        LANGUAGE_MANAGER->setFontByLang(castedNode, StoryLanguageManager::TextType::BOLD);
    }
    else if (auto castedNode = dynamic_cast<cocos2d::ui::Button*>(node)) {

        if (languageKeyContent.find("|") != std::string::npos) {

            languageKeyContent.erase(std::remove_if(languageKeyContent.begin(), languageKeyContent.end(), [](const char& c) {
                return c == '|';
            }), languageKeyContent.end());
        }

        castedNode->setTitleText(languageKeyContent);
        LANGUAGE_MANAGER->setFontByLang(castedNode, StoryLanguageManager::TextType::BOLD);
    }
}


