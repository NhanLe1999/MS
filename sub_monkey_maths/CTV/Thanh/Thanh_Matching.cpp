//
//  Thanh_Matching.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_Matching.h"
#include "Math_Libs.h"
#define color_1 cocos2d::Color4F(cocos2d::Color3B(138, 104, 21))
#define color_2 cocos2d::Color4F(cocos2d::Color3B(47, 58, 62))
#define line_width 6

Thanh_Matching* Thanh_Matching::createQuiz()
{
	Thanh_Matching* quiz = new Thanh_Matching();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_Matching::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_Matching::initQuiz(mj::model::Game game)
{
	CCLOG("%s override", __func__);

	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: Game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	std::string str_key = json_game.FindMember("key_compare")->value.GetString();
	if (str_key == "total") {
		_mode = GameMode::total;
	}
	else {
		_mode = GameMode::same;
		if (str_key == "kind") {
			_key_comparision = math::comparision::Key::kind;
		}
		else if (str_key == "name") {
			_key_comparision = math::comparision::Key::name;
		}
		else if (str_key == "family") {
			_key_comparision = math::comparision::Key::family;
		}
		else if (str_key == "color") {
			_key_comparision = math::comparision::Key::color;
		}
		else if (str_key == "number") {
			_key_comparision = math::comparision::Key::number;
		}
	}

	number_pair = 3;

	rapidjson::Value &json_size = json_game.FindMember("size")->value;
	object_size = cocos2d::Size(json_size.FindMember("object")->value.FindMember("width")->value.GetInt(), json_size.FindMember("object")->value.FindMember("height")->value.GetInt());
	slot_size = cocos2d::Size(json_size.FindMember("slot")->value.FindMember("width")->value.GetInt(), json_size.FindMember("slot")->value.FindMember("height")->value.GetInt());

	rapidjson::Value &json_bg = json_game.FindMember("background")->value;
	str = json_bg.FindMember("object")->value.GetString();
	object_background = math::resource::ResHelper::createImage(getUIPath("gameui/match/box.png"));
	str = json_bg.FindMember("slot")->value.GetString();
	slot_background = slotBG();

	max_number = json_game.FindMember("max_number")->value.GetInt();

	//TODO: OBJECT CONFIG
	rapidjson::Value& json_object = json_data.FindMember("object")->value;

	object_reuse = json_object.FindMember("resource_reuse")->value.GetBool();

	str = json_object.FindMember("object_1")->value.FindMember("display")->value.GetString();
	if (str == "name") object_option_1 = ObjectOption::text;
	else if (str == "number") object_option_1 = ObjectOption::number;
	else if (str == "icon") object_option_1 = ObjectOption::image;
	str = json_object.FindMember("object_2")->value.FindMember("display")->value.GetString();
	if (str == "name") object_option_2 = ObjectOption::text;
	else if (str == "number") object_option_2 = ObjectOption::number;
	else if (str == "icon") object_option_2 = ObjectOption::image;

	//TODO:
	//lay tat ca icon trong media_set
	std::vector<math::resource::Image*> icons; 
	if (object_option_1 == ObjectOption::image) {
		icons = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
	}
	else {
		icons = getNumbers(0, max_number); 
	}

	if (_mode == GameMode::total) {
		equal = cocos2d::random(4, max_number);
		bool *ok;
		ok = new bool[max_number];
		for (int i = 0; i < max_number; i++) ok[i] = false;

		for (int i = 0; i < number_pair; i++)
		{
			int number = cocos2d::random(1, equal - 1);
			while (ok[number])
			{
				number = cocos2d::random(1, equal - 1);
			}
			values_a.push_back(number);
			values_b.push_back(equal - number);
			ok[number] = true;
		}
		for (int i = 0; i < values_a.size(); i++) {
			for (math::resource::Image *img : icons) {
				if (img->getComparisionValue("number") == values_a[i]) {
					icons_1.push_back(img);
					break;
				}
			}
		}
		for (int i = 0; i < values_b.size(); i++) {
			for (math::resource::Image *img : icons) {
				if (img->getComparisionValue("number") == values_b[i]) {
					icons_2.push_back(img);
					break;
				}
			}
		}
	}
	else {
		std::vector<math::resource::Image*> icons_11, icons_22;
		if (object_reuse) {
			for (math::resource::Image *img : icons) {
				icons_11.push_back(img);
				icons_22.push_back(img);
			}
		}
		else {
			for (math::resource::Image *img : icons) {
				if (img->getComparisionValue("role") == 0) {
					icons_11.push_back(img);
				}
				else if (img->getComparisionValue("role") == 1) {
					icons_22.push_back(img);
				}
			}
		}
		math::func::shuffle(icons_11);
		for (int i = 0; i < number_pair; i++) {
			icons_1.push_back(icons_11[i]);
			for (int j = 0; j < icons_22.size(); j++) {
				if (icons_11[i]->getComparisionValue(str_key) == icons_22[j]->getComparisionValue(str_key)) {
					icons_2.push_back(icons_22[j]);
					break;
				}
			}
		}
	}
	/*---> Lay xong data cua game*/

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	// optional 
	auto _icons_question = getNumbers(equal, equal);
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : _icons_question) {
				auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : _icons_question) {
				auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else {
			getQuestion()->setOptionalTexts(key, _icons_question);
		}
	}
	/*
	 if (random_sorted) {
	 random_shuffle(list_questiondata.begin(), list_questiondata.end());
	 }
	 */
	// gen cot a
	//Gen doi tuong
	obj_id = 0;
	for (math::resource::Image *img : icons_1)
	{
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::TouchPermission::touch, img, object_background);
		object->setObjectSize(object_size);
		switch (object_option_1) {
		case ObjectOption::text:
		{
			object->setImageVisible(false);
			object->setValueText(img->getText("name_n", m_lang->getLanguage()));
			object->fitValueTextSize();
			object->showValueTextWithAnimation(true);
			break;
		}
		case ObjectOption::number:
		{
			object->setImageVisible(false);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->fitValueTextSize();
			object->showValueTextWithAnimation(true);
			object->getLabelValue()->setFontSize(object_size.height*0.6);
			break;
		}

		default:
			break;
		}

		_objects.push_back(object);
		objects.push_back(object);
		status.push_back(1);
	}
	// gen cot b
	for (math::resource::Image *img : icons_2)
	{
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::TouchPermission::touch, img, object_background);
		object->setObjectSize(object_size);
		switch (object_option_1) {
		case ObjectOption::text:
		{
			object->setImageVisible(false);
			object->setValueText(img->getText("name_n", m_lang->getLanguage()));
			object->fitValueTextSize();
			object->showValueTextWithAnimation(true);
			break;
		}
		case ObjectOption::number:
		{
			object->setImageVisible(false);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->fitValueTextSize();
			object->showValueTextWithAnimation(true);
			object->getLabelValue()->setFontSize(object_size.height*0.6);
			break;
		}

		default:
			break;
		}

		object->getLabelValue()->disableEffect();
		_objects.push_back(object);
		objects.push_back(object);
		status.push_back(1);
	}
}

void Thanh_Matching::onStart() {
	CCLOG("%s override", __func__);

	std::vector<cocos2d::Vec2> positions = randomPositionsInRect(object_size, cocos2d::Rect(50, 50, _panel->getContentSize().width - 100, 600), (int)objects.size());
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setPosition(positions[i]);
	}

	// gen slot
	for (int i = 0; i < number_pair; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(obj_id++, slot_background);
		slot->setSlotSize(slot_size);
		_slots.push_back(slot);
		slots.push_back(slot);
	}
	math::func::setPositionList(slots, number_pair, cocos2d::Rect(0, 0.2, 0.3, 0.8)); 

	//Show
	float delay = 0;
	for (Math_Object* object : objects) {
		addChildNode(object, 2);
		object->onShow(delay);
	}  

	//Gen question
	showQuestionBox();

	// touch
	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(Thanh_Matching::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(Thanh_Matching::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(Thanh_Matching::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(Thanh_Matching::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

	/**
	 * gen draw
	 * gen màu sắc
	 * random màu
	 **/
	for (int i = 0; i < objects.size(); i++)
	{
		cocos2d::DrawNode *draw = cocos2d::DrawNode::create();
		drawer.push_back(draw);
		this->addChildNode(draw, 1);
		draw->setPosition(cocos2d::Vec2(0, 0));
	}
}

void Thanh_Matching::startQuestion() {
	CCLOG("%s override", __func__);

	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();

		touch_disable = false;
	}
}

void Thanh_Matching::onExplanation(bool correct) {
	CCLOG("%s override", __func__);

	if (correct) {
		for (cocos2d::DrawNode *dr : drawer)
		{
			dr->clear();
		}
		btn_done->disableClickAnswer();
		this->nextQuestion();
	}
}

void Thanh_Matching::onComplete() {
	CCLOG("%s override", __func__);

	//TODO: Thuc hien them cac lenh o day
	//Math_Pool::getInstance()->resetAnswers();

	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	m_delay = delay - 0.3;
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
	}
	
	for( cocos2d::DrawNode *dr : drawer ) {
		dr->runAction( cocos2d::Sequence::create( cocos2d::DelayTime::create( m_delay - 0.4 ), cocos2d::FadeOut::create( 0.4 ), cocos2d::CallFunc::create( [=]() {
			dr->removeFromParentAndCleanup( true );
		} ), nullptr ) );
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Thanh_Matching::onAnswerDone(bool correct)
{
	bool done = true;

	for (int i = 0; i < number_pair; i++)
	{
		if (status[i] == 1) done = false;
	}

	this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_Matching::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_Matching::autoTestingRun(float dt) {

}

#pragma mark - Add Event
bool Thanh_Matching::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (touch_disable) return false;
	CCLOG("%s override", __func__);
	/**
	 * không cho vào queue luôn khi touch mà đợi end mới cho vào
	 * nếu queue rỗng thì chạm vào object return true;
	 * các object được sắp xếp trong list node lần lượt là object/group ở cột a, b và c
	 * index/number_pair hay pass/number_pair là vị trí cột. 0 là cột a, 1 là cột b, 2 là cột c
	 **/

	onlyTouch = true;
	pass = -1;
	if (queue_obj.size() == 0)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (status[i] != 0 && isTouchedOnObject(touch->getLocation(), objects[i]))
			{
				index = i;
				setSelected(true, objects[index]);
				return true;
			}
		}
	}

	if (queue_obj.size() == 1)
	{
		index = queue_obj.front();
		if (isTouchedOnObject(touch->getLocation(), objects[index]))
		{
			setSelected(false, objects[index]);
			queue_obj.pop();
			return false;
		}

		for (int i = 0; i < objects.size(); i++)
		{
			if (status[i] != 0 && i != index && isTouchedOnObject(touch->getLocation(), objects[i]))
			{
				index = i;
				setSelected(true, objects[index]);
				return true;
			}
		}
	}

	CCLOG("play audio: Oh");
	return false;
}

void Thanh_Matching::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);

	if (isTouchedOnObject(touch->getLocation(), objects[index]) == false)
	{
		if (queue_obj.size() == 0)
		{
			float distance = cocos2d::ccpDistance(objects[index]->getPosition(), touch->getLocation());
			int sodoan = distance / 30;
			drawer[index]->clear();
			drawer[index]->setLocalZOrder(3);
			drawer[index]->setLineWidth(line_width);
			// lấy vị trí để đặt điểm vẽ
			cocos2d::Vec2 position = objects[index]->getPosition();
			// ve 2 dau cham dau day cho dep
			drawer[index]->drawDot(position, 10 * line_width / 2, color_1);
			drawer[index]->drawDot(_panel->convertToNodeSpace(touch->getLocation()), line_width / 2, color_2);
			// day noi thang
			auto pos1 = objects[index]->getPosition();
			auto pos2 = objects[index]->getPosition() - (objects[index]->getPosition() - touch->getLocation()) / sodoan;
			for (int i = 0; i < sodoan + 1; i++) {
				drawer[index]->drawLine(pos1, pos2, i % 2 == 0 ? color_1 : color_2);
				pos1 = objects[index]->getPosition() - i*(objects[index]->getPosition() - touch->getLocation()) / sodoan;
				pos2 = objects[index]->getPosition() - (i + 1)*(objects[index]->getPosition() - touch->getLocation()) / sodoan;
				CCLOG("%f %f %f %f", pos1.x, pos1.y, pos2.x, pos2.y);
			} 

			// nếu trước đó có move vào một đối tượng mà giờ k còn trong đối tượng đó thì selected = false và pass = -1, pass là vị trí đối tượng move vào
			if (pass >= 0 && isTouchedOnObject(touch->getLocation(), objects[pass]) == false)
			{
				setSelected(false, objects[pass]);
				pass = -1;
			}

			// selected đối tượng được chọn hoặc được move vào
			for (int i = 0; i < objects.size(); i++)
			{
				if (status[i] != 0 && i != index && isTouchedOnObject(touch->getLocation(), objects[i]))
				{
					setSelected(true, objects[i]);
					pass = i;
				}
			}
		}
		onlyTouch = false;
	}
}
void Thanh_Matching::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);

	if (onlyTouch) {
		// oblytouch == true tức là nó chỉ là touch hoặc move trong object(coi là touch)
		queue_obj.push(index);
		if (queue_obj.size() == 2)
		{
			int i, j;
			i = queue_obj.front(); queue_obj.pop();
			j = queue_obj.front(); queue_obj.pop();
			setSelected(false, objects[i]);
			setSelected(false, objects[j]);
			if (isCorrect(i, j) == true) connect(i, j);
		}
	}
	else {
		// onlytouch == false là move
		if (pass >= 0 && pass != index && isCorrect(index, pass))
		{
			setSelected(false, objects[index]);
			setSelected(false, objects[pass]);
			connect(index, pass);
			return;
		}
		drawer[index]->clear();
		drawer[index]->setLocalZOrder(1);
		setSelected(false, objects[index]);
		if (pass >= 0) setSelected(false, objects[pass]);
	}
}
void Thanh_Matching::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) {
	CCLOG("%s override", __func__);

	drawer[index]->clear();
	setSelected(false, objects[index]);
	if (pass >= 0) {
		setSelected(false, objects[pass]);
		pass = -1;
	}
}

bool Thanh_Matching::isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node *object) {
	cocos2d::Vec2 locationInNode = object->convertToNodeSpace(position);
	cocos2d::Size size = object->getBoundingBox().size;
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

	if (rect.containsPoint(locationInNode)) return true;
	return false;
}

#pragma mark - game
bool Thanh_Matching::isCorrect(int i, int j)
{
	switch (_mode) {
	case GameMode::total:
	{
		int a = objects[i]->getComparisionValue().number;
		int b = objects[j]->getComparisionValue().number;
		return a + b == equal; 
	}
	case same: {
		return objects[i]->isEqual(objects[j], _key_comparision);
	}

	default:
		return false;
	}
}

#pragma mark - Value & Postion Object 

#pragma mark - Connect Option
void Thanh_Matching::connect(int i, int j)
{
	// lock object & clear draw
	status[i]--;
	status[j]--;
	drawer[index]->clear();
	if (i / number_pair == 1) CC_SWAP(i, j, int);
	cocos2d::Vec2 posA = objects[i]->getPosition();
	cocos2d::Vec2 posB = objects[j]->getPosition();

	float distance = cocos2d::ccpDistance(posA, posB);
	int sodoan = distance / 30;
	// lấy vị trí để đặt điểm vẽ
	cocos2d::Vec2 position = objects[index]->getPosition();
	// ve 2 dau cham dau day cho dep
	drawer[index]->drawDot(posA, line_width / 2, color_2);
	drawer[index]->drawDot(posB, line_width / 2, color_2);
	// day noi thang
	auto pos1 = posA;
	auto pos2 = posA - (posA - posB) / sodoan;
	for (int i = 0; i < sodoan + 1; i++) {
		drawer[index]->drawLine(pos1, pos2, i % 2 == 0 ? color_1 : color_2);
		pos1 = posA - i*(posA - posB) / sodoan;
		pos2 = posA - (i + 1)*(posA - posB) / sodoan; 
	}
	drawer[index]->setLineWidth(line_width);
	drawer[index]->setLocalZOrder(0);   

	// end game
	bool done = true;
	for (int i = 0; i < status.size(); i++) {
		if (status[i] != 0) done = false;
	}
	if (done) nextQuestion();

	//Math_Slot *slot;
	//for (int i = slots.size() - 1; i >= 0; i--)
	//{
	//	if (slots[i]->isUsed() == false) slot = slots[i];
	//}


	///*
	// - lấy trung điểm và chọn vị trí của 2 object sẽ moveto den
	// */
	//cocos2d::Vec2 avg = cocos2d::Vec2((posA + posB) / 2); //trung diem
	//cocos2d::Vec2 pos1, pos2;
	//if (posA.x > posB.x)
	//{
	//	pos1 = cocos2d::Vec2(object_size.width / 2, 0);
	//	pos2 = cocos2d::Vec2(-object_size.width / 2, 0);
	//}
	//else
	//{
	//	pos1 = cocos2d::Vec2(-object_size.width / 2, 0);
	//	pos2 = cocos2d::Vec2(object_size.width / 2, 0);
	//}

	///*
	// - cho 2 đối tượng vao 1 node chung pair_node
	// - set position trong pair trước khi di chuyển 2 object
	// */
	//cocos2d::Node *pair_node = cocos2d::Node::create();
	//pair_node->setPosition(avg);
	//this->addChildNode(pair_node, 3);

	//objects[i]->removeFromParent();
	//pair_node->addChild(objects[i]);
	//objects[i]->setPosition(pair_node->convertToNodeSpace(_panel->convertToWorldSpace(objects[i]->getPosition())));

	//objects[j]->removeFromParent();
	//pair_node->addChild(objects[j]);
	//objects[j]->setPosition(pair_node->convertToNodeSpace(_panel->convertToWorldSpace(objects[j]->getPosition())));

	////move 2 đối tượng vào cạnh nhau, tính thời gian move
	//float time_move_a = posA.distance(posB) / 1500;
	//if (time_move_a > 0.7)
	//	time_move_a = 0.7;
	//else if (time_move_a < 0.3)
	//	time_move_a = 0.3;

	//auto move_i = cocos2d::MoveTo::create(time_move_a, pos1);
	//objects[i]->runAction(move_i);
	//auto move_j = cocos2d::MoveTo::create(time_move_a, pos2);
	//objects[j]->runAction(move_j);

	//// chon slot, move pair to slot, tính thời gian move to slot
	//auto callB = cocos2d::CallFunc::create([this, pair_node, slot, i, j]() {
	//	float time_move_b = pair_node->getPosition().distance(slot->getPosition()) / 1500;
	//	if (time_move_b > 1)
	//		time_move_b = 1;
	//	if (time_move_b < 0.5)
	//		time_move_b = 0.5;

	//	auto scale = cocos2d::ScaleTo::create(time_move_b * 2, slot_size.height / object_size.height);
	//	auto move = cocos2d::MoveTo::create(time_move_b * 2, slot->getPosition());
	//	pair_node->runAction(scale);
	//	pair_node->runAction(cocos2d::EaseElasticOut::create(move, 1));
	//	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(time_move_b), cocos2d::CallFunc::create([this, i, j]() {
	//		objects[i]->setBackgroundVisible(true);
	//		objects[j]->setBackgroundVisible(true);
	//	}), NULL));
	//});

	//auto sequence = cocos2d::Sequence::create(cocos2d::DelayTime::create(time_move_a + 0.5), callB, NULL);
	//this->runAction(sequence);
	//slot->setLinker(objects[i]);
	//pair_node->setLocalZOrder(2);
}
void Thanh_Matching::setSelected(bool selected, Math_Object* object)
{
	object->setSelected(selected);
	if (selected == true) object->setLocalZOrder(4);
	else object->setLocalZOrder(2);
}
