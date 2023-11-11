//
//  Thanh_ReadClockAndWriteTime.cpp
//  MJQuiz
//
//  Created by Daniel on 12/12/17.
//
//

#include "Thanh_ReadClockAndWriteTime.h"
#include "Math_Libs.h"

Thanh_ReadClockAndWriteTime* Thanh_ReadClockAndWriteTime::createQuiz()
{
    Thanh_ReadClockAndWriteTime* quiz=new Thanh_ReadClockAndWriteTime();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_ReadClockAndWriteTime::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Thanh_ReadClockAndWriteTime::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);

	Math_Quiz::initQuiz(game);

    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    //TODO: CLOCK CONFIG
    rapidjson::Value& json_clock=json_data.FindMember("clock")->value;
    _radius = json_clock.FindMember("radius")->value.GetInt();
	int min_hour = json_clock.FindMember("hour")->value.FindMember("min")->value.GetInt();
	int max_hour = json_clock.FindMember("hour")->value.FindMember("max")->value.GetInt();
	_hour = cocos2d::random(min_hour, max_hour);

	int min_minute = json_clock.FindMember("minute")->value.FindMember("min")->value.GetInt();
	int max_minute = json_clock.FindMember("minute")->value.FindMember("max")->value.GetInt(); 
	_minute = cocos2d::random(min_minute, max_minute);

	_grade_1_or_lower = json_clock.FindMember("grade")->value.GetBool();
	number_text = json_clock.FindMember("display")->value.GetBool();

    //TODO:
    // set answer
	_number_answer = 4; 
	_value_answers.push_back(std::make_pair(_hour, _minute));
    for(int i = 0; i < _number_answer - 1; i++){
        bool ok = false;
        while(!ok){
            ok = true;
            std::pair<int, int> temp = std::make_pair(cocos2d::random(1, 12), cocos2d::random(0, 11));
            for(std::pair<int, int> p: _value_answers){
                if(p.first == temp.first && p.second == temp.second)
                {
                    ok = false;
                }
            }
            if(ok){
                _value_answers.push_back(temp);
            }
        }
    }
	if (_grade_1_or_lower) {
		for (std::pair<int, int> p : _value_answers) {
			p.second = (p.second / 6) * 6;
		}
	} 

    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    /*
     if (random_sorted) {
     random_shuffle(list_questiondata.begin(), list_questiondata.end());
     }
     */

    //Math_Pool::getInstance()->resetImages();
    // gen clock
    _analogClock = math::AnalogClock::createClock(1);
    _analogClock->setClockSize(_radius);
    _analogClock->setNumberAsText(true);
	_analogClock->setTime(_hour, _minute * 5); 

    //Math_Pool::getInstance()->resetImages();
}

void Thanh_ReadClockAndWriteTime::onStart() {
    CCLOG("%s override", __func__);
	onShowBottomBar(0, 0.3);
    _analogClock->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, _panel->getContentSize().height/2 - 40));
    addChildNode(_analogClock, 2);
    _analogClock->show();
    _analogClock->setVisible(true);
	_background = math::resource::ResHelper::createImage(getUIPath("common/done.png"));
    for(std::pair<int, int> p: _value_answers){
        Math_Answer* btn_answer=Math_Pool::getInstance()->getAnswer();
        std::string text;
		cocos2d::Size size;
		math::ClockBase *clock = new math::ClockBase();
		clock->setTime(p.first, p.second*5);
		if (number_text && _grade_1_or_lower) {
			text = clock->getTimeText(false);
			size = cocos2d::Size(getAnswerSize(5).width, 70);
		}
		else {
			text = clock->getTimeNumber(false);
			size = cocos2d::Size(getAnswerSize(3).width, 70);
		}

        btn_answer->setEnable(text, math::common::AnswerType::answer, CC_CALLBACK_1(Thanh_ReadClockAndWriteTime::onAnswerDone, this), size); 
        btn_answer->setVisible(true);
		btn_answer->setCorrect(false);
        _answers.push_back(btn_done);
        _list_answer.push_back(btn_answer);
    }
	_list_answer[0]->setCorrect(true);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_list_answer.begin(), _list_answer.end(), g);
    math::func::setPositionList(_list_answer, 2, cocos2d::Rect(0.1, 0, 0.8, 0.25));

    //Gen question
    showQuestionBox(0);
}

void Thanh_ReadClockAndWriteTime::startQuestion() {
    CCLOG("%s override", __func__);

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();

        _disable = false;

        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=](){
            // show answer
            for(Math_Answer* answer: _list_answer){ 
                answer->onShowInParent(_panel_bot, 3);
            }
        }), nullptr));
    };
}

void Thanh_ReadClockAndWriteTime::onExplanation(bool correct) {
    CCLOG("%s override", __func__);
}

void Thanh_ReadClockAndWriteTime::onComplete() {
    CCLOG("%s override", __func__);
    float delay = showConratulation(0.3);

    float m_delay=delay-0.5;
    if (m_delay < 0) {
        m_delay = 0;
    }
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=](){
        //#1. hide answers
        for(Math_Answer* btn: _list_answer){
            btn->setDisable();
        }
        _analogClock->destroy();
    }), nullptr));

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


void Thanh_ReadClockAndWriteTime::onAnswerDone(cocos2d::Node* node)
{
    if(_disable) return;
    _disable = true;
    Math_Answer *btn = (Math_Answer*)node;
    if(btn->isCorrect()) {
        this->nextQuestion();
    }
    else {
        btn->onAnimationClick(false);
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::CallFunc::create([=](){
            // show answer
            _disable = false;
        }), nullptr));
    }
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_ReadClockAndWriteTime::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_ReadClockAndWriteTime::autoTestingRun(float dt)
{
    
}
