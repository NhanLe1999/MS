//
//  Nam_WhichCardEqual
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_WhichCardEqual.h"

Nam_WhichCardEqual* Nam_WhichCardEqual::createQuiz()
{
    Nam_WhichCardEqual* quiz=new Nam_WhichCardEqual();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_WhichCardEqual::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_WhichCardEqual::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	min_result = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	max_result = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
	result = RandomHelper::random_int(min_result, max_result);
	cardCount = RandomHelper::random_int(json_game.FindMember("card_count")->value.FindMember("min")->value.GetInt(), json_game.FindMember("card_count")->value.FindMember("max")->value.GetInt());
	// get Image
	img_result = this->getNumbers(result, result)[0];
	img_result->used = true;
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = img_result->getTextValue("name_1", m_lang->getLanguage());
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	// RANDOM phep tinh
	std::vector <std::string> str_dapandung;
	std::vector <std::string> str_answers;
	for (int i = 0; i <= result; i++) {
		str_dapandung.push_back(cocos2d::StringUtils::toString(i) + " + " + cocos2d::StringUtils::toString(result - i));
	}
	std::random_shuffle(str_dapandung.begin(), str_dapandung.end());
    int so_dap_an_dung = RandomHelper::random_int(1, std::max(2,cardCount - 2));
	for (int i = 0; i < so_dap_an_dung; i++) {
		str_answers.push_back(str_dapandung[i]);
	}
	for (int i = 0; i < cardCount - so_dap_an_dung; i++) {
		while (1) {
			int a = RandomHelper::random_int(0, max_result);
			int r = RandomHelper::random_int(a, max_result);
			if (r == result) {
				continue;
			}
			std::string str = cocos2d::StringUtils::toString(a) + " + " + cocos2d::StringUtils::toString(r-a) ;
			bool kt = true;
			for (std::string s : str_answers) {
				if (s == str) {
					kt = false;
					break;
				}
			}
			if (kt) {
				str_answers.push_back(str);
				break;
			}
		}
    }
	bg_sentence = math::resource::ResHelper::createImage(getUIPath("common/answer2.png"));
    bg_sentence->used = true;
    bg_sentence_choosed = math::resource::ResHelper::createImage(getUIPath("common/answer.png"));
    bg_sentence_choosed->used = true;
	// TODO: GEN CAC DOI TUONG
	for (int i = 0; i < cardCount; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		if (i < so_dap_an_dung) {
			obj->setEnable(1, math::common::TouchPermission::touch, bg_sentence, NULL);
        }
		else {
			obj->setEnable(0, math::common::TouchPermission::touch, bg_sentence, NULL);
		}
        obj->setImageVisible(false);
        obj->setBackground(bg_sentence,0.4);
		obj->setName("0");
		obj->setObjectSize(Size(400, 120));
		obj->setValueText(str_answers[i]);
		obj->getLabelValue()->setVisible(true);
		obj->getLabelValue()->setFontSize(60);
		obj->setTouchCallback(CC_CALLBACK_1(Nam_WhichCardEqual::onTouchedObject, this));
		_objects.push_back(obj);
	}
	std::random_shuffle(_objects.begin(), _objects.end());
	math::func::setPositionList(_objects, 2, Rect(0, 0.1, 1, 0.6));
	
	//Math_Pool::getInstance()->resetImages();
	
}

void Nam_WhichCardEqual::onStart() {
	math_statistic->setCurrentGame(_game);
	
	for (Math_Object* obj : _objects) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
	for (Math_Answer* ans : _answers) {
		ans->onShowInParent(_panel, 2);
	}
	//Gen question

    showQuestionBox();
}

void Nam_WhichCardEqual::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	}
}
void Nam_WhichCardEqual::onExplanation(bool correct) {}
void Nam_WhichCardEqual::onComplete() {
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
void Nam_WhichCardEqual::onTouchedObject(cocos2d::Node* node){
	Math_Object* object = (Math_Object*)node;
	if (object->getName() == "0") {
        object->setBackground(bg_sentence_choosed,0.4);
		object->setName("1");
	}
	else {
		object->setBackground(bg_sentence,0.4);
		object->setName("0");
	}
	bool done = true;
	for (Math_Object* obj : _objects) {
		if (obj->getName() == "1" && obj->getID() == 0) {
			done = false;
			break;
		}
		if (obj->getName() == "0" && obj->getID() == 1) {
			done = false;
			break;
		}
	}
	if (done) {
		for (Math_Object* obj : _objects) {
			obj->setLocked(true);
			if (obj->getID() == 1) {
				obj->getBackground()->runAction(Sequence::create(TintTo::create(0.25, Color3B::GREEN), TintTo::create(0.25, Color3B::WHITE), TintTo::create(0.25, Color3B::GREEN), TintTo::create(0.25, Color3B::WHITE), TintTo::create(0.25, Color3B::GREEN), TintTo::create(0.25, Color3B::WHITE),NULL));
			}
		}
		this->nextQuestion();
	}
}
void Nam_WhichCardEqual::onAnswerDone(Node* node, std::string str) {
	Math_Answer* btn = (Math_Answer*)node;	
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_WhichCardEqual::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("test-%d", testing_step++));
}

void Nam_WhichCardEqual::autoTestingRun(float delaytime)
{
   
}

