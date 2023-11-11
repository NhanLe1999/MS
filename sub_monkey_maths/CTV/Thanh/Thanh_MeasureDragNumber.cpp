//
//  Thanh_MeasureDragNumber.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//. Measure and drag numbers into corresponding height objects

#include "Thanh_MeasureDragNumber.h"
#include "Math_Libs.h"

Thanh_MeasureDragNumber* Thanh_MeasureDragNumber::createQuiz()
{
	Thanh_MeasureDragNumber* quiz = new Thanh_MeasureDragNumber();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_MeasureDragNumber::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_MeasureDragNumber::initQuiz(mj::model::Game game)
{

	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	str = json_game.FindMember("option_game")->value.GetString();
	if (str == "measure_leng") option_game = OptionGame::measure_leng;
	else if (str == "measure_height") option_game = OptionGame::measure_height;

	sapxep = json_game.FindMember("sapxep")->value.GetString();

	_mode_question = "";
	rapidjson::Value &json_question_list = json_game.FindMember("question")->value;
	for (rapidjson::SizeType i = 0; i < json_question_list.Size(); i++) {
		str = json_question_list[i].GetString();
		tasks.push_back(str);
		if (str == "touch_choose_object_largest_smallest") {
			_mode_question = json_game.FindMember("mode_question")->value.GetString();
			if (_mode_question == "random") {
				if (cocos2d::random(0, 1)) {
					_mode_question = "heightest_lengest";
				}
				else {
					_mode_question = "shortest";
				}
			}
		}
	}
	next_task = false;
	index_question = 0;
	task = tasks[index_question];
	std::string icon_config = json_game.FindMember("icon")->value.GetString();
	 
	//TODO: OBJECT (NUMBER) CONFIG
	rapidjson::Value& json_number = json_data.FindMember("number")->value;
	number_size = cocos2d::Size(json_number.FindMember("size")->value.FindMember("width")->value.GetInt(), json_number.FindMember("size")->value.FindMember("height")->value.GetInt());

	str = json_number.FindMember("display")->value.GetString();
	if (str == "number") number_option = ObjectOption::number;
	else if (str == "number_text") number_option = ObjectOption::number_text;

	number_bg = math::resource::ResHelper::createImage(getUIPath("themeui/question.png"));

	//TODO: SLOT CONFIG
	rapidjson::Value& json_slot = json_data.FindMember("slot")->value;
	slot_size = cocos2d::Size(json_slot.FindMember("size")->value.FindMember("width")->value.GetInt() - 5, json_slot.FindMember("size")->value.FindMember("height")->value.GetInt() - 5);

	slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));

	//TODO:
	//lay tat ca icon trong media_set

	std::random_device rd;
	std::mt19937 g(rd());
	std::vector < math::resource::Image*> list_icons, list_ruler;
	auto list_icon_object = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
	for (math::resource::Image *img : list_icon_object) {
		if (img->getComparisionValue("role") == 1) {
			list_ruler.push_back(img);
		}
		else if (img->getComparisionValue("role") == 0) {
			list_icons.push_back(img);
		}
	}
	if (list_ruler.size() > 0) {
		icon_ruler_drag = list_ruler[cocos2d::random(0, (int)list_ruler.size() - 1)];
	}
	else {
		if (option_game == OptionGame::measure_height) {
			icon_ruler_drag = math::resource::ResHelper::createImage(getUIPath("common/ruler/height.png"));
		}
		else {
			icon_ruler_drag = math::resource::ResHelper::createImage(getUIPath("common/ruler/width.png"));
		}
	}
	if (list_icons.size() == 0) {
		// nếu config có sử dụng role		
		for (math::resource::Image *img : list_icon_object) {
			list_icons.push_back(img);
		}
	}
	math::resource::Image *icon = list_icons[cocos2d::random(0, (int)list_icons.size() - 1)];
	if (icon_config == "pair") {
		// su dụng pair
		list_icons.clear();
		for (math::resource::Image *img : list_icon_object) {
			if (img->getComparisionValue("pair") == icon->getComparisionValue("pair")) {
				list_icons.push_back(img);
			}
		}
	}
	else {
		// không dùng pair
		for (math::resource::Image *img : list_icon_object) {
			list_icons.push_back(img);
		}
	}
	int index = 0;
	for (int i = 0; i < list_icons.size(); i++) {
		int value = list_icons[i]->getComparisionValue("number");
		bool ok = true;
		for (int j = 0; j < i; j++) {
			if (value == list_icons[j]->getComparisionValue("number")) {
				ok = false;
				break;
			}
		}
		if (ok) {
			index++;
			icons_object.push_back(list_icons[i]);
			icons_number_drag.push_back(getNumbers(value, value).at(0));
			if (index == 3) break;
		}
	}

	std::shuffle(icons_object.begin(), icons_object.end(), g);
	std::shuffle(icons_number_drag.begin(), icons_number_drag.end(), g);
	std::vector<math::resource::Image*> list_img;
	list_img.push_back(icons_object[0]);

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	// optional
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : list_img) {
				auto img_txt = img->getTextValue("name_1	", m_lang->getLanguage());
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : list_img) {
				auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else {
			getQuestion()->setOptionalTexts(key, list_img);
		}
	}
	// keyword
	for (int i = 0; i < tasks.size(); i++) {
		if (tasks[i] == "touch_choose_object_largest_smallest") {
			if (_mode_question == "shortest" && option_game == OptionGame::measure_height) {
				getQuestion()->setKeywordValue(0);
			}
			else if (_mode_question == "heightest_lengest" && option_game == OptionGame::measure_height) {
				getQuestion()->setKeywordValue(1);
			}
			else if(_mode_question == "shortest" && option_game == OptionGame::measure_leng) {
				getQuestion()->setKeywordValue(2);
			}
			else {
				getQuestion()->setKeywordValue(3);
			}
		}
	}
	/*
	 if (random_sorted) {
	 random_shuffle(list_questiondata.begin(), list_questiondata.end());
	 }
	 */

	obj_id = 0;

	if (option_game == OptionGame::measure_height)
	{
		// TODO: MEASURE HEIGHT OPTION
		// TODO: gen ruler drag
		ruler_drag = Math_Pool::getInstance()->getObject();
		ruler_drag->setEnable(obj_id++, math::common::deny, icon_ruler_drag, number_bg);
		ruler_drag->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this), CC_CALLBACK_2(Thanh_MeasureDragNumber::onMoveObjectEnded, this));
		ruler_drag->setObjectSize((cocos2d::ui::ImageView::create(icon_ruler_drag->path))->getContentSize());
		scale = 1;
		if (ruler_drag->getContentSize().height > 650.0f)
		{
			scale = 650.0f / ruler_drag->getContentSize().height;
		}
		CCLOG("%f %f dsgdjgsajhg", (cocos2d::ui::ImageView::create(icon_ruler_drag->path))->getContentSize().width, (cocos2d::ui::ImageView::create(icon_ruler_drag->path))->getContentSize().height);
		ruler_drag->setObjectSize(cocos2d::Size(ruler_drag->getContentSize().width*scale, ruler_drag->getContentSize().height*scale));
		ruler_drag->setMoveDirection(1);
		ruler_drag->setBackgroundVisible(false);
		ruler_drag->setImageVisible(true);
		_objects.push_back(ruler_drag);

		scale *= 2;
		// TODO: gen object
		for (math::resource::Image *img : icons_object)
		{
			// object
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::touch, img, number_bg);
			auto imageView = cocos2d::ui::ImageView::create(img->path);
			object->setTouchCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this));
			object->setObjectSize(imageView->getContentSize()*scale);
			CCLOG("%f %f", imageView->getContentSize().width, imageView->getContentSize().height);
			object->setBackgroundVisible(false);
			object->setImageVisible(true);
			object->getLabelValue()->disableEffect();
			_objects.push_back(object);
			objects.push_back(object);
		}
		if (sapxep == "random") {
			std::shuffle(objects.begin(), objects.end(), g);
		}
		else if (sapxep == "tangdan") {
			for (int ii = 0; ii < objects.size(); ii++) {
				for (int jj = ii + 1; jj < objects.size(); jj++) {
					if (objects[ii]->getComparisionValue().number > objects[jj]->getComparisionValue().number) {
						std::swap(objects[ii], objects[jj]);
					}
				}
			}
		}
		else {
			for (int ii = 0; ii < objects.size(); ii++) {
				for (int jj = ii + 1; jj < objects.size(); jj++) {
					if (objects[ii]->getComparisionValue().number < objects[jj]->getComparisionValue().number) {
						std::swap(objects[ii], objects[jj]);
					}
				}
			}
		}

		// TODO: gen slot
		for (int i = 0; i < 3; i++)
		{
			// slot
			Math_Slot *slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(obj_id++, slot_bg);
			slot->setSlotSize(slot_size);
			_slots.push_back(slot);
			slots.push_back(slot);
		}

		// TODO: gen number drag
		for (math::resource::Image* img : icons_number_drag)
		{
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, img, number_bg);
			object->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this), CC_CALLBACK_2(Thanh_MeasureDragNumber::onMoveObjectEnded, this));
			object->setObjectSize(number_size);
			object->setImageVisible(false);
			if (number_option == ObjectOption::number)
			{
				object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
				object->getLabelValue()->setFontSize(number_size.height*0.75);
			}
			else object->setValueText(img->getText("name_n", m_lang->getLanguage()));
			object->getLabelValue()->setTextColor(cocos2d::Color4B(44, 51, 144, 232));
			object->showValueTextWithAnimation(false);
			object->fitValueTextSize();
			object->getLabelValue()->disableEffect();
			_objects.push_back(object);
			numbers_drag.push_back(object);
		}
		std::shuffle(numbers_drag.begin(), numbers_drag.end(), g);
	}
	else if (option_game == OptionGame::measure_leng)
	{
		// TODO: MEASURE LENG OPTION
		// TODO: gen ruler drag
		ruler_drag = Math_Pool::getInstance()->getObject();
		ruler_drag->setEnable(obj_id++, math::common::deny, icon_ruler_drag, number_bg);
		ruler_drag->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this), CC_CALLBACK_2(Thanh_MeasureDragNumber::onMoveObjectEnded, this));
		ruler_drag->setObjectSize((cocos2d::ui::ImageView::create(icon_ruler_drag->path))->getContentSize());
		scale = 1;
		if (ruler_drag->getContentSize().width > 900.0f) {
			scale = 1000.0f / ruler_drag->getContentSize().width;
		}
		ruler_drag->setObjectSize(cocos2d::Size(ruler_drag->getContentSize().width*scale, ruler_drag->getContentSize().height*scale));
		ruler_drag->setMoveDirection(2);
		ruler_drag->setBackgroundVisible(false);
		ruler_drag->setImageVisible(true);
		_objects.push_back(ruler_drag);

		scale *= 2;
		// TODO: gen object // TODO: gen object
		for (math::resource::Image *img : icons_object)
		{
			// object
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::touch, img, number_bg);
			auto imageView = cocos2d::ui::ImageView::create(img->path);
			object->setTouchCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this));
			object->setObjectSize(imageView->getContentSize()*scale);
			object->setBackgroundVisible(false);
			object->setImageVisible(true);
			_objects.push_back(object);
			objects.push_back(object);
		}

		if (sapxep == "random") {
			std::shuffle(objects.begin(), objects.end(), g);
		}
		else if (sapxep == "tangdan") {
			for (int ii = 0; ii < objects.size(); ii++) {
				for (int jj = ii + 1; jj < objects.size(); jj++) {
					if (objects[ii]->getComparisionValue().number > objects[jj]->getComparisionValue().number) {
						std::swap(objects[ii], objects[jj]);
					}
				}
			}
		}
		else {
			for (int ii = 0; ii < objects.size(); ii++) {
				for (int jj = ii + 1; jj < objects.size(); jj++) {
					if (objects[ii]->getComparisionValue().number < objects[jj]->getComparisionValue().number) {
						std::swap(objects[ii], objects[jj]);
					}
				}
			}
		}

		// TODO: gen slot
		for (int i = 0; i < 3; i++)
		{
			// slot
			Math_Slot *slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(obj_id++, slot_bg);
			slot->setSlotSize(slot_size*0.95);
			slot->setBackground(slot_bg, 10);
			_slots.push_back(slot);
			slots.push_back(slot);
		}

		// TODO: gen number drag
		for (math::resource::Image* img : icons_number_drag)
		{
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, img, number_bg);
			object->setBackground(number_bg, 30);
			object->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragNumber::onTouchedObject, this), CC_CALLBACK_2(Thanh_MeasureDragNumber::onMoveObjectEnded, this));
			object->setObjectSize(number_size);
			object->setImageVisible(false);
			if (number_option == ObjectOption::number)
			{
				object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
				object->getLabelValue()->setFontSize(number_size.height*0.75);
			}
			else object->setValueText(img->getText("name_n", m_lang->getLanguage()));
			object->getLabelValue()->setTextColor(cocos2d::Color4B(44, 51, 144, 232));
			object->showValueTextWithAnimation(false);
			object->fitValueTextSize();
			object->getLabelValue()->disableEffect();
			_objects.push_back(object);
			numbers_drag.push_back(object);
		}
		std::shuffle(numbers_drag.begin(), numbers_drag.end(), g);
	}

	//Math_Pool::getInstance()->resetImages();
}

void Thanh_MeasureDragNumber::onStart() {
	// set position
	if (option_game == OptionGame::measure_leng) {
		// option leng
		// show ruler
		if( task != "touch_choose_object_largest_smallest" ) {
			ruler_drag->setPosition( cocos2d::Vec2( ruler_drag->getImage()->getContentSize().width / 2 + 118, 200 ) );
			this->addChildNode( ruler_drag, 3 );
			ruler_drag->onShow( 0 );
		}
		
		// show objects
		math::func::setPositionList(objects, 1, cocos2d::Rect(0, 0.35, 1, 0.435));
		for (Math_Object* object : objects) {
			object->setPositionX(ruler_drag->getPositionX() - ruler_drag->getImage()->getContentSize().width / 2 + object->getImage()->getContentSize().width / 2 + 63);
			this->addChildNode(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation, 0.5);
		}

		math::func::setPositionList(slots, 1, cocos2d::Rect(0.75, 0.35, 0.2, 0.435));
		math::func::setPositionList(numbers_drag, (int)numbers_drag.size(), cocos2d::Rect(0.2, 0.05, 0.6, 0.1));
		if (task == "drag_number_to_slot") {
			// show slots number drag
			for (Math_Slot* slot : slots) {
				slot->removeLinker();
				addChildNode(slot, 2);
				slot->onShow(0, math::animation::ScaleUpAndRotation);
			}
			for (Math_Object* object : numbers_drag) {
				addChildNode(object, 3);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
		}
		if( task == "touch_choose_object_largest_smallest" ) { 
			int index = 1;
			int max_leng = 0;
			for( Math_Object *obj : objects ) {
				if( obj->getContentSize().width > max_leng )
					max_leng = obj->getContentSize().width;
			}  
			for( Math_Object *obj : objects ) {
				obj->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2 - max_leng / 2 + obj->getContentSize().width / 2,
					( _panel->getContentSize().height - 100 ) *index / ( objects.size() + 1 ) ) );
				index++;
			}
		}
	}
	else {
		// option height
		if( task != "touch_choose_object_largest_smallest" ) {
			ruler_drag->setPosition( cocos2d::Vec2( 150, 193 + ruler_drag->getImage()->getContentSize().height / 2 - 100 ) );
			this->addChildNode( ruler_drag, 3 );
			ruler_drag->onShow( 0 ); 
		}
		math::func::setPositionList(objects, (int)objects.size(), cocos2d::Rect(0.2, 0.25, 0.8, 0.1));
		for (Math_Object* object : objects)
		{
			object->setPositionY(object->getImage()->getContentSize().height / 2 + 225 - 100);
			this->addChildNode(object, 3);
			object->onShow(0);
		}
		math::func::setPositionList(slots, (int)slots.size(), cocos2d::Rect(0.2, 0.17, 0.8, 0.1));
		math::func::setPositionList(numbers_drag, (int)numbers_drag.size(), cocos2d::Rect(0.325, 0.05, 0.55, 0.1));
		if (task == "drag_number_to_slot") {
			ruler_drag->setPosition(cocos2d::Vec2(150, 193 + ruler_drag->getImage()->getContentSize().height / 2));
			for (Math_Object *obj : objects) {
				obj->setPositionY(obj->getImage()->getContentSize().height / 2 + 225);
			}

			// show slots
			for (Math_Slot* slot : slots) {
				slot->removeLinker();
				addChildNode(slot, 2);
				slot->onShow(0, math::animation::ScaleUpAndRotation);
			}

			for (Math_Object* object : numbers_drag) {
				addChildNode(object, 3);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
		}
		if( task == "touch_choose_object_largest_smallest") {
			if( option_game == OptionGame::measure_height ) {
				int index = 1;
				for( Math_Object *obj : objects ) {
					obj->setPositionX( _panel->getContentSize().width*index / ( objects.size() + 1 ) );
					index++;
				}
			}   
		}
	}

	//--------------------
	//Gen button done
	btn_done = Math_Pool::getInstance()->getAnswer();
	btn_done->setEnable("Done", math::common::AnswerType::done, CC_CALLBACK_1(Thanh_MeasureDragNumber::onAnswerDone, this), this->getAnswerSize(3));
	btn_done->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, -100));
	btn_done->setVisible(false); 
	_answers.push_back(btn_done);
	this->scheduleUpdate();

	//Gen question
	showQuestionBox(0);
}

void Thanh_MeasureDragNumber::startQuestion() {

	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
		disable_touch = false;

		ruler_drag->enableDrag(_panel);

		for (Math_Object* object : numbers_drag) {
			object->enableDrag(_panel);
		}
		btn_done->setVisible(false);
		btn_done->onShowInParent(_panel, 3);
		for (int i = 0; i < objects.size(); i++) {
			task1.push_back(false);
		}
	}
}

void Thanh_MeasureDragNumber::onExplanation(bool correct) {
	if (correct) {
		btn_done->disableClickAnswer();
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			btn_done->setVisible(false);
		}), nullptr));

		if (task == "drag_number_to_slot") {
			index_question++;
			if (index_question < tasks.size()) {
				task = tasks[index_question];
				goToQuestionAtIndex(index_question);
				if (option_game == OptionGame::measure_height) { 
					int index = 1;
					for (Math_Object *obj : objects) {
						obj->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width*index/(objects.size() + 1), obj->getPosition().y - 100)));
						index++;
					}
				}
				else if( option_game == OptionGame::measure_leng ) {
					int index = 1;
					int max_leng = 0;
					for( Math_Object *obj : objects ) {
						if( obj->getContentSize().width > max_leng )
							max_leng = obj->getContentSize().width;
					}
					for( Math_Object *obj : objects ) {
						obj->runAction( cocos2d::MoveTo::create( 0.5, cocos2d::Vec2( _panel->getContentSize().width/2 - max_leng/2 + obj->getContentSize().width/2,
							( _panel->getContentSize().height - 100 )*index / ( objects.size() + 1 )) ) );
						index++;
					}
				}

				// hide slots
				for (Math_Slot* slot : slots) {
					slot->removeLinker();
					slot->onHide(0, math::animation::ScaleUpAndRotation);
				}
				// hide number drag
				for (Math_Object* object : numbers_drag) {
					object->onHide(0, math::animation::ScaleUpAndRotation);
				}
				// hide ruler
				ruler_drag->onHide( 0, math::animation::ScaleUpAndRotation );
				return;
			}
			else nextQuestion();
		}
	}
	else {
		btn_done->onAnimationClick(false);

		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([this]() {
			for (int i = 0; i < slots.size(); i++) {
				Math_Object *m_object = objects[i];
				Math_Slot *m_slot = slots[i];
				if (m_slot->getLinker() != nullptr && m_object->getComparisionValue().number != m_slot->getLinker()->getComparisionValue().number) {
					m_slot->getLinker()->backToStart();
					m_slot->removeLinker();
				}
			}
			btn_done->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width / 2, -100)));
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
				btn_done->setVisible(false);
			}), nullptr));
		}), nullptr));
	}
}

void Thanh_MeasureDragNumber::onComplete() {
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();;
		}
	}

	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	m_delay = delay - 0.3;
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=]() {
		//#1. hide ruler drag
		ruler_drag->onHide(0);
	}), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
		//#2. hide object
		for (Math_Object *object : objects) {
			object->onHide(0);
		}
	}), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
		//#3. hide slot
		for (Math_Slot* slot : slots) {
			slot->onHide(0);
		}
	}), cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=]() {
		//#4. hide number drag
		for (Math_Object* object : numbers_drag) {
			object->onHide(0);
		}
	}), nullptr));

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Thanh_MeasureDragNumber::onAnswerDone(bool correct)
{
	bool done = true;
	if (disable_touch) return;
	// kiểm tra giá trị trong slot
	for (int i = 0; i < slots.size(); i++) {
		Math_Object *m_object = objects[i];
		Math_Object *t_object = slots[i]->getLinker();
		if (t_object == nullptr || m_object->getComparisionValue().number != t_object->getComparisionValue().number) done = false;
	}
	onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_MeasureDragNumber::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Thanh_MeasureDragNumber::autoTestingRun(float dt)
{

}

#pragma mark - on this game
void Thanh_MeasureDragNumber::update(float dt) {
	if (task == "drag_ruler_to_measure_object") {
		if (current_object_measurement != nullptr) {
			cocos2d::Vec2 locationInNode;
			if (option_game == OptionGame::measure_height) {
				locationInNode = current_object_measurement->convertToNodeSpace(ruler_drag->getPosition() + cocos2d::Vec2(ruler_drag->getContentSize().width / 2, -250));
			}
			else {
				locationInNode = current_object_measurement->convertToNodeSpace(ruler_drag->getPosition() + cocos2d::Vec2(-ruler_drag->getImage()->getContentSize().width*0.4, ruler_drag->getImage()->getContentSize().height / 2));
			}
			cocos2d::Size size = current_object_measurement->getBoundingBox().size;
			cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				time_measurement += dt;
				if (time_measurement > 0.75) {
					for (int i = 0; i < objects.size(); i++) {
						if (current_object_measurement == objects[i]) {
							task1[i] = true;
							break;
						}
					}
					auto seq = cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.5, 1.1), cocos2d::ScaleTo::create(0.5, 1));
					current_object_measurement->runAction(cocos2d::RepeatForever::create(seq));
				}
				return;
			}
		}
		for (int i = 0; i < objects.size(); i++) {
			if (task1.size() == objects.size() && task1[i] == true) continue;
			Math_Object *object = objects[i];
			cocos2d::Vec2 locationInNode;
			if (option_game == OptionGame::measure_height) {
				locationInNode = object->convertToNodeSpace(ruler_drag->getPosition() + cocos2d::Vec2(ruler_drag->getContentSize().width / 2, -250));
			}
			else {
				locationInNode = object->convertToNodeSpace(ruler_drag->getPosition() + cocos2d::Vec2(-ruler_drag->getImage()->getContentSize().width*0.4, ruler_drag->getImage()->getContentSize().height / 2));
			}
			cocos2d::Size size = object->getBoundingBox().size;
			cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				if (object == current_object_measurement) {
					time_measurement += dt;
					if (time_measurement > 0.75) {
						task1[i] = true;
						auto seq = cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.5, 1.1), cocos2d::ScaleTo::create(0.5, 1));
						object->runAction(cocos2d::RepeatForever::create(seq));
					}
				}
				else {
					time_measurement = 0;
					current_object_measurement = object;
				}
				break;
			}
			else {
				time_measurement = 0;
				current_object_measurement = nullptr;
			}
		}
	}
}
void Thanh_MeasureDragNumber::onTouchedObject(cocos2d::Node* node)
{
	if (disable_touch) return;
	Math_Object* object = (Math_Object*)node;
	object->setLocalZOrder(4);

	if (task == "touch_choose_object_largest_smallest") {
		CCLOG("here");
		bool done = true;
		if (_mode_question == "heightest_lengest") {
			for (Math_Object *obj : objects) {
				if (obj->getComparisionValue().number > object->getComparisionValue().number) {
					done = false;
				}
			}
		}
		else if (_mode_question == "shortest") {
			for (Math_Object *obj : objects) {
				if (obj->getComparisionValue().number < object->getComparisionValue().number) {
					done = false;
				}
			}
		}
		if (done) {
			object->runAction(cocos2d::Repeat::create(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.25, 1.1), cocos2d::ScaleTo::create(0.25, 1), nullptr), 3));
			disable_touch = true;
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.5), cocos2d::CallFunc::create([=]() { 
				this->nextQuestion();
			}), nullptr));
		}
		else { 
			object->runAction( cocos2d::Sequence::createWithTwoActions( cocos2d::FadeOut::create( 0.25 ), cocos2d::FadeIn::create( 0.25 ) ) );
		}
	}
}

void Thanh_MeasureDragNumber::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
	if (disable_touch) return;
	Math_Object* object = (Math_Object*)node;
	this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=]() {
		for (Math_Object *obj : numbers_drag) {
			obj->setTouch(math::common::TouchPermission::deny);
		}
	}), cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=]() {
		for (Math_Object *obj : numbers_drag) {
			obj->setTouch(math::common::TouchPermission::move);
		}
	}), nullptr)); 
	if (object == ruler_drag) {
		if (task == "drag_ruler_to_measure_object") {
			bool done = true;
			for (int j = 0; j < task1.size(); j++) {
				if (task1[j] == false) done = false;
			}
			if (done) {
				index_question++;
				for (Math_Object *obj : objects) {
					obj->stopAllActions();
					obj->setScale(1);
				}
				if (index_question < tasks.size()) {
					task = tasks[index_question];
					goToQuestionAtIndex(index_question);
					if (task == "drag_number_to_slot" && option_game == OptionGame::measure_height) {
						disable_touch = true;
						ruler_drag->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(0, 100) + ruler_drag->getPosition()));
						this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.6), cocos2d::CallFunc::create([=]() {
							ruler_drag->updateRoot(_panel);
							disable_touch = false;
						}), nullptr));
						for (Math_Object *obj : objects) {
							obj->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(0, 100) + obj->getPosition()));
						}

						// show slots
						for (Math_Slot* slot : slots) {
							slot->removeLinker();
							this->addChildNode(slot, 2);
							slot->onShow(0, math::animation::ScaleUpAndRotation);
						}

						for (Math_Object* object : numbers_drag) {
							this->addChildNode(object, 3);
							object->onShow(0, math::animation::ScaleUpAndRotation);
						}
					}
					else if (task == "drag_number_to_slot" && option_game == OptionGame::measure_leng) {
						disable_touch = true;
						this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.6), cocos2d::CallFunc::create([=]() {
							ruler_drag->updateRoot(_panel);
							disable_touch = false;
						}), nullptr));
						// show slots
						for (Math_Slot* slot : slots) {
							slot->removeLinker();
							this->addChildNode(slot, 2);
							slot->onShow(0, math::animation::ScaleUpAndRotation);
						}

						for (Math_Object* object : numbers_drag) {
							this->addChildNode(object, 3);
							object->onShow(0, math::animation::ScaleUpAndRotation);
						}
					}
					else if( task == "touch_choose_object_largest_smallest" ) {
						if( option_game == OptionGame::measure_height ) {
							int index = 1;
							for( Math_Object *obj : objects ) {
								obj->runAction( cocos2d::MoveTo::create( 0.5, cocos2d::Vec2( _panel->getContentSize().width*index / ( objects.size() + 1), 0 ) ) );
								index++;
							}
						}else if( option_game == OptionGame::measure_leng ) { 
							int index = 1;
							int max_leng = 0;
							for( Math_Object *obj : objects ) {
								if( obj->getContentSize().width > max_leng )
									max_leng = obj->getContentSize().width;
							}
							for( Math_Object *obj : objects ) {
								obj->runAction( cocos2d::MoveTo::create( 0.5, cocos2d::Vec2( _panel->getContentSize().width / 2 - max_leng / 2 + obj->getContentSize().width/2,
																							 ( _panel->getContentSize().height -100)*index / ( objects.size() + 1 ) ) ) );
								index++;
							}
						}
						ruler_drag->onHide(0);
					}
				}
				else nextQuestion();
			}
		}
		object->backToStart();
		return;
	}

	object->removeFromParent();
	cocos2d::Vec2 position = _panel->convertToNodeSpace(point);
	object->setPosition(position);
	_panel->addChild(object);

	//Doi tuong keo tha nam trong 1 slot nao do.
	Math_Slot* m_slot = nullptr;
	for (Math_Slot* slot : _slots) {
		if (object->isSame(slot->getLinker())) {
			m_slot = slot;
			break;
		}
	}

	Math_Slot* t_slot = nullptr;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			t_slot = slot;
			break;
		}
	}

	if (m_slot != nullptr) {
		if (t_slot != nullptr) {
			//swap
			if (m_slot == t_slot) {
				t_slot->setLinker(object);
				object->runAction(cocos2d::ScaleTo::create(0.3, slot_size.width / object->getContentSize().width));
				object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			}
			else {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					m_slot->removeLinker();
					t_slot->removeLinker();

					t_slot->setLinker(object);
					object->runAction(cocos2d::ScaleTo::create(0.3, slot_size.width / object->getContentSize().width));
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

					m_slot->setLinker(linker);
					linker->runAction(cocos2d::ScaleTo::create(0.3, slot_size.width / linker->getContentSize().width));
					linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
				}
				else {
					m_slot->removeLinker();
					t_slot->setLinker(object);
					object->runAction(cocos2d::ScaleTo::create(0.3, slot_size.width / object->getContentSize().width));
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
				}
			}
		}
		else {
			//move to start
			m_slot->removeLinker();
			object->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / slot_size.width));
			object->setLocalZOrder(2);
			object->backToStart();
		}
	}
	else {
		if (t_slot != nullptr) {
			if (t_slot->isUsed()) {
				auto linker = t_slot->getLinker();
				t_slot->removeLinker();
				linker->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / slot_size.width));
				linker->setLocalZOrder(2);
				linker->backToStart();
			}

			t_slot->setLinker(object);
			object->runAction(cocos2d::ScaleTo::create(0.3, slot_size.width / object->getContentSize().width));
			object->setLocalZOrder(4);
			object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
		}
		else {
			//move start
			object->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / object->getContentSize().width));
			object->setLocalZOrder(2);
			object->backToStart();
		}
	}

	// on show on hide button done
	bool show_btn_done = true;
	for (Math_Slot *slot : slots) {
		if (slot->isUsed() == false) show_btn_done = false;
	}
	if (show_btn_done) {
		btn_done->setVisible(true);
		btn_done->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width / 2, 75)), 0.5));
	}
	else {
		btn_done->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width / 2, -100)));
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			btn_done->setVisible(false);
		}), nullptr));
	}
}

