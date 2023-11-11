//
//  Nam_WhichSentenceIsTrue
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#include "Nam_WhichSentenceIsTrue.h"

Nam_WhichSentenceIsTrue* Nam_WhichSentenceIsTrue::createQuiz()
{
    Nam_WhichSentenceIsTrue* quiz=new Nam_WhichSentenceIsTrue();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_WhichSentenceIsTrue::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_WhichSentenceIsTrue::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	minResult = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	maxResult = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
	int minNumberSen = json_game.FindMember("numberOfSentences")->value.FindMember("min")->value.GetInt();
	int maxNumberSen = json_game.FindMember("numberOfSentences")->value.FindMember("max")->value.GetInt();
	numberSentence = RandomHelper::random_int(minNumberSen, maxNumberSen);
	if (RandomHelper::random_int(0, 1)) {
		modeGame = "true";
	}
	else {
		modeGame = "false";
	}
	keyGame = json_game.FindMember("keyGame")->value.GetString();
	if (keyGame == "random") {
		if (RandomHelper::random_int(0, 1)) {
			keyGame = "add";
		}
		else {
			keyGame = "sub";
		}
	}
	//TODO: GEN 
    bg_sentence = math::resource::ResHelper::createImage(getUIPath("common/answer2.png"));
	bg_sentence->used = true;
	bg_sentence_choosed = objectBG();
	bg_sentence_choosed->used = true;
	soDapAnDung = RandomHelper::random_int(1, numberSentence - 1);
    Size size_sentence = Size(350,120);
	// GET QUESTION
	this->parseQuestion();
	if (modeGame == "true" && soDapAnDung == 1) this->getQuestion()->setKeywordValue(0);
	if (modeGame == "true" && soDapAnDung != 1) this->getQuestion()->setKeywordValue(2);
	if (modeGame == "false" && (numberSentence-soDapAnDung) == 1) this->getQuestion()->setKeywordValue(1);
	if (modeGame == "false" && (numberSentence - soDapAnDung) != 1) this->getQuestion()->setKeywordValue(3);
	// gen dap an dung
	for (int i = 0; i < soDapAnDung; i++) {
		while (1) {
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			int dapan = RandomHelper::random_int(minResult, maxResult);
			obj->setEnable(1, math::common::TouchPermission::touch, bg_sentence, NULL);
			obj->setImageVisible(false);
			obj->setBackground(bg_sentence, 0.4);
			obj->setObjectSize(size_sentence);
			obj->setTouchCallback(CC_CALLBACK_1(Nam_WhichSentenceIsTrue::onTouchedObject, this));
			int x = RandomHelper::random_int(1, dapan);
			if (keyGame == "add") {
				obj->setValueText(cocos2d::StringUtils::toString(x) + " + " + cocos2d::StringUtils::toString(dapan - x) + " = " + cocos2d::StringUtils::toString(dapan));
			}
			else {
				// sub
				obj->setValueText(cocos2d::StringUtils::toString(dapan)+ " - " +cocos2d::StringUtils::toString(x) + " = " + cocos2d::StringUtils::toString(dapan - x));
			}
			obj->setName("0");
			obj->getLabelValue()->setVisible(true);
			obj->getLabelValue()->setFontSize(50);
			obj->getLabelValue()->setTextColor(Color4B::WHITE);
            obj->getLabelValue()->disableEffect();
			bool random_done = true;
			for (Math_Object* ob : _objects) {
				if (ob->getValueText() == obj->getValueText()) {
					random_done = false;
					break;
				}
			}
			if (random_done) {
				_objects.push_back(obj);
				break;
			}
		}
		
	}
	//gen dap an sai
	for (int i = 0; i < numberSentence - soDapAnDung; i++) {
		while (1) {
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			int dapan = RandomHelper::random_int(minResult, maxResult);
			obj->setEnable(0, math::common::TouchPermission::touch, bg_sentence, NULL);
			obj->setImageVisible(false);
			obj->setBackground(bg_sentence, 0.4);
			obj->setObjectSize(size_sentence);
			obj->setTouchCallback(CC_CALLBACK_1(Nam_WhichSentenceIsTrue::onTouchedObject, this));
			if (keyGame == "add") {
				while (1) {
					int x = RandomHelper::random_int(0, dapan - 1);
					int y = RandomHelper::random_int(0, dapan - 1);
					if (x + y != dapan) {
						obj->setValueText(cocos2d::StringUtils::toString(x) + " + " + cocos2d::StringUtils::toString(y) + " = " + cocos2d::StringUtils::toString(dapan));
						break;
					}
				}
			}
			else {
				// sub
				while (1) {
					int x = RandomHelper::random_int(0, dapan - 1);
					int y = RandomHelper::random_int(0, x);
					if (x - y != dapan) {
						obj->setValueText(cocos2d::StringUtils::toString(x) + " - " + cocos2d::StringUtils::toString(y) + " = " + cocos2d::StringUtils::toString(dapan));
						break;
					}
				}
			}
			
			obj->setName("0");
			obj->getLabelValue()->setVisible(true);
			obj->getLabelValue()->setFontSize(50);
			obj->getLabelValue()->setTextColor(Color4B::WHITE);
            obj->getLabelValue()->disableEffect();
			bool random_done = true;
			for (Math_Object* ob : _objects) {
				if (ob->getValueText() == obj->getValueText()) {
					random_done = false;
					break;
				}
			}
			if (random_done) {
				_objects.push_back(obj);
				break;
			}
		}
		
	}
	std::random_shuffle(_objects.begin(), _objects.end());
	if (numberSentence > 2) math::func::setPositionList(_objects, 2, Rect(0.1, 0.1, 0.8, 0.4));
	else math::func::setPositionList(_objects, 1, Rect(0.1, 0.1, 0.8, 0.4));
	//fix position
	for (Math_Object* obj : _objects) {
		//obj->setPositionY(obj->getPositionY() - 0.2*quiz_size.height);

	}
	//Math_Pool::getInstance()->resetImages();
}

void Nam_WhichSentenceIsTrue::onStart() {
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	//Gen question
	onShowBottomBar(0, 0.2, false, CC_CALLBACK_1(Nam_WhichSentenceIsTrue::onAnswerDone, this), nullptr, true);
    quizBegan(0, true);
}

void Nam_WhichSentenceIsTrue::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_WhichSentenceIsTrue::onExplanation(bool correct) {}
void Nam_WhichSentenceIsTrue::onComplete() {
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
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback
void Nam_WhichSentenceIsTrue::onTouchedObject(cocos2d::Node* node){	
	Math_Object* object = (Math_Object*)node;
	if (object->getName() == "0") {
		object->setBackground(bg_sentence_choosed, 0.4);
		object->getLabelValue()->setTextColor(Color4B(Color3B(99,51,15)));
		object->setName("1");
	}
	else {
		object->setBackground(bg_sentence, 0.4);
		object->getLabelValue()->setTextColor(Color4B::WHITE);
		object->setName("0");
	}
}
void Nam_WhichSentenceIsTrue::onAnswerDone(Node* node) {
	Math_Answer* btn = (Math_Answer*)node;
	bool done = true;
	if (modeGame == "true") {
		for (Math_Object* obj : _objects) {
			if (obj->getID() == 1 && obj->getName() == "0") {
				done = false;

			}
			if (obj->getID() == 0 && obj->getName() == "1") {
				done = false;
			}
		}
	}
	else {
		for (Math_Object* obj : _objects) {
			if (obj->getID() == 1 && obj->getName() == "1") {
				done = false;

			}
			if (obj->getID() == 0 && obj->getName() == "0") {
				done = false;
			}
		}
	}
    btn->onAnimationClick(done);
	if (done) this->nextQuestion();
	else {
		for (Math_Object* obj : _objects) {
			if (obj->getID() == 0 && obj->getName() == "1") {
				obj->setBackground(bg_sentence, 0.4);
				obj->getLabelValue()->setTextColor(Color4B::WHITE);
			}
			else {
				obj->setBackground(bg_sentence, 0.4);
				obj->getLabelValue()->setTextColor(Color4B::WHITE);
			}
			obj->setName("0");
		}
	}
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_WhichSentenceIsTrue::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("test-%d", testing_step++));
}

void Nam_WhichSentenceIsTrue::autoTestingRun(float delaytime)
{
   
}

