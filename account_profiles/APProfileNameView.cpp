//
//  APProfileNameView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APProfileNameView.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "APDatabase.h"
#include "APProfileAgeView.h"
#include "MSEventLogger.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "MJDefault.h"

USING_NS_CC;

INITIALIZE_READER(APProfileNameView);

std::once_flag ap_profile_name_view_reader_flag;
APProfileNameView * APProfileNameView::createView()
{
    std::call_once(ap_profile_name_view_reader_flag, []
    {
        REGISTER_CSB_READER(APProfileNameView);
    });
    
    auto node = reinterpret_cast<APProfileNameView *>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileNamePopup.csb"));
    
    if (node)
    {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    
    return nullptr;
}


void APProfileNameView::didLoadFromCSB()
{
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setCursorEnabled(true);
    textfield->setCursorChar('|');
    textfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
    textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
    textfield->setMaxLengthEnabled(true);
    textfield->setMaxLength(10);
    textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	textfield->addEventListener([=](Ref * sender, ui::TextField::EventType event)
	{
		switch (event)
		{
		case TextField::EventType::DELETE_BACKWARD:
		case TextField::EventType::INSERT_TEXT:
			this->doCheckNext();
			break;
		default:
			break;
		}
	});

	auto icon_boy = utils::findChild<ui::ImageView *>(this, "icon_boy");
	icon_boy->setTag(0);
	auto icon_girl = utils::findChild<ui::ImageView *>(this, "icon_girl");
	icon_girl->setTag(0);
	//mj::helper::grayscaleImageV2(icon_boy, true);
	//mj::helper::grayscaleImageV2(icon_girl, true);
	icon_boy->loadTexture("mjstory/new_ui/login/male 1.png");
	icon_girl->loadTexture("mjstory/new_ui/login/female 1.png");

    if(auto genderLayout = utils::findChild(this, "layout_gender"))
    {
        genderLayout->setVisible(!MJDEFAULT->getBoolForKey(key_is_submit, false));
    }
    
	doCheckNext();
}

void  APProfileNameView::setEnableCloseButton(bool isEnableCloseButton)
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

void APProfileNameView::onEnter()
{
    ui::Layout::onEnter();
    changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    ms::EventLogger::getInstance().logEvent("view_profile_name", {});
    //    auto csb_name = mj::helper::getCSBName("csb/account/ProfileNamePopup.csb");
    auto csb_name = "csb/account/ProfileNamePopup.csb";
    auto runani = CSLoader::createTimeline(csb_name);
    runani->setTag(1014);
    this->runAction(runani);
    runani->play("show", false);
}

void APProfileNameView::onExit()
{
    ui::Layout::onExit();
}

void APProfileNameView::changeDisplayLanguage(int lang_id)
{
    utils::findChild<Text*>(this, "Text_1")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<Text*>(this, "Text_2")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<TextField*>(this, "textfield")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<Text*>(this, "gender_label")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<Text*>(this, "Text_1_0_0")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    
    utils::findChild<Text*>(this, "Text_1")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.name.title.new"));
    utils::findChild<Text*>(this, "Text_2")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.name.title.des"));
    utils::findChild<TextField*>(this, "textfield")->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.name.text"));
    //     utils::findChild<Text*>(this, "gender_label")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(" key.parent.profile.gender"));
    utils::findChild<Text*>(this, "gender_label")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.profile.gender"));
    //    utils::findChild<Text*>(this, "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(" key.account.btn.next"));
    utils::findChild<Text*>(this, "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_continue_button_title"));
}

void APProfileNameView::onNext(Ref* sender) {

    ms::EventLogger::getInstance().logEvent("new_profile_created", {});
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);

	// Validate name
	auto name = utils::findChild<ui::TextField*>(this, "textfield")->getString();

	bool is_valid_name = (!name.empty() && !ap::Database::getInstance().checkDuplicateName(name));
	
    if (!is_valid_name)
    {
		auto name_icon = utils::findChild(this, "name_icon");

		auto action_required_indicator_icon = Repeat::create(Sequence::create(TintTo::create(0.25, Color3B::RED), DelayTime::create(0.5), TintTo::create(0.25, Color3B::WHITE), nullptr), 5);
		name_icon->runAction(action_required_indicator_icon);

		if (name.empty())
        {
            std::string msg = LANGUAGE_MANAGER->getDisplayTextByKey("key.please.enter.child.name");
            
			StoryAlert::showAlert(msg, "OK", LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
		}
		else
		{
            std::string msg = LANGUAGE_MANAGER->getDisplayTextByKey("key.profile.created");

            StoryAlert::showAlert(msg , "OK", LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
		}
        
		this->scheduleOnce([=](float)
		{
			button->setTouchEnabled(true);
		}, 0.2f, "delay show alert" );
		
		return;
	}

	// Validate gender
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");

	auto gender = boy->getTag() | girl->getTag();
    
    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        gender = 1;
    }
    
    if (gender == 0)
    {
        auto gender_label = utils::findChild(this, "gender_label");
        
        auto action_required_indicator_label = Repeat::create(Sequence::create(TintTo::create(0.25, Color3B::RED), DelayTime::create(0.5), TintTo::create(0.25, mj::helper::colorFromHex(0x888888)), nullptr), 3);
        
        gender_label->runAction(action_required_indicator_label);
        
        std::string msg = LANGUAGE_MANAGER->getDisplayTextByKey("key.please.select.gender");
        
        StoryAlert::showAlert(msg, "OK",LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
        
        this->scheduleOnce([=](float) {
            
            button->setTouchEnabled(true);
            
        }, 0.2f, "delay show alert");
        
        return;
    }

    pushCleverTapEvent("continue");
    
    auto age_view = APProfileAgeView::createView(name, gender == 1);
    age_view->setEnableCloseButton(!MJDEFAULT->getBoolForKey("will_create_new_profile", false));
	age_view->setDelegate(_delegate);
	this->getParent()->addChild(age_view,INT_MAX-1);
	this->removeFromParent();
}

void APProfileNameView::onSelectBoy(cocos2d::Ref * sender)
{
	auto boy    = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl   = utils::findChild<ui::ImageView*>(this, "icon_girl");
	boy->loadTexture("mjstory/new_ui/login/male.png");
	girl->loadTexture("mjstory/new_ui/login/female 1.png");
	//mj::helper::grayscaleImageV2(boy, false);
	//mj::helper::grayscaleImageV2(girl, true);
	boy->setTag(1);
	girl->setTag(0);

	doCheckNext();
}

void APProfileNameView::onSelectGirl(cocos2d::Ref * sender)
{
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	boy->loadTexture("mjstory/new_ui/login/male 1.png");
	girl->loadTexture("mjstory/new_ui/login/female.png");
	//mj::helper::grayscaleImageV2(boy, true);
	//mj::helper::grayscaleImageV2(girl, false);
	boy->setTag(0);
	girl->setTag(2);

	doCheckNext();
}

void APProfileNameView::onClose(cocos2d::Ref * sender)
{
    if(auto button = dynamic_cast<ui::Button*>(sender))
    {
        button->setTouchEnabled(false);
    }
    
    pushCleverTapEvent("exit");
    
    ms::EventLogger::getInstance().logEvent("profile_create_age_exit", {});
    
	mj::PlatformConfig::getInstance().forceDismissKeyboard();
    auto csb_name = "csb/account/ProfileNamePopup.csb";
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([=] {
        
		this->removeFromParent();
        
	});
}

void APProfileNameView::doCheckNext()
{
	auto name = utils::findChild<ui::TextField*>(this, "textfield")->getString();
	auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
	auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
	auto gender = boy->getTag() | girl->getTag();

	auto can_next = MJDEFAULT->getBoolForKey(key_is_submit, false)? (!name.empty()):(!name.empty() && gender != 0);
	auto btn_next = utils::findChild<ui::Button*>(this, "button_next");	
	btn_next->setBright(can_next);
	btn_next->setTouchEnabled(can_next);
	
}

ui::Widget::ccWidgetClickCallback APProfileNameView::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APProfileNameView, onNext),
		CLICK_MAP(APProfileNameView, onSelectBoy),
		CLICK_MAP(APProfileNameView, onSelectGirl),
		CLICK_MAP(APProfileNameView, onClose)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void APProfileNameView::pushCleverTapEvent(std::string i_clickedType)
{
    std::string name = "empty";
    
    if(auto nameTextField = utils::findChild<ui::TextField*>(this, "textfield"))
    {
        name = nameTextField->getString();
    
        name = name.empty()? "empty":name;
    }
    
    auto boy = utils::findChild<ui::ImageView*>(this, "icon_boy");
    auto girl = utils::findChild<ui::ImageView*>(this, "icon_girl");
    auto gender = boy->getTag() | girl->getTag();
    
    CleverTapManager::GetInstance()->pushEvent("profile_name", {
        {"name",cocos2d::Value(name)},
        {"gender",cocos2d::Value(gender==0 ? "empty":(gender==1 ? "male":"female"))},
        {"click",cocos2d::Value(i_clickedType)},
    });
}
