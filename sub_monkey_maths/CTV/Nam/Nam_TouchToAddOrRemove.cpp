//
//  Nam_TouchToAddOrRemove.h
//  MJQuiz
//
//  Created by Tunado on 4/10/17.
//
//

#include "Nam_TouchToAddOrRemove.h"

Nam_TouchToAddOrRemove* Nam_TouchToAddOrRemove::createQuiz() {
	Nam_TouchToAddOrRemove* quiz = new Nam_TouchToAddOrRemove();

	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}
bool Nam_TouchToAddOrRemove::init() {
	return cocos2d::ui::Layout::init();
}
void Nam_TouchToAddOrRemove::initQuiz(mj::model::Game game) {

	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_number = json_game.FindMember("number")->value;
	objectNumberAddOrRemove = cocos2d::RandomHelper::random_int(json_number.FindMember("min")->value.GetInt(), json_number.FindMember("max")->value.GetInt());
	rapidjson::Value& json_result = json_game.FindMember("result")->value;
	while (1) {
		result = cocos2d::RandomHelper::random_int(json_result.FindMember("min")->value.GetInt(), json_result.FindMember("max")->value.GetInt());
		if (result%objectNumberAddOrRemove == 0) break;
	}
	if (result <= 20) groupNumber = 2;
	else groupNumber = 4;


	//lay image
	objectImage = getImageList("icons", 1, "name_1")[0];
	objectImage->used = true;
	math::resource::Image* box = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/frame.png");
	box->used = true;
	auto imageResult = Math_Pool::getInstance()->getImage();
	imageResult = this->getNumbers(result, result)[0];
	imageResult->used = true;
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = imageResult->getTextValue("name_1");
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = imageResult->getTextValue("name_n");
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "name_n") {
			if (result < 2) {
				std::vector<math::resource::Text> texts;
				math::resource::Text t = objectImage->getTextValue("name_1");
				texts.push_back(t);
				getQuestion()->setOptionalTexts(key, texts);
			}
			else {
				std::vector<math::resource::Text> texts;
				math::resource::Text t = objectImage->getTextValue(key);
				texts.push_back(t);
				getQuestion()->setOptionalTexts(key, texts);
			}
		}
		else {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = objectImage->getTextValue(key);
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	//test
	// FIX GAME CHUNG
	//Gen group
	Size objectSize = Size(65, 65);
	for (int i = 0; i < groupNumber; i++) {
		Math_Group* group = Math_Pool::getInstance()->getGroup();
		group->setEnable(0, _objects, math::group::frame, 10);
		group->setObjectSize(objectSize);
		group->setGroupFrames(5, 2, box, NULL);
		group->setAlignObjects(math::group::GroupAlign::left);
		this->addChildNode(group, 2);
		group->onShow();
		_groups.push_back(group);
	}
    math::func::setPositionList(_groups, 2, Rect(0.1,0.1,0.8,0.5));
    for (int i = 0; i < groupNumber; i++){
        this->addChild(_groups[i]);
        _groups[i]->onShow();
    }
	// tạo vector các object được gen sẵn là object_gen_truoc; _objects sẽ là vector các object đang trong group
	for (int i = 0; i < groupNumber * 10; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		obj->setEnable(i, math::common::TouchPermission::deny, objectImage, NULL);
		obj->setObjectSize(objectSize);
		obj->setPosition(Vec2(0, -200));
		this->addChildNode(obj, 2);
		obj->onShow(0);
		objects_gen_truoc.push_back(obj);
	}
	//Gen add and remove
	add = Math_Pool::getInstance()->getObject();
	this->genAddAndRemove(add, objectImage, objectNumberAddOrRemove, NULL);
	add->setTouchCallback(CC_CALLBACK_1(Nam_TouchToAddOrRemove::addObjectToGroup, this));
	add->setPosition(Vec2(quiz_size.width*0.48, 70));
	_panel_bot->addChild(add,2);
}
void Nam_TouchToAddOrRemove::onStart() {
	//Gen question
	onShowBottomBar(0,0.2,true, CC_CALLBACK_1(Nam_TouchToAddOrRemove::onAnswerDone, this), CC_CALLBACK_1(Nam_TouchToAddOrRemove::removeObject, this));
	showQuestionBox();
}
void Nam_TouchToAddOrRemove::startQuestion() {
	// bắt đầu câu hỏi
	enableControlQuiz();
	this->scheduleUpdate();
}

void Nam_TouchToAddOrRemove::removeObject(cocos2d::Node* node) {
	Math_Object* obj = (Math_Object*)node;
	int total = 0;
	for (Math_Group* group : _groups) {
		total += group->getNumberObjects();
	}
	if (total > 0) {
		obj->runAction(Sequence::create(ScaleTo::create(0.2f, 1.3f), ScaleTo::create(0.2f, 1.f), NULL));
		for (int i = 0; i < objectNumberAddOrRemove; i++) {
			for (int j = _groups.size() - 1; j >= 0; j--) {
				if (_groups[j]->getNumberObjects() > 0) {
					Math_Object* obj = _groups[j]->popBackObject();
					_objects.pop_back();
					objects_gen_truoc.push_back(obj);
					break;
				}
			}
		}
	}
}
void  Nam_TouchToAddOrRemove::addObjectToGroup(cocos2d::Node* node) {
	Math_Object* obj = (Math_Object*)node;
	int total = 0;
	for (Math_Group* group : _groups) {
		total += group->getNumberObjects();
	}
	if (total < groupNumber * 10) {
		obj->runAction(Sequence::create(ScaleTo::create(0.2f, 1.3f), ScaleTo::create(0.2f, 1.f), NULL));
		for (int i = 0; i < objectNumberAddOrRemove; i++) {
			for (Math_Group* group : _groups) {
				if (group->getNumberObjects() < 10) {
					if (group->pushBackObject(objects_gen_truoc.back())) {
						_objects.push_back(objects_gen_truoc.back());
						objects_gen_truoc.pop_back();
						break;
					}
				}
			}
		}
	}
}
void Nam_TouchToAddOrRemove::genAddAndRemove(Math_Object* obj, math::resource::Image* img, int num, math::resource::Image* background) {
	obj->setEnable(0, math::common::TouchPermission::touch, img, background);
	obj->setObjectSize(Size(250, 110));
	obj->getImage()->setContentSize(Size(90, 90));
	obj->getImage()->setPosition(Vec2(50, 45));
	obj->setValueText(cocos2d::StringUtils::toString(num));
	obj->getLabelValue()->setVisible(true);
	obj->getLabelValue()->setPosition(Vec2(150, 45));
	obj->getLabelValue()->setFontSize(60.f);
    obj->getLabelValue()->setTextColor(Color4B::WHITE);
    obj->getLabelValue()->disableEffect();
}

void Nam_TouchToAddOrRemove::onAnswerDone(cocos2d::Node* node) {
	Math_Answer* btn = (Math_Answer*)(node);
	int total = 0;
	for (Math_Group* group : _groups) {
		total += group->getNumberObjects();
	}
	if (total == result) {
		add->setLocked(true);
        btn->onAnimationClick(true);
		this->nextQuestion();
	}
	else btn->onAnimationClick(false);
}

void Nam_TouchToAddOrRemove::onExplanation(bool correct) {
	CCLOG("%s override", __func__);
}
void Nam_TouchToAddOrRemove::onComplete() {
	CCLOG("%s override", __func__);
	this->unscheduleUpdate();
	//TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Object* obj : _objects) {
		obj->onHide(m_delay += 0.01);
	}
	for (Math_Object* obj : objects_gen_truoc) {
		obj->onHide(m_delay);
	}
	for (Math_Group* group : _groups) {
		group->setDisable();
	}
	add->onHide(m_delay);
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
void Nam_TouchToAddOrRemove::update(float dt) {
	for (Math_Group* group : _groups) {
		group->updatePosition(false, false);
	}
}
#define auto_testing_delay 2
void Nam_TouchToAddOrRemove::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}
void Nam_TouchToAddOrRemove::autoTestingRun(float delaytime) {

}

