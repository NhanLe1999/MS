#include "APProfileEditView.h"
#include "MJHelper.h"
#include "api/APUpdateProfileAPI.h"
#include "api/APDeleteProfileAPI.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "APDatabase.h"
#include "MMFirebaseDatabase.h"

USING_NS_CC;

std::once_flag ap_profile_edit_view_flag_reader_once;
INITIALIZE_READER(APProfileEditView);

APProfileEditView * APProfileEditView::createView(const ap::Profile & profile)
{
	std::call_once(ap_profile_edit_view_flag_reader_once, [] {
		REGISTER_CSB_READER(APProfileEditView);
	});

	auto view = reinterpret_cast<APProfileEditView*>(CSLoader::createNodeWithVisibleSize("account/ProfileEdit.csb"));
	if (view)
	{
		view->didLoadFromCSB(profile);
		return view;
	}
	CC_SAFE_DELETE(view);
	return nullptr;
}

void APProfileEditView::didLoadFromCSB(const ap::Profile & profile)
{
	_profile = profile;
	auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
	textfield->setCursorEnabled(true);
	textfield->setCursorChar('|');
	textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
	textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	textfield->setMaxLengthEnabled(true);
	textfield->setMaxLength(10);
	textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	textfield->setString(profile.name);
	onLoadListAvatar();
	if (profile.gender == ap::Profile::MALE)
	{
		onSelectBoy(this);
	}
	else
	{
		onSelectGirl(this);
	}

	auto layout_age = utils::findChild(this, "layout_age");
	onSelectAge(layout_age->getChildByTag(profile.age));

}

void APProfileEditView::onEnter()
{
	ui::Layout::onEnter();
    changeDisplayLanguage();
	this->schedule(schedule_selector(APProfileEditView::updateFX));
}

void APProfileEditView::onExit()
{
	ui::Layout::onExit();
}

void APProfileEditView::changeDisplayLanguage(){
    
    utils::findChild<Text*>(this, "Text_1")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.edit.title"));
    
    utils::findChild<Text*>(this, "Text_16")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.name"));
    
    utils::findChild<Text*>(this, "gender_label")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.gender"));
    
    utils::findChild<ui::TextField *>(this, "textfield")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.name.text"));
    
}

void APProfileEditView::onLoadListAvatar()
{
//    auto list_avatar = FileUtils::getInstance()->listFiles("mjstory/avatar");
//    list_avatar.erase(std::remove_if(list_avatar.begin(), list_avatar.end(), [](const std::string & p) {
//        if (p.length() <= 4)
//            return true;
//        const std::string ext = ".png";
//        return p.compare(p.length() - 4, 4, ext) != 0;
//    }), list_avatar.end());
    std::vector<std::string> list_avatar = {
        "boy_1.png", "boy_2.png", "boy_3.png", "boy_4.png", "boy_5.png", "boy_6.png", "boy_7.png", "boy_8.png",
        "girl_1.png", "girl_2.png", "girl_3.png", "girl_4.png", "girl_5.png", "girl_6.png", "girl_7.png", "girl_8.png"
    };
	mj::helper::shuffle(list_avatar);
	_listview = utils::findChild<ui::ListView*>(this, "listview");
	_listview->setMagneticType(ui::ListView::MagneticType::CENTER);
	_listview->setScrollBarEnabled(false);
	for (const auto & p : list_avatar) {
		auto wrapper = ui::Layout::create();
		wrapper->setContentSize(Size(200, 200));
		wrapper->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
		wrapper->setBackGroundColor(Color3B(random(0, 0xff), random(0, 0xff), random(0, 0xff)));
		wrapper->setName(p);
		auto ava = ui::ImageView::create(p);
		ava->setContentSize(Size(200, 200));
		ava->setAnchorPoint(Point::ANCHOR_MIDDLE);
		ava->setPosition(Point(100, 100));
		ava->setName("avatar");
		wrapper->addChild(ava);
		ava->ignoreContentAdaptWithSize(false);

		_listview->pushBackCustomItem(wrapper);
	}
	//TODO: Scrool to item avatar's profile
	int index = list_avatar.size()/2;
	for (int i = 0; i < list_avatar.size(); ++i)
	{
		if (_profile.avatar.find(list_avatar[i]) != std::string::npos)
		{
			index = i;
			break;
		}
	}
	_listview->forceDoLayout();
	_listview->jumpToLeft();
	_listview->scrollToItem(index, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE,1.0f);
}

void APProfileEditView::updateFX(float dt)
{
	auto items = _listview->getItems();
	const auto middle_point = _listview->getWorldPosition();
	const auto radius = 350.f;
	for (auto i : items) {
		auto position = i->getPosition() + i->getContentSize() / 2;
		position = i->getParent()->convertToWorldSpace(position);
		position = position - middle_point;
		auto ratio = 0.f;
		if (fabs(position.x) < radius) {
			ratio = sqrt(radius * radius - position.x * position.x);
			ratio = ratio / radius;
		}
		i->getChildByName("avatar")->setScale(ratio);
		i->getChildByName("avatar")->setOpacity(ratio * 0xff);
	}
}

void APProfileEditView::onSelectBoy(cocos2d::Ref * sender)
{
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	mj::helper::grayscaleImageV2(boy, false);
	boy->setTag(1);
	mj::helper::grayscaleImageV2(girl, true);
	girl->setTag(0);
}

void APProfileEditView::onSelectGirl(cocos2d::Ref * sender)
{
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	mj::helper::grayscaleImageV2(boy, true);
	boy->setTag(0);
	mj::helper::grayscaleImageV2(girl, false);
	girl->setTag(2);
}

void APProfileEditView::onSelectAge(cocos2d::Ref * sender)
{
	auto layout_age = utils::findChild(this, "layout_age");
	auto old_choose = layout_age->getChildByTag<ui::Button* > (_age);
	if (old_choose)
	{
		old_choose->setColor(Color3B::WHITE);
	}

	auto btn_age = reinterpret_cast<ui::Button*>(sender);
	_age = btn_age->getTag();
	btn_age->setColor(cocos2d::Color3B(0, 242, 0));
}

void APProfileEditView::onSave(cocos2d::Ref * sender)
{
	bool is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	auto gender = boy->getTag() | girl->getTag();
	auto name = utils::findChild<ui::TextField*>(this, "textfield")->getString();
	// get current avatar
	_listview->stopOverallScroll();
	auto current_avatar = _listview->getCenterItemInCurrentView();
	auto avatar = current_avatar->getName();
	bool is_valid_name = !name.empty() && ap::Database::getInstance().checkDuplicateName(name);
	if (!is_valid_name) {
		if (name.empty()) {
			StoryAlert::showAlert(is_vn ? "Vui lòng nhập tên của bé." : "Please enter your child's name", "OK", is_vn ? "Đóng" : "Close");
		}
		else
		{
			StoryAlert::showAlert(is_vn ? "Tên hồ sơ đã được tạo." : "Profile name has been created", "OK", is_vn ? "Đóng" : "Close");
		}
		return;
	}

	if (gender == 0) {
		StoryAlert::showAlert(is_vn ? "Vui lòng chọn giới tính của bé." : "Please choose your child's gender", "OK", is_vn ? "Đóng" : "Close");
		return;
	}

	if (_age == -1)
	{
		StoryAlert::showAlert(is_vn ? "Vui lòng chọn tuổi của bé" : "Please choose your child's age", "OK", is_vn ? "Đóng" : "Close");
		return;
	}
	_profile.name = name;
	_profile.age = _age;
	_profile.gender = gender == 1 ? ap::Profile::MALE : ap::Profile::FEMALE;
	_profile.avatar = avatar;
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
		ap::api::UpdateProfile::call(_profile, [this](bool is_success, std::string msg) {
			bool is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;
			if (is_success)
			{
				//TODO: show notification updated
				StoryAlert::showAlert(is_vn ? "Thông tin của bé đã được sửa thành công." : "The profile was updated successfully", "OK", is_vn ? "Đóng" : "Close");
			}
			else
			{
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

void APProfileEditView::onClickPhoto(cocos2d::Ref * sender)
{
	//TODO: select photo from device or capture an photo
}

//void APProfileEditView::onDelete(cocos2d::Ref * sender)
//{
//	ap::api::DeleteProfile::call(_profile.profile_id, [this](bool is_success, std::string message) {
//		if (is_success)
//		{
//			//TODO: show notification deleted
//			onClose(this);
//		}
//		else
//		{
//			//TODO: show notification
//		}
//	});
//}

void APProfileEditView::onClose(cocos2d::Ref * sender)
{
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
          ime->closeIME();
//    mj::PlatformConfig::getInstance().forceDismissKeyboard();
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.event.account.profile.edit.success", &_profile);
	this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback APProfileEditView::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
	CLICK_MAP(APProfileEditView, onSave),
	CLICK_MAP(APProfileEditView, onSelectBoy),
	CLICK_MAP(APProfileEditView, onSelectGirl),
	CLICK_MAP(APProfileEditView, onSelectAge),
	CLICK_MAP(APProfileEditView, onClickPhoto),
	//CLICK_MAP(APProfileEditView, onDelete),
	CLICK_MAP(APProfileEditView, onClose)
	};

	if (function_map.find(callBackName) != function_map.end()) {
		return function_map.at(callBackName);
	}

	return nullptr;
}
