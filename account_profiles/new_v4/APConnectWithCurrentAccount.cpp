#include "APConnectWithCurrentAccount.h"
#include "APProfile.h"
#include "APProfileCard.h"
#include "APLoginMethodsScreen.h"
#include "APAccountConnectionScreen.h"
#include "CleverTapManager.h"

INITIALIZE_READER(APConnectWithCurrentAccount);

std::once_flag v4_ap_connect_with_current_account_flag;

#define USER_NAME_MAX_LENGTH 23

#pragma mark - Initialize

APConnectWithCurrentAccount * APConnectWithCurrentAccount::createView(ap::api::Login::LoginDataInfo info)
{
	std::call_once(v4_ap_connect_with_current_account_flag, []
	{
		REGISTER_CSB_READER(APConnectWithCurrentAccount);
	});

	auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APConnectWithCurrentAccount.csb");

	auto p = static_cast<APConnectWithCurrentAccount*>(CSLoader::createNodeWithVisibleSize(csb_name));

	if (p && p->didLoadFromCSB(info))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APConnectWithCurrentAccount::didLoadFromCSB(ap::api::Login::LoginDataInfo info)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	animationMoveToLeft(this);

	m_info = info;

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
			  std::string k_userName	= "" ;
		const std::string k_fontName	= isThaiLandLanguage() ? "fonts/leelawdb.ttf": "fonts/Montserrat-Bold.ttf";
		const int		  k_fontSize	= 45;

		auto it = std::find(m_info.oldUserInfo._type.begin(), m_info.oldUserInfo._type.end(), ap::api::Login::FACEBOOK);

		if (it != m_info.oldUserInfo._type.end()) //found
		{
			k_userName = info.oldUserInfo._name;
		}
		else
		{
			k_userName = info.oldUserInfo._phone.empty() ? (info.oldUserInfo._mail.empty() ? info.oldUserInfo._name : info.oldUserInfo._mail) : info.oldUserInfo._phone;
		}

		//"<font color='#000000' size=\"%d\" face=\"%s\"> Kết nối với tài khoản <font color='#FFFFFF'> %s </font> để giữ nguyên lộ trình học của con </font>"
		std::string k_textXML = StringUtils::format(getDisplayTextByKey("key.v4.connect.with.current.account").c_str(),
			k_fontSize,
			k_fontName.c_str(),
			( resizeString(k_userName, USER_NAME_MAX_LENGTH)).c_str()
			);

		if (CONFIG_MANAGER->isVietnameseStories())
		{
			k_textXML = mj::helper::replace_string(k_textXML, "2CB8E9", "422e87");
		}

		auto richText =  ui::RichText::createWithXML(k_textXML);
		richText->ignoreContentAdaptWithSize(false);
		richText->setContentSize(title->getContentSize());
		richText->setPosition(title->getPosition());
		richText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
		title->getParent()->addChild(richText, title->getZOrder());

		title->removeFromParent();
	}

	auto item = cocos2d::utils::findChild<ui::Widget*>(this, "item");

	if (auto profileList = cocos2d::utils::findChild<ui::ListView*>(this, "profile_list"))
	{
		cocos2d::Size  listViewSize = cocos2d::Size(0, profileList->getContentSize().height);

		std::vector<APProfileCard*> cards;

		for (auto profile : info.profiles)
		{
			auto profileCard = APProfileCard::createCard(profile);
		
			listViewSize.width += profileCard->getContentSize().width + 66;

			cards.push_back(profileCard);
		}

		profileList->setContentSize(listViewSize);
		//profileList->setPosition(profileList->getParent()->getContentSize() / 2);
		for (auto card: cards)
		{
			profileList->pushBackCustomItem(card);
		}

		profileList->setScrollBarEnabled(false);
	}

	if (item)
	{
		item->removeFromParent();
	}

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		if (auto connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "connection_button"))
		{
			connectionButton->loadTextures("account/new_v2/register_button_vn.png", "account/new_v2/register_button_vn.png", "account/new_v2/register_button_vn.png");
		}
	}

	return true;
}

void APConnectWithCurrentAccount::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "profile_info_title"), "key.account.information",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "conection_button"), "key.v4.connection",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "skip_button"), "key.skip",APPopupBase::TextType::BOLD},
	});
}

void APConnectWithCurrentAccount::onExit()
{
	APPopupBase::onExit();
    
    if(!m_isClevertapTrackingPushed)
    {
        clevertapTracking("no");
    }
}

void APConnectWithCurrentAccount::onConnectionClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APConnectWithCurrentAccount::onShowPasswordButtonClicked");
		return;
	}

    clevertapTracking("yes");
    
	auto screen = APLoginMethodsScreen::createView(m_info, APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD);

	screen->setNeedToCallLinkAccount(true);

	this->getParent()->addChild(screen, this->getLocalZOrder());

	//this->removeFromParent();
}

void APConnectWithCurrentAccount::onSkipButton(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APConnectWithCurrentAccount::onShowPasswordButtonClicked");
		return;
	}

	ap::api::Login::LoginDataInfo info;

    info    =   m_info;
    
    info.oldUserInfo = ap::api::Login::NewUserInfo();
	
    //m_info.oldUserInfo = info.oldUserInfo;

	auto screen = APAccountConnectionScreen::createView(info);

    screen->setStartFrom(APAccountConnectionScreen::START_FROM::ConnectWithCurrentAccount);

	screen->changeTextTitle();

	this->getParent()->addChild(screen, this->getLocalZOrder());

	//this->removeFromParent();
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APConnectWithCurrentAccount::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APConnectWithCurrentAccount, onCancel),
		CLICK_MAP(APConnectWithCurrentAccount, onConnectionClicked),
		CLICK_MAP(APConnectWithCurrentAccount, onSkipButton),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void APConnectWithCurrentAccount::clevertapTracking(std::string connectSuccessfull)
{
    m_isClevertapTrackingPushed = true;
    
    try
    {
          CleverTapManager::GetInstance()->storePendingEvent("login_connection",
          {
              {"device_type",cocos2d::Value("old")},
              {"connection_type",cocos2d::Value("connect_with_old_user")},
              {"connect_successfull",cocos2d::Value(connectSuccessfull)},
              {"facebook_update_phone",cocos2d::Value("no")},
          });
    }
    catch (const std::runtime_error& e)
    {
       
    }
}
