#include "APHaveUsedMonkeyApps.h"
#include "APAccountConnectionScreen.h"
#include "APUpdatePhoneNumberScreen.h"

INITIALIZE_READER(APHaveUsedMonkeyApps);

#pragma mark - Initialize

//AP_CREATE_VIEW(APHaveUsedMonkeyApps, ap_have_used_monkey_apps_v4, "csb/account/new_v4/APHaveUsedMonkeyApps.csb")

std::once_flag v4_ap_have_used_monkey_apps_flag;

APHaveUsedMonkeyApps * APHaveUsedMonkeyApps::createView(ap::api::Login::LoginDataInfo i_info)
{
	std::call_once(v4_ap_have_used_monkey_apps_flag, []
	{
		REGISTER_CSB_READER(APHaveUsedMonkeyApps);
	});

	auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APHaveUsedMonkeyApps.csb");

	auto p = static_cast<APHaveUsedMonkeyApps*>(CSLoader::createNodeWithVisibleSize(csb_name));

	if (p && p->didLoadFromCSB(i_info))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APHaveUsedMonkeyApps::didLoadFromCSB(ap::api::Login::LoginDataInfo i_info)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_info = i_info;

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		if (auto  title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
			title->setTextColor(Color4B(66, 46, 135, 255));
		}

		if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "btn_use"))
		{
			button->loadTextures("account/new_v3/button_view_detail_vm.png", "account/new_v3/button_view_detail_vm.png", "account/new_v3/button_view_detail_vm.png");
		}

		if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "btn_not_use"))
		{
			button->loadTextures("account/new_v3/button_view_detail_vm.png", "account/new_v3/button_view_detail_vm.png", "account/new_v3/button_view_detail_vm.png");
		}
	}

	animationMoveToLeft(this);

    return true;
}

void APHaveUsedMonkeyApps::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"), "key.v4.have.used.monkey.apps",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "small_title"), "key.v4.free.or.not",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "btn_use"), "key.v4.have.used",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "btn_not_use"), "key.v4.not.use",APPopupBase::TextType::BOLD},
		});
}

void APHaveUsedMonkeyApps::onExit()
{
	APPopupBase::onExit();
}

void APHaveUsedMonkeyApps::onBack(cocos2d::Ref * sender)
{
   
}

void APHaveUsedMonkeyApps::onUseClicked(cocos2d::Ref * sender)
{
	//safeDisableButton(sender);

	auto screen = APAccountConnectionScreen::createView(m_info);
    screen->setStartFrom(APAccountConnectionScreen::START_FROM::HaveUsedMonkeyApps);
	this->getParent()->addChild(screen, this->getLocalZOrder());

	//this->removeFromParent();
}

void APHaveUsedMonkeyApps::onNotUseClicked(cocos2d::Ref * sender)
{
	//safeDisableButton(sender);

	auto screen = APUpdatePhoneNumberScreen::createView(m_info, APUpdatePhoneNumberScreen::START_FROM::HAVE_USED_APPS);

	this->getParent()->addChild(screen,this->getLocalZOrder());

	//this->removeFromParent();
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APHaveUsedMonkeyApps::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APHaveUsedMonkeyApps, onBack),
		CLICK_MAP(APHaveUsedMonkeyApps, onUseClicked),
		CLICK_MAP(APHaveUsedMonkeyApps, onNotUseClicked),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}
