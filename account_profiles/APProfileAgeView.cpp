//
//  APProfileAge.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APProfileAgeView.h"
#include "APProfileAvatarView.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"
#include "MJDefault.h"

USING_NS_CC;

INITIALIZE_READER(APProfileAgeView);

std::once_flag ap_profile_age_reader_flag;

APProfileAgeView * APProfileAgeView::createView(const std::string & name, const bool is_boy)
{
    std::call_once(ap_profile_age_reader_flag, [] {
        
        REGISTER_CSB_READER(APProfileAgeView);
    
    });
    
    auto node = reinterpret_cast<APProfileAgeView *>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileAgePopup.csb"));
    
    if (node)
    {
        node->didLoadFromCSB(name, is_boy);
        
        return node;
    }
    
    CC_SAFE_DELETE(node);
    
    return nullptr;
}

void APProfileAgeView::didLoadFromCSB(const std::string& name, const bool is_boy)
{
	_name = name;
	_is_boy = is_boy;
	_age = 0;
	doCheckNext();
}

void APProfileAgeView::onEnter()
{
    ui::Layout::onEnter();
    
    changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    
    ms::EventLogger::getInstance().logEvent("view_profile_avatar", {});
    
    auto csb_name = "csb/account/ProfileAgePopup.csb";
    auto runani = CSLoader::createTimeline(csb_name);
    runani->setTag(1014);
    this->runAction(runani);
    runani->play("show", false);
    
    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        this->setVisible(false);
        _age = 9;
        onNext(utils::findChild(this, "button_next"));
    }
}

void APProfileAgeView::changeDisplayLanguage(int lang_id)
{
    if(auto text = utils::findChild<Text*>(this, "Text_1"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.profile.age.title"));
    }
    
    if(auto text = utils::findChild<Text*>(utils::findChild<Button*>(this, "button_next"), "Text_1_0_0"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_continue_button_title"));
    }
}

void APProfileAgeView::onExit()
{
    ui::Layout::onExit();
}

void APProfileAgeView::onClose(cocos2d::Ref * sender)
{
    ms::EventLogger::getInstance().logEvent("profile_create_avatar_exit", {});

    pushCleverTapEvent("exit");
    
    auto csb_name = "csb/account/ProfileAgePopup.csb";
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([this] {
		this->removeFromParent();
	});
}
void APProfileAgeView::onNext(Ref * sender)
{
    ms::EventLogger::getInstance().logEvent("new_profile_avatar_created", {});
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);
	
    if (_age == 0)
	{
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.select.age"), "OK", LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
        
		button->setTouchEnabled(true);
		return;
	}

    pushCleverTapEvent("continue");
    
    auto avatar_view = APProfileAvatarView::createView(_name, _is_boy, _age);
	avatar_view->setDelegate(_delegate);
    avatar_view->setEnableCloseButton(!MJDEFAULT->getBoolForKey("will_create_new_profile", false));
	this->getParent()->addChild(avatar_view,INT_MAX - 1);

	button->setTouchEnabled(true);
	this->removeFromParent();
}

void APProfileAgeView::onSelectAge(cocos2d::Ref * sender)
{
	auto layout_age = utils::findChild(this, "Panel_1");
	
    auto old_choose = layout_age->getChildByTag<ui::Layout* >(_age);

    if (old_choose)
	{
		old_choose->setBackGroundImage("mjstory/new_ui/login/button age 2.png");
		utils::findChild<ui::Text*>(old_choose, "Text_1_0_0")->setTextColor(Color4B(41,176,223,225));
	}

	auto btn_age = reinterpret_cast<ui::Layout*>(sender);
	_age = btn_age->getTag();
	btn_age->setBackGroundImage("mjstory/new_ui/login/button age.png");
	utils::findChild<ui::Text*>(btn_age, "Text_1_0_0")->setTextColor(Color4B::WHITE);

	doCheckNext();
}


ui::Widget::ccWidgetClickCallback APProfileAgeView::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APProfileAgeView, onNext),
		CLICK_MAP(APProfileAgeView, onSelectAge),
		CLICK_MAP(APProfileAgeView, onClose)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void APProfileAgeView::doCheckNext()
{
	auto can_next = _age != 0;
	auto btn_next = utils::findChild<ui::Button*>(this, "button_next");
	btn_next->setBright(can_next);
	btn_next->setTouchEnabled(can_next);
}

void APProfileAgeView::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("profile_age",
    {
        {"age",cocos2d::Value(_age)},
        {"click",cocos2d::Value(i_clickedType)},
    });
}

void  APProfileAgeView::setEnableCloseButton(bool isEnableCloseButton)
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
