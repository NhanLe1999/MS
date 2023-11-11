//
//  EverydayEventsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "EverydayEventsQuiz.h"

EverydayEventsQuiz* EverydayEventsQuiz::createQuiz()
{
    EverydayEventsQuiz* quiz=new EverydayEventsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool EverydayEventsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void EverydayEventsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string time_style_str = getJsonString(json_game, "time_style", "number_12");
    if (time_style_str == "number_12") {
        show_style = ST_show_number;
    } else if (time_style_str == "text_12") {
        show_style = ST_fully_name;
    } else  {
        show_style = ST_short_name;
    }
    
    std::string game_type_str = getJsonString(json_game, "game_type", "am_pm");
    if (game_type_str == "am_pm") {
        game_type = GT_am_pm;
    } else  {
        game_type = GT_time_range;
    }
    
    std::string time_type_str = getJsonString(json_game, "time_type", "time_right");
    if (time_type_str == "time_right") {
        time_type = TT_time_right;
    } else  {
        time_type = TT_time_half;
    }
    
    all_images = getImageList("icons");
    auto img_select = math::func::getRandomNode(all_images);
    if (game_type == GT_am_pm) {
        int hour = img_select->getComparisionValue("time");
        int minute = 0;
        if (time_type == TT_time_half) {
            minute = 30;
        }
        auto clock_1 = new math::ClockBase();
        clock_1->setTime(hour, minute);
        clock_list.push_back(clock_1);
        
        if (hour >= 12) {
            hour -= 12;
        } else {
            hour += 12;
        }
        auto clock_2 = new math::ClockBase();
        clock_2->setTime(hour, minute);
        clock_list.push_back(clock_2);
    } else {
        int hour = img_select->getComparisionValue("time");
        int minute = 0;
        if (time_type == TT_time_half) {
            minute = 30;
        }
        auto clock_1 = new math::ClockBase();
        clock_1->setTime(hour, minute);
        clock_list.push_back(clock_1);
        
        hour = img_select->getComparisionValue("number");
        auto clock_2 = new math::ClockBase();
        clock_2->setTime(hour, minute);
        clock_list.push_back(clock_2);
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue((int)game_type);
    
    for (int i=0; i<2; i++) {
        auto img = math::resource::ResHelper::createImage("");
        
        math::resource::Voice voice;
        voice.voice_id = 1;
        voice.paths = clock_list[i]->getTimeAudios(false);
        
        math::resource::Text text;
        if (show_style == ST_fully_name) {
            text.text = clock_list[i]->getTimeText(false);
        } else if (show_style == ST_short_name) {
            text.text = clock_list[i]->getTimeSemiText(false);
        } else {
            text.text = clock_list[i]->getTimeNumber(false);
        }
        text.voices.push_back(voice);
        
        math::resource::Property prop_add;
        prop_add.type = math::resource::Property::PROP;
        prop_add.key = "name_1";
        prop_add.value = text;
        
        math::resource::PropertiesSet prop_set;
        prop_set.lang = m_lang->getLanguage();
        prop_set.properties.push_back(prop_add);
        img->properties.push_back(prop_set);
        img_time.push_back(img);
    }
    
    int answer_size = 5;
    if (show_style == ST_show_number) {
        answer_size = 3;
    } else if (show_style == ST_fully_name) {
        answer_size = 7;
    }
    
    for (int i=0; i<2; i++) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(EverydayEventsQuiz::onAnswered, this), getAnswerSize(answer_size));
        answer->setIcon(img_time[i]);
        answer->setTitleWithIcon("name_1");
        if (i==0) {
            answer->setCorrect(true);
        }
        _answers.push_back(answer);
    }
    math::func::shuffle(_answers);
    
    cocos2d::Size object_size = cocos2d::Size(500, 500);
    auto object = mpool->getObject();
    object->setEnable(0, math::common::deny, img_select, nullptr);
    object->setObjectSize(object_size);
    object->fixHeightObject();
    _objects.push_back(object);
    
}

void EverydayEventsQuiz::onStart() {
    float bar_height = 0.25;
    onShowBottomBar(0, bar_height);
    Math_Quiz::onStart();
    
    cocos2d::Size object_size = _objects.front()->getContentSize();
    _objects.front()->setPosition(cocos2d::Vec2(quiz_size.width * 0.5, (quiz_size.height * 0.75 - getQuestion()->getContentSize().height - 20) * 0.5));
    addChildNode(_objects.front());
    _objects.front()->onShow(0);
    
    math::func::setPositionList(_answers, 2, cocos2d::Rect(quiz_size.width *0.1, 20, quiz_size.width*0.8, quiz_size.height * bar_height - 70));
}

void EverydayEventsQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    int i=0;
    for (Math_Answer* answer : _answers) {
        answer->onShowInParent(_panel_bot, 2, 0.1*(i++), math::animation::SlideUp);
    }
}

void EverydayEventsQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}


void EverydayEventsQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    //Show full number
    int i=0;
    for (Math_Answer* node : _answers) {
        node->setDisable(0.1*(i++), math::animation::SlideUp);
    }
    
    _objects.front()->onHide(0.2);
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void EverydayEventsQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void EverydayEventsQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void EverydayEventsQuiz::autoTestingRun(float delaytime) {
    
}

