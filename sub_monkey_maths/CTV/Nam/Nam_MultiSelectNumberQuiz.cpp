//
//  Nam_MultiSelectNumberQuiz
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_MultiSelectNumberQuiz.h"

Nam_MultiSelectNumberQuiz* Nam_MultiSelectNumberQuiz::createQuiz()
{
    Nam_MultiSelectNumberQuiz* quiz=new Nam_MultiSelectNumberQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
     
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_MultiSelectNumberQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_MultiSelectNumberQuiz::initQuiz(mj::model::Game game)
{
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    rapidjson::Value& json_data=document.FindMember("data")->value;
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    
    std::string game_mode = json_game.FindMember("game_mode")->value.GetString();
    if (game_mode == "random"){
        game_mode = RandomHelper::random_int(0, 1)? "tang" : "giam";
    }
    rapidjson::Value& json_number_start= (game_mode == "tang") ?  json_game.FindMember("number_start_tang")->value : json_game.FindMember("number_start_giam")->value;
    int number_start = RandomHelper::random_int(json_number_start.FindMember("minimum")->value.GetInt(), json_number_start.FindMember("maximum")->value.GetInt());
    int number_skip = json_game.FindMember("skip_number")->value.GetInt();
    rapidjson::Value& json_number_jsons=json_game.FindMember("slots_number")->value;
    int number_slots =RandomHelper::random_int(json_number_jsons.FindMember("minimum")->value.GetInt(), json_number_jsons.FindMember("maximum")->value.GetInt());
    rapidjson::Value& json_object_size=json_game.FindMember("object_size")->value;
    object_size = Size(json_object_size.FindMember("width")->value.GetInt(),json_object_size.FindMember("height")->value.GetInt());
    // get Image skip number
    math::resource::Image* image_skip_number = this->getNumbers(number_skip,number_skip).front();
    image_skip_number->used = true;
    math::resource::Image* image_box_object = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png"));
    image_box_object->used = true;
    math::resource::Image* image_box_longpress = math::resource::ResHelper::createImage(getUIPath("gameui/touch_select/longpress.png"));
    image_box_longpress->used = true;
    math::resource::Image* image_box_answer = objectBG();
    image_box_answer->used = true;
    // get question
    this->parseQuestion();
    // gen objects
    int number_boxes;
    if (number_slots == 1){
        number_boxes = 5;
    }else{
        number_boxes = 10;
    }
    std::vector<int> index_slots = randomArrayIndexes(number_slots, number_boxes);
    for (int i = 0; i < number_boxes; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        int num = (game_mode == "tang") ? number_start+i*number_skip : number_start-i*number_skip;
        this->genNumberBox(i,obj,num,image_box_object,Color4B::WHITE);
        _objects.push_back(obj);
    }
    // add vao man hinh
    for (Math_Object* obj :_objects){
        this->addChildNode(obj,2);
        obj->onShow(0);
    }
    math::func::setPositionList(_objects, number_boxes/((number_boxes > 5) ? 2 : 1), Rect(0.1,0.06,0.8,0.71));
    std::vector<int> ints_answer;
    for (int i : index_slots){
        _objects[i]->setImage(image_box_longpress);
        _objects[i]->getLabelValue()->setVisible(false);
        ints_answer.push_back(_objects[i]->getComparisionValue().number);
        _objects[i]->setTouchCallback(CC_CALLBACK_1(Nam_MultiSelectNumberQuiz::onTouchedObject, this));
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i, image_box_longpress);
        slot->setSlotSize(object_size);
        slot->setPosition(_objects[i]->getPosition());
        _slots.push_back(slot);
        this->addChildNode(slot,2);
        slot->onShow(0);
        // tao bang chon box choice
        auto img_view = cocos2d::ui::ImageView::create(m_theme->getThemePath()+"gameui/touch_select/choice.png");
        img_view->setScale9Enabled(true);
        img_view->setContentSize(Size((object_size.width+30)*2,object_size.height+30));
        img_view->setPosition(Vec2(_objects[i]->getPositionX(),_objects[i]->getPositionY()+object_size.height/2 + 80));
        img_view->setTag(i);
        images_box_choice.push_back(img_view);
        img_view->setOpacity(0);
        img_view->setScale(0);
        this->addChildNode(img_view,2);
    }
    // gen answer
    for (int i = 0; i < number_slots; i++){
        // answer dung
        Math_Object* obj1 = Math_Pool::getInstance()->getObject();
        this->genNumberBox(index_slots[i],obj1,ints_answer[i],image_box_answer,Color4B(Color3B(99,51,15)));
        obj1->setPosition(Vec2(images_box_choice[i]->getPositionX()-images_box_choice[i]->getContentSize().width/4,images_box_choice[i]->getPositionY()+6));
        obj1->setTouchCallback(CC_CALLBACK_1(Nam_MultiSelectNumberQuiz::onTouchedAnswer, this));
        this->addChildNode(obj1,2);
        obj1->onShow(0);
        obj1->setVisible(false);
        true_answers.push_back(obj1);
        auto obj_gen_truoc1 = obj1->clone();
        obj_gen_truoc1->setPosition(Vec2(-200,-200));
        obj_gen_truoc1->setOpacity(0);
        obj_gen_truoc1->setLocked(true);
        objects_gen_truoc.push_back(obj_gen_truoc1);
        // answer sai
        Math_Object* obj2 = Math_Pool::getInstance()->getObject();
        int soSai;
        while(1){
            soSai = RandomHelper::random_int(ints_answer[i]-number_skip, ints_answer[i]+number_skip);
            if (soSai != ints_answer[i]) break;
        }
        this->genNumberBox(index_slots[i],obj2,soSai,image_box_answer,Color4B(Color3B(99,51,15)));
        obj2->setTouchCallback(CC_CALLBACK_1(Nam_MultiSelectNumberQuiz::onTouchedAnswer, this));
        obj2->setPosition(Vec2(images_box_choice[i]->getPositionX()+images_box_choice[i]->getContentSize().width/4,images_box_choice[i]->getPositionY()+6));
        this->addChildNode(obj2,2);
        obj2->onShow(0);
        obj2->setVisible(false);
        false_answers.push_back(obj2);
        auto obj_gen_truoc2 = obj2->clone();
        objects_gen_truoc.push_back(obj_gen_truoc2);
        obj_gen_truoc2->setPosition(Vec2(-200,-200));
        obj_gen_truoc2->setLocked(true);
        obj_gen_truoc2->setOpacity(0);
        // random dap an
        if (RandomHelper::random_int(0, 1)){
            obj1->setPosition(Vec2(images_box_choice[i]->getPositionX()+images_box_choice[i]->getContentSize().width/4,images_box_choice[i]->getPositionY()+6));
            obj2->setPosition(Vec2(images_box_choice[i]->getPositionX()-images_box_choice[i]->getContentSize().width/4,images_box_choice[i]->getPositionY()+6));
        }
    }
    isHide = false;
}

void Nam_MultiSelectNumberQuiz::onStart() {
	math_statistic->setCurrentGame(_game);
	
	//Gen question
    showQuestionBox();
    this->scheduleUpdate();
    
    
}

void Nam_MultiSelectNumberQuiz::startQuestion() {
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();
        for (auto obj : true_answers){
            obj->setVisible(true);
            obj->setOpacity(0);
            obj->setScale(0);
        }
        for (auto obj : false_answers){
            obj->setVisible(true);
            obj->setOpacity(0);
            obj->setScale(0);
        }
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(Nam_MultiSelectNumberQuiz::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}
}
void Nam_MultiSelectNumberQuiz::genNumberBox(int id, Math_Object* obj, int num,math::resource::Image* img_box,Color4B color){
    obj->setEnable(id, math::common::TouchPermission::touch, img_box, NULL);
    obj->setObjectSize(object_size);
    obj->setValueText(cocos2d::StringUtils::toString(num));
    obj->setComparisionValue("number",num);
    auto text = obj->getLabelValue();
    text->setTextColor(color);
    text->setVisible(true);
    text->disableEffect();
}

void Nam_MultiSelectNumberQuiz::onExplanation(bool correct) {}
void Nam_MultiSelectNumberQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node : _answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}
    float delay = showConratulation(0.3);
	float m_delay = 0.2;
	//m_delay += 0.5;
	for (Math_Object* object : _objects) {
        object->onHide(m_delay,math::animation::SlideUp);
	}
//    for (Math_Slot* slot : _slots) {
//        slot->onHide(m_delay);
//
//    }
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
bool Nam_MultiSelectNumberQuiz::onTouchBegan(Touch* touch, Event* event)
{
    float m_delay = 0.45f;
    for (int i = 0; i < images_box_choice.size(); i++){
        if (images_box_choice[i]->getOpacity()==255 && !images_box_choice[i]->getBoundingBox().containsPoint(touch->getLocation())){
            images_box_choice[i]->stopAllActions();
            true_answers[i]->stopAllActions();
            false_answers[i]->stopAllActions();
            images_box_choice[i]->runAction(Sequence::create(DelayTime::create(0.05),EaseBackIn::create(ScaleTo::create(m_delay-0.05,0)),nullptr));
            images_box_choice[i]->runAction(FadeOut::create(m_delay));
            true_answers[i]->runAction(FadeOut::create(m_delay));
            true_answers[i]->runAction(EaseBackIn::create(ScaleTo::create(m_delay-0.1,0)));
            false_answers[i]->runAction(EaseBackIn::create(ScaleTo::create(m_delay-0.1,0)));
            false_answers[i]->runAction(FadeOut::create(m_delay));
        }
    }
    return true;
}
void Nam_MultiSelectNumberQuiz::onTouchedObject(cocos2d::Node* node){
    Math_Object* object = (Math_Object*)node;
    float delay = 0.45f;
    for (int i = 0; i < images_box_choice.size(); i++){
        if (images_box_choice[i]->getTag() == object->getID()){
            if (images_box_choice[i]->getOpacity() == 0){
                images_box_choice[i]->stopAllActions();
                true_answers[i]->stopAllActions();
                false_answers[i]->stopAllActions();
                images_box_choice[i]->setScale(0);
                images_box_choice[i]->setOpacity(0);
                true_answers[i]->setScale(0);
                true_answers[i]->setOpacity(0);
                false_answers[i]->setScale(0);
                false_answers[i]->setOpacity(0);
                images_box_choice[i]->runAction(EaseBackOut::create(ScaleTo::create(delay,1)));
                images_box_choice[i]->runAction(FadeIn::create(delay));
                true_answers[i]->runAction(Sequence::create(DelayTime::create(0.05),EaseBackOut::create(ScaleTo::create(delay-0.05,1)),nullptr));
                true_answers[i]->runAction(FadeIn::create(delay));
                false_answers[i]->runAction(Sequence::create(DelayTime::create(0.05),EaseBackOut::create(ScaleTo::create(delay-0.05,1)),nullptr));
                false_answers[i]->runAction(FadeIn::create(delay));
            }
        }
    }
}
void Nam_MultiSelectNumberQuiz::onTouchedAnswer(cocos2d::Node* node){
    Math_Object* object = (Math_Object*)node;
    for (auto slot : _slots){
        if (slot->getId() == object->getID()){
            if (slot->isUsed()){
                if (slot->getLinker() == object){
                    return;
                }else{
                    auto obj = slot->getLinker();
                    slot->removeLinker();
                    obj->setOpacity(0);
                    for (auto o : objects_gen_truoc){
                        if (o->getValueText() == object->getValueText()){
                            o->runAction(FadeIn::create(0.35f));
                            o->setPosition(slot->getPosition());
                            slot->setLinker(o);
                        }
                    }
                }
            }else{
                for (auto o : objects_gen_truoc){
                    if (o->getValueText() == object->getValueText()){
                        o->runAction(FadeIn::create(0.35f));
                        o->setPosition(slot->getPosition());
                        slot->setLinker(o);
                    }
                }
            }
        }
    }
}
void Nam_MultiSelectNumberQuiz::onAnswerDone(Node* node, std::string str_ans) {
	Math_Answer* btn = (Math_Answer*)node;
	
}
void Nam_MultiSelectNumberQuiz::update(float dt){
    bool done = true;
    for (auto slot : _slots){
        if (!slot->isUsed()){
            done = false;
        }else{
            if (slot->getLinker()->getValueText() != _objects[slot->getId()]->getValueText()){
                done = false;
            }
        }
    }
    if (done){
        for (auto o : false_answers){
            o->setLocked(true);
        }
        float m_delay = 0.45f;
        for (int i = 0; i < images_box_choice.size(); i++){
            if (images_box_choice[i]->getOpacity() == 255 && isHide == false){
                isHide = true;
                images_box_choice[i]->stopAllActions();
                true_answers[i]->stopAllActions();
                false_answers[i]->stopAllActions();
                images_box_choice[i]->runAction(Sequence::create(DelayTime::create(0.05),EaseBackIn::create(ScaleTo::create(m_delay-0.05,0)),nullptr));
                images_box_choice[i]->runAction(FadeOut::create(m_delay));
                true_answers[i]->runAction(FadeOut::create(m_delay));
                true_answers[i]->runAction(EaseBackIn::create(ScaleTo::create(m_delay-0.1,0)));
                false_answers[i]->runAction(EaseBackIn::create(ScaleTo::create(m_delay-0.1,0)));
                false_answers[i]->runAction(FadeOut::create(m_delay));
            }
        }
        this->nextQuestion();
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_MultiSelectNumberQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_MultiSelectNumberQuiz::autoTestingRun(float delaytime)
{
   
}

