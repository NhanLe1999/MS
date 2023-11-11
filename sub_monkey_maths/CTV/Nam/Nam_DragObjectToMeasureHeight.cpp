//
//  Nam_DragObjectToMeasureHeight
//  MJQuiz
//
//  Created by tunado on 8/30/17.
//
//

#include "Nam_DragObjectToMeasureHeight.h"

Nam_DragObjectToMeasureHeight* Nam_DragObjectToMeasureHeight::createQuiz(){
	Nam_DragObjectToMeasureHeight* quiz = new Nam_DragObjectToMeasureHeight();

	if (quiz->_initQuiz()){
        quiz->autorelease();
        return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return nullptr;
}
bool Nam_DragObjectToMeasureHeight::init(){
	return cocos2d::ui::Layout::init();	
}
void Nam_DragObjectToMeasureHeight::initQuiz(mj::model::Game game){
	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
	rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	rapidjson::Value& json_character = json_game.FindMember("height of character")->value;
	heighOfCharacter = cocos2d::RandomHelper::random_int(json_character.FindMember("min")->value.GetInt(), json_character.FindMember("max")->value.GetInt());
	//TODO: lay du lieu drager
	rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
	dragerSize = Size(json_drager.FindMember("size")->value.FindMember("width")->value.GetDouble(), json_drager.FindMember("size")->value.FindMember("height")->value.GetDouble());
    
    mode_game = json_game.FindMember("mode_game")->value.GetString();
	//TODO: lấy image
    auto allImages = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()),"",image_prefix);
    math::func::shuffle(allImages);
    for (auto img1 : allImages){
        for (auto img2 : allImages){
            if (img1->code != img2->code && img1->getComparisionValue("pair") == img2->getComparisionValue("pair")){
                if (img1->getComparisionValue("role") == 0){
                    imageCharacter = img1;
                    imageBlock = img2;
                }else{
                    imageCharacter = img2;
                    imageBlock = img1;
                }
            }
        }
    }
	imageBlock->used = true;
    imageCharacter->used = true;
    box_slot = math::resource::ResHelper::createImage(getUIPath("gameui/Nam_DragObjectToMeasureHeight/box_slot.png"));
	box_slot->used = true;
    image_answer = objectBG();
    image_answer->used = true;
	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "name_1") {
			std::vector<math::resource::Text> texts;
			auto img_txt = imageCharacter->getTextValue("name_1", m_lang->getLanguage());
			texts.push_back(img_txt);
			getQuestion()->setOptionalTexts(key, texts);
		}
        if (key == "name_n") {
            std::vector<math::resource::Text> texts;
            auto img_txt = imageBlock->getTextValue("name_n", m_lang->getLanguage());
            texts.push_back(img_txt);
            getQuestion()->setOptionalTexts(key, texts);
        }
	}
	// get number result
	for (int i = 1; i <= 3; i++) {
		int x;
		while (true)
		{
			int kt = true;
			x = RandomHelper::random_int(1, 10);
			if (x == heighOfCharacter) kt = false;
			for (int j : numbers_result) {
				if (x == j) {
					kt = false;
				}
			}
			if (kt) break;
		}
		numbers_result.push_back(x);
	}
	numbers_result.push_back(heighOfCharacter);
	std::random_shuffle(numbers_result.begin(), numbers_result.end());
	///// FIX GAME CHUNG
    // gen character
    float deltaY = (mode_game == "height") ? 20 : (100 + dragerSize.height);
    float deltaX; // dung trong height
    character = Math_Pool::getInstance()->getObject();
    character->setEnable(0, math::common::TouchPermission::deny, imageCharacter, nullptr);
    if (mode_game == "height"){
        character->setObjectSize(Size(character->getContentSize().width,heighOfCharacter*dragerSize.height - 5));
        character->setAnchorPoint(Vec2(1, 0));
        character->setPositionX(quiz_size.width/2 - 150);
        character->setPositionY(deltaY);
    }
    else{
        character->setObjectSize(Size(heighOfCharacter*dragerSize.width - 5, character->getContentSize().height));
        character->setAnchorPoint(Point(0.5,0));
        character->setPositionX(quiz_size.width/2);
        character->setPositionY(deltaY);
        deltaX =quiz_size.width/2 - (heighOfCharacter*dragerSize.width - 5)/2;
    }
    this->addChildNode(character, 2);
    character->onShow(0);
	// gen slot to measure height
	indexSlotCurrent = 0;
	for (int i = 0; i < heighOfCharacter; i++) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(i, box_slot);
		slot->getBackground()->setVisible(false);
		slot->setSlotSize(dragerSize);
        if (mode_game == "height"){
            slot->setAnchorPoint(Point(0.5,0));
            slot->setPosition(Vec2(character->getPositionX() + dragerSize.width/2 + 100, deltaY + i*(dragerSize.height - 2)));
        }
        else{
            slot->setAnchorPoint(Point(0,0.5));
            slot->setPosition(Vec2(deltaX + i*(dragerSize.width-2), deltaY - dragerSize.height));
        }
		slotHeightOfCharacter.push_back(slot);
		_slots.push_back(slot);
		this->addChildNode(slot, 2);
		slot->onShow(0);
        this->fixAnchorPoint(slot);
	}
	_slots[0]->getBackground()->setVisible(true);
    math::resource::Image* lineImg =math::resource::ResHelper::createImage("gameui/Nam_DragObjectToMeasureHeight/line_red.png");
	lineImg->used = true;
	line1 = Math_Pool::getInstance()->getObject();
	line1->setEnable(0, math::common::TouchPermission::deny, lineImg, nullptr);
	line1->setObjectSize(Size(300, 18));
	line1->setAnchorPoint(Vec2(1, 1));
	line1->setPosition(Vec2(_slots[0]->getPositionX() + dragerSize.height / 2 + 2, _slots[0]->getPositionY() - dragerSize.height / 2 + 10));
	this->addChildNode(line1, 2);
	line1->onShow(0);
	line2 = Math_Pool::getInstance()->getObject();
	line2->setEnable(0, math::common::TouchPermission::deny, lineImg, nullptr);
	line2->setObjectSize(Size(300, 18));
	line2->setAnchorPoint(Vec2(1, 0));
	line2->setPosition(Vec2(_slots[heighOfCharacter - 1]->getPositionX() + dragerSize.height / 2 + 2, _slots[heighOfCharacter - 1]->getPositionY() + dragerSize.height / 2 - 10));
	this->addChildNode(line2, 2);
	line2->onShow(0);
    
	// gen dragers
	Math_Object* drager = Math_Pool::getInstance()->getObject();
	this->genDrager(drager);
	_dragers.push_back(drager);
	//Math_Pool::getInstance()->resetImages();
}
void Nam_DragObjectToMeasureHeight::onStart() {
	// gen answers
	for (int i = 1; i <= 4; i++) {
		Math_Answer* btn;
		btn = Math_Pool::getInstance()->getAnswer();
		btn->setEnable(cocos2d::StringUtils::toString(numbers_result[i - 1]), math::common::answer, CC_CALLBACK_1(Nam_DragObjectToMeasureHeight::onAnswerDone, this, numbers_result[i - 1]), this->getAnswerSize());
		btn->setCustomAnswer(image_answer,Size(120,120),80,Color4B::BLACK);
		_answers.push_back(btn);
	}
    if (mode_game == "height"){
         math::func::setPositionList(_answers, 1, cocos2d::Rect(quiz_size.width*0, quiz_size.height*0.1, quiz_size.width*0.25, quiz_size.height*0.8));
    }
    else{
        math::func::setPositionList(_answers, (int)_answers.size(), Rect(0.1, 0, 0.8, 0.22));
    }
    
    (mode_game == "height") ? this->onShowRightBar(0,0.25,nullptr) : this->onShowBottomBar();
	this->showQuestionBox();
}
void Nam_DragObjectToMeasureHeight::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
	} 
	else
	{
        for (Math_Answer* btn : _answers) {
            (mode_game == "height") ? btn->onShowInParent(_panel_right,2) : btn->onShowInParent(_panel_bot,2);
        }
	}
}
void Nam_DragObjectToMeasureHeight::onExplanation(bool correct) {
}
void Nam_DragObjectToMeasureHeight::onComplete() {
	//TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
	character->onHide(m_delay);
	line1->onHide(m_delay);
	line2->onHide(m_delay);
	for (Math_Object* drg : _dragers) {
		drg->onHide(m_delay);
	}
	for (Math_Slot* slot : _slots) {
		slot->onHide(m_delay);
	}
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
void Nam_DragObjectToMeasureHeight::onAnswerDone(Node* node, int index) {
	Math_Answer* btn = (Math_Answer*)node;

	if (index == heighOfCharacter) {
        btn->onAnimationClick(true);
        this->nextQuestion();
        
	}
	else {
		btn->onAnimationClick(false);
	}
}
void Nam_DragObjectToMeasureHeight::onTouchedDrager(Node* node) {
	Math_Object* object = (Math_Object*)node;
	if (selected_object == nullptr) {
		selected_object = object;
		//first_pos = object->getPosition();
	}
	else {
		object->setAllowMove(false);
	}
}
void Nam_DragObjectToMeasureHeight::onMoveDragerEnded(Node* node, Vec2 point) {
	Math_Object* object = (Math_Object*)node;
    point = _panel->convertToNodeSpace(point);
    object->removeFromParentAndCleanup(false);
    object->setPosition(point);
    addChildNode(object, 2);
    if (mode_game == "height"){
        if (point.x < (quiz_size.width + 350) && point.y > _slots[indexSlotCurrent]->getPositionY()-dragerSize.height/2) {
            object->runAction(MoveTo::create(0.5f, _slots[indexSlotCurrent]->getPosition()));
            _slots[indexSlotCurrent]->setLinker(object);
            object->setLocked(true);
            indexSlotCurrent++;
            if (indexSlotCurrent < _slots.size()) _slots[indexSlotCurrent]->getBackground()->setVisible(true);
            if (indexSlotCurrent == heighOfCharacter) {
                if (this->getQuestion()->getNumberQuestions() == 1){
                    this->scheduleOnce([=](float dt) {
                        this->nextQuestion();
                    },1.f, cocos2d::StringUtils::format("complete"));
                }else{
                    this->nextQuestion();
                }
            }
            else{
                Math_Object* obj = Math_Pool::getInstance()->getObject();
                this->genDrager(obj);
                _dragers.push_back(obj);
            }
            selected_object = nullptr;
        }
        else {
            //Object k nam trong slot nao
            //Move ve vi tri ban dau
            object->backToLast(2);
            selected_object = nullptr;
        }
    }else{
        if (point.x > _slots[indexSlotCurrent]->getPositionX() && point.y > 0) {
            object->runAction(MoveTo::create(0.5f, _slots[indexSlotCurrent]->getPosition()));
            _slots[indexSlotCurrent]->setLinker(object);
            object->setLocked(true);
            indexSlotCurrent++;
            if (indexSlotCurrent < _slots.size()) _slots[indexSlotCurrent]->getBackground()->setVisible(true);
            if (indexSlotCurrent == heighOfCharacter) {
                if (this->getQuestion()->getNumberQuestions() == 1){
                    this->scheduleOnce([=](float dt) {
                        this->nextQuestion();
                    },1.f, cocos2d::StringUtils::format("complete"));
                }else{
                     this->nextQuestion();
                }
                //this->nextQuestion();
            }
            else{
                Math_Object* obj = Math_Pool::getInstance()->getObject();
                this->genDrager(obj);
                _dragers.push_back(obj);
            }
            selected_object = nullptr;
        }
        else {
            //Object k nam trong slot nao
            //Move ve vi tri ban dau
            object->backToLast(2);
            selected_object = nullptr;
        }
    }
	
}
//void Nam_DragObjectToMeasureHeight::updateSlotHeightOfCharacter() {
//    int key = 696969;
//    for (int i = 0; i < slotHeightOfCharacter.size(); i++) {
//        if (!slotHeightOfCharacter[i]->isUsed()) {
//            key = i;
//            break;
//        }
//    }
//    for (int i = key + 1; i < slotHeightOfCharacter.size(); i++) {
//        if (slotHeightOfCharacter[i]->isUsed()) {
//            Math_Object* obj = slotHeightOfCharacter[i]->getLinker();
//            slotHeightOfCharacter[i]->removeLinker();
//            obj->runAction(MoveTo::create(0.2f, slotHeightOfCharacter[i - 1]->getPosition()));
//            slotHeightOfCharacter[i - 1]->setLinker(obj);
//        }
//    }
//}
void Nam_DragObjectToMeasureHeight::fixAnchorPoint(Math_Slot* obj) {
    float deltaX = 0.5 - obj->getAnchorPoint().x;
    float deltaY = 0.5 - obj->getAnchorPoint().y;
    obj->setAnchorPoint(Vec2(0.5, 0.5));
    obj->setPosition(Vec2(deltaX*obj->getContentSize().width + obj->getPositionX(), deltaY*obj->getContentSize().height + obj->getPositionY()));
}
void Nam_DragObjectToMeasureHeight::genDrager(Math_Object* obj) {
	obj->setEnable(0, math::common::TouchPermission::move, imageBlock, nullptr);
	obj->setObjectSize(dragerSize);
	//obj->setPosition(Vec2(, 20));
	obj->setMoveCallback(CC_CALLBACK_1(Nam_DragObjectToMeasureHeight::onTouchedDrager, this), CC_CALLBACK_2(Nam_DragObjectToMeasureHeight::onMoveDragerEnded, this));
	obj->setVisible(true);
	obj->getImage()->setVisible(true);
    std::vector<Math_Object*>tmps;
    tmps.clear();
    tmps.push_back(obj);
    if (mode_game == "height"){
        // width
        math::func::setPositionList(tmps, 1, cocos2d::Rect(quiz_size.width*0, quiz_size.height*0.1, quiz_size.width*0.25, quiz_size.height*0.8));
        this->addChildNodeToRightPanel(obj,2);
        obj->onShow(0);
        obj->enableDrag(_panel_right);
        
    }
    else{
        // height
        math::func::setPositionList(tmps, 1, Rect(0.1, 0, 0.8, 0.22));
        this->addChildNodeToBottomPanel(obj,2);
        obj->onShow(0);
        obj->enableDrag(_panel_bot);
    }
}
#define auto_testing_delay 2
void Nam_DragObjectToMeasureHeight::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}
void Nam_DragObjectToMeasureHeight::autoTestingRun(float delaytime) {
	
}

