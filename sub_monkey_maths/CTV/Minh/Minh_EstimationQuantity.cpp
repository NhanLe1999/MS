////////
//        Minh_EstimationQuantity.cpp
//        MJ_Game
//        Create By MinhTzy on 08/11/2017
//

#include "Minh_EstimationQuantity.h"

Minh_EstimationQuantity * Minh_EstimationQuantity::createQuiz()
{
	Minh_EstimationQuantity * quiz = new Minh_EstimationQuantity();
	if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Minh_EstimationQuantity::init()
{
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Minh_EstimationQuantity::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value & json_data = document.FindMember("data")->value;

	//TODO : Game Config
	rapidjson::Value & game_data = getJsonMember(json_data, "game"); 

    // est_number
	condition = getJsonInt(game_data, "condition", 1);
    est_num = randomRangeJson(game_data, "est_number") * condition;
    number_about = est_num * 0.2;
    
    if (est_num > 15) {
        object_size = cocos2d::Size(60, 60);
    } else if (est_num > 10) {
        object_size = cocos2d::Size(70, 70);
    }
    
    std::vector<int> number_est;
    number_est.push_back(est_num + cocos2d::random(-number_about, number_about));

    int hafl = est_num/2;
    int lower_number = (est_num - number_about);
    if (hafl < lower_number) {
        lower_number = cocos2d::random(hafl, lower_number);
    }
    number_est.push_back(lower_number);
    
    if (hafl > 10) {
        hafl = 10;
    }
    int upper_number = est_num + number_about + cocos2d::random(0, hafl);
    number_est.push_back(upper_number);
    
    int index_correct = cocos2d::random(0, 2);
    
    std::vector<int> numbers_gen;
    numbers_gen.push_back(est_num);
    numbers_gen.push_back(number_est[index_correct]);
    
    std::vector<math::resource::Image*> compare_images;
    compare_images.push_back(getTextByTextGroup("about"));
    compare_images.push_back(getTextByTextGroup("fewer than"));
    compare_images.push_back(getTextByTextGroup("more than"));
    
    std::vector<math::resource::Image*> number_images;
    number_images.push_back(math::number::getNumber(est_num));

    auto obj_img = math::func::getRandomNode(getImageList("icons"));
    list_icons.push_back(obj_img);
    
    std::vector<math::resource::Image*> gr_bgs;
    gr_bgs.push_back(groupBG("red"));
    gr_bgs.push_back(groupBG("blue"));
    
    for (int i=0; i<2; i++) {
        auto group = mpool->getGroup();
        group->setEnable(i, GenObjects(numbers_gen[i]), math::group::custom, numbers_gen[i]);
        group->setObjectSize(object_size);
        group->setGroupSize(group_size);
        group->setBackground(gr_bgs[i]);
        _groups.push_back(group);
    }
    
    for (int i=0; i<3; i++) {
        auto m_answer = mpool->getAnswer();
        std::string txt_ans = cocos2d::StringUtils::format("%s %s", compare_images[i]->getText().c_str(), number_images.front()->getText().c_str());
        m_answer->setEnable(txt_ans, math::common::answer, CC_CALLBACK_1(Minh_EstimationQuantity::onTouchedAnswer, this), getAnswerSize());
        if (i==index_correct) {
            m_answer->setCorrect(true);
        }
        _answers.push_back(m_answer);
    }
    math::func::shuffle(_answers);
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : number_images) {
                texts.push_back(img->getTextValue("name_1"));
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        else {
            getQuestion()->setOptionalTexts(key, list_icons);
        }
    }
}

void Minh_EstimationQuantity::onStart()
{
    quizBegan(0, true);
    auto center = cocos2d::Vec2(quiz_size.width/2, (quiz_size.height*0.75-getQuestion()->getContentSize().height-20)/2 + quiz_size.height*0.25);
    _groups.front()->setPosition(center);
    addChildNode(_groups.front());
    _groups.front()->onShow(0, math::animation::ScaleUp);
    
    fixAnswerSize(quiz_size.width*0.25, quiz_size.width*0.2);
    float delta = (quiz_size.width - 40 - _answers.front()->getContentSize().width * (int)_answers.size()) / ((int)_answers.size() + 1);
    if (delta>100) {
        delta = 100;
    }
    
    math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(20, 0, quiz_size.width-40, quiz_size.height*0.25-30), delta);
}

void Minh_EstimationQuantity::startQuestion()
{
	if(this->getQuestion()->getData().index == 0) {
        nextQuestion(3.5);
	}
	else {
        disableControlQuiz(0.5);
		// Start Question
		onShowBottomBar();
        auto center = cocos2d::Vec2(quiz_size.width/2, (quiz_size.height*0.75-getQuestion()->getContentSize().height-20)/2);
        float delta = (group_size.width + 50) / 2;
        
        _groups.front()->setPosition(center);
        _groups.front()->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(-delta, 0))));
        
        _groups.back()->setPosition(cocos2d::Vec2(center.x + delta, center.y));
        addChildNode(_groups.back());
        _groups.back()->onShow(0, math::animation::SlideLeft);
        
        for (int i=0; i<3; i++) {
            _answers[i]->onShowInParent(_panel_bot, 2, 0.5+i*0.1, math::animation::SlideUp);
        }
	}
}


void Minh_EstimationQuantity::onExplanation(bool correct)
{
	if (!correct) {
        disableControlQuiz(0.25);
	}
	else {
        disableControlQuiz();
		this->nextQuestion(0.75);
	}
}

void Minh_EstimationQuantity::onComplete()
{
	//TODO: Thuc hien them cac lenh o day
    for (int i=0; i<3; i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }
    
    for (int i=0; i<2; i++) {
        _groups[i]->onHide(0.25+i*0.1, math::animation::SlideUp);
    }
    quizEnded(0.5, true);
}


#pragma mark - object touch events callback

void Minh_EstimationQuantity::onTouchedAnswer(cocos2d::Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
	bool done = answer->isCorrect();
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

// #debug mode
#define auto_testing_delay 2
void Minh_EstimationQuantity::autoTesting()
{
	if (this->getQuestion()->getData().index == 0) {
		this->scheduleOnce([=](float dt){
		}, 5.0, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
	}
	else {
		this->scheduleOnce([=](float dt) {
			this->autoTestingRun(3.0);
		}, auto_testing_delay, cocos2d::StringUtils::format("aitoTesting-%d", testing_step++));
	}
}

void Minh_EstimationQuantity::autoTestingRun(float dt)
{
	Math_Answer* answer = math::func::getRandomNode(_answers);
	answer->onAutoClick();
	autoTesting();
}

////
// Danh rieng cho game nay
std::vector<Math_Object*> Minh_EstimationQuantity::GenObjects(int num)
{
	std::vector<Math_Object*> objects;
	for (int i = 0; i < num; ++i) {
		Math_Object * object = mpool->getObject();
		object->setEnable(i, math::common::deny, list_icons.front(), NULL);
		object->setObjectSize(object_size);
		objects.push_back(object);
	}
	return objects;
}
