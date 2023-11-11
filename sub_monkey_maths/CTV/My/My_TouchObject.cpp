#include "My_TouchObject.h"

My_TouchObject* My_TouchObject::createQuiz() {
	My_TouchObject* quiz = new My_TouchObject();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_TouchObject::init() {
	return cocos2d::ui::Layout::init();
}

void My_TouchObject::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	int temp = cocos2d::random(0, 1);
	if (temp == 0) {
		game_Type = "more";
	}
	else {
		game_Type = "less";
	}
	distance = cocos2d::random(1, 1);
	rapidjson::Value& json_data = document.FindMember("data")->value;
	if (number_object <= 4) {
		distance = cocos2d::random(1, 1);
	}

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	int min = json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
	int max = json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
	number_object = cocos2d::random(min, max);
	if (number_object == 10) game_Type = "less";
	if (game_Type.compare("more") == 0) {
		result = number_object + distance;
	}
	else {
		result = number_object - distance;
	}
	number_objects.push_back(result);

	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetInt();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetInt();

	//Group Config
	rapidjson::Value& json_group = json_data.FindMember("group")->value;
	number_group = cocos2d::random(json_group.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_group.FindMember("number")->value.FindMember("maximum")->value.GetInt());

	while (number_objects.size() < number_group) {
		int ran = cocos2d::random(number_object - distance - 1, number_object + distance + 1);
		if (number_object >= 9) ran = cocos2d::random(number_object - 3, number_object - 1);
		bool done = true;
		for (int i = 0; i < number_objects.size(); i++) {
			if (number_objects.at(i) == ran || ran == number_object || ran == 0) {
				done = false;
				break;
			}
		}
		if (done) number_objects.push_back(ran);
	}

	std::vector<math::resource::Image*> imgs = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicon.json", _game_path.c_str()));
	int x = cocos2d::random(0, (int)imgs.size() - 2);
	icon = imgs.at(x);
	obj_bg = imgs.at(imgs.size() - 1);

	//Random kiểu hiển thị khi đếm object (số hay chữ)
	int ran = cocos2d::random(0, 1);
	if (ran == 0) {
		display_number == false;
	}
	else display_number = true;

	//Lấy backgroud cho group
	group_bg = groupBG();

	//Question Config
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			//for (math::resource::Image* img : img_number_list) {
				auto img_txt = icon->getTextValue("name_1", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(icon->getComparisionValue(key));
				texts.push_back(img_txt);
			//}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}

	if (game_Type.compare("more") == 0) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}
}

void My_TouchObject::onStart() {
	//Tạo Objects để đếm
	int obj_id = 0;
	for (int i = 0; i < number_object; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::touch, icon, NULL);
		object->setObjectSize(object_size);
		object->setTouchCallback(CC_CALLBACK_1(My_TouchObject::onTouchObject, this));
		_objects.push_back(object);
	}

	objects_width = number_object * object_size.width;
	//Tạo object nền của các object
	object_bg = Math_Pool::getInstance()->getObject();
	object_bg->setEnable(0, math::common::touch, obj_bg, NULL);
	//object_bg->setImageVisible(false);
	object_bg->setObjectSize(cocos2d::Size(objects_width + object_size.width, object_size.height * 1.2));
	objects.push_back(object_bg);

	//Tạo các Groups
	math::group::GroupType gr_type = math::group::custom;
	for (int i = 0; i < number_group; i++) {
		obj_id = 0;
		std::vector<Math_Object*> _group_list;
		for (int j = 0; j < number_objects.at(i); j++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, icon, NULL);
			object->setObjectSize(object_size*0.75);
			//object->onShow(0);
			_group_list.push_back(object);
		}
		Math_Group* group = Math_Pool::getInstance()->getGroup();
		group->setEnable(i, _group_list, gr_type, 5);
		group->setObjectSize(object_size*0.75);
		//group->setBackground(group_bg);
		group->enableToch(CC_CALLBACK_1(My_TouchObject::onTouchGroup, this));
		group->setGroupFrames(10, 1, group_bg);
		_groups.push_back(group);
	}

	//Show các Objects
	math::func::setPositionList(_objects, number_object, cocos2d::Rect((float)(450 - objects_width / 2) / 900, 0.4, (float)objects_width / 900, 0.2));
	math::func::setPositionList(objects, 1, cocos2d::Rect(0, 0.4, 1, 0.2));
	for (Math_Object* object : _objects) {
		this->addChildNode(object, 2);
		object->onShow(0.5);
	}

	object_bg->onShow(0.4);
	addChildNode(object_bg);

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
	this->getQuestion()->onStart();
}

void My_TouchObject::startQuestion() {
	CCLOG("Starting question");

}

void My_TouchObject::onTouchObject(cocos2d::Node * node) {
	Math_Object* object = (Math_Object*)node;
	int vt = 0;
	for (int i = 0; i < number_object; i++) {
		if (_objects.at(i) == object) {
			vt = i;
			break;
		}
	}

	if (display_number) {
		object->setValueText(cocos2d::StringUtils::format("%d", number_counting + 1));
	}
	else {
		object->setValueText(cocos2d::StringUtils::format("%d", number_counting + 1));
	}
	number_counting++;
	object->showValueTextWithAnimation(true);
	object->getLabelValue()->setFontSize(50);
	object->setLocked(true);

	if (number_counting == number_object) {
		counting = false;
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=]() {
			this->onExplanation(true);
			math::func::setPositionList(_objects, number_object, cocos2d::Rect((float)(450 - objects_width / 2) / 900, 0.5, (float)objects_width / 900, 0.4), true);
			math::func::setPositionList(objects, 1, cocos2d::Rect(0, 0.5, 1, 0.4), true);
			math::func::setPositionList(_groups, 1, cocos2d::Rect(0, 0.1, 1, 0.45));
			for (Math_Group* group : _groups) {
				group->onShow();
				addChildNode(group);
			}
		}), NULL));
	}
}

void My_TouchObject::onTouchGroup(cocos2d::Node* node) {
	Math_Group* group = (Math_Group*)node;
	if (group->getNumberObjects() == result) {
		group->onTouchedAnimation(true);
		onExplanation(true);
	}
	else {
		group->onTouchedAnimation(false);
	}
}

void My_TouchObject::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
}

void My_TouchObject::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}
	scheduleOnce([=](float dt) {
		for (Math_Group* group : _groups) {
			group->setDisable();
		}
	}, 0.5, "hide_groups");

	delay = 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(delay);
		delay += 0.3;
	}

	object_bg->onHide(delay);

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}