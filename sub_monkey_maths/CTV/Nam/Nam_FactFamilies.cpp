//
//  Nam_FactFamilies
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_FactFamilies.h"

Nam_FactFamilies* Nam_FactFamilies::createQuiz()
{
    Nam_FactFamilies* quiz=new Nam_FactFamilies();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_FactFamilies::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_FactFamilies::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	game_mode = json_game.FindMember("mode")->value.GetString();
	min_result = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	max_result = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
	
	//
	img_null = Math_Pool::getInstance()->getImage();
	this->parseQuestion();
	//TODO: random ket qua
	int a, b, c;
	a = RandomHelper::random_int(std::max(min_result, 3), max_result - 1);
	while (1) {
		b = RandomHelper::random_int(0, max_result - a);
		if (b != a && b != std::abs(a-b)) break;
	}
	if (a < b) std::swap(a, b);
	str_box.push_back(cocos2d::StringUtils::toString(a) + " + " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(a + b));
	str_box.push_back(cocos2d::StringUtils::toString(b) + " + " + cocos2d::StringUtils::toString(a) + " = " + cocos2d::StringUtils::toString(a + b));
	str_box.push_back(cocos2d::StringUtils::toString(a+b) + " - " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(a));
	str_box.push_back(cocos2d::StringUtils::toString(a+b) + " - " + cocos2d::StringUtils::toString(a) + " = " + cocos2d::StringUtils::toString(b));
	if (game_mode == "yes_or_no") {
		// random ket qua mode yes_or_no
		if (RandomHelper::random_int(0, 1)) {
			str_result = "yes";
		}
		else {
			str_result = "no";
			while (1) {
				int index = RandomHelper::random_int(0, 3);
				a = RandomHelper::random_int(std::max(min_result, 3), max_result - 1);
				b = RandomHelper::random_int(0, max_result - a);
				std::string str_false;
				if (index < 2) {
					str_false = cocos2d::StringUtils::toString(a) + " + " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(a + b);
				}
				else {
					if (a < b) std::swap(a, b);
					str_false = cocos2d::StringUtils::toString(a) + " - " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(a - b);
				}
				if (str_false != str_box[index]) {
					str_box[index] = str_false;
					break;
				}
			}
		}
	}
	else {
		// random ket qua va dap an mode fill_missing
		int index = RandomHelper::random_int(0, 3);
		str_result = str_box[index];
		str_box[index] = "?";
		for (int i = 0; i < 3; i++) {
			while (1) {
				int x, y;
				std::string dap_an_sai;
				x = RandomHelper::random_int(std::max(min_result, 3), max_result - 1);
				y = RandomHelper::random_int(0, max_result - x);
				if (x < y) std::swap(x, y);
				if (RandomHelper::random_int(0,1)) {
					dap_an_sai = cocos2d::StringUtils::toString(x) + " + " + cocos2d::StringUtils::toString(y) + " = " + cocos2d::StringUtils::toString(x+y);
				}
				else {
					dap_an_sai = cocos2d::StringUtils::toString(x) + " - " + cocos2d::StringUtils::toString(y) + " = " + cocos2d::StringUtils::toString(x-y);
				}
				bool kt = true;
				for (std::string old : str_answers) {
					if (old == dap_an_sai) kt = false;

				}
				if (dap_an_sai != str_result  && kt) {
					str_answers.push_back(dap_an_sai);
					break;
				}
			}
		}
		str_answers.push_back(str_result);
		std::random_shuffle(str_answers.begin(), str_answers.end());
	}
	
	//TODO: GEN 
	box_1 = ui::ImageView::create(m_theme->getThemePath() + "common/frame/square.png");
	box_1->setScale9Enabled(true);
	box_1->setCapInsets(Rect(10, 10, box_1->getVirtualRendererSize().width - 20, box_1->getVirtualRendererSize().height - 20));
	box_1->setContentSize(Size(400, 300));
	box_1->setPosition(Vec2(quiz_size.width/2-250,quiz_size.height*0.35));
	this->addChildNode(box_1, 1);
	box_2 = ui::ImageView::create(m_theme->getThemePath()+ "common/frame/square.png");
	box_2->setScale9Enabled(true);
	box_2->setCapInsets(Rect(10, 10, box_2->getVirtualRendererSize().width - 20, box_2->getVirtualRendererSize().height - 20));
	box_2->setContentSize(Size(400, 300));
	box_2->setPosition(Vec2(quiz_size.width / 2 + 250, quiz_size.height*0.35));
	this->addChildNode(box_2, 1);

	for (int i = 0; i < 4; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		this->genObjCal(obj, str_box[i]);
		if (i < 2) obj->setPosition(Vec2(quiz_size.width / 2 - 250, quiz_size.height*0.35 + 70 - i * 130));
		else obj->setPosition(Vec2(quiz_size.width / 2 + 250, quiz_size.height*0.35 + 70 - (i-2) * 130));
		_objects.push_back(obj);
	}
	auto img_slot = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/answer.png");
	if (game_mode == "yes_or_no") {
		// gen dap an cho mode yes_or_no
		Math_Answer* yes = Math_Pool::getInstance()->getAnswer();
		yes->setEnable("Yes", math::common::answer, CC_CALLBACK_1(Nam_FactFamilies::onAnswerDone, this, "yes"), this->getAnswerSize());
		yes->setCustomAnswer(img_slot, this->getAnswerSize(2), 40, Color4B(Color3B(99,51,15)),0.2);
		_answers.push_back(yes);
		Math_Answer* no = Math_Pool::getInstance()->getAnswer();
		no->setEnable("No", math::common::answer, CC_CALLBACK_1(Nam_FactFamilies::onAnswerDone, this, "no"), this->getAnswerSize());
		no->setCustomAnswer(img_slot, this->getAnswerSize(2), 40, Color4B(Color3B(99,51,15)),0.2);
		_answers.push_back(no);
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0.2, 0.00, 0.6, 0.21));
	}
	else {
		// gen dap an cho mode fill_missing
		for (int i = 0; i < 4; i++) {
			Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
			ans->setEnable(str_answers[i], math::common::answer, CC_CALLBACK_1(Nam_FactFamilies::onAnswerDone, this, str_answers[i]), this->getAnswerSize(3));
			ans->setCustomAnswer(img_slot, Size(250,80), 40, Color4B(Color3B(99,51,15)),0.2);
			_answers.push_back(ans);
		}
		math::func::setPositionList(_answers, 2 , cocos2d::Rect(0.1, 0.00, 0.8, 0.21));
	}
	
	//Math_Pool::getInstance()->resetImages();
}
void Nam_FactFamilies::genObjCal(Math_Object* obj, std::string str) {
	obj->setEnable(0, math::common::TouchPermission::deny, img_null, NULL);
	obj->setObjectSize(Size(250, 150));
	obj->setValueText(str);
	obj->setImageVisible(false);
	obj->getLabelValue()->setVisible(true);
	obj->getLabelValue()->setFontSize(65);
	obj->getLabelValue()->setTextColor(Color4B::WHITE);
    obj->getLabelValue()->disableEffect();
}
void Nam_FactFamilies::onStart() {
	math_statistic->setCurrentGame(_game);
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	for (Math_Answer* ans : _answers) {
		ans->onShowInParent(_panel_bot, 2);
	}
	onShowBottomBar();
    showQuestionBox();
}

void Nam_FactFamilies::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_FactFamilies::onExplanation(bool correct) {}
void Nam_FactFamilies::onComplete() {
    //TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.02;
	}
	m_delay += 0.1;
	box_2->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay),
		cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)),
		cocos2d::CallFunc::create([=]() {
		box_2->setVisible(false);
	}), NULL));
	box_1->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay),
		cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)),
		cocos2d::CallFunc::create([=]() {
		box_1->setVisible(false);
	}), NULL));
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback

void Nam_FactFamilies::onAnswerDone(Node* node, std::string str) {

	Math_Answer* btn = (Math_Answer*)node;
	if (str == str_result) {
		if (game_mode == "fill_missing") {
			for (Math_Object* obj : _objects) {
				if (obj->getValueText() == "?") {
					obj->setValueText(str_result);
				}
			}
		}
        btn->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		btn->onAnimationClick(false);
	}
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_FactFamilies::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_FactFamilies::autoTestingRun(float delaytime)
{
   
}

