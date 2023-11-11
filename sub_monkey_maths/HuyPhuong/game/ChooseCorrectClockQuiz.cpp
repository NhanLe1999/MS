//
//  ChooseCorrectClockQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "ChooseCorrectClockQuiz.h"

ChooseCorrectClockQuiz* ChooseCorrectClockQuiz::createQuiz()
{
    ChooseCorrectClockQuiz* quiz=new ChooseCorrectClockQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ChooseCorrectClockQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void ChooseCorrectClockQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string game_type_str = getJsonString(json_game, "game_type", "hour_hand_point");
    if (game_type_str == "hour_hand_point") {
        game_type = GT_hour_hand_point;
    } else if (game_type_str == "minute_hand_point") {
        game_type = GT_minute_hand_point;
    } else if (game_type_str == "compare_time") {
        game_type = GT_compare_time;
    } else if (game_type_str == "compare_clock") {
        game_type = GT_compare_clock;
    } else {
        game_type = GT_different_time;
    }
    
    std::string time_type_str = getJsonString(json_game, "time_stype", "time_right");
    if (time_type_str == "time_right") {
        time_type = TT_time_right;
    } else if (time_type_str == "time_half") {
        time_type = TT_time_half;
    } else if (time_type_str == "right_and_half") {
        time_type = cocos2d::random(0, 1) == 0 ? TT_time_right : TT_time_half;
    } else {
        time_type = TT_random;
    }
    
    std::string display_str = getJsonString(json_game, "display", "analog");
    if (display_str == "analog") {
        is_analog = true;
    } else if (display_str == "digital") {
        is_analog = false;
    } else {
        is_analog = (cocos2d::random(0, 1) == 0);
    }
    
    if (game_type == GT_hour_hand_point || game_type == GT_minute_hand_point) {
        is_analog = true;
    }
    
    std::string show_time_str = getJsonString(json_game, "show_time", "number_12");
    if (show_time_str == "number_12") {
        show_time = ST_time;
        mode_24 = false;
    } else if (show_time_str == "number_24") {
        show_time = ST_time;
        mode_24 = true;
    } else if (show_time_str == "text_12") {
        show_time = ST_text;
        mode_24 = false;
    } else if (show_time_str == "text_24") {
        show_time = ST_text;
        mode_24 = true;
    } else if (show_time_str == "semi_text_12") {
        show_time = ST_semi_text;
        mode_24 = false;
    } else {
        show_time = ST_semi_text;
        mode_24 = true;
    }
    
    number_clock = randomRangeJson(json_game, "number_clock");
    if (game_type == GT_different_time && number_clock < 3) {
        number_clock = 3;
    }
    
    clock_size = getJsonInt(json_game, "clock_size", 200);
    same_style = getJsonBool(json_game, "same_style", true);
    analog_decimal = getJsonBool(json_game, "analog_decimal", true);
    
    switch (game_type) {
        case GT_hour_hand_point:
        {
            auto list_hours = randomArrayIndexes(number_clock, 12);
            for (int hour : list_hours) {
                time_list.push_back(std::make_pair(hour+1, 0));
            }
        }
            break;
        case GT_minute_hand_point:
        {
            auto list_minute = randomArrayIndexes(number_clock, 12);
            for (int minute : list_minute) {
                int hour = cocos2d::random(1, 12);
                time_list.push_back(std::make_pair(hour, minute*5));
            }
        }
            break;
        case GT_different_time:
        {
            int hour = 0;
            int minute = 0;
            if (time_type == TT_time_right) {
                minute = 0;
            } else if (time_type == TT_time_half) {
                minute = 30;
            }
            auto list_hours = randomArrayIndexes(2, 12);
            for (int i=0; i < number_clock; i++) {
                if (i==0) {
                    hour = list_hours[0];
                } else {
                    hour = list_hours[1];
                }
                time_list.push_back(std::make_pair(hour+1, minute));
            }
        }
            break;
        default:
        {
            int minute = 0;
            if (time_type == TT_time_right) {
                minute = 0;
            } else if (time_type == TT_time_half) {
                minute = 30;
            }
            auto list_hours = randomArrayIndexes(number_clock, 12);
            for (int hour : list_hours) {
                time_list.push_back(std::make_pair(hour+1, minute));
            }
        }
            break;
    }
    
    cmp_hour = time_list.front().first;
    cmp_minute = time_list.front().second;

    if (is_analog) {
        clock_cmp = math::DigitalClock::createClock(1);
        clock_cmp->setTime(cmp_hour, cmp_minute);
        clock_cmp->setClockSize(clock_size * 1.25);
        
        for (int i=0; i<number_clock; i++) {
            auto pair = time_list[i];
            math::AnalogClock* clock = math::AnalogClock::createClock(1);
            clock->setTime(pair.first, pair.second);
            clock->setClockSize(clock_size);
            clock_list.push_back(clock);
        }
    } else {
        clock_cmp = math::AnalogClock::createClock(1);
        clock_cmp->setTime(cmp_hour, cmp_minute);
        clock_cmp->setClockSize(clock_size*1.25);
        
        for (int i=0; i<number_clock; i++) {
            auto pair = time_list[i];
            math::DigitalClock* clock = math::DigitalClock::createClock(1);
            clock->setTime(pair.first, pair.second);
            clock->setClockSize(clock_size);
            clock_list.push_back(clock);
        }
    }
    math::func::shuffle(clock_list);
    
    cocos2d::Size object_size = cocos2d::Size(clock_size, clock_size);
    auto img_object = math::resource::ResHelper::createImage("");
    for (int i=0; i<number_clock; i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::touch, img_object, nullptr);
        object->setObjectSize(object_size);
        object->setTouchCallback(CC_CALLBACK_1(ChooseCorrectClockQuiz::onTouchObject, this));
        object->setImageVisible(false);
        _objects.push_back(object);
        
        clock_list[i]->setPosition(object_size/2);
        object->addChild(clock_list[i]);
    }
    
    //img list
    switch (game_type) {
        case GT_hour_hand_point:
            img_list = getNumbers(cmp_hour, cmp_hour);
            break;
        case GT_minute_hand_point:
            img_list = getNumbers(cmp_minute/5,cmp_minute/5);
            break;
        case GT_compare_time:
            
            break;
        default:
            img_list = getNumbers(cmp_hour, cmp_hour);
            break;
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "time_24") {
            std::vector<math::resource::Text> texts;
            
            math::resource::Voice voice;
            voice.voice_id = 1;
            voice.paths = clock_cmp->getTimeAudios();
            
            std::string time_txt = "";
            if (show_time == ST_time) {
                time_txt = clock_cmp->getTimeNumber(true);
            } else if (show_time == ST_text) {
                time_txt = clock_cmp->getTimeText(true);
            } else {
                time_txt = clock_cmp->getTimeSemiText(true);
            }
            
            math::resource::Text text;
            text.text = time_txt;
            text.voices.push_back(voice);
            
            texts.push_back(text);
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "time_12") {
            std::vector<math::resource::Text> texts;
            
            math::resource::Voice voice;
            voice.voice_id = 1;
            voice.paths = clock_cmp->getTimeAudios();
            
            std::string time_txt = "";
            if (show_time == ST_time) {
                time_txt = clock_cmp->getTimeNumber(false);
            } else if (show_time == ST_text) {
                time_txt = clock_cmp->getTimeText(false);
            } else {
                time_txt = clock_cmp->getTimeSemiText(false);
            }
            
            math::resource::Text text;
            text.text = time_txt;
            text.voices.push_back(voice);
            
            texts.push_back(text);
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_n");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue((int)game_type);
}

void ChooseCorrectClockQuiz::onStart() {
    
    int delta = 200;
    float objs_width = number_clock*(clock_size + delta) - delta;
    while (objs_width>quiz_size.width * 0.95) {
        delta-=5;
        objs_width = number_clock*(clock_size + delta) - delta;
    }
    float pos_x = (quiz_size.width-objs_width) /2.0;
    
    if (game_type == GT_compare_clock) {
        onShowBottomBar(0, 0.4);
        Math_Quiz::onStart();
        math::func::setPositionList(_objects, number_clock, cocos2d::Rect(pos_x, 0, quiz_size.width-pos_x*2, quiz_size.height*0.4-40));
        
        clock_cmp->setPosition(cocos2d::Vec2(quiz_size.width * 0.5, quiz_size.height * 0.3 - (getQuestion()->getContentSize().height+20)/2));
        addChildNode(clock_cmp);
        clock_cmp->show();
        
        for (Math_Object* object : _objects) {
            addChildNodeToBottomPanel(object);
            object->onShow(0, math::animation::No_Effect);
        }
        for (int i=0; i<number_clock; i++) {
            auto clock = clock_list[i];
            clock->show(0.5+0.1*i, math::animation::SlideUp);
        }
    } else {
        Math_Quiz::onStart();
        math::func::setPositionList(_objects, number_clock, cocos2d::Rect(pos_x, 0, quiz_size.width-pos_x*2, quiz_size.height- 30 -getQuestion()->getContentSize().height));
        
        for (Math_Object* object : _objects) {
            addChildNode(object);
            object->onShow(0, math::animation::No_Effect);
        }
        for (int i=0; i<number_clock; i++) {
            auto clock = clock_list[i];
            clock->show(0.5);
        }
    }
}

void ChooseCorrectClockQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
}

void ChooseCorrectClockQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void ChooseCorrectClockQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    
    if (game_type == GT_compare_clock) {
        clock_cmp->destroy(0.5);
        for (int i=0; i<number_clock; i++) {
            clock_list[i]->destroy(i*0.1, math::animation::SlideUp);
        }
    } else {
        for (int i=0; i<number_clock; i++) {
            clock_list[i]->destroy(i*0.1);
        }
    }
    
    for (Math_Object* object : _objects) {
        object->onHide(number_clock*0.1+0.5, math::animation::No_Effect);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void ChooseCorrectClockQuiz::onTouchObject(Node* node) {
    disableControlQuiz(0.5);
    
    Math_Object* object = static_cast<Math_Object*>(node);
    auto clock = clock_list[object->getID()];
    bool correct = abs(cmp_hour - clock->getHour())%12==0 && (clock->getMinute() == cmp_minute);
    object->onTouchObject(correct);
    this->onExplanation(correct);
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ChooseCorrectClockQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ChooseCorrectClockQuiz::autoTestingRun(float delaytime) {
    
}
