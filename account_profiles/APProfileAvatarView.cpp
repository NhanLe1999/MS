//
//  APProfileAvatarView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APProfileAvatarView.h"
#include "APProfileFinishView.h"
#include "APProfile.h"
#include "api/APUpdateProfileAPI.h"
#include "APUpdateUserInfoAPI.h"
#include "MJHelper.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "MMFirebaseDatabase.h"
#include "manager/CleverTapManager.h"
#include "APDatabase.h"
#include "APProfileManager.h"


USING_NS_CC;

INITIALIZE_READER(APProfileAvatarView);

std::once_flag ap_profile_avatar_view_reader_flag;

APProfileAvatarView * APProfileAvatarView::createView(const std::string& name, const bool is_boy, const int age) {
	std::call_once(ap_profile_avatar_view_reader_flag, [] {
		REGISTER_CSB_READER(APProfileAvatarView);
	});

	auto node = reinterpret_cast<APProfileAvatarView *>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileAvatarPopup.csb"));
	if (node) {
		node->didLoadFromCSB(name, is_boy, age);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

void APProfileAvatarView::didLoadFromCSB(const std::string& name, const bool is_boy, const int age) {
	_name = name;
	_is_boy = is_boy;
	_age = age;

//    auto is_exist = FileUtils::getInstance()->isDirectoryExist("hdr/mjstory/avatar");
//    auto list_avatar = FileUtils::getInstance()->listFiles("mjstory/avatar");
//    list_avatar.erase(std::remove_if(list_avatar.begin(), list_avatar.end(), [](const std::string & p) {
//        if (p.length() <= 4)
//            return true;
//        const std::string ext = ".png";
//        return p.compare(p.length() - 4, 4, ext) != 0;
//    }), list_avatar.end());
    std::vector<std::string> list_avatar = {
        "boy_1.png", "boy_2.png", "boy_3.png", "boy_4.png", "boy_5.png", "boy_6.png", "boy_7.png", "boy_8.png",
        "girl_1.png", "girl_2.png", "girl_3.png", "girl_4.png", "girl_5.png", "girl_6.png", "girl_7.png"
    };
	mj::helper::shuffle(list_avatar);
	_listview = utils::findChild<ui::ListView*>(this, "listview");
	_listview->setMagneticType(ui::ListView::MagneticType::CENTER);
	_listview->setScrollBarEnabled(false);

	for (const auto & p : list_avatar) {
		auto wrapper = ui::Layout::create();
		wrapper->setContentSize(Size(175, 200));
		wrapper->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
		wrapper->setBackGroundColor(Color3B(random(0, 0xff), random(0, 0xff), random(0, 0xff)));
		auto found = p.find("mjstory/avatar");
		auto avatar_path = found != std::string::npos ? p.substr(found) : p;
		wrapper->setName(avatar_path);
		auto ava = ui::ImageView::create(p);
		ava->setContentSize(Size(200, 200));
		ava->setAnchorPoint(Point::ANCHOR_MIDDLE);
		ava->setPosition(Vec2(175.f/2,100));
		ava->setName("avatar");
		wrapper->addChild(ava);
		ava->ignoreContentAdaptWithSize(false);

		_listview->pushBackCustomItem(wrapper);
	}

	_listview->forceDoLayout();
	_listview->jumpToLeft();
	_listview->scrollToItem(7, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.5f);
}

void APProfileAvatarView::onEnter() {
	ui::Layout::onEnter();
    changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
	this->schedule(schedule_selector(APProfileAvatarView::updateFX));
//    auto csb_name = mj::helper::getCSBName("csb/account/ProfileAvatarPopup.csb");
     auto csb_name = "csb/account/ProfileAvatarPopup.csb";
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
}

void APProfileAvatarView::changeDisplayLanguage(int lang_id)
{
    if(auto text = utils::findChild<Text*>(this, "Text_1"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.avatar.title"));
    }
    
    if(auto text = utils::findChild<Text*>(this, "Text_1_0_0"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("new_finish"));
    }
}

void APProfileAvatarView::onExit()
{
	ui::Layout::onExit();
    
    MJDEFAULT->deleteValueForKey("will_create_new_profile");
}

void APProfileAvatarView::updateFX(float dt)
{
	auto items = _listview->getItems();
	const auto middle_point = Point(Director::getInstance()->getVisibleSize() / 2);
	const auto radius = 350.0f;
	const auto middle_radius = 125.0f;
	for (auto i : items) {
		auto position = i->getPosition() + i->getContentSize() / 2;
		position = i->getParent()->convertToWorldSpace(position);
		position = position - middle_point;
		auto ratio = 0.5f;
		if (fabs(position.x) < middle_radius)
		{
			ratio = sqrt(middle_radius * middle_radius - position.x * position.x);
			ratio = 0.5f + 0.5f * ratio / middle_radius;
		}
		i->getChildByName("avatar")->setScale(ratio);
		i->getChildByName("avatar")->setOpacity(ratio * 0xff);
	}
}

void APProfileAvatarView::onNext(Ref * sender)
{
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);

	// get current avatar
	this->unscheduleUpdate();
	_listview->stopOverallScroll();
	auto current_avatar = _listview->getCenterItemInCurrentView();
	_avatar = current_avatar->getName();

	// create profile to update
	ap::Profile profile;
	profile.profile_id;
	profile.name = _name;
	profile.gender = _is_boy ? ap::Profile::MALE : ap::Profile::FEMALE;
	profile.age = _age;
	profile.avatar = _avatar;

    pushCleverTapEvent("continue");

	ap::api::UpdateProfile::call(profile, [this, profile,button](bool is_success, std::string msg)
	{
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,is_success,msg,profile, button] {
			if (is_success)
			{
				if (_delegate)
				{
                    
					const int k_currentProfileID = MJDEFAULT->getIntegerForKey("key_last_profile_updated");
					try
					{
						auto profile = ap::Database::getInstance().getProfileById(k_currentProfileID);

						std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =
						{
						  {"view_screen", cocos2d::Value(true)},
						  {"profile_name", cocos2d::Value(profile.name)},
						  {"profile_gender", cocos2d::Value(profile.gender== ap::Profile::Gender::MALE?"M":"F")},
						  {"profile_age", cocos2d::Value(profile.age)},
						};

						CleverTapManager::GetInstance()->pushEvent("create_new_profile", eventTrackingProperties);
					}
					catch (const char * i_errorCode)
					{
						CCLOG("Error: cannot push story tracking data to server !");
					}

					_delegate->onProfileCreated(k_currentProfileID);
				}
				this->removeFromParent();
			}
			else
			{
				if (button) button->setTouchEnabled(true);
				bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
				//TODO: show notification and do something
                if(msg == ""){
                    StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                }else{
                    //TODO: show notification
                    StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
                }

			}
		});
	});
}

void APProfileAvatarView::onClose(cocos2d::Ref * sender)
{
    pushCleverTapEvent("exit");

//    auto csb_name = mj::helper::getCSBName("csb/account/ProfileAvatarPopup.csb");
     auto csb_name = "csb/account/ProfileAvatarPopup.csb";
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([this] {
		this->removeFromParent();
	});
	//	this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback APProfileAvatarView::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APProfileAvatarView, onNext),
		CLICK_MAP(APProfileAvatarView, onClose)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}


void APProfileAvatarView::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("profile_avatar",
    {
        {"avatar_name",cocos2d::Value(_avatar.empty()?"empty":_avatar)},
        {"click",cocos2d::Value(i_clickedType)},
    });
}

void  APProfileAvatarView::setEnableCloseButton(bool isEnableCloseButton)
{
    if(auto closeButton = utils::findChild<ui::Button*>(this, "btn_close"))
    {
        closeButton->setVisible(isEnableCloseButton);
    }
    
    if(auto layout = utils::findChild<ui::Layout*>(this, "layout"))
    {
        layout->setTouchEnabled(isEnableCloseButton);
    }
}
