#include "My_VennDiagram.h"

My_VennDiagram* My_VennDiagram::createQuiz() {
	My_VennDiagram* quiz = new My_VennDiagram();

	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_VennDiagram::init() {
	return cocos2d::ui::Layout::init();
}

void My_VennDiagram::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	rapidjson::Value& number = json_object.FindMember("number")->value;

	number_object_1 = cocos2d::random(number.FindMember("minimum")->value.GetInt(), number.FindMember("maximum")->value.GetInt());
	number_object_2 = cocos2d::random(number.FindMember("minimum")->value.GetInt(), number.FindMember("maximum")->value.GetInt());
	number_same_1 = cocos2d::random(1, number_object_1 / 2);
	number_same_2 = cocos2d::random(1, number_object_2 / 2);
	number_object = number_object_1 + number_object_2;

	prop_1 = getJsonString(json_object, "prop_1");
	prop_2 = getJsonString(json_object, "prop_2");

	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();

	//Lấy hình ảnh hiển thị
	auto icon = getImageList("icons");
	std::vector<math::resource::Image*> temp_1;
	std::vector<math::resource::Image*> temp_2;

	auto random_1 = math::func::getRandomNode(icon);
	auto random_2 = math::func::getRandomNode(icon);
	for (math::resource::Image* im : icon) {
		if (im->getText(prop_1).compare(random_1->getText(prop_1)) == 0) {
			temp_1.push_back(im);
		}

		if (im->getText(prop_2).compare(random_2->getText(prop_2)) == 0) {
			temp_2.push_back(im);
		}
	}

	//Lấy hình ảnh các icons giống nhau
	std::vector<math::resource::Image*> same_temp_1;
	std::vector<math::resource::Image*> same_temp_2;
	for (math::resource::Image* im : temp_1) {
		if (im->getText(prop_2).compare(temp_2[0]->getText(prop_2)) == 0) {
			same_temp_1.push_back(im);
		}
	}

	for (math::resource::Image* im : temp_2) {
		if (im->getText(prop_1).compare(temp_1[0]->getText(prop_1)) == 0) {
			same_temp_2.push_back(im);
		}
	}

	while (icon_same_1.size() < number_same_1) {
		for (math::resource::Image* im : same_temp_1) {
			icon_same_1.push_back(im);
			if (icon_same_1.size() == number_same_1) {
				break;
			}
		}
		auto x = math::func::getRandomNode(same_temp_1);
		icon_same_1.push_back(x);
	}

	while (icon_same_2.size() < number_same_2) {
		for (math::resource::Image* im : same_temp_2) {
			icon_same_2.push_back(im);
			if (icon_same_2.size() == number_same_2) {
				break;
			}
		}
		auto x = math::func::getRandomNode(same_temp_2);
		icon_same_2.push_back(x);
	}

	//Lấy hình ảnh object 1
	for (math::resource::Image* im : temp_1) {
		if (im->getText(prop_1).compare(icon_same_1[0]->getText(prop_1)) != 0) {
			icon_1.push_back(im);
		}
		if (icon_1.size() == number_object_1) {
			break;
		}
	}
	if (icon_1.size() < number_object_1) {
		while (icon_1.size() < number_object_1) {
			int ran = cocos2d::random(0, (int)temp_1.size() - 1);
			if (temp_1[ran]->getText(prop_2).compare(temp_2[0]->getText(prop_2)) == 0) {
				continue;
			}
			icon_1.push_back(temp_1[ran]);
		}
	}

	//Lấy hình ảnh cho object 2
	for (math::resource::Image* im : temp_2) {
		if (im->getText(prop_2).compare(same_temp_2[0]->getText(prop_2)) != 0) {
			icon_2.push_back(im);
		}
		if (icon_2.size() == number_object_2) {
			break;
		}
	}
	if (icon_2.size() < number_object_2) {
		while (icon_2.size() < number_object_2) {
			int ran = cocos2d::random(0, (int)temp_2.size() - 1);
			if (temp_2[ran]->getText(prop_1).compare(temp_1[0]->getText(prop_1)) == 0) {
				continue;
			}
			icon_2.push_back(temp_2[ran]);
		}
	}

	//Lấy hình nền cho group
	group_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png"));

	//Lấy hình ảnh biểu đồ
	venn_left = math::resource::ResHelper::createImage(getUIPath("gameui/venn/venn_left.png"));
	venn_right = math::resource::ResHelper::createImage(getUIPath("gameui/venn/venn_right.png"));
	venn_mid = math::resource::ResHelper::createImage(getUIPath("gameui/venn/venn_middle.png"));
	venn = math::resource::ResHelper::createImage(getUIPath("gameui/venn_diagram.png"));
	venn_imgs.push_back(venn_left);
	venn_imgs.push_back(venn_mid);
	venn_imgs.push_back(venn_right);

	//Question Config
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			auto img_txt = icon_same_1[0]->getTextValue("name_n", m_lang->getLanguage());
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "color") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : icon_same_1) {
				auto img_txt = img->getTextValue("color", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}

		else if (key == "name_1") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : icon_same_1) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
}

void My_VennDiagram::onStart() {
	onShowBottomBar(0.25, 0.25, true, CC_CALLBACK_1(My_VennDiagram::onAnswerDone, this));

	//Gen Objects
	int obj_id = 0;
	for (int i = 0; i < number_object_1 - number_same_1; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::touch, icon_1.at(i), NULL);
		object->setObjectSize(object_size);
		_objects.push_back(object);
	}

	for (int i = 0; i < number_object_2 - number_same_2; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::touch, icon_2.at(i), NULL);
		object->setObjectSize(object_size);
		_objects.push_back(object);
	}


	for (int i = 0; i < number_same_1; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::touch, icon_same_1[i], NULL);
		object->setObjectSize(object_size);
		_objects.push_back(object);
	}

	for (int i = 0; i < number_same_2; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::touch, icon_same_2[i], NULL);
		object->setObjectSize(object_size);
		_objects.push_back(object);
	}

	for (Math_Object* object : _objects) {
		object->setBackgroundVisible(false);
		object->setMoveCallback(nullptr, CC_CALLBACK_2(My_VennDiagram::onMoveObjectEnded, this));
	}

	std::random_shuffle(_objects.begin(), _objects.end());

	//Gen diagram
	venn_diagram = mpool->getObject();
	venn_diagram->setEnable(0, math::common::deny, venn, NULL);
	venn_diagram->setBackgroundVisible(false);
	float yy = quiz_size.height - quiz_size.height / 4 - getQuestion()->getContentSize().height - 40;
	float pos_y = quiz_size.height * 0.75 / 2 - getQuestion()->getContentSize().height / 2 -20;
	venn_diagram->setObjectSize(cocos2d::Size(1.7 * yy, yy));
	venn_diagram->setPosition(cocos2d::Vec2(quiz_size.width / 2, pos_y));
	addChildNode(venn_diagram, 2);

	//Gen Groups
	for (int i = 0; i < 3; i++) {
		std::vector<Math_Object*> _group_list;
		Math_Group* group = mpool->getGroup();
		group->setEnable(i, _group_list, math::group::custom, 10);
		//group->setObjectSize(object_size);
		if (i == 1) {
			group->setGroupSize(cocos2d::Size(2 * yy / 2.5 * 0.9, yy * 0.9));
		}
		else group->setGroupSize(cocos2d::Size(yy * 0.75, yy));
		group->setBackground(venn_imgs.at(i));
		group->setCheckTouchImage(venn_imgs[i]);
		group->setBackgroundVisiable(false);
		_groups.push_back(group);
	}

	//Show Object
	float delay = 0;
	math::func::setPositionList(_objects, 8, cocos2d::Rect(0.05, 0, 0.7, 0.2));
	for (Math_Object* object : _objects) {
		addChildNodeToBottomPanel(object, 2);
		object->onShow(delay);
		delay += 0.2;
	}

	//Show groups
	_groups.at(1)->setPosition(cocos2d::Point(quiz_size.width / 2, pos_y));
	_groups.at(0)->setPosition(cocos2d::Point(quiz_size.width / 2 - yy / 2, pos_y));
	_groups.at(2)->setPosition(cocos2d::Point(quiz_size.width / 2 + yy / 2, pos_y));

	for (Math_Group* group : _groups) {
		group->onShow();
		addChildNode(group, 2);
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_VennDiagram::startQuestion() {
	for (Math_Object* object : _objects) {
		object->enableDrag(_panel_bot);
	}
}

void My_VennDiagram::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point) {
	Math_Object* object = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	for (Math_Group* group : _groups) {
		if (group->hasObject(object)) {
			group->removeObject(object);
			break;
		}
	}

	if (_groups.at(1)->isTouchOnGroup(pos)) {
		_groups.at(1)->addObject(object, pos);
		return;
	}
	if (_groups.at(0)->isTouchOnGroup(pos)) {
		_groups.at(0)->addObject(object, pos);
		return;
	}
	if (_groups.at(2)->isTouchOnGroup(pos)) {
		_groups.at(2)->addObject(object, pos);
		return;
	}

	object->backToStart();

}

void My_VennDiagram::onAnswerDone(cocos2d::Node* node) {
	Math_Answer* ans = (Math_Answer*)node;
	bool done = true;
	if (_groups.at(0)->getNumberObjects() + _groups.at(1)->getNumberObjects() + _groups.at(2)->getNumberObjects() == number_object) {
		clickable = true;
	}
	if (clickable) {
		clickable = false;
		bool check = true;
		for (int i = 1; i < _groups.at(1)->getNumberObjects(); i++) {
			if (_groups.at(1)->getObjectAtIndex(i)->getImageSource()->getText(prop_1).compare(_groups.at(1)->getObjectAtIndex(0)->getImageSource()->getText(prop_1)) != 0 || _groups.at(1)->getObjectAtIndex(i)->getImageSource()->getText(prop_2).compare(_groups.at(1)->getObjectAtIndex(0)->getImageSource()->getText(prop_2)) != 0) {
				check = false;
				break;
			}
		}
		if (!check || _groups.at(1)->getNumberObjects() != (number_same_1 + number_same_2)) {
			this->onExplanation(false);
			ans->onAnimationClick(false);
			return;
		}
		for (int j = 0; j < 3; j++) {
			if (j == 1) continue;
			bool ok = true;
			std::string textCompare;
			if (_groups.at(j)->getObjectAtIndex(0)->getImageSource()->getText(prop_1).compare(_groups.at(j)->getObjectAtIndex(1)->getImageSource()->getText(prop_1)) == 0) {
				textCompare = prop_1;
			}
			else {
				textCompare = prop_2;
			}
			for (int i = 1; i < _groups.at(j)->getNumberObjects(); i++) {
				Math_Object* obj = _groups.at(j)->getObjectAtIndex(i);
				if (obj->getImageSource()->getText(textCompare).compare(_groups.at(j)->getObjectAtIndex(0)->getImageSource()->getText(textCompare)) != 0) {
					done = false;
					break;
				}
			}
		}
		this->onExplanation(done);
		ans->onAnimationClick(done);
	}
}

void My_VennDiagram::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		for (Math_Group* group : _groups) {
			for (int i = 0; i < group->getNumberObjects(); i++) {
				group->getObjectAtIndex(i)->backToStart();
			}
		}
	}
}

void My_VennDiagram::onComplete() {
	float delay = showConratulation(0.3);

	for (Math_Object* object : _objects) {
		object->onHide(delay);
		delay += 0.1;
	}

	venn_diagram->onHide(delay);

	scheduleOnce([=](float dt) {
		//gro->setDisable();
		for (Math_Group* group : _groups) {
			group->setDisable();
		}
	}, delay, "hide_groups");

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
