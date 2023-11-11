//
//  Nam_CompareObjectVsGroup.cpp
//  MJQuiz
//
//  Created by tunado on 8/10/17.
//
//

#include "Nam_CompareObjectVsGroup.h"

Nam_CompareObjectVsGroup* Nam_CompareObjectVsGroup::createQuiz() {
	Nam_CompareObjectVsGroup* quiz = new Nam_CompareObjectVsGroup();

	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}
bool Nam_CompareObjectVsGroup::init() {
	return cocos2d::ui::Layout::init();
}
void Nam_CompareObjectVsGroup::initQuiz(mj::model::Game game) {

	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	//TODO: lấy dữ liệu data
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	game_mode = json_game.FindMember("game mode")->value.GetString();
	int min = json_game.FindMember("number")->value.FindMember("min")->value.GetInt();
	int max = json_game.FindMember("number")->value.FindMember("max")->value.GetInt();
	if (game_mode == "AddOperator") {
		maxObjectInGroup = max;
		number_object = RandomHelper::random_int(min, max);
		number_object_in_group = RandomHelper::random_int(min, max);
		if (number_object == number_object_in_group) key = "equal";
		else
			if (number_object > number_object_in_group) key = "more";
			else key = "less";
	}
	else {
		maxObjectInGroup = max;
		key = "equal";
		number_object = RandomHelper::random_int(min, max);
		number_object_in_group = number_object;
	}

	auto number_list = getNumbers(number_object, number_object);

	//TODO: lấy ảnh trong cac media
	std::vector <math::resource::Image*> all_images_group = getImageList("icons", 1, "none");
	image_group = all_images_group.front();
	image_group->used = true;

	auto img_ope_equal  = getOperator("=");
	auto img_ope_more = getOperator(">");
	auto img_ope_less = getOperator("<");

	all_images_operator.push_back(img_ope_equal);
	all_images_operator.push_back(img_ope_more);
	all_images_operator.push_back(img_ope_less);

	for (math::resource::Image* img : all_images_operator) {
		img->used = true;
	}
	//TODO: lấy dữ liệu object
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	value_type_object = json_object.FindMember("value")->value.FindMember("type")->value.GetString();
	float object_height = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	float object_width = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();
	object_size.height = object_height;
	object_size.width = object_width;
	object_bg = json_object.FindMember("background")->value.GetString();
	img_bg = objectBG();

	//TODO: lấy dữ liệu drager
	rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
	float drager_width = json_drager.FindMember("size")->value.FindMember("width")->value.GetDouble();
	float drager_height = json_drager.FindMember("size")->value.FindMember("height")->value.GetDouble();
	drager_size.setSize(drager_width, drager_height);
	display_drager = json_drager.FindMember("display")->value.GetString();

	//TODO: GROUP CONFIG
	rapidjson::Value& json_group = json_data.FindMember("group")->value;
	rapidjson::Value& group_names = json_group.FindMember("name")->value;
	std::vector<std::string> list_name;
	for (rapidjson::SizeType i = 0; i<group_names.Size(); i++) {
		list_name.push_back(group_names[i].GetString());
	}
	object_align = json_group.FindMember("align")->value.GetString();
	group_type = json_group.FindMember("type")->value.GetString();
	group_column = json_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
	group_row = json_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
	group_size = cocos2d::Size(json_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("size")->value.FindMember("height")->value.GetInt());
	object_group_size = cocos2d::Size(json_group.FindMember("child_size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("child_size")->value.FindMember("height")->value.GetInt());
	if (game_mode == "AddOperator") {
		object_group_size = Size(80, 80);
	}
	else object_group_size = Size(80, 80);

	/// FIX GAME CHUNG
	//TODO:Gen Slot
    math::resource::Image* slot_bg = groupBG();
	slot = mpool->getSlot();
	slot->setEndble(1, slot_bg);
	slot->setSlotSize(object_size);
	slot->setPosition(Vec2(quiz_size.width/2 - 170, 210));
	_slots.push_back(slot);

	//TODO:Gen Object
	object = mpool->getObject();
	//fix tam image
	object->setEnable(number_object, math::common::TouchPermission::deny, number_list.front(), NULL);
	object->setObjectSize(object_size);
	object->setPosition(Vec2(slot->getPositionX() - 180, 210));
	object->setValueTextByKey("number");
	object->getLabelValue()->setFontSize(100);
	object->setImageVisible(false);
	object->getLabelValue()->setVisible(true);
	object->setBackgroundVisible(false);
    object->setLocked(true);
	_objects.push_back(object);
	// TODO:Gen Group
	gr_type = math::group::custom;
	if (group_type.compare("frame") == 0) {
		gr_type = math::group::frame;
	}
	if (gr_type == math::group::custom) {
		std::vector<Math_Object*> _group_list;
		if (game_mode == "AddOperator") {
			for (int i = 0; i < number_object_in_group; i++) {
				Math_Object* dot = mpool->getObject();
				dot->setEnable(1, math::common::TouchPermission::deny, image_group, NULL);
				dot->setObjectSize(Size(object_group_size));
				_group_list.push_back(dot);
			}
		}
		Math_Group* group = mpool->getGroup();
		group->setEnable(0, _group_list, gr_type, maxObjectInGroup);
		group->setObjectSize(cocos2d::Size(object_group_size));
		group->setGroupSize(group_size);
		group->setBackground(slot_bg);
		group->setPosition(Vec2(slot->getPositionX() + group_size.width / 2 + 200, 210));
		group->updatePosition(false, true);
		_groups.push_back(group);
	}
	else {
		int groups_size = (number_object_in_group - 1) / (group_column*group_row) + 1;
		for (int i = 0; i < groups_size; i++) {
			std::vector<Math_Object*> _group_list;
			if (game_mode == "AddOperator") {
				int tmp;
				if (object_align == "right" || object_align == "left") {
					if (i != (groups_size - 1)) tmp = (group_column*group_row);
					else tmp = number_object_in_group - (groups_size - 1) * (group_column*group_row);
				}
				else {
					if (i != (groups_size - 1)) tmp = number_object_in_group / groups_size;
					else tmp = number_object_in_group - (groups_size - 1) * number_object_in_group / groups_size;
				}
				for (int i = 0; i < tmp; i++) {
					Math_Object* dot = mpool->getObject();
					dot->setEnable(1, math::common::TouchPermission::deny, image_group, NULL);
					dot->setObjectSize(Size(object_group_size));
					_group_list.push_back(dot);
				}
			}
			Math_Group* group = mpool->getGroup();
			group->setEnable(i, _group_list, gr_type, group_column*group_row);
			group->setObjectSize(cocos2d::Size(object_group_size));
			group->setGroupFrames(group_column, group_row, slot_bg, NULL);
			if (object_align == "right") {
				group->setAlignObjects(math::group::GroupAlign::right);
				group->updatePosition(false, true);
			}
			else
				if (object_align == "left") {
					group->setAlignObjects(math::group::GroupAlign::left);
					group->updatePosition(false, true);
				}
				else {
					group->setAlignObjects(math::group::GroupAlign::random);
					if (game_mode == "AddOperator") {
						std::vector <std::pair<int, int> > rand;
						for (int i = 0; i < group->getNumberObjects(); i++) {
							while (1) {
								int c = RandomHelper::random_int(0, group_column - 1);
								int r = RandomHelper::random_int(0, group_row - 1);
								bool kt = true;
								for (int i = 0; i < rand.size(); i++) {
									if (c == rand[i].first && r == rand[i].second) {
										kt = false;
										break;
									}
								}
								if (kt) {
									rand.push_back({ c,r });
									break;
								}
							}
							group->getObjectAtIndex(i)->setPosition(group->getPositionAtFrame(rand[i].first, rand[i].second));
						}
						rand.clear();
						group->updatePosition(false, true);
					}
					else {

					}
				}

				_groups.push_back(group);
		}
	}

	// TODO: Gen Drager
	object_equals = mpool->getObject();
	object_equals->setEnable(0, math::common::TouchPermission::move, img_ope_equal, NULL);
	object_equals->setObjectSize(drager_size);
	object_equals->setPosition(Vec2(450, 150));
	object_equals->setImageVisible(false);
	object_equals->setValueTextByKey("name_n");
	object_equals->showValueTextWithAnimation(false);
	object_equals->getLabelValue()->setFontSize(drager_size.height * 0.75);
	object_equals->setMoveCallback(nullptr, CC_CALLBACK_2(Nam_CompareObjectVsGroup::onMoveObjectEnded, this));
	_objects.push_back(object_equals);

	object_greather = mpool->getObject();
	object_greather->setEnable(0, math::common::TouchPermission::move, img_ope_more, NULL);
	object_greather->setObjectSize(drager_size);
	object_greather->setPosition(Vec2(320, 150));
	object_greather->setImageVisible(false);
	object_greather->setValueTextByKey("name_n");
	object_greather->showValueTextWithAnimation(false);
	object_greather->getLabelValue()->setFontSize(drager_size.height * 0.75);
	object_greather->setMoveCallback(nullptr, CC_CALLBACK_2(Nam_CompareObjectVsGroup::onMoveObjectEnded, this));
	_objects.push_back(object_greather);

	object_less = mpool->getObject();
	object_less->setEnable(0, math::common::TouchPermission::move, img_ope_less, NULL);
	object_less->setObjectSize(drager_size);
	object_less->setPosition(Vec2(580, 150));
	object_less->setImageVisible(false);
	object_less->setValueTextByKey("name_n");
	object_less->showValueTextWithAnimation(false);
	object_less->getLabelValue()->setFontSize(drager_size.height * 0.75);
	object_less->setMoveCallback(nullptr, CC_CALLBACK_2(Nam_CompareObjectVsGroup::onMoveObjectEnded, this));
	_objects.push_back(object_less);

	// show
	float delay = 0.3;
	if (this->game_mode == "AddOperator") {
		if (gr_type == math::group::frame) {
			math::func::setPositionList(_groups, 1, cocos2d::Rect(0.5, 0.28, 0.4, 0.55));
		}
		this->addChildNode(object, 1);
		object->onShow(delay);
		delay += 0.2;
		this->addChildNode(_slots[0], 1);
		_slots[0]->onShow(delay);
		delay += 0.2;
		for (Math_Group* gr : _groups) {
			this->addChildNode(gr,2);
			gr->onShow();
		}
		for (int i = 0; i < 3; i++) {
			this->addChildNode(_objects.at(i), 5);
			_objects.at(i)->onShow(delay);
			_objects.at(i)->enableDrag(_panel);
			delay += 0.2;
		}
	}
	else {
		// set position theo ui 
		_slots[0]->setBackground(NULL);
		if (gr_type == math::group::frame) {
			if (_groups.size() < 2) {
				math::func::setPositionList(_groups, 1, cocos2d::Rect(0.5, 0.28, 0.4, 0.55));
				_slots[0]->setPositionX(_slots[0]->getPositionX() - 10);
				//object->setPositionX(object->getPositionX() + 20);
				_groups[0]->setPositionX(_groups[0]->getPositionX() - 10);
				_slots[0]->setPositionY(_slots[0]->getPositionY() + 20);
				object->setPositionY(object->getPositionY() + 20);
				_groups[0]->setPositionY(_groups[0]->getPositionY() + 20);
			}
			else {
				math::func::setPositionList(_groups, 1, cocos2d::Rect(0.5, 0.2, 0.4, 0.63));
				_slots[0]->setPositionY(_slots[0]->getPositionY() - 20);
				_slots[0]->setPositionX(_slots[0]->getPositionX() - 10);
				object->setPositionY(object->getPositionY() - 20);
			}
		}
		else {
			_slots[0]->setPositionX(_slots[0]->getPositionX() + 10);
			object->setPositionX(object->getPositionX() + 20);
			_groups[0]->setPositionX(_groups[0]->getPositionX() - 10);
			_slots[0]->setPositionY(_slots[0]->getPositionY() + 10);
			object->setPositionY(object->getPositionY() + 10);
			_groups[0]->setPositionY(_groups[0]->getPositionY() + 10);
		}
		for (Math_Group* gr : _groups) {
			this->addChildNode(gr,2);
            // fix lai
			//gr->setPositionX(gr->getPositionX() + 200);
			//gr->setPositionY(gr->getPositionY() - 200);
			gr->onShow();
		}
		this->addChildNode(object, 1);
		object->onShow(delay);
		delay += 0.2;
		this->addChildNode(_slots[0], 1);
		_slots[0]->onShow(delay);
		delay += 0.2;
        object_equals->setLocked(true);
        object_equals->setPosition(slot->getPosition());
        object_equals->setLocked(true);
        _slots[0]->setLinker(object_equals);
        this->addChildNode(object_equals, 2);
        object_equals->onShow(delay);
        
        delay += 0.1;
        /*object_equal->
		for (int i = 0; i < 3; i++) {
			if (_objects.at(i)->getComparisionValue().name == "equal") {
				_objects.at(i)->setLocked(true);
				_objects.at(i)->setPosition(slot->getPosition());
				_slots[0]->setLinker(_objects.at(i));
				this->addChildNode(_objects.at(i), 2);
				_objects.at(i)->onShow(delay);
				delay += 0.1;
			}
		}*/
		Math_Object* dragger_object = mpool->getObject();
		this->createGroupObject(dragger_object);
		this->addChildNodeToBottomPanel(dragger_object, 2);
		dragger_object->onShow(delay);
		delay += 0.2;
		objects_dot.push_back(dragger_object);
	}

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : number_list) {
				auto img_txt = img->getTextValue("name_1");
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "number_text") {
			std::vector<math::resource::Text> texts;
			for (math::resource::Image* img : number_list) {
				auto img_txt = img->getTextValue("name_n");
				img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
				texts.push_back(img_txt);
			}
			getQuestion()->setOptionalTexts(key, texts);
		}
		else if (key == "name_n") {
			if (number_object < 2) {
				std::vector<math::resource::Text> texts;
				for (math::resource::Image* img : all_images_group) {
					auto img_txt = img->getTextValue("name_1");
					texts.push_back(img_txt);
				}
				getQuestion()->setOptionalTexts(key, texts);
			}
			else {
				getQuestion()->setOptionalTexts(key, all_images_group);
			}
		}
		else {
			getQuestion()->setOptionalTexts(key, all_images_group);
		}
	}
	// set lai position
	/*for (Math_Object* obj : _objects) {
		obj->setPositionY(obj->getPositionY() - 200);
		obj->setPositionX(obj->getPositionX() + 200);
	}*/


}

void Nam_CompareObjectVsGroup::onStart() {
	//Gen question
	onShowBottomBar(0, 0.25,true, CC_CALLBACK_1(Nam_CompareObjectVsGroup::onAnswerDone, this));
	showQuestionBox();
}

void Nam_CompareObjectVsGroup::startQuestion() {
	enableControlQuiz();
	for (Math_Object* obj : _objects) {
		obj->enableDrag(_panel);
	}
}

void Nam_CompareObjectVsGroup::onExplanation(bool correct) {
	// 
}

void Nam_CompareObjectVsGroup::onComplete() {

	//TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	for (Math_Object* obj : _objects) {
		obj->onHide(m_delay += 0.02);
	}
	object->onHide(m_delay += 0.02);
	for (Math_Object* drg : objects_dot) {
		drg->onHide(m_delay += 0.02);
	}
	scheduleOnce([=](float dt) {
		for (Math_Group* group : _groups) {
			group->setDisable();
		}
	}, m_delay += 0.02, "hide_group");
	for (Math_Slot* slot : _slots) {
		slot->onHide(m_delay += 0.02);
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Nam_CompareObjectVsGroup::createGroupObject(Math_Object* object) {
	object->setEnable(0, math::common::TouchPermission::move, image_group, NULL);
	object->setObjectSize(Size(object_group_size));
	object->setPosition(Vec2(quiz_size.width*0.38, quiz_size.height*0.1));
	object->enableDrag(_panel_bot);
	object->setMoveCallback(CC_CALLBACK_1(Nam_CompareObjectVsGroup::onTouchedObject, this), CC_CALLBACK_2(Nam_CompareObjectVsGroup::onMoveObjectEnded, this));
}

void Nam_CompareObjectVsGroup::onAnswerDone(Node* node) {
	auto btn_done = (Math_Answer*)node;
	if (game_mode == "AddOperator") {
		if (_slots[0]->isUsed() && _slots[0]->getLinker()->getComparisionValue().name != key) {
			btn_done->onAnimationClick(false);
			_slots[0]->getLinker()->backToStart();
			_slots[0]->removeLinker();
		}
		else if (_slots[0]->isUsed()) {
			btn_done->onAnimationClick(true);
			this->nextQuestion();
		}
	}
	else {
		int number = 0;
		for (int i = 0; i < _groups.size(); i++) {
			number += _groups[i]->getNumberObjects();
		}
		CCLOG("%d %d", number, number_object_in_group);
		if (number == number_object_in_group) {
			btn_done->onAnimationClick(true);
			this->nextQuestion();
		}
		else btn_done->onAnimationClick(false);
	}
}

void Nam_CompareObjectVsGroup::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
	if (selected_object == NULL) {
		selected_object = object;
		first_pos = object->getPosition();
	}
	else {
		object->setAllowMove(false);
	}
}

void Nam_CompareObjectVsGroup::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
	Math_Object* object = (Math_Object*)node;
	position = _panel->convertToNodeSpace(position);
	object->removeFromParentAndCleanup(false);
	object->setPosition(position);
	addChildNode(object, 2);
	if (game_mode == "AddOperator") {
		Math_Object* object = (Math_Object*)node;

		//Doi tuong keo tha nam trong 1 slot nao do.
		Math_Slot* m_slot = NULL;
		for (Math_Slot* slot : _slots) {
			if (object->isSame(slot->getLinker())) {
				m_slot = slot;
				break;
			}
		}

		Math_Slot* t_slot = NULL;
		for (Math_Slot* slot : _slots) {
			if (slot->isAvailable(position)) {
				t_slot = slot;
				break;
			}
		}

		if (m_slot != NULL) {
			if (t_slot != NULL) {
				//swap
				if (m_slot == t_slot) {
					object->backToLast(2);
				}
				else {
					if (t_slot->isUsed()) {
						auto linker = t_slot->getLinker();
						m_slot->removeLinker();
						t_slot->removeLinker();

						t_slot->setLinker(object);
						object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

						m_slot->setLinker(linker);
						linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
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
				object->backToStart();
			}
		}
		else {
			if (t_slot != NULL) {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					t_slot->removeLinker();
					linker->backToStart();
				}
				t_slot->setLinker(object);
				object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
			}
			else {
				//move start
				object->backToStart();
			}
		}
		selected_object = NULL;
	}
	else {
		//remove object khoi group hien tai
		bool isInGroup = false;
		for (Math_Group* _group : _groups) {
			if (_group->hasObject(object)) {
				if (_group->isTouchOnGroup(position) && gr_type == math::group::frame && object_align != "random") {
					object->backToLast(2);
					selected_object = NULL;
					return;
				}
				_group->removeObject(object);
				isInGroup = true;
				break;
			}
		}
		Math_Group* group = NULL;
		for (Math_Group* _group : _groups) {
			if (_group->isTouchOnGroup(position)) {
				group = _group;
				break;
			}
		}
		if (group != NULL) {
			//if (gr_type == math::group::GroupType::custom) object->removeFromParent();
			if (group->addObject(object, position)) {
				if (!isInGroup) {
					Math_Object* drager_object = mpool->getObject();
					this->createGroupObject(drager_object);
					this->addChildNodeToBottomPanel(drager_object, 2);
					drager_object->onShow(0);
					objects_dot.push_back(drager_object);
				}

				//Check answer
				selected_object = NULL;
				return;
			}
		}
		if (isInGroup) {
			object->setDisable();
		}
		else {
			object->backToLast(2);
		}
		selected_object = NULL;
	}
}

#define auto_testing_delay 2
void Nam_CompareObjectVsGroup::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Nam_CompareObjectVsGroup::autoTestingRun(float delaytime) {
	/*if (game_mode == "AddOperator") {
		int k = RandomHelper::random_int(0, 2);
		Vec2 pos = slot->getPosition();
		auto move = TargetedAction::create(_objects[k], MoveTo::create(0.5f, pos));
		this->runAction(Sequence::create(move,
			CallFunc::create(
				[=] {
			this->onMoveObjectEnded(_objects[k], this->convertToWorldSpace(pos));
		}),
			DelayTime::create(1.f),
			CallFunc::create(
				[=] {
			this->onAnswerDone(true);
		}),
			DelayTime::create(1.f),
			CallFunc::create(
				[=] {
			this->autoTestingRun(delaytime);
		})
			, nullptr));
	}
	else {
		int index_group;
		while (1) {
			index_group = RandomHelper::random_int(0, (int)_groups.size() - 1);
			if (gr_type == math::group::GroupType::custom) break;
			if (_groups[index_group]->getNumberObjects() < group_column*group_row) break;
		}
		if (testing_step < 20) {
			Vec2 pos = _groups[index_group]->getPosition();
			if (gr_type == math::group::GroupType::frame) {
				auto move = TargetedAction::create(objects_dot[objects_dot.size() - 1], MoveTo::create(0.5f, pos));
				this->runAction(Sequence::create(move,
					CallFunc::create(
						[=] {
					objects_dot[objects_dot.size() - 1]->removeFromParentAndCleanup(false);
					this->addChild(objects_dot[objects_dot.size() - 1]);
					objects_dot[objects_dot.size() - 1]->onShow(0);
					objects_dot[objects_dot.size() - 1]->setPosition(this->convertToWorldSpace(pos));
					//this->onMoveObjectEnded(objects_dot[objects_dot.size() - 1], this->convertToWorldSpace(pos));
					if (_groups[index_group]->addObject(objects_dot[objects_dot.size() - 1], this->convertToWorldSpace(pos))) {
						Math_Object* drager_object = mpool->getObject();
						this->createGroupObject(drager_object);
						this->addChildNode(drager_object, 2);
						drager_object->onShow(0);
						objects_dot.push_back(drager_object);
					}
					else {
						objects_dot[objects_dot.size() - 1]->runAction(MoveTo::create(0.5f, Vec2(450, 200)));

					}
				}),
					DelayTime::create(1.f),
					CallFunc::create(
						[=] {
					this->onAnswerDone(true);
				}),
					DelayTime::create(1.f),
					CallFunc::create(
						[=] {
					int number = 0;
					for (int i = 0; i < _groups.size(); i++) {
						number += _groups[i]->getNumberObjects();
					}
					if (number != number_object_in_group) this->autoTestingRun(delaytime);
				})
					, nullptr));
			}
			else {
				pos.x = pos.x + RandomHelper::random_real(-_groups[index_group]->getContentSize().width / 2, _groups[index_group]->getContentSize().width / 2);
				pos.y = pos.y + RandomHelper::random_real(-_groups[index_group]->getContentSize().height / 2, _groups[index_group]->getContentSize().height / 2);
				auto move = TargetedAction::create(objects_dot[objects_dot.size() - 1], MoveTo::create(0.5f, pos));
				this->runAction(Sequence::create(move,
					CallFunc::create(
						[=] {
					objects_dot[objects_dot.size() - 1]->removeFromParentAndCleanup(false);
					this->addChild(objects_dot[objects_dot.size() - 1]);
					objects_dot[objects_dot.size() - 1]->onShow(0);
					objects_dot[objects_dot.size() - 1]->setPosition(this->convertToWorldSpace(pos));
					//this->onMoveObjectEnded(objects_dot[objects_dot.size() - 1], this->convertToWorldSpace(pos));
					if (_groups[index_group]->addObject(objects_dot[objects_dot.size() - 1], this->convertToWorldSpace(pos))) {
						Math_Object* drager_object = mpool->getObject();
						this->createGroupObject(drager_object);
						this->addChildNode(drager_object, 2);
						drager_object->onShow(0);
						objects_dot.push_back(drager_object);
					}
					else {
						objects_dot[objects_dot.size() - 1]->runAction(MoveTo::create(0.5f, Vec2(450, 200)));
					}
				}),
					DelayTime::create(1.f),
					CallFunc::create(
						[=] {
					this->onAnswerDone(true);
				}),
					DelayTime::create(1.f),
					CallFunc::create(
						[=] {
					int number = 0;
					for (int i = 0; i < _groups.size(); i++) {
						number += _groups[i]->getNumberObjects();
					}
					if (number != number_object_in_group) this->autoTestingRun(delaytime);
				}), NULL));
			}
		}
	}*/
}
