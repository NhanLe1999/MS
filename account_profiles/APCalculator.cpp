//
//  APCalculator.cpp
//  MJStory
//
//  Created by ABN on 10/8/19.
//

#include "APCalculator.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "APDatabase.h"
#include "APProfileAgeView.h"
#include "GlobalFunc.h"
#include "MSEventLogger.h"
#include "MJHelper.h"
#include "APAuth.h"
#include "MSIntroLicense.h"
#include "MJHelper.h"

USING_NS_CC;

INITIALIZE_READER(APCalculator);

std::once_flag ap_calculator_reader_flag;

APCalculator * APCalculator::createView() {
    std::call_once(ap_calculator_reader_flag, [] {
        REGISTER_CSB_READER(APCalculator);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/AccountCalculator.csb");
    auto node = reinterpret_cast<APCalculator *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (node) {
        node->didLoadFromCSB();
        
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void APCalculator::didLoadFromCSB()
{
	ms::EventLogger::getInstance().logEvent("view_verify_calculator", {});
    _input_counter = 0;
    root_panel = utils::findChild<Layout*>(this, "Panel_calculator");
    _result = utils::findChild<ui::Text *>(this, "result");
    auto button_clear = utils::findChild<Button*>(this,"numpad_clear");
    button_clear->setScale(1.5);
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    auto is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;

    if(auto title = utils::findChild<Text*>(this, "Title"))
    {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.ap.calculator"));

    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto blackLayer = LayerColor::create(cocos2d::Color4B::BLACK);

    blackLayer->setIgnoreAnchorPointForPosition(false);

    blackLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    blackLayer->setOpacity(0.75f*255.0f);

    blackLayer->setPosition(origin+visibleSize/2);
    
    this->addChild(blackLayer, -1);
    
    generateFormula();
}


void APCalculator::onEnter() {
    
    ui::Layout::onEnter();
    
    changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    
}

void APCalculator::onExit() {
    ui::Layout::onExit();
}

void APCalculator::changeDisplayLanguage(int lang_id) {
//    if (!lang_id) {
//        utils::findChild<Text*>(this, "title")->setString("Your child's name");
//        utils::findChild<TextField*>(this, "textfield")->setPlaceHolder("Your child's name");
//        utils::findChild<Text*>(this, "gender_label")->setString("Gender");
//        utils::findChild<Text*>(this, "text_next")->setString("Next");
//    }
    
}

void APCalculator::showErrorAlert(std::string msg) {
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
        if (msg == "") {
            
            StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
            
        }
        else {
             StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4)? "Đóng" : "Close");
        }
    });
}

void APCalculator::generateFormula() {
    auto type = random(0, 1);
    std::string formula = "";
    
    if (type == 0) {
        //phép cộng
        auto first_element = random(0, 9);
        auto second_element = random(0, 9);
        while (first_element + second_element >= 10) {
            second_element = random(0, 9);
        }
        formula = StringUtils::format("%d + %d = ", first_element, second_element);
        _saved_answer = StringUtils::toString(first_element + second_element);
    }
    else {
        //phép trừ
        auto first_element = random(5, 9);
        auto second_element = random(0, first_element);
        formula = StringUtils::format("%d - %d = ", first_element, second_element);
        _saved_answer = StringUtils::toString(first_element - second_element);
    }
    formula += "?";
    _formula = utils::findChild<ui::Text *>(this, "label_formula");
    _formula->setString(formula);
}



void APCalculator::onNumPad(cocos2d::Ref *sender) {
    if (_input_counter > 3) {
        return;
    }
    auto numpad = static_cast<ui::Button *>(sender);
    auto number = numpad->getName().substr(std::string("numpad_").size(), 1);
    if (_result->getString() == "___") _result->setString("");
    _result->setString(_result->getString() + number);
    ++_input_counter;
    if (_result->getString() == _saved_answer) {
        
        auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountCalculator.csb");
        auto runani = CSLoader::createTimeline(csb_name);
        runani->setTag(1014);
        this->runAction(runani);
        runani->play("hide", false);
        runani->setLastFrameCallFunc([=] {
            if(m_okCallback)
            {
                m_okCallback();
            }
            this->removeFromParent();
        });
    }
}

void APCalculator::onClear(cocos2d::Ref *) {
    if (_input_counter <= 0) {
        return;
    }
    
    auto _result_str = _result->getString();
    _result_str.resize(_result_str.length() - 1);
    _result->setString(_result_str);
    
    --_input_counter;
    if (_input_counter <= 0) {
        _result->setString("___");
    }
}

void APCalculator::onOK(cocos2d::Ref *) {
    auto result_str = _result->getString();
    if (result_str == _saved_answer) {
        
        auto csb_name = mj::helper::getCSBName("csb/account/AccountCalculator.csb");
        auto runani = CSLoader::createTimeline(csb_name);
        runani->setTag(1014);
        this->runAction(runani);
        runani->play("hide", false);
        runani->setLastFrameCallFunc([=] {
            this->removeFromParent();
        });
    }
    else {
        _input_counter = 0;
        _result->setString("___");
    }
}
void APCalculator::onTest(cocos2d::Ref *) {
    CCLOG("asd");
}

ui::Widget::ccWidgetClickCallback APCalculator::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APCalculator, onOK),
        CLICK_MAP(APCalculator, onTest),
        CLICK_MAP(APCalculator, onNumPad),
        CLICK_MAP(APCalculator, onClear)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
