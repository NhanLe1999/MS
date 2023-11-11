#include "Minh_CompareClock.h"


////////
//		Minh_CompareClock.cpp
//		MJ_Game
//		Create By MinhTzy on 08/11/2017
//

Minh_CompareClock * Minh_CompareClock::createQuiz()
{
	Minh_CompareClock * quiz = new Minh_CompareClock();
	if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Minh_CompareClock::init()
{
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Minh_CompareClock::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
	rapidjson::Value & json_data = document.FindMember("data")->value;

	//TODO : Game Config
	rapidjson::Value & game_data = getJsonMember(json_data,"game");
	int num_act_min = getJsonInt(getJsonMember(game_data, "num_activities"), "minimum");
	int num_act_max = getJsonInt(getJsonMember(game_data, "num_activities"), "maximum");
	num_activity = cocos2d::random(num_act_min, num_act_max);
	
	answer = getJsonString(game_data, "answer","first");
	std::string ans_temp[] = { "first","later" };
    int index = cocos2d::random(0, 1);
    if (answer.compare("rand") == 0) {
		answer = ans_temp[index];
	}

	clock_type = getJsonString(getJsonMember(game_data, "clock"), "type","analog");
	clock_mode = getJsonInt(getJsonMember(game_data, "clock"), "mode",12);
	clock_show = getJsonString(getJsonMember(game_data, "clock"), "number_type","text");

	//clock_size -= (num_activity - 1) / 3 * 30;

	// TODO : ICON CONFIG
	rapidjson::Value & icon_data = getJsonMember(json_data, "icon");

	icon_same = getJsonString(icon_data, "same", "name");
	icon_filter = getJsonString(icon_data, "different", "id");

	// fix select icons to compare
    same_name = (icon_same.compare("name") == 0 || icon_same.compare("name_1") == 0);
	
	// TODO : GET ICONS OPERATOR & NUMBER
	activities_icon.clear();
	activities_icon = getImageList("icons", num_activity, icon_filter, icon_same);
	math::func::shuffle(activities_icon);
	//----> Lay xong data cua game
	
	// #1. Gen Clock
	int obj_id = 0;
	if (clock_type.compare("analog") == 0) {
		for (math::resource::Image * img : activities_icon) {
			auto time = getTimeFromImage(img);
			int hour, minute;
			hour = time.first;
			minute = time.second;

			math::AnalogClock * clock = math::AnalogClock::createClock(1);

			clock->setTime(hour, minute);
			clock->setClockSize(clock_size);
			clock->setNumberAsText(true);
			if (clock_show.compare("roman") == 0) {
				clock->setNumberRoman(true);
			}
			clock->showAllNumbers();
			_clocks.push_back(clock);
			addChildNode(clock, 2);
		}
	}
	else {
		for (math::resource::Image * img : activities_icon) {
			auto time = getTimeFromImage(img);
			int hour, minute;
			hour = time.first;
			minute = time.second;

			math::DigitalClock * clock = math::DigitalClock::createClock(1);

			clock->setTime(hour, minute);
			clock->setClockSize(clock_size);
			if (clock_mode == 24) clock->setMode24(true);
			else clock->setMode24(false);
			_clocks.push_back(clock);
			addChildNode(clock, 2);
		}
	}

	// #2 Gen text below clock
	auto object_size = cocos2d::Size(clock_size, clock_size);
	for (math::resource::Image * img : activities_icon) {
		Math_Object * object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::TouchPermission::touch, img, NULL);
		object->setObjectSize(object_size);
		//object->setTouchCallback(CC_CALLBACK_1(Minh_CompareClock::onTouchedObject, this));
        if (same_name) {
            object->setValueTextByKey("kind");
        }
        else {
            object->setValueTextByKey("name_1");
		}
        object->getLabelValue()->setFontSize(36);
        object->setValueTextPosition("bottom");
        object->showValueTextWithAnimation(false);
        object->setImageVisible(false);
		_objects.push_back(object);
	}
    
	// #2. Gen answer
	math::resource::Image * res_img;
	if (answer.compare("first") == 0) {
		res_img = findFirstActivityImage(activities_icon);
	}
	else {
		res_img = findLastActivityImage(activities_icon);
	}
	for (math::resource::Image * img : activities_icon) {
		Math_Answer * answer = Math_Pool::getInstance()->getAnswer();
        auto answer_size = getAnswerSize(3);
        if (num_activity > 3) answer_size = cocos2d::Size(answer_size.width, 60);
        answer->setEnable("", math::common::AnswerType::answer, CC_CALLBACK_1(Minh_CompareClock::onTouchedAnswer, this), answer_size);
        answer->setIcon(img);
        if (same_name) {
            answer->setTitleWithIcon("kind");
        }
        else {
            answer->setTitleWithIcon("name_1");
        }
        if (img == res_img) {
			answer->setCorrect(true);
		}
		else {
			answer->setCorrect(false);
		}
		_answers.push_back(answer);
	}
	
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "time") {
            std::vector<math::resource::Text> texts;
            for (math::ClockBase* clock : _clocks) {
                auto img_time = clock->getImgOfClock();
                texts.push_back(img_time->getTextValue("name_1"));
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        else if (key == "kind") {
            std::vector<math::resource::Text> texts;
            math::resource::Text img_txt;
            img_txt.text = answer;
            texts.push_back(img_txt);
            getQuestion()->setOptionalTexts(key, texts);
        }
        else {
            getQuestion()->setOptionalTexts(key, activities_icon);
        }
    }
    
    getQuestion()->setKeywordValue(index);
}

void Minh_CompareClock::onStart()
{
	onShowBottomBar();
    quizBegan(0, true);
    
    math::func::shuffle(_answers);
    float max_w = quiz_size.width * 0.35;
    if (num_activity == 3) {
        max_w = quiz_size.width * 0.3;
    }
    fixAnswerSize(max_w, quiz_size.width * 0.2);
    auto ans_size = _answers.front()->getContentSize();
    float delta = (quiz_size.width - 40 - num_activity * ans_size.width) / (num_activity + 1);
    if (delta > 100) {
        delta = 100;
    }
    math::func::smartSetPosition(_answers, ans_size, cocos2d::Rect(20, 0, quiz_size.width-40, quiz_size.height*0.25-30), delta);
    
    float delta_clock = (quiz_size.width - num_activity * clock_size) / (num_activity + 1);
    if (delta_clock > 200) {
        delta_clock = 200;
    }
    math::func::smartSetPosition(_clocks, cocos2d::Size(clock_size, clock_size), cocos2d::Rect(0, 36, quiz_size.width, quiz_size.height*0.75-getQuestion()->getContentSize().height-56), delta_clock);
    for (int i=0; i<num_activity; i++) {
        _objects[i]->setPosition(_clocks[i]->getPosition());
        addChildNode(_objects[i]);
        _objects[i]->onShow(i*0.1, math::animation::SlideUp);
        _clocks[i]->show(0.1*i, math::animation::ScaleUp);
    }
}

void Minh_CompareClock::startQuestion()
{	
	enableControlQuiz();
    for (int i=0; i<num_activity; i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
    }
}


void Minh_CompareClock::onExplanation(bool correct)
{
	if (!correct) {
        disableControlQuiz(0.25);
	}
	else {
        disableControlQuiz();
		this->nextQuestion(0.75);
	}
}

void Minh_CompareClock::onComplete()
{
    for (int i=0; i<num_activity; i++) {
        _answers[i]->setDisable(0.1*i, math::animation::SlideUp);
    }
    
    for (int i=0; i<num_activity; i++) {
        _objects[i]->onHide(i*0.1, math::animation::No_Effect);
        _clocks[i]->destroy(0.1*i, math::animation::ScaleUp);
    }
    
    quizEnded(0.75, true);
}


#pragma mark - object touch events callback

void Minh_CompareClock::onTouchedObject(cocos2d::Node * node)
{
//    disableControlQuiz(0.5);
//    Math_Object * object = (Math_Object *)node;
//    if(same_name) {
//        object->onTouchObject(false);
//    }
//    else {
//        object->onTouchObject(false);
//    }
}

void Minh_CompareClock::onTouchedAnswer(cocos2d::Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
	bool done = answer->isCorrect();
    answer->onAnimationClick(done);
    this->onExplanation(done);
}


// #debug mode
#define auto_testing_delay 2
void Minh_CompareClock::autoTesting()
{
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(3.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
	
}

void Minh_CompareClock::autoTestingRun(float dt)
{
	Math_Answer* answer = math::func::getRandomNode(_answers);
	answer->onAutoClick();
	autoTesting();
}
////
// Danh rieng cho game nay


math::resource::Image * Minh_CompareClock::findFirstActivityImage(std::vector<math::resource::Image*> images)
{
	if (images.size() == 0) return nullptr;

	math::resource::Image * img;

	img = images[0];
	for (math::resource::Image * image : images) {

		if ((image->getComparisionValue("time") % 720) < (img->getComparisionValue("time") % 720)) {
			img = image;
		}
		
	}

	return img;
}

math::resource::Image * Minh_CompareClock::findLastActivityImage(std::vector<math::resource::Image*> images)
{
	if (images.size() == 0) return nullptr;

	math::resource::Image * img;

	img = images[0];
	for (math::resource::Image * image : images) {

		if ((image->getComparisionValue("time") % 720) > (img->getComparisionValue("time") % 720)) {
			img = image;
		}
	}

	return img;
}

std::pair<int, int> Minh_CompareClock::getTimeFromImage(math::resource::Image * img)
{
	int time = img->getComparisionValue("time");
	int hour = (time / 60) % 24;
	int minute = time % 60;
	return std::make_pair(hour, minute);
}


