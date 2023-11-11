#include "My_AdditionInTable.h"

My_AdditionInTable* My_AdditionInTable::createQuiz() {
	My_AdditionInTable* quiz = new My_AdditionInTable();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_AdditionInTable::init() {
	return cocos2d::ui::Layout::init();
}

void My_AdditionInTable::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	number_slot = json_game.FindMember("number_slot")->value.GetString();
	min_number = json_game.FindMember("min_number")->value.GetInt();
	max_number = json_game.FindMember("max_number")->value.GetInt();
	max_sum = json_game.FindMember("max_sum")->value.GetInt();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	object_size.width = json_object.FindMember("size")->value.FindMember("width")->value.GetInt();
	object_size.height = json_object.FindMember("size")->value.FindMember("height")->value.GetInt();

	//Lấy các giá trị cho phép cộng
	//Lấy các giá trị cho hàng ngang
	int temp_1 = cocos2d::random(min_number, max_number);
	int x = cocos2d::random(0, 1);
	int num_1;
	if (x == 0) {
		num_1 = 3;
	}
	else {
		num_1 = 4;
	}
	for (int i = 0; i < num_1; i++) {
		horizon_values.push_back(temp_1 + i);
	}
	//Lấy các giá trị cho hàng dọc
	int temp_2 = cocos2d::random(min_number, max_number);
	int y = cocos2d::random(0, 1);
	int num_2;
	if (y == 0) {
		num_2 = 3;
	}
	else {
		num_2 = 4;
	}
	for (int i = 0; i < 3; i++) {
		vertical_values.push_back(temp_2 + i);
	}

	min = horizon_values.at(0) + vertical_values.at(0);
	max = horizon_values.at(horizon_values.size() - 1) + vertical_values.at(vertical_values.size() - 1);

	numbers = getNumbers(0, horizon_values.at(horizon_values.size() - 1) + vertical_values.at(vertical_values.size() - 1));
	//Lấy hình ảnh cho các chữ số
	for (int i = 0; i < horizon_values.size(); i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == horizon_values.at(i)) {
				horizon_imgs.push_back(numbers.at(j));
				break;
			}
		}
	}
	for (int i = 0; i < vertical_values.size(); i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == vertical_values.at(i)) {
				vertical_imgs.push_back(numbers.at(j));
				break;
			}
		}
	}

	if (max_sum < (horizon_values.at(2) + vertical_values.at(0)) || max_sum >= max) {
		max_sum = cocos2d::random(horizon_values.at(2) + vertical_values.at(0), max - 1);
	}

	int temp = 0;
	for (int i = 1; i < max - max_sum + 1; i++) {
		temp += i;
	}

	number_answer = horizon_values.size() * vertical_values.size() - temp;

	//Lấy giá trị cho dragger
	for (int i = min; i <= max; i++) {
		dragger_values.push_back(i);
	}

	//Lấy hình ảnh cho dragger
	for (int i = 0; i < dragger_values.size(); i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == dragger_values.at(i)) {
				dragger_imgs.push_back(numbers.at(j));
				break;
			}
		}
	}

	//Lấy background cho slot
	bg_img = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png"));
	bg_img_1 = math::resource::ResHelper::createImage(getUIPath("common/frame/square4.png"));
	bg_img_2 = math::resource::ResHelper::createImage(getUIPath("common/frame/brown2.png"));

	//Random các vị trí trống
	if (number_slot.compare("one") == 0) {
		int x = cocos2d::random(1, number_answer - 2);
		slot_index.push_back(x);
	}
	else {
		int x = cocos2d::random(number_answer / 2 - 1, number_answer / 2 + 1);
		while (slot_index.size() < x) {
			int y = cocos2d::random(0, number_answer - 1);
			bool ok = true;
			for (int i = 0; i < slot_index.size(); i++) {
				if (y == slot_index.at(i)) {
					ok = false;
					break;
				}
			}
			if (ok) {
				slot_index.push_back(y);
			}
		}
	}

	//Question Config
	parseQuestion();
}

void My_AdditionInTable::onStart() {
	onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_AdditionInTable::onTouchButtonDone, this));
	//Tạo Object
	int id_1 = 0;
	int id_2 = 0;

	if (quiz_size.height * 0.75 - getQuestion()->getContentSize().height / 2 < (vertical_values.size() + 1)* object_size.height) {
		object_size.height = (quiz_size.height * 0.75 - getQuestion()->getContentSize().height / 2) / (vertical_values.size() + 1);
		object_size.width = object_size.height;
	}

	for (int i = 0; i <= horizon_values.size(); i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		if (i == 0) {
			object->setEnable(id_1++, math::common::deny, bg_img_1, bg_img_1);
			object->setObjectSize(object_size);
		}
		else {
			object->setEnable(id_1++, math::common::deny, horizon_imgs.at(i - 1), bg_img_1);
			object->setObjectSize(object_size);
			object->setValueText(cocos2d::StringUtils::toString(horizon_values.at(i - 1)));
			object->showValueTextWithAnimation(true);
			object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		}
		object->setImageVisible(false);
		horizon_objects.push_back(object);
	}

	for (int i = 0; i < vertical_values.size(); i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(id_2++, math::common::deny, vertical_imgs.at(i), bg_img_1);
		object->setObjectSize(object_size);
		object->setValueText(cocos2d::StringUtils::toString(vertical_values.at(i)));
		object->showValueTextWithAnimation(true);
		object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
		object->setImageVisible(false);
		vertical_objects.push_back(object);
	}


	int sum_id = 0;
	for (int i = 0; i < vertical_objects.size(); i++) {
		for (int j = 0; j < horizon_objects.size() - 1; j++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			math::resource::Image* img;
			for (int k = 0; k < numbers.size(); k++) {
				if (numbers.at(k)->getComparisionValue("number") == vertical_values.at(i) + horizon_values.at(j)) {
					img = numbers.at(k);
					break;
				}
			}
			if (vertical_values.at(i) + horizon_values.at(j) > max_sum) {
				img = bg_img;
				object->setEnable(sum_id++, math::common::touch, img, bg_img);
				object->setObjectSize(object_size);
				object->setImageVisible(false);
				empty_objects.push_back(object);
				continue;
			}
			object->setEnable(sum_id++, math::common::touch, img, bg_img);
			object->setObjectSize(object_size);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->getLabelValue()->setTextColor(cocos2d::Color4B::BLUE);
			object->showValueTextWithAnimation(true);
			object->setImageVisible(false);
			sum_objects.push_back(object);
		}
	}

	//Show Object
	float delay = 0;
	auto locations = math::func::setPositionListOnFrame(horizon_objects, horizon_objects.size(), vertical_objects.size() + 1, cocos2d::Size(object_size.width + 1.5, object_size.height + 1.5), cocos2d::Vec2(quiz_size.width / 2, quiz_size.height * 0.75 / 2 - getQuestion()->getContentSize().height / 2), 0);

	for (int i = 0; i < horizon_objects.size(); i++) {
		horizon_objects[i]->setPosition(locations[i]);
	}
	for (int i = 1; i <= vertical_objects.size(); i++) {
		vertical_objects[i - 1]->setPosition(locations[(i)* horizon_objects.size()]);
	}
	for (Math_Object* object : horizon_objects) {
		object->onShow(delay);
		delay += 0.1;
		addChildNode(object, 2);
	}
	for (Math_Object* object : vertical_objects) {
		object->onShow(delay);
		delay += 0.1;
		addChildNode(object, 2);
	}

	double x = (horizon_objects.at(1)->getPositionX() - horizon_objects.at(0)->getPositionX()) / 2;
	double y = (vertical_objects.at(0)->getPositionY() - vertical_objects.at(1)->getPositionY()) / 2;

	int index = 0;
	int empty_index = 0;
	for (int i = 0; i < vertical_objects.size(); i++) {
		for (int j = 0; j < horizon_objects.size() - 1; j++) {
			if (vertical_values.at(i) + horizon_values.at(j) > max_sum) {
				empty_objects.at(empty_index)->setPosition(cocos2d::Vec2(horizon_objects.at(j + 1)->getPositionX(), vertical_objects.at(i)->getPositionY()));
				empty_objects.at(empty_index)->onShow(delay);
				delay += 0.1;
				addChildNode(empty_objects.at(empty_index), 2);
				empty_index++;
				continue;
			}
			sum_objects.at(index)->setPosition(cocos2d::Vec2(horizon_objects.at(j + 1)->getPositionX(), vertical_objects.at(i)->getPositionY()));
			sum_objects.at(index)->onShow(delay);
			delay += 0.1;
			addChildNode(sum_objects.at(index), 2);

			if (slot_index.size() == 1) {
				if (index == slot_index.at(0)) {
					answer_values.push_back(vertical_values.at(i) + horizon_values.at(j));
					sum_objects.at(index)->setValueText("?");
					sum_objects.at(index)->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
				}
			}
			else {
				int slot_id = 0;
				for (int k = 0; k < slot_index.size(); k++) {
					if (slot_index.at(k) == index) {
						sum_objects.at(index)->setVisible(false);
						Math_Slot* slot = Math_Pool::getInstance()->getSlot();
						slot->setEndble(slot_id++, bg_img_2);
						slot->setSlotSize(cocos2d::Size(object_size.width - 1, object_size.height - 1));
						slot->setComparisionValue(sum_objects.at(index)->getComparisionValue());
						slot->setPosition(sum_objects.at(index)->getPosition());
						slot->onShow(delay);
						addChildNode(slot, 2);
						_slots.push_back(slot);
					}
				}
			}
			index++;
		}
	}

	//Tạo dragger
	int drag_id = 0;
	for (int i = 0; i < dragger_values.size(); i++) {
		Math_Object* drag = Math_Pool::getInstance()->getObject();
		drag->setEnable(drag_id++, math::common::touch, dragger_imgs.at(i), bg_img);
		drag->setObjectSize(object_size);
		drag->setValueText(cocos2d::StringUtils::toString(dragger_values.at(i)));
		drag->showValueTextWithAnimation(true);
		drag->getLabelValue()->setTextColor(cocos2d::Color4B::BLUE);
		drag->setImageVisible(false);
		drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_AdditionInTable::onMoveObjectEnded, this));
		_draggers.push_back(drag);
	}

	//Tạo các slot lưu vị trị các dragger
	int id = 0;
	for (int i = 0; i < dragger_values.size(); i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(id++, bg_img);
		slot->setBackgroundVisible(false);
		slot->setSlotSize(object_size);
		_dragger_slots.push_back(slot);
	}

	//Tạo các giá trị cho các đáp án
	while (answer_values.size() < 4) {
		int x = cocos2d::random(1, max);
		bool ok = true;
		for (int i = 0; i < answer_values.size(); i++) {
			if (answer_values.at(i) == x) {
				ok = false;
				break;
			}
		}
		if (ok) {
			answer_values.push_back(x);
		}
	}

	//Tạo các đáp án lựa chọn
	for (int i = 0; i < 4; i++) {
		Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
		ans->setEnable(cocos2d::StringUtils::toString(answer_values.at(i)), math::common::answer, CC_CALLBACK_1(My_AdditionInTable::onTouchAnswer, this), cocos2d::Size(200, 60));
		_answers.push_back(ans);
	}
	answer = _answers.at(0);
	std::random_shuffle(_answers.begin(), _answers.end());

	background = cocos2d::ui::ImageView::create(getUIPath("common/frame/square.png"));
	background->setScale9Enabled(true);
	background->setContentSize(cocos2d::Size((object_size.width + 1) * horizon_objects.size() + 12, (object_size.height + 1) * (vertical_objects.size() + 1) + 14));
	background->setPosition(cocos2d::Vec2(quiz_size.width / 2, quiz_size.height * 0.75 / 2 - getQuestion()->getContentSize().height / 2));
	_panel->addChild(background, 1);

	//Show Answer
	if (number_slot.compare("one") == 0) {
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0, 0.0, 1, 0.2));
		for (Math_Answer* ans : _answers) {
			ans->onShowInParent(_panel_bot, 2);
		}
	}
	else {
		math::func::setPositionList(_draggers, _draggers.size(), cocos2d::Rect(0.05, 0, 0.7, 0.2));
		math::func::setPositionList(_dragger_slots, _dragger_slots.size(), cocos2d::Rect(0.05, 0, 0.7, 0.2));
		for (Math_Object* drag : _draggers) {
			drag->onShow(2);
			addChildNodeToBottomPanel(drag, 2);
		}

		for (Math_Slot* slot : _dragger_slots) {
			slot->onShow(0);
			addChildNodeToBottomPanel(slot, 1);
		}
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_AdditionInTable::startQuestion() {
	CCLOG("Start Question!");

	if (number_slot.compare("one") != 0) {
		scheduleOnce([=](float dt) {
			for (Math_Object* drag : _draggers) {
				drag->enableDrag(_panel_bot);
			}
		}, 0.5, "enable drag");
	}
}

void My_AdditionInTable::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		if (number_slot.compare("one") != 0) {
			for (Math_Slot* slot : _slots) {
				if (!slot->isCorrect(math::comparision::number)) {
					slot->getLinker()->onHide(0);
					slot->removeLinker();
				}
			}
		}
	}
}

void My_AdditionInTable::onComplete() {
	float delay = showConratulation(0.3);

	background->setVisible(false);

	for (Math_Object* object : horizon_objects) {
		object->onHide(delay);
		delay += 0.1;
	}

	for (Math_Object* object : vertical_objects) {
		object->onHide(delay);
		delay += 0.1;
	}

	int empty = 0;
	int index = 0;
	for (int i = 0; i < vertical_objects.size(); i++) {
		for (int j = 0; j < horizon_objects.size() - 1; j++) {
			if (vertical_values.at(i) + horizon_values.at(j) > max_sum) {
				empty_objects.at(empty++)->onHide(delay);
				delay += 0.1;
			}
			else {
				sum_objects.at(index++)->onHide(delay);
				delay += 0.1;
			}
		}
	}

	if (number_slot.compare("one") == 0) {
		for (Math_Answer* ans : _answers) {
			ans->setDisable();
		}
	}
	else {
		//done->setDisable();

		for (Math_Object* drag : _draggers) {
			drag->onHide(delay);
			delay += 0.1;
		}

		for (Math_Slot* slot : _slots) {
			slot->onHide(delay);
			delay += 0.1;
		}
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_AdditionInTable::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* ans = (Math_Answer*)node;
	bool done = false;

	if (ans == answer) {
		done = true;
		this->onExplanation(true);
	}

	ans->onAnimationClick(done);
}

void My_AdditionInTable::onTouchButtonDone(cocos2d::Node* node) {
	Math_Answer* done = (Math_Answer*)node;
	bool ok = true;

	for (Math_Slot* slot : _slots) {
		if (!slot->isUsed()) {
			return;
		}
	}

	for (Math_Slot* slot : _slots) {
		if (!slot->isCorrect(math::comparision::number)) {
			ok = false;
		}
	}

	this->onExplanation(ok);
	done->onAnimationClick(ok);
}

void My_AdditionInTable::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point) {
	Math_Object* dragger = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	int index;
	for (int i = 0; i < _draggers.size(); i++) {
		if (dragger->isSame(_draggers.at(i))) {
			index = i;
		}
	}

	Math_Slot* d_slot = NULL;
	for (Math_Slot* slot:_slots) {
		if (slot->getLinker()->isSame(dragger)) {
			d_slot = slot;
		}
	}

	Math_Slot* t_slot = NULL;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(pos)) {
			t_slot = slot;
		}
	}

	if (d_slot != NULL) {
		d_slot->removeLinker();
		if (t_slot != NULL) {
			if (t_slot->isUsed()) {				
				d_slot->setLinker(t_slot->getLinker());
				t_slot->getLinker()->runAction(cocos2d::MoveTo::create(0.1, _panel->convertToWorldSpace(d_slot->getPosition())));
				t_slot->removeLinker();

				t_slot->setLinker(dragger);
				dragger->runAction(cocos2d::MoveTo::create(0.1, _panel->convertToWorldSpace(t_slot->getPosition())));
			}
			else {
				t_slot->setLinker(dragger);
				dragger->runAction(cocos2d::MoveTo::create(0.1, _panel->convertToWorldSpace(t_slot->getPosition())));
			}
		}
		else {
			dragger->onHide(0);
		}

		return;
	}

	Math_Object* drag = dragger->clone();
	drag->setImageVisible(false);
	drag->setValueText(dragger->getValueText());
	drag->showValueTextWithAnimation(true);
	drag->getLabelValue()->setTextColor(cocos2d::Color4B::BLUE);
	drag->setPosition(_dragger_slots.at(index)->getPosition());
	drag->setMoveCallback(nullptr, CC_CALLBACK_2(My_AdditionInTable::onMoveObjectEnded, this));
	drag->onShow(0);
	_draggers.erase(_draggers.begin() + index);
	_draggers.emplace(_draggers.begin() + index, drag);

	for (int i = 0; i < _slots.size(); i++) {
		if (_slots.at(i)->isAvailable(pos)) {
			if (_slots.at(i)->isUsed()) {
				_slots.at(i)->getLinker()->onHide(0);
				_slots.at(i)->removeLinker();
			}
			dragger->runAction(cocos2d::MoveTo::create(0.1, _panel->convertToWorldSpace(_slots.at(i)->getPosition())));
			_slots.at(i)->setLinker(dragger);

			return;
		}
	}

	dragger->onHide(0);
}
