#include "StoryParentEditProfile_Landscape.h"
#include "MJHelper.h"
#include "APUpdateProfileAPI.h"
#include "APDeleteProfileAPI.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "APDatabase.h"
#include "MMFirebaseDatabase.h"

USING_NS_CC;

std::once_flag story_parent_profile_edit_view_flag_reader_once;
INITIALIZE_READER(StoryParentEditProfile_Landscape);

StoryParentEditProfile_Landscape * StoryParentEditProfile_Landscape::createView(const ap::Profile & profile)
{
	std::call_once(story_parent_profile_edit_view_flag_reader_once, [] {
		REGISTER_CSB_READER(StoryParentEditProfile_Landscape);
	});
//    auto csb_name = mj::helper::getCSBName("csb/StoryParentEditProfile_Landscape.csb");
	auto view = reinterpret_cast<StoryParentEditProfile_Landscape*>(CSLoader::createNodeWithVisibleSize("csb/StoryParentEditProfile_Landscape.csb"));
	if (view)
	{
		view->didLoadFromCSB(profile);
		return view;
	}
	CC_SAFE_DELETE(view);
	return nullptr;
}

void StoryParentEditProfile_Landscape::didLoadFromCSB(const ap::Profile & profile)
{
	flag = true;
    onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
	_profile = profile;
	auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setTextColor(Color4B(77, 77, 77, 255-77)); // không hiểu sao nếu chỉ set ở cocos studio mà ko set ở đây thì cursor lại ra màu trắng
	textfield->setCursorEnabled(true);
	textfield->setCursorChar('|');
	textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
	textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	textfield->setMaxLengthEnabled(true);
	textfield->setMaxLength(10);
	textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	textfield->setString(profile.name);
	onLoadListAvatar();
    
    if(MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        if(auto layoutGender = utils::findChild(this, "layout_gender"))
        {
            layoutGender->setVisible(false);
        }
        
        if(auto layoutGender = utils::findChild(this, "layout_age"))
        {
            layoutGender->setVisible(false);
        }
    }
    
	if (profile.gender == ap::Profile::MALE)
	{
		onSelectBoy(this);
	}
	else
	{
		onSelectGirl(this);
	}
	_age = 0;
	auto layout_age = utils::findChild(this, "layout_age");
	onSelectAge(layout_age->getChildByTag(profile.age));
	registerKeyboardEvent();

	scheduleEnter([=]() {
		auto alertPopup = dynamic_cast<StoryAlert*>(utils::findChild(Director::getInstance()->getRunningScene(), "storyalert_popup"));
		if (alertPopup) {
			alertPopup->onOK(this);
			flag = true;
		}
		else {
			auto btn_save = utils::findChild<ui::Button*>(this, "btn_save");
			auto name = mj::helper::str_trim(utils::findChild<ui::TextField*>(this, "textfield")->getString());
			if (!name.empty() && flag) {
				onSave(btn_save);
				flag = false;
			}
		}
		});
	scheduleWindowsTab([=]() {
		auto textfield = utils::findChild<ui::TextField*>(this, "textfield");
		if (textfield) {
			textfield->attachWithIME();
		}
		});
}

void StoryParentEditProfile_Landscape::onEnter()
{
	ui::Layout::onEnter();
//    auto csb_name = mj::helper::getCSBName("csb/StoryParentEditProfile_Landscape.csb");
	auto runani = CSLoader::createTimeline("csb/StoryParentEditProfile_Landscape.csb");
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
	this->schedule(schedule_selector(StoryParentEditProfile_Landscape::updateFX));

}

void StoryParentEditProfile_Landscape::onChangeDisplayLanguage(int lang_id){
    
   
    std::vector<cocos2d::ui::Text*> textSameFonts =
    {
        utils::findChild<ui::Text*>(this, "Text_1"),
        utils::findChild<ui::Text*>(this, "Text_16"),
        utils::findChild<ui::Text *>(this, "gender_label"),
        utils::findChild<ui::Text *>(this, "text_age"),
        
    };
    
    for(auto text : textSameFonts)
    {
        if(!text)
        {
            continue;
        }
        
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    }

    utils::findChild<ui::TextField*>(this, "textfield")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<ui::Button*>(this, "btn_save")->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    utils::findChild<ui::Text*>(this, "Text_1")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.edit.profile"));
    utils::findChild<ui::Text*>(this, "Text_16")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.child.name"));
    utils::findChild<ui::TextField*>(this, "textfield")->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.name.text"));
    utils::findChild<ui::Text*>(this, "gender_label")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.gender"));
    utils::findChild<ui::Text*>(this, "text_age")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.edit.age"));
    utils::findChild<ui::Button*>(this, "btn_save")->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.save"));
}

void StoryParentEditProfile_Landscape::onLoadListAvatar()
{
//    auto is_exist = FileUtils::getInstance()->isFileExist("mjstory/avatar/boy_3.png");
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
		ava->setPosition(Vec2(175.f / 2, 100));
		ava->setName("avatar");
		wrapper->addChild(ava);
		ava->ignoreContentAdaptWithSize(false);

		_listview->pushBackCustomItem(wrapper);
	}
	//TODO: Scrool to item avatar's profile
	int index = list_avatar.size() / 2;
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
	_listview->scrollToItem(index, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 1.0f);
}

void StoryParentEditProfile_Landscape::updateFX(float dt)
{
	auto items = _listview->getItems();
	const auto middle_point = Point(Director::getInstance()->getVisibleSize() / 2);
	const auto radius = 350.0f;
	const auto middle_radius = 140.0f;
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

void StoryParentEditProfile_Landscape::onSelectBoy(cocos2d::Ref * sender)
{
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	//mj::helper::grayscaleImageV2(boy, false);
	boy->setTag(1);
	//mj::helper::grayscaleImageV2(girl, true);
	girl->setTag(0);

	boy->loadTexture("mjstory/new_ui/login/male.png");
	girl->loadTexture("mjstory/new_ui/login/female 1.png");
}

void StoryParentEditProfile_Landscape::onSelectGirl(cocos2d::Ref * sender)
{
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	//mj::helper::grayscaleImageV2(boy, true);
	boy->setTag(0);
	//mj::helper::grayscaleImageV2(girl, false);
	girl->setTag(2);
	boy->loadTexture("mjstory/new_ui/login/male 1.png");
	girl->loadTexture("mjstory/new_ui/login/female.png");
}

void StoryParentEditProfile_Landscape::onSelectAge(cocos2d::Ref * sender)
{
	auto layout_age = utils::findChild(this, "layout_age");
	auto old_choose = layout_age->getChildByTag<ui::Layout* >(_age);
	if (old_choose)
	{
		old_choose->setBackGroundImage("mjstory/new_ui/login/button age 2.png");
		utils::findChild<ui::Text*>(old_choose, "Text_1_0_0")->setTextColor(Color4B(41, 176, 223, 225));
	}

	auto btn_age = reinterpret_cast<ui::Layout*>(sender);

	if (btn_age)
	{
		_age = btn_age->getTag();
		btn_age->setBackGroundImage("mjstory/new_ui/login/button age.png");
		utils::findChild<ui::Text*>(btn_age, "Text_1_0_0")->setTextColor(Color4B::WHITE);
	}
	else
	{
		_age = 0;
	}

}

void StoryParentEditProfile_Landscape::onSave(cocos2d::Ref * sender)
{

	auto btn_save = (ui::Button *) (sender);
	btn_save->setTouchEnabled(false);
	bool is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	auto gender = boy->getTag() | girl->getTag();
	auto name = mj::helper::str_trim(utils::findChild<ui::TextField*>(this, "textfield")->getString());
	// get current avatar
	_listview->stopOverallScroll();
	auto current_avatar = _listview->getCenterItemInCurrentView();
	auto avatar = current_avatar->getName();
	bool is_valid_name = !name.empty() && !ap::Database::getInstance().checkDuplicateName(name,_profile.profile_id) && !mj::helper::isNameInValid(name); // kiểm tra tên đã trùng hay chưa
	auto layout_name = utils::findChild(this, "layout_name");
	auto layout_gender = utils::findChild(this, "layout_gender");
	auto layout_age = utils::findChild(this, "layout_age");
	
	hideError(layout_name);
	hideError(layout_gender);
	hideError(layout_age);

    if(MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        gender = 1;
        
        _age = 8;
    }
    
	std::string msg;
	
    if (!is_valid_name)
    {
        std::string str_error;
        if (name.empty()) {
            str_error = LANGUAGE_MANAGER->getDisplayTextByKey("key.please.enter.child.name");
        } else if (mj::helper::isNameInValid(name)) {
            str_error = LANGUAGE_MANAGER->getDisplayTextByKey("key.profile.invalid");
        } else {
            str_error = LANGUAGE_MANAGER->getDisplayTextByKey("key.profile.created");
        }
        showError(layout_name, str_error);
    }

	if (gender == 0)
    {
		showError(layout_gender,LANGUAGE_MANAGER->getDisplayTextByKey("key.please.select.gender."));
	}

	if (_age == 0)
	{
		msg = is_vn ? StringUtils::StringUTF8("Vui lòng chọn tuổi của bé").getAsCharSequence() : "Please choose your child's age";
		showError(layout_age,msg, true);
	}
	if (!is_valid_name || !gender || !_age)
	{
		btn_save->setTouchEnabled(true);
		return;
	}
	_profile.name = name;
	_profile.age = _age;
	_profile.gender = gender == 1 ? ap::Profile::MALE : ap::Profile::FEMALE;
	_profile.avatar = avatar;

	ap::api::UpdateProfile::call(_profile, [this](bool is_success, std::string msg) {
        
        if (is_success)
        {
            
            if(MJDEFAULT->getIntegerForKey(key_current_profile_id) == _profile.profile_id)
            {
                //Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.change.current.profile");
				ap::Database::getInstance().updateProfile(_profile);
				CleverTapManager::GetInstance()->pushProfileByID(MJDEFAULT->getIntegerForKey(key_current_profile_id), CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
            }
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                
                onClose(this);
                
            });
        }
        else
        {
            utils::findChild<ui::Button*>(this,"btn_save")->setTouchEnabled(true);
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
                
                StoryAlert::showAlert(msg.empty() ?
                                      LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error"):msg,
                                      "OK",
                                      msg.empty() ?"": LANGUAGE_MANAGER->getDisplayTextByKey("StickerListScene_btn_close"));
            });
        }
	});

}


void StoryParentEditProfile_Landscape::onClose(cocos2d::Ref * sender)
{
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
             ime->closeIME();
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.event.account.profile.edit.success", &_profile);
//    auto csb_name = mj::helper::getCSBName("csb/StoryParentEditProfile_Landscape.csb");
	auto runani = CSLoader::createTimeline("csb/StoryParentEditProfile_Landscape.csb");
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([=] {
		this->removeFromParent();
	});
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentEditProfile_Landscape::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
	CLICK_MAP(StoryParentEditProfile_Landscape, onSave),
	CLICK_MAP(StoryParentEditProfile_Landscape, onSelectBoy),
	CLICK_MAP(StoryParentEditProfile_Landscape, onSelectGirl),
	CLICK_MAP(StoryParentEditProfile_Landscape, onSelectAge),
	CLICK_MAP(StoryParentEditProfile_Landscape, onClose)
	};

	if (function_map.find(callBackName) != function_map.end()) {
		return function_map.at(callBackName);
	}

	return nullptr;
}

void StoryParentEditProfile_Landscape::hideError(cocos2d::Node * sender)
{
	auto img_error = utils::findChild<ui::ImageView*>(sender, "img_error");
	auto text_error = utils::findChild<ui::Text*>(sender, "text_error");

	if (img_error) img_error->setVisible(false);
	if (text_error) text_error->setVisible(false);
}

void StoryParentEditProfile_Landscape::showError(cocos2d::Node * sender, std::string message, bool update_pos)
{
	auto img_error = utils::findChild<ui::ImageView*>(sender, "img_error");
	auto text_error = utils::findChild<ui::Text*>(sender, "text_error");
    
    text_error->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
        text_error->setFontSize(15);
    } else {
        text_error->setFontSize(16);
    }

	if (!img_error || !text_error) return;

	img_error->setVisible(true);
	text_error->setVisible(true);

	text_error->setString(message);
	if(update_pos)
		img_error->setPositionX(text_error->getPositionX() - text_error->getVirtualRendererSize().width / 2 - img_error->getContentSize().width / 2);
}

void StoryParentEditProfile_Landscape::scheduleEnter(std::function<void()> _callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->schedule([=](float dt) {

		if (_pressedKeys.size())
		{
			if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_ENTER) != _pressedKeys.end()) {

				if (_callback) {

					_pressedKeys.clear();
					_callback();
				}
			}
		}
		}, "scheduleEnter");
#endif
}

void StoryParentEditProfile_Landscape::registerKeyboardEvent()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		//_callback(keyCode, event);
		_pressedKeys.insert(keyCode);
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		_pressedKeys.erase(keyCode);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

#endif
}

void StoryParentEditProfile_Landscape::scheduleWindowsTab(std::function<void()> _callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->schedule([=](float dt) {

		if (_pressedKeys.size())
		{
			if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_SHIFT) != _pressedKeys.end())
			{
				return;
			}
			else if (_pressedKeys.find(EventKeyboard::KeyCode::KEY_TAB) != _pressedKeys.end())
			{

				if (_callback) {
					_pressedKeys.clear();
					_callback();
				}

			}
		}

		}, "scheduleWindowsTab");
#endif
}
