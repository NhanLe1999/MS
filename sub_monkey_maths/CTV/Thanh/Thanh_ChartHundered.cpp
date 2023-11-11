//
//  Thanh_ChartHundered.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//. Move the numbers to the corresponding position in chart hundered
//. Fill color all cell of row with number 34

#include "Thanh_ChartHundered.h"
#include "Math_Libs.h" 

Thanh_ChartHundered* Thanh_ChartHundered::createQuiz()
{
	Thanh_ChartHundered* quiz = new Thanh_ChartHundered();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_ChartHundered::init() {
	return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle

void Thanh_ChartHundered::initQuiz(mj::model::Game game)
{
	CCLOG("%s override", __func__);

	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	str = json_game.FindMember("option_game")->value.GetString();
	if (str == "drag_number") option_game = OptionGame::drag_number;
	else if (str == "draw_row") option_game = OptionGame::draw_row;
	else if (str == "find_number") option_game = OptionGame::find_number;

	consecutive = json_game.FindMember("consecutive")->value.GetBool();
	number_miss = json_game.FindMember("number_miss")->value.GetInt();
	if (number_miss < 2) consecutive = false;
	number_result = json_game.FindMember("number_result")->value.GetInt();
	if (number_result < number_miss) number_result = number_miss;

	str = json_game.FindMember("touch_mode")->value.GetString();
	if (str == "only_row") {
		touch_mode = TouchMode::only_row;
	}
	else if (str == "only_column") {
		touch_mode = TouchMode::only_column;
	}
	else if (str == "only_block") {
		touch_mode = TouchMode::only_block;
	}
	else if (str == "cell_and_block") {
		touch_mode = TouchMode::cell_and_block;
	}
	else if (str == "only_cell") {
		touch_mode = TouchMode::only_cell;
	}
	enable_sound = json_game.FindMember("read")->value.GetBool();

	//TODO: OBJECT CONFIG
	rapidjson::Value& json_number = json_data.FindMember("number")->value;
	number_size = cocos2d::Size(json_number.FindMember("size")->value.FindMember("width")->value.GetInt(), json_number.FindMember("size")->value.FindMember("height")->value.GetInt());

	//TODO: CHART CONFIG
	rapidjson::Value& json_chart = json_data.FindMember("chart")->value;
	int first_number = json_chart.FindMember("first_number")->value.GetInt();
	str = json_chart.FindMember("option")->value.GetString();
	if (str == "full_chart") {
		option_chart = OptionChart::full_chart;
		min_number = first_number;
		max_number = first_number + 99;
		number_row = 10;
		number_column = 10;
		cell_size = cocos2d::Size(550 / number_column, 550 / number_row);
	}
	else if (str == "half_before") {
		option_chart = OptionChart::half_chart;
		min_number = first_number;
		max_number = first_number + 49;
		number_row = 5;
		number_column = 10;
		cell_size = cocos2d::Size(700 / number_column, 350 / number_row);
	}
	else if (str == "half_after") {
		option_chart = OptionChart::half_chart;
		min_number = first_number + 50;
		max_number = first_number + 99;
		number_row = 5;
		number_column = 10;
		cell_size = cocos2d::Size(700 / number_column, 350 / number_row);
	}
	number_size = cell_size;
	background = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));

	//TODO:
	//lay tat ca icon trong media_set
	auto list_icon = getNumbers(0, 100);
	for (math::resource::Image *img : list_icon) { 
		img->used = true;
	}
	bool *a;
	a = new bool[max_number + 1];
	for (int i = 0; i <= max_number; i++) {
		a[i] = false;
	}
	for (math::resource::Image *img : list_icon) {
		int number = img->getComparisionValue("number");
		if (number >= min_number && number <= max_number && a[number] == false) {
			icons_number.push_back(img);
			a[number] = true;
		}
	}
	std::sort(icons_number.begin(), icons_number.end(), Thanh_ChartHundered::cmpImage);

	// TODO: GET ICONS RESULT
	bool *tmp;
	tmp = new bool[101];
	for (int i = min_number; i <= max_number; i++) tmp[i] = false;
	if (consecutive)
	{
		int hang_chuc = cocos2d::random(min_number / 10, (max_number - 1) / 10);
		int hang_don_vi = cocos2d::random(min_number % 10, (min_number + 9 - number_miss) % 10);
		for (int i = 0; i < number_miss; i++) {
			for (math::resource::Image* img : icons_number) {
				if (img->getComparisionValue("number") == hang_chuc * 10 + hang_don_vi + i) {
					icons_result.push_back(img);
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 5; i++) {
			int rd = cocos2d::random(min_number, max_number);
			while (tmp[rd]) rd = cocos2d::random(min_number, max_number);
			tmp[rd] = true;
			for (math::resource::Image* img : icons_number) {
				if (img->getComparisionValue("number") == rd) {
					icons_result.push_back(img);
					break;
				}
			}
		}
	}
	while (icons_result.size() < number_result) {
		int rd = cocos2d::random(min_number, max_number);
		while (tmp[rd]) rd = cocos2d::random(min_number, max_number);
		tmp[rd] = true;
		for (math::resource::Image* img : icons_number) {
			if (img->getComparisionValue("number") == rd) {
				icons_result.push_back(img);
				break;
			}
		}
	}

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	if (option_game == OptionGame::draw_row || option_game == OptionGame::find_number) {
		// optional
		std::vector<math::resource::Image*> list_img;
		list_img.push_back(icons_result[0]);
		auto opt_keys = getQuestion()->getOptionalKeys();
		for (std::string key : opt_keys) {
			if (key == "number") {
				std::vector<math::resource::Text> texts;
				for (math::resource::Image* img : list_img) {
					auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
					img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
					texts.push_back(img_txt);
				}
				getQuestion()->setOptionalTexts(key, texts);
			}
			else if (key == "name_1") {
				std::vector<math::resource::Text> texts;
				for (math::resource::Image* img : list_img) {
					auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
					texts.push_back(img_txt);
				}
				getQuestion()->setOptionalTexts(key, texts);
			}
			else {
				getQuestion()->setOptionalTexts(key, list_img);
			}
		}
	}

	/*
	 if (random_sorted) {
	 random_shuffle(list_questiondata.begin(), list_questiondata.end());
	 }
	 */

	obj_id = 0;
	int gr_id = 0;
	auto sort_key = getComparisionKey("number");

	switch (option_game) {
		// TODO: OPTION DRAG NUMBER
	case OptionGame::drag_number:
	{
		// TODO: gen chart hundered
		chart = Math_Pool::getInstance()->getGroup();
		chart->setObjectSize(cell_size);
		chart->setComparisionKey(sort_key);

		std::vector<Math_Object*> _group_list;
		for (math::resource::Image *img : icons_number)
		{
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, img, background);
			object->setBackgroundVisible(true);
			object->setImageVisible(false);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->showValueTextWithAnimation(false);
			object->fitValueTextSize();
			object->setObjectSize(cell_size);
			object->getLabelValue()->setFontSize(cell_size.height*0.6);
			object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			object->getLabelValue()->disableEffect();
			object->getBackground()->setContentSize(cell_size*1.21);
			_objects.push_back(object);
			numbers.push_back(object);
			_group_list.push_back(object);
		}

		chart->setEnable(gr_id++, _group_list, math::group::GroupType::frame, 1);
		chart->setGroupFrames(number_column, number_row, 
			math::resource::ResHelper::createImage(getUIPath("")),
			math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"))); 
		chart->setBackgroundVisiable(true);
		_groups.push_back(chart);

		// TODO: gen object drag
		for (math::resource::Image* img : icons_result)
		{
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::touch, img, background);
			object->setSelectedAnimation(math::common::SelectedAnimation::scale);
			object->setMoveCallback(CC_CALLBACK_1(Thanh_ChartHundered::onTouchedObject, this), CC_CALLBACK_2(Thanh_ChartHundered::onMoveObjectEnded, this));
			object->setBackgroundVisible(true);
			object->setImageVisible(false);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			object->showValueTextWithAnimation(false);
			object->getLabelValue()->disableEffect();
			object->fitValueTextSize();
			object->setObjectSize(number_size);
			object->getLabelValue()->setFontSize(number_size.height*0.6); 
			_objects.push_back(object);
			numbers_drag.push_back(object);
		}
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(numbers_drag.begin(), numbers_drag.end(), g);
		// TODO: gen slot
		for (int k = 0; k < number_miss; k++)
		{
			Math_Slot *slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(obj_id++, math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"))); 
			slot->setSlotSize(cell_size*1.2);
			_slots.push_back(slot);
			slots.push_back(slot); 
		}
	}
	break;

	// TODO: OPTION FILL ROW BY COLOR
	case OptionGame::draw_row:
	case OptionGame::find_number:
	{
		// gen chart
		chart = Math_Pool::getInstance()->getGroup();
		chart->setObjectSize(cell_size);
		chart->setComparisionKey(sort_key);

		std::vector<Math_Object*> _group_list;
		for (math::resource::Image *img : icons_number)
		{
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, img, background);
			object->setSelectedAnimation(math::common::SelectedAnimation::scale);
			object->setBackgroundVisible(true);
			object->setImageVisible(false);
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
			object->showValueTextWithAnimation(false);
			object->fitValueTextSize();
			object->setObjectSize(cell_size);
			object->getLabelValue()->setFontSize(cell_size.height*0.6);
			object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			object->getLabelValue()->disableEffect();
			object->getBackground()->setContentSize(cell_size*1.2);
			_objects.push_back(object);
			numbers.push_back(object);
			_group_list.push_back(object);
		}

		chart->setEnable(gr_id++, _group_list, math::group::GroupType::frame, 1);
		chart->setGroupFrames(number_column, number_row,
			math::resource::ResHelper::createImage(""), 
			math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"))); 
		chart->setBackgroundVisiable(true);
		_groups.push_back(chart);

		// gen object
		if (option_game == OptionGame::draw_row) {
			Math_Object* obj = Math_Pool::getInstance()->getObject();
			obj->setEnable(obj_id++, math::common::deny, icons_result[0], background);
			obj->setBackgroundVisible(true);
			obj->setImageVisible(false);
			obj->setValueText(cocos2d::StringUtils::toString(icons_result[0]->getComparisionValue("number")));
			obj->showValueTextWithAnimation(false);
			obj->fitValueTextSize();
			obj->setObjectSize(cell_size);
			obj->getLabelValue()->setFontSize(cell_size.height*0.6);
			obj->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			obj->getLabelValue()->disableEffect();
			_objects.push_back(obj);
		}
	}
	break;

	default:
		break;
	}
	//Math_Pool::getInstance()->resetImages();
}

void Thanh_ChartHundered::onStart() {
	CCLOG("%s override", __func__);

	obj_id = 0;

	switch (option_game) {
		// TODO: OPTION DRAG NUMBER
	case OptionGame::drag_number:
	{
		onShowRightBar(0, 0.2, CC_CALLBACK_1(Thanh_ChartHundered::onAnswerDone, this));
		// show chart
		if (option_chart == OptionChart::full_chart) {
			chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, _panel->getContentSize().height / 2));
		}
		else chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, _panel->getContentSize().height / 2));
		addChildNode(chart, 2);
		chart->onShow(0, math::animation::No_Effect); 
		chart->getBackground()->setOpacity( 0 ); 
		chart->getBackground()->runAction( cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(1 ), cocos2d::FadeIn::create( 0.8 ) ));
		math::func::setPositionList(numbers_drag, 1, cocos2d::Rect(0, 0.2, 0.2, 0.75));
		for (Math_Object *object : numbers_drag) {
			object->getBackground()->setContentSize(number_size*1.2);
			addChildNodeToRightPanel(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
			object->enableDrag(_panel);
		}
		// show slot
		for (int k = 0; k < number_miss; k++)
		{
			Math_Slot *slot = slots[k];
			Math_Object *object = nullptr;
			for (int i = 0; i < chart->getNumberObjects(); i++) {
				if (chart->getObjectAtIndex(i)->getComparisionValue().number == icons_result[k]->getComparisionValue("number")) {
					object = chart->getObjectAtIndex(i);
					break;
				}
			}
			addChildNode(slot, 3);
			slot->setPosition(chart->convertToWorldSpace(object->getWorldPosition()));
			pair_slots.push_back(std::make_pair(slot, object));
			object->removeFromParent();
			slot->onShow(0.5, math::animation::ScaleUpAndRotation);
		} 
	}
	break;

	// TODO: OPTION FILL ROW BY COLOR
	case OptionGame::draw_row:
	case OptionGame::find_number:
	{
		// show chart 
		if (option_chart == OptionChart::full_chart) {
			chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2 - 100, _panel->getContentSize().height / 2));
		}
		else chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, _panel->getContentSize().height / 2));
		addChildNode(chart, 2);
		chart->onShow(0, math::animation::No_Effect); 
		chart->getBackground()->setOpacity( 0 ); 
		chart->getBackground()->runAction( cocos2d::Sequence::createWithTwoActions( cocos2d::DelayTime::create( 1 ), cocos2d::FadeIn::create( 0.8 ) ) );

		// add event
		auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
		touch_listener->onTouchBegan = CC_CALLBACK_2(Thanh_ChartHundered::onTouchBegan, this);
		touch_listener->onTouchEnded = CC_CALLBACK_2(Thanh_ChartHundered::onTouchEnded, this);
		touch_listener->onTouchMoved = CC_CALLBACK_2(Thanh_ChartHundered::onTouchMoved, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

		// gen button done
		btn_done = Math_Pool::getInstance()->getAnswer();
		btn_done->setEnable("Done", math::common::custom_bg, CC_CALLBACK_1(Thanh_ChartHundered::onAnswerDone, this), cocos2d::Size(200, 70));
		btn_done->setCustomAnswer(math::resource::ResHelper::createImage( getUIPath( "common/done.png" )), cocos2d::Size( 200, 70 ), 70 * 0.6, cocos2d::Color4B::WHITE, 0.2);
		btn_done->setPosition(cocos2d::Vec2(chart->getPositionX() + chart->getContentSize().width / 2 + 100 + btn_done->getContentSize().width/2, 
											chart->getPositionY() - chart->getContentSize().height / 2 +35));
		btn_done->setVisible(true);

		_answers.push_back(btn_done);
	}
	break;

	default:
		break;
	}

	//Gen question
	showQuestionBox( );
}

void Thanh_ChartHundered::startQuestion() {
	CCLOG("%s override", __func__);

	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();

		disable_touch = false; 

		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=]() {
			//#1. show number drag
			for (Math_Object *object : numbers_drag) { 
				object->enableDrag(_panel_right);
			}
		}), cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			//#2. show button done
			if (btn_done != nullptr) {
				if (option_game == OptionGame::drag_number && option_chart == OptionChart::full_chart) {
					if (option_game == OptionGame::drag_number && option_chart == OptionChart::full_chart) {
						btn_done->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, -100));
						btn_done->setVisible(false);
					}
					btn_done->onShowInParent(_panel, 3);
				}
				else {
					btn_done->onShowInParent(_panel, 3);
				}
			}
		}), nullptr));
	};
}

void Thanh_ChartHundered::onExplanation(bool correct) {
	CCLOG("%s override", __func__);

	if (!correct) disable_touch = false;

	switch (option_game) {
	case OptionGame::drag_number:
	{
		if (correct) {
			for (Math_Object *object : numbers_drag) {
				object->setTouch(math::common::TouchPermission::deny);
			} 
			this->nextQuestion();
		}
		else { 
			this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([this]() {
				for (std::pair<Math_Slot*, Math_Object*> p : pair_slots) {
					if (p.first->isUsed() == true && p.first->getLinker()->getComparisionValue().number != p.second->getComparisionValue().number) {
						p.first->getLinker()->runAction(cocos2d::ScaleTo::create(0.2, number_size.height / p.first->getLinker()->getContentSize().height));
						p.first->getLinker()->getLabelValue()->setFontSize(number_size.height*0.6);
						p.first->setLocalZOrder(2);
						p.first->getLinker()->backToStart(); 
						p.first->getLinker()->getBackground()->runAction(cocos2d::FadeIn::create(0.25));
						p.first->removeLinker();
					}
				}
			}), nullptr));
		}
	}
	break;

	case OptionGame::draw_row:
	{
		if (correct)
		{
			if (enable_sound) {
				disable_touch = true;


				for (Math_Object *object : choose_list) {
					list_audio.push_back(object);
				}
				std::sort(list_audio.begin(), list_audio.end(), cmpObject);
				if (read_left_2_right == false) { 
					int l = 0, r = list_audio.size() - 1;
					while (l < r) {
						std::swap(list_audio[r], list_audio[l]);
						l++;
						r--;
					}
				}
				index_audio = 0;
				list_audio[0]->setScale(1.1);
				my_audio = math::AudioHelper::getInstance();
				my_audio->play_sequence(list_audio[0]->getImageSource()->getAudios(), CC_CALLBACK_0(Thanh_ChartHundered::play_next_audio, this), cocos2d::StringUtils::toString(index_audio));
			}
			else {
				this->nextQuestion();
			}
		}
		else {
			btn_done->onAnimationClick(false);
			for (Math_Object *object : choose_list) {
				object->getBackground()->loadTexture(background->path);
				object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			}
			choose_list.clear();
		}
	}
	case OptionGame::find_number:
	{
		if (correct)
		{
			this->nextQuestion();
		}
		else {
			btn_done->onAnimationClick(false);
			for (Math_Object *object : choose_list) {
				object->getBackground()->loadTexture(background->path);
				object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			}
			choose_list.clear();
		}
	}

	break;

	default:
		break;
	}
}

void Thanh_ChartHundered::onComplete() {
	CCLOG("%s override", __func__);

	//Math_Pool::getInstance()->resetAnswers();

	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	chart->getBackground()->runAction( cocos2d::Sequence::createWithTwoActions( cocos2d::DelayTime::create( m_delay ) , cocos2d::FadeOut::create(0.4)));
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=]() {
		//#1. hide chart hunder
		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			object->getBackground()->setContentSize(cell_size); 
			object->runAction( cocos2d::FadeOut::create( 0.4 ) );
		}
		 
		chart->setDisable();

		for (Math_Slot *slot : slots) {
			slot->getLinker()->removeFromParent();
			slot->onHide(0.2);
		}
	}), cocos2d::DelayTime::create(0.2), cocos2d::CallFunc::create([=]() {
		//#2. show number drag
		for (Math_Object *object : numbers_drag) {
			object->onHide(0);
		}
	}), nullptr));

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Thanh_ChartHundered::onAnswerDone(bool correct)
{
	disable_touch = true;
	bool done = true;
	if (option_game == OptionGame::drag_number)
	{
		for (std::pair<Math_Slot*, Math_Object*> p : pair_slots) {
			if (p.first->isUsed() == false || p.first->getLinker()->getComparisionValue().number != p.second->getComparisionValue().number) {
				done = false;
			}
		}
	}
	else if (option_game == OptionGame::draw_row) {
		bool ok = false;
		int minimum_object = 100, maximum_object = 0;
		for (Math_Object *object : choose_list) {
			if (object->getComparisionValue().number == icons_result[0]->getComparisionValue("number")) ok = true;
			if (object->getComparisionValue().number < minimum_object) minimum_object = object->getComparisionValue().number;
			if (object->getComparisionValue().number > maximum_object) maximum_object = object->getComparisionValue().number;
		}
		if (!ok) done = false;
		if (choose_list.size() != 10) done = false;
		if (maximum_object - minimum_object >= 10 || (minimum_object - min_number) % 10 != 0) done = false;
	}
	else if (option_game == OptionGame::find_number) {
		if (choose_list.size() == 0) {
			done = false;
		}
		else {
			for (Math_Object *object : choose_list) {
				if (object->getComparisionValue().number != icons_result[0]->getComparisionValue("number")) done = false;
			}
		}
	}

	this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_ChartHundered::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_ChartHundered::autoTestingRun(float dt)
{

}

#pragma mark - touch of drag number option
void Thanh_ChartHundered::onTouchedObject(cocos2d::Node* node)
{
	Math_Object* object = (Math_Object*)node;
	object->setLocalZOrder(4); 
	object->getBackground()->runAction(cocos2d::FadeIn::create(0.25));
	for (Math_Object* obj : numbers_drag) {
		if (obj->isSame(object) == false) {
			obj->setTouch(math::common::TouchPermission::deny);
		}
	}
}
void Thanh_ChartHundered::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
	Math_Object* object = (Math_Object*)node;
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
				object->runAction(cocos2d::ScaleTo::create(0.3, cell_size.width / object->getContentSize().width));
				object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition())); 
				object->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
			}
			else {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					m_slot->removeLinker();
					t_slot->removeLinker();

					t_slot->setLinker(object);
					object->runAction(cocos2d::ScaleTo::create(0.3, cell_size.width / object->getContentSize().width));
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

					m_slot->setLinker(linker);
					linker->runAction(cocos2d::ScaleTo::create(0.3, cell_size.width / linker->getContentSize().width));
					linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
					this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=]() { 
						linker->getBackground()->runAction(cocos2d::FadeIn::create(0.25));
					}), cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=]() { 
						linker->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
						object->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
					}), nullptr));
				}
				else {
					m_slot->removeLinker();
					t_slot->setLinker(object);
					object->runAction(cocos2d::ScaleTo::create(0.3, cell_size.width / object->getContentSize().width));
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition())); 
					object->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
				}
			}
		}
		else {
			//move to start
			m_slot->removeLinker();
			object->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / object->getContentSize().width));
			object->setLocalZOrder(2);
			object->backToStart(); 
			object->getBackground()->runAction(cocos2d::FadeIn::create(0.25));
		}
	}
	else {
		if (t_slot != nullptr) {
			if (t_slot->isUsed()) {
				auto linker = t_slot->getLinker();
				t_slot->removeLinker();
				linker->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / object->getContentSize().width));
				linker->setLocalZOrder(2);
				linker->backToStart();
				linker->getBackground()->runAction(cocos2d::FadeIn::create(0.25));
			}

			t_slot->setLinker(object);
			object->runAction(cocos2d::ScaleTo::create(0.3, cell_size.width / object->getContentSize().width));
			object->setLocalZOrder(4);
			object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			object->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
		}
		else {
			//move start
			object->runAction(cocos2d::ScaleTo::create(0.3, number_size.width / object->getContentSize().width));
			object->setLocalZOrder(2);
			object->backToStart();
			object->getBackground()->runAction(cocos2d::FadeIn::create(0.25));

		}
	}

	// enable touch
	for (Math_Object* obj : numbers_drag) {
		if (obj->isSame(object) == false) {
			obj->setTouch(math::common::TouchPermission::move);
		}
	}

	// on show on hide button done
	//if (option_chart == OptionChart::full_chart) {
	//	int count = 0;
	//	for (Math_Slot *slot : slots) {
	//		if (slot->isUsed()) count++;
	//	}
	//	if (count == number_miss) {
	//		for (Math_Object *object : numbers_drag) {
	//			bool fade = true;
	//			for (Math_Slot*slot : slots) {
	//				if (slot->getLinker()->isSame(object)) {
	//					fade = false;
	//				}
	//			}
	//			if (fade) {
	//				object->runAction(cocos2d::FadeOut::create(0.2));
	//				object->setTouch(math::common::TouchPermission::deny);
	//			}
	//		}

	//		// show button done
	//		btn_done->setVisible(true);
	//		btn_done->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width / 2, 50)));
	//	}
	//	else {
	//		// hide button done
	//		btn_done->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_panel->getContentSize().width / 2, -100)));
	//		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
	//			btn_done->setVisible(false);
	//		}), nullptr));
	//		for (Math_Object *object : numbers_drag) {
	//			bool show = true;
	//			for (Math_Slot*slot : slots) {
	//				if (slot->getLinker()->isSame(object)) {
	//					show = false;
	//				}
	//			}
	//			if (show) {
	//				object->runAction(cocos2d::FadeIn::create(0.2));
	//				object->setTouch(math::common::TouchPermission::move);
	//			}
	//		}
	//	}
	//}
}

#pragma mark - touch of draw row option
bool Thanh_ChartHundered::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) {
	if (disable_touch) return false;
	first_position_touch = _panel->convertToNodeSpace(touch->getLocation());
	only_touch = true;
	if (touch_mode == TouchMode::only_cell) {
		for (Math_Object* object : choose_list) {
			object->getBackground()->loadTexture(background->path);
			object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
		}
		choose_list.clear();
	}
	return true;
}

void Thanh_ChartHundered::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
	CCLOG("%s override", __func__);

	only_touch = false; 
	switch (touch_mode) {
	case TouchMode::only_row:
		// TODO: option only row
		bottom_right_touch = _panel->convertToNodeSpace(touch->getLocation());
		top_left_touch = first_position_touch;
		if (top_left_touch.x > bottom_right_touch.x) {
			float x = top_left_touch.x;
			top_left_touch.x = bottom_right_touch.x;
			bottom_right_touch.x = x;
		}
		if (top_left_touch.y < bottom_right_touch.y) {
			float y = top_left_touch.y;
			top_left_touch.y = bottom_right_touch.y;
			bottom_right_touch.y = y;
		}

		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
			if (object_position.x - cell_size.width / 2 < bottom_right_touch.x
				&& object_position.x + cell_size.width / 2 > top_left_touch.x
				&& object_position.y - cell_size.height / 2 < first_position_touch.y
				&& object_position.y + cell_size.height / 2 > first_position_touch.y)
			{
				object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png")); 
				choose_list.insert(object);
			}
			else {
				if (choose_list.find(object) != choose_list.end()) {
					object->getBackground()->loadTexture(background->path); 
					choose_list.erase(object);
				}
			}
		}
		break;
	case TouchMode::only_column:
		// TODO: option only column
		bottom_right_touch = _panel->convertToNodeSpace(touch->getLocation());
		top_left_touch = first_position_touch;
		if (top_left_touch.x > bottom_right_touch.x) {
			float x = top_left_touch.x;
			top_left_touch.x = bottom_right_touch.x;
			bottom_right_touch.x = x;
		}
		if (top_left_touch.y < bottom_right_touch.y) {
			float y = top_left_touch.y;
			top_left_touch.y = bottom_right_touch.y;
			bottom_right_touch.y = y;
		}

		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
			if (object_position.y + cell_size.height / 2 > bottom_right_touch.y
				&& object_position.y - cell_size.height / 2 < top_left_touch.y
				&& object_position.x - cell_size.width / 2 < first_position_touch.x
				&& object_position.x + cell_size.width / 2 > first_position_touch.x)
			{
				object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png")); 
				choose_list.insert(object);
			}
			else {
				if (choose_list.find(object) != choose_list.end()) {
					object->getBackground()->loadTexture(background->path); 
					choose_list.erase(object);
				}
			}
		}
		break;
	case TouchMode::only_block:
		// TODO: option only block
		bottom_right_touch = _panel->convertToNodeSpace(touch->getLocation());
		top_left_touch = first_position_touch;
		if (top_left_touch.x > bottom_right_touch.x) {
			float x = top_left_touch.x;
			top_left_touch.x = bottom_right_touch.x;
			bottom_right_touch.x = x;
		}
		if (top_left_touch.y < bottom_right_touch.y) {
			float y = top_left_touch.y;
			top_left_touch.y = bottom_right_touch.y;
			bottom_right_touch.y = y;
		}

		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
			if (object_position.y + cell_size.height / 2 > bottom_right_touch.y
				&& object_position.y - cell_size.height / 2 < top_left_touch.y
				&& object_position.x - cell_size.width / 2 < bottom_right_touch.x
				&& object_position.x + cell_size.width / 2 > top_left_touch.x)
			{
				object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png"));
				choose_list.insert(object);
			}
			else {
				if (choose_list.find(object) != choose_list.end()) {
					object->getBackground()->loadTexture(background->path);
					choose_list.erase(object);
				}
			}
		}
		break;
	case TouchMode::cell_and_block:
		// TODO: option cell and block
		bottom_right_touch = _panel->convertToNodeSpace(touch->getLocation());
		top_left_touch = first_position_touch;
		if (top_left_touch.x > bottom_right_touch.x) {
			float x = top_left_touch.x;
			top_left_touch.x = bottom_right_touch.x;
			bottom_right_touch.x = x;
		}
		if (top_left_touch.y < bottom_right_touch.y) {
			float y = top_left_touch.y;
			top_left_touch.y = bottom_right_touch.y;
			bottom_right_touch.y = y;
		}

		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
			if (object_position.y + cell_size.height / 2 > bottom_right_touch.y
				&& object_position.y - cell_size.height / 2 < top_left_touch.y
				&& object_position.x - cell_size.width / 2 < bottom_right_touch.x
				&& object_position.x + cell_size.width / 2 > top_left_touch.x)
			{
				object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png"));
				choose_list.insert(object);
			}
		}
		break;

	case TouchMode::only_cell:
		// TODO: option cell and block
		only_touch = false;
		for (int i = 0; i < chart->getNumberObjects(); i++) {
			Math_Object *object = chart->getObjectAtIndex(i);
			cocos2d::Vec2 locationInNode = object->convertToNodeSpace(first_position_touch);
			cocos2d::Size size = object->getBoundingBox().size;
			cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

			if (rect.containsPoint(locationInNode)) {
				locationInNode = object->convertToNodeSpace(touch->getLocation());
				if (rect.containsPoint(locationInNode) == true) {
					only_touch = true;
				}
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Thanh_ChartHundered::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
	CCLOG("%s override", __func__);
	if (first_position_touch.x > _panel->convertToNodeSpace(touch->getLocation()).x) {
		read_left_2_right = true;
	}

	switch (touch_mode) {
	case TouchMode::only_row:
		if (choose_list.size() < 10) {
			for (Math_Object *object : choose_list) {
				object->getBackground()->loadTexture(background->path);
			}
			choose_list.clear();
		}
		break;
	case TouchMode::only_column:
		if (choose_list.size() < 10) {
			for (Math_Object *object : choose_list) {
				object->getBackground()->loadTexture(background->path);
			}
			choose_list.clear();
		}
		break;
	case TouchMode::cell_and_block:
		if (only_touch) {
			for (int i = 0; i < chart->getNumberObjects(); i++) {
				Math_Object *object = chart->getObjectAtIndex(i);
				cocos2d::Vec2 locationInNode = object->convertToNodeSpace(touch->getLocation());
				cocos2d::Size size = object->getBoundingBox().size;
				cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

				if (rect.containsPoint(locationInNode)) {
					if (choose_list.find(object) == choose_list.end()) {
						object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png"));
						choose_list.insert(object);
					}
					else {
						object->getBackground()->loadTexture(background->path);
						choose_list.erase(object);
					}
				}
			}
		}
		break;
	case TouchMode::only_cell:
		if (only_touch) {
			for (int i = 0; i < chart->getNumberObjects(); i++) {
				Math_Object *object = chart->getObjectAtIndex(i);
				cocos2d::Vec2 locationInNode = object->convertToNodeSpace(first_position_touch);
				cocos2d::Size size = object->getBoundingBox().size;
				cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

				if (rect.containsPoint(locationInNode)) {
					if (choose_list.find(object) == choose_list.end()) {
						object->getBackground()->loadTexture(getUIPath("common/frame/brown1.png"));
						choose_list.insert(object);
						break;
					}
				}
			}
		}

	default:
		break;
	}
}

void Thanh_ChartHundered::onTouchCancel(cocos2d::Touch *touch, cocos2d::Event *event)
{
	for (Math_Object*object : choose_list) {
		object->getBackground()->loadTexture(background->path);
	}
	choose_list.clear();
}

#pragma mark - bool of sort
bool Thanh_ChartHundered::cmpImage(math::resource::Image *a, math::resource::Image *b)
{
	return a->getComparisionValue("number") < b->getComparisionValue("number");
}

bool Thanh_ChartHundered::cmpObject(Math_Object *a, Math_Object *b)
{
	return a->getComparisionValue().number < b->getComparisionValue().number;
}


void Thanh_ChartHundered::play_next_audio() {
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.2), cocos2d::CallFunc::create([=]() {
		list_audio[index_audio]->setScale(1);
		index_audio++;
		if (index_audio >= list_audio.size()) this->startQuestion();
		list_audio[index_audio]->setScale(1.1);
		my_audio->play_sequence(list_audio[index_audio]->getImageSource()->getAudios(), CC_CALLBACK_0(Thanh_ChartHundered::play_next_audio, this), cocos2d::StringUtils::toString(index_audio));
	}), nullptr));
}
