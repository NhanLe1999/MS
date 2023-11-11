//
//  Thanh_WhereDoesClockHandPointing.cpp
//  MJQuiz
//
//  Created by Daniel on 11/23/17.
//
//

#include "Thanh_WhereDoesClockHandPointing.h"
#include "Math_Libs.h"

Thanh_WhereDoesClockHandPointing* Thanh_WhereDoesClockHandPointing::createQuiz()
{
	Thanh_WhereDoesClockHandPointing* quiz = new Thanh_WhereDoesClockHandPointing();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_WhereDoesClockHandPointing::init() {
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Thanh_WhereDoesClockHandPointing::initQuiz(mj::model::Game game)
{
	CCLOG("%s override", __func__);

	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	str = json_game.FindMember("mode")->value.GetString();
	if (str == "hour") {
		_mode = Mode::hour;
	}
	else if (str == "minute") {
		_mode = Mode::minute;
	}
	str = json_game.FindMember("option")->value.GetString();
	if (str == "grade_1") {
		_option = Option::grade_1;
	}
	else if (str == "grade_1++") {
		_option = Option::grade_1_plus_plus;
	}
	_answer_mode = json_game.FindMember("answer_mode")->value.GetString();

	//TODO: CLOCK CONFIG
	rapidjson::Value& json_clock = json_data.FindMember("clock")->value;
	_radius = json_clock.FindMember("radius")->value.GetInt();
	_random_time = json_clock.FindMember("mode")->value.GetBool();
	if (_random_time == false) {
		_hour = json_clock.FindMember("hour")->value.GetInt();
		_minute = json_clock.FindMember("minute")->value.GetInt();
	}
	else {
		_hour = cocos2d::random(1, 12);
		_minute = cocos2d::random(0, 11);
	}
	if (_option == Option::grade_1) {
		_minute = (_minute / 6) * 6; // de no la 0 hoac 6
	}

	//TODO: ANSWER CONFIG 
	if(_mode == Mode::minute && _option == Option::grade_1) {
		_number_answer = 2;
	}
	else {
		_number_answer = 4;
	}

	int temp;
	if (_mode == Mode::hour) {
		_vector_answers.push_back(_hour);
		for (int i = 0; i < _number_answer - 1; i++) {
			temp = cocos2d::random(1, 12);
			for (int j = 0; j < _vector_answers.size(); j++) {
				if (_vector_answers[j] == temp) {
					j = -1;
					temp = cocos2d::random(1, 12);
				}
			}
			_vector_answers.push_back(temp);
		}
	}
	else {
		_vector_answers.push_back(_minute);
		if (_option == Option::grade_1) {
			_vector_answers.push_back((_minute + 6) % 12);
		}
		else {
			for (int i = 0; i < _number_answer - 1; i++) {
				temp = cocos2d::random(0, 11);
				for (int j = 0; j < _vector_answers.size(); j++) {
					if (_vector_answers[j] == temp) {
						j = -1;
						temp = cocos2d::random(0, 11);
					}
				}
				_vector_answers.push_back(temp);
			}
		}
	}
	//TODO:
	//lay tat ca icon trong media_set


	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	if (_mode == Mode::hour) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}
	/*
	 if (random_sorted) {
	 random_shuffle(list_questiondata.begin(), list_questiondata.end());
	 }
	 */

	 // gen clock
	_analogClock = math::AnalogClock::createClock(1);
	_analogClock->setClockSize(_radius);
	_analogClock->setNumberAsText(true);
	_analogClock->setTime(_hour, _minute*5);

	//    //Math_Pool::getInstance()->resetImages();

	//    /*--- analog clock ---*/
	//    math::AnalogClock* clock = math::AnalogClock::createClock(1);
	//    clock->setClockSize(400);
	//    clock->setNumberAsText(true);
	//    clock->setTime(3, 30);
	//    clock->show();
	//    clock->setPosition(cocos2d::Vec2(450, 300));
	//    addChildNode(clock, 10);
	//
	//
	//    /*--- digital clock ---*/
	//    math::DigitalClock* dclock = math::DigitalClock::createClock(1);
	//    dclock->setMode24(false);
	//    dclock->setClockSize(400);
	//    dclock->setTime(15, 30);
	//    dclock->show();
	//    dclock->setPosition(cocos2d::Vec2(450, 300));
	//    addChildNode(dclock, 10);
}

void Thanh_WhereDoesClockHandPointing::onStart() {
	CCLOG("%s override", __func__);
	onShowBottomBar(0, 0.3);
	_analogClock->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, _panel->getContentSize().height / 2 - 40));
	addChildNode(_analogClock, 2);
	_analogClock->show();
	_analogClock->setVisible(true);
	 
	// gen answer 
	_background = math::resource::ResHelper::createImage(getUIPath("common/done.png"));
	for (int i = 0; i < _number_answer; i++) {
		Math_Answer *answer = Math_Pool::getInstance()->getAnswer();
		std::string text;
		cocos2d::Size size;
		auto img = getNumbers(_vector_answers[i], _vector_answers[i]).at(0);
		if (_answer_mode == "text") {
			text = "at " + cocos2d::StringUtils::toString(img->getText("name_n", m_lang->getLanguage()));
			size = cocos2d::Size(getAnswerSize(3).width, 70);
		}
		else {
			text = "at " + cocos2d::StringUtils::toString(_vector_answers[i]);
			size = cocos2d::Size(getAnswerSize().width, 70);
		};
		answer->setEnable(text, math::common::answer, CC_CALLBACK_1(Thanh_WhereDoesClockHandPointing::onAnswerDone, this), size); 
		answer->setVisible(true);
		answer->setCorrect(false);
		_answers.push_back(answer);
		_list_answer.push_back(answer); 
	} 
	_list_answer[0]->setCorrect(true);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(_list_answer.begin(), _list_answer.end(), g); 
	if (_number_answer == 2) {
		math::func::setPositionList(_list_answer, 2, cocos2d::Rect(0.1, 0, 0.8, 0.25));
	}
	else {
		math::func::setPositionList(_list_answer, 2, cocos2d::Rect(0.1, 0, 0.8, 0.2));
	}
	//Gen question
	showQuestionBox(0.5f);
}

void Thanh_WhereDoesClockHandPointing::startQuestion() {
	CCLOG("%s override", __func__);

	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();

		for (Math_Answer *answer : _list_answer) {
			answer->onShowInParent(_panel_bot, 2);
		}

		_disable = false;
	};
}

void Thanh_WhereDoesClockHandPointing::onExplanation(bool correct) {
	CCLOG("%s override", __func__);
}

void Thanh_WhereDoesClockHandPointing::onComplete() {
	CCLOG("%s override", __func__);
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.5;
	if (m_delay < 0) {
		m_delay = 0;
	}
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=]() {
		//#1. hide answers
		for (Math_Answer* answer : _list_answer) {
			answer->setDisable(0, math::animation::ScaleUpAndRotation);
		}
		_analogClock->removeFromParentAndCleanup(true);
	}), nullptr));

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_WhereDoesClockHandPointing::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_WhereDoesClockHandPointing::onAnswerDone(cocos2d::Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
	if (answer->isCorrect()) {
		nextQuestion();
	}
	else {
		answer->onAnimationClick(false);
	}
}

void Thanh_WhereDoesClockHandPointing::autoTestingRun(float dt)
{

}

