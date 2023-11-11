#include "My_OddAndEven.h"

My_OddAndEven* My_OddAndEven::createQuiz() {
	My_OddAndEven *quiz = new My_OddAndEven();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_OddAndEven::init() {
	return cocos2d::ui::Layout::init();
}

void My_OddAndEven::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	operation_type = json_game.FindMember("operation_type")->value.GetString();
	drag = json_game.FindMember("drag")->value.GetBool();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	rapidjson::Value& json_size = json_object.FindMember("size")->value;
	object_size.width = json_size.FindMember("width")->value.GetDouble();
	object_size.height = json_size.FindMember("height")->value.GetDouble();

	//Tạo phép toán
	if (operation_type.compare("odd_only") == 0) {
		//Trường hợp cộng hai số lẻ
		do operand_1 = cocos2d::random(1, 9);
		while (operand_1 % 2 == 0);

		do {
			operand_2 = cocos2d::random(1, 10 - operand_1);
		} while (operand_2 % 2 == 0);

		answer_values.push_back(operand_1 + operand_2);
		while (answer_values.size() < 4) {
			int x = cocos2d::random(1, 10);
			bool ok = true;
			if (x % 2 != 0) {
				for (int i = 0; i < answer_values.size(); i++) {
					if (answer_values.at(i) == x) {
						ok = false;
						break;
					}
				}
			}
			else {
				ok = false;
			}
			if (ok) {
				answer_values.push_back(x);
			}
		}
	}
	else if (operation_type.compare("even_only") == 0) {
		//Cộng hai số chẵn
		do operand_1 = cocos2d::random(1, 9);
		while (operand_1 % 2 != 0);

		do operand_1 = cocos2d::random(1, 10 - operand_1);
		while (operand_1 % 2 != 0);

		answer_values.push_back(operand_1 + operand_2);
		while (answer_values.size() < 4) {
			int x = cocos2d::random(1, 10);
			bool ok = true;
			if (x % 2 != 0) {
				for (int i = 0; i < answer_values.size(); i++) {
					if (answer_values.at(i) == x) {
						ok = false;
						break;
					}
				}
			}
			else {
				ok = false;
			}
			if (ok) {
				answer_values.push_back(x);
			}
		}
	}
	else if (operation_type.compare("odd_even") == 0) {
		//Cộng hai số chẵn + lẻ
		do {
			operand_1 = cocos2d::random(1, 8);
			operand_2 = cocos2d::random(1, 10 - operand_1);
		} while ((operand_1 % 2 == 0 && operand_2 % 2 == 0) || (operand_1 % 2 != 0 && operand_2 % 2 != 0));

		answer_values.push_back(operand_1 + operand_2);
		while (answer_values.size() < 4) {
			int x = cocos2d::random(1, 10);
			bool ok = true;
			if (x % 2 == 0) {
				for (int i = 0; i < answer_values.size(); i++) {
					if (answer_values.at(i) == x) {
						ok = false;
						break;
					}
				}
			}
			else {
				ok = false;
			}
			if (ok) {
				answer_values.push_back(x);
			}
		}
	}

	//Lấy hình ảnh hiển thị cho objects
	auto numbers = getNumbers(0, 20);
	auto opera = getOperators();
	//auto opera = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%soperator.json", _game_path.c_str()));

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == operand_1) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 2) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == operand_2) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 1) {
			for (int j = 0; j < opera.size(); j++) {
				if (opera.at(j)->getText("name_n").compare("plus") == 0) {
					icons.push_back(opera.at(j));
					break;
				}
			}
		}
		else if (i == 3) {
			for (int j = 0; j < opera.size(); j++) {
				if (opera.at(j)->getText("name_n").compare("equal") == 0) {
					icons.push_back(opera.at(j));
					break;
				}
			}
		}
	}

	//Lấy hình ảnh các chữ số cho các đáp án
	for (int i = 0; i < answer_values.size(); i++) {
		for (int j = 0; j < numbers.size(); j++) {
			if (numbers.at(j)->getComparisionValue("number") == answer_values.at(i)) {
				answer_imgs.push_back(numbers.at(i));
				break;
			}
		}
	}

	//Lấy background cho slot
	slot_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png"));

	//Question Config
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
				auto img_txt = numbers.at(0)->getTextValue("name_n", m_lang->getLanguage());
				texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
}

void My_OddAndEven::onStart() {
	if (drag) {
		onShowBottomBar(0.25, 0.25, true, CC_CALLBACK_1(My_OddAndEven::onTouchAnswer, this));
	}
	else {
		onShowBottomBar();
	}
	
	//Tạo Object
	int obj_id = 0;
	for (int i = 0; i < 4; i++) {
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, icons.at(i), NULL);
		object->setObjectSize(object_size);
		if (i == 0) {
			object->setValueText(cocos2d::StringUtils::toString(operand_1));
		}
		if (i == 2) {
			object->setValueText(cocos2d::StringUtils::toString(operand_2));
		}
		if (i == 1) {
			object->setValueText("+");
		}
		if (i == 3) {
			object->setValueText("=");
		}
		object->getLabelValue()->setFontSize(80);
		object->showValueTextWithAnimation(true);
		object->setImageVisible(false);
		_objects.push_back(object);
	}

	Math_Object* obj = Math_Pool::getInstance()->getObject();
	obj->setObjectSize(object_size);
	if (drag) {
		obj->setEnable(4, math::common::deny, answer_imgs.at(0), NULL);
	}
	else {
		obj->setEnable(4, math::common::deny, answer_imgs.at(0), slot_bg);
		obj->setValueText("?");
		obj->getLabelValue()->setFontSize(70);
		obj->showValueTextWithAnimation(true);
	}
	obj->setImageVisible(false);
	_objects.push_back(obj);

	//Tạo Slot
	Math_Slot* slot = Math_Pool::getInstance()->getSlot();
	slot->setEndble(0, slot_bg);
	slot->setSlotSize(object_size);
	slot->setComparisionValue(_objects.at(4)->getComparisionValue());
	_slots.push_back(slot);

	if (drag) {
		//Tạo Dragger (Nếu là game kéo thả)
		int drag_id = 0;
		for (int i = 0; i < 4; i++) {
			Math_Object* dragger = Math_Pool::getInstance()->getObject();
			dragger->setEnable(drag_id++, math::common::touch, answer_imgs.at(i), NULL);
			dragger->setObjectSize(cocos2d::Size(100, 100));
			dragger->setValueText(cocos2d::StringUtils::toString(answer_values.at(i)));
			dragger->showValueTextWithAnimation(true);
			dragger->setImageVisible(false);
			dragger->getLabelValue()->setFontSize(80);
			dragger->setMoveCallback(nullptr, CC_CALLBACK_2(My_OddAndEven::onMoveObjectEnded, this));
			_draggers.push_back(dragger);
		}
		std::random_shuffle(_draggers.begin(), _draggers.end());

	}
	else {

		//Tạo đáp án lựa chọn (game lựa chọn)
		for (int i = 0; i < 4; i++) {
			Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
			ans->setEnable(cocos2d::StringUtils::toString(answer_values.at(i)), math::common::answer, CC_CALLBACK_1(My_OddAndEven::onTouchAnswer, this), cocos2d::Size(200, 60));
			_answers.push_back(ans);
		}

		answer = _answers.at(0);
		std::random_shuffle(_answers.begin(), _answers.end());
	}

	//Show Object
	math::func::setPositionList(_objects, _objects.size(), cocos2d::Rect(0.1, 0.15, 0.8, 0.3));
	for (Math_Object* object : _objects) {
		object->onShow(0);
		addChildNode(object, 2);
	}

	if (drag) {
		//Show Slot
		_slots.at(0)->setPosition(_objects.at(4)->getPosition());
		_slots.at(0)->onShow(0);
		addChildNode(_slots.at(0), 2);

		//Show Dragger
		math::func::setPositionList(_draggers, 4, cocos2d::Rect(0, 0, 0.75, 0.2));
		for (Math_Object* dragger : _draggers) {
			dragger->onShow(0);
			addChildNodeToBottomPanel(dragger, 2);
		}
	}
	else {
		//Show Answer
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0.15, 0, 0.7, 0.2));
		for (Math_Answer* ans : _answers) {
			ans->onShowInParent(_panel_bot, 2);
		}
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_OddAndEven::startQuestion() {
	for (Math_Object* dragger : _draggers) {
		dragger->enableDrag(_panel_bot);
	}
}

void My_OddAndEven::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		if (drag) {
			_slots.at(0)->getLinker()->backToStart();
			_slots.at(0)->removeLinker();
		}
	}
}

void My_OddAndEven::onComplete() {
	float delay = showConratulation(0.3);

	for (Math_Object* object : _objects) {
		object->onHide(delay);
		delay += 0.15;
	}

	if (drag) {
		delay -= 0.15;
		_slots.at(0)->onHide(delay);

		for (Math_Object* dragger : _draggers) {
			delay += 0.15;
			dragger->onHide(delay);
		}
	}
	else {
		for (Math_Answer* ans : _answers) {
			ans->setDisable();
		}
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_OddAndEven::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object* object = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	if (_slots.at(0)->isAvailable(pos)) {
		object->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(0)->getPosition())));
		_slots.at(0)->setLinker(object);
	}
	else {
		object->backToStart();
	}
}

void My_OddAndEven::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* ans = (Math_Answer*)node;

	bool correct = true;

	if (drag) {
		if (_slots.at(0)->isCorrect(math::comparision::number)) {
			correct = true;
		}
		else {
			correct = false;
		}
	}
	else {
		if (ans == answer) {
			correct = true;
		}
		else {
			correct = false;
		}
	}

	this->onExplanation(correct);
	ans->onAnimationClick(correct);
}
