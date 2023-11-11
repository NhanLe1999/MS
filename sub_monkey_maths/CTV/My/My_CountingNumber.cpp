#include "My_CountingNumber.h"

My_CountingNumber* My_CountingNumber::createQuiz() {
	My_CountingNumber *quiz = new My_CountingNumber();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_CountingNumber::init() {
	return cocos2d::ui::Layout::init();
}

void My_CountingNumber::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//Count_type config
	rapidjson::Value& type = json_data.FindMember("count_type")->value;
	std::vector<std::string> type_values;
	for (rapidjson::SizeType i = 0; i < type.Size(); i++) {
		type_values.push_back(type[i].GetString());
	}
	count_type = type_values.at(cocos2d::random(0, (int)(type_values.size() - 1)));

	//Dragger Config
	rapidjson::Value& json_dragger = json_data.FindMember("object")->value;
	int x = json_dragger.FindMember("number")->value.FindMember("minimum")->value.GetInt();
	number_dragger = cocos2d::random(json_dragger.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_dragger.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	int min = json_dragger.FindMember("start")->value.FindMember("minimum")->value.GetInt();
	int max = json_dragger.FindMember("start")->value.FindMember("maximum")->value.GetInt();
	dragger_start = cocos2d::random(min, max);
	/*if (count_type.compare("forward") == 0) {
		dragger_start = cocos2d::random(0, 8);
	}
	else {
		dragger_start = cocos2d::random(number_dragger, 10);
	}*/

	rapidjson::Value& json_size = json_dragger.FindMember("size")->value;
	dragger_size.height = json_size.FindMember("height")->value.GetDouble();
	dragger_size.width = json_size.FindMember("width")->value.GetDouble();

	//Slot Config
	rapidjson::Value& json_slot = json_data.FindMember("slots")->value;
	slot_size.width = json_slot.FindMember("size")->value.FindMember("width")->value.GetDouble();
	slot_size.height = json_slot.FindMember("size")->value.FindMember("height")->value.GetDouble();

	number_slot = number_dragger;
	if (number_dragger < 3) {
		number_dragger = 4;
	}

	//lấy background cho slot
	slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
	drag_bg = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));

	//Tạo vector lưu các số đếm
	auto num = getNumbers(0, dragger_start + number_dragger + 2);
	if (count_type.compare("forward") == 0) {
		for (int i = 0; i < number_dragger; i++) {
			numbers.push_back(num.at(dragger_start + i));
		}
	}
	else {
		for (int i = dragger_start; i >= dragger_start - number_dragger; i--) {
			numbers.push_back(num.at(i));
		}
	}

	//Question Config
	auto img_number_list = getNumbers(dragger_start, dragger_start);
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

	if (count_type.compare("forward") == 0) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}
}

void My_CountingNumber::onStart() {
	onShowBottomBar();

	first = Math_Pool::getInstance()->getObject();
	first->setEnable(0, math::common::touch, numbers.at(0), drag_bg);
	first->setImageVisible(false);
	first->setObjectSize(dragger_size);
	first->setValueText(cocos2d::StringUtils::toString(numbers.at(0)->getComparisionValue("number")));
	//first->getLabelValue()->setFontSize(70);

	//Tạo slots
	int slot_id = 0;
	for (int i = 0; i < number_slot; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(slot_id++, slot_bg);
		slot->setSlotSize(dragger_size);
		if (i == 0) {
			slot->setComparisionValue(first->getComparisionValue());
			slot->setBackgroundVisible(false);
			slot->setLinker(first);
		}
		_slots.push_back(slot);
	}

	//Tạo mũi tên
	int arrow_id = 0;
	math::resource::Image* ar_bg = math::resource::ResHelper::createImage(getUIPath("common/arrow.png"));
	if (count_type.compare("backward") == 0) {
		
	}
	for (int i = 0; i < number_slot - 1; i++) {
		cocos2d::ui::ImageView* arrow = cocos2d::ui::ImageView::create(getUIPath("common/arrow.png"));
		if (count_type.compare("backward") == 0) {
			arrow->setFlippedX(true);
		}
		arrow->setContentSize(dragger_size * 0.5);
		_arrows.push_back(arrow);
	}

	//Tạo dragger
	math::common::TouchPermission touch_per = math::common::touch;
	int drag_id = 0;
	for (int i = 0; i < number_dragger - 1; i++) {
		Math_Object* dragger = Math_Pool::getInstance()->getObject();
		dragger->setEnable(drag_id++, touch_per, numbers.at(i + 1), drag_bg);
		dragger->setImageVisible(false);
		dragger->setObjectSize(dragger_size);
		dragger->setValueText(cocos2d::StringUtils::toString(numbers.at(i + 1)->getComparisionValue("number")));
		dragger->showValueTextWithAnimation(true);
		if (i + 1 < _slots.size()) {
			_slots.at(i + 1)->setComparisionValue(dragger->getComparisionValue());
		}		
		dragger->setMoveCallback(nullptr, CC_CALLBACK_2(My_CountingNumber::onMoveObjectEnded, this));
		test_vector.push_back(dragger);
		_draggers.push_back(dragger);
	}

	std::random_shuffle(_draggers.begin(), _draggers.end());
	std::random_shuffle(test_vector.begin(), test_vector.end());

	//Show slots
	math::func::setPositionList(_slots, number_slot, cocos2d::Rect(0, 0.25, 1, 0.2));
	float delay = 0;
	for (int i = 0; i < _slots.size(); i++) {
		_slots[i]->onShow(delay);
		addChildNode(_slots[i], 2);
		if (i > 0) {
			float xx = (_slots[i]->getPosition().x + _slots[i - 1]->getPosition().x) / 2;
			_arrows[i - 1]->setPosition(cocos2d::Vec2(xx, _slots[i]->getPosition().y));
		}		
		delay += 0.15;
	}

	for (cocos2d::ui::ImageView* arrow : _arrows) {
		addChildNode(arrow, 2);
	}

	first->getLabelValue()->setVisible(true);
	first->setPosition(_slots.at(0)->getPosition());
	first->onShow(0);
	first->setLocked(true);
	first->setVisible(true);
	this->addChildNode(first, 10);

	//Show dragger
	math::func::setPositionList(_draggers, number_dragger, cocos2d::Rect(0, 0, 1, 0.2));
	for (Math_Object* object : _draggers) {
		object->onShow(delay);
		delay += 0.15;
		addChildNodeToBottomPanel(object, 2);
	}

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_CountingNumber::startQuestion() {
	for (Math_Object* dragger : _draggers) {
		dragger->enableDrag(_panel_bot);
	}
}

void My_CountingNumber::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object* dragger = (Math_Object*)node;
	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	for (int i = count + 2; i < number_slot; i++) {
		if (_slots.at(i)->isAvailable(pos)) {
			_slots.at(count + 1)->runAction(cocos2d::Sequence::create(cocos2d::ScaleBy::create(0.2, 1.2), cocos2d::ScaleTo::create(0.2, 1), NULL));
		}
	}

	if (_slots.at(count + 1)->isAvailable(pos)) {
		_slots.at(count + 1)->setLinker(dragger);
		if (_slots.at(count + 1)->isCorrect(math::comparision::number)) {
			dragger->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(count + 1)->getPosition())));
			count++;
			dragger->setLocked(true);
			if (count == number_slot - 1) {
				scheduleOnce([=](float dt) {
					this->nextQuestion();
				}, 2, "Done");
			}
		}
		else {
			_slots.at(count + 1)->removeLinker();
			dragger->backToStart();
		}

		return;
	}

	dragger->backToStart();
}

void My_CountingNumber::onExplanation(bool correct) {
	CCLOG("Override");
}

void My_CountingNumber::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	first->onHide(0);

	m_delay = 0.15;
	for (Math_Slot* slot : _slots) {
		slot->getLinker()->onHide(m_delay);
		slot->onHide(m_delay);
		m_delay += 0.15;
	}
	m_delay = 0.225;

	for (cocos2d::ui::ImageView* im : _arrows) {
		im->removeFromParent();
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_CountingNumber::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, 3, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void My_CountingNumber::autoTestingRun(float delaytime) {
	if (count + 1 == number_slot) {
		this->nextQuestion();
		return;
	}

	Math_Object* drag = test_vector.at(test_vector.size() - 1);
	test_vector.pop_back();
	drag->runAction(cocos2d::MoveTo::create(0.8, _slots.at(count + 1)->getPosition()));
	_slots.at(count + 1)->setLinker(drag);
	if (_slots.at(count + 1)->isCorrect(math::comparision::number)) {
		count++;
		for (int i = 0; i < _draggers.size(); i++) {
			if (_slots.at(count)->getLinker() == _draggers.at(i)) {
				_draggers.erase(_draggers.begin() + i);
				break;
			}
		}
		test_vector.clear();
		for (int i = 0; i < _draggers.size(); i++) {
			test_vector.push_back(_draggers.at(i));
		}
		std::random_shuffle(test_vector.begin(), test_vector.end());

		this->scheduleOnce([=](float dt) {
			this->autoTestingRun(delaytime);
		}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
	}
	else {
		drag->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.9), cocos2d::CallFunc::create([=]() {
			_slots.at(count + 1)->removeLinker();
			drag->backToStart();
		}), NULL));
		this->scheduleOnce([=](float dt) {
			this->autoTestingRun(delaytime);
		}, 2 * delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
	}
}
