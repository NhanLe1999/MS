//
// Created by To Ngoc Linh on 8/23/17.
//

#include "MJCalculator.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "CleverTapManager.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"
#include "MJPlatformConfig.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(MJCalculator);

#define calculator_node_name "calculator_block"

void MJCalculator::showCalculator(cocos2d::Node * blocked_node, std::function<void()> callback, std::function<void()> close_callback)
{
    if (blocked_node == nullptr) {
        blocked_node = Director::getInstance()->getRunningScene();
    }

    MJCalculator * calculator = blocked_node->getChildByName<MJCalculator *>(calculator_node_name);

    if (calculator)
    {
        //chỉ có 1 calculator trên màn hình
        calculator->generateFormula();
        calculator->_callback = callback;
        calculator->_close_callback = close_callback;
        return;
    }

    calculator = MJCalculator::createCalculator();
    calculator->setPosition(blocked_node->getContentSize() / 2);
    calculator->setName(calculator_node_name);
    blocked_node->addChild(calculator, INT_MAX - 1);
    calculator->_callback = callback;
    calculator->_close_callback = close_callback;
}

std::once_flag _once_flag_calculator_reader;
MJCalculator *MJCalculator::createCalculator() {
    std::call_once(_once_flag_calculator_reader, [] {
        REGISTER_CSB_READER(MJCalculator);
    });
    auto calculator = static_cast<MJCalculator *>(CSLoader::createNodeWithVisibleSize("csb/Calculator.csb"));
    if (calculator) {
        calculator->didLoadFromCSB();
        return calculator;
    }

    CC_SAFE_DELETE(calculator);
    return nullptr;
}

void MJCalculator::didLoadFromCSB()
{
    _input_counter = 0;

    if (auto title = utils::findChild<ui::Text *>(this, "calc_title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-SemiBold.ttf");
        
        title->setString(LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display), "calculator_title"));
    }
    
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);

    auto timeline = CSLoader::createTimeline("csb/Calculator.csb");
    auto info = timeline->getAnimationInfo("anim_shake");
    this->runAction(timeline);
    timeline->gotoFrameAndPause(info.endIndex);

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        auto wrapper = utils::findChild(this, "wrapper");
        wrapper->setRotation(-90);
        wrapper->setPosition(Director::getInstance()->getVisibleSize() / 2);
    }
    
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        if(auto calculatorBackground =utils::findChild<ImageView*>(this, "Image_1"))
        {
            calculatorBackground->loadTexture("mjstory/popup/calculator_bg_new_vm.png");
        }
    }

    
    //generate formula
    generateFormula();
}

void MJCalculator::generateFormula() {
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
        _saved_answer = formula + StringUtils::toString(first_element + second_element);
    } else {
        //phép trừ
        auto first_element = random(5, 9);
        auto second_element = random(0, first_element);
        formula = StringUtils::format("%d - %d = ", first_element, second_element);
        _saved_answer = formula + StringUtils::toString(first_element - second_element);
    }

    if (_formula = utils::findChild<ui::Text*>(this, "label_formula"))
    {
        _formula->setString(formula);
    }
}

ui::Widget::ccWidgetClickCallback MJCalculator::onLocateClickCallback(const std::string &callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
            CLICK_MAP(MJCalculator, onNumPad),
            CLICK_MAP(MJCalculator, onClear),
            CLICK_MAP(MJCalculator, onOK),
            CLICK_MAP(MJCalculator, onClose)
    };

    if (function_map.count(callBackName)) {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void MJCalculator::onNumPad(cocos2d::Ref *sender)
{
    if (_input_counter > 3)
    {
        return;
    }
    
    auto numpad = static_cast<ui::Button *>(sender);
    auto number = numpad->getName().substr(std::string("numpad_").size(), 1);

    _formula->setString(_formula->getString() + number);
    ++_input_counter;

    CleverTapManager::GetInstance()->pushEvent("view_calculator",
    {
        {"correct",cocos2d::Value(_formula->getString() == _saved_answer?"yes":"no")},
        {"click_type",cocos2d::Value(_formula->getString() == _saved_answer?"enter":"exit")}
    });
    MS_LOGGER.logEvent("view_calculator",
    {
        {"correct",cocos2d::Value(_formula->getString() == _saved_answer?"yes":"no")}
    });

    if (_formula->getString() == _saved_answer)
    {
        if (_callback) _callback();
    
        this->removeFromParent();
    }
}

void MJCalculator::onClear(cocos2d::Ref *)
{
    if (_input_counter <= 0)
    {
        return;
    }

    auto formula_str = _formula->getString();
    formula_str.resize(formula_str.length() - 1);
    _formula->setString(formula_str);

    --_input_counter;
}

void MJCalculator::onOK(cocos2d::Ref *) {
    auto formula_str = _formula->getString();
    if (formula_str == _saved_answer) {
        if (_callback) _callback();
        this->removeFromParent();
    } else {
        auto timeline = CSLoader::createTimeline("csb/Calculator.csb");
        this->runAction(timeline);
        timeline->play("anim_shake", false);
    }
}

void MJCalculator::onClose(cocos2d::Ref *) {
   // HelperManager::GetInstance()->addSidebarBlack();
    _close_callback ? _close_callback() : (void) nullptr;
    this->removeFromParent();
}

//test pull win32
