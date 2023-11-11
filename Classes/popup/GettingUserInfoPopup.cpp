#include "GettingUserInfoPopup.h"
#include "StoryLanguageManager.h"
#include "manager/GettingUserInfoManager.h"
#include "manager/CleverTapManager.h"

#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "RewardPopup.h"
#include "MMFirebaseDatabase.h"

#define TEXTFIELD_REQUIRED " (*)"

USING_NS_CC;
INITIALIZE_READER(GettingUserInfoPopup);

std::once_flag getting_user_info_popup_flag;

GettingUserInfoPopup * GettingUserInfoPopup::createView(bool i_isskipButtonEnable, bool i_isRewardPopupShow)
{
	std::call_once(getting_user_info_popup_flag, [] {
		REGISTER_CSB_READER(GettingUserInfoPopup);
		});

	auto p = static_cast<GettingUserInfoPopup*>(CSLoader::createNodeWithVisibleSize("csb/popup/GettingUserInfoPopup.csb"));

	if (p && p->didLoadFromCSB(i_isskipButtonEnable, i_isRewardPopupShow))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool GettingUserInfoPopup::didLoadFromCSB(bool i_isskipButtonEnable, bool i_isRewardPopupShow)
{
	m_isskipButtonEnable = i_isskipButtonEnable;
	m_isRewardPopupShow = i_isRewardPopupShow;

	if (!PopupBase::didLoadFromCSB())
	{
		return false;
	}

	showingAnimation(utils::findChild(this, "popup_bg"));

	if (auto exitButton = cocos2d::utils::findChild<ui::Button*>(this, "exit_button"))
	{
		exitButton->setVisible(!i_isskipButtonEnable);
	}

	if (auto skipButton = cocos2d::utils::findChild<ui::Button*>(this, "skip_button"))
	{
		skipButton->setVisible(i_isskipButtonEnable);
	}

	if (auto finishButton = cocos2d::utils::findChild<ui::Button*>(this, "finish_button"))
	{
		finishButton->setEnabled(false);

		if (!i_isskipButtonEnable) 
		{
			if (auto panel = utils::findChild<ui::ImageView*>(this, "popup_bg"))
			{
				finishButton->setPositionX(panel->getContentSize().width / 2);
			}
		}
	}

	std::vector<ui::TextField*> textFields = 
	{
		cocos2d::utils::findChild<ui::TextField*>(this, "name_textfield"),
		cocos2d::utils::findChild<ui::TextField*>(this, "email_textfield")
	};

	auto userInfo = ap::Database::getInstance().getUserInfo();

	if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "name_textfield"))
	{
        txtField->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		txtField->setString(userInfo._name);
	}

	if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "email_textfield"))
	{
		txtField->setString(userInfo._mail);
	}

	if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield"))
	{
		txtField->setString(userInfo._phone);
	}

	for (auto textField : textFields)
	{
		textField->setPlaceHolderColor(Color4B(155, 155, 155, 255));
		textField->setCursorChar('|');
		textField->setCursorEnabled(true);
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		//get text from sever
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Regular.ttf");

        if(m_isRewardPopupShow)
        {
            title->setString(GettingUserInfoManager::GetInstance()->getGettingUserInfoPopupTitle());
        }
        else // start from settings menu, we dont show RewardPopup popup.
        {
            title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.information"));
        }
	}

	enableAButtonBasedOnTextfieldsLength(cocos2d::utils::findChild<ui::Button*>(this, "finish_button"), textFields, nullptr, 1);

	if (auto phoneTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield"))
	{
		phoneTextfield->setPlaceHolderColor(Color4B(155, 155, 155, 255));
		phoneTextfield->setCursorChar('|');
		phoneTextfield->setCursorEnabled(true);
		phoneTextfield->addEventListener([=](Ref *pSender, cocos2d::ui::TextField::EventType i_type) {

			auto box = utils::findChild(this, "popup_bg");

			switch (i_type)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			case TextField::EventType::ATTACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
				break;
			case TextField::EventType::DETACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
				break;
#endif
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			{
				validPhoneInput(phoneTextfield);

				(phoneTextfield->getString().size() == 0) ? phoneTextfield->setTextColor(Color4B(155, 155, 155, 255)) : phoneTextfield->setTextColor(Color4B::BLACK);

				//enableButtonBasedOnValidPhoneNumber(phoneTextfield->getString(), cocos2d::utils::findChild<ui::Button*>(this, "finish_button"));

				break;
			}
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			{
				validPhoneInput(phoneTextfield);

				(phoneTextfield->getString().size() == 0) ? phoneTextfield->setTextColor(Color4B(155, 155, 155, 255)) : phoneTextfield->setTextColor(Color4B::BLACK);

				//enableButtonBasedOnValidPhoneNumber(phoneTextfield->getString(), cocos2d::utils::findChild<ui::Button*>(this, "finish_button"));

				break;
			}
			default:
				break;
			}

		});

	}


	m_onCancelCallBack = [=]() { //khong bo vao onExit() vi cai nay chi start tinh khi cancel, success thi khong tinh

        pushCleverTapEvent("exit_button");
        
		GettingUserInfoManager::GetInstance()->startCalculationTime();

	};

	return true;
}

void GettingUserInfoPopup::onEnter()
{
	Layout::onEnter();
}

void GettingUserInfoPopup::onExit()
{
	Layout::onExit();

    hideVirtualKeyboardsOfTextFields();

	//bo trong day vi khi nao hien cai nay la gia tri isNewUser phai luon
	// la false de lan sau no se bo qua
	//update local -> isNewUser = false
	auto userInfo = ap::Database::getInstance().getUserInfo();

	//ap::Database::getInstance().saveIsNewUser(std::to_string(userInfo._id), false);
}

void GettingUserInfoPopup::onFinishButtonClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<ui::Button*>(sender);

	button->setEnabled(false);

	auto nameTextField = cocos2d::utils::findChild<ui::TextField*>(this, "name_textfield");
	
	auto emailTextField = cocos2d::utils::findChild<ui::TextField*>(this, "email_textfield");

	auto phoneTextField = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield");

	auto user_info		= ap::Database::getInstance().getUserInfo();
	user_info._name		= nameTextField->getString();
	user_info._mail		= emailTextField->getString();
	user_info._phone	= phoneTextField->getString();

    hideVirtualKeyboardsOfTextFields();
    
	if (!mj::helper::isValidEmail(emailTextField->getString()))
	{
		showErrorAlert(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"));//
		
		button->setEnabled(true);

		return;
	}

    if (m_isRewardPopupShow)
    {
        pushCleverTapEvent("ok_button");
    }
    
	ap::api::UpdateUserInfo::call(user_info, true, [this, button](bool is_success, std::string msg)
	{
		if (is_success)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				//update local -> isNewUser = false
				//auto userInfo = ap::Database::getInstance().getUserInfo();
				//ap::Database::getInstance().saveIsNewUser(std::to_string(userInfo._id), false);

				if (m_isRewardPopupShow)
				{
					Director::getInstance()->getRunningScene()->addChild(RewardPopup::createView(), INT_MAX - 1);
                    pushCleverTapEvent("ok_button");
				}
				else  // start from settings menu, we dont show RewardPopup popup.
				{
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.event.account.profile.edit.user.info.success");
				}
                                
                //CleverTapManager::GetInstance()->updateUserInfoForAllAccounts();
                
				GettingUserInfoManager::GetInstance()->deleteData();

				this->removeFromParentAndCleanup(true);

			});
		}
		else
		{
			showErrorAlert(msg);

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				button->setEnabled(true);
			});
		}

	});
}

void GettingUserInfoPopup::onSkipButtonClicked(cocos2d::Ref * sender)
{
	onCancel(sender);
}

void GettingUserInfoPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "email_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.popup.user.info.email") + TEXTFIELD_REQUIRED);
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "name_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.popup.user.info.name") + TEXTFIELD_REQUIRED);
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "phone_txt"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.popup.user.info.phone") );
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "warning_label"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("rating_entering_warning_text"));
	}
    
    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "finish_button_title"))
    {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Roboto-Bold.ttf");

        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_finish_button_title"));
    }
    
}

cocos2d::ui::Widget::ccWidgetClickCallback GettingUserInfoPopup::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(GettingUserInfoPopup, onCancel),
		CLICK_MAP(GettingUserInfoPopup, onSkipButtonClicked),
		CLICK_MAP(GettingUserInfoPopup, onFinishButtonClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}

void GettingUserInfoPopup::hideVirtualKeyboardsOfTextFields()
{
    if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "name_textfield"))
    {
        hideVirtualKeyboard(txtField);
    }
    
    if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "email_textfield"))
    {
        hideVirtualKeyboard(txtField);
    }
    
    if (auto txtField = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield"))
    {
        hideVirtualKeyboard(txtField);
    }
}

void GettingUserInfoPopup::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value("home_screen")},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("getting_user_info_popup")},
    });
}





