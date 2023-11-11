#include "APLoginMethodsScreen.h"
#include "APChangeDefaultPasswordScreen.h"
#include "APDatabase.h"
#include "APLoginView.h"
#include "APLinkUserCrmToAccount.h"
#include "APLoginScreen.h"
#include "CleverTapManager.h"

INITIALIZE_READER(APLoginMethodsScreen);

std::once_flag v4_ap_login_methods_screen_flag;

#pragma mark - Initialize

APLoginMethodsScreen * APLoginMethodsScreen::createView(ap::api::Login::LoginDataInfo i_info, ScreenType i_type)
{
	std::call_once(v4_ap_login_methods_screen_flag, []
	{
		REGISTER_CSB_READER(APLoginMethodsScreen);
	});

	auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APLoginMethodsScreen.csb");

	auto p = static_cast<APLoginMethodsScreen*>(CSLoader::createNodeWithVisibleSize(csb_name));

	if (p && p->didLoadFromCSB(i_info,i_type))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APLoginMethodsScreen::didLoadFromCSB(ap::api::Login::LoginDataInfo i_info, ScreenType i_type)
{
	m_needToCallLinkAccount = false;

	m_info = i_info;

	m_screenType = i_type;

	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	animationMoveToLeft(cocos2d::utils::findChild(this, "white_background"));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ap::api::Login::NewUserInfo info;

	std::copy(m_info.oldUserInfo._type.begin(), m_info.oldUserInfo._type.end(), back_inserter(info._type));

	std::copy(m_info.currentUserInfo._type.begin(), m_info.currentUserInfo._type.end(), back_inserter(info._type));

	auto remove = [=](std::vector<ap::api::Login::LoginType> &v)
	{
        auto end = v.end();
        
        for (auto it = v.begin(); it != end; ++it)
        {
            end = std::remove(it + 1, end, *it);
        }
        
        v.erase(end, v.end());
	};
	
	remove(info._type);

	if (info._type.size() == 2) //2 methods.
	{
		auto it = std::find(info._type.begin(), info._type.end(), ap::api::Login::FACEBOOK);

		if (it != info._type.end()) //found
		{
			m_loginMethodsInfo.fbName = m_info.oldUserInfo._name;

			m_methodsDiplayType = APLoginMethodsScreen::MethodsDisplayType::DISPLAY_2_METHOD_INCLUDING_FACEBOOK;
		}
		else
		{
			m_methodsDiplayType = APLoginMethodsScreen::MethodsDisplayType::DISPLAY_2_METHOD;
		}
	}
	else if (info._type.size() == 3) //3 methods.
	{
		auto it = std::find(info._type.begin(), info._type.end(), ap::api::Login::FACEBOOK);

		if (it != info._type.end()) //found
		{
			m_loginMethodsInfo.fbName = m_info.oldUserInfo._name;

			m_methodsDiplayType = APLoginMethodsScreen::MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK;
		}
		else
		{
			m_methodsDiplayType = APLoginMethodsScreen::MethodsDisplayType::DISPLAY_3_METHOD;
		}
	}
	else if (info._type.size() == 4)
	{
		CCLOG("4 method");
	}
	else
	{
		m_methodsDiplayType = APLoginMethodsScreen::MethodsDisplayType::NONE;
		CCLOG("something wrong!");
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_loginMethodsInfo.userName = m_info.currentUserInfo._userNameID;
	m_loginMethodsInfo.phoneNumber = m_info.oldUserInfo._phone;
	m_loginMethodsInfo.email = m_info.oldUserInfo._mail;

	if (m_screenType == ScreenType::CONTINUE_TO_CHANGE_PASSWORD &&
		(m_methodsDiplayType == MethodsDisplayType::DISPLAY_2_METHOD_INCLUDING_FACEBOOK ||
			m_methodsDiplayType == MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK))
	{
		auto it = std::find(info._type.begin(), info._type.end(), ap::api::Login::GOOGLE);

		if (it == info._type.end()) //not found login with email type -> hide email.
		{
			m_loginMethodsInfo.email = "";
		}
	}
	else if (m_screenType == ScreenType::CONTINUE_TO_SHOW_PROFILE &&
		!m_loginMethodsInfo.email.empty() &&
		(m_methodsDiplayType == MethodsDisplayType::DISPLAY_2_METHOD_INCLUDING_FACEBOOK ||
			m_methodsDiplayType == MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK))
	{
		auto it = std::find(info._type.begin(), info._type.end(), ap::api::Login::GOOGLE);

		if (it == info._type.end()) //not found login with email type -> hide email.
		{
			if (m_methodsDiplayType == MethodsDisplayType::DISPLAY_2_METHOD_INCLUDING_FACEBOOK)
			{
				m_methodsDiplayType = MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK;
			}
			else if (m_methodsDiplayType == MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK)
			{
				m_methodsDiplayType = MethodsDisplayType::DISPLAY_4_METHOD;
			}
		}
	}


	auto listView = cocos2d::utils::findChild<ui::ListView*>(this, "listview");

	auto item = cocos2d::utils::findChild<ui::Layout*>(this, "item");
	auto itemMedthosChanged = cocos2d::utils::findChild<ui::Layout*>(this, "item_methods_changed2");
	auto itemOr = cocos2d::utils::findChild<ui::Layout*>(this, "item_or");

	std::vector<cocos2d::Node*> items;

	std::string xmlFormat = "<font color='#000000' face='fonts/Montserrat-Medium.ttf' size='25'> %s : <font color='#29b0df' > %s </font> </font>";

	if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
	{
		xmlFormat = "<font color='#000000' face='fonts/leelawdb.ttf' size='25'> %s : <font color='#29b0df' > %s </font> </font>";
	}

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		xmlFormat = mj::helper::replace_string(xmlFormat, "29b0df", "422e87");
	}

	std::vector<std::string> infoList =
	{
		m_loginMethodsInfo.userName.empty() ? m_loginMethodsInfo.userName : StringUtils::format(xmlFormat.c_str(), getDisplayTextByKey("key.v4.username").c_str(), m_loginMethodsInfo.userName.c_str()),
		m_loginMethodsInfo.phoneNumber.empty() ? m_loginMethodsInfo.phoneNumber : StringUtils::format(xmlFormat.c_str(), getDisplayTextByKey("key.popup.user.info.phone").c_str(), m_loginMethodsInfo.phoneNumber.c_str()),
		m_loginMethodsInfo.email.empty() ? m_loginMethodsInfo.email : StringUtils::format(xmlFormat.c_str(), getDisplayTextByKey("key.v4.email").c_str(), +m_loginMethodsInfo.email.c_str()),
		m_loginMethodsInfo.fbName.empty() ? m_loginMethodsInfo.fbName : StringUtils::format(xmlFormat.c_str(), "Facebook", m_loginMethodsInfo.fbName.c_str()),
	};

    if(m_screenType == APLoginMethodsScreen::ScreenType::CONTINUE_TO_SHOW_PROFILE)
    {
        clevertapTracking(true);
    }
    
	infoList.erase(std::remove_if(
		infoList.begin(),
		infoList.end(),
		[](const std::string i_input) { return i_input.empty(); }),
		infoList.end());

	const std::string k_title = StringUtils::format(getDisplayTextByKey("key.v4.login.methods.screen.title").c_str(), (int)infoList.size());

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
		{
			title->setFontName("fonts/leelawdb.ttf");
		}

		title->setString(k_title);
	}

	cocos2d::Size listViewSize(listView->getContentSize().width, 0);

	std::vector<Widget*> nodes;

	const float k_childSpacingHeight = 20.0f;

	for (int i = 0; i < infoList.size(); i++)
	{
		auto newItem = item->clone();

		if (auto method_name = cocos2d::utils::findChild<ui::Text*>(newItem, "method_name"))
		{
			if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
			{
				method_name->setFontName("fonts/leelawdb.ttf");
			}

			method_name->setString(StringUtils::format(std::string(getDisplayTextByKey("key.v4.method") + ": %d").c_str(), i + 1));
		}

		if (auto des = cocos2d::utils::findChild<ui::Text*>(newItem, "des_txt"))
		{
			des->setString(infoList.at(i));

			auto richText = ui::RichText::createWithXML(infoList.at(i));
			richText->ignoreAnchorPointForPosition(false);
			richText->setAnchorPoint(des->getAnchorPoint());
			richText->ignoreContentAdaptWithSize(false);
			richText->setContentSize(des->getContentSize());
			richText->setPosition(des->getPosition());
			richText->setName(des->getName());
			des->getParent()->addChild(richText, des->getZOrder());

			des->removeFromParent();
		}

		listViewSize.height += newItem->getContentSize().height;
		listViewSize.height += k_childSpacingHeight;
		nodes.push_back(newItem);

		//listView->pushBackCustomItem(newItem);

		if (i < (infoList.size() - 1))
		{
			//listView->pushBackCustomItem(itemOr->clone());
			if ((m_screenType == ScreenType::CONTINUE_TO_CHANGE_PASSWORD && infoList.size() <= 3)||
				(m_screenType == ScreenType::CONTINUE_TO_SHOW_PROFILE && infoList.size() < 3))
			{
				auto itemOrNew = itemOr->clone();
				listViewSize.height += itemOrNew->getContentSize().height;
				listViewSize.height += k_childSpacingHeight;
				
				if (auto orText = cocos2d::utils::findChild<ui::Text*>(itemOrNew, "or_txt"))
				{
					if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
					{
						orText->setFontName("fonts/leelawdb.ttf");
					}

					orText->setString(StringUtils::format(orText->getString().c_str(), getDisplayTextByKey("key.or").c_str()));
				}

				nodes.push_back(itemOrNew);
			}
		}
		else
		{
			if (m_screenType == ScreenType::CONTINUE_TO_SHOW_PROFILE)
			{
				auto itemMedthosChangedNew = itemMedthosChanged->clone();

				if (auto changeTxt = cocos2d::utils::findChild<ui::Text*>(itemMedthosChangedNew, "change_txt"))
				{
					std::string methodsNumText = " ";

					switch (m_methodsDiplayType)
					{
					case APLoginMethodsScreen::MethodsDisplayType::DISPLAY_2_METHOD:
						methodsNumText = "1,2";
						break;
					case APLoginMethodsScreen::MethodsDisplayType::DISPLAY_2_METHOD_INCLUDING_FACEBOOK:
						methodsNumText = "1";
						break;
					case APLoginMethodsScreen::MethodsDisplayType::DISPLAY_3_METHOD:
						methodsNumText = "1,2,3";
						break;
					case APLoginMethodsScreen::MethodsDisplayType::DISPLAY_3_METHOD_INCLUDING_FACEBOOK:
						methodsNumText = "1,2";
						break;
					case APLoginMethodsScreen::MethodsDisplayType::DISPLAY_4_METHOD:
						methodsNumText = "1,2,3";
						break;
					case APLoginMethodsScreen::MethodsDisplayType::NONE:
						break;
					default:
						break;
					}

					//changeTxt->setString();

					auto richText = ui::RichText::createWithXML(StringUtils::format(getDisplayTextByKey("key.v4.methods.password.changed").c_str(), methodsNumText.c_str()));
					richText->ignoreAnchorPointForPosition(false);
					richText->setAnchorPoint(changeTxt->getAnchorPoint());
					richText->ignoreContentAdaptWithSize(false);
					richText->setContentSize(changeTxt->getParent()->getContentSize());
					richText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
					richText->setPosition({ changeTxt->getParent()->getContentSize().width / 2 ,-10.0f});
					richText->setName(changeTxt->getName());
					changeTxt->getParent()->addChild(richText, changeTxt->getZOrder());

					changeTxt->removeFromParent();
				}

				//listView->pushBackCustomItem(itemMedthosChangedNew);

				listViewSize.height += itemMedthosChangedNew->getContentSize().height;
				nodes.push_back(itemMedthosChangedNew);

			}
		}
	}

	const float k_layoutHeightMax = 439.0f;

	listView->setContentSize({ listViewSize.width, std::min(k_layoutHeightMax, listViewSize.height)});
	listView->setInnerContainerSize(listViewSize);

	for (auto node : nodes)
	{
		listView->pushBackCustomItem(node);
	}

	if (auto listViewLayout = cocos2d::utils::findChild<ui::ImageView*>(this, "listview_layout"))
	{
		listView->setScrollBarEnabled(listView->getInnerContainerSize().height > k_layoutHeightMax);

		listViewLayout->setContentSize(Size(listViewLayout->getContentSize().width, std::min(k_layoutHeightMax,listView->getInnerContainerSize().height + 30.0f)));

		listView->setPosition(listViewLayout->getContentSize() / 2);

		if (auto nextButton = cocos2d::utils::findChild<ui::Button*>(this, "next_button"))
		{
			nextButton->setPositionY(listViewLayout->getPositionY() - listViewLayout->getContentSize().height*0.5f - 25.0f - nextButton->getContentSize().height*0.5f);
		}

		if (auto title = cocos2d::utils::findChild(this, "title"))
		{
			title->setPositionY(listViewLayout->getPositionY() + listViewLayout->getContentSize().height*0.5f + 15.0f + title->getContentSize().height*0.5f);
		}
	}
	
	item->removeFromParent();
	itemMedthosChanged->removeFromParent();
	itemOr->removeFromParent();

	m_onCancelCallBack = [=]() {


	};

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		if (auto connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "next_button"))
		{
			connectionButton->loadTextures("account/new_v4/connection_button_vm.png", "account/new_v4/connection_button_vm.png", "account/new_v4/connection_button_vm.png");
		}

		if (auto backButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_back"))
		{
			backButton->loadTextures("account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png");
		}

		if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
			title->setTextColor(Color4B(66, 46, 135, 255));
		}

		if (auto image = cocos2d::utils::findChild<ui::ImageView*>(cocos2d::utils::findChild(this, "textfield_phone_bg"), "textfield_enable"))
		{
			image->loadTexture("account/new_v2/textfied_enable2_vm.png");
		}
	}

	return true;
}

void APLoginMethodsScreen::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Button*>(this, "next_button"), m_screenType == APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD ?"key.v4.change.password" :"ap_continue_button_title",APPopupBase::TextType::BOLD},
	});
}

void APLoginMethodsScreen::onExit()
{
	APPopupBase::onExit();
}

void APLoginMethodsScreen::onNextButtonClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APLoginMethodsScreen::onShowPasswordButtonClicked");
		return;
	}

	showWaittingView();

	if (m_needToCallLinkAccount)
	{
		linkAccountAPICall();
	}

	switch (m_screenType)
	{
		case APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD:
		{
			auto screen = APChangeDefaultPasswordScreen::createView(m_info, APChangeDefaultPasswordScreen::START_FROM::LOGIN_METHOS_SCREEN);
			this->getParent()->addChild(screen,this->getLocalZOrder());
			//this->removeFromParent();
			removeWaittingView();
			break;
		}
		case APLoginMethodsScreen::ScreenType::CONTINUE_TO_SHOW_PROFILE:
		{
			if (m_info.currentUserInfo._id <= 0) // login with google and facebook , currentUserInfo._id = -1
			{
				m_info.currentUserInfo = m_info.oldUserInfo;
			}
            
 			Database::getInstance().saveIdentity(std::to_string(m_info.currentUserInfo._id), m_info.oldAccessToken);
			Database::getInstance().saveUserInfo(std::to_string(m_info.currentUserInfo._id), _name, m_info.currentUserInfo._mail, m_info.currentUserInfo._phone, m_info.currentUserInfo._address, m_info.currentUserInfo._userName);
			MJDEFAULT->setBoolForKey("account_unverified", false);
			MJDEFAULT->setIntegerForKey(key_current_profile_id, 0);
			MJDEFAULT->setBoolForKey("is_link_account_processing", true);

			if (auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
			{
				loginView->onLoginSuccess("link_account");
			}
			else
			{
				removeWaittingView();
			}

			break;
		}
		default:
			break;
	}

}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APLoginMethodsScreen::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APLoginMethodsScreen, onCancel),
		CLICK_MAP(APLoginMethodsScreen, onNextButtonClicked),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void APLoginMethodsScreen::linkAccountAPICall()
{
	//showWaittingViewInCocosThread();

	APLinkUserCrmToAccount::call(m_info.newAccessToken, m_info.oldAccessToken, [=](bool i_isSuccess, std::string i_msg) {

		//removeWaittingView();

		if (i_isSuccess)
		{

		}
		else
		{
			if (!i_msg.empty())
			{
				showErrorAlert(i_msg);
			}
			else
			{
				showNetworkDownAlert(nullptr, [=]()
				{
					APLoginMethodsScreen::linkAccountAPICall();
				});
			}
		}
	});
}

void APLoginMethodsScreen::clevertapTracking(bool isAccountLinked)
{
    std::string loginable_type = "";
    
    std::vector<std::string> infoList =
    {
        m_loginMethodsInfo.userName.empty()     ? m_loginMethodsInfo.userName : "username",
        m_loginMethodsInfo.phoneNumber.empty()  ? m_loginMethodsInfo.phoneNumber :"phone",
        m_loginMethodsInfo.email.empty()        ? m_loginMethodsInfo.email : "email",
        m_loginMethodsInfo.fbName.empty()       ? m_loginMethodsInfo.fbName : "facebook",
    };
    
    infoList.erase(std::remove_if(
        infoList.begin(),
        infoList.end(),
        [](const std::string i_input) { return i_input.empty(); }),
        infoList.end());
    
    for(auto info : infoList)
    {
        if(!info.empty())
        {
            loginable_type += info;
            loginable_type += ",";
        }
    }
    
    if(loginable_type.at(loginable_type.size()-1) == ',')
    {
        loginable_type.pop_back();
    }
    
    try
    {
        CleverTapManager::GetInstance()->storePendingEvent("login_show_account_loginable", {
            {"loginable_type", cocos2d::Value(loginable_type) },
            {"count", cocos2d::Value((int)infoList.size()) },
            {"confirm_account", cocos2d::Value(isAccountLinked)},
        });
    }
    catch (const std::exception&)
    {
        
    }
}
