#include "Minh_RememberActivities.h"


////////
//		Minh_RememberActivities.cpp
//		MJ_Game
//		Create By MinhTzy on 08/11/2017
//

// Compare class
struct Minh_CompImageByTime {
	bool operator()(math::resource::Image * a, math::resource::Image * b) {
		return a->getComparisionValue("time") < b->getComparisionValue("time");
	}
} minhCompImgTime;
// Life Quiz
Minh_RememberActivities * Minh_RememberActivities::createQuiz()
{
	Minh_RememberActivities * quiz = new Minh_RememberActivities();
	if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Minh_RememberActivities::init()
{
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Minh_RememberActivities::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value & json_data = document.FindMember("data")->value;

	//TODO : Game Config
	rapidjson::Value & game_data = getJsonMember(json_data, "game");

    num_activity = randomRangeJson(game_data, "num_activity");
	sorted = getJsonBool(game_data, "sorted", true);

	// TODO : ICON CONFIG
	rapidjson::Value & icon_data = getJsonMember(json_data, "icon");

	icon_same = getJsonString(icon_data, "same", "name"); 
	icon_different = getJsonString(icon_data, "different", "id");

	// TODO : GET ICONS OPERATOR & NUMBER
	activities_icon = getImageList("icons", num_activity, icon_different, icon_same);

	if (sorted) std::sort(activities_icon.begin(), activities_icon.end(),minhCompImgTime);
	else math::func::shuffle(activities_icon);
	//----> Lay xong data cua game
	
	//TODO: GEN QUIZ COMPONENTS
	// Gen clock
	for (int i = 0; i < num_activity; ++i) {
		std::pair<int, int> time = getTimeFromImage(activities_icon.at(i));
        
        auto clock = math::AnalogClock::createClock(1);
        clock->setClockSize(300);
        clock->setNumberAsText(true);
        clock->setTime(time.first, time.second);
        _clocks.push_back(clock);
        
        Math_Object * object = mpool->getObject();
        object->setEnable(i, math::common::deny, activities_icon[i], NULL);
        object->setObjectSize(image_size);
        object->fixHeightObject();
        _objects.push_back(object);
	}
    
    std::vector<math::AnalogClock *> _clocks_ask;
    std::vector<Math_Object *> _object_ask;
    for (int i = 0; i < num_activity; ++i) {
        correct_answers.push_back(cocos2d::random(0, 1)==0);
    }

    for (int i = 0; i < num_activity; ++i) {
        std::pair<int, int> time = getTimeFromImage(activities_icon.at(i));
        auto hour_fix = time.first;
        if (!correct_answers[i]) {
            hour_fix = (time.first + cocos2d::random(4, 8)) % 12;
        }
        auto clock = math::AnalogClock::createClock(1);
        clock->setClockSize(300);
        clock->setNumberAsText(true);
        clock->setTime(hour_fix, time.second);
        _clocks_ask.push_back(clock);
        
        Math_Object * object = mpool->getObject();
        object->setEnable(i, math::common::deny, activities_icon[i], NULL);
        object->setObjectSize(image_size);
        object->fixHeightObject();
        _object_ask.push_back(object);
    }
    
    auto correct = correct_answers;
    correct_answers.clear();
    
    auto random_quest = randomArrayIndexes(num_activity, num_activity);
    for (int i=0; i<random_quest.size(); i++) {
        _clocks.push_back(_clocks_ask[i]);
        _objects.push_back(_object_ask[i]);
        correct_answers.push_back(correct[i]);
        activities_icon.push_back(activities_icon[i]);
    }
    
    answer_img.push_back(getTextByTextGroup("yes"));
    answer_img.push_back(getTextByTextGroup("no"));
    answer_img.push_back(getTextByTextGroup("next"));
    
    //#4. Gen Answer
	for (int i = 0; i < num_activity; ++i) {
        auto next = mpool->getAnswer();
        next->setEnable("", math::common::answer, CC_CALLBACK_1(Minh_RememberActivities::onTouchedAnswer, this), getAnswerSize());
        next->setIcon(answer_img.back());
        next->setTitleWithIcon("name_1");
        next->setCorrect(true);
        _answers.push_back(next);
	}
    
    for (int i = 0; i < num_activity; ++i) {
        auto btn_yes = mpool->getAnswer();
        btn_yes->setEnable("", math::common::answer, CC_CALLBACK_1(Minh_RememberActivities::onTouchedAnswer, this), getAnswerSize());
        btn_yes->setIcon(answer_img[0]);
        btn_yes->setTitleWithIcon("name_1");
        _answers.push_back(btn_yes);
        
        auto btn_no = mpool->getAnswer();
        btn_no->setEnable("", math::common::answer, CC_CALLBACK_1(Minh_RememberActivities::onTouchedAnswer, this), getAnswerSize());
        btn_no->setIcon(answer_img[1]);
        btn_no->setTitleWithIcon("name_1");
        _answers.push_back(btn_no);
        
        if (correct_answers[i]) {
            btn_yes->setCorrect(true);
        } else {
            btn_no->setCorrect(true);
        }
    }
    
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
    std::vector<int> question_loops;
    question_loops.push_back(num_activity);
    question_loops.push_back(num_activity);
	parseQuestionWithLoops(question_loops);
    
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "time") {
			std::vector<math::resource::Text> texts;
            for (int i=0; i<num_activity*2; i++) {
                auto img = _clocks[i]->getImgOfClock();
				texts.push_back(img->getTextValue("name_1"));
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else {
			getQuestion()->setOptionalTexts(key, activities_icon);
		}
	}
}

void Minh_RememberActivities::onStart()
{
    onShowBottomBar();
    quizBegan(0, true);
    
    auto clock = _clocks.front();
    auto object = _objects.front();
    
    auto center = cocos2d::Vec2(quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height-20) * 0.5;
    float total_width = object->getContentSize().width + clock->getContentSize().width;
    float delta = (quiz_size.width - total_width - 50) / 2;
    
    clock->setPosition(cocos2d::Vec2(delta + clock->getContentSize().width * 0.5, center.y));
    object->setPosition(cocos2d::Vec2(quiz_size.width - delta - object->getContentSize().width * 0.5, center.y));
    addChildNode(clock);
	addChildNode(object);
    clock->show();
    object->onShow(0);
}

void Minh_RememberActivities::startQuestion()
{
    enableControlQuiz();
    int index_ques = getQuestion()->getData().index;
    
    //show clock and object
    if (index_ques == 0) {
        
    } else {
        disableControlQuiz(0.5);
        _clocks[index_ques-1]->destroy(0, math::animation::No_Effect);
        _objects[index_ques-1]->onHide(0, math::animation::No_Effect);
        
        auto clock = _clocks[index_ques];
        auto object = _objects[index_ques];
        
        auto center = cocos2d::Vec2(quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height-20) * 0.5;
        float total_width = object->getContentSize().width + clock->getContentSize().width;
        float delta = (quiz_size.width - total_width - 50) / 2;
        
        clock->setPosition(cocos2d::Vec2(delta + clock->getContentSize().width * 0.5, center.y));
        object->setPosition(cocos2d::Vec2(quiz_size.width - delta - object->getContentSize().width * 0.5, center.y));
        addChildNode(clock);
        addChildNode(object);
        clock->show();
        object->onShow(0);
    }
    
    //Hide old answers
    for (int i=0; i<answers.size(); i++) {
        answers[i]->setDisable(0.1*i, math::animation::SlideDown);
    }
    
    //show new answers
    answers.clear();
    if (index_ques < num_activity) {
        answers.push_back(_answers[index_ques]);
    } else {
        int fix_index = index_ques-num_activity;
        answers.push_back(_answers[num_activity + fix_index*2]);
        answers.push_back(_answers[num_activity + fix_index*2+1]);
    }
    math::func::shuffle(answers);
    math::func::smartSetPosition(answers, answers.front()->getContentSize(), cocos2d::Rect(20, 0, quiz_size.width-40, quiz_size.height*0.25-30), 150);
    for (int i=0; i<answers.size(); i++) {
        answers[i]->onShowInParent(_panel_bot, 2, 0.1*i + 0.25, math::animation::SlideUp);
    }
}

void Minh_RememberActivities::onExplanation(bool correct)
{
	if (!correct) {
        disableControlQuiz(0.25);
	} else {
        disableControlQuiz();
        nextQuestion(0.75);
    }
}

void Minh_RememberActivities::onComplete()
{
	//TODO: Thuc hien them cac lenh o day
    for (int i=0; i<answers.size(); i++) {
        answers[i]->setDisable(0.1*i + 0.25, math::animation::SlideUp);
    }
    _objects.back()->onHide(0);
    _clocks.back()->destroy();
    
    quizEnded(0.75, true);
}


#pragma mark - object touch events callback
void Minh_RememberActivities::onTouchedAnswer(cocos2d::Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
	bool done = answer->isCorrect();
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

std::pair<int, int> Minh_RememberActivities::getTimeFromImage(math::resource::Image * img)
{
    int time = img->getComparisionValue("time");
    int hour = (time / 60) % 24;
    int minute = time % 60;
    return std::make_pair(hour, minute);
}

// #debug mode
#define auto_testing_delay 1.5f
void Minh_RememberActivities::autoTesting()
{
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Minh_RememberActivities::autoTestingRun(float dt)
{
//    if (current_index <= num_activity) {
//        clock_touch->onTouched();
//    }
//    else {
//        int index = (current_index - 1) % num_activity;
//        Math_Answer * answer = static_cast<Math_Answer *> (math::func::getRandomNode(_answers_map.at(index)));
//        answer->onAutoClick();
//    }
//    this->scheduleOnce([=](float dt) {
//        this->autoTesting();
//    }, dt, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step - 1));
}
