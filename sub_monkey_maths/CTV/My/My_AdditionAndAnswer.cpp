#include "My_AdditionAndAnswer.h"

My_AdditionAndAnswer* My_AdditionAndAnswer::createQuiz() {
	My_AdditionAndAnswer* quiz = new My_AdditionAndAnswer();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_AdditionAndAnswer::init() {
	return cocos2d::ui::Layout::init();
}

void My_AdditionAndAnswer::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	std::vector<std::string> game_st;
	rapidjson::Value& json_style = json_game.FindMember("game_style")->value;
	for (rapidjson::SizeType i = 0; i < json_style.Size(); i++) {
		game_st.push_back(json_style[i].GetString());
	}
	int vt = cocos2d::random(0, (int)game_st.size() - 1);
	game_style = game_st.at(vt);
	number_display = json_game.FindMember("number_display")->value.GetString();
	operator_display = json_game.FindMember("operator_display")->value.GetString();
	math_display = json_game.FindMember("math_display")->value.GetBool();
	icon_display = json_game.FindMember("icon_display")->value.GetBool();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	number_object = cocos2d::random(json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	operand_1 = cocos2d::random(0, number_object - 1);
	operand_2 = number_object - operand_1;
	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();

	//Dragger Config
	rapidjson::Value& json_dragger = json_data.FindMember("drager")->value;
	number_dragger = cocos2d::random(json_dragger.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_dragger.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	dragger_size.width = json_dragger.FindMember("size")->value.FindMember("width")->value.GetDouble();
	dragger_size.height = json_dragger.FindMember("size")->value.FindMember("height")->value.GetDouble();
	if (game_style.compare("style_2") == 0) {
		int temp = cocos2d::random(0, 1);
		if (temp == 0) {
			drag_number = true;
		}
		else drag_number = false;
	}

	//Background Config
	bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
	drag_bg = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));

	//Lấy hình ảnh cho object
	auto bg_list_2 = getImageList("icons");
	int ran = cocos2d::random(0, (int)bg_list_2.size() - 1);
	icon = bg_list_2.at(ran);

	//Tạo các đáp án lựa chọn
	answer_values.push_back(number_object);
	while (answer_values.size() < 4) {
		int ran = cocos2d::random(1, 10);
		bool done = true;
		for (int i = 0; i < answer_values.size(); i++) {
			if (answer_values.at(i) == ran) {
				done = false;
				break;
			}
		}
		if (done) answer_values.push_back(ran);
	}
	std::random_shuffle(answer_values.begin(), answer_values.end());


	//Tạo các giá trị kéo thả
	if (game_style.compare("style_2.1") == 0) {
		drag_values.push_back(operand_1);
		drag_values.push_back(operand_2);
	}
	else if (game_style.compare("style_2.2") == 0) {
		drag_values.push_back(number_object);
	}
	else {
		drag_values.push_back(operand_1);
		drag_values.push_back(operand_2);
		drag_values.push_back(number_object);
	}
	while (drag_values.size() < 4) {
		bool ok = true;
		int ran_1 = cocos2d::random(1, 10);
		for (int i = 0; i < drag_values.size(); i++) {
			if (drag_values.at(i) == ran_1) {
				ok = false;
				break;
			}
		}
		if (ok) drag_values.push_back(ran_1);
	}

	//Lấy hình ảnh kéo thả
	auto imgs = getNumbers(0, 20);
	for (int i = 0; i < drag_values.size(); i++) {
		for (int j = 0; j < imgs.size(); j++) {
			if (imgs[j]->getComparisionValue("number") == drag_values.at(i)) {
				drag_imgs.push_back(imgs[j]);
				break;
			}
		}
	}

	//Tạo các đáp án chọn cho game chọn đáp án theo mô tả	
	int temp;
	do {
		temp = cocos2d::random(4, 10);
	} while (temp == number_object);
	while (answer_value_1.size() < 3) {
		std::pair<int, int> p;
		bool ok = true;
		int x = cocos2d::random(1, temp - 1);
		for (int i = 0; i < answer_value_1.size(); i++) {
			if (answer_value_1.at(i).first == x) {
				ok = false;
				break;
			}
		}
		if (ok) {
			p.first = x;
			p.second = temp - p.first;
			answer_value_1.push_back(p);
		}
	}
	std::pair<int, int> pa;
	pa.first = operand_1;
	pa.second = operand_2;
	answer_value_1.push_back(pa);
	std::random_shuffle(answer_value_1.begin(), answer_value_1.end());

	if (game_style.compare("style_1") == 0) {
		for (int i = 0; i < answer_values.size(); i++) {
			if (answer_values.at(i) == number_object) {
				answer_pos = i;
				break;
			}
		}
	}
	else if (game_style.compare("style_3") == 0) {
		for (int i = 0; i < answer_value_1.size(); i++) {
			if (answer_value_1.at(i).first + answer_value_1.at(i).second == number_object) {
				answer_pos = i;
				break;
			}
		}
	}

	//Question Config
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			auto img_txt = icon->getTextValue("name_n", m_lang->getLanguage());
			//img_txt.text = cocos2d::StringUtils::toString(icon->getComparisionValue(key));
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
}

void My_AdditionAndAnswer::onStart() {
	if (game_style.compare("style_1") != 0 && game_style.compare("style_3") != 0) {
		onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_AdditionAndAnswer::onTouchButtonDone, this));
	}
	else {
		onShowBottomBar(0.5, 0.25);
	}

	//Gen Object (Phép toán)
	int obj_id_1 = 0;
	int obj_id_2 = 0;
	int obj_id_3 = 0;
	for (int i = 0; i < 5; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id_1++, math::common::deny, bg, NULL);
		object->setObjectSize(object_size);
		if (i == 0) object->setValueText(cocos2d::StringUtils::toString(operand_1));
		else if (i == 1) {
			object->setValueText("+");
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		else if (i == 2) object->setValueText(cocos2d::StringUtils::toString(operand_2));
		else if (i == 3) {
			object->setValueText("=");
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		else if (i == 4) object->setValueText(cocos2d::StringUtils::toString(number_object));
		object->setImageVisible(false);
		object->setBackgroundVisible(false);
		object->getLabelValue()->setFontSize(60);
		object->showValueTextWithAnimation(true);
		_object_1.push_back(object);
	}

	//Gen Object (Hình ảnh)
	if (operand_1 != 0) {
		for (int i = 0; i <= number_object; i++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id_1++, math::common::deny, icon, bg);
			object->setObjectSize(object_size);
			object->setBackgroundVisible(false);
			if (i == operand_1) object->setImageVisible(false);
			_object_2.push_back(object);
		}
	}
	else {
		for (int i = 0; i < number_object; i++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id_1++, math::common::deny, icon, bg);
			object->setObjectSize(object_size);
			object->setBackgroundVisible(false);
			_object_2.push_back(object);
		}
	}

	//Gen Object (Mô tả phép toán)
	for (int i = 0; i < 5; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id_3++, math::common::deny, bg, NULL);
		object->setObjectSize(object_size);
		if (i == 0) {
			object->setValueText(cocos2d::StringUtils::toString(operand_1));
		}
		else if (i == 1) {
			object->setValueText("plus");
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		else if (i == 2) object->setValueText(cocos2d::StringUtils::toString(operand_2));
		else if (i == 3) {
			object->setValueText("is");
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		else if (i == 4) object->setValueText(cocos2d::StringUtils::toString(number_object));
		object->setImageVisible(false);
		object->setBackgroundVisible(false);
		object->getLabelValue()->setFontSize(60);
		object->showValueTextWithAnimation(true);
		_object_3.push_back(object);
	}

	//Gen Dragger
	int drag_id = 0;
	if (game_style.compare("style_2") == 0 || game_style.compare("style_2.1") == 0 || game_style.compare("style_2.2") == 0) {
		for (int i = 0; i < 4; i++) {
			Math_Object* drag = Math_Pool::getInstance()->getObject();
			drag->setEnable(drag_id++, math::common::move, drag_imgs.at(i), drag_bg);
			drag->setObjectSize(object_size);
			drag->setValueText(cocos2d::StringUtils::toString(drag_values.at(i)));
			drag->showValueTextWithAnimation(true);
			drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_AdditionAndAnswer::onMoveObjectEnded, this));
			drag->setImageVisible(false);
			_draggers.push_back(drag);
		}
	}
	else if (game_style.compare("style_4") == 0) {
		for (int i = 0; i < 2; i++) {
			Math_Object* drag = Math_Pool::getInstance()->getObject();
			drag->setEnable(drag_id++, math::common::move, bg, drag_bg);
			drag->setObjectSize(object_size);
			if (i == 0) drag->setValueText("+");
			else drag->setValueText("=");
			drag->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
			drag->showValueTextWithAnimation(true);
			drag->setImageVisible(false);
			drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_AdditionAndAnswer::onMoveObjectEnded, this));
			_draggers.push_back(drag);
		}
	}

	//Gen Slots
	int slot_id = 0;
	if (game_style.compare("style_2") == 0) {
		for (int i = 0; i < 3; i++) {
			Math_Slot* slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(slot_id++, bg);
			slot->setSlotSize(object_size);
			slot->setComparisionValue(_draggers.at(i)->getComparisionValue());
			_slots.push_back(slot);
		}
	}
	else if (game_style.compare("style_2.1") == 0) {
		int x = cocos2d::random(0, 1);
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(x, bg);
		slot->setSlotSize(object_size);
		slot->setComparisionValue(_draggers.at(x)->getComparisionValue());
		_slots.push_back(slot);
	}
	else if (game_style.compare("style_2.2") == 0) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(2, bg);
		slot->setSlotSize(object_size);
		slot->setComparisionValue(_draggers.at(0)->getComparisionValue());
		_slots.push_back(slot);
	}
	else if (game_style.compare("style_4") == 0) {
		for (int i = 0; i < 2; i++) {
			Math_Slot* slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(slot_id++, bg);
			slot->setSlotSize(object_size);
			_slots.push_back(slot);
		}
	}

	std::random_shuffle(_draggers.begin(), _draggers.end());

	//Gen Answer
	if (game_style.compare("style_2") != 0 && game_style.compare("style_4") != 0) {
		for (int i = 0; i < 4; i++) {
			Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
			if (game_style.compare("style_1") == 0) {
				answer->setEnable(cocos2d::StringUtils::toString(answer_values.at(i)), math::common::answer, CC_CALLBACK_1(My_AdditionAndAnswer::onTouchAnswer, this), cocos2d::Size(200, 60));
			}
			else if (game_style.compare("style_3") == 0) {
				std::string str = cocos2d::StringUtils::toString(answer_value_1.at(i).first) + " + " + cocos2d::StringUtils::toString(answer_value_1.at(i).second) + " = " + cocos2d::StringUtils::toString(answer_value_1.at(i).first + answer_value_1.at(i).second);
				answer->setEnable(str, math::common::answer, CC_CALLBACK_1(My_AdditionAndAnswer::onTouchAnswer, this), cocos2d::Size(280, 60));
			}
			_answers.push_back(answer);
		}
	}

	if (game_style.compare("style_1") == 0) {
		//Show object
		if (math_display) {
			if (!icon_display) {
				math::func::setPositionList(_object_1, 3, cocos2d::Rect(0.3, 0.15, 0.4, 0.3));
			}
			else math::func::setPositionList(_object_1, 3, cocos2d::Rect(0.3, 0.05, 0.4, 0.2));
			for (int i = 0; i < 3; i++) {
				addChildNode(_object_1.at(i), 2);
				_object_1.at(i)->onShow(0.3);
			}
		}
		if (icon_display) {
			cocos2d::Rect pos;
			if (number_object <= 4) pos = cocos2d::Rect(0.25, 0.3, 0.5, 0.2);
			else if (number_object <= 7) pos = cocos2d::Rect(0.15, 0.3, 0.7, 0.2);
			else pos = cocos2d::Rect(0, 0.55, 1, 0.2);
			math::func::setPositionList(_object_2, number_object + 1, pos);
			for (Math_Object* obj : _object_2) {
				this->addChildNode(obj, 2);
				obj->onShow(0.3);
			}
		}

		//Show Answer
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.2));
		for (Math_Answer* answer : _answers) {
			answer->onShowInParent(_panel_bot, 2);
		}
	}
	else if (game_style.compare("style_3") == 0) {
		cocos2d::Rect pos;
		if (number_object <= 4) pos = cocos2d::Rect(0.25, 0.25, 0.5, 0.3);
		else if (number_object <= 7) pos = cocos2d::Rect(0.15, 0.25, 0.7, 0.3);
		else pos = cocos2d::Rect(0, 0.5, 1, 0.2);

		math::func::setPositionList(_object_2, number_object + 1, pos);
		math::func::setPositionList(_object_3, 5, cocos2d::Rect(0.1, 0.07, 0.8, 0.2));

		for (Math_Object* obj : _object_2) {
			obj->onShow(0.3);
			addChildNode(obj);
		}

		for (Math_Object* obj : _object_3) {
			obj->onShow(0.3);
			addChildNode(obj);
		}

		math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.2));
		for (Math_Answer* ans : _answers) {
			ans->onShowInParent(_panel_bot, 2);
		}
	}
	else if (game_style.compare("style_4") == 0) {
		math::func::setPositionList(_object_3, 5, cocos2d::Rect(0.1, 0.3, 0.8, 0.3));
		math::func::setPositionList(_object_1, 5, cocos2d::Rect(0.1, 0.1, 0.8, 0.2));
		math::func::setPositionList(_draggers, 4, cocos2d::Rect(0.05, 0, 0.7, 0.2));

		for (Math_Object* object : _object_1) {
			if (object->getID() % 2 != 0) {
				object->setValueText("");
			}
			object->onShow(0.3);
			addChildNode(object, 2);
		}

		for (Math_Object* object : _object_3) {
			object->onShow(0.3);
			addChildNode(object, 2);
		}

		for (Math_Object* drag : _draggers) {
			drag->onShow(0.3);
			addChildNodeToBottomPanel(drag, 2);
		}

		int x = 0;
		for (int i = 0; i < _object_1.size(); i++) {
			if (i % 2 == 1) {
				_slots.at(x)->setPosition(_object_1.at(i)->getPosition());
				x++;
			}
		}

		for (Math_Slot* slot : _slots) {
			addChildNode(slot, 2);
			slot->onShow(0.3);
		}
	}
	else {
		math::func::setPositionList(_object_3, 5, cocos2d::Rect(0.1, 0.3, 0.8, 0.3));
		math::func::setPositionList(_object_1, 5, cocos2d::Rect(0.1, 0.1, 0.8, 0.2));
		math::func::setPositionList(_draggers, 4, cocos2d::Rect(0.05, 0, 0.7, 0.2));

		for (Math_Object* object : _object_1) {
			object->onShow(0.3);
			addChildNode(object, 2);
		}

		for (Math_Object* object : _object_3) {
			object->onShow(0.3);
			addChildNode(object, 2);
		}

		for (Math_Object* drag : _draggers) {
			drag->onShow(0.3);
			addChildNodeToBottomPanel(drag, 2);
		}
		if (game_style.compare("style_2") == 0) {
			int x = 0;
			for (int i = 0; i < _object_1.size(); i++) {
				if (i % 2 == 0) {
					_slots.at(x)->setPosition(_object_1.at(i)->getPosition());
					x++;
				}
			}
		}
		else if (game_style.compare("style_2.1") == 0) {
			_slots.at(0)->setPosition(_object_1.at(_slots.at(0)->getId())->getPosition());
		}
		else if (game_style.compare("style_2.2") == 0) {
			_slots.at(0)->setPosition(_object_1.at(4)->getPosition());
		}

		for (Math_Slot* slot : _slots) {
			addChildNode(slot, 2);
			slot->onShow(0.3);
		}

	}

	//Tạo Câu hỏi
	showQuestionBox();
	disableControlQuiz(3);
}

void My_AdditionAndAnswer::startQuestion() {
	if (game_style.compare("style_2") == 0 || game_style.compare("style_4") == 0 || game_style.compare("style_2.1") == 0 || game_style.compare("style_2.2") == 0) {
		scheduleOnce([=](float dt) {
			for (Math_Object* drag : _draggers) {
				drag->enableDrag(_panel_bot);
			}
		}, 0.5, "complete_quiz");
	}
}

void My_AdditionAndAnswer::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		if (game_style.compare("style_2") == 0 || game_style.compare("style_2.1") == 0 || game_style.compare("style_2.2") == 0) {
			for (Math_Slot* slot : _slots) {
				if (!slot->isCorrect(math::comparision::number)) {
					slot->getLinker()->backToStart();
					slot->removeLinker();
					count--;
				}
			}
		}
		else if (game_style.compare("style_4") == 0) {
			for (Math_Slot* slot : _slots) {
				slot->getLinker()->backToStart();
				slot->removeLinker();
				count--;
			}
		}
	}
}

void My_AdditionAndAnswer::onComplete() {
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	if (game_style.compare("style_1") == 0) {
		if (math_display) {
			for (Math_Object* obj : _object_1) {
				obj->onHide(m_delay);
				m_delay += 0.1;
			}
		}
		if (icon_display) {
			m_delay = 0;
			for (Math_Object* obj : _object_2) {
				obj->onHide(m_delay);
				m_delay += 0.1;
			}
		}
	}
	else if (game_style.compare("style_3") == 0) {
		float dt = 0;
		for (Math_Object* obj : _object_3) {
			obj->onHide(dt);
			dt += 0.1;
		}
		dt = 0;
		for (Math_Object* obj : _object_2) {
			obj->onHide(dt);
			dt += 0.1;
		}
	}
	else {
		float dt = 0;
		for (Math_Object* obj : _object_1) {
			obj->onHide(dt);
			dt += 0.1;
		}

		dt = 0;
		for (Math_Object* obj : _object_3) {
			obj->onHide(dt);
			dt += 0.1;
		}

		dt = 0;
		for (Math_Object* drag : _draggers) {
			drag->onHide(dt);
			dt += 0.1;
		}

		for (Math_Slot* slot : _slots) {
			slot->onHide(0);
		}
	}

	for (Math_Answer* ans : _answers) {
		ans->setDisable();
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_AdditionAndAnswer::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	bool done = false;
	if (answer == _answers.at(answer_pos)) {
		done = true;
		onExplanation(done);
	}
	answer->onAnimationClick(done);
}

void My_AdditionAndAnswer::onTouchButtonDone(cocos2d::Node* node) {
	clickable = true;
	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed()) {
			clickable = false;
		}
	}

	Math_Answer* done = (Math_Answer*)node;

	if (clickable) {
		disableControlQuiz(3);
		clickable = false;
		if (game_style.compare("style_2") == 0 || game_style.compare("style_2.1") == 0 || game_style.compare("style_2.2") == 0) {
			bool right = true;
			for (Math_Slot* slot : _slots) {
				if (!slot->isCorrect(math::comparision::number)) {
					right = false;
				}
			}
			onExplanation(right);
			done->onAnimationClick(right);
		}
		else if (game_style.compare("style_4") == 0) {
			bool right = true;
			if (_slots.at(0)->getLinker()->getValueText().compare("+") != 0) {
				right = false;
			}
			onExplanation(right);
			done->onAnimationClick(right);
		}
	}
}

void My_AdditionAndAnswer::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point) {
	Math_Object* dragger = (Math_Object*)node;
	cocos2d::Point pos = _panel->convertToNodeSpace(point);

	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(pos)) {
			if (slot->isUsed()) {
				slot->getLinker()->backToStart();
				slot->removeLinker();
			}
			slot->setLinker(dragger);
			dragger->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(slot->getPosition())));
			count++;
			return;
		}
	}
	dragger->backToStart();
}
