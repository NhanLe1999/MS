//
//  Nam_Commutative.h
//  MJQuiz
//
//  Created by Tunado on 30/9/17.
//
//


#include "Nam_Commutative.h"

Nam_Commutative* Nam_Commutative::createQuiz(){
	Nam_Commutative* quiz = new Nam_Commutative();

	if (quiz->_initQuiz()){
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}
bool Nam_Commutative::init(){
	return cocos2d::ui::Layout::init();	
}
void Nam_Commutative::initQuiz(mj::model::Game game){
	
	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_result = json_game.FindMember("result")->value;
    int max =json_result.FindMember("max")->value.GetInt();
    //result = RandomHelper::random_int(json_result.FindMember("min")->value.GetInt(), json_result.FindMember("max")->value.GetInt());
	while (1) {
		number1 = RandomHelper::random_int(1, max/2);
		number2 = RandomHelper::random_int(1, max/2);
        result = number1 + number2;
        if (number1 != number2) break;
	}
	//TODO lấy dữ liệu object
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	objectSize = Size(json_object.FindMember("size")->value.FindMember("width")->value.GetDouble(),
		json_object.FindMember("size")->value.FindMember("height")->value.GetDouble());
	//TODO lấy dữ liệu drager
	rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
	dragerSize = Size(json_drager.FindMember("size")->value.FindMember("width")->value.GetDouble(),
		json_drager.FindMember("size")->value.FindMember("height")->value.GetDouble());
	//TODO: lấy ảnh trong cac media
	imageObject = getImageList("icons", 1, "name_1")[0];
	imageObject->used = true;
	imgSlot = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/drag.png");
	imgSlot->used = true;
	img_box_slot = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/drop.png");
	img_box_slot->used = true;
    auto line = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/numberline/line.png");
    line->used = true;
	this->parseQuestion();
    auto img_equal = getOperator("=");
    auto img_add = getOperator("+");
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	// FIX GAME CHUNG
    objectSize = Size(75,75);
	//Gen Slot
    for (int i = 0; i < 3; i++){
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i, img_box_slot);
        slot->setSlotSize(dragerSize);
        _slots.push_back(slot);
        this->addChildNode(slot,2);
        slot->onShow(0);
    }
    _slots[0]->setName(cocos2d::StringUtils::toString(number1));
    _slots[1]->setName(cocos2d::StringUtils::toString(result));
    _slots[2]->setName(cocos2d::StringUtils::toString(number2));
    // gen equal
    std::vector<Math_Object*>equals;
    for (int i = 0; i < 2; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(i, math::common::TouchPermission::deny, img_equal, NULL);
        obj->setObjectSize(dragerSize*0.7);
        equals.push_back(obj);
        this->addChildNode(obj,2);
        obj->onShow(0);
    }
    // gen add
    std::vector<Math_Object*>adds;
    for (int i = 0; i < 2; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(i, math::common::TouchPermission::deny, img_add, NULL);
        obj->setObjectSize(dragerSize*0.7);
        adds.push_back(obj);
        this->addChildNode(obj,2);
        obj->onShow(0);
    }
    // gen number
    int a[3] = {number2,number1,result};
    std::vector<Math_Object*>numbers;
    for (int i = 0; i < 3; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(i, math::common::TouchPermission::deny, this->getNumbers(a[i], a[i]).front(), NULL);
        this->genOperator(obj, cocos2d::StringUtils::toString(a[i]));
        numbers.push_back(obj);
        this->addChildNode(obj,2);
        obj->onShow(0);
    }
    calculation1.push_back(_slots[0]);
    calculation1.push_back(adds[0]);
    calculation1.push_back(numbers[0]);
    calculation1.push_back(equals[0]);
    calculation1.push_back(_slots[1]);
    math::func::setPositionList(calculation1, calculation1.size(), Rect(0.1,0,0.9,0.2));
    calculation1.back()->setPositionX(calculation1.back()->getPositionX()-50);
    calculation2.push_back(_slots[2]);
    calculation2.push_back(adds[1]);
    calculation2.push_back(numbers[1]);
    calculation2.push_back(equals[1]);
    calculation2.push_back(numbers[2]);
    math::func::setPositionList(calculation2, calculation2.size(), Rect(0.1,0.3,0.9,0.2));
    calculation2.back()->setPositionX(calculation2.back()->getPositionX()-50);
    float delay = 0;
    //GEN OBJECT
    for (int count = 0; count <= 1; count++) {
        for (int i = 0; i < number1; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::TouchPermission::deny, imageObject, NULL);
            obj->setObjectSize(objectSize);
            obj->setLocked(true);
            if (count == 0) caculationObjects1[0].push_back(obj);
            else caculationObjects2[1].push_back(obj);
            _objects.push_back(obj);
            this->addChildNode(obj,2);
            obj->onShow(0);
        }
    }
    for (int count = 0; count <= 1; count++) {
        for (int i = 0; i < number2; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::TouchPermission::deny, imageObject, NULL);
            obj->setObjectSize(objectSize);
            obj->setLocked(true);
            if (count == 0) caculationObjects1[1].push_back(obj);
            else caculationObjects2[0].push_back(obj);
            _objects.push_back(obj);
            this->addChildNode(obj,2);
            obj->onShow(0);
        }
    }
    math::func::setPositionListOnFrame(caculationObjects1[0], (int)caculationObjects1[0].size(), 1, objectSize, calculation1[0]->getPosition(), 0);
    math::func::setPositionListOnFrame(caculationObjects1[1], (int)caculationObjects1[1].size(), 1, objectSize, calculation1[2]->getPosition(), 0);
    math::func::setPositionListOnFrame(caculationObjects2[0], (int)caculationObjects2[0].size(), 1, objectSize, calculation2[0]->getPosition(), 0);
    math::func::setPositionListOnFrame(caculationObjects2[1], (int)caculationObjects2[1].size(), 1, objectSize, calculation2[2]->getPosition(), 0);
    for (auto obj : _objects){
        obj->setPositionY(obj->getPositionY() + dragerSize.height/2 + objectSize.height/2 + 10);
    }
    Math_Object* obj = Math_Pool::getInstance()->getObject();
    obj->setEnable(0, math::common::TouchPermission::deny, line, NULL);
    obj->setObjectSize(Size(quiz_size.width*0.7,10));
    obj->setAnchorPoint(Vec2(0,0.5));
    obj->setPosition(Vec2(0.1*quiz_size.width,calculation2[0]->getPositionY() - dragerSize.height/2 - 20));
    this->addChildNode(obj,2);
    obj->onShow(0);
	//TODO: Show object theo yeu cau
//    for (int i = 0; i < 2; i++) {
//        for (Math_Object* obj : caculationObjects1[i]) {
//            this->addChildNode(obj, 2);
//            obj->onShow(delay);
//        }
//        this->addChildNode(operatorObjs[i + 2], 2);
//        operatorObjs[i + 2]->onShow(delay + 0.2);
//    }
//    delay += 0.2;
//    for (int i = 3; i < 6; i++) {
//        this->addChildNode(_slots[i], 2);
//        _slots[i]->onShow(delay);
//
//    }
//    delay += 0.2;
//    for (int i = 0; i < 2; i++) {
//        for (Math_Object* obj : caculationObjects2[i]) {
//            this->addChildNode(obj, 2);
//            obj->onShow(delay);
//        }
//        this->addChildNode(operatorObjs[i], 2);
//        operatorObjs[i]->onShow(delay + 0.2);
//    }
//    delay += 0.2;
//    for (int i = 0; i < 3; i++) {
//        this->addChildNode(_slots[i], 2);
//        _slots[i]->onShow(delay);
//
//    }
//    delay += 0.2;
	//Gen drager
	int wrongAnswer;
	while (1) {
		wrongAnswer = RandomHelper::random_int(0, 10);
		if (wrongAnswer != number1 && wrongAnswer != number2 && wrongAnswer != result) break;
	}
	intResult.push_back(wrongAnswer);
	intResult.push_back(number1);
	intResult.push_back(number2);
	intResult.push_back(result);
	std::random_shuffle(intResult.begin(), intResult.end());
	for (int i = 0; i < 4; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(i, imgSlot);
		slot->setSlotSize(dragerSize);
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		this->genOperator(obj, cocos2d::StringUtils::toString(intResult[i]));
        obj->setImage(imgSlot);
        obj->setImageVisible(true);
		slot->setLinker(obj);
		dragers.push_back(obj);
		resultSlots.push_back(slot);
		//_slots.push_back(slot);
	}
	math::func::setPositionList(dragers, 4, cocos2d::Rect(0.1, 0.01, 0.6, 0.2));
	math::func::setPositionList(resultSlots, 4, cocos2d::Rect(0.1, 0.01, 0.6, 0.2));
    for (Math_Slot* slot : resultSlots){
        this->addChildNodeToBottomPanel(slot,2);
        slot->onShow(delay += 0.03);
    }
	for (Math_Object* obj : dragers) {
		obj->setMoveCallback(CC_CALLBACK_1(Nam_Commutative::onTouchedObject, this), CC_CALLBACK_2(Nam_Commutative::onMoveObjectEnded, this));
		this->addChildNodeToBottomPanel(obj,2);
		obj->enableDrag(_panel_bot);
		obj->onShow(delay);
	}
    

}
void Nam_Commutative::onStart() {
	
	//Gen button done
    onShowBottomBar(0, 0.25,true, CC_CALLBACK_1(Nam_Commutative::onAnswerDone, this));
	showQuestionBox();
}
void Nam_Commutative::startQuestion() {
	enableControlQuiz();
}

void Nam_Commutative::onExplanation(bool correct) {
}
void Nam_Commutative::onComplete() {
	//TODO: Thuc hien them cac lenh o day
//    for (Math_Answer* node : _answers) {
//        if (node->isEnable()) {
//            node->setDisable();
//        }
//    }
    float delay = showConratulation(0.3);
//    float m_delay = delay - 0.3;
//    for (int i = 0; i < 2; i++) {
//        for (Math_Object* obj : caculationObjects1[i]) {
//            obj->onHide(m_delay);
//        }
//        operatorObjs[i + 2]->onHide(m_delay + 0.1);
//    }
//    m_delay += 0.1;
//    for (int i = 3; i < 6; i++) {
//        _slots[i]->onHide(m_delay);
//
//    }
//    m_delay += 0.1;
//    for (int i = 0; i < 2; i++) {
//        for (Math_Object* obj : caculationObjects2[i]) {
//            obj->onHide(m_delay);
//        }
//        operatorObjs[i]->onHide(m_delay + 0.1);
//    }
//    m_delay += 0.1;
//    for (int i = 0; i < 3; i++) {
//        _slots[i]->onHide(m_delay);
//
//    }
//    m_delay += 0.1;
//    for (int i = 0; i < dragers.size(); i++) {
//        dragers.at(i)->onHide(m_delay);
//        resultSlots.at(i)->onHide(m_delay);
//    }
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
void Nam_Commutative::genOperator(Math_Object* obj, std::string text) {
	// fix tam image
	obj->setEnable(0, math::common::TouchPermission::deny, img_box_slot, NULL);
	obj->setObjectSize(dragerSize);
	obj->setImageVisible(false);
	obj->setValueText(text);
	obj->getLabelValue()->setVisible(true);
	obj->getLabelValue()->setFontSize(80.f);
}
void Nam_Commutative::onAnswerDone(Node* node) {
    Math_Answer* btn = (Math_Answer*) node;
	bool done = true;
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed()) {
			if (slot->getLinker()->getValueText() != slot->getName()) {
				done = false;
				slot->getLinker()->setDisable();
				slot->removeLinker();
			}
		}
		else {
			done = false;
		}
	}
    btn->onAnimationClick(done);
    if (done) this->nextQuestion();
}
void Nam_Commutative::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
}
void Nam_Commutative::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
	Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
	//Doi tuong keo tha nam trong 1 slot nao do.
	//Remove object khoi slot hien tai.
	bool checkCreate = false;
	int indexCreate;
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed() && slot->getLinker()->isSame(object)) {
			slot->removeLinker();
			break;
		}
	}
	for (int i = 0; i < resultSlots.size(); i++) {
		if (resultSlots[i]->isUsed() && resultSlots[i]->getLinker()->isSame(object)) {
			checkCreate = true;
			resultSlots[i]->removeLinker();
			indexCreate = i;
			break;
		}
	}
	//Add object lai _panel
	
		
	//Check slot dich.e
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position)) {
			if (slot->getLinker() != NULL) {
				Math_Object* old_object = slot->getLinker();
				slot->removeLinker();
				old_object->runAction(cocos2d::Sequence::create(
					cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.3, 0)),
					cocos2d::CallFunc::create([=]() {
					old_object->removeFromParent();
				}), NULL));
			}
			slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, slot->getPosition()));
			object->setZOrder(2);
			if (checkCreate) {
				Math_Object* obj = Math_Pool::getInstance()->getObject();
				this->genOperator(obj, cocos2d::StringUtils::toString(intResult[indexCreate]));
                obj->setImage(imgSlot);
                obj->setImageVisible(true);
				this->addChildNodeToBottomPanel(obj, 2);
				obj->onShow(0);
				resultSlots[indexCreate]->setLinker(obj);
				obj->setPosition(resultSlots[indexCreate]->getPosition());
				obj->enableDrag(_panel_bot);
				obj->setMoveCallback(CC_CALLBACK_1(Nam_Commutative::onTouchedObject, this), CC_CALLBACK_2(Nam_Commutative::onMoveObjectEnded, this));
			}
			return;
		}
	}
	/*for (Math_Slot* slot : resultSlots) {
		if (!slot->isUsed() && slot->isAvailable(position)) {
			slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, slot->getPosition()));
			object->setZOrder(2);
			return;
		}
	}*/
	//Object k nam trong slot nao
	//Move ve vi tri ban dau
	this->removeChild(object);
	object->setDisable();
	if (checkCreate) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		this->genOperator(obj, cocos2d::StringUtils::toString(intResult[indexCreate]));
		this->addChildNodeToBottomPanel(obj, 2);
        obj->setImage(imgSlot);
        obj->setImageVisible(true);
		obj->onShow(0);
		resultSlots[indexCreate]->setLinker(obj);
		obj->setPosition(resultSlots[indexCreate]->getPosition());
		obj->enableDrag(_panel_bot);
		obj->setMoveCallback(CC_CALLBACK_1(Nam_Commutative::onTouchedObject, this), CC_CALLBACK_2(Nam_Commutative::onMoveObjectEnded, this));
	}
	

	
}
#define auto_testing_delay 2
void Nam_Commutative::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}
void Nam_Commutative::autoTestingRun(float delaytime) {
}
