//
//  MissingInClockFaceQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "MissingInClockFaceQuiz.h"

MissingInClockFaceQuiz* MissingInClockFaceQuiz::createQuiz()
{
    MissingInClockFaceQuiz* quiz=new MissingInClockFaceQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MissingInClockFaceQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MissingInClockFaceQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    clock_size = getJsonInt(json_game, "size", 400);
    decimal_number = getJsonBool(json_game, "display", true);
    
    auto all_numbers = getNumbers(1, 12);
    math::func::shuffle(all_numbers);
    for (int i = 0; i<4; i++) {
        img_list.push_back(all_numbers[i]);
    }
    missing_number = img_list.front()->getComparisionValue("number");
    
    clock = math::AnalogClock::createClock(1);
    clock->setClockSize(clock_size);
    clock->setTime(missing_number, 0);
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    math::func::shuffle(img_list);
    auto answer_bg = objectBG();
    auto answer_color = getColor4B(math::func::getRandomNode(getColors())->getText());
    
    for (math::resource::Image * img : img_list) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(MissingInClockFaceQuiz::onAnswered, this));
        answer->setIcon(img);
        answer->setTitleWithIconProp("number");
        answer->setCustomAnswer(answer_bg, cocos2d::Size(135, 135), 75, answer_color, 0.3);
        if (img->getComparisionValue("number") == missing_number) {
            answer->setCorrect(true);
        }
        _answers.push_back(answer);
    }
}

void MissingInClockFaceQuiz::onStart() {
    float barwidth = 250.0/quiz_size.width;
    onShowRightBar(0, barwidth);
    Math_Quiz::onStart();
    
    clock->setPosition(cocos2d::Vec2(quiz_size.width*(1-barwidth)/2, (quiz_size.height - getQuestion()->getContentSize().height - 30)/2));
    addChildNode(clock, 10);
    clock->show();
    clock->setMissingNumber(missing_number);
    
    math::func::setPositionList(_answers, 1, cocos2d::Rect(30, 50, quiz_size.width*barwidth - 60, quiz_size.height-100));
}

void MissingInClockFaceQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    int i=0;
    for (Math_Answer* answer : _answers) {
        answer->onShowInParent(_panel_right, 2, 0.1*(i++), math::animation::SlideLeft);
    }
}

void MissingInClockFaceQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        clock->showMissingNumber(missing_number);
        
        time_count = 720;
        hour = clock->getHour();
        minute = clock->getMinute();
        
        scheduleOnce([=](float){
            this->clockRun(0.03);
        }, 0.5, cocos2d::StringUtils::format("clockRun %d", time_count));
    }
}

void MissingInClockFaceQuiz::clockRun(float dt) {
    if (time_count <= 0) {
        std::string gaf = "click1";
        auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
        auto anim = gaf_asset->createObjectAndRun(false);
        anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        anim->setScale(clock->getContentSize().width/300.0);
        anim->setPosition(clock->getPosition());
        addChildNode(anim, 20);
        
        audio_helper->play("magic_dust.mp3");
        
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "next question");
    } else {
        time_count-=5;
        minute+=5;
        if (minute >= 60) {
            minute-=60;
            hour++;
            if (hour>12) {
                hour = 1;
            }
            clock->showMissingNumber(hour);
        }
        clock->setTime(hour, minute);
        scheduleOnce([=](float){
            this->clockRun(dt);
        }, dt, cocos2d::StringUtils::format("clockRun %d", time_count));
    }
}

void MissingInClockFaceQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    //Show full number
    int i=0;
    for (Math_Answer* node : _answers) {
        node->setDisable(0.1*(i++), math::animation::SlideLeft);
    }
    clock->destroy();
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void MissingInClockFaceQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MissingInClockFaceQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MissingInClockFaceQuiz::autoTestingRun(float delaytime) {
    
}

