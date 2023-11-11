//
//  Minh_Calendar.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/25/17.
//
//

#include "Minh_Calendar.h"
#include <string.h>
#include <algorithm>

Minh_Calendar* Minh_Calendar::createQuiz()
{
	Minh_Calendar* quiz = new Minh_Calendar();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Minh_Calendar::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Minh_Calendar::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;

	rapidjson::Value& json_question = getJsonMember(json_game, "question");
	question_type = getJsonString(json_question, "question_type");
    if (question_type.compare("rand") == 0) {
        int n = cocos2d::random(0, 1);
        switch (n) {
            case 0: {
                question_type = "day_of_week";
                break;
            }
            case 1: {
                question_type = "date_of_month";
            }
        }
    }
    
	date_ask = getJsonString(json_question, "date_ask");
	hight_light_date_ask = getJsonBool(json_question, "hight_light_date_ask", false);
	//TODO: OBJECT CONFIG
	rapidjson::Value& json_calendar = getJsonMember(json_game,"calendar");
	calendar_size = getSizeJson(json_calendar, "size");
	monday_start = getJsonBool(json_calendar, "monday_start");
	date_format = getJsonString(json_calendar, "date_format");
	// -> lay xong du lieu cua game

	//TODO: GENERATE QUIZ COMPONENTS
	//Gen Calendar
	int month = cocos2d::random(0, 11);
	int year = cocos2d::random(2017, 2019);
	math::calendar::MonthModel month_model = math::calendar::MonthModel(month, year, monday_start);
	calendar = math::calendar::Calendar::createCalendar(month_model, 1);
	// Gen answer
	// option question day_of_week
	int date_for_ask;
	// option date_of_month
	int day_for_ask;
	int order;
	
	int number_days = calendar->getMonthData().getNumberDays();
	if (date_ask.compare("first") == 0) {
		date_for_ask = 1;
	}
	else if (date_ask.compare("end") == 0) {
		date_for_ask = number_days;
	}
	else {
		date_for_ask = cocos2d::random(1, number_days);
	}
    
	if (question_type.compare("date_of_month") == 0) {		
		day_for_ask = (calendar->getFirstDate().getDayOfWeek()  + date_for_ask - 1) % 7;
		order = (date_for_ask - 1) / 7;
	}
	
	if (question_type.compare("day_of_week") == 0) {
		calendar->setHighlightDateOfMonth(date_for_ask, hight_light_date_ask);
	}
	else {
		calendar->setHighlightDateOfWeek(day_for_ask, hight_light_date_ask);
	}
	
	_answers.clear();

	if (question_type.compare("day_of_week") == 0) {
		std::vector<int> dates_answer;
		dates_answer.clear();
		dates_answer.push_back(date_for_ask);
		while (dates_answer.size() < 4) {
			int r = cocos2d::random(1,number_days);
			bool add = true;
			for (int n : dates_answer) {
				if (n  == r || (n - r ) % 7 == 0) {
					add = false;
					break;
				}
			}
			if (add) dates_answer.push_back(r);
		}
		for(int n : dates_answer) {
			int date_of_week = calendar->getDateOfMonth(n).getDayOfWeek();
            auto date = getDateOfWeek(date_of_week);
			Math_Answer  * answer = mpool->getAnswer();
			answer->setEnable("", math::common::answer, CC_CALLBACK_1(Minh_Calendar::onTouchedAnswer, this), getAnswerSize(2));
            answer->setIcon(date);
            answer->setTitleWithIcon("name_1");
			if (n == date_for_ask) {
				answer->setCorrect(true);
			}
			else {
				answer->setCorrect(false);
			}
			_answers.push_back(answer);
		}
	}
	else {
		std::vector<int> list_answer;
		list_answer.push_back(date_for_ask);
		while (list_answer.size() < 4) {
			int r = cocos2d::random(1, number_days);
			bool add = true;
			for (int n : list_answer) {
				if (n == r) {
					add = false;
					break;
				}
			}
			if (add) list_answer.push_back(r);
		}
        
        auto ans_bg = objectBG();
		for (int n : list_answer) {
            auto day = math::ordinal::getOrdinal(n);
			Math_Answer  * answer = mpool->getAnswer();
			answer->setEnable("", math::common::answer, CC_CALLBACK_1(Minh_Calendar::onTouchedAnswer, this), getAnswerSize(1));
            answer->setIcon(day);
            answer->setTitleWithIcon("name_1");
			if (n == date_for_ask) {
				answer->setCorrect(true);
			}
			else {
				answer->setCorrect(false);
			}
            answer->setCustomAnswer(ans_bg, cocos2d::Size(140, 140), 50);
            _answers.push_back(answer);
		}
	}
	math::func::shuffle(_answers);

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_1") {
			int date_of_week = calendar->getDateOfMonth(date_for_ask).getDayOfWeek();
			++date_of_week;
			date_of_week %= 7;
			std::vector<math::resource::Text> texts;
            math::resource::Image* img = getDateOfWeek(date_of_week);
            auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
			texts.push_back(img_txt);
			
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : getNumbers(date_for_ask, date_for_ask)) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : getOrdinals(order + 1, order + 1)) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		
	}
}

void Minh_Calendar::onStart() {
    
    onShowBottomBar();
    quizBegan(0, true);
    
    auto center = cocos2d::Vec2(quiz_size.width/2, (quiz_size.height*0.75-getQuestion()->getContentSize().height-20)/2);
    calendar->setCalendarSize(calendar_size);
    calendar->setPosition(center);
    calendar->show(0, math::animation::SlideLeft);
    addChildNode(calendar);
    
    if (question_type.compare("day_of_week") == 0) {
        fixAnswerSize(quiz_size.width * 0.22, quiz_size.width*0.2);
    }
    
    int number_answer = (int)_answers.size();
    auto answer_size = _answers.front()->getContentSize();
    float delta = (quiz_size.width - 40 - answer_size.width * number_answer) / (number_answer+1);
    if (delta > 100) {
        delta = 100;
    }
    math::func::smartSetPosition(_answers, answer_size, cocos2d::Rect(20, 0, quiz_size.width - 40, quiz_size.height*0.25 - 30), delta);
    
}

void Minh_Calendar::startQuestion() {
	//Bat dau cau hoi
	enableControlQuiz();
    
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
    }
}

void Minh_Calendar::onExplanation(bool correct) {
	if (!correct) {
        disableControlQuiz(0.25);
	}
	else {
		disableControlQuiz();
		this->nextQuestion(0.75);
	}
}

void Minh_Calendar::onComplete() {
	//TODO: Thuc hien them cac lenh o day
    calendar->hide(0.5, math::animation::SlideRight);
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
void Minh_Calendar::onTouchedAnswer(Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	bool done = answer->isCorrect();
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Minh_Calendar::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Minh_Calendar::autoTestingRun(float delaytime)
{
	Math_Answer * answer = math::func::getRandomNode(_answers);
	answer->onAutoClick();
	autoTesting();
}
