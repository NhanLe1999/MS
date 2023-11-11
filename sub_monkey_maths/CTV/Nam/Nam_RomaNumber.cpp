//
//  Nam_RomaNumber
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#include "Nam_RomaNumber.h"

Nam_RomaNumber* Nam_RomaNumber::createQuiz()
{
    Nam_RomaNumber* quiz=new Nam_RomaNumber();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_RomaNumber::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_RomaNumber::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	modeGame = json_game.FindMember("modeGame")->value.GetString();
	int min = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	int max = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
	result = RandomHelper::random_int(min,max);
	auto allImage = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
	for (math::resource::Image* img : allImage) {
		if (img->getComparisionValue("number") == result) {
			img->used = true;
			imageResult = img;
			break;
		}
	}
	imageResult = this->getNumbers(result, result)[0];
	imageResult->used = true;
	math::resource::Image* img_slot = objectBG();
	answers.push_back(result);
	while (1) {
		int r = RandomHelper::random_int(min, max);
		bool kt = true;
		for (int i : answers) {
			if (i == r) {
				kt = false;
				break;
			}
		}
		if (kt) answers.push_back(r);
		if (answers.size() == 4) break;
	}
	std::random_shuffle(answers.begin(), answers.end());
	// GEN QUESTION
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = imageResult->getTextValue("name_1", m_lang->getLanguage());
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	//Math_Pool::getInstance()->resetImages();
	//TODO: GEN
    
	objResult = Math_Pool::getInstance()->getObject();
	objResult->setEnable(0, math::common::TouchPermission::deny, imageResult, NULL);
	objResult->setImageVisible(false);
	objResult->setObjectSize(Size(200, 200));
	if (modeGame == "DecimalToRoma") {
		objResult->setValueText(cocos2d::StringUtils::toString(objResult->getComparisionValue().number));
	}
	else {
        objResult->setValueText(math::RomanNumeralsUtils::int_2_roman(imageResult->getComparisionValue("number")));
	}
	objResult->getLabelValue()->setFontSize(250);
	objResult->getLabelValue()->setVisible(true);
	objResult->setPosition(Vec2(quiz_size.width/2,quiz_size.height*0.35));
	_objects.push_back(objResult);
	for (int i = 0; i < 4; i++) {
		Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
		if (modeGame == "DecimalToRoma") {
            ans->setEnable(math::RomanNumeralsUtils::int_2_roman(answers[i]), math::common::answer,CC_CALLBACK_1(Nam_RomaNumber::onAnswerDone,this,answers[i]), cocos2d::Size(220, 70));
		}
		else {
			ans->setEnable(cocos2d::StringUtils::toString(answers[i]), math::common::answer, CC_CALLBACK_1(Nam_RomaNumber::onAnswerDone, this, answers[i]), cocos2d::Size(220, 70));
		}
		ans->setCustomAnswer(img_slot,Size(150,150),80,Color4B(Color3B(99,51,15)));
		_answers.push_back(ans);
	}
	math::func::setPositionList(_answers, _answers.size(), Rect(0.1, 0, 0.8, 0.21));
}
void Nam_RomaNumber::onStart() {
	math_statistic->setCurrentGame(_game);
	//
	this->addChildNode(objResult, 2);
	objResult->onShow(0);
	//
	for (Math_Answer* ans : _answers) {
		ans->onShowInParent(_panel_bot, 2);
	}
	onShowBottomBar();
    showQuestionBox();
}

void Nam_RomaNumber::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_RomaNumber::onExplanation(bool correct) {}
void Nam_RomaNumber::onComplete() {
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
void Nam_RomaNumber::onTouchedObject(cocos2d::Node* node){	
	//Math_Object* object = (Math_Object*)node;
}
void Nam_RomaNumber::onAnswerDone(Node* node, int ans) {
	Math_Answer* btn = (Math_Answer*)node;
    if (ans == result) {
        btn->onAnimationClick(true);
        this->nextQuestion();
    }
	else btn->onAnimationClick(false);
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_RomaNumber::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Nam_RomaNumber::autoTestingRun(float delaytime)
{
   
}

