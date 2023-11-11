#include "Minh_SetTheCalculation.h"
////////
//		Minh_SetTheCaculation.cpp
//		MJ_Game
//		Create By MinhTzy on 16/10/2017
//

Minh_SetTheCaculation * Minh_SetTheCaculation::createQuiz()
{
	Minh_SetTheCaculation * quiz = new Minh_SetTheCaculation();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Minh_SetTheCaculation::init()
{
	return cocos2d::ui::Layout::init();
}
#pragma mark - Quiz Life-cycle
void Minh_SetTheCaculation::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	rapidjson::Value & json_data = document.FindMember("data")->value;

	//TODO : Game Config

	rapidjson::Value & json_game = getJsonMember(json_data, "game");

	is_carry = getJsonBool(json_game, "carry");
	sequence = getJsonBool(json_game, "sequence");
	num_operand = getJsonInt(json_game, "num_op");
	op_type = getJsonString(json_game, "op_type", "+");
	range_minimum = getJsonInt(getJsonMember(json_game, "range"), "minimum");
	range_maximum = getJsonInt(getJsonMember(json_game, "range"), "maximum");
	digits.clear();
	if (json_game.HasMember("digits")) {
		rapidjson::Value  & digits_data = getJsonMember(json_game, "digits");
		for (rapidjson::SizeType i = 0; i < digits_data.Size(); ++i) {
			digits.push_back(digits_data[i].GetInt());
		}
	}
	
	//----> Lay xong data cua game

	//TODO: Xử lý dữ liệu cấu hình câu hỏi

	parseQuestion();

	//TODO: GENERATE QUIZ COMPONENTS

	float delay = 1;

	// Tao doi tuong
	math::common::TouchPermission touch_per = math::common::TouchPermission::deny;
	auto number_imgs = getNumbers(0, 9);

	// Gen number
	if ((int)digits.size() == 0) {
		GenOperand();
	}
	else {
		GenOperandDigits();
	}

	//  Gen object
	_objects.clear();
	int obj_id = 0;
	int group_id = 0;

	objects_list.clear();
	for (int v : operand_values) {
		std::vector<Math_Object * > objects;
		math::resource::Image* obj_img;
		objects.clear();

		std::vector<int> num_list;
		auto num_str = cocos2d::StringUtils::toString(v);
		for(int i = num_str.size()-1; i >= 0; --i) {
			for (math::resource::Image* img : number_imgs) {
				if (img->getComparisionValue("number") == (num_str[i] - '0')) {
					obj_img = img;
					break;
				}
			}
			Math_Object * object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, touch_per, obj_img, NULL);
			object->setObjectSize(object_size);
			// Show only text 
			object->setValueTextByKey("number");
			ShowValueTextOnly(object);
			object->onShow(delay);
			delay += 0.1;
			this->addChildNode(object, 2);
			objects.push_back(object);
			_objects.push_back(object);
		}

		objects_list.push_back(objects);
	}

	// Gen operators
	_operators.clear();
	math::resource::Image * img_operator;
	img_operator = getOperator(op_type);
	Math_Object * ope = Math_Pool::getInstance()->getObject();
	ope->setEnable(obj_id++, touch_per, img_operator, NULL);
	ope->onShow(delay);
	delay += 0.15;
	ope->setObjectSize(object_size * 0.75);
	addChildNode(ope, 2);
	_operators.push_back(ope);
	// Show only text 
	//ope->setValueText(img_operator->getText("name_1"));
	//ShowValueTextOnly(ope);
	// Equal Sign
	equal_sign = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
	equal_sign->retain();
	equal_sign->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	equal_sign->setScale9Enabled(true);
	equal_sign->setCapInsets(cocos2d::Rect(1, 1, equal_sign->getContentSize().width - 2, equal_sign->getContentSize().height - 2));
	//Gen Drager
	std::vector<int> drag_values;
	drag_values = { 0,1,2,3,4,5,6,7,8,9 };
	math::resource::Image * drag_bg = objectBG();
	for (int drag_v : drag_values) {
		math::resource::Image* drag_img;
		for (math::resource::Image* img : number_imgs) {
			if (img->getComparisionValue("number") == drag_v) {
				drag_img = img;
				break;
			}
		}
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, drag_img, drag_bg);
		object->setObjectSize(drager_size);
		object->setSwallowTouches(true);
		// Show only text 
		object->setValueTextByKey("number");
		ShowValueTextOnly(object);
		object->setMoveCallback(CC_CALLBACK_1(Minh_SetTheCaculation::onTouchedObject, this), CC_CALLBACK_2(Minh_SetTheCaculation::onMoveObjectEnded, this));
		object->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		_dragers.push_back(object);
	}
	//Gen Slot
	std::string s_s = cocos2d::StringUtils::toString(math_result);
	std::vector<int> slot_values;
	number_slot = (int)s_s.size();
	for (int i = (int)s_s.size() - 1; i >= 0; --i) {
		slot_values.push_back(int(s_s[i] - '0'));
	}

	math::resource::Image* slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
	int slot_id = 0;
	for (int slot_v : slot_values) {
		math::resource::Image* slot_img;
		for (math::resource::Image* img : number_imgs) {
			if (img->getComparisionValue("number") == slot_v) {
				slot_img = img;
				break;
			}
		}
		Math_Object* object = Math_Pool::getInstance()->getObject();
		object->setEnable(obj_id++, math::common::deny, slot_img, NULL);

		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(slot_id++, slot_bg);
		slot->setSlotSize(cocos2d::Size(object_size));
		slot->setComparisionValue(object->getComparisionValue());
		_slots.push_back(slot);

	}
	

	sequence_slot = NULL;
}

void Minh_SetTheCaculation::onStart()
{
	onShowRightBar(0, 300 / quiz_size.width, CC_CALLBACK_1(Minh_SetTheCaculation::onAnswerDone, this));
    quizBegan(0.5, true);
    
	///////////////////////////////
	// Set position and show 
	int line_size = 20;
	auto locations = math::func::getLocations(num_operand + 1, 1, cocos2d::Rect(0, 0.2, 1 - (300 - object_size.width) /quiz_size.width, 0.6), quiz_size);
	// find max_line
	int col = 0;
	for (int v : operand_values) {
		if (col < (int)cocos2d::StringUtils::toString(v).size()) col = (int)cocos2d::StringUtils::toString(v).size();
	}
	if (col < (int)cocos2d::StringUtils::toString(math_result).size()) col = (int)cocos2d::StringUtils::toString(math_result).size();

	equal_sign->setContentSize(cocos2d::Size(object_size.width  *(col + 1),line_size));
	// equal sign
	equal_sign->setPosition(cocos2d::Vec2(locations[num_operand].x - object_size.width /2, locations[num_operand].y + object_size.height * 0.5));
	// object
	int i = 0;
	for (auto obj_list : objects_list) {
		math::func::setPositionListOnFrame(obj_list, col, 1, object_size, cocos2d::Vec2(locations[i].x, locations[i].y), 1);
		++i;
	}

	// operator
	float delay = 1;
	math::func::setPositionListOnFrame(_operators, 1, (int)_operators.size(), object_size, cocos2d::Vec2(locations[0].x - (1 + col)* object_size.width / 2, locations[0].y - object_size.height / 2), 0);

	// #slot

	math::func::setPositionListOnFrame(_slots, col, 1, object_size, cocos2d::Vec2(locations[num_operand].x, locations[num_operand].y - object_size.height / 2), 1);
	// #drager
	//math::func::setPositionListOnFrame(_dragers, 2, 5, drager_size, cocos2d::Vec2(150, 0.5* quiz_size.height + 60), 0);
	auto locations_drager = math::func::getLocations((int)_dragers.size(), 2, cocos2d::Rect(50/quiz_size.width, 160 / quiz_size.height, 200 / quiz_size.width, 1 - 200 / quiz_size.height), quiz_size);
	for (int i = 0; i < _dragers.size(); ++i) {
		_dragers[i]->setPosition(locations_drager[i]);
	}
	//_dragers[9]->setPositionX(_dragers[9]->getPositionX() + _dragers[9]->getContentSize().width);

	// Show 
	//#1. Show equal sign
	scheduleOnce([&](float dt) {
		_panel->addChild(equal_sign);
	}, delay, "show equal sign");
	//#2. Show Slot
	for (Math_Slot* slot : _slots) {
		this->addChildNode(slot, 2);
		slot->onShow(delay);
		delay += 0.15;
	}
	//#3. show drager
	for (Math_Object* object : _dragers) {
		addChildNodeToRightPanel(object, 2);
		object->onShow(delay, math::animation::SlideLeft);
		delay += 0.075;
	}
}

void Minh_SetTheCaculation::startQuestion()
{
	enableControlQuiz();
	for (Math_Object* object : _dragers) {
		object->enableDrag(_panel_right);
	}
}

void Minh_SetTheCaculation::onExplanation(bool correct)
{
	if (!correct) {
        disableControlQuiz(0.25);
		//return wrong drager
		for (int i = 0; i < _slots.size(); ++i) {
			if (_slots[i]->isUsed() && !_slots[i]->isCorrect(key_compare)) {
				if (sequence) sequence_slot = _slots[i];
				for (int j = i; j < _slots.size(); ++j) {
					if (_slots[j]->isUsed()) {
						_slots[j]->getLinker()->onHide(0);
						_slots[j]->removeLinker();
					}
				}
				break;
			}
		}
	}
	else {
        disableControlQuiz();
		this->nextQuestion(0.75);
	}
}

void Minh_SetTheCaculation::onComplete()
{
	//TODO: Thuc hien them cac lenh o day
	float delay = 0;
	for (Math_Object* object : _objects) {
		object->onHide(delay);
		delay += 0.1;
	}
	delay = delay - 0.3;
	for (Math_Object * object : _operators) {
		object->onHide(delay);
		delay += 0.1;
	}
	scheduleOnce([=](float dt) {
		equal_sign->removeFromParentAndCleanup(true);
	}, delay, "remove_equal_sign");
	delay = delay - 0.3;
	for (Math_Object* object : _dragers) {
		object->onHide(delay);
		delay += 0.05;
	}
	for (Math_Slot* slot : _slots) {
		slot->onHide(delay);
	}

    quizEnded(0.75, true);
}


#pragma mark - object touch events callback
void Minh_SetTheCaculation::onTouchedObject(Node * node)
{
	Math_Object* object = (Math_Object*)node;
	if (sequence) {
		sequence_slot = NULL;
		for (int i = 0; i < _slots.size(); ++i) {
			if (!_slots[i]->isUsed()) {
				sequence_slot = _slots[i];
				break;
			}
		}
	}
	if (sequence && sequence_slot != NULL) sequence_slot->setBackgroundColor(cocos2d::Color3B::GRAY);
	object->setLocked(true);

	bool clone = true;
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed() && slot->getLinker()->isSame(object)) {
			clone = false;
			break;
		}
	}
	if (clone) {
		CloneDrager(object);
	}
	object->setObjectSize(object_size);
	ShowValueTextOnly(object);
}

void Minh_SetTheCaculation::onMoveObjectEnded(cocos2d::Node * node, cocos2d::Vec2 position)
{


	Math_Slot *  current_slot = NULL;
	Math_Slot * olded_slot = NULL;
	Math_Object* object = (Math_Object*)node;
	object->setLocked(false);
	//Doi tuong keo tha nam trong 1 slot nao do.
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed() && slot->getLinker()->isSame(object)) {
			olded_slot = slot;
			break;
		}
	}

	//Check slot dich.
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			current_slot = slot;
			break;
		}
	}


	if (sequence_slot != NULL) sequence_slot->setBackgroundColor(cocos2d::Color3B::WHITE);
	// Neu khong nam trong slot nao
	if (current_slot == NULL) {
		RemoveDrager(object);
		// Neu truoc do nam trong 1 slot 
		if (olded_slot != NULL) {
			olded_slot->removeLinker();
		}
	}
	// Neu nam trong 1 slot nao do
	else {
		// Neu truoc do k nam trong slot nao
		if (olded_slot == NULL) {
			if (sequence && sequence_slot != current_slot && !current_slot->isUsed()) {
				RemoveDrager(object);
				return;
			}
			if (current_slot->isUsed()) {
				RemoveDrager(current_slot->getLinker());
				current_slot->removeLinker();
			}
			current_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
		}
		// Neu truoc do nam o slot nao do
		else {
			if (sequence && !current_slot->isUsed()) {
				olded_slot->getLinker()->runAction(cocos2d::MoveTo::create(0.2, olded_slot->getPosition()));
				return;
			}
			if (current_slot == olded_slot) {
				object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
			}
			else {
				Math_Object * ocs = current_slot->getLinker();
				Math_Object * oos = olded_slot->getLinker();
				current_slot->removeLinker();
				olded_slot->removeLinker();

				current_slot->setLinker(oos);
				oos->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));

				if (ocs != NULL) {
					olded_slot->setLinker(ocs);
					ocs->runAction(cocos2d::MoveTo::create(0.2, olded_slot->getPosition()));
				}

			}
		}
	}
}


void Minh_SetTheCaculation::onAnswerDone(Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
    bool done = true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isCorrect(key_compare)) {
            done = false;
        }
    }
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

void Minh_SetTheCaculation::GenOperand()
{
	CCLOG("Debug : Gen operand...");
	operand_values.clear();
	if (op_type.compare("+") == 0) {
		math_result = cocos2d::random(range_minimum, range_maximum);
		CCLOG("Result %d", math_result);
		if (is_carry) {
			if (range_maximum < 10) {
				CCLOG("Maximum can be more than 10");
				return;
			}
			while (math_result < 10) {
				math_result = cocos2d::random(range_minimum, range_maximum);
			}
			if (math_result % 10 == 9 && is_carry) {
				if (num_operand > 2) {
					int  r = 9 + cocos2d::random(0, math_result / 10 - 1) * 10;
					operand_values.push_back(r);
				}
				else {
					do {
						math_result = cocos2d::random(range_minimum, range_maximum);
					} while (math_result >= 10);
				}
			}
		}

		int remain = math_result;
		for (int n : operand_values) remain -= n;
		bool carried = false;
		while (operand_values.size() < num_operand) {
			int r;
			bool add;
			do {
				add = true;
				if (operand_values.size() == num_operand - 1) {
					r = remain;
				}
				else {
					r = cocos2d::random(0, remain);
				}
				int a = remain;
				int b = r;
				if (is_carry) {
					if (carried) {
						add = true;
						continue;
					}
					while (a > 0 && b > 0) {
						add = false;
						if (a % 10 - b % 10 < 0) {
							add = true;
							carried = true;
							break;
						}
						a /= 10; b /= 10;
					}
				}
				else {
					while (a > 0 && b > 0) {
						if (a % 10 - b % 10 < 0) {
							add = false;
							break;
						}
						a /= 10; b /= 10;
					}
				}
			} while (!add);
			if (r != 0) {
				operand_values.push_back(r);
				remain -= r;
			}
			else if (remain == 0) {
				this->GenOperand();
				return;
			}
		}
	}

	else if (op_type.compare("-") == 0) {
		int sub;
		sub = cocos2d::random(range_minimum, range_maximum);
		if (is_carry) {
			if (range_maximum < 10) {
				CCLOG("Maximum can be more than 10");
				return;
			}
			while (sub < 10 || sub % 10 == 9) {
				sub = cocos2d::random(range_minimum, range_maximum);
			}
		}

		operand_values.push_back(sub);

		int remain = sub;

		bool carried = false;
		while (operand_values.size() < num_operand) {
			int r;
			bool add;
			do {
				add = true;
				r = cocos2d::random(0, remain);
				int a = remain;
				int b = r;
				if (is_carry) {
					if (carried) {
						add = true;
						continue;
					}
					while (a > 0 && b > 0) {
						add = false;
						if (a % 10 - b % 10 < 0) {
							add = true;
							carried = true;
							break;
						}
						a /= 10; b /= 10;
					}
				}
				else {
					while (a > 0 && b > 0) {
						if (a % 10 - b % 10 < 0) {
							add = false;
							break;
						}
						a /= 10; b /= 10;
					}
				}
			} while (!add);
			if (r != 0) {
				operand_values.push_back(r);
				remain -= r;
			}
			else if (remain == 0) {
				this->GenOperand();
				return;
			}
		}
		math_result = remain;
	}
	//for (int n : operand_values)  CCLOG("%d ", n);
	//CCLOG("Debug : Gen operand complete");
}

void Minh_SetTheCaculation::GenOperandDigits()
{
	CCLOG("Debug : Gen Operand by digits");
	operand_values.clear();
	math_result = 0;
	int remain = range_maximum;
	if (op_type.compare("+") == 0) {

		for (int i : digits) {
			int r_min = (int)std::pow(10, i - 1);
			int r_max = (int)std::min((int)std::pow(10, i) - 1, remain);
			if (r_min > r_max){
				this->GenOperandDigits();
				return;
			}
			int r = cocos2d::random<int>(r_min, r_max);
			remain -= r;
			operand_values.push_back(r);
			math_result += r;
		}
		if (math_result < range_minimum || math_result > range_maximum) {
			this->GenOperandDigits();
			return;
		}
	}
	else {

	}
	CCLOG("Debug : Gen Complete");

}

void Minh_SetTheCaculation::CloneDrager(Math_Object * object)
{
	// Gen Drager thay the

	CCLOG("Clone Drager Object");
	Math_Object * drag_object = object->clone();
	// Show only text 
	ShowValueTextOnly(drag_object);
	CCLOG("Drager Postiton : %f %f", drag_object->getPositionX(), drag_object->getPositionY());
	_dragers.push_back(drag_object);
	drag_object->removeFromParentAndCleanup(false);
	this->addChildNodeToRightPanel(drag_object, 3);
	drag_object->enableDrag(_panel_right);
	drag_object->onShow(0);
}

void Minh_SetTheCaculation::RemoveDrager(Math_Object * object)
{
	object->onHide(0);
	for (int i = 0; i < _dragers.size(); i++) {
		if (_dragers[i]->isSame(object)) {
			_dragers.erase(_dragers.begin() + i);
			break;
		}
	}
}

void Minh_SetTheCaculation::ShowValueTextOnly(Math_Object * object, std::string position)
{
	object->setValueTextPosition(position);
	object->getLabelValue()->setFontSize(0.9 * object->getContentSize().height);
	object->showValueTextWithAnimation(true);
	object->setImageVisible(false);
}
