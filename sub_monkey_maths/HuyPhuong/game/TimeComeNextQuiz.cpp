//
//  TimeComeNextQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "TimeComeNextQuiz.h"

TimeComeNextQuiz* TimeComeNextQuiz::createQuiz()
{
    TimeComeNextQuiz* quiz=new TimeComeNextQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool TimeComeNextQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void TimeComeNextQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string time_type_str = getJsonString(json_game, "time_type", "date_of_week");
    if (time_type_str == "date_of_week") {
        time_type = date_of_week;
        all_images = getDatesOfWeek();
    } else if (time_type_str == "month_of_year") {
        time_type = month_of_year;
        all_images = getMonthsOfYear();
    } else if (time_type_str == "season_of_year") {
        time_type = season_of_year;
        all_images = getSeasonsOfYear();
    }
    
    std::string game_type_str = "come_next";
    rapidjson::Value& game_type_json = getJsonMember(json_game, "game_play");
    if (!game_type_json.Empty()) {
        game_type_str = game_type_json[cocos2d::random(0, (int)game_type_json.Size()-1)].GetString();
    }
    if (game_type_str == "come_next") {
        game_type = come_next;
    } else if (game_type_str == "come_before") {
        game_type = come_before;
    } else {
        game_type = between;
    }
    
    only_answer = (getJsonString(json_game, "display", "only_answer") == "only_answer");
    if (!only_answer) {
        number_object = randomRangeJson(json_game, "number_object");
    }
    
    std::string show_str = getJsonString(json_game, "show", "fully_name");
    if (show_str == "fully_name") {
        show_type = fully_name;
    } else if (show_str == "short_name") {
        show_type = short_name;
    } else {
        show_type = show_number;
    }
    
    if (time_type == season_of_year) {
        show_type = fully_name;
    }
    
    //get number question
    int value_last = 3;
    if (time_type == season_of_year) {
        value_last = 3;
    } else if (time_type == date_of_week) {
        value_last = 6;
    } else if (time_type == month_of_year) {
        value_last = 11;
    }
    number_question = cocos2d::random(0, value_last);
    
    if (game_type == come_next) {
        int index = number_question-1;
        if (number_question == 0) {
            index = value_last;
        }
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == index) {
                img_list.push_back(img);
            }
        }
    } else if (game_type == come_before) {
        int index = number_question+1;
        if (number_question == value_last) {
            index = 0;
        }
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == index) {
                img_list.push_back(img);
            }
        }
    } else {
        int index_before = number_question-1;
        if (number_question == 0) {
            index_before = value_last;
        }
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == index_before) {
                img_list.push_back(img);
            }
        }
        
        int index_after = number_question+1;
        if (number_question == value_last) {
            index_after = 0;
        }
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == index_after) {
                img_list.push_back(img);
            }
        }
    }
    for (math::resource::Image* img : all_images) {
        if (img->getComparisionValue("number") == number_question) {
            img_list.push_back(img);
        }
    }
    
    math::func::shuffle(all_images);
    while (img_list.size() < 4) {
        for (math::resource::Image* img : all_images) {
            bool valid = true;
            for (math::resource::Image* img2 : img_list) {
                if (img->code == img2->code) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                img_list.push_back(img);
                break;
            }
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue((int)game_type);
    
    CCLOG("question keyword: %d", (int)game_type);
    
    math::func::shuffle(img_list);
    auto answer_color = getColor4B(math::func::getRandomNode(getColors())->getText());
    
    int answer_size = 2;
    std::string key_name = "name_n";
    if (show_type == fully_name) {
        answer_size = 6;
        key_name = "name_1";
    }
    
    for (math::resource::Image * img : img_list) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(TimeComeNextQuiz::onAnswered, this), getAnswerSize(answer_size));
        answer->setIcon(img);
        answer->setTitleWithIcon(key_name);
        if (img->getComparisionValue("number") == number_question) {
            answer->setCorrect(true);
        }
        answer->getValueText()->setTextColor(answer_color);
        _answers.push_back(answer);
    }
}

void TimeComeNextQuiz::onStart() {
    float bar_height = 0.25;
    int number_in_line = 4;
    if (show_type == fully_name) {
        number_in_line = 2;
        bar_height = 0.4;
    }
    onShowBottomBar(0, bar_height);
    Math_Quiz::onStart();
    
    math::func::setPositionList(_answers, number_in_line, cocos2d::Rect(quiz_size.width *0.05, 20, quiz_size.width*0.9, quiz_size.height * bar_height - 70));
}

void TimeComeNextQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    int i=0;
    for (Math_Answer* answer : _answers) {
        answer->onShowInParent(_panel_bot, 2, 0.1*(i++), math::animation::SlideUp);
    }
}

void TimeComeNextQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        
        this->nextQuestion();
    }
}


void TimeComeNextQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    //Show full number
    int i=0;
    for (Math_Answer* node : _answers) {
        node->setDisable(0.1*(i++), math::animation::SlideUp);
    }
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void TimeComeNextQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void TimeComeNextQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void TimeComeNextQuiz::autoTestingRun(float delaytime) {
    
}

