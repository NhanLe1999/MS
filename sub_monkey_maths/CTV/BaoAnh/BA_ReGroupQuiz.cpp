//
//  BA_ReGroupQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/2/17.
//

#include "BA_ReGroupQuiz.hpp"
BA_ReGroupQuiz* BA_ReGroupQuiz::createQuiz()
{
    BA_ReGroupQuiz* quiz=new BA_ReGroupQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_ReGroupQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_ReGroupQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    
    std::string object_kind = json_game.FindMember("kind")->value.GetString();
    
    number_min = json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    number_max = json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    std::string obj_display_value = json_object.FindMember("display_value")->value.GetString();
    
    int width = json_object.FindMember("size")->value.FindMember("width")->value.GetInt();
    int height = json_object.FindMember("size")->value.FindMember("height")->value.GetInt();
    
    object_size = cocos2d::Size(width, height);
    
    //TODO: SLOT CONFIG
    rapidjson::Value& json_slot=json_data.FindMember("slot")->value;
    slot_size = cocos2d::Size(json_slot.FindMember("size")->value.FindMember("width")->value.GetInt(), json_slot.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    //PARSING QUESTION
    parseQuestion();
    
    number_object = cocos2d::random(number_min, number_max);
    CCLOG("num : %i", number_object);
    
    int number_answer = (int) (cocos2d::StringUtils::toString(number_object)).size();
    
    for(int i = 0 ; i< number_answer; i++){
        number.push_back(number_object%10);
        number_object /= 10;
    }
    
    std::vector<int> expr_1;
    int chon, rand_no, thua =0;
    bool create = true;
    int count =0;
    number_reverse = number;
    
    std::reverse(number_reverse.begin(), number_reverse.end());
    
    while (create) {
        expr_1.clear();
        for(int i = (int)number.size()-1 ; i > 0; i--){
            
            rand_no = cocos2d::random(0, number.at(i));
            chon = rand_no + thua*10;
            thua = number.at(i) - rand_no ;
            CCLOG("chon: %i", chon);
            expr_1.push_back(chon);
            
        }
        expr_1.push_back(thua*10+ number.at(0));
        
        for(int i = 0 ; i < expr_1.size(); i++){
            CCLOG("expr: %d", expr_1.at(i));
            CCLOG("ex: %i", number_reverse.at(i));
            if(expr_1.at(i) == number_reverse.at(i)){
                count++;
            }
        }
        if(count != expr_1.size()){
            create = false;
        }
    }
    
    int fontSize = object_size.height*0.5;
    if(number_answer >2){
        fontSize *= 0.6;
    }
    if(object_kind == "number"){
        fontSize *= 1.5;
    }
    
    //Gen object
    auto obj_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
    //math::resource::Image* number_bg = getBackground("circle");
    
    std::vector<Math_Object*> expr_1_no;
    std::vector<Math_Object*> expr_1_base ;
    std::vector<Math_Object*> expr_1_oprt ;
    
    for(int i = 0 ; i < expr_1.size(); i++){
        
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(i, math::common::deny, obj_bg, obj_bg);
        object->setImageVisible(false);
        if(object_kind != "number"){
            object->setBackgroundVisible(false);
        }
        object->setObjectSize(object_size);
        object->getLabelValue()->setFontSize(object_size.height*0.8);
        object->setComparisionValue("number", expr_1.at(i));
//        object->setValueTextByKey("number");
        object->setValueText(cocos2d::StringUtils::toString(expr_1.at(i)));
        object->showValueTextWithAnimation(false);
        
        expr_1_no.push_back(object);
    }
    
    //Gen bases
   // math::resource::Image* slot_bg = getBackground("box");
    
    for(int i = 0; i < number_answer +1; i++){
        Math_Object* bases = Math_Pool::getInstance()->getObject();
        bases->setEnable(0, math::common::deny,obj_bg , NULL);
        
        Math_Object* bases_1 = Math_Pool::getInstance()->getObject();
        bases_1->setEnable(0, math::common::deny,obj_bg , NULL);
        
        //Math_Object* bases_1 = bases->clone();
        //Math_Pool::getInstance()->getObject() ;
        //bases_1->cloneFrom(bases);
        
        bases->setImageVisible(false);
        
        if(object_kind == "number"){
            if(i == number_answer -1 ){
                bases->setValueText("");
                bases_1->setValueText("");
            } else if (i == number_answer -2){
                bases->setValueText("0");
                bases_1->setValueText("0");
            } else if(i == number_answer -3){
                bases->setValueText("00");
                bases_1->setValueText("00");
            } else if(i == number_answer -4) {
                bases->setValueText("000");
                bases_1->setValueText("000");
            }
        }else{
            if(i == number_answer -1 ){
                bases->setValueText("ones");
                bases_1->setValueText("ones");
            } else if (i == number_answer -2){
                bases->setValueText("tens");
                bases_1->setValueText("tens");
            } else if(i == number_answer -3){
                bases->setValueText("hundreds");
                bases_1->setValueText("hundreds");
            } else if(i == number_answer -4) {
                bases->setValueText("thousands");
                bases_1->setValueText("thousands");
            }
        }
        
        bases->getLabelValue()->setFontSize(fontSize);
        bases->showValueTextWithAnimation(false);
        
        expr_base.push_back(bases);
        
        bases_1->setImageVisible(false);
        bases_1->showValueTextWithAnimation(false);
        bases_1->getLabelValue()->setFontSize(fontSize);
        expr_1_base.push_back(bases_1);
    }
    
    
    auto oprt_imgs = getOperator("+");
    
    //Gen oprt
    int number_oprt = number_answer -1;
    for(int i = 0; i < number_oprt; i++){
        Math_Object* oprt = Math_Pool::getInstance()->getObject();
        oprt->setEnable(0, math::common::deny, oprt_imgs , NULL);
        oprt->setImageVisible(false);
        oprt->setValueTextByKey("name_n");
        oprt->getLabelValue()->setFontSize(object_size.height);
        oprt->showValueTextWithAnimation(false);
        expr_oprt.push_back(oprt);
        
       // Math_Object* oprt_1 = oprt->clone();
        //oprt_1->clone();
        Math_Object* oprt_1 = Math_Pool::getInstance()->getObject();
        oprt_1->setEnable(0, math::common::deny, oprt_imgs , NULL);
        oprt_1->setImageVisible(false);
        oprt_1->setValueTextByKey("name_n");
        oprt_1->getLabelValue()->setFontSize(object_size.height );
        oprt_1->showValueTextWithAnimation(false);
        expr_1_oprt.push_back(oprt_1);
    }
    
    auto equal_oprt = getOperator("=");
    Math_Object* equal = Math_Pool::getInstance()->getObject();
    equal->setEnable(0, math::common::deny, equal_oprt, NULL);
    equal->setImageVisible(false);
    equal->setValueTextByKey("name_n");
    equal->getLabelValue()->setFontSize(object_size.height );
    equal->showValueTextWithAnimation(false);
    
    expr_oprt.push_back(equal);
    expr_1_oprt.push_back(equal);
    
    
    //Gen slot
    auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    for(int i = 0; i< number_answer; i++){
        
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(0, slot_bg) ;
        slot->setSlotSize(object_size);
        _slots.push_back(slot);
    }
    
    
    //Gen drager
    number_list = getNumbers(0, 9);
    drag_bg = objectBG();
    for(int i = 0; i < 10; i++){
        math::resource::Image* drag_img;
        for (math::resource::Image* img : number_list){
            if(i == img->getComparisionValue("number")){
                drag_img = img;
            }
        }
        
        Math_Object* drager = Math_Pool::getInstance()->getObject();
        drager->setEnable(i, math::common::deny, drag_img, drag_bg);
        drager->setComparisionValue("number", i);
        drager->setImageVisible(false);
        drager->setObjectSize(object_size);
        drager->setValueTextByKey("number");
        drager->showValueTextWithAnimation(false);
        drager->getLabelValue()->setFontSize(object_size.height*0.5);
        drager->setMoveCallback(nullptr, CC_CALLBACK_2(BA_ReGroupQuiz::onMoveObjectEnded, this));
        
        _dragers.push_back(drager);
        
//        Math_Slot* drag_slot = Math_Pool::getInstance()->getSlot();
//        drag_slot->setEndble(i, slot_bg);
//        drag_slot->setBackgroundVisible(false);
//        drag_slots.push_back(drag_slot);
    }
    
    
    //setPosition
    cocos2d::Vec2 expr_position;
    int expr_column = 3* number_answer ;
    expr_position.x = _panel->getContentSize().width/(expr_column+ 1);
    int startPosX = expr_position.x + 100;
    int up_space = 200;
    if(expr_column > 4)
        if(object_kind == "number"){
            expr_position.x += 12 ;
            startPosX -= 25;
            up_space = 150;
            
        }
    int delta =12;
    expr_position.y = quiz_size.height* 0.12;
    
    for(int i = 0 ; i< _slots.size() ; i++){
        
        Math_Slot* slot = _slots.at(i);
        slot->setPosition(cocos2d::Vec2( startPosX + expr_position.x*(i*3), expr_position.y -15));
        
        Math_Object* expr_number = expr_1_no.at(i);
        expr_number->setPosition(cocos2d::Vec2( startPosX + expr_position.x*(i*3), expr_position.y + up_space));
        
        if(_slots.size() > 2 && object_kind == "text"){
            
            Math_Object* base_1 = expr_1_base.at(i);
            base_1->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3), expr_position.y - delta *6 + up_space));
            
            Math_Object* oprt_1 = expr_1_oprt.at(i);
            oprt_1->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+1.5), expr_position.y +up_space));
            
            Math_Object* base = expr_base.at(i);
            base->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3), expr_position.y - delta *6));
            
            Math_Object* oprt = expr_oprt.at(i);
            oprt->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+1.5), expr_position.y));
            
        }else{
            Math_Object* base_1 = expr_1_base.at(i);
            base_1->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+1), expr_position.y + up_space));
            
            Math_Object* oprt_1 = expr_1_oprt.at(i);
            oprt_1->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+2), expr_position.y +up_space));
            
            Math_Object* base = expr_base.at(i);
            base->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+1), expr_position.y));
            
            Math_Object* oprt = expr_oprt.at(i);
            oprt->setPosition(cocos2d::Vec2(startPosX + expr_position.x*(i*3+2), expr_position.y));
            
        }
        
    }
    
    math::func::setPositionList(_dragers, (int)_dragers.size()/2, cocos2d::Rect(quiz_size.width* 0.02,quiz_size.height* 0,quiz_size.width* 0.7,quiz_size.height* 0.3));
    
   // math::func::setPositionList(drag_slots, (int)drag_slots.size(), cocos2d::Rect(0, 0.1, 1, 0.2));
    
    cocos2d::Vec2 equal_pos(quiz_size.width/12 ,quiz_size.height*0.3);
    if(object_kind == "number"){
        equal_pos.x = quiz_size.width/14;
    }
    equal->setPosition(equal_pos);
    equal->onShow(0.3);
    addChildNode(equal);
    
    for(Math_Object* obj: _dragers){
        obj->onShow(0);
        addChildNodeToBottomPanel(obj,3);
    }
    
    //onShow slots, numbers
    for(int i = 0; i < _slots.size(); i++){
        Math_Slot* slot = _slots[i];
        addChildNode(slot);
        slot->onShow(1);
        
        Math_Object* obj = expr_1_no.at(i);
        addChildNode(obj);
        obj->onShow(1);
    }
    
    //onShow oprts
    for(int i = 0; i < expr_oprt.size()-1; i++){
        Math_Object* obj = expr_oprt.at(i);
        addChildNode(obj);
        obj->onShow(1);
        
        Math_Object* obj_1 = expr_1_oprt.at(i);
        addChildNode(obj_1);
        obj_1->onShow(1);
    }
    
    //onShow bases
    for(int i = 0; i < expr_base.size(); i++){
        Math_Object* obj = expr_base[i];
        addChildNode(obj);
        obj->onShow(1);
        
        Math_Object* obj_1 = expr_1_base[i];
        addChildNode(obj_1);
        obj_1->onShow(1);
    }
}

void BA_ReGroupQuiz::onStart() {

    onShowBottomBar(0, 0.35, true, CC_CALLBACK_1(BA_ReGroupQuiz::onAnswerDone, this));
    showQuestionBox(0);
    
}

void BA_ReGroupQuiz::startQuestion() {
    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();
    }
    
    for(Math_Object* obj: _dragers){
        obj->enableDrag(_panel_bot);
    }
}

void BA_ReGroupQuiz::onExplanation(bool correct) {
    if(correct){
        this->nextQuestion();
    }else{
        enableControlQuiz();
        //return wrong drager
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() && !slot->isCorrect(math::comparision::number)) {
                slot->getLinker()->setDisable();
                slot->removeLinker();
            }
            else{
                
            }
        }
    }
}

void BA_ReGroupQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
//    for (Math_Answer* node : _answers) {
//        if (node->isEnable()) {
//            node->setDisable();
//        }
//    }
//
    float delay = showConratulation(0.3);
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - object touch events callback
void BA_ReGroupQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
    disableControlQuiz();
    bool create = false;
    
    Math_Object* object=(Math_Object*)node;
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
            create = true;
            break;
        }
        
    }
    
    if (m_slot!=NULL) {
        if (t_slot != NULL) {
            //swap
            if (m_slot == t_slot) {
                object->backToLast(2);
            } else {
                if (t_slot->isUsed()) {
                    auto linker = t_slot->getLinker();
                    m_slot->removeLinker();
                    t_slot->removeLinker();
                    
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    
                    m_slot->setLinker(linker);
                    linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                }
            }
            create = false;
        } else {
            //move to start
            m_slot->removeLinker();
            // object->backToStart();
            //  object->runAction(cocos2d::MoveTo::create(0.2,drag_slots.at(object->getID())->getPosition() ));
            object->setDisable();
            //create = true;
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                //linker->backToStart();
                linker->setDisable();
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            //create = true;
        } else {
            //move start
            object->backToStart();
           // object->runAction(cocos2d::MoveTo::create(0.2,drag_slots.at(object->getID())->getPosition() ));
            // object->setDisable();
            // create = true;
        }
    }
    
    
    if(create){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        
        obj->setEnable(object->getID(), math::common::touch, number_list.at(object->getID()), drag_bg);
        
        obj->setImageVisible(false);
        
        obj->setComparisionValue("number", obj->getID()) ;
        obj->setValueTextByKey("number");
        obj->setObjectSize(object_size);
        obj->getLabelValue()->setFontSize(object_size.height*0.5);
        obj->showValueTextWithAnimation(false);
        obj->setPosition(object->getStartPosition());
        obj->setMoveCallback(nullptr, CC_CALLBACK_2(BA_ReGroupQuiz::onMoveObjectEnded, this));
        obj->onShow(0);
        addChildNodeToBottomPanel(obj,3);
 //       CCLOG("image: %s", obj->getValueText().c_str());
        obj->enableDrag(_panel_bot);
    }
    
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
        enableControlQuiz();
    }), nullptr));
    
    
}

void BA_ReGroupQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    
    int count = 0;
    bool done = false;
    
    
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed()){
            count++;
        }
    }
    
    if(count != _slots.size()){
        
        done = false;
        
    }else{
        int check = 0;
        for(int i = 0 ; i < _slots.size(); i++){
            Math_Slot* slot = _slots.at(i);
            if(slot->getLinker()->getID() == number_reverse.at(i)){
                
                check ++;
            }
            
        }
        if(check == _slots.size()){
            done = true;
        }
    }
    
    this->onExplanation(done);
    //_answers.at(0)->onAnimationClick(done);
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 4
void BA_ReGroupQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_ReGroupQuiz::autoTestingRun(float delaytime)
{
   // _answers[0]->onAutoClick();
}

