#include "My_NumberInSubtraction.h"

My_NumberInSubtraction* My_NumberInSubtraction::createQuiz() {
	My_NumberInSubtraction *quiz = new My_NumberInSubtraction();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_NumberInSubtraction::init() {
	return cocos2d::ui::Layout::init();
}

void My_NumberInSubtraction::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	auto number_max = json_game.FindMember("range")->value.FindMember("max")->value.GetInt();
	auto number_min = json_game.FindMember("range")->value.FindMember("min")->value.GetInt();
	drag = json_game.FindMember("drag_drop")->value.GetBool();
	multiple = json_game.FindMember("multiple")->value.GetString();

	number_object = cocos2d::random(number_min, number_max);
	number_remove = cocos2d::random(1, number_object - 1);
	result = number_object - number_remove;

	if (multiple.compare("none") != 0) {
		if (multiple.compare("multiple") == 0) {
			number_object = cocos2d::random(1, (int)number_max / 10) * 10;
		}
		else if (multiple.compare("random") == 0) {
			number_object = cocos2d::random(10, number_max);
		}

		number_remove = cocos2d::random(1, number_object / 10) * 10;
		result = number_object - number_remove;
	}

	//TODO: ICONS PREVIEW
	rapidjson::Value& json_preview = json_data.FindMember("preview")->value;
	show_icon = json_preview.FindMember("show")->value.GetBool();
	touch_remove = json_preview.FindMember("touch_remove")->value.GetBool();
	icon_size = cocos2d::Size(json_preview.FindMember("size")->value.FindMember("width")->value.GetInt(), json_preview.FindMember("size")->value.FindMember("height")->value.GetInt());

	//TODO: OBJECT CONFIG
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	object_size = cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
	object_bg = json_object.FindMember("background")->value.GetString();

	object_show = json_object.FindMember("number")->value.FindMember("type")->value.GetString();
	object_value_position = json_object.FindMember("number")->value.FindMember("position")->value.GetString();
	operator_show = json_object.FindMember("operator")->value.FindMember("type")->value.GetString();

	//TODO: DRAGER CONFIGslo
	rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
	number_dragger = cocos2d::random(json_drager.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_drager.FindMember("number")->value.FindMember("maximum")->value.GetInt());
	dragger_size = cocos2d::Size(json_drager.FindMember("size")->value.FindMember("width")->value.GetInt(), json_drager.FindMember("size")->value.FindMember("height")->value.GetInt());
	int value_min = json_drager.FindMember("value")->value.FindMember("minimum")->value.GetInt();
	int value_max = json_drager.FindMember("value")->value.FindMember("maximum")->value.GetInt();

	if (multiple.compare("multiple") == 0 || multiple.compare("random") == 0) {
		show_icon = false;
		touch_remove = false;
	}

	//game luôn có 4 dap an (drrager hoặc answer)
	std::vector<int> wrong_list;
	if (multiple.compare("none") == 0) {
		while (wrong_list.size() < number_dragger - 1) {
			bool add = true;
			int value = cocos2d::random(value_min, value_max);
			for (int _val : wrong_list) {
				if (value == _val) {
					add = false;
					break;
				}
			}
			if (add) {
				if (value != result) {
					wrong_list.push_back(value);
				}
			}
		}
	}
	else {
		while (wrong_list.size() < number_dragger - 1) {
			bool add = true;
			int value;
			if (multiple.compare("multiple") == 0) {
				value = cocos2d::random(value_min / 10, value_max / 10) * 10;
			}
			else {
				value = cocos2d::random(value_min, value_max);
			}
			for (int _val : wrong_list) {
				if (value == _val) {
					add = false;
					break;
				}
			}
			if (add) {
				if (value != result) {
					wrong_list.push_back(value);
				}
			}
		}
	}

	drag_values.push_back(result);
	for (int i = 0; i < wrong_list.size(); i++) {
		drag_values.push_back(wrong_list.at(i));
	}
	math::func::shuffle(drag_values);

	//Lấy tất cả các icon
	img_list = getImageList("icons");
	auto im = math::func::getRandomNode(img_list);
	for (math::resource::Image* img : img_list) {
		if (img->getComparisionValue("pair") == im->getComparisionValue("pair")) {
			if (im->getComparisionValue("role") == 0) {
				icon_imgs.push_back(im);
				icon_imgs.push_back(img);
			}
			else {
				icon_imgs.push_back(img);
				icon_imgs.push_back(im);
			}
			break;
		}
	}

	//Lấy hình ảnh các chữ số
	num = getNumbers(0, number_max);
	std::vector<math::resource::Image*> opera = getOperators();

	for (int i = 0; i < num.size(); i++) {
		int img_val = num.at(i)->getComparisionValue("number");
		if (img_val == number_object) {
			number_imgs.push_back(num.at(i));
			num.at(i)->used = true;
			continue;
		}

		if (img_val == number_remove) {
			number_imgs.push_back(num.at(i));
			num.at(i)->used = true;
			continue;
		}

		for (int value : drag_values) {
			if (value == img_val) {
				num.at(i)->used = true;
				number_imgs.push_back(num.at(i));
				break;
			}
		}
	}

	for (int i = 0; i < opera.size(); i++) {
		if (opera.at(i)->getText("name_1").compare("minus") == 0) {
			number_imgs.push_back(opera.at(i));
			opera.at(i)->used = true;
			continue;
		}
		if (opera.at(i)->getText("name_1").compare("equal") == 0) {
			number_imgs.push_back(opera.at(i));
			opera.at(i)->used = true;
			continue;
		}
	}

	//Xử lý câu hỏi
	auto number_1 = getNumbers(number_object, number_object);
	auto number_2 = getNumbers(number_remove, number_remove);
	std::vector<math::resource::Image*> img_number_list;
	img_number_list.push_back(number_1.at(0));
	img_number_list.push_back(number_2.at(0));

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
		if (key == "name_n") {
			std::vector<math::resource::Text> texts;
			for (int i = 0; i < img_number_list.size(); i++) {
				auto number = img_number_list.at(i)->getComparisionValue("number");
				if (number >= 2) {
					math::resource::Text img_txt;
					img_txt = im->getTextValue("name_n", m_lang->getLanguage());
					texts.push_back(img_txt);
				}
				else {
					math::resource::Text img_txt;
					img_txt = im->getTextValue("name_1", m_lang->getLanguage());
					texts.push_back(img_txt);
				}
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
	}

	if (drag) {
		getQuestion()->setKeywordValue(0);
	}
	else {
		getQuestion()->setKeywordValue(1);
	}

	//Tạo đối tượng mo ta
	if (show_icon) {
		math::common::TouchPermission touch_per = math::common::deny;
		if (touch_remove) {
			touch_per = math::common::touch;
		}
		for (int i = 0; i < number_object; i++) {
			math::resource::Image* img = icon_imgs.at(0);
			if (getQuestion()->getNumberQuestions() == 1 && i >= result) {
				img = icon_imgs.at(1);
			}

			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(i, touch_per, img, NULL);
			object->setObjectSize(icon_size);
			object->setTouchCallback(CC_CALLBACK_1(My_NumberInSubtraction::onTouchedObject, this));
			_icons.push_back(object);
		}
	}

	if (drag) {
		if (getQuestion()->getNumberQuestions() == 1) {
			onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(My_NumberInSubtraction::onTouchAnswer, this));
		}
		else {
			onShowBottomBar(0.5, 3, true, CC_CALLBACK_1(My_NumberInSubtraction::onTouchAnswer, this));
		}
	}
	else {
		if (getQuestion()->getNumberQuestions() == 1) {
			onShowBottomBar();
		}
		else {
			onShowBottomBar(3);
		}
	}

	if (drag) {
		for (int i = 0; i < number_dragger; i++) {
			for (math::resource::Image* img : number_imgs) {
				if (img->getComparisionValue("number") == drag_values[i]) {
					image = img;
					break;
				}
			}

			std::string title;
			if (object_show.compare("number") == 0) {
				title = cocos2d::StringUtils::toString(drag_values[i]);
			}
			else {
				//math::resource::Image* image = NULL;                
				title = image->getText("name_1");
			}
			math::resource::Image* drag_bg = math::resource::ResHelper::createImage(getUIPath("common/drag_2.png"));
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(i, math::common::deny, image, drag_bg);
			object->setObjectSize(dragger_size);
			object->setImageVisible(false);
			object->setValueText(title);
			object->showValueTextWithAnimation(false);
			object->getLabelValue()->setFontSize(object_size.height*.75);
			object->setMoveCallback(CC_CALLBACK_1(My_NumberInSubtraction::onMoveObjectBegan, this), CC_CALLBACK_2(My_NumberInSubtraction::onMoveObjectEnded, this));
			_draggers.push_back(object);

			if (drag_values[i] == result) {
				math::resource::Image* slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop_2.png"));
				Math_Slot* slot = Math_Pool::getInstance()->getSlot();
				slot->setEndble(0, slot_bg);
				slot->setSlotSize(dragger_size);
				slot->setComparisionValue(object->getComparisionValue());
				_slots.push_back(slot);
			}
		}
		math::func::setPositionList(_draggers, number_dragger, cocos2d::Rect(0.01, 0, 0.73, 0.2));
	}
	else {
		//Tạo các đáp án (Game yêu cầu chọn đáp án)
		for (int i = 0; i < number_dragger; i++) {
			std::string title;
			if (object_show.compare("number") == 0) {
				title = cocos2d::StringUtils::toString(drag_values[i]);
			}
			else {
				math::resource::Image* image = NULL;
				for (math::resource::Image* img : number_imgs) {
					if (img->getComparisionValue("number") == drag_values[i]) {
						image = img;
						break;
					}
				}
				title = image->getText("name_1");
			}

			Math_Answer* answer = Math_Pool::getInstance()->getAnswer();
			answer->setEnable(title, math::common::answer, CC_CALLBACK_1(My_NumberInSubtraction::onTouchAnswer, this), cocos2d::Size(250, 60));
			_answers.push_back(answer);
			test_chose.push_back(answer);
			if (drag_values[i] == result) {
				answer->setCorrect(true);
			}
		}
		math::func::setPositionList(_answers, 2, cocos2d::Rect(0, 0, 1, 0.2));
		math::func::shuffle(test_chose);
	}

	//Tao phep toan
	for (int i = 0; i < 5; i++) {
		//math::resource::Image* image=NULL;
		std::string show_key = "number";
		switch (i) {
		case 0:
		{
			for (math::resource::Image* img : number_imgs) {
				if (img->getComparisionValue("number") == number_object) {
					image = img;
					break;
				}
			}
			if (object_show.compare("number") == 0) {
				show_key = cocos2d::StringUtils::toString(number_object);
			}
			else {
				show_key = image->getText("name_1");
			}
		}
		break;
		case 2:
		{
			for (math::resource::Image* img : number_imgs) {
				if (img->getComparisionValue("number") == number_remove) {
					image = img;
					break;
				}
			}
			if (object_show.compare("number") == 0) {
				show_key = cocos2d::StringUtils::toString(number_remove);
			}
			else {
				show_key = image->getText("name_1");
			}
		}
		break;
		case 4:
		{
			show_key = "?";
			if (drag) {
				show_key = "";
			}
		}
		break;
		case 1:
		{
			for (math::resource::Image* img : number_imgs) {
				if (img->getText("name_n").compare("minus") == 0) {
					image = img;
					break;
				}
			}
			if (operator_show.compare("number") == 0) {
				show_key = "-";
			}
			else {
				show_key = "minus";
			}
		}
		break;
		case 3:
		{
			for (math::resource::Image* img : number_imgs) {
				if (img->getText("name_n").compare("equal") == 0) {
					image = img;
					break;
				}
			}
			if (operator_show.compare("number") == 0) {
				show_key = "=";
			}
			else {
				show_key = "equal";
			}
		}
		break;
		default:
			break;
		}

		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(i, math::common::deny, image, NULL);
		object->setObjectSize(object_size);
		object->setImageVisible(false);
		object->setValueText(show_key);
		object->showValueTextWithAnimation(false);
		object->getLabelValue()->setFontSize(object_size.height*.75);
		_objects.push_back(object);
	}
}

void My_NumberInSubtraction::onStart() {
	if (show_icon) {
		cocos2d::Rect rect = cocos2d::Rect(0, 0.35, 1, 0.15);
		if (number_object <= 5) {
			rect = cocos2d::Rect(0.2, 0.35, 0.6, 0.15);
		}
		math::func::setPositionList(_icons, number_object, rect);
		for (Math_Object* icon : _icons) {
			this->addChildNode(icon);
			icon->onShow(0);
		}
	}
	else {
		//TODO: fix vị trí cho phép toán.
	}

	if (drag) {
		for (Math_Object* object : _draggers) {
			this->addChildNodeToBottomPanel(object);
			object->onShow(1.0);
		}
	}
	else {
		for (Math_Answer* answer : _answers) {
			answer->onShowInParent(_panel_bot, 2);
		}
	}

	float object_y = 0.25;
	if (drag) {
		if (show_icon) {
			object_y = 0.125;
		}
		else {
			object_y = 0.175;
		}
	}
	else {
		if (show_icon) {
			object_y = 0.1;
		}
		else {
			object_y = 0.15;
		}
	}

	if (getQuestion()->getNumberQuestions() > 1) {
		_objects[0]->setPosition(cocos2d::Vec2(quiz_size.width / 2, (object_y + 0.1) * quiz_size.height));
		this->addChildNode(_objects[0], 2);
		_objects[0]->onShow(0.5);
	}
	else {
		math::func::setPositionList(_objects, 5, cocos2d::Rect(0.15, object_y, 0.7, 0.2));
		if (drag) {
			_slots[0]->setPosition(_objects.back()->getPosition());
			this->addChildNode(_slots[0]);
			_slots[0]->onShow(0.5);
		}

		for (Math_Object* object : _objects) {
			this->addChildNode(object);
			object->onShow(0.5);
		}
	}

	showQuestionBox();
	disableControlQuiz(2);
}

void My_NumberInSubtraction::startQuestion() {
	CCLOG("Override");
	enableControlQuiz();
	/*if (show_icon) {

	}
	else {*/
	if (getQuestion()->getData().index == 0) {
		if (!touch_remove && getQuestion()->getNumberQuestions() > 1) {
			scheduleOnce([=](float) {
				float object_y = show_icon ? 0.1 : 0.15;
				math::func::setPositionList(_objects, 5, cocos2d::Rect(0.2, object_y, .6, 0.2));
				for (Math_Object* object : _objects) {
					if (object != _objects.front()) {
						this->addChildNode(object);
						object->onShow(0.5);
					}
				}

				for (int i = result; i < _icons.size(); i++) {
					_icons.at(i)->setImage(icon_imgs.at(1));
				}
				nextQuestion();
			}, 3, "next question");
		}
	}

	if (drag) {
		scheduleOnce([=](float) {
			for (Math_Object* object : _draggers) {
				object->enableDrag(_panel_bot);
			}
		}, 0.5, "next ques");
	}
	//}
}

void My_NumberInSubtraction::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
	object->setImage(icon_imgs.at(1));
	object->setLocked(true);
	count++;
	if (count == number_remove) {
		for (Math_Object* obj : _icons) {
			obj->setLocked(true);
		}
		Math_Object* obj_first = _objects[0];
		float object_y = obj_first->getPosition().y / quiz_size.height - 0.1;
		math::func::setPositionList(_objects, 5, cocos2d::Rect(0.2, object_y, .6, 0.2));

		float delay = 0.1;
		for (Math_Object* obj : _objects) {
			if (!obj->isSame(obj_first)) {
				this->addChildNode(obj);
				delay += 0.15;
				obj->onShow(delay);
			}
		}
		if (drag) {
			_slots[0]->setPosition(_objects.back()->getPosition());
			this->addChildNode(_slots[0]);
			_slots[0]->onShow(delay);
		}

		scheduleOnce([=](float) {
			onExplanation(true);
		}, delay, "next ques");
	}
}

void My_NumberInSubtraction::onMoveObjectBegan(cocos2d::Node* node) {

}

void My_NumberInSubtraction::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {

	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);

	Math_Slot* m_slot = _slots[0];
	Math_Object *object = (Math_Object*)node;
	Math_Object *linker = m_slot->getLinker();

	if (object->isSame(linker)) {
		if (m_slot->isAvailable(pos)) {
			object->backToLast(2);
			return;
		}
		else {
			m_slot->removeLinker();
			object->backToStart();
			return;
		}
	}
	else {
		if (m_slot->isAvailable(pos)) {
			if (m_slot->isUsed()) {
				m_slot->getLinker()->backToStart();
				m_slot->removeLinker();
			}
			object->runAction(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(0)->getPosition())));
			m_slot->setLinker(object);
			return;
		}
		else {
			object->backToStart();
			return;
		}
	}
}

void My_NumberInSubtraction::onTouchAnswer(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	if (drag) {
		bool done = false;
		bool ok = true;
		for (Math_Slot* slot : _slots) {
			if (!slot->isUsed()) {
				ok = false;
			}
		}
		if (ok) {
			if (_slots[0]->isCorrect(math::comparision::number)) {
				done = true;
			}
			answer->onAnimationClick(done);
			onExplanation(done);
		}
		else {
			return;
		}
	}
	else {
		answer->onAnimationClick(answer->isCorrect());
		onExplanation(answer->isCorrect());
	}
}

void My_NumberInSubtraction::onExplanation(bool correct) {
	if (correct) {
		this->nextQuestion();
	}
	else {
		if (drag) {
			_slots.at(0)->getLinker()->backToStart();
			_slots.at(0)->getLinker()->runAction(cocos2d::ScaleTo::create(0.2, 1));
			_slots.at(0)->removeLinker();
			clickable = false;
		}
	}
}

void My_NumberInSubtraction::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.5;
	for (Math_Object* object : _draggers) {
		if (!object->isSame(_slots[0]->getLinker())) {
			object->onHide(m_delay);
		}
	}

	float d_delay = delay - 0.3;
	for (Math_Object* object : _icons) {
		object->onHide(d_delay);
	}
	for (Math_Object* object : _objects) {
		d_delay += 0.15;
		object->onHide(d_delay);
	}
	if (_slots.size() > 0) {
		_slots[0]->onHide(d_delay);
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void My_NumberInSubtraction::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, 8, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void My_NumberInSubtraction::autoTestingRun(float delaytime) {
	if (object_removed < number_remove) {
		int vt = 0;
		int ran = cocos2d::random(0, number_object - 1);
		bool selected = false;
		for (int i = 0; i < selected_Objects.size(); i++) {
			if (_objects.at(ran) == selected_Objects.at(i)) {
				selected = true;
				vt = i;
				break;
			}
		}
		if (selected) {
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
		else {
			_objects.at(ran)->setColor(cocos2d::Color3B(0, 0, 0));
			object_removed++;
			if (object_removed == number_remove) {
				//this->showDragger();
				this->scheduleOnce([=](float dt) {
					this->autoTestingRun(delaytime);
				}, 3, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
				return;
			}
			this->scheduleOnce([=](float dt) {
				this->autoTestingRun(delaytime);
			}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
		}
	}
	else {
		if (drag) {
			Math_Object* object = test_draged.at(test_draged.size() - 1);
			test_draged.pop_back();
			object->runAction(cocos2d::MoveTo::create(0.2, _slots.at(0)->getPosition()));
			_slots.at(0)->setLinker(object);
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
				_answers.at(0)->onAutoClick();
				if (_slots.at(0)->getLinker()->getImageSource()->getComparisionValue("number") == result) {
					this->nextQuestion();
				}
				else {
					_slots.at(0)->getLinker()->backToStart();
					_slots.at(0)->removeLinker();
					this->scheduleOnce([=](float dt) {
						this->autoTestingRun(delaytime);
					}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
				}
			}), NULL));
		}
		else {
			Math_Answer* answer = test_chose.at(test_chose.size() - 1);
			test_chose.pop_back();
			answer->onAutoClick();
			int vt = 0;
			for (int i = 0; i < 4; i++) {
				if (_answers.at(i) == answer) {
					vt = i;
					break;
				}
			}
			if (drag_values.at(vt) == result) {
				this->nextQuestion();
			}
			else {
				this->scheduleOnce([=](float dt) {
					this->autoTestingRun(delaytime);
				}, delaytime, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
			}
		}
	}
}

