#include "My_DragInOrder.h"

My_DragInOrder* My_DragInOrder::createQuiz() {
	My_DragInOrder *quiz = new My_DragInOrder();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_DragInOrder::init() {
	return cocos2d::ui::Layout::init();
}

void My_DragInOrder::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Order Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;

	order = json_game.FindMember("order")->value.GetString();
	count_style = json_game.FindMember("countstyle")->value.GetString();

	int max_number = json_game.FindMember("max_number")->value.GetInt();
	number_object = cocos2d::random(json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	min = cocos2d::random(json_game.FindMember("start_number")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("start_number")->value.FindMember("maximum")->value.GetInt());
	while (number_object + min > max_number + 1) {
		number_object = cocos2d::random(json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt());
		min = cocos2d::random(json_game.FindMember("start_number")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("start_number")->value.FindMember("maximum")->value.GetInt());
	}
	distance = json_game.FindMember("distance")->value.GetInt();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;

	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();
	value = json_object.FindMember("value")->value.FindMember("type")->value.GetString();
	background = json_object.FindMember("background")->value.GetString();

	//Lưu hình ảnh hiện thị trên object
	auto imgs = getNumbers(min, min + number_object * distance);
	for (int i = min; i <= min + (number_object - 1) * distance; i += distance) {
		for (math::resource::Image* img : imgs) {
			if (img->getComparisionValue("number") == i) {
				object_icons.push_back(img);
			}
		}
	}

	if (order.compare("asc") != 0) {
		std::reverse(object_icons.begin(), object_icons.end());
	}

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	if (count_style.compare("asc") == 0) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}

	//Lấy backgeound cho slots
	slot_bg = objectBG();
	dot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));

	//TODO: GENERATE QUIZ COMPONENTS
	//Tạo Object
	float max_length = 0;
	math::common::TouchPermission touch_per = math::common::touch;
	int obj_id = 0;
	for (int i = 0; i < number_object; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, touch_per, object_icons.at(i), slot_bg);
		object->setObjectSize(object_size);
		object->getImage()->setContentSize(object_size);
		object->setMoveCallback(CC_CALLBACK_1(My_DragInOrder::onTouchedObject, this), CC_CALLBACK_2(My_DragInOrder::onMoveObjectEnded, this));
		if (value.compare("number") == 0) {
			object->setValueTextByKey("number");
			object->setImageVisible(false);
			object->showValueTextWithAnimation(false);
			object->getLabelValue()->setFontSize(object_size.height*0.75);
		}
		else if (value.compare("name") == 0) {
			object->setValueTextByKey("name");
			object->setImageVisible(false);
			object->showValueTextWithAnimation(false);
			object->getLabelValue()->setFontSize(object_size.height*0.4);
		}
		else if (value.compare("name_n") == 0) {
			object->setValueTextByKey("name_n");
			object->setImageVisible(false);
			object->showValueTextWithAnimation(false);
			object->getLabelValue()->setFontSize(object_size.height*0.4);
		}
		if (object->getLabelValue()->getContentSize().width > max_length) {
			max_length = object->getLabelValue()->getContentSize().width;
		}
		_objects.push_back(object);
	}

	if (max_length > object_size.width*0.9) {
		auto font_size = _objects.front()->getLabelValue()->getFontSize();
		font_size = font_size*object_size.width*0.9 / max_length;
		for (Math_Object* object : _objects) {
			object->getLabelValue()->setFontSize(font_size);
		}
	}


	//Tạo Slots
	for (int i = 0; i < number_object; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(i, NULL);
		slot->setSlotSize(object_size);
		slot->setBackgroundVisible(false);
		int index_obj = i;
		if (count_style.compare("asc") != 0) {
			index_obj = number_object - i - 1;
		}
		slot->setComparisionValue(_objects.at(index_obj)->getComparisionValue());
		_slots.push_back(slot);
	}

	//Tạo dấu nhắc
	dot = Math_Pool::getInstance()->getObject();
	dot->setEnable(0, math::common::TouchPermission::deny, object_icons.front(), dot_bg);
	dot->setObjectSize(object_size);
	//dot->setValueText("?");
	dot->setImageVisible(false);
	//dot->showValueTextWithAnimation(false);
	//dot->getLabelValue()->setFontSize(object_size.height*.75);

}
void My_DragInOrder::onStart() {
	math_statistic->setCurrentGame(_game);
	onShowBottomBar(0, 0.4, true);

	//Show objects
	float delay = 0;
	float delta_x = 50;
	number_object = (int)_objects.size();
	int number_in_line = number_object;
	int number_row = 1;

	float objs_width = number_in_line*(object_size.width + delta_x) - delta_x;
	while (objs_width > 900) {
		number_in_line = (number_object + number_row) / (number_row + 1);
		objs_width = number_in_line*(object_size.width + delta_x) - delta_x;
		number_row++;
	}

	math::func::setPositionList(_objects, number_in_line, cocos2d::Rect(0, 0, 1, 0.36));
	for (Math_Object* object : _objects) {
		this->addChildNodeToBottomPanel(object, 2);
		object->onShow(delay);
		delay += 0.1;
	}

	//Show slots
	math::func::setPositionList(_slots, number_in_line, cocos2d::Rect(0, 0, 1, 0.45));
	for (Math_Slot* slot : _slots) {
		this->addChildNode(slot, 2);
		slot->onShow(0);
	}

	//Show Question
	showQuestionBox();
}

void My_DragInOrder::readNumber(int index) {
	if (index < number_object) {
		playObjectAudio(_objects[index]);
		_objects[index]->runAction(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.45, 1.2), cocos2d::ScaleTo::create(0.45, 1), NULL));

		scheduleOnce([=](float) {
			this->readNumber(index + 1);
		}, 1.0, cocos2d::StringUtils::format("read_number_%d", index));
	}
	else {
		auto rand_list = randomPositionsInRect(object_size, cocos2d::Rect(50, 0, quiz_size.width - 100, quiz_size.height / 2.7), number_object);
		for (int i = 0; i < _objects.size(); i++) {
			Math_Object* object = _objects[i];
			object->runAction(cocos2d::Spawn::createWithTwoActions(cocos2d::MoveTo::create(0.25, rand_list[i]), cocos2d::RotateTo::create(0.25, cocos2d::random(-0, 0))));
		}

		scheduleOnce([=](float) {
			int i_start = 0;
			vt = 0;
			if (count_style.compare("asc") != 0) {
				i_start = number_object - 1;
			}
			_slots[vt]->setLinker(_objects[i_start]);
			_objects[i_start]->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5), cocos2d::Spawn::createWithTwoActions(cocos2d::MoveTo::create(0.25, _panel->convertToWorldSpace(_slots[vt]->getPosition())), cocos2d::RotateTo::create(0.25, 0))));
			_objects[i_start]->setLocked(true);

			vt++;
		}, 0.5, "start");

		scheduleOnce([=](float) {
			enableControlQuiz();
			for (Math_Object* object : _objects) {
				object->enableDrag(_panel_bot);
			}
			dot->setPosition(_slots.at(vt)->getPosition());
			this->addChildNode(dot, 2);
			dot->onShow(0);
			/*dot->setSelectedAnimation(math::common::blink);
			dot->setSelected(true);*/

		}, 1.5, "start game");
	}
}

void My_DragInOrder::startQuestion() {
	readNumber(0);
}

void My_DragInOrder::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
	object->runAction(cocos2d::RotateTo::create(0.2, 0));
}

void My_DragInOrder::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object* object = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	Math_Slot* m_slot = _slots[vt];
	int index_obj = vt;
	if (count_style.compare("asc") != 0) {
		index_obj = number_object - vt - 1;
	}
	Math_Object* m_object = _objects[index_obj];

	if (m_object->isSame(object)) {
		if (m_slot->isAvailable(pos)) {
			//move done
			m_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.1, _panel->convertToWorldSpace(m_slot->getPosition())));
			object->setLocked(true);

			vt++;
			if (vt < number_object) {
				dot->runAction(cocos2d::MoveTo::create(0.2, _slots.at(vt)->getPosition()));
			}
			else {
				dot->onHide(0);
				//complete
				scheduleOnce([=](float) {
					this->onExplanation(true);
				}, 0.5, "complete");
			}
			return;
		}
	}

	object->backToStart();
}

void My_DragInOrder::onComplete() {
	float delay = showConratulation(0.3);
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	m_delay = delay - 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.1;
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_DragInOrder::onExplanation(bool correct) {
	if (correct) this->nextQuestion();
}

void My_DragInOrder::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, 12, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void My_DragInOrder::autoTestingRun(float delaytime) {
	if (count_style.compare("asc") == 0) {
		if (count + 1 == number_object) {
			this->nextQuestion();
			return;
		}

		int temp = test.at(test.size() - 1);
		test.pop_back();

		if (count + 2 == number_object) {
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::CallFunc::create([=]() {
				dot->setVisible(false);
			}), NULL));
		}
		int x = 0;
		for (int i = 1; i < _objects.size(); i++) {
			if (_objects.at(i)->getImageSource()->getComparisionValue("number") == temp) {
				x = i;
				break;
			}
		}
		_objects.at(x)->runAction(cocos2d::MoveTo::create(0.8, _slots.at(count + 1)->getPosition()));

		if (x == count + 1) {
			if (count + 2 < 10) {
				dot->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::MoveTo::create(0.2, _slots.at(count + 2)->getPosition()), NULL));
			}
			_objects.at(x)->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::RotateTo::create(0.1, 0), NULL));
			count++;
			test.clear();
			for (int i = count + 1; i < number_object; i++) {
				test.push_back(_objects.at(i)->getImageSource()->getComparisionValue("number"));
				std::random_shuffle(test.begin(), test.end());
			}
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
		else {
			_objects.at(x)->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.9), cocos2d::MoveTo::create(0.8, object_position.at(x).first), NULL));
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, 2 * delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
	}
	else {
		if (count + 1 == number_object) {
			this->nextQuestion();
			return;
		}

		int temp = test.at(test.size() - 1);
		test.pop_back();

		if (count + 2 == number_object) {
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::CallFunc::create([=]() {
				dot->setVisible(false);
			}), NULL));
		}
		int x = 0;
		for (int i = 0; i < _objects.size(); i++) {
			if (_objects.at(i)->getImageSource()->getComparisionValue("number") == temp) {
				x = i;
				break;
			}
		}
		_objects.at(x)->runAction(cocos2d::MoveTo::create(0.8, _slots.at(vt)->getPosition()));

		if (x == vt) {
			if (vt > 0) {
				dot->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::MoveTo::create(0.2, _slots.at(vt - 1)->getPosition()), NULL));
			}
			_objects.at(x)->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::RotateTo::create(0.1, 0), NULL));
			count++;
			vt--;
			test.clear();
			for (int i = 0; i <= vt; i++) {
				test.push_back(_objects.at(i)->getImageSource()->getComparisionValue("number"));
				std::random_shuffle(test.begin(), test.end());
			}
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
		else {
			_objects.at(x)->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.9), cocos2d::MoveTo::create(0.8, object_position.at(x).first), NULL));
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, 2 * delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
	}
}

