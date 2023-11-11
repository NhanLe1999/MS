#include "RemindingUpdateInfoPopup.h"
#include "StoryLanguageManager.h"
#include  "GettingUserInfoPopup.h"
#include "manager/GettingUserInfoManager.h"
#include "StoryConfigManager.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(RemindingUpdateInfoPopup);

std::once_flag reminding_update_info_popup_flag;

RemindingUpdateInfoPopup * RemindingUpdateInfoPopup::createView()
{
	std::call_once(reminding_update_info_popup_flag, [] {
		REGISTER_CSB_READER(RemindingUpdateInfoPopup);
		});

	auto p = static_cast<RemindingUpdateInfoPopup*>(CSLoader::createNodeWithVisibleSize("csb/popup/RemindingUpdateInfoPopup.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}


bool RemindingUpdateInfoPopup::didLoadFromCSB()
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

		GettingUserInfoManager::GetInstance()->increaseRemindingUpdateInfoPopupCount();
	};

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		//get text from sever;
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Roboto-Regular.ttf");

		title->setString(GettingUserInfoManager::GetInstance()->getRemaidingUpdateInfoPopupTitle());
	}

	if (auto okButton = cocos2d::utils::findChild<ui::Button*>(this, "ok_button"))
	{
		if (!CONFIG_MANAGER->isVietnameseStories())//MS
		{
			okButton->loadTextures("mjstory/new_ui/login/button seleced.png", "", "", ui::Widget::TextureResType::LOCAL);
		}
	}

	if (auto logo = cocos2d::utils::findChild<ui::ImageView*>(this, "logo"))
	{
		if (!CONFIG_MANAGER->isVietnameseStories())//MS
		{
			logo->setVisible(false);

			auto msLogo = Sprite::create("mjstory/monkey_logo.png");
			msLogo->setPosition(logo->getPosition()+Vec2(0, msLogo->getContentSize().height*0.05f));
			logo->getParent()->addChild(msLogo);
		}
	}

	//

	return true;
}

void RemindingUpdateInfoPopup::onEnter()
{
	Layout::onEnter();
}

void RemindingUpdateInfoPopup::onExit()
{
	Layout::onExit();
}

void RemindingUpdateInfoPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "finish_button_title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.yes"));
	}
}

void RemindingUpdateInfoPopup::onOKButtonClicked(cocos2d::Ref * i_sender)
{
    pushCleverTapEvent("ok_button");

	if (m_okButtonCallback)
	{
		m_okButtonCallback();
	}

	this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback RemindingUpdateInfoPopup::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(RemindingUpdateInfoPopup, onCancel),
		CLICK_MAP(RemindingUpdateInfoPopup, onOKButtonClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void RemindingUpdateInfoPopup::setOKButtonCallback(std::function<void()> i_callback)
{
	m_okButtonCallback = i_callback;
}

void RemindingUpdateInfoPopup::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value("home_screen")},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("reminding_update_info_popup")},
    });
}
