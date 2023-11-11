#include "My_MultiAddition.h"

My_MultiAddition* My_MultiAddition::createQuiz() {
	My_MultiAddition *quiz = new My_MultiAddition();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_MultiAddition::init() {
	return cocos2d::ui::Layout::init();
}

void My_MultiAddition::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_style = json_game.FindMember("game_style")->value;
	std::vector<std::string> game_st;
	for (rapidjson::SizeType i = 0; i < json_style.Size(); i++) {
		game_st.push_back(json_style[i].GetString());
	}
	int rand = cocos2d::random(0, (int)game_st.size() - 1);
	game_style = game_st.at(rand);

	rapidjson::Value& json_limit = json_game.FindMember("number")->value;
	min = json_limit.FindMember("minimum")->value.GetInt();
	max = json_limit.FindMember("maximum")->value.GetInt();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	rapidjson::Value& json_number = json_object.FindMember("number")->value;
	number_object = cocos2d::random(json_number.FindMember("minimum")->value.GetInt(), json_number.FindMember("maximum")->value.GetInt());
	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetInt();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetInt();

	//Lấy số mở đầu cho chuỗi
	if (game_style.compare("addition") == 0) {
		//Phép cộng
		start_number = cocos2d::random(min, min + 2);
	}
	else if (game_style.compare("subtraction") == 0) {
		//Phép trừ
		start_number = cocos2d::random(max - 2, max);
	}
	else {
		//Trường hợp cả phép cộng và trừ
		start_number = cocos2d::random(min, max);
	}

	result_numbers.push_back(start_number);

	//Lấy dữ liệu phép toán
	int temp = start_number;
	while (game_data.size() < number_object - 1) {
		if (game_style.compare("addition") == 0) {
			//Trường hợp chuỗi phép cộng
			std::pair<int, std::string> pair;
			pair.second = "+";
			int x = cocos2d::random(1, 3);
			if (temp + x >= max) {
				x = cocos2d::random(0, 1);
			}
			if (temp == max) {
				x = 0;
			}
			pair.first = x;
			game_data.push_back(pair);
			temp += x;
			result_numbers.push_back(temp);
		}
		else if (game_style.compare("subtraction") == 0) {
			//Trường hợp chuỗi phép trừ
			std::pair<int, std::string> pair;
			pair.second = "-";
			int x = cocos2d::random(1, 3);
			if (temp - x < min + 1) {
				x = cocos2d::random(0, 1);
			}
			if (temp == min) {
				x = 0;
			}
			pair.first = x;
			game_data.push_back(pair);
			temp -= x;
			result_numbers.push_back(temp);
		}
		else {
			//Trường hợp cả phép cộng và trừ (game_style = "mix")
			std::pair<int, std::string> pair;
			int s = cocos2d::random(0, 1);
			if (s == 0) {
				pair.second = "+";
			}
			else {
				pair.second = "-";
			}
			int x = cocos2d::random(1, 3);
			if (pair.second.compare("+") == 0) {
				if (temp + x > max) {
					pair.second = "-";
					temp -= x;
				}
				else {
					temp += x;
				}
			}
			else if (pair.second.compare("-") == 0) {
				if (temp - x < min) {
					pair.second = "+";
					temp += x;
				}
				else {
					temp -= x;
				}
			}
			pair.first = x;
			game_data.push_back(pair);
			result_numbers.push_back(temp);
		}
	}

	//Number_slot
	if (number_object > 4) {
		number_dragger = number_object - 1;
	}
	else {
		number_dragger = 4;
	}

	//Tạo các giá trị kéo thả
	for (int i = 0; i < result_numbers.size() - 1; i++) {
		dragger_values.push_back(result_numbers[i + 1]);
	}
	while (dragger_values.size() < number_dragger) {
		int x = cocos2d::random(min, max);
		bool ok = true;
		for (int j = 0; j < dragger_values.size(); j++) {
			if (dragger_values.at(j) == x) {
				ok = false;
			}
		}
		if (ok) {
			dragger_values.push_back(x);
		}
	}
	std::random_shuffle(dragger_values.begin(), dragger_values.end());

	//Lấy hình ảnh hiển thị object
	auto numbers = getNumbers(0, max);
	for (int i = 0; i < result_numbers.size(); i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == result_numbers.at(i)) {
				result_imgs.push_back(numbers.at(j));
				break;
			}
		}
	}

	for (int i = 0; i < number_object - 1; i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == game_data.at(i).first) {
				operand_imgs.push_back(numbers.at(j));
				break;
			}
		}
	}

	//Lấy hình ảnh cho dragger
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < numbers.size(); k++) {
			if (numbers.at(k)->getComparisionValue("number") == dragger_values.at(i)) {
				dragger_imgs.push_back(numbers.at(k));
				break;
			}
		}
	}

	//Lấy background cho object
	background = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
	drag_bg = objectBG();

	//Question Config
	parseQuestion();

}

void My_MultiAddition::onStart() {
	onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_MultiAddition::onTouchAnswer, this));

	//Tạo Object
	int id_1 = 0;
	for (int i = 0; i < number_object; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(id_1++, math::common::deny, result_imgs.at(i), NULL);
		object->setObjectSize(object_size);
		if (i == 0) {
			object->setEnable(id_1++, math::common::deny, result_imgs.at(i), background);
			object->setValueText(cocos2d::StringUtils::toString(result_numbers.at(i)));
			object->showValueTextWithAnimation(true);
		}
		object->setImageVisible(false);
		_objects_1.push_back(object);
	}

	int id_2 = 0;
	for (int i = 0; i < number_object - 1; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(id_2++, math::common::deny, operand_imgs.at(i), NULL);
		object->setValueText(game_data.at(i).second + cocos2d::StringUtils::toString(game_data.at(i).first));
		object->showValueTextWithAnimation(true);
		object->setObjectSize(object_size);
		object->setImageVisible(false);
		object->getLabelValue()->setFontSize(30);
		_objects_2.push_back(object);
	}

	//Tạo Slot
	int slot_id = 0;
	for (int i = 0; i < number_object - 1; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(slot_id++, background);
		slot->setComparisionValue(_objects_1.at(i + 1)->getComparisionValue());
		slot->setSlotSize(object_size);
		_slots.push_back(slot);
	}

	//Tạo Dragger
	int drag_id = 0;
	for (int i = 0; i < number_dragger; i++) {
		std::vector<Math_Object*> drags;
		Math_Object* dragger = Math_Pool::getInstance()->getObject();
		dragger->setEnable(drag_id++, math::common::touch, dragger_imgs.at(i), drag_bg);
		dragger->setObjectSize(object_size);
		dragger->setValueText(cocos2d::StringUtils::toString(dragger_values.at(i)));
		dragger->showValueTextWithAnimation(true);
		dragger->setImageVisible(false);
		dragger->setMoveCallback(nullptr, CC_CALLBACK_2(My_MultiAddition::onMoveObjectEnded, this));
		_draggers.push_back(dragger);
	}

	//Show Object
	math::func::setPositionList(_objects_1, number_object, cocos2d::Rect(0, 0.15, 1, 0.3));

	float delay_1 = 0;
	float delay_2 = 0.1;
	for (Math_Object* object : _objects_1) {
		object->onShow(delay_1);
		addChildNode(object, 2);
	}

	for (int i = 0; i < _objects_2.size(); i++) {
		_objects_2.at(i)->setPosition(cocos2d::Point((_objects_1.at(i)->getPosition().x + _objects_1.at(i + 1)->getPosition().x) / 2, _objects_1.at(i)->getPosition().y + 60));
		_objects_2.at(i)->onShow(delay_2);
		delay_2 += 0.2;
		addChildNode(_objects_2.at(i), 2);
	}

	//Show Slots
	for (int i = 0; i < _slots.size(); i++) {
		delay_1 += 0.2;
		_slots.at(i)->setPosition(_objects_1.at(i + 1)->getPosition());
		_slots.at(i)->onShow(delay_1);
		addChildNode(_slots.at(i), 2);
	}

	//Show Dragger
	math::func::setPositionList(_draggers, 4, cocos2d::Rect(0.1, 0, 0.6, 0.2));
	for (Math_Object* dragger : _draggers) {
		dragger->onShow(delay_1);
		delay_1 += 0.2;
		addChildNodeToBottomPanel(dragger, 2);
	}	

	//Curve arrows
	float delta_line = _objects_1[1]->getPosition().x - _objects_1[0]->getPosition().x;
	for (int i = 0; i < number_object - 1; i++) {
		auto curve_arrow = cocos2d::ui::ImageView::create(getUIPath("common/line/curve.png"));
		curve_arrow->setFlippedX(true);
		curve_arrow->setContentSize(cocos2d::Size(delta_line, object_size.height / 3));
		curve_arrow->setPosition(cocos2d::Vec2(_objects_1[i]->getPosition().x + delta_line / 2, (_objects_1[i]->getPosition().y + 30)));
		ui_images.push_back(curve_arrow);
		addChildNode(curve_arrow);
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_MultiAddition::startQuestion() {
	scheduleOnce([=](float dt) {
		for (Math_Object* dragger : _draggers) {
			dragger->enableDrag(_panel_bot);
		}
	}, 0.5, "complete_quiz");	
}

void My_MultiAddition::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		for (int i = 0; i < _slots.size(); i++) {
			if (!_slots[i]->isCorrect(math::comparision::number)) {
				for (int j = i; j < _slots.size(); j++) {
					_slots[j]->getLinker()->backToStart();
					_slots[j]->removeLinker();
				}
				return;
			}
		}
		
	}
}

void My_MultiAddition::onComplete() {
	float delay = showConratulation(0.3);

	for (cocos2d::ui::ImageView* im : ui_images) {
		im->removeFromParentAndCleanup(true);
	}

	for (Math_Object* object : _objects_1) {
		object->onHide(delay);
	}

	for (Math_Object* object : _objects_2) {
		object->onHide(delay);
	}

	for (int i = 0; i < number_object - 1; i++) {
		for (Math_Object* object : _draggers) {
			object->onHide(1.5);
		}
	}

	for (Math_Slot* slot : _slots) {
		slot->onHide(delay);
		delay += 0.2;
	}

	for (Math_Answer* answer : _answers) {
		answer->setDisable();
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_MultiAddition::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object* object = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	int index = 0;

	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed()) {
			index = slot->getId();
		}
	}

	for (Math_Slot* slot : _slots) {
		if (slot->getLinker()->isSame(object)) {
			for (Math_Slot* t_slot : _slots) {
				if (t_slot->isAvailable(pos) && slot != t_slot) {
					Math_Slot* n_slot = t_slot;
					slot->removeLinker();
					slot->setLinker(t_slot->getLinker());
					t_slot->getLinker()->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(slot->getPosition())));
					t_slot->removeLinker();
					object->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(t_slot->getPosition())));
					t_slot->setLinker(object);
					return;
				}
			}
			object->backToLast(2);
			return;
		}
	}

	for (int i = 0; i <= index; i++) {
		if (_slots.at(i)->isAvailable(pos)) {
			if (_slots.at(i)->isUsed()) {
				_slots.at(i)->getLinker()->backToStart();
				_slots.at(i)->removeLinker();
			}
			object->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(i)->getPosition())));
			_slots.at(i)->setLinker(object);
			return;
		}
	}

	object->backToStart();
}

void My_MultiAddition::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	clickable = true;
	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed()) {
			clickable = false;
		}
	}
	if (clickable) {
		clickable = false;
		bool pass = true;

		for (Math_Slot* slot : _slots) {
			if (!slot->isCorrect(math::comparision::number)) {
				pass = false;
				break;
			}
		}

		onExplanation(pass);
		answer->onAnimationClick(pass);
	}
}
