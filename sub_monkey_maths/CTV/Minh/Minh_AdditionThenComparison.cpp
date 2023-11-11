#include "Minh_AdditionThenComparison.h"
////////
//		Minh_AdditionThenComparison.cpp
//		MJ_Game
//		Create By MinhTzy on 08/11/2017
//


Minh_AdditionThenComparison * Minh_AdditionThenComparison::createQuiz()
{
	Minh_AdditionThenComparison * quiz = new Minh_AdditionThenComparison();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Minh_AdditionThenComparison::init()
{
	return cocos2d::ui::Layout::init();
}
#pragma mark - Quiz Life-cycle
void Minh_AdditionThenComparison::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
	rapidjson::Value & json_data = document.FindMember("data")->value;

	//TODO : Game Config
	rapidjson::Value & game_data = getJsonMember(json_data,"game");
	// Operator
	rapidjson::Value & operator_data = getJsonMember(game_data, "operator"); 
	rapidjson::Value & operation_data = getJsonMember(operator_data, "operation"); 
	for (rapidjson::SizeType index = 0; index < operation_data.Size(); ++index) {
		operation.push_back(operation_data[index].GetString());
	}
	number_min = getJsonInt(getJsonMember(operator_data, "number"), "minimum",1); 
	number_max = getJsonInt(getJsonMember(operator_data, "number"), "maximum",9); 
	number_operation_min = getJsonInt(getJsonMember(operator_data, "number_operation"), "minimum",1);
	number_operation_max = getJsonInt(getJsonMember(operator_data, "number_operation"), "maximum",1);
	
    number_operator = randomRangeJson(game_data, "number_operator");

    // Object
	object_size = getSizeJson(game_data, "size");
    
	// Dragers
	rapidjson::Value & drager_data = getJsonMember(json_data, "drager");
	display = getJsonString(drager_data, "display");

	// TODO : GET ICONS OPERATOR & NUMBER
	//----> Lay xong data cua game
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();

	//TODO: GENERATE QUIZ COMPONENTS
	// #1. Gen Sentence
	for (int i = 0; i < number_operator; ++i) {
		auto left = genOperator();
		auto right = genOperator();
		_operators.insert({ i * 2, left });
		_operators.insert({ i * 2 + 1, right });
	}
    
	for (int i = 0; i < number_operator; ++i) {
		auto left = _operators.at(i * 2);
		auto right = _operators.at(i * 2 + 1);
		Math_Object * left_objects;
		Math_Object * right_objects;
		// Left Object 
		left_objects = genOperatorObject(left);
		// Right Object
		right_objects = genOperatorObject(right);

		_operators_object.insert({ 2 * i,left_objects });
		_operators_object.insert({ 2 * i + 1,right_objects });
	}
	
	// #2. Gen Dragers
	int drag_id = 100;
	std::vector<std::string> drag_values = { "<","=",">" };
		
	math::resource::Image * drag_bg = objectBG();
    float max_width = 0;
	for (int i = 0; i < drag_values.size(); ++i) {
		Math_Object * object = mpool->getObject();
		object->setEnable(drag_id++, math::common::TouchPermission::move, getOperator(drag_values[i]), drag_bg);
		object->setObjectSize(object_size);
		if (display == "text") {
			showObjectWithText(object);
            object->setValueTextByKey("name_n");
			object->getLabelValue()->setFontSize(object_size.height*0.5);
            if (max_width < object->getLabelValue()->getContentSize().width + 40) {
                max_width = object->getLabelValue()->getContentSize().width + 40;
            }
		}
		object->setMoveCallback(CC_CALLBACK_1(Minh_AdditionThenComparison::onTouchedObject, this), CC_CALLBACK_2(Minh_AdditionThenComparison::onMoveObjectEnded, this));
		_dragers.push_back(object);
	}
    
    if (display == "text") {
        float font_size = object_size.height*0.5;
        while (max_width > (quiz_size.width - 450)/3) {
            max_width = 0;
            font_size -= 3;
            for (int i=0; i<3; i++) {
                _dragers[i]->getLabelValue()->setFontSize(font_size);
                if (max_width < _dragers[i]->getLabelValue()->getContentSize().width + 40) {
                    max_width = _dragers[i]->getLabelValue()->getContentSize().width + 40;
                }
            }
        }
        for (int i=0; i<3; i++) {
            _dragers[i]->setObjectSize(cocos2d::Size(max_width, object_size.height));
        }
    }
    
	// #3. Gen Slots
    math::resource::Image* slot_bg = slotBG();
    for (int i = 0; i < number_operator; ++i) {
		auto left = _operators.at(2 * i);
		auto right = _operators.at(2 * i + 1);
        int index = 0;
		if (left.result == right.result) {
			index = 1;
		} else if (left.result > right.result) {
			index = 2;
		}
		Math_Slot* slot = mpool->getSlot();
		slot->setEndble(i, slot_bg);
        if (display == "text") {
            slot->setSlotSize(cocos2d::Size(max_width, object_size.height) * 0.95);
        } else {
            slot->setSlotSize(object_size * 0.95);
        }
        slot->setComparisionValue(_dragers[index]->getComparisionValue());
        _slots.push_back(slot);
    }
}

void Minh_AdditionThenComparison::onStart()
{
	onShowBottomBar(0, 0.25,true,CC_CALLBACK_1(Minh_AdditionThenComparison::onAnswerDone,this));
    quizBegan(0.5, true);
    
    auto drag_size = object_size;
    if (display == "text") {
        drag_size = _dragers.front()->getContentSize();
    }
    
    //Slot
    float delta_sl = (quiz_size.height * 0.75 - getQuestion()->getContentSize().height-20 - number_operator * drag_size.height) / (number_operator+1);
    if (delta_sl > 100) {
        delta_sl = 100;
    }
    math::func::smartSetPosition(_slots, drag_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height * 0.75 - getQuestion()->getContentSize().height-20), delta_sl, false);

    //Drager
    float delta_dr = (quiz_size.width - 330 - 3 * drag_size.width) / 4;
    if (delta_dr > 100) {
        delta_dr = 100;
    }
    math::func::smartSetPosition(_dragers, drag_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height * 0.25 - 30), delta_dr);

    //Sentence
	for (int i = 0; i < number_operator; ++i) {
		//Left Object
		auto left = _operators_object.at(2 * i);
		left->setPosition(cocos2d::Vec2(_slots[i]->getPositionX() - drag_size.width/2 - left->getLabelValue()->getContentSize().width / 2 - drag_size.height/2, _slots[i]->getPositionY()));
		//Right Object
		auto right = _operators_object.at(2 * i + 1);
		right->setPosition(cocos2d::Vec2(_slots[i]->getPositionX() + drag_size.width/2 + right->getLabelValue()->getContentSize().width / 2 + drag_size.height/2, _slots[i]->getPositionY()));
	}
    
	// Show
    for (int i=0; i<number_operator; i++) {
        addChildNode(_slots[i]);
        addChildNode(_objects[i*2]);
        addChildNode(_objects[i*2+1]);
        
        _slots[i]->onShow(0.1*i, math::animation::SlideDown);
        _objects[i*2]->onShow(0.1*i, math::animation::SlideDown);
        _objects[i*2+1]->onShow(0.1*i, math::animation::SlideDown);
    }
    
    for (int i=0; i<_dragers.size(); i++) {
        addChildNodeToBottomPanel(_dragers[i], 4);
        _dragers[i]->onShow(.5 + i*0.1, math::animation::SlideUp);
    }
}

void Minh_AdditionThenComparison::startQuestion()
{
	enableControlQuiz();
    for (int i=0; i<_dragers.size(); i++) {
        _dragers[i]->enableDrag(_panel_bot);
    }
}

void Minh_AdditionThenComparison::onExplanation(bool correct)
{
	if (!correct) {
        disableControlQuiz(0.25);
        for (int i = 0; i < _slots.size(); ++i) {
			if (_slots[i]->isUsed() && !_slots[i]->isCorrect(key_compare)) {
                _slots[i]->getLinker()->onHide(0, math::animation::ScaleUpAndRotation);
				_slots[i]->removeLinker();
			}
		}
	}
	else {
        disableControlQuiz();
		this->nextQuestion(0.75);
	}
}

void Minh_AdditionThenComparison::onComplete()
{
	//TODO: Thuc hien them cac lenh o day
    for (int i=0; i<_dragers.size(); i++) {
        _dragers[i]->onHide(0.1*i, math::animation::SlideUp);
    }
    
    for (int i=0; i<number_operator; i++) {
        _slots[i]->onHide(0.1*i + 0.25, math::animation::SlideDown);
        _objects[i*2]->onHide(0.1*i + 0.25, math::animation::SlideDown);
        _objects[i*2+1]->onHide(0.1*i + 0.25, math::animation::SlideDown);
    }
    
    quizEnded(0.5, true);
}


#pragma mark - object touch events callback
void Minh_AdditionThenComparison::onTouchedObject(Node * node)
{
	Math_Object* object = (Math_Object*)node;
    
    bool clone = true;
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            clone = false;
            break;
        }
    }
    
    if (clone) {
        CloneDrager(object);
    }
}

void Minh_AdditionThenComparison::onMoveObjectEnded(cocos2d::Node * node, cocos2d::Vec2 position)
{
    disableControlQuiz(0.25);

	Math_Slot * current_slot = NULL;
	Math_Slot * olded_slot = NULL;
	Math_Object* object = (Math_Object*)node;
    
    position = _panel->convertToNodeSpace(position);
	
	//Doi tuong keo tha nam trong 1 slot nao do.
	for (Math_Slot* slot : _slots) {
		if (object->isSame(slot->getLinker())) {
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
	
	// Neu khong nam trong slot nao
	if (current_slot == NULL) {
        object->onHide(0, math::animation::ScaleUpAndRotation);
	}
	// Neu nam trong 1 slot nao do
	else {
		// Neu truoc do k nam trong slot nao
		if (olded_slot == NULL) {
			// neu slot hien tai dang duoc dung -> xoa object cu, chen object moi vao
			if (current_slot->isUsed()) {
                auto linker = current_slot->getLinker();
                linker->onHide(0, math::animation::ScaleUpAndRotation);
				current_slot->removeLinker();
			}
            
            object->setPosition(position);
            object->removeFromParentAndCleanup(false);
            addChildNode(object, 10);
			current_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
		}
		// Neu truoc do nam o slot nao do
		// Doi cho 2 slot
		else {
			if (current_slot == olded_slot) {
                object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
			}
			else {
				Math_Object * ocs = current_slot->getLinker();
				Math_Object * oos = olded_slot->getLinker();
				current_slot->removeLinker();
				olded_slot->removeLinker();

                object->setPosition(position);
                object->removeFromParentAndCleanup(false);
                addChildNode(object, 10);
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

void Minh_AdditionThenComparison::onAnswerDone(Node * node)
{
	Math_Answer* answer = (Math_Answer*)node;
	if (clickable) {
		clickable = false;

		bool  done = true;
		for (Math_Slot* slot : _slots) {
			if (!slot->isCorrect(key_compare)) {
				done = false;
			}
		}
		this->onExplanation(done);
		answer->onAnimationClick(done);
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=]() {
			clickable = true;
		}), NULL));

	}
}

void Minh_AdditionThenComparison::showObjectWithText(Math_Object * object)
{
	object->setValueTextPosition("middle");
	object->showValueTextWithAnimation(false);
	object->setImageVisible(false);
}

minh_opearator Minh_AdditionThenComparison::genOperator()
{
	struct minh_opearator _operator;
	// Load operations
	int plus = 0;
	int minus = 0;
	int number_operation = cocos2d::random(number_operation_min, number_operation_max);
	for (int i = 0; i < number_operation; ++i) {
		int index = cocos2d::random(0, (int)operation.size() - 1);
		_operator._ops.push_back(operation.at(index));
		if (operation.at(index).compare("+") == 0) plus++;
		else if (operation.at(index).compare("-") == 0) minus++;
	}
	// Gen result
	int result = 0;
	int r;
	do {
		r = cocos2d::random(number_min, number_max);
	} while (r < minus * number_min);

	_operator._nums.push_back(r);
	result += r;
	for (int i = 0; i < number_operation; ++i) {
		if (_operator._ops[i].compare("+") == 0) {
			r = cocos2d::random(number_min, number_max);
			result += r;
			_operator._nums.push_back(r);
		}
		else if (_operator._ops[i].compare("-") == 0) {
			r = cocos2d::random(std::min(number_min, result), std::min(number_max, result));
			result -= r;		
			_operator._nums.push_back(r);
		}
	}
	_operator.result = result;
	return _operator;
}

Math_Object* Minh_AdditionThenComparison::genOperatorObject(minh_opearator ope)
{
	auto text_size = 0.7 * object_size.height;
	// Objects
	std::string op_text;

	for(int  i = 0; i < (int)ope._nums.size(); ++i) {
		op_text.append(cocos2d::StringUtils::toString(ope._nums[i]));
		if (i == ope._ops.size()) break;
		if (ope._ops.at(i).compare("+") == 0) {
			op_text.append(" + ");
		}
		else if (ope._ops.at(i).compare("-") == 0) {
			op_text.append(" - ");
		}
	}
	math::resource::Image * num_img = getNumbers(ope.result, ope.result)[0];
	Math_Object * object = mpool->getObject();
	object->setEnable(obj_id++, math::common::deny, num_img, NULL);
	object->setObjectFullSize(object_size);
	object->getLabelValue()->setString(op_text);
	object->getLabelValue()->setFontSize(text_size);
	showObjectWithText(object);
	_objects.push_back(object);
	return object;
}

void Minh_AdditionThenComparison::CloneDrager(Math_Object * object)
{
    for (int i=0; i<_dragers.size(); i++) {
        if (object->isSame(_dragers[i])) {
            _dragers.erase(_dragers.begin() + i);
            break;
        }
    }
	Math_Object *  drag_object = object->clone();
	drag_object->setPosition(object->getStartPosition());
	drag_object->removeFromParentAndCleanup(false);
	addChildNodeToBottomPanel(drag_object, 10);
	_dragers.push_back(drag_object);
    drag_object->enableDrag(_panel_bot);
    drag_object->onShow(0, math::animation::No_Effect);
}

