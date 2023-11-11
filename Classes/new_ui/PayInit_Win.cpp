//
//  PayInit_Win.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/2/19.
//

#include "PayInit_Win.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"

USING_NS_CC;

INITIALIZE_READER(PayInit_Win);

std::once_flag ms_pay_init_win_reader_flag;

PayInit_Win * PayInit_Win::createView(std::function<void()> pay_callback) 
{
    std::call_once(ms_pay_init_win_reader_flag, [] 
	{
        REGISTER_CSB_READER(PayInit_Win);
    });
    
    auto node = reinterpret_cast<PayInit_Win *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/PayInit_Win.csb"));
    
	if (node) 
	{
        node->didLoadFromCSB(pay_callback);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    
	return nullptr;
}


void PayInit_Win::didLoadFromCSB(std::function<void()> pay_callback) 
{
    this->setTouchEnabled(false);
    
	_callback = pay_callback;

	auto str	= LANGUAGE_MANAGER->getDisplayTextByKey("key.pay.init.window");    
	
	auto title	= LANGUAGE_MANAGER->getDisplayTextByKey("key.pay.init.button.title"); 

	if (auto message = utils::findChild<ui::Text *>(this, "message"))
	{
		message->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Italic.ttf");

		message->setString(str);
	}

	if (auto buttonTitle = utils::findChild<ui::Text *>(this, "btn_title"))
	{
		buttonTitle->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Black.ttf");

		buttonTitle->setString(title);
	}
}

void PayInit_Win::onEnter() 
{
    ui::Layout::onEnter();
}

void PayInit_Win::onExit() 
{
    ui::Layout::onExit();
}

void PayInit_Win::onClose(cocos2d::Ref *sender) 
{
    this->removeFromParent();
}

void PayInit_Win::onBuy(cocos2d::Ref *sender) 
{
	Application::getInstance()->openURL(MJDEFAULT->getStringForKey("pay_use_url"));
	
	if (_callback) 
	{
        _callback();
    }
}

ui::Widget::ccWidgetClickCallback PayInit_Win::onLocateClickCallback(const std::string & name) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(PayInit_Win, onClose),
        CLICK_MAP(PayInit_Win, onBuy)
    };
    
    if (function_map.find(name) != function_map.end()) 
	{
        return function_map.at(name);
    }
    
    return nullptr;
}

