//
//  Thanh_DoubleAddition.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//. drag the number to result to get the correct answer

#include "Thanh_DoubleAddition.h"
#include "Math_Libs.h"
#define line_color cocos2d::Color4F(53, 102, 47, 255)

Thanh_DoubleAddition* Thanh_DoubleAddition::createQuiz()
{
	Thanh_DoubleAddition* quiz = new Thanh_DoubleAddition();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_DoubleAddition::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_DoubleAddition::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
    add_three_operand = getJsonBool(json_game, "three_operand", false);
    str = getJsonString(json_game, "hide", "operand_1");
    
	if (str == "operand_1") hide = Hide::operand_1;
	else if (str == "operand_2") hide = Hide::operand_2;
	else if (str == "operand_3") hide = Hide::operand_3;
	else if (str == "equal") hide = Hide::equal;
    
	if (!add_three_operand && hide == Hide::operand_3) {
		hide = Hide::equal;
	}

    int minimum_result = getJsonInt(getJsonMember(json_game, "result"), "minimum", 1);
	int maximum_result = getJsonInt(getJsonMember(json_game, "result"), "maximum", 5);

    slot_in_group_row = getJsonBool(json_game, "slot_in_group_row", true);
    operand_is_the_same = getJsonBool(json_game, "operand_is_the_same", true);
    hide_group = getJsonBool(json_game, "hide_group", false);
	if (hide_group) {
        slot_in_group_row = false;
        counting_xx = getJsonInt(json_game, "counting_option", 1);
    } else {
        counting_xx = 1;
    }

	//TODO: OBJECT CONFIG
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
    
    str = getJsonString(json_object, "display", "name");
	if (str == "number" || counting_xx != 1) object_option = ObjectOption::number;
	else object_option = ObjectOption::text;

    object_size = getSizeJson(json_object, "size");
    
    object_bg = objectBG();
    auto result_bg = objectBG();
    math::resource::Image*plus_operator = getOperator("+");
    math::resource::Image*equal_operator = getOperator("=");
    
	//TODO: GROUP CONFIG
	rapidjson::Value& json_group = json_data.FindMember("group")->value;

    str = getJsonString(json_group, "group_type", "custom");
    group_type = math::group::GroupType::custom;
	if (str == "frame") group_type = math::group::GroupType::frame;
	else if (str == "queue") group_type = math::group::GroupType::queue;

    element_group_size = getSizeJson(json_group, "object_size");
    number_column = getJsonInt(json_group, "number_column", 5);
    number_row = getJsonInt(json_group, "number_row", 2);
	if (add_three_operand) {
		number_row = 3;
		number_column = 3;
	}
    group_bg = groupBG();
    auto group_frame = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));

	// TODO: SLOT CONFIG
	rapidjson::Value& json_slot = json_data.FindMember("slot")->value;
    slot_size = getSizeJson(json_slot, "size");
    slot_bg = slotBG();

	//TODO:
	//lay tat ca icon trong media_set
	// tạo ra các giá trị
	result = cocos2d::random(minimum_result, maximum_result);
	if (operand_is_the_same) {
		if (add_three_operand) {
			bool ok = false;
			for (int t = minimum_result; t <= maximum_result; t++) {
				if (t % 3 == 0) ok = true;
			}
			if (!ok) {
				CCLOG("khong dung in put, result khong chia het cho 3");
				return;
			}
			while (result % 3 != 0) {
				result = cocos2d::random(minimum_result, maximum_result);
			}
			a = result / 3;
			b = result / 3;
			c = result / 3;
		}
		else {
			if (minimum_result == maximum_result && minimum_result % 2 != 0) {
				CCLOG("sai result input");
				return;
			}
			while (result % 2 != 0) {
				result = cocos2d::random(minimum_result, maximum_result);
			}
			a = result / 2;
			b = result / 2;
		}
	}
	else if (operand_is_the_same == false) {
		if (add_three_operand) {
			a = 30;
			while (a > 9 || b > 9 || c > 9) {
				a = cocos2d::random(0, result);
				b = cocos2d::random(0, result - a);
				c = result - a - b;
			}
		}
		else {
			a = 20;
			while (a > 10 || b > 10) {
				a = cocos2d::random(0, result);
				b = result - a;
			}
		}
	}
	results_value.clear();
	std::vector<int> temp;
	std::vector<int>::iterator it;
	int max_number = 20;
	if (add_three_operand) max_number = 30;
	for (int i = 0; i <= max_number; i++) {
		temp.push_back(i);
	}
	it = temp.begin() + result;
	results_value.push_back(*it);
	temp.erase(it);
	if (hide == Hide::operand_1) {
		it = temp.begin() + a;
		results_value.push_back(*it);
		temp.erase(it);
	}
	else if (hide == Hide::operand_2) {
		it = temp.begin() + b;
		results_value.push_back(*it);
		temp.erase(it);
	}
	else if (hide == Hide::operand_3) {
		it = temp.begin() + b;
		results_value.push_back(*it);
		temp.erase(it);
	}

	while (results_value.size() < 4) {
		it = temp.begin() + cocos2d::random(0, (int)temp.size() - 1);
		results_value.push_back(*it);
		temp.erase(it);
	}

	// lấy các icons
	auto list_icons = getNumbers(0, max_number);
	for (math::resource::Image* img : list_icons)
	{
		int number = img->getComparisionValue("number");
		// lấy img của số a
		if (number == a)
			img_a = img;

		// lấy img của số b
		if (number == b)
			img_b = img;
		if (number == c)
			img_c = img;
	}

	for (int i = 0; i < results_value.size(); i++) {
		for (math::resource::Image* img : list_icons)
		{
			int number = img->getComparisionValue("number");
			if (number == results_value[i]) {
				icons_result.push_back(img);
				break;
			}
		}
	}
	for (int i = 0; i < icons_result.size(); i++) {
		icons_result[i]->used = true;
	}
    math::func::shuffle(icons_result);
    
    list_icons = getImageList("icons");
    icon_group = math::func::getRandomNode(list_icons);

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	// optional
	auto _icons_question = getNumbers(equal, equal);
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : _icons_question) {
				texts.push_back(img->getTextValue("name_1"));
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : _icons_question) {
                texts.push_back(img->getTextValue("name_n"));
            }
			getQuestion()->setOptionalTexts(key, texts);
		}
		else {
			getQuestion()->setOptionalTexts(key, _icons_question);
		}
	}

	int obj_id = 0;
	int gr_id = 0;
	// TODO: GEN ROW 1
	Math_Object *object;
	Math_Group *group;
	auto sort_key = getComparisionKey("number");

	// taoj group thu 1
	group = mpool->getGroup();
	group->setObjectSize(element_group_size);
	group->setComparisionKey(sort_key); 

	std::vector<Math_Object*> _group_list_1;
	for (int i = 0; i < img_a->getComparisionValue("number"); i++)
	{
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, icon_group, icon_group);
		object->setImageVisible(false);
		object->setValueText("");
		object->setObjectSize(element_group_size);
		_objects.push_back(object);
		_group_list_1.push_back(object);
	}
	if (add_three_operand) {
		if (img_a->getComparisionValue("number") > 6) {
			number_row = 3;
		}
		else if (img_a->getComparisionValue("number") > 3) {
			number_row = 2;
		}
		else {
			number_row = 1;
		}
	}
	else {
		if (img_a->getComparisionValue("number") > 5 || img_a->getComparisionValue("number") == 0) {
			number_row = 2;
		}
		else {
			number_row = 1;
		}
	}

	if (group_type == math::group::GroupType::frame) {
		group->setEnable(gr_id++, _group_list_1, math::group::GroupType::frame, (int)_group_list_1.size());
		group->setGroupFrames(number_column, number_row, group_frame);
	}
	else {
		while (_group_list_1.size() < number_column*number_row) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, icon_group, object_bg);
			object->setBackgroundVisible(false);
			object->setImageVisible(false);
			object->setValueText("");
			object->setObjectSize(element_group_size);
			_objects.push_back(object);
			_group_list_1.push_back(object);
		}
		auto size1 = cocos2d::Size(element_group_size.width*1.2*number_column, element_group_size.height*number_row*1.2);
		group->setEnable(gr_id++, _group_list_1, math::group::GroupType::queue, (int)_group_list_1.size());
		group->setGroupQueue(size1); 
		group->setBackground(group_bg, 0.4);
	}
	row_1.push_back(group);
	all_groups.push_back(group);

	// tao toan tu +
	object = Math_Pool::getInstance()->getObject();
	object->setEnable(obj_id++, math::common::deny, plus_operator, plus_operator);
	object->setObjectSize(object_size / 2);
	object->setBackgroundVisible(false);
	object->setImageVisible(true);
	object->setValueText("");
	object->showValueTextWithAnimation(false);
	object->getLabelValue()->setFontSize(object_size.height*0.6);
	object->fitValueTextSize();
	_objects.push_back(object);
	row_1.push_back(object);
	all_objects_row_1.push_back(object);

	// tao group thu 2
	group = Math_Pool::getInstance()->getGroup();
	group->setObjectSize(element_group_size);
	group->setComparisionKey(sort_key); 

	std::vector<Math_Object*> _group_list_2;
	for (int i = 0; i < img_b->getComparisionValue("number"); i++)
	{
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, icon_group, icon_group);
		object->setImageVisible(false);
		object->setValueText("");
		object->setObjectSize(element_group_size);
		_objects.push_back(object);
		_group_list_2.push_back(object);
	}

	if (add_three_operand) {
		if (img_b->getComparisionValue("number") > 6) {
			number_row = 3;
		}
		else if (img_b->getComparisionValue("number") > 3) {
			number_row = 2;
		}
		else {
			number_row = 1;
		}
	}
	else {
		if (img_b->getComparisionValue("number") > 5 || img_b->getComparisionValue("number") == 0) {
			number_row = 2;
		}
		else {
			number_row = 1;
		}
	}
	if (group_type == math::group::GroupType::frame) {
		group->setEnable(gr_id++, _group_list_2, math::group::GroupType::frame, (int)_group_list_2.size());
		group->setGroupFrames(number_column, number_row, group_frame);
	}
	else {
		while (_group_list_2.size() < number_column*number_row) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, icon_group, object_bg);
			object->setBackgroundVisible(false);
			object->setImageVisible(false);
			object->setValueText("");
			object->setObjectSize(element_group_size); 
			_objects.push_back(object);
			_group_list_2.push_back(object);
		}
		auto size2 = cocos2d::Size(element_group_size.width*1.2*number_column, element_group_size.height*number_row*1.2);
		group->setEnable(gr_id++, _group_list_2, math::group::GroupType::queue, (int)_group_list_2.size());
		group->setGroupQueue(size2);
		//group->setObjectSize(element_group_size);
		group->setBackground(group_bg, 20);
		group->setBackgroundVisiable(true);
	}
	row_1.push_back(group);
	all_groups.push_back(group);

	if (add_three_operand) {
		// tao toan tu +
		object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, plus_operator, plus_operator);
		object->setObjectSize(object_size / 2);
		object->setBackgroundVisible(false);
		object->setImageVisible(true);
		object->setValueText("");
		object->showValueTextWithAnimation(false);
		object->getLabelValue()->setFontSize(object_size.height*0.6);
		object->fitValueTextSize();
		_objects.push_back(object);
		row_1.push_back(object);
		all_objects_row_1.push_back(object);

		// tao group thu 3
		group = Math_Pool::getInstance()->getGroup();
		group->setObjectSize(element_group_size);
		group->setComparisionKey(sort_key); 

		std::vector<Math_Object*> _group_list_2;
		for (int i = 0; i < img_c->getComparisionValue("number"); i++)
		{
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, icon_group, icon_group);
			object->setImageVisible(false);
			object->setValueText("");
			object->setObjectSize(element_group_size); 
			_objects.push_back(object);
			_group_list_2.push_back(object);
		}

		if (add_three_operand) {
			if (img_c->getComparisionValue("number") > 6) {
				number_row = 3;
			}
			else if (img_c->getComparisionValue("number") > 3) {
				number_row = 2;
			}
			else {
				number_row = 1;
			}
		}
		else {
			if (img_c->getComparisionValue("number") > 5 || img_c->getComparisionValue("number") == 0) {
				number_row = 2;
			}
			else {
				number_row = 1;
			}
		}
		if (group_type == math::group::GroupType::frame) {
			group->setEnable(gr_id++, _group_list_2, math::group::GroupType::frame, (int)_group_list_2.size());
			group->setGroupFrames(number_column, number_row, group_frame);
		}
		else {
			while (_group_list_2.size() < number_column*number_row) {
				Math_Object* object = Math_Pool::getInstance()->getObject();
				object->setEnable(obj_id++, math::common::deny, icon_group, object_bg);
				object->setBackgroundVisible(false);
				object->setImageVisible(false);
				object->setValueText("");
				object->setObjectSize(element_group_size); 
				_objects.push_back(object);
				_group_list_2.push_back(object);
			}
			auto size3 = cocos2d::Size(element_group_size.width*1.2*number_column, element_group_size.height*number_row*1.2);
			group->setEnable(gr_id++, _group_list_2, math::group::GroupType::queue, (int)_group_list_2.size());
			group->setGroupQueue(size3);
			group->setBackground(group_bg, 20);
			group->setBackgroundVisiable(true);
		}
		row_1.push_back(group);
		all_groups.push_back(group);
	}

	// tao toan tu =
	object = Math_Pool::getInstance()->getObject();
	object->setEnable(obj_id++, math::common::deny, equal_operator, equal_operator);
	object->setObjectSize(object_size / 2);
	object->setBackgroundVisible(false);
	object->setImageVisible(true);
	object->setValueText("");
	object->showValueTextWithAnimation(false);
	object->getLabelValue()->setFontSize(object_size.height*0.6);
	object->fitValueTextSize();
	_objects.push_back(object);
	row_1.push_back(object);
	all_objects_row_1.push_back(object);

	// tao slot result
	Math_Slot *slot = Math_Pool::getInstance()->getSlot();
	slot->setEndble(obj_id++, slot_bg);
	slot->setSlotSize(slot_size);
	slot->setBackgroundVisible(true);
	slot_1 = slot;

	_slots.push_back(slot);
	slots.push_back(slot);
	row_1.push_back(slot);
	slot->removeLinker();

	// TODO: GEN ROW 2
	// taoj object thu 1
	if (hide != Hide::operand_1) {
		object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, object_bg, object_bg);
		object->setObjectSize(slot_size);
		object->setBackgroundVisible(false);
		object->setImageVisible(true);
		if (object_option == ObjectOption::number)
		{
			object->setValueText(cocos2d::StringUtils::toString(img_a->getComparisionValue("number") * counting_xx));
		}
		else {
			object->setValueText(img_a->getText("name_1", m_lang->getLanguage()));
		}
		object->getLabelValue()->setFontSize(object_size.height*0.6);
		//object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
		object->showValueTextWithAnimation(false);
		object->fitValueTextSize();
		_objects.push_back(object);
		row_2.push_back(object);
		all_objects_row_2.push_back(object);
	}
	else {
		Math_Slot *slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(obj_id++, slot_bg);
		slot->setSlotSize(slot_size);
		slot->setBackgroundVisible(true);
		slot->removeLinker();

		_slots.push_back(slot);
		slots.push_back(slot);
		row_2.push_back(slot);
		slot_2 = slot;
	}

	// tao toan tu +
	object = Math_Pool::getInstance()->getObject();
	object->setEnable(obj_id++, math::common::deny, plus_operator, plus_operator);
	object->setObjectSize(object_size / 2);
	object->setBackgroundVisible(false);
	object->setImageVisible(true);
	object->setValueText("");
	object->showValueTextWithAnimation(false);
	object->getLabelValue()->setFontSize(object_size.height*0.6);
	object->fitValueTextSize();
	_objects.push_back(object);
	row_2.push_back(object);
	all_objects_row_2.push_back(object);

	// tao object thu 2
	if (hide != Hide::operand_2) {
		object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, object_bg, object_bg);
		object->setObjectSize(slot_size);
		object->setBackgroundVisible(false);
		object->setImageVisible(true);
		if (object_option == ObjectOption::number)
		{
			object->setValueText(cocos2d::StringUtils::toString(img_b->getComparisionValue("number") * counting_xx));
		}
		else {
			object->setValueText(img_b->getText("name_1", m_lang->getLanguage()));
		}
		object->getLabelValue()->setFontSize(object_size.height*0.6);
		//object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
		object->showValueTextWithAnimation(false);
		object->fitValueTextSize();
		_objects.push_back(object);
		row_2.push_back(object);
		all_objects_row_2.push_back(object);
	}
	else {
		Math_Slot *slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(obj_id++, slot_bg);
		slot->setSlotSize(slot_size);
		slot->setBackgroundVisible(true);
		slot->removeLinker();

		_slots.push_back(slot);
		slots.push_back(slot);
		row_2.push_back(slot);
		slot_2 = slot;
	}

	if (add_three_operand)
	{
		// tao toan tu +
		object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, plus_operator, plus_operator);
		object->setObjectSize(object_size / 2);
		object->setBackgroundVisible(false);
		object->setImageVisible(true);
		object->setValueText("");
		object->showValueTextWithAnimation(false);
		object->getLabelValue()->setFontSize(object_size.height*0.6);
		object->fitValueTextSize();
		_objects.push_back(object);
		row_2.push_back(object);
		all_objects_row_2.push_back(object);
		// tao object thu 3
		if (hide != Hide::operand_3) {
			object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, object_bg, object_bg);
			object->setObjectSize(slot_size);
			object->setBackgroundVisible(false);
			object->setImageVisible(true);
			if (object_option == ObjectOption::number)
			{
				object->setValueText(cocos2d::StringUtils::toString(img_c->getComparisionValue("number") * counting_xx));
			}
			else {
				object->setValueText(img_c->getText("name_1", m_lang->getLanguage()));
			}
			object->getLabelValue()->setFontSize(object_size.height*0.6);
			//object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
			object->showValueTextWithAnimation(false);
			object->fitValueTextSize();
			_objects.push_back(object);
			row_2.push_back(object);
			all_objects_row_2.push_back(object);
		}
		else {
			Math_Slot *slot = Math_Pool::getInstance()->getSlot();
			slot->setEndble(obj_id++, slot_bg);
			slot->setSlotSize(slot_size);
			slot->setBackgroundVisible(true);
			slot->removeLinker();
			_slots.push_back(slot);
			slots.push_back(slot);
			row_2.push_back(slot);
			slot_2 = slot;
		}
	}
	// tao toan tu =
	object = Math_Pool::getInstance()->getObject();
	object->setEnable(obj_id++, math::common::deny, equal_operator, equal_operator);
	object->setObjectSize(object_size / 2);
	object->setBackgroundVisible(false);
	object->setImageVisible(true);
	object->setValueText("");
	object->showValueTextWithAnimation(false);
	object->getLabelValue()->setFontSize(object_size.height*0.6);
	object->fitValueTextSize();
	_objects.push_back(object);
	row_2.push_back(object);
	all_objects_row_2.push_back(object);

	// tao slot result
	if (hide == Hide::equal) {
		Math_Slot *slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(obj_id++, slot_bg);
		slot->setSlotSize(slot_size);
		slot->setBackgroundVisible(true);
		slot->removeLinker();

		_slots.push_back(slot);
		slots.push_back(slot);
		row_2.push_back(slot);
		slot_2 = slot;
	}
	else {
		for (math::resource::Image*img : icons_result) {
			if (img->getComparisionValue("number") == result) {
				object = Math_Pool::getInstance()->getObject();
				object->setEnable(obj_id++, math::common::deny, object_bg, object_bg);
				object->setObjectSize(slot_size);
				object->setBackgroundVisible(false);
				object->setImageVisible(true);
				if (object_option == ObjectOption::number)
				{
					object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number") * counting_xx));
				}
				else {
					object->setValueText(img->getText("name_1", m_lang->getLanguage()));
				}
				object->getLabelValue()->setFontSize(object_size.height*0.6);
				//object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
				object->showValueTextWithAnimation(false);
				object->fitValueTextSize();
				_objects.push_back(object);
				row_2.push_back(object);
				all_objects_row_2.push_back(object);
			}
		}
	}
	// TODO: GEN RESULTS
	for (math::resource::Image *img : icons_result)
	{
		Math_Object *object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::move, img, result_bg, false);
		object->setMoveCallback(CC_CALLBACK_1(Thanh_DoubleAddition::onTouchedObject, this), CC_CALLBACK_2(Thanh_DoubleAddition::onMoveObjectEnded, this));
		object->setObjectSize(object_size);
		object->setBackgroundVisible(true);
		object->setImageVisible(false);
		if (object_option == ObjectOption::number)
		{
			object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number") * counting_xx));
		}
		else {
			object->setValueText(img->getText("name_1", m_lang->getLanguage()));
		}
		object->getLabelValue()->setFontSize(object_size.height*0.6);
		object->fitValueTextSize();
		object->getLabelValue()->setTextColor(cocos2d::Color4B(119, 60, 28, 255));
		object->showValueTextWithAnimation(false);
		_objects.push_back(object);
		results.push_back(object);
		object->setOpacity(255);
	}
}

void Thanh_DoubleAddition::onStart() {
	onShowBottomBar(0.5, 0.25, true, CC_CALLBACK_1(Thanh_DoubleAddition::onAnswerDone, this));
    quizBegan(0, true);
    
	if (hide_group) {
		// show row 2
		math::func::setPositionList(row_2, (int)row_2.size(), cocos2d::Rect(0.1, 0.2, 0.8, 0.2));
		for (Math_Object *object : all_objects_row_2)
		{
			addChildNode(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
		}
		addChildNode(slot_2, 2);
		slot_2->onShow(0, math::animation::ScaleUpAndRotation);

		// show RESULTS
		math::func::setPositionList(results, (int)results.size(), cocos2d::Rect(0.1, 0, 0.6, 0.2));
		for (Math_Object *object : results) {
			addChildNodeToBottomPanel(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
		}
	}
	else {
		if (slot_in_group_row) {
			// show row 1
			if (add_three_operand) {
				math::func::setPositionList(row_1, (int)row_1.size(), cocos2d::Rect(0.1, 0.25, 0.9, 0.25));
			}
			else {
				math::func::setPositionList(row_1, (int)row_1.size(), cocos2d::Rect(0.12, 0.3, 0.95, 0.25));
			}
			for (Math_Group *group : all_groups) {
				addChildNode(group, 2);
				group->onShow(0, math::animation::No_Effect);
			}
			for (Math_Object *object : all_objects_row_1) {
				addChildNode(object);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
			addChildNode(slot_1, 2);
			slot_1->onShow(0, math::animation::ScaleUpAndRotation);
			slot_1->setPositionX(row_1[row_1.size() - 2]->getPosition().x + 100);

			// show row 2
			if (add_three_operand) {
				math::func::setPositionList(row_2, (int)row_2.size(), cocos2d::Rect(0.1, 0, 0.9, 0.25));
			}
			else {
				math::func::setPositionList(row_2, (int)row_2.size(), cocos2d::Rect(0.12, 0, 0.95, 0.25));
			}
			for (Math_Object *object : all_objects_row_2) {
				addChildNode(object);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
			addChildNode(slot_2, 2);
			slot_2->onShow(0, math::animation::ScaleUpAndRotation);
			row_2[row_2.size() - 1]->setPositionX(row_2[row_2.size() - 2]->getPosition().x + 100);
			cocos2d::Vec2 posa, posb;
			auto image1 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
			image1->setScale9Enabled(true);
			posa = row_1[0]->getPosition() - cocos2d::Vec2(0, row_1[0]->getContentSize().height / 2);
			posb = row_2[0]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[0]->getContentSize().height / 2);
			image1->setContentSize(cocos2d::Size(5, posa.y - posb.y));
			addChildNode(image1, 0);
			image_line.push_back(image1);
			image1->setPosition((posa + posb) / 2);
			auto image2 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
			image2->setScale9Enabled(true); 
			posa = row_1[2]->getPosition() - cocos2d::Vec2(0, row_1[2]->getContentSize().height / 2);
			posb = row_2[2]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[2]->getContentSize().height / 2);
			image2->setContentSize(cocos2d::Size(5, posa.y - posb.y)); 
			addChildNode(image2, 0);
			image_line.push_back(image2);
			image2->setPosition((posa + posb) / 2);
			if (add_three_operand) {
				auto image3 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
				image3->setScale9Enabled(true);
				posa = row_1[4]->getPosition() - cocos2d::Vec2(0, row_1[4]->getContentSize().height / 2);
				posb = row_2[4]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[4]->getContentSize().height / 2);
				image3->setContentSize(cocos2d::Size(5, posa.y - posb.y));
				addChildNode(image3, 0);
				image_line.push_back(image3);
				image3->setPosition((posa + posb) / 2);
			}

			// show RESULTS
			math::func::setPositionList(results, (int)results.size(), cocos2d::Rect(0.1, 0, 0.6, 0.2));
			for (Math_Object *object : results) {
				addChildNodeToBottomPanel(object, 3);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
		}
		else if (!slot_in_group_row) {
			// show row 1 
			if (add_three_operand) { 
				math::func::setPositionList(row_1, (int)row_1.size(), cocos2d::Rect(0.1, 0.25, 0.95, 0.25));
			}else {
				math::func::setPositionList(row_1, (int)row_1.size(), cocos2d::Rect(0.1, 0.3, 0.95, 0.25));
			}
			addChildNode(row_1[0], 2); ((Math_Group*)row_1[0])->onShow(0, math::animation::No_Effect);
			addChildNode(row_1[1], 2); ((Math_Object*)row_1[1])->onShow(0, math::animation::ScaleUpAndRotation);
			addChildNode(row_1[2], 2); ((Math_Group*)row_1[2])->onShow(0, math::animation::No_Effect);
			if (add_three_operand) {
				addChildNode(row_1[3], 2); ((Math_Object*)row_1[3])->onShow(0, math::animation::ScaleUpAndRotation);
				addChildNode(row_1[4], 2); ((Math_Group*)row_1[4])->onShow(0, math::animation::No_Effect);
				row_1[6]->setPosition(cocos2d::Vec2(-1000, -1000));
			}
			else {
				row_1[4]->setPosition(cocos2d::Vec2(-1000, -1000));
			}

			// show row 2
			math::func::setPositionList(row_2, (int)row_2.size(), cocos2d::Rect(0.1, 0, 0.95, 0.25));
			for (Math_Object *object : all_objects_row_2) {
				addChildNode(object);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
			addChildNode(slot_2, 2);
			slot_2->onShow(0, math::animation::ScaleUpAndRotation);

			if (add_three_operand) {
				row_2[5]->setPosition(cocos2d::Vec2(row_2[4]->getPosition().x + 100, row_2[3]->getPosition().y));
				row_2[6]->setPosition(cocos2d::Vec2(row_2[5]->getPosition().x + 100, row_2[3]->getPosition().y));
			}
			else {
				row_2[3]->setPosition(cocos2d::Vec2(row_2[2]->getPosition().x + 150, row_2[2]->getPosition().y));
				row_2[4]->setPosition(cocos2d::Vec2(row_2[3]->getPosition().x + 150, row_2[3]->getPosition().y));
			}

			cocos2d::Vec2 posa, posb;
			auto image1 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
			image1->setScale9Enabled(true); 
			posa = row_1[0]->getPosition() - cocos2d::Vec2(0, row_1[0]->getContentSize().height / 2);
			posb = row_2[0]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[0]->getContentSize().height / 2);
			image1->setContentSize(cocos2d::Size(5, posa.y - posb.y)); 
			addChildNode(image1, 0);
			image_line.push_back(image1);
			image1->setPosition((posa + posb) / 2);
			auto image2 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
			image2->setScale9Enabled(true); 
			posa = row_1[2]->getPosition() - cocos2d::Vec2(0, row_1[2]->getContentSize().height / 2);
			posb = row_2[2]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[2]->getContentSize().height / 2);
			image2->setContentSize(cocos2d::Size(5, posa.y - posb.y)); 
			addChildNode(image2, 0);
			image_line.push_back(image2);
			image2->setPosition((posa + posb) / 2);
			if (add_three_operand) {
				auto image3 = cocos2d::ui::ImageView::create("themes/default/common/line/line_4.png");
				image3->setScale9Enabled(true);
				posa = row_1[4]->getPosition() - cocos2d::Vec2(0, row_1[4]->getContentSize().height / 2);
				posb = row_2[4]->getPosition() + cocos2d::Vec2(0, 0.8*row_2[4]->getContentSize().height / 2);
				image2->setContentSize(cocos2d::Size(5, posa.y - posb.y)); 
				addChildNode(image3, 0);
				image_line.push_back(image3);
				image3->setPosition((posa + posb) / 2);
			}

			// show RESULTS
			math::func::setPositionList(results, (int)results.size(), cocos2d::Rect(0.1, 0, 0.6, 0.2));
			for (Math_Object *object : results) {
				addChildNodeToBottomPanel(object, 2);
				object->onShow(0, math::animation::ScaleUpAndRotation);
			}
		}
	}

}

void Thanh_DoubleAddition::startQuestion() {
	CCLOG("%s override", __func__);

	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
		for (Math_Object* object : results) {
			object->enableDrag(_panel_bot);
			root_position.push_back(std::make_pair(object->getComparisionValue().number, object->getPosition()));
		}
	}
}

void Thanh_DoubleAddition::onExplanation(bool correct) {
	CCLOG("%s override", __func__);
	if (!correct)
	{
		disableControlQuiz(0.7);
		for (Math_Slot *slot : slots)
		{
			if (slot->isUsed()) {
				slot->getLinker()->onHide(0);
				/*for (std::vector<Math_Object*>::iterator it = results.begin(); it != results.end();) {
					if ((*it) == slot->getLinker()) {
						results.erase(it);
						break;
					}
					else it++;
				}*/
				slot->removeLinker();
			}
		}
	}
	else if (correct)
	{
		disableControlQuiz();
		this->nextQuestion();
	}
}

void Thanh_DoubleAddition::onComplete() {
	CCLOG("%s override", __func__);

	//Math_Pool::getInstance()->resetAnswers();

	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}

	m_delay = delay - 0.3;
	auto dt = cocos2d::DelayTime::create(0.3);

	auto call = cocos2d::CallFunc::create([=]() {
		// ẩn 2 group do nó có thời gian delay là 0.4
		for (Math_Group *group : all_groups) {
			group->setDisable(0);
		}
	});
	auto callA = cocos2d::CallFunc::create([=]() {
		// ẩn hàng 1
		for (Math_Object *object : all_objects_row_1) {
			object->onHide(0, math::animation::ScaleUpAndRotation);
		}
		slot_1->onHide(0, math::animation::ScaleUpAndRotation);
		// an day
		for (cocos2d::ui::ImageView *img : image_line) {
			img->removeFromParentAndCleanup(true);
		} 
	});
	auto callB = cocos2d::CallFunc::create([=]() {
		// ẩn hàng 2
		for (Math_Object *object : all_objects_row_2) {
			object->onHide(0, math::animation::ScaleUpAndRotation);
		}
		slot_2->onHide(0, math::animation::ScaleUpAndRotation);
	});
	auto callC = cocos2d::CallFunc::create([this]() {
		// ẩn number drag
		for (Math_Object* object : results) {
			object->onHide(0);
		}
	});
	this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.4), call, dt, callA, dt, callB, dt, callC, NULL));

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Thanh_DoubleAddition::onAnswerDone(bool correct)
{
	bool done = true;


	if (hide_group == false && slot_in_group_row) {
		if (slots[0]->isUsed() == false || slots[0]->getLinker()->getComparisionValue().number != result) done = false;

	}
	Math_Slot* slot = slots[1]; // slot cua hang 2
	if (slot->isUsed() == true) {
		switch (hide) {
		case Hide::operand_1:
			if (slot->getLinker()->getComparisionValue().number != a) done = false;
			break;
		case Hide::operand_2:
			if (slot->getLinker()->getComparisionValue().number != b) done = false;
			break;
		case Hide::operand_3:
			if (slot->getLinker()->getComparisionValue().number != c) done = false;
			break;
		case Hide::equal:
			if (slot->getLinker()->getComparisionValue().number != result) done = false;
			break;
		default:
			break;
		}
	}
	else {
		done = false;
	}
	this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_DoubleAddition::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_DoubleAddition::autoTestingRun(float dt)
{

}

#pragma mark - on this game
void Thanh_DoubleAddition::onTouchedObject(cocos2d::Node *node)
{
	Math_Object* object = (Math_Object*)node;
	object->setLocalZOrder(4); 
	for (Math_Slot *slot : slots) {
		if (slot->getLinker() == object) return;
	}
	Math_Object *object_copy = object->clone();  
	object_copy->onShow(0, math::animation::No_Effect);
	results.push_back(object_copy);
}

void Thanh_DoubleAddition::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	Math_Object* object = (Math_Object*)node;
	disableControlQuiz(0.25);

	object->removeFromParentAndCleanup(false);
	cocos2d::Vec2 position = _panel->convertToNodeSpace(point);
	object->setPosition(position);
	_panel->addChild(object, 2);
	root_position.size();

	//Doi tuong keo tha nam trong 1 slot nao do
	Math_Slot* m_slot = nullptr;
	for (Math_Slot* slot : slots) {
		if (object == slot->getLinker()) {
			m_slot = slot;
			break;
		}
	}

	Math_Slot* t_slot = nullptr;
	for (Math_Slot* slot : slots) {
		if (slot->isAvailable(position)) {
			t_slot = slot;
			break;
		}
	}

	CCLOG("SlotSize %d", (int)slots.size());

	if (m_slot != nullptr) {
		if (t_slot != nullptr) {
			// swap
			if (m_slot == t_slot) {
				object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			}
			else {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					m_slot->removeLinker();
					t_slot->removeLinker();

					t_slot->setLinker(object);
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

					m_slot->setLinker(linker);
					linker->runAction(cocos2d::MoveTo::create(0.2, m_slot->getPosition()));
				}
				else {
					m_slot->removeLinker();
					t_slot->setLinker(object);
  					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
				}
			}
		}
		else {
			//move to start
			m_slot->removeLinker();
			for (std::vector<Math_Object*>::iterator it = results.begin(); it != results.end(); it++) {
				if ((*it) == object) {
					results.erase(it);
					break;
				}
			}
			object->removeFromParentAndCleanup(false);
			cocos2d::Vec2 position = _panel_bot->convertToNodeSpace(point);
			object->setPosition(position);
			_panel_bot->addChild(object, 2);
			object->onHide(0, math::animation::ScaleUpAndRotation);
		}
	}
	else {
		if (t_slot != nullptr) {
			if (t_slot->isUsed()) {
				auto linker = t_slot->getLinker();
				for (std::vector<Math_Object*>::iterator it = results.begin(); it != results.end(); it++) {
					if ((*it) == linker) {
						results.erase(it);
						break;
					}
				}
				t_slot->removeLinker();
				linker->setVisible(false);
				linker->removeFromParentAndCleanup(false);
				cocos2d::Vec2 position = _panel_bot->convertToNodeSpace(point);
				linker->setPosition(position);
				_panel_bot->addChild(linker, 2);
				linker->onHide(0, math::animation::ScaleUpAndRotation);
			}
			t_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			object->setLocalZOrder(4);
		}
		else {
			//move start
			object->removeFromParentAndCleanup(false);
			cocos2d::Vec2 position = _panel_bot->convertToNodeSpace(point);
			object->setPosition(position);
			_panel_bot->addChild(object, 2);
			object->onHide(0, math::animation::ScaleUpAndRotation);
		}
	}
}
