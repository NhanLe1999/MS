#include "My_MatchingNumber.h"

My_MatchingNumber* My_MatchingNumber::createQuiz() {
	My_MatchingNumber *quiz = new My_MatchingNumber();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool My_MatchingNumber::init() {
	return cocos2d::ui::Layout::init();
}

void My_MatchingNumber::initQuiz(mj::model::Game game) {
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	//GAME CONFIG
	rapidjson::Value& json_data = document.FindMember("data")->value;
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	num_block = json_game.FindMember("num_block")->value.GetInt();

	int ran = cocos2d::random(0, 1);
	if (ran == 1) {
		kieu_hien_thi = "number";
	}
	else {
		kieu_hien_thi = "text";
	}
	kieu_sap_xep = json_game.FindMember("kieu_sap_xep")->value.GetString();
	phep_toan = json_game.FindMember("phep_toan")->value.GetString();

	if (phep_toan.compare("tong") == 0) {
		game_type = GameType::Sum;
	}
	else if (phep_toan.compare("hieu") == 0) {
		game_type = GameType::Sub;
	}
	else if (phep_toan.compare("bang_nhau") == 0) {
		game_type = GameType::Equal;
	}
	else {
		game_type = GameType::Image;
	}

	if (game_type == GameType::Sum || game_type == GameType::Image) {
		num_request = cocos2d::random(num_block + 1, 10);
	}
	else {
		num_request = cocos2d::random(1, 10 - num_block); 
	}

	//OBJECT CONFIG
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	auto maximum = json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
	auto minimum = json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
	object_size = cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
	object_bg = json_object.FindMember("background")->value.GetString();
	//object_value = json_object.FindMember("value")->value.FindMember("type")->value.GetString();
	//object_value_position = json_object.FindMember("value")->value.FindMember("position")->value.GetString();
	number_object = cocos2d::random(minimum, maximum);

	//Lấy hình ảnh các chữ số
	auto num = getNumbers(0, 20);

	//Tạo vector lưu giá trị bên trái (trên)
	while (left_values.size() < num_block) {
		int ran;
		if (game_type == GameType::Sum || game_type == GameType::Image) {
			ran = cocos2d::random(1, num_request - 1);
		}
		else if (game_type == GameType::Sub) {
			ran = cocos2d::random(num_request, 9);
		}
		else {
			ran = cocos2d::random(1, 9);
		}
		bool add = true;
		for (int val : left_values) {
			if (ran == val) {
				add = false;
				break;
			}
		}
		if (add) left_values.push_back(ran);
	}
	std::random_shuffle(left_values.begin(), left_values.end());

	for (int i = 0; i < left_values.size(); i++) {
		for (int j = 0; j < num.size(); j++) {
			if (num.at(j)->getComparisionValue("number") == left_values.at(i)) {
				left_numbers.push_back(num.at(j));
				break;
			}
		}
	}

	std::vector<int> values;
	while (values.size() < num_block) {
		int ran = cocos2d::random(1, 8);
		bool added = false;
		for (int i = 0; i < values.size(); i++) {
			if (values.at(i) == ran) {
				added = true;
			}
		}
		if (!added) {
			values.push_back(ran);
		}
	}
	//lấy ảnh nền cho left-block
	for (int i = 0; i < num_block; i++) {
		math::resource::Image* img;
		std::string str = "gameui/puzzle/left_" + cocos2d::StringUtils::toString(values.at(i)) + ".png";
		img = math::resource::ResHelper::createImage(getUIPath(str));
		left_imgs_bg.push_back(img);
	}

	//Lấy ảnh nền cho right-block
	for (int i = 0; i < num_block; i++) {
		math::resource::Image* img;
		std::string str = "gameui/puzzle/right_" + cocos2d::StringUtils::toString(values.at(i)) + ".png";
		img = math::resource::ResHelper::createImage(getUIPath(str));
		right_imgs_bg.push_back(img);
	}

	//Lấy hình ảnh hiển thị trên object
	auto img_list = getImageList("icons");
	icon = math::func::getRandomNode(img_list);

	//Tạo vector lưu các giá trị bên phải (dưới) theo vector left_values
	if (game_type == GameType::Sub) {
		for (int i = 0; i < num_block; i++) {
			int temp;
			if (left_values.at(i) > num_request)
				temp = left_values.at(i) - num_request;
			else temp = num_request - left_values.at(i);
			right_values.push_back(temp);
		}
	}
	else if (game_type == GameType::Equal || game_type == GameType::Image) {
		for (int temp : left_values) {
			right_values.push_back(temp);
		}
	}
	else {
		for (int i = 0; i < num_block; i++) {
			int temp = num_request - left_values.at(i);
			right_values.push_back(temp);
		}
	}

	for (int i = 0; i < right_values.size(); i++) {
		for (int j = 0; j < num.size(); j++) {
			if (num.at(j)->getComparisionValue("number") == right_values.at(i)) {
				right_numbers.push_back(num.at(j));
				break;
			}
		}
	}

	auto img_number_list = getNumbers(num_request, num_request);

	//Xử lý câu hỏi
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

void My_MatchingNumber::onStart() {
	onShowBottomBar(0.25, 0.15, false, CC_CALLBACK_1(My_MatchingNumber::onAnswerDone, this), nullptr, true);
	cocos2d::Size size_fix(30, 30);

	int obj_id = 0;
	if (game_type != GameType::Image) {
		//Tạo object
		math::common::TouchPermission touch_per_object = math::common::deny;		
		for (int i = 0; i < num_block; i++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setObjectSize(cocos2d::Size(object_size.width, object_size.height * 1.3));
			object->setEnable(obj_id++, touch_per_object, left_numbers.at(i), left_imgs_bg[i]);
			if (kieu_hien_thi.compare("number") != 0) {
				object->setValueText(object->getImageSource()->getText("name_1"));
				object->getLabelValue()->setFontSize(40);
				object->getLabelValue()->setPosition(cocos2d::Vec2(object_size.width * 0.35, object_size.height * 1.3 / 2));
				object->setImageVisible(false);
			}
			else {
				object->setImageVisible(true);
			}
			if (kieu_sap_xep.compare("left-right") == 0) {
				object->getBackground()->setRotation(-90);
			}
			object->getImage()->setContentSize(object_size* 1.1);
			object->getImage()->setPosition(cocos2d::Vec2(object_size.width * 0.35, object_size.height * 1.3 / 2));
			object->showValueTextWithAnimation(true);
			_objects.push_back(object);
		}
	}
	else {
		//Tạo Group
		for (int i = 0; i < num_block; i++) {
			cocos2d::Size obj_size = object_size / 4;
			std::vector<Math_Object*> _group_list;
			for (int j = 0; j < left_values.at(i); j++) {
				Math_Object* object = mpool->getObject();
				object->setEnable(j, math::common::deny, icon, NULL);
				object->setObjectSize(obj_size);
				_group_list.push_back(object);
			}

			Math_Group* group = mpool->getGroup();
			group->setEnable(i, _group_list, math::group::custom, left_values[i]);
			group->setObjectSize(obj_size);
			//group->setGroupSize(cocos2d::Size(object_size.width, object_size.height));
			group->setGroupQueue(cocos2d::Size(object_size.width, object_size.height * 1.3));
			group->setBackground(left_imgs_bg[i]);
			_groups.push_back(group);
		}
	}

	//Tạo các slot
	math::resource::Image* slot_bg;
	slot_bg = left_imgs_bg.at(0);

	for (int i = 0; i < num_block; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(obj_id++, slot_bg);
		slot->setSlotSize(object_size);
		slot->setBackgroundVisible(false);
		_slots.push_back(slot);
	}

	//Tạo Dragger
	math::common::TouchPermission touch_per_dragger = math::common::touch;
	int dra_id = 0;
	for (int i = 0; i < num_block; i++) {
		Math_Object* dragger = Math_Pool::getInstance()->getObject();
		dragger->setEnable(dra_id++, touch_per_dragger, right_numbers[i], right_imgs_bg.at(i));
		dragger->setObjectSize(object_size);
		dragger->setComparisionValue(4);
		dragger->getImage()->setContentSize(cocos2d::Size(object_size * 1.1));
		dragger->getImage()->setPosition(cocos2d::Vec2(object_size.width * 0.6, object_size.height / 2));
		_slots[i]->setComparisionValue(dragger->getComparisionValue());

		if (kieu_hien_thi.compare("number") != 0) {
			dragger->setImageVisible(false);
			dragger->setValueText(dragger->getImageSource()->getText("name_1"));
			dragger->getLabelValue()->setFontSize(40);
			dragger->getLabelValue()->setPosition(cocos2d::Vec2(object_size.width * 0.6, object_size.height / 2));
		}
		if (kieu_sap_xep.compare("left-right") == 0) {
			dragger->getBackground()->setRotation(-90);
		}
		dragger->showValueTextWithAnimation(true);
		dragger->setMoveCallback(nullptr, CC_CALLBACK_2(My_MatchingNumber::onMoveObjectEnded, this));
		_draggers.push_back(dragger);
	}
	std::random_shuffle(_draggers.begin(), _draggers.end());

	//SHOW
	float delay = 0;
	//#1. show drager
	if (kieu_sap_xep.compare("left-right") == 0) {
		math::func::setPositionList(_draggers, 1, cocos2d::Rect(0.6, 0, 0.2, 0.7));
	}
	else {
		math::func::setPositionList(_draggers, num_block, cocos2d::Rect(0, 0.2, 1, 0.2));
	}
	for (Math_Object* dragger : _draggers) {
		dragger->onShow(delay);
		delay += 0.15;
		this->addChildNode(dragger, 1);
	}

	//#2. show object
	if (game_type != GameType::Image) {
		if (kieu_sap_xep.compare("left-right") == 0) {
			math::func::setPositionList(_objects, 1, cocos2d::Rect(0.2, 0, 0.2, 0.7));
		}
		else {
			math::func::setPositionList(_objects, num_block, cocos2d::Rect(0, 0.55, 1, 0.2));
		}
		for (Math_Object* object : _objects) {
			this->addChildNode(object, 2);
			object->onShow(delay);
			delay += 0.15;
		}
	}
	else {
		math::func::setPositionList(_groups, 1, cocos2d::Rect(0.2, 0, 0.2, 0.7));
		for (Math_Group* group : _groups) {
			addChildNode(group, 2);
			group->onShow(delay);
			delay += 0.15;
		}
	}

	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create([&]() {
		//#3. show slot
		if (kieu_sap_xep.compare("left-right") == 0) {
			float xx = (quiz_size.width * 0.3 + object_size.width * 0.28) / quiz_size.width;
			float rect_x = object_size.width / quiz_size.width;
			math::func::setPositionList(_slots, 1, cocos2d::Rect(xx, 0, rect_x, 0.7));
		}
		else {
			math::func::setPositionList(_slots, num_block, cocos2d::Rect(0, 0.432, 1, (float)(object_size.height / 750)));
		}

		for (Math_Slot* slot : _slots) {
			slot->onShow(0);
			this->addChildNode(slot, 0);
		}
	}), NULL));

	//Show Question
	showQuestionBox();
	disableControlQuiz(3);
}

void My_MatchingNumber::startQuestion() {
	for (Math_Object* dragger : _draggers) {
		dragger->enableDrag(_panel);
	}
}

void My_MatchingNumber::onTouchedObject(cocos2d::Node* node) {
	CCLOG("Touched dragger!");
	Math_Object* object = (Math_Object*)node;
	originPoint.x = object->getPosition().x;
	originPoint.y = object->getPosition().y;
}

void My_MatchingNumber::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object *object = (Math_Object*)node;

	cocos2d::Vec2 pos = _panel->convertToNodeSpace(point);
	for (int i = 0; i < _slots.size(); i++) {
		if (_slots.at(i)->getLinker() == object) {
			_slots.at(i)->removeLinker();
			count--;
			break;
		}
	}
	for (int i = 0; i < _slots.size(); i++) {
		//cocos2d::Rect rectObj = _objects.at(i)->getBoundingBox();
		if (_slots.at(i)->isAvailable(pos) /*|| rectObj.containsPoint(pos)) */ && !_slots.at(i)->isUsed()) {
			auto action = cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.2, _panel->convertToWorldSpace(_slots.at(i)->getPosition())));
			object->runAction(action);
			_slots.at(i)->setLinker(object);
			count++;
			return;
		}
	}
	object->backToStart();
}

void My_MatchingNumber::onExplanation(bool correct) {
	clickable_done = false;
	if (!correct) {
		for (int i = 0; i < num_block; i++) {
			if (!check(i)) {
				_slots.at(i)->getLinker()->backToStart();
				_slots.at(i)->removeLinker();
				count--;
			}
		}
	}
	else {
		this->nextQuestion();
	}
}

void My_MatchingNumber::onComplete() {
	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	m_delay = delay - 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
		m_delay += 0.15;
	}

	float dl = 0;
	for (Math_Object* drag : _draggers) {
		drag->onHide(dl);
		dl += 0.15;
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

bool My_MatchingNumber::check(int i) {
	if (_slots.at(i)->isCorrect(math::comparision::number)) return true;

	return false;
}

bool My_MatchingNumber::checkTest(int i, int j) {
	if (game_type == GameType::Sum) {
		if (_draggers.at(i)->getImageSource()->getComparisionValue("number") + _objects.at(j)->getImageSource()->getComparisionValue("number") != num_request) {
			return false;
		}
	}
	else if (game_type == GameType::Sub) {
		if (abs(_draggers.at(i)->getImageSource()->getComparisionValue("number") - _objects.at(j)->getImageSource()->getComparisionValue("number")) != num_request) {
			return false;
		}
	}
	else {
		if (_draggers.at(i)->getImageSource()->getComparisionValue("number") != _objects.at(j)->getImageSource()->getComparisionValue("number")) {
			return false;
		}
	}

	return true;
}

bool My_MatchingNumber::check() {
	for (int i = 0; i < num_block; i++) {
		if (!check(i)) {
			return false;
		}
	}

	return true;
}

void My_MatchingNumber::onAnswerDone(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
	if (count == num_block) {
		clickable_done = true;
	}

	bool done = true;

	if (clickable_done) {
		clickable_done = false;
		done = this->check();

		this->onExplanation(done);
		answer->onAnimationClick(done);
	}
}

void My_MatchingNumber::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, 3, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void My_MatchingNumber::autoTestingRun(float delaytime) {
	if (count == number_object) {
		count = 0;
		for (int i = 0; i < number_object; i++) {
			_slots.at(i)->getLinker()->backToStart();
			_slots.at(i)->removeLinker();
		}
	}

	int x;
	do {
		x = cocos2d::random(0, number_object - 1);
	} while (_slots.at(x)->getLinker() != NULL);

	_draggers.at(count)->runAction(cocos2d::MoveTo::create(0.4, _slots.at(x)->getPosition()));
	_slots.at(x)->setLinker(_draggers.at(count));
	count++;

	if (count == number_object) {
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			_answers.at(0)->onAnimationClick(check());
		}), NULL));

		if (check()) {
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(2), cocos2d::CallFunc::create([=]() {
				this->nextQuestion();
			}), NULL));
		}
	}

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(delaytime);
	}, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
}
