//
//  Nam_WayToMakeANumber
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#include "Nam_WayToMakeANumber.h"
Nam_WayToMakeANumber* Nam_WayToMakeANumber::createQuiz()
{
    Nam_WayToMakeANumber* quiz=new Nam_WayToMakeANumber();
    if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_WayToMakeANumber::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_WayToMakeANumber::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
	int min = json_game.FindMember("result")->value.FindMember("min")->value.GetInt();
	int max = json_game.FindMember("result")->value.FindMember("max")->value.GetInt();
    game_mode = json_game.FindMember("game_mode")->value.GetString();
    if (game_mode == "add_or_sub"){
        if (cocos2d::RandomHelper::random_int(0,1)){
            game_mode = "sub_result";
        }else{
            game_mode = "add";
        }
    }
    resultOfSentence = RandomHelper::random_int(min, max);
    //resultOfSentence = RandomHelper::random_int(1,3);
    if (game_mode == "sub_result") resultOfSentence = RandomHelper::random_int(min, max - 2);
	image_result = this->getNumbers(resultOfSentence, resultOfSentence)[0];
	image_result->used = true;
    image_slot = math::resource::ResHelper::createImage(this->getUIPath("common/frame/brown1.png"));
    image_slot->used = true;
    bg_sentence = math::resource::ResHelper::createImage(this->getUIPath("common/frame/square3.png"));
	bg_sentence->used = true;
    bg_drager = math::resource::ResHelper::createImage(this->getUIPath("common/answer.png"));
    bg_drager->used = true;
    size_sentence = Size(300,90);
    
    
    //int col =
	// GET QUESTION
	this->parseQuestion();
	auto opt_keys = getQuestion()->getOptionalKeys();
	for (std::string key : opt_keys) {
		if (key == "number") {
			std::vector<math::resource::Text> texts;
			math::resource::Text t = image_result->getTextValue("name_1", m_lang->getLanguage());
			texts.push_back(t);
			getQuestion()->setOptionalTexts(key, texts);
		}
	}
	//TODO: GEN
	// random slot
    int soPhepTinhTrongDay =((resultOfSentence < 8) ? resultOfSentence : 7);
    if (game_mode == "sub_result"){
        soPhepTinhTrongDay = (max - resultOfSentence) ;
        if (soPhepTinhTrongDay > 7) soPhepTinhTrongDay = 7  ;
    }
	if (soPhepTinhTrongDay < 4) {
		numberOfSlot = 1;
	}
	else 
		if (soPhepTinhTrongDay < 6) {
            numberOfSlot = 2;
		}
		else
		{
            numberOfSlot = 3;
		}
	for (int i = 0; i < numberOfSlot; i++) {
		while (1) {
			bool kt = true;
			int id = RandomHelper::random_int(0, soPhepTinhTrongDay);
			for (int j = 0; j < slotId.size(); j++) {
				if (slotId[j] == id) {
					kt = false;
					break;
				}
			}
			if (kt) {
				slotId.push_back(id);
				break;
			}
		}
	}
    
	//gen sentence 
	int randomSX = RandomHelper::random_int(0, 1);
    if (game_mode == "sub_divisor") randomSX = 1;
    Vec2 originSentence = (soPhepTinhTrongDay >= 4  ) ? Vec2(quiz_size.width/2 - size_sentence.width/2,370) : Vec2(quiz_size.width/2,370);
    int col = (game_mode != "sub_result") ? ((resultOfSentence > 3)?( resultOfSentence + 2) / 2 : 4) : (soPhepTinhTrongDay < 4 ? (soPhepTinhTrongDay+1) : (soPhepTinhTrongDay+2)/2);
    if (resultOfSentence > 7 && game_mode!= "sub_result") col = 4;
    
    for (int i = 0; i <= soPhepTinhTrongDay; i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
        if (game_mode != "sub_result"){
            if (randomSX) {
                this->genSentence(i, obj, i, resultOfSentence - i, resultOfSentence);
                aUsed.push_back(i);
                bUsed.push_back(resultOfSentence - i);
            }
            else {
                this->genSentence(i, obj,resultOfSentence - i, i,resultOfSentence);
                bUsed.push_back(i);
                aUsed.push_back(resultOfSentence - i);
            }
        }else{
            // gamemode phep tru giu nguyen ket qua
            this->genSentence(i, obj, i+resultOfSentence, i, resultOfSentence);
            aUsed.push_back(i+resultOfSentence);
            bUsed.push_back(i);
        }
		_objects.push_back(obj);
        if (i < col) {
            obj->setPosition(Vec2(originSentence.x,originSentence.y - i*size_sentence.height));
        }
        else{
            obj->setPosition(Vec2(originSentence.x + size_sentence.width,originSentence.y - (i - col)*size_sentence.height));
        }
        
	}
    if (soPhepTinhTrongDay == 4 || soPhepTinhTrongDay == 6){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        this->genSentence(0, obj, 0, 0, 0);
        obj->getLabelValue()->setVisible(false);
        obj->setPositionX(_objects.back()->getPositionX());
        obj->setPositionY(_objects.back()->getPositionY() - size_sentence.height);
        _objects.push_back(obj);
        
    }
    frame_box = ui::ImageView::create(this->getUIPath("common/frame/square.png"));
    frame_box->setScale9Enabled(true);
    frame_box->setCapInsets(Rect(10, 10, frame_box->getVirtualRendererSize().width - 20, frame_box->getVirtualRendererSize().height - 20));
    if (_objects.size() <= 4) {
        frame_box->setContentSize(Size(size_sentence.width + 20, (size_sentence.height) * _objects.size() + 15));
    }
    else {
         frame_box->setContentSize(Size(size_sentence.width*2 + 20, (size_sentence.height+5) * (_objects.size())/2));
    }
    frame_box->setVisible(true);
    frame_box->setAnchorPoint(Point(0,1));
    frame_box->setPosition(Vec2(originSentence.x - size_sentence.width/2 - 10,originSentence.y+size_sentence.height/2 + 10));
    this->addChildNode(frame_box, 2);
    /*frame_box->setScale(0);
    frame_box->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1))));*/
	// gen slot
	for (int i : slotId) {
		Math_Slot* slot = Math_Pool::getInstance()->getSlot();
		slot->setEndble(i, image_slot);
		slot->setSlotSize(size_sentence);
		slot->setPosition(_objects[i]->getPosition());
        _objects[i]->setName("drager");
		_slots.push_back(slot);
		dragers.push_back(_objects[i]);
	}
    for (Math_Object* obj: _objects){
        if (obj->getName() != "drager"){
            non_dragers.push_back(obj);
        }
    }
	// random dap an sai
	int numberOfAnswers = 4;
	if (numberOfSlot > 3) numberOfAnswers =  6;
	for (int i = 0; i < numberOfAnswers - slotId.size(); i++) {
		Math_Object* obj = Math_Pool::getInstance()->getObject();
		while (1) {
			int a = RandomHelper::random_int(0, resultOfSentence);
            int b = RandomHelper::random_int(0, resultOfSentence);
			bool kt = true;
            std::string tmp;
            if (game_mode == "add"){
                tmp =cocos2d::StringUtils::toString(a) + " + " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(resultOfSentence);
            }
            else{
                if (game_mode == "sub_divisor"){
                    tmp = cocos2d::StringUtils::toString(resultOfSentence) + " - " + cocos2d::StringUtils::toString(a) + " = " + cocos2d::StringUtils::toString(b);
                }else{
                    tmp = cocos2d::StringUtils::toString(std::max(a,b)) + " - " + cocos2d::StringUtils::toString(std::min(a,b)) + " = " + cocos2d::StringUtils::toString(resultOfSentence);
                }
            }
			for (Math_Object* drg : dragers) {
				if (drg->getValueText() == tmp) {
					kt = false;
					break;
				}
			}
			if (kt) {
                if (game_mode == "add"){
                    this->genSentence(-1, obj, a, b, resultOfSentence);
                }
                else{
                    if (game_mode == "sub_divisor"){
                        this->genSentence(-1, obj, resultOfSentence, a, b);
                    }else{
                        this->genSentence(-1, obj, std::max(a,b), std::min(a,b), resultOfSentence);
                    }
                }
                obj->setPosition(Vec2(0,0));
				break;
			}
		}
		dragers.push_back(obj);
	}
    for (Math_Object* obj : dragers){
        obj->getLabelValue()->setTextColor(Color4B(Color3B(99,51,15)));
        obj->setImageVisible(false);
        obj->setBackground(bg_drager,0.2);
    }
    math::func::setPositionList(dragers, (int)dragers.size()/2, Rect(0.05, 0.01, 0.65, 0.24));
	
}
void Nam_WayToMakeANumber::genSentence(int id, Math_Object* object, int a, int b, int result) {
	object->setEnable(id, math::common::TouchPermission::move, bg_sentence, NULL);
	object->setMoveCallback(CC_CALLBACK_1(Nam_WayToMakeANumber::onTouchedObject, this), CC_CALLBACK_2(Nam_WayToMakeANumber::onMoveObjectEndedSlot, this));
	object->setLocked(true);
	object->setObjectSize(size_sentence);
    if (game_mode == "add"){
        object->setValueText(cocos2d::StringUtils::toString(a) + " + " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(result));
    }else{
        if (game_mode =="sub_divisor")
            object->setValueText(cocos2d::StringUtils::toString(result) + " - " + cocos2d::StringUtils::toString(a) + " = " + cocos2d::StringUtils::toString(b));
        else{
             object->setValueText(cocos2d::StringUtils::toString(a) + " - " + cocos2d::StringUtils::toString(b) + " = " + cocos2d::StringUtils::toString(result));
        }
    }
	object->getLabelValue()->setVisible(true);
	object->getLabelValue()->setFontSize(50);
    object->getLabelValue()->setTextColor(Color4B::WHITE);
    object->getLabelValue()->disableEffect();
}
void Nam_WayToMakeANumber::onStart() {
	math_statistic->setCurrentGame(_game);
	for (Math_Object* obj : non_dragers) {
		this->addChildNode(obj, 2);
		obj->onShow(0);
	}
    
	for (Math_Object* obj : dragers) {
        this->addChildNodeToBottomPanel(obj,2);
        obj->onShow(0);
        obj->enableDrag(_panel_bot);
	}
	for (Math_Slot* slot : _slots) {
		this->addChildNode(slot,2);
		slot->onShow(0);
	}
    onShowBottomBar(0,0.3,true, CC_CALLBACK_1(Nam_WayToMakeANumber::onAnswerDone, this));
    this->getQuestion()->adjustQuestionSize();
    float x = this->getQuestion()->getContentSize().height;
    if (x > 100){
        showQuestionBox();
    }
    else{
        showQuestionBox();
    }
}

void Nam_WayToMakeANumber::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
        for (Math_Object* obj : dragers) {
            obj->enableDrag(_panel_bot);
        }
        
	}
}
void Nam_WayToMakeANumber::onExplanation(bool correct) {}
void Nam_WayToMakeANumber::onComplete() {
    //TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
    float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
    frame_box->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay),
                                                   /*cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)),*/
                                                   cocos2d::CallFunc::create([=]() {
        frame_box->setVisible(false);
    }), NULL));
    for (Math_Object* obj : non_dragers){
        obj->onHide(m_delay,math::animation::SlideUp);
    }
    for (Math_Object* obj : dragers){
        obj->onHide(m_delay,math::animation::SlideUp);
    }
	//m_delay += 0.2;
	for (Math_Slot* slot : _slots) {
		slot->onHide(m_delay,math::animation::SlideUp);
	}
	m_delay += 0.1;
	
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

#pragma mark - object touch events callback
void Nam_WayToMakeANumber::onTouchedObject(Node* node) {
	Math_Object* object = (Math_Object*)node;
	if (selected_object == NULL) {
		selected_object = object;
	}
	else {
		object->setAllowMove(false);
	}
}
void Nam_WayToMakeANumber::onMoveObjectEndedSlot(Node* node, Vec2 position) {
	//CODE ANH PHUONG
    Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
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
void Nam_WayToMakeANumber::onAnswerDone(Node* node) {
	Math_Answer* btn = (Math_Answer*)node;
	bool done = true;
    //this->nextQuestion();
	for (Math_Slot* slot : _slots) {
		if (slot->isUsed()) {
			if (slot->getLinker()->getID() != slot->getId()) {
				Math_Object* obj = slot->getLinker();
				obj->backToStart();
				slot->removeLinker();
				done = false;
			}
		}
		else {
			done = false;
		}
	}
    btn->onAnimationClick(done);
	if (done) this->nextQuestion();
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_WayToMakeANumber::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("test-%d", testing_step++));
}

void Nam_WayToMakeANumber::autoTestingRun(float delaytime)
{
   
}

