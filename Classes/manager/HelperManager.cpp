#include "HelperManager.h"
#include "StoryLanguageManager.h"
#include "popup/GettingUserInfoPopup.h"
#include "popup/RemindingUpdateInfoPopup.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MJCalculator.h"
#include "StoryConfigManager.h"
#include "StoryData.h"
USING_NS_CC;

void HelperManager::parseCountryCodesInfomationFromJson()
{
	const std::string k_filePath = "json/flags.json";

	std::string jsonStr = "";
	
	if (FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + k_filePath))
	{
		jsonStr =  FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + k_filePath);
	}
	else if(FileUtils::getInstance()->isFileExist(k_filePath))
	{
		jsonStr = FileUtils::getInstance()->getStringFromFile(k_filePath);
	}
	else
	{
		CCLOG("<HelperManager::parseCountryCodesInfomationFromJson>: cannot read file %s", k_filePath.c_str());

		return;
	}

	if (jsonStr.empty())
	{
		CCLOG("<HelperManager::parseCountryCodesInfomationFromJson>: empty file %s", k_filePath.c_str());

		return;
	}

	rapidjson::Document doc;

	doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>(jsonStr.c_str());

	if (doc.HasParseError())
    {
        CCLOG("<HelperManager::parseCountryCodesInfomationFromJson>: something wrong!");
        
		return;
	}

	if (!doc.IsObject())
    {
        CCLOG("<HelperManager::parseCountryCodesInfomationFromJson>: something wrong!");
        
		return;
	}

    m_phonCodeList.clear();
    
    const std::string k_countryCode = MJDEFAULT->getStringForKey(key_country_code, "VN");
    
    CCLOG("<HelperManager::parseCountryCodesInfomationFromJson>: defautl flag is= %s",k_countryCode.c_str());
    
    if (JSON_SAFE_CHECK(doc, "country_codes", Array))
    {
        auto countryCodes = doc["country_codes"].GetArray();

        for (size_t i = 0; i < countryCodes.Size(); i++)
        {
            auto viObj = countryCodes[i].GetObject();

			PhoneCodeInfo info = PhoneCodeInfo
			{
				viObj["key"].GetString(),
				viObj["name"].GetString(),
				viObj["code"].GetString(),
				k_countryCode == viObj["key"].GetString(),
			};
            
            if(info.selected)
            {
                MJDEFAULT->setStringForKey("phonecode_id",info.id);
                MJDEFAULT->setStringForKey("phonecode_code",info.code);
            }
            
            m_phonCodeList.push_back(info);
        }
    }

}

std::string HelperManager::getProfileAvatarPathFromConfigJson(std::string i_config)
{
	if (i_config.empty())
	{
		return i_config;
	}
	
	std::string path = "";

	rapidjson::Document doc;

	doc.Parse<rapidjson::kParseDefaultFlags>(i_config.c_str());

	if (!doc.HasParseError())
	{
		const std::string avatarPathKey = StringUtils::format("avatar_path_%d", CONFIG_MANAGER->isVietnameseStories()?51:40);

		if (JSON_SAFE_CHECK(doc, avatarPathKey.c_str(), String))
		{
			path = doc[avatarPathKey.c_str()].GetString();
		}
	}

	return  path;
}

std::string HelperManager::getFontPath(ui::Text* i_text, std::string i_defaultValue)
{
	if (!i_text)
	{
		return i_defaultValue;
	}

	std::string fontPath = i_defaultValue;

	if (auto label = dynamic_cast<Label*>(i_text->getVirtualRenderer()))
	{
		auto  config = label->getTTFConfig();

		fontPath = config.fontFilePath;
	}

	return fontPath;
}

void HelperManager::writeLogToFile(std::string log)
{
    std::string fileName    = FileUtils::getInstance()->getWritablePath() + "mslog.txt";
    
    std::string logStr      = "";
    
    if(FileUtils::getInstance()->isFileExist(fileName))
    {
        logStr += FileUtils::getInstance()->getStringFromFile(fileName);
        logStr += "\n";
    }
    
    logStr += log;
    logStr += "\n";
    
    FileUtils::getInstance()->writeStringToFile(logStr,fileName);
}

std::string HelperManager::convertToDatetimeForClevertap(int i_time)
{
    return StringUtils::format("%d",i_time);;
}

void HelperManager::addSpriteFramesWithFile(const std::string & plist, const std::string & textureFileName)
{
	const std::string k_textureFileInWriablePath = FileUtils::getInstance()->getWritablePath() + (StoryData::getInstance()->isHdr?"hdr":"hd") + textureFileName;

	if (FileUtils::getInstance()->isFileExist(k_textureFileInWriablePath))
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, k_textureFileInWriablePath);
	}
	else
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
	}

}

void HelperManager::addSidebarBlack()
{

}
void HelperManager::scrollingViewVibrate() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//scrollingViewVibrateIOS();
#endif
}

void HelperManager::trimSpace(cocos2d::Node* i_node)
{
	MK_RETURN_IF(!i_node);

	std::string input = "";

	if (auto textfield = dynamic_cast<ui::TextField*>(i_node)) {
		input = textfield->getString();
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_node)) {
		input = editbox->getText();
	}

	cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(input);

	MK_RETURN_IF(txt.getString().empty());

	auto new_char = txt.getString().at(txt.length() - 1)._char;

	while ((new_char == " " && !txt.getString().empty())) {

		txt.deleteChar(txt.length() - 1);

		if (txt.getString().empty()) {
			break;
		}

		new_char = txt.getString().at(txt.length() - 1)._char;
	}

	if (auto textfield = dynamic_cast<ui::TextField*>(i_node)) {
		textfield->setString(txt.getAsCharSequence());
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_node)) {
		editbox->setText(txt.getAsCharSequence().c_str());
	}
}

void HelperManager::removeSpace(cocos2d::Node* i_textField)
{
	MK_RETURN_IF(!i_textField);

	std::string txt = "";

	if (auto textfield = dynamic_cast<ui::TextField*>(i_textField))
	{
		txt = textfield->getString();
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_textField))
	{
		txt = editbox->getText();
	}

	MK_RETURN_IF(txt.empty());

	txt.erase(std::remove_if(txt.begin(), txt.end(), [](const char i_char) {

		return i_char == ' ';

		}), txt.end());

	if (auto textfield = dynamic_cast<ui::TextField*>(i_textField))
	{
		textfield->setString(txt);
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_textField))
	{
		editbox->setText(txt.c_str());
	}
}

void HelperManager::removeSpace(std::vector<cocos2d::Node*> i_textFields)
{
	for (auto textFiled : i_textFields)
	{
		removeSpace(textFiled);
	}
}
