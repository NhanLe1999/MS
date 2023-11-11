#include "My_SubtractZeroAndAll.h"

My_SubtractZeroAndAll* My_SubtractZeroAndAll::createQuiz() {
	My_SubtractZeroAndAll *quiz = new My_SubtractZeroAndAll();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_SubtractZeroAndAll::init() {
	return cocos2d::ui::Layout::init();
}

void My_SubtractZeroAndAll::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Game Config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	operation_type = json_game.FindMember("operation_type")->value.GetString();
	drag = json_game.FindMember("drag")->value.GetBool();
	rapidjson::Value& json_number = json_game.FindMember("number")->value;
	int min_number = json_number.FindMember("minimum")->value.GetInt();
	int max_number = json_number.FindMember("maximum")->value.GetInt();

	//Object Config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	rapidjson::Value& json_size = json_object.FindMember("size")->value;
	object_size.width = json_size.FindMember("width")->value.GetDouble();
	object_size.height = json_size.FindMember("height")->value.GetDouble();

	//Tạo dữ liệu cho phép toán
	subtrahend = cocos2d::random(min_number, max_number);
	if (operation_type.compare("zero") == 0) {
		minuend = 0;
	}
	else if (operation_type.compare("all") == 0) {
		minuend = subtrahend;
	}
	else {
		minuend = 0;
	}
	different = subtrahend - minuend;

	//Tạo các đáp án
	answer_values.push_back(different);
	while (answer_values.size() < 4) {
		int x = cocos2d::random(0, 10);
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

	//Lấy hình ảnh hiển thị cho objects
	auto numbers = getNumbers(0, max_number);
	auto opera = getOperators();
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == subtrahend) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 2) {
			for (int j = 0; j < numbers.size(); j++) {
				if (numbers.at(j)->getComparisionValue("number") == minuend) {
					icons.push_back(numbers.at(j));
					break;
				}
			}
		}
		else if (i == 1) {
			if (operation_type.compare("add_zero") == 0) {
				for (int j = 0; j < opera.size(); j++) {
					if (opera.at(j)->getText("name_n").compare("plus") == 0) {
						icons.push_back(opera.at(j));
						break;
					}
				}
			}
			else {
				for (int j = 0; j < opera.size(); j++) {
					if (opera.at(j)->getText("name_n").compare("minus") == 0) {
						icons.push_back(opera.at(j));
						break;
					}
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
	slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));

	//Question Config
	parseQuestion();
	if (drag) {
		getQuestion()->setKeywordValue(1);
	}
	else {
		getQuestion()->setKeywordValue(0);
	}
}

void My_SubtractZeroAndAll::onStart() {
	if (drag) {
		onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_SubtractZeroAndAll::onTouchAnswer, this));
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
			object->setValueText(cocos2d::StringUtils::toString(subtrahend));
		}
		if (i == 2) {
			object->setValueText(cocos2d::StringUtils::toString(minuend));
		}
		if (i == 1) {
			object->setValueText("-");
			if (operation_type.compare("add_zero") == 0) {
				object->setValueText("+");
			}			
		}
		if (i == 3) {
			object->setValueText("=");
		}
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
		obj->showValueTextWithAnimation(true);
	}
	obj->setImageVisible(false);
	_objects.push_back(obj);

	//Tạo Slot
	Math_Slot* slot = Math_Pool::getInstance()->getSlot();
	slot->setEndble(0, slot_bg);
	slot->setSlotSize(object_size * 0.8);
	slot->setComparisionValue(_objects.at(4)->getComparisionValue());
	_slots.push_back(slot);

	if (drag) {
		//Tạo Dragger (Nếu là game kéo thả)
		int drag_id = 0;
		for (int i = 0; i < 4; i++) {
			Math_Object* dragger = Math_Pool::getInstance()->getObject();
			dragger->setEnable(drag_id++, math::common::touch, answer_imgs.at(i), NULL);
			dragger->setObjectSize(object_size);
			dragger->setValueText(cocos2d::StringUtils::toString(answer_values.at(i)));
			dragger->showValueTextWithAnimation(true);
			dragger->setImageVisible(false);
			dragger->setMoveCallback(nullptr, CC_CALLBACK_2(My_SubtractZeroAndAll::onMoveObjectEnded, this));
			_draggers.push_back(dragger);
		}
		std::random_shuffle(_draggers.begin(), _draggers.end());
	}
	else {
		//Tạo đáp án lựa chọn (game lựa chọn)
		for (int i = 0; i < 4; i++) {
			Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
			ans->setEnable(cocos2d::StringUtils::toString(answer_values.at(i)), math::common::answer, CC_CALLBACK_1(My_SubtractZeroAndAll::onTouchAnswer, this), cocos2d::Size(200, 70));
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
		math::func::setPositionList(_draggers, 4, cocos2d::Rect(0.05, 0, 0.7, 0.2));
		for (Math_Object* dragger : _draggers) {
			dragger->onShow(0);
			addChildNodeToBottomPanel(dragger, 2);
		}
	}
	else {
		//Show Answer
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0.1, 0, 0.8, 0.2));
		for (Math_Answer* ans : _answers) {
			ans->onShowInParent(_panel_bot, 2);
		}
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_SubtractZeroAndAll::startQuestion() {
	scheduleOnce([=](float dt) {
		for (Math_Object* dragger : _draggers) {
			dragger->enableDrag(_panel_bot);
		}
	}, 0.5, "Enable drag");	
}

void My_SubtractZeroAndAll::onExplanation(bool correct) {
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

void My_SubtractZeroAndAll::onComplete() {
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

void My_SubtractZeroAndAll::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
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

void My_SubtractZeroAndAll::onTouchAnswer(cocos2d::Node* node) {
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
