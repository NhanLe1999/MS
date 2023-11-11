//
//  Nam_ChooseRelatedFact
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_ChooseRelatedFact.h"

Nam_ChooseRelatedFact* Nam_ChooseRelatedFact::createQuiz()
{
    Nam_ChooseRelatedFact* quiz=new Nam_ChooseRelatedFact();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_ChooseRelatedFact::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_ChooseRelatedFact::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	game_mode = json_game.FindMember("mode")->value.GetString();
	if (game_mode == "random") {
		if (RandomHelper::random_int(0, 1)) {
			game_mode = "add";
		}
		else {
			game_mode = "sub";
		}
	}
	minResult = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	maxResult = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
	
	
	// FIX LAI QUESTION
	this->parseQuestion();
	if (game_mode == "add") this->getQuestion()->setKeywordValue(0);
	else this->getQuestion()->setKeywordValue(1);
	//TODO: RANDOM ket qua
	int a, b, c;
	a = RandomHelper::random_int(std::max(minResult,3),maxResult-1);
	while (1) {
		b = RandomHelper::random_int(0, maxResult - a);
		if (b != a && b != std::abs(a - b)) break;
	}
	if (game_mode == "add") {
		c = a + b;
	}
	else {
		if (a < b) std::swap(a, b);
		c = a - b;
	}
	if (game_mode == "add") {
        str_ques_cal = cocos2d::StringUtils::format("%d + %d = %d", a, b, c);
		str_result = cocos2d::StringUtils::format("%d + %d = %d", b, a, c);
	}
	else {
        str_ques_cal = cocos2d::StringUtils::format("%d - %d = %d", a, b, c);
        str_result = cocos2d::StringUtils::format("%d - %d = %d", a, c, b);
	}
	// gen dap an sai
	for (int i = 0; i < 3; i++) {
		while (1) {
			int x, y, z;
			std::string dap_an_sai;
			x = RandomHelper::random_int(std::max(minResult, 3), maxResult - 1);
			y = RandomHelper::random_int(0, maxResult - x);
			if (game_mode == "add") {
				z = x + y;
			}
			else {
				if (x < y) std::swap(x, y);
				z = x - y;
			}
			if (game_mode == "add") {
				dap_an_sai = cocos2d::StringUtils::format("%d + %d = %d", x, y, z);
			}
			else {
                dap_an_sai = cocos2d::StringUtils::format("%d - %d = %d", x, y, z);
			}
			bool kt = true;
			for (std::string old : str_answers) {
				if (old == dap_an_sai) kt = false;

			}
			if (dap_an_sai != str_result && dap_an_sai != str_ques_cal && kt) {
				str_answers.push_back(dap_an_sai);
				break;
			}
		}
	}
	str_answers.push_back(str_result);
	std::random_shuffle(str_answers.begin(), str_answers.end());
	//TODO: GEN CAC DOI TUONG
	auto allImage = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()),"",image_prefix);
	auto imageResult = Math_Pool::getInstance()->getImage();
	for (math::resource::Image* img : allImage) {
		if (img->getComparisionValue("number") == c) {
			img->used = true;
			imageResult = img;
			break;
		}
	}
	Math_Object *obj_ques = Math_Pool::getInstance()->getObject();
	obj_ques->setEnable(0, math::common::TouchPermission::deny, imageResult, NULL);
	obj_ques->setObjectSize(Size(400, 200));
	obj_ques->setValueText(str_ques_cal);
	obj_ques->setPosition(Vec2(quiz_size.width/2, quiz_size.height / 2-200));
	obj_ques->setImageVisible(false);
	obj_ques->getLabelValue()->setVisible(true);
	obj_ques->getLabelValue()->setFontSize(100);
	_objects.push_back(obj_ques);
	Math_Object *obj_result = Math_Pool::getInstance()->getObject();
	obj_result->setEnable(0, math::common::TouchPermission::deny, imageResult, NULL);
	obj_result->setObjectSize(Size(400, 200));
	obj_result->setValueText(str_result);
	obj_result->setPosition(Vec2(-300, -300));
	obj_result->setImageVisible(false);
	obj_result->getLabelValue()->setVisible(true);
	obj_result->getLabelValue()->setFontSize(100);
	_objects.push_back(obj_result);

    for (int i = 0; i < 4; i++) {
		Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
		ans->setEnable(str_answers[i],math::common::answer, CC_CALLBACK_1(Nam_ChooseRelatedFact::onAnswerDone, this, str_answers[i]), this->getAnswerSize(3));
		_answers.push_back(ans);
	}
}

void Nam_ChooseRelatedFact::onStart() {
    onShowBottomBar(0, 0.4);
    quizBegan(0, true);
    
    _objects.front()->setPosition(cocos2d::Vec2(quiz_size.width, quiz_size.height*0.6-getQuestion()->getContentSize().height-20) * 0.5);
    this->addChildNode(_objects.front());
    _objects.front()->onShow(0);
    
    fixAnswerSize(quiz_size.width *0.275, quiz_size.width * 0.25);
    float start = (quiz_size.width - _answers.front()->getContentSize().width * 2 - 150) / 2;
    math::func::setPositionList(_answers, 2, cocos2d::Rect(start, 10, quiz_size.width-2*start, quiz_size.height*0.4-40));
}

void Nam_ChooseRelatedFact::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index == 0) {
        for (int i=0; i < _answers.size(); i++) {
            _answers[i]->onShowInParent(_panel_bot, 2, i*0.1, math::animation::SlideUp);
        }
	}
}

void Nam_ChooseRelatedFact::onExplanation(bool correct) {
    if (correct) {
        disableControlQuiz();
        auto position = _objects.front()->getPosition();
        _objects.front()->runAction(cocos2d::MoveBy::create(0.3, cocos2d::Vec2(0, 60)));
        
        _objects[1]->setPosition(cocos2d::Vec2(position.x, position.y - 60));
        addChildNode(_objects[1]);
        _objects[1]->onShow(0.25, math::animation::SlideUp);
        
        nextQuestion(2.0);
    } else {
        disableControlQuiz(0.25);
    }
}

void Nam_ChooseRelatedFact::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (int i=0; i < _answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }
    
    float m_delay = 0;
	for (Math_Object* object : _objects) {
        object->onHide(m_delay, math::animation::ScaleUp);
		m_delay += 0.1;
	}

    quizEnded(0.75, true);
}

#pragma mark - object touch events callback

void Nam_ChooseRelatedFact::onAnswerDone(Node* node, std::string str_ans) {
	Math_Answer* btn = (Math_Answer*)node;
    bool done = str_ans == str_result;
    btn->onAnimationClick(done);
    this->onExplanation(done);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_ChooseRelatedFact::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_ChooseRelatedFact::autoTestingRun(float delaytime)
{
   
}

