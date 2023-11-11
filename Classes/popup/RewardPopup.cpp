#include "RewardPopup.h"
#include "StoryLanguageManager.h"
#include  "GettingUserInfoPopup.h"
#include "manager/GettingUserInfoManager.h"
#include "StoryConfigManager.h"
#include "CleverTapManager.h"
#include <regex>

USING_NS_CC;
INITIALIZE_READER(RewardPopup);

std::once_flag reward_popup_flag;

RewardPopup * RewardPopup::createView()
{
	std::call_once(reward_popup_flag, [] {
		REGISTER_CSB_READER(RewardPopup);
		});

	auto p = static_cast<RewardPopup*>(CSLoader::createNodeWithVisibleSize("csb/popup/RewardPopup.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}


bool RewardPopup::didLoadFromCSB()
{
	if (!PopupBase::didLoadFromCSB())
	{
		return false;
	}

	showingAnimation(utils::findChild(this, "popup_bg"));

	m_onCancelCallBack = [=]() 
	{
        pushCleverTapEvent("exit_button");

		GettingUserInfoManager::GetInstance()->startCalculationTime();
	};

	auto languageList = GettingUserInfoManager::GetInstance()->getLanguageListFromServer();

	if (!languageList.empty())
	{
        const std::string k_downloadButtonLabelKey  = StringUtils::format("%s_link_download_ebook_lable", GettingUserInfoManager::GetInstance()->getLanguageKey().c_str());
        const std::string k_urlKey                  = StringUtils::format("%s_link_download_ebook_link", GettingUserInfoManager::GetInstance()->getLanguageKey().c_str());
        const std::string k_titleTxtKey             = StringUtils::format("%s_link_download_ebook_detail", GettingUserInfoManager::GetInstance()->getLanguageKey().c_str());

        std::string buttonText	= languageList.count(k_downloadButtonLabelKey) ? languageList.at(k_downloadButtonLabelKey):LANGUAGE_MANAGER->getDisplayTextByKey("lesson.item.download");
        m_url					= languageList.count(k_urlKey) ? languageList.at(k_urlKey):"";
		std::string titleTxt	= languageList.count(k_titleTxtKey) ? languageList.at(k_titleTxtKey):"";

		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
			title->setVisible(false);

			titleTxt = std::regex_replace(titleTxt, std::regex("###"), m_url);

			title->setString(titleTxt);

            std::string titleConfig = "<font color='#000000' size='26' face='fonts/Roboto-Regular.ttf'>"+ titleTxt + "</font>";
            
            if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE )
            {
                titleConfig = "<font color='#000000' size='26' face='fonts/leelawad.ttf'>"+ titleTxt + "</font>";
                
                title->setFontName("fonts/leelawad.ttf");

                title->setString(titleTxt);
            }

			auto richText  = ui::RichText::createWithXML(titleConfig);
			richText->ignoreContentAdaptWithSize(false);
			richText->setContentSize(title->getContentSize());
			richText->setPosition(title->getPosition());
			richText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
			title->getParent()->addChild(richText, title->getZOrder()+1);
		}

		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "download_title"))
		{
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Roboto-Bold.ttf");

			title->setText(buttonText);
		}
	}

	if (auto logo = cocos2d::utils::findChild<ui::ImageView*>(this, "logo"))
	{
		if (!CONFIG_MANAGER->isVietnameseStories())//MS
		{
			logo->setVisible(false);
			auto msLogo = Sprite::create("mjstory/monkey_logo.png");
			msLogo->setScale(0.85f);
			msLogo->setPosition(logo->getPosition() + Vec2(0, msLogo->getContentSize().height*0.15f));
			logo->getParent()->addChild(msLogo);
		}
	}
	return true;
}

void RewardPopup::onEnter()
{
	Layout::onEnter();
}

void RewardPopup::onExit()
{
	Layout::onExit();
}

void RewardPopup::changeDisplayLanguage()
{
	
}

void RewardPopup::onDownloadButtonClicked(cocos2d::Ref * i_sender)
{
    pushCleverTapEvent("download_button");
    
	if (!m_url.empty())
	{
		Application::getInstance()->openURL(m_url);
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback RewardPopup::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(RewardPopup, onCancel),
		CLICK_MAP(RewardPopup, onDownloadButtonClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void RewardPopup::setOKButtonCallback(std::function<void()> i_callback)
{
	//m_okButtonCallback = i_callback;
}

void RewardPopup::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value("home_screen")},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("reward_popup")},
    });
}




