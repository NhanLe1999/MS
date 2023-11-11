#include "APProfileSwitch.h"
#include "MJDefault.h"
#include "APDatabase.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "MSEventLogger.h"
#include "APProfileManager.h"
#include "api/APUpdateUserInfoAPI.h"
#include "MSHome.h"

USING_NS_CC;

INITIALIZE_READER(APProfileSwitch);

std::once_flag ap_profile_switch_view_reader_flag;
APProfileSwitch * APProfileSwitch::createView()
{
	std::call_once(ap_profile_switch_view_reader_flag, [] {
		REGISTER_CSB_READER(APProfileSwitch);
	});

	auto node = reinterpret_cast<APProfileSwitch *>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileSwitchPopup.csb"));
	if (node) {
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

APProfileSwitch::~APProfileSwitch()
{
	
}

void APProfileSwitch::didLoadFromCSB()
{
	_listview = utils::findChild<ui::ListView *>(this, "listview");
	_listview->setScrollBarColor(Color3B(255, 255, 255));
	_listview->setScrollBarAutoHideEnabled(false);
	_listview->setScrollBarEnabled(true);
    _listview->setTouchEnabled(true);
    
	auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id, 0);

	auto list_profile = ap::Database::getInstance().getProfiles();

	_listview->removeAllItems();
	
    auto list_width = 0.0f;
    
	for (auto profile : list_profile)
	{
		APProfileItem * item = APProfileItem::createItem(profile, profile.profile_id == current_profile_id);
		item->setDelegate(this);
		_listview->pushBackCustomItem(item);
		list_width += item->getContentSize().width + _listview->getItemsMargin();
        _listview->setInnerContainerSize(Size(_listview->getContentSize().height, list_width));
	}
    
	if (list_width < _listview->getContentSize().width)
	{
		_listview->setContentSize(Size(list_width, _listview->getContentSize().height));
		_listview->setTouchEnabled(false);
	}
    
    _listview->setClippingEnabled(true);
	_listview->forceDoLayout();
}

void APProfileSwitch::onEnter()
{
	ui::Layout::onEnter();
	onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    ms::EventLogger::getInstance().logEvent("view_switch_profile", {});
	auto runani = CSLoader::createTimeline("csb/account/ProfileSwitchPopup.csb");
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
}

void APProfileSwitch::onExit()
{
	ui::Layout::onExit();
}

void APProfileSwitch::onClose(cocos2d::Ref * sender)
{
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(key_event_profile_switched);
	auto runani = CSLoader::createTimeline("csb/account/ProfileSwitchPopup.csb");
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([this] {
		this->removeFromParent();
	});
}

void APProfileSwitch::onChangeDisplayLanguage(int lang_id)
{
    if(auto text = utils::findChild<ui::Text*>(this, "Text_1"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.change.title"));
    }
}

void APProfileSwitch::onClickSelectItem(cocos2d::Ref * sender)
{
	auto item = (APProfileItem *)sender;
	item->setItemSelected(true);
	if (item->getProfile().profile_id == MJDEFAULT->getIntegerForKey(key_current_profile_id))
	{
		onClose(this);
		return;
	}

	for (auto item : _listview->getItems())
	{
		((APProfileItem *)item)->setItemSelected(false);
	}


	// switch profile
    ms::EventLogger::getInstance().logEvent("switch_profile", {});
	PROFILE_MANAGER.switchProfile(item->getProfile().profile_id, true);
	auto user_info = ap::Database::getInstance().getUserInfo();
	//update current profile id
    mm::RealtimeDatabase::getInstance().updateUserSettings(user_info._id);

	auto grade_id = -1;
	switch (MJDEFAULT->getIntegerForKey(key_current_hometab)) {
	case 0:
		//tab stories
		grade_id = PROFILE_MANAGER.getIntegerForKey(key_grade_stories, -1);
		break;
	case 1:
		//tab lesson
		grade_id = PROFILE_MANAGER.getIntegerForKey(key_grade_lessons, 84);
		break;
	case 2:
		//tab lesson phonic
		grade_id = PROFILE_MANAGER.getIntegerForKey(key_grade_lessons_phonic, 479);
		break;
	case 3:
		//tab audios
		grade_id = PROFILE_MANAGER.getIntegerForKey(key_grade_audiobooks, -84);
		break;

	default:
		break;
	}
	//   auto e = EventCustom(key_event_grade_changed);
	//   e.setUserData(&grade_id);
	//   Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);

	   //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_profile_switched);
	   //this->removeFromParent();

    PROFILE_MANAGER.resetFavoriteBooks();
    
	auto home = MSHome::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
    
    home->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f),CallFunc::create([=](){
        
        try
        {
            auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id, 0);
            
            auto list_profile        = ap::Database::getInstance().getProfiles();
            std::string profileName = "";
            for (auto profile : list_profile)
            {
                if (profile.profile_id == current_profile_id)
                {
                    profileName = profile.name;
                    break;
                }
            }
            
            CleverTapManager::GetInstance()->pushEvent("change_profile",
                                                       {
                {"view_screen",cocos2d::Value(true)},
                {"profile_name",cocos2d::Value(profileName)},
                {"profile_id",cocos2d::Value(current_profile_id)},
            });
        }
        catch (const std::exception&)
        {
            
        }
        
    })));

}

cocos2d::ui::Widget::ccWidgetClickCallback APProfileSwitch::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APProfileSwitch, onClose)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}

void APProfileSwitch::pushCleverTapEvent()
{
   
}
