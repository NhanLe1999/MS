#include "My_NumberBone.h"

My_NumberBone* My_NumberBone::createQuiz() {
	My_NumberBone* quiz = new My_NumberBone();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

void My_NumberBone::setPositionGroups(float radius, cocos2d::Vec2 center_pos, std::vector<Math_Slot*> slots) {
	float PI = 3.14;
	int vt = 0;
	float x = 360 / (int)slots.size();
	for (float i = 0; i <= 360; i += x) {
		cocos2d::Vec2 pos;
		pos.x = std::sin(i*PI / 180)*radius + center_pos.x;
		pos.y = std::cos(i*PI / 180)*radius + center_pos.y;
		slots.at(vt++)->setPosition(pos);
	}
}

bool My_NumberBone::init() {
	return cocos2d::ui::Layout::init();
}

void My_NumberBone::initQuiz(mj::model::Game game) {
	_game = game;
	_game_path = math_statistic->getGamePath(_game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Group Config
	rapidjson::Value& json_group = json_data.FindMember("group")->value;
	number_group = cocos2d::random(json_group.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_group.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	group_size.width = json_group.FindMember("size")->value.FindMember("width")->value.GetDouble();
	group_size.height = json_group.FindMember("size")->value.FindMember("height")->value.GetDouble();

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	tong = cocos2d::random(json_game.FindMember("tong")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("tong")->value.FindMember("maximum")->value.GetInt());
	rapidjson::Value& json_style = json_game.FindMember("game_style")->value;
	std::vector<std::string> game_st;
	for (rapidjson::SizeType i = 0; i < json_style.Size(); i++) {
		game_st.push_back(json_style[i].GetString());
	}
	int rand = cocos2d::random(0, (int)game_st.size() - 1);
	game_style = game_st.at(rand);

	while (values.size() < number_group) {
		std::pair<int, int> pa;
		int pos = cocos2d::random(0, 1);
		int value = cocos2d::random(0, tong);
		bool add = false;
		for (int i = 0; i < values.size(); i++) {
			if (values.at(i).first == value) {
				add = true;
				break;
			}
		}
		if (!add) {
			int temp = tong - value;
			dragger_values.push_back(temp);
			pa.first = value;
			pa.second = pos;
			values.push_back(pa);
		}
	}

	//Dragger Config
	rapidjson::Value& json_dragger = json_data.FindMember("drager")->value;
	number_dragger = cocos2d::random(json_dragger.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_dragger.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	dragger_size.width = json_dragger.FindMember("size")->value.FindMember("width")->value.GetDouble();
	dragger_size.height = json_dragger.FindMember("size")->value.FindMember("height")->value.GetDouble();

	//Slot Config
	rapidjson::Value& json_slot = json_data.FindMember("slot")->value;
	number_slot = cocos2d::random(json_slot.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_slot.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	slot_size.width = json_slot.FindMember("size")->value.FindMember("width")->value.GetDouble();
	slot_size.height = json_slot.FindMember("size")->value.FindMember("height")->value.GetDouble();
	if (game_style.compare("style_1") == 0) {
		slot_size = cocos2d::Size(80, 80);
	}

	//Lấy hình ảnh icon
	auto imgs = getImageList("icons");
	int vt = cocos2d::random(0, (int)imgs.size() - 1);
	icon = imgs.at(vt);

	//Lấy Background cho object và group
	bg_1 = objectBG();
	bg_2 = slotBG();

	//Lấy hình ảnh (chữ số) cho các dragger
	auto drag_number = getNumbers(0, 20);
	for (int i = 0; i < dragger_values.size(); i++) {
		for (int j = 0; j < drag_number.size(); j++) {
			if (drag_number.at(j)->getComparisionValue("number") == dragger_values.at(i)) {
				numbers.push_back(drag_number.at(j));
				break;
			}
		}
	}
	if (game_style.compare("style_2") == 0) {
		tong = cocos2d::random(2, 12);
	}
	int x, y;
	do {
		if (tong > 6) x = cocos2d::random(1, 6);
		else x = cocos2d::random(1, tong - 1);
		y = tong - x;
	} while (x > 6 || y > 6);
	group_value.push_back(tong);
	if (x > y) {
		group_value.push_back(y);
		group_value.push_back(x);
	}
	else {
		group_value.push_back(x);
		group_value.push_back(y);
	}
	while (group_value.size() < 4) {
		int ran = cocos2d::random(1, 10);
		bool add = true;
		for (int i = 0; i < group_value.size(); i++) {
			if (group_value.at(i) == ran) {
				add = false;
				break;
			}
		}
		if (add) group_value.push_back(ran);
	}

	for (int i = 0; i < group_value.size(); i++) {
		for (int j = 0; j < drag_number.size(); j++) {
			if (drag_number.at(j)->getComparisionValue("number") == group_value.at(i)) {
				drag_imgs.push_back(drag_number.at(j));
				break;
			}
		}
	}

	//Lấy resource cho game
	auto resources = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sresource.json", _game_path.c_str()));
	resource_1 = math::resource::ResHelper::createImage(getUIPath("gameui/numberbond/1.png"));
	resource_2 = math::resource::ResHelper::createImage(getUIPath("gameui/numberbond/2.png"));

	//Question Config
	auto img_number_list = getNumbers(tong, tong);
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : img_number_list) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : img_number_list) {
				auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
}

void My_NumberBone::onStart() {
	if (game_style.compare("style_1") == 0) {
		onShowBottomBar(0.25, 0.25, true, CC_CALLBACK_1(My_NumberBone::onTouchAnswer, this));
	}
	else {
		onShowRightBar(0.5, 0.2, CC_CALLBACK_1(My_NumberBone::onTouchAnswer, this));
	}
	object_center.x = quiz_size.width / 2;
	object_center.y = quiz_size.height / 2 - quiz_size.height / 4.5;

	//Tạo Object chứa tổng các phép toán
	object_tong = Math_Pool::getInstance()->getObject();
	object_tong->setEnable(0, math::common::deny, bg_1, NULL);
	object_tong->setImageVisible(false);
	object_tong->setObjectSize(slot_size);
	object_tong->setValueText(cocos2d::StringUtils::toString(tong));
	object_tong->getLabelValue()->setFontSize(60);
	object_tong->showValueTextWithAnimation(true);
	object_tong->setPosition(object_center);

	for (int i = 0; i < 4; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(0, math::common::deny, resource_1, NULL);
		object->setObjectSize(cocos2d::Size(300, 100));
		object_resources.push_back(object);
	}

	int obj_id = 0;
	for (int i = 0; i < 8; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id, math::common::deny, bg_1, NULL);
		object->setObjectSize(slot_size);
		if (i >= 4) {
			object->setValueText("+");
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		else object->setValueText(cocos2d::StringUtils::toString(values.at(i).first));
		object->setImageVisible(false);
		object->getLabelValue()->setFontSize(60);
		object->onShow(0);
		object->showValueTextWithAnimation(true);
		_objects.push_back(object);
	}

	//Tạo object (game Domino)
	obj_id = 0;
	for (int i = 0; i < group_value.at(1); i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id, math::common::deny, icon, NULL);
		object->setObjectSize(slot_size*0.3);
		_objects_1.push_back(object);
	}

	obj_id = 0;
	for (int i = 0; i < group_value.at(2); i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id, math::common::deny, icon, NULL);
		object->setObjectSize(slot_size*0.3);
		_objects_2.push_back(object);
	}

	domino = Domino::createDomino();
	domino->setDomino(group_value.at(2), group_value.at(1), 200, true);
	domino->setSize(cocos2d::Size(2590, 600));
	domino->setRotation(90);
	domino->setPosition(cocos2d::Vec2((quiz_size.width - quiz_size.width / 5) / 2, quiz_size.height / 4));

	//Gen Dragger
	int drag_id = 0;
	if (game_style.compare("style_1") == 0) {
		for (int i = 0; i < number_group; i++) {
			Math_Object* drag = Math_Pool::getInstance()->getObject();
			drag->setEnable(drag_id++, math::common::move, numbers.at(i), NULL);
			drag->setBackgroundVisible(false);
			drag->setImageVisible(false);
			drag->setRotation(0);
			drag->setObjectSize(slot_size);
			drag->setValueText(cocos2d::StringUtils::toString(tong - values.at(i).first));
			drag->getLabelValue()->setFontSize(60);
			drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_NumberBone::onMoveObjectEnded, this));
			drag->showValueTextWithAnimation(true);
			_draggers.push_back(drag);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			Math_Object* drag = Math_Pool::getInstance()->getObject();
			drag->setEnable(drag_id++, math::common::move, drag_imgs.at(i), NULL);
			drag->setBackgroundVisible(false);
			drag->setImageVisible(false);
			drag->setObjectSize(slot_size);
			drag->setRotation(0);
			drag->setValueText(cocos2d::StringUtils::toString(group_value.at(i)));
			drag->getLabelValue()->setFontSize(60);
			drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_NumberBone::onMoveObjectEnded, this));
			drag->showValueTextWithAnimation(true);
			_draggers.push_back(drag);
		}
	}

	//Gen Slot
	int slot_id = 0;
	int x;
	if (game_style.compare("style_1") == 0) {
		x = number_group;
	}
	else {
		x = 3;
	}
	for (int i = 0; i < x; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(slot_id, bg_2);
		slot->setSlotSize(slot_size);
		if (game_style.compare("style_1") == 0) {
			slot->setBackgroundVisible(false);
		}
		slot->setComparisionValue(_draggers.at(i)->getComparisionValue());
		_slots.push_back(slot);
	}
	std::random_shuffle(_draggers.begin(), _draggers.end());

	obj_re_1 = Math_Pool::getInstance()->getObject();
	obj_re_1->setEnable(0, math::common::deny, resource_2, NULL);
	obj_re_1->setObjectSize(cocos2d::Size(350, 250));

	//Show Groups
	float delay = 0;
	if (game_style.compare("style_1") == 0) {
		obj_re_1->setPosition(cocos2d::Point(object_center.x, object_center.y));
		obj_re_1->onShow(0.3);
		addChildNode(obj_re_1, 2);

		object_tong->onShow(0.3);
		this->addChildNode(object_tong, 2);
		for (int i = 0; i < 4; i++) {
			int x, y;
			if (i % 2 == 0) {
				x = -1;
			}
			else x = 1;
			if (i < 2) y = 1;
			else y = -1;
			_objects.at(i + 4)->setPosition(cocos2d::Vec2(object_center.x + x * 270, object_center.y + y * 130));
			object_resources.at(i)->setPosition(_objects.at(i + 4)->getPosition());

			if (values.at(i).second == 0) {
				_objects.at(i)->setPosition(cocos2d::Vec2(_objects.at(i + 4)->getPositionX() - slot_size.width - 20, _objects.at(i + 4)->getPosition().y));
			}
			else {
				_objects.at(i)->setPosition(cocos2d::Vec2(_objects.at(i + 4)->getPositionX() + slot_size.width + 20, _objects.at(i + 4)->getPosition().y));
			}

			addChildNode(object_resources.at(i), 2);
			addChildNode(_objects.at(i), 2);
			addChildNode(_objects.at(i + 4), 2);

			object_resources.at(i)->onShow(0);
		}
	}
	else {
		addChildNode(domino, 2);
	}

	//Show dragger
	delay = 0;
	if (game_style.compare("style_2") == 0) {
		math::func::setPositionList(_draggers, 1, cocos2d::Rect(0, 0.2, 0.2, 0.75));
		for (Math_Object* drag : _draggers) {
			this->addChildNodeToRightPanel(drag, 2);
			drag->onShow(delay);
			delay += 0.3;
		}
	}
	else {
		math::func::setPositionList(_draggers, number_group, cocos2d::Rect(0.05, 0, 0.7, 0.2));
		for (Math_Object* drag : _draggers) {
			this->addChildNodeToBottomPanel(drag, 2);
			drag->onShow(delay);
			delay += 0.3;
		}
	}

	//Show Slot
	if (game_style.compare("style_1") == 0) {
		for (int i = 0; i < number_group; i++) {
			this->addChildNode(_slots.at(i), 2);
			_slots.at(i)->onShow(delay);
			if (values.at(i).second == 0) {
				_slots.at(i)->setPosition(cocos2d::Vec2(_objects.at(i + 4)->getPosition().x + slot_size.width + 20, _objects.at(i + 4)->getPosition().y));
			}
			else {
				_slots.at(i)->setPosition(cocos2d::Vec2(_objects.at(i + 4)->getPosition().x - slot_size.width - 20, _objects.at(i + 4)->getPosition().y));
			}
		}
		delay += 0.3;
	}
	else {
		_slots.at(0)->setPosition(cocos2d::Vec2((quiz_size.width - quiz_size.width / 5) / 2, quiz_size.height / 2 + quiz_size.height / 5));
		_slots.at(1)->setPosition(cocos2d::Vec2((quiz_size.width - quiz_size.width / 5) / 4, quiz_size.height / 5 * 2));
		_slots.at(2)->setPosition(cocos2d::Vec2((quiz_size.width - quiz_size.width / 5) / 4 * 3, quiz_size.height / 5 * 2));
		for (Math_Slot* slot : _slots) {
			slot->onShow(0);
			addChildNode(slot, 2);
		}
	}

	//Line
	if (game_style.compare("style_2") == 0) {
		float line_x = std::sqrt((_slots[0]->getPositionY() - _slots[1]->getPositionY()) * (_slots[0]->getPositionY() - _slots[1]->getPositionY()) + (_slots[0]->getPosition().x - _slots[1]->getPosition().x) * (_slots[0]->getPosition().x - _slots[1]->getPosition().x));
		cocos2d::Vec2 line_pos_1 = cocos2d::Vec2((_slots[0]->getPositionX() - slot_size.width / 2 + _slots[1]->getPositionX() - slot_size.width / 2) / 2 + 5, (_slots[0]->getPositionY() + _slots[1]->getPositionY()) / 2);
		cocos2d::Vec2 line_pos_2 = cocos2d::Vec2((_slots[0]->getPositionX() + slot_size.width / 2 + _slots[2]->getPositionX() + slot_size.width / 2) / 2 - 5, (_slots[0]->getPositionY() + _slots[2]->getPositionY()) / 2);
		auto img_line_1 = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
		img_line_1->setScale9Enabled(true);
		auto line_render = img_line_1->getVirtualRendererSize();
		img_line_1->setRotation(-45);
		img_line_1->setContentSize(cocos2d::Size(line_x, line_render.height));
		img_line_1->setPosition(line_pos_1);
		addChildNode(img_line_1);

		auto img_line_2 = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
		img_line_2->setScale9Enabled(true);
		img_line_2->setRotation(45);
		img_line_2->setContentSize(cocos2d::Size(line_x, line_render.height));
		img_line_2->setPosition(line_pos_2);
		addChildNode(img_line_2);

		ui_image.push_back(img_line_1);
		ui_image.push_back(img_line_2);
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_NumberBone::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point) {
	Math_Object* dragger = (Math_Object*)node;
	cocos2d::Point pos = _panel->convertToNodeSpace(point);
	int vt = -1;
	for (int i = 0; i < _slots.size(); i++) {
		if (_slots.at(i)->getLinker()->isSame(dragger)) {
			_slots.at(i)->removeLinker();
			for (int j = 0; j < _slots.size(); j++) {
				if (_slots.at(j)->isAvailable(pos) && _slots.at(j)->getLinker() != NULL && _slots.at(j)->getLinker() != dragger) {
					_slots.at(j)->getLinker()->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(i)->getPosition())));
					_slots.at(i)->setLinker(_slots.at(j)->getLinker());
					_slots.at(j)->removeLinker();
					dragger->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(j)->getPosition())));
					_slots.at(j)->setLinker(dragger);
					return;
				}
			}
		}
	}
	for (int i = 0; i < _slots.size(); i++) {
		if (_slots.at(i)->isAvailable(pos)) {
			if (_slots.at(i)->getLinker()->isSame(dragger)) {
				dragger->backToLast(2);
				return;
			}
			if (_slots.at(i)->getLinker() != NULL && !_slots.at(i)->getLinker()->isSame(dragger)) {
				_slots.at(i)->getLinker()->backToStart();
				_slots.at(i)->removeLinker();
				count--;
			}
			dragger->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(i)->getPosition())));
			_slots.at(i)->setLinker(dragger);
			count++;

			bool ok = true;
			for (Math_Slot* slot : _slots) {
				if (!slot->isUsed()) {
					ok = false;
				}
			}

			if (ok) clickable = true;
			return;
		}
	}

	dragger->backToStart();
}

void My_NumberBone::startQuestion() {
	scheduleOnce([=](float dt) {
		if (game_style.compare("style_1") == 0) {
			for (Math_Object* drag : _draggers) {
				drag->enableDrag(_panel_bot);
			}
		}
		else {
			for (Math_Object* drag : _draggers) {
				drag->enableDrag(_panel_right);
			}
		}
	}, 0.5, "Enable drag!");
}

void My_NumberBone::onExplanation(bool correct) {
	if (correct) {
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=]() {
			this->nextQuestion();
		}), NULL));
	}
	else {
		for (Math_Slot* slot : _slots) {
			if (!slot->isCorrect(math::comparision::number)) {
				slot->getLinker()->backToStart();
				slot->removeLinker();
				count--;
			}
		}
	}
}

void My_NumberBone::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	delay = 0;
	if (game_style.compare("style_1") == 0) {
		obj_re_1->onHide(0.3);

		for (Math_Object* object : _objects) {
			object->onHide(0.3);
		}

		for (Math_Object* object : object_resources) {
			object->onHide(delay);
		}
	} 
	else {
		for (cocos2d::ui::ImageView* im : ui_image) {
			im->removeFromParent();
		}
	}
	
	delay = 0;
	for (Math_Slot* slot : _slots) {
		slot->getLinker()->onHide(0);
		slot->onHide(0.3);
	}

	for (Math_Object* drag : _draggers) {
		drag->onHide(delay);
		delay += 0.3;
	}

	if (game_style.compare("style_1") == 0) object_tong->onHide(0);

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, 2, "complete_quiz");
}

void My_NumberBone::onTouchAnswer(cocos2d::Node* node) {
	if (clickable) {
		clickable = false;
		bool done = true;
		if (game_style.compare("style_1") == 0) {
			for (Math_Slot* slot : _slots) {
				if (!slot->isCorrect(math::comparision::number)) {
					done = false;
					break;
				}
			}
		}
		else {
			for (Math_Slot* slot : _slots) {
				if (!slot->isCorrect(math::comparision::number)) {
					done = false;
					break;
				}
			}
		}
		onExplanation(done);
	}
}
