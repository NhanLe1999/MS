//
//  SumAndSubQuiz.cpp
//  MJQuiz
//
//  Created by Admin on 8/7/17.
//
//

//Game co 3 cap do (game_mode)
//Da hoan thanh ca 3 cap
//game_mode 1: cộng trừ theo group
//game_mode 2: chọn đáp án đúng
//game_mode 3: kéo thả + chọn đáp án đúng

#include "BA_SumAndSubQuiz.hpp"
////#include "AudioEngineHelper.h"
#include "BA_SumAndSubQuizFunction.h"

BA_SumAndSubQuiz* BA_SumAndSubQuiz::createQuiz(){
    
    BA_SumAndSubQuiz* quiz=new BA_SumAndSubQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_SumAndSubQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void BA_SumAndSubQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    max_range=json_game.FindMember("range")->value.FindMember("maximum")->value.GetInt();
    min_range=json_game.FindMember("range")->value.FindMember("minimum")->value.GetInt();
    
    game_mode = json_game.FindMember("game_mode")->value.GetInt();
    number_group = json_game.FindMember("group")->value.GetInt();
    
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());

    group_cap=number_max*2;
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    rapidjson::Value& group_names=json_group.FindMember("name")->value;
    for (rapidjson::SizeType i=0; i<group_names.Size(); i++) {
        list_groupname.push_back(group_names[i].GetString());
    }
    
    object_align=json_group.FindMember("align")->value.GetString();
    group_type=json_group.FindMember("type")->value.GetString();
    group_column=json_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
    group_row=json_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
    group_size=cocos2d::Size(json_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    //TODO: DRAGER CONFIG
    sort_key = getComparisionKey(sort_by);
    
    rapidjson::Value& json_drager=json_data.FindMember("drager")->value;
    number_drager= cocos2d::random(json_drager.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_drager.FindMember("number")->value.FindMember("maximum")->value.GetInt());
    
    drager_size=cocos2d::Size(json_drager.FindMember("size")->value.FindMember("width")->value.GetInt(), json_drager.FindMember("size")->value.FindMember("height")->value.GetInt());
    value_min=json_drager.FindMember("value")->value.FindMember("minimum")->value.GetInt();
    value_max=json_drager.FindMember("value")->value.FindMember("maximum")->value.GetInt();
    
    //TODO: SLOT CONFIG
    rapidjson::Value& json_slot=json_data.FindMember("slot")->value;
    slot_size = cocos2d::Size(json_slot.FindMember("size")->value.FindMember("width")->value.GetInt(), json_slot.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    if(game_mode == 1) {
        number_slot = number_group +1;
    } else {
        number_slot = number_group;
    }
    
    //TODO: OPERATOR CONFIG
    oprt_json = json_data.FindMember("operator")->value.GetString();
    number_oprt = number_group - 1 ;
    oprt_List = {"+", "-"};
    
    //---------------------------------
    // Gen operator
    math_symbols.clear();
    
   //
        for(int i = 0; i < number_oprt; i++){
            genOperator(oprt_json);
        }
     if(game_mode == 1){
        //create equal operator
        genOperator("=");
    }
//    else{
//        genOperator("+");
//    }
    
    //---------------------------------
    //group background
    math::resource::Image* group_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png") );
    
    // list images for object
    icon_list= math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
    for (math::resource::Image* img : icon_list) {
        img->path = image_prefix +img->path;
    }
    
    std::vector<math::resource::Image*> ob_list;
    for(int i = 0; i < icon_list.size()-2; i++){
        ob_list.push_back(icon_list.at(i));
    }
    
    //group type
    math::group::GroupType gr_type=math::group::custom;
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
        object_size = cocos2d::Size(50,50);
        group_cap = number_max;
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
    }
    //gr_type=math::group::queue;

    
    //------------------
    //random object image
    math::func::shuffle(ob_list);
    int random_img = 0;//rand()%ob_list.size();
    
    //--------------------
    //Gen group
    //create groups, group list, drag_value list
    
    for(int i = min_range; i < max_range+1; i++){
        value_list.push_back(i);
    }
    _groups.clear();
    
    int group_no;
    
    if(game_mode == 1){
        createObjectAndSum();
        group_no = number_group +1;
    }else {
        createSum(false);
        group_no = number_group;
    }
    
    for(int i = 0; i < group_no; i++){
        //creat group
        genGroup(i, random_img, gr_type, group_bg, ob_list);
    }
    
    
    if(game_mode==1){
        //---------------------------------
        //Gen order group list (game_mode 1)
        if(!oprt_json.compare("-")){
            std::reverse(_groups.begin(), _groups.end());
        }
        
        //---------------------------------
        //random more wrong values for drag_values list
        if(drag_values.size()!= 0){
            for(int i = (int) drag_values.size(); i >0; i-- ){
                drag_values.push_back(cocos2d::random(value_min, value_max));
            }
        }
    }
    
    
    //---------------------------------
    //image list
    auto number_imgs = getNumbers(0, 10);
    
    //--------------------
    //Gen Drager
    _dragers.clear();
    
    if(game_mode !=2){
        if(game_mode != 1){
            number_drager = max_range - min_range +1;
        }
        
        for (int i=0; i<number_drager; i++) {
            math::resource::Image* drag_img;
            for (math::resource::Image* img : number_imgs ) {
                if(game_mode ==1 ){
                    if (img->getComparisionValue("number")==drag_values[i]) {
                        drag_img=img;
                        break;
                    }
                }else {
                    if (img->getComparisionValue("number")== i+1 ) {
                        drag_img=img;
                        break;
                    }
                }
            }
            genDrager(drag_img);
        }
    }
    
    //---------------------------------
    //Gen Slot
    auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    if(game_mode!=2){
        for (int i=0; i<number_slot; i++) {
            genSlot(slot_bg);
        }
    }
    
    //---------------------------------
    //Gen answers
    _answers.clear();
    answer_list.clear();
    
    if(game_mode != 1){
        answer_list.push_back(number_objects[number_objects.size()-1]);
        
        while (answer_list.size() < 4) {
            int temp;
            temp = cocos2d::random(value_min, value_max);
            bool add = true;
            for (int i = 0; i < answer_list.size(); i++) {
                if (answer_list.at(i) == temp) {
                    add = false;
                    break;
                }
            }
            if (add){
                answer_list.push_back(temp);
            }
        }
        
        for (int i=0; i<4; i++) {
            Math_Answer* btn=Math_Pool::getInstance()->getAnswer();
            btn->setEnable(cocos2d::StringUtils::toString(answer_list[i]), math::common::answer, CC_CALLBACK_1(BA_SumAndSubQuiz::onTouchedAnswer, this), this->getAnswerSize(1));
            if(i == 0){
                btn->setCorrect(true);
            }
            multiChoice.push_back(btn);
        }
        
        math::func::shuffle(multiChoice);
    }
    if(game_mode != 2){
        //---------------------
        //Gen button done
//        Math_Answer* btn=Math_Pool::getInstance()->getAnswer();
//        btn->setEnable("Done", math::common::answer, CC_CALLBACK_1(BA_SumAndSubQuiz::onAnswerDone, this),this->getAnswerSize() );
//        btn->setPosition(cocos2d::Vec2(450, 55));
//        _answers.push_back(btn);
//
//         math::func::setPositionList(_answers, 1, cocos2d::Rect(quiz_size.width*0.5, quiz_size.height*0.05, quiz_size.width*0.8, quiz_size.height*0.15));
    }
    
    //parsing question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, ob_list);
    }
    
    if (img_bg!=NULL) {
        img_bg->used=true;
    }
   // //Math_Pool::getInstance()->resetImages();
}

//--------------------------------------------------
void BA_SumAndSubQuiz::onStart(){
    
    //---------------------------------
    //Gen question
    if(game_mode != 2){
        onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(BA_SumAndSubQuiz::onAnswerDone, this));
    }else{
        onShowBottomBar(0, 0.3);
    }
    
    showQuestionBox(0);
    
    //---------------------------------
    //set position of groups and objects
    if(game_mode == 1) {
        math::func::shuffle(_dragers);
        math::func::setPositionList(math_symbols, (int) math_symbols.size(), cocos2d::Rect(quiz_size.width* 0.16, quiz_size.height* 0.18, quiz_size.width* 0.68,quiz_size.height* 0.3));
        math::func::setPositionList(_slots, number_slot, cocos2d::Rect(0, 0.05, 1, 0.12));
       // math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(0, 0.1, 1, 0.2));
            math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(quiz_size.width*0.01, quiz_size.height*0.05, quiz_size.width*0.8, quiz_size.height*0.15));
    } else if (game_mode ==2 ) {
        math::func::setPositionList(math_symbols, (int) math_symbols.size(), cocos2d::Rect(quiz_size.width* 0.16, quiz_size.height* 0.18, quiz_size.width* 0.68,quiz_size.height* 0.3));
    } else if (game_mode == 3){
      //  math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(0, 0.1, 1, 0.2));
            math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(quiz_size.width*0.01, quiz_size.height*0.05, quiz_size.width*0.8, quiz_size.height*0.15));
        math::func::setPositionList(_slots, number_slot, cocos2d::Rect(0, 0.12, 1, 0.25));
    }
    
    //--------------------
    if (align_group_object.compare("left_right")==0) {
        math::func::setPositionList(_groups, 1, cocos2d::Rect(quiz_size.width* 0,quiz_size.height* 0.15,quiz_size.width* 1,quiz_size.height* 0.3));
    } else {
        math::func::setPositionList(_groups, (int)_groups.size(), cocos2d::Rect(quiz_size.width* 0,quiz_size.height* 0.15,quiz_size.width* 1,quiz_size.height* 0.35));
    }
    
}

void BA_SumAndSubQuiz::startQuestion() {
    float delay=0;
    enableControlQuiz();
    
    if(getQuestion()->getData().index == 0) {
        for (Math_Group* group:_groups) {
            addChildNode(group, 2);
            group->onShow();
        }
        
        for (Math_Object* drager : _dragers) {
            addChildNodeToBottomPanel(drager, 2);
            drager->onShow(delay);
            drager->enableDrag(_panel_bot);
            
            delay+=0.15;
        }
        
        if(game_mode == 2){
            math::func::setPositionList(multiChoice, 2, cocos2d::Rect(quiz_size.width* 0.1, quiz_size.height* 0.01, quiz_size.width* 0.78, quiz_size.height *0.25));
            
            for (Math_Answer* answer : multiChoice) {
                addChildNodeToBottomPanel(answer, 2);
                answer->onShowInParent(_panel_bot, 2);
            }
        } else {
            for (Math_Answer* answer : _answers) {
                addChildNodeToBottomPanel(answer, 2);
                answer->onShowInParent(_panel_bot, 2);
            }
        }
        
        for (Math_Slot* slot :  _slots) {
            this->addChildNode(slot, 2);
            slot->onShow(delay);
        }
        
        if(game_mode != 3){
            for (Math_Object* obj: math_symbols){
                this->addChildNode(obj, 2);
                obj->onShow(delay);
                delay += 0.1;
            }
        }
    } else {
        auto bg_list = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "box");
        center_pos = cocos2d::Vec2 (_panel->getContentSize().width/2 ,_groups.at(0)->getPosition().y );
        math::func::setPositionList(copy_obj, (int) copy_obj.size(), cocos2d::Rect(0.15, 0.25, 0.7, 0.2));
        math::func::setPositionList(math_symbols, (int) math_symbols.size(), cocos2d::Rect(0.15, 0.25, 0.7, 0.2));
        math::func::setPositionList(multiChoice, 2, cocos2d::Rect(quiz_size.width* 0.1, quiz_size.height* 0.008, quiz_size.width* 0.78, quiz_size.height *0.2));
        
        _groups.at(0)->runAction(cocos2d::MoveTo::create(0.35, cocos2d::Vec2(center_pos.x - group_size.width/2 +3, center_pos.y)));
        _groups.at(1)->runAction(cocos2d::MoveTo::create(0.35, cocos2d::Vec2(center_pos.x + group_size.width/2 -3, center_pos.y)));
        
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5f), cocos2d::CallFunc::create([=] {
            _groups.at(0)->setBackground(bg_list.at(bg_list.size()-2));
            _groups.at(1)->setBackground(bg_list.at(bg_list.size()-1));
        }), nullptr));
        
        for(Math_Object* object : copy_obj){
            object->setLocked(true) ;
            addChildNode(object);
            object->onShow(0.3);
        }
        
        for (Math_Object* obj: math_symbols){
            this->addChildNode(obj, 2);
            obj->onShow(0.5);
        }
        
        for(Math_Answer* btn : multiChoice){
            this->addChildNodeToBottomPanel(btn, 2);
            btn->onShowInParent(_panel_bot, 2);
            
        }
    }
}

void BA_SumAndSubQuiz::onExplanation(bool correct){
    if (!correct) {
        if(getQuestion()->getData().index == 0){
            //return wrong drager
            for (Math_Slot* slot : _slots) {
                if (slot->isUsed() && !slot->isCorrect(sort_key)) {
                    slot->getLinker()->backToStart();
                    slot->removeLinker();
                }
                else{
                    
                }
            }
        }
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
            enableControlQuiz();
        }), nullptr));
    }
    else {
        for (Math_Slot* slot :  _slots) {
            slot->onHide(0);
        }
        
        for (Math_Object* drager : _dragers) {
            drager->onHide(0);
        }
        
//        for(Math_Answer* answer: _answers){
//            answer->setDisable();
//        }
        
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=](){
            this->nextQuestion();
        }), NULL));
    }
    
}


void BA_SumAndSubQuiz::onComplete(){
    //TODO: Thuc hien them cac lenh o day
//    for (Math_Answer* node : _answers) {
//        if (node->isEnable()) {
//            node->setDisable();
//        }
//    }
    
    float delay = showConratulation(0.3);
    
    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }
    scheduleOnce([=](float dt) {
        for (Math_Group* group : _groups) {
            group->setDisable();
        }
        
    }, m_delay, "hide_groups");
    
    m_delay= delay-0.3;
    for (Math_Object* object : _objects) {
        object->onHide(m_delay);
        m_delay+=0.15;
    }
    
    for (Math_Answer* answer: multiChoice ){
        answer->setDisable();
    }
    
//    for(Math_Answer* answer: _answers){
//        answer->setDisable();
//    }
    
    for (Math_Object* drager : _dragers) {
        drager->onHide(delay);
    }
    for (Math_Slot* slot :  _slots) {
        slot->onHide(delay);
    }
    
    for (Math_Object* obj: math_symbols){
        obj->onHide(delay);
        delay += 0.1;
    }
    
    for(Math_Object* object : copy_obj){
        object->onHide(m_delay);
        m_delay +=0.15;
    }
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void BA_SumAndSubQuiz::onTouchedObject(cocos2d::Node *node){
}

void BA_SumAndSubQuiz::onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 position){
    disableControlQuiz();
    
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
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->backToStart();
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            //move start
            object->backToStart();
        }
    }
    
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
        enableControlQuiz();
    }), nullptr));
}

void BA_SumAndSubQuiz::onTouchedAnswer(cocos2d::Node *node){
    Math_Answer* answer = (Math_Answer*) node;
    bool done = false;
    
    if(clickable){
        clickable = false;
        if(answer->isCorrect() == true){
            done = true;
            
        }
        
        this->onExplanation(done);
        answer->onAnimationClick(done);
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=]() {
            clickable = true;
        }), NULL));
    }
    
}

void BA_SumAndSubQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    
    int count = 0;
    int correctAnswer = 0;
    
    
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed()){
            count++;
        }
    }
    
    if(count != number_slot){
        
        this->onExplanation(false);
        //_answers.at(0)->onAnimationClick(false);
        
        
    }else{
        
        correctAnswer = checkResult();
    }
    
    if(correctAnswer == number_slot){
        if(game_mode == 3){
            if(counting ==1 ){
                for(Math_Slot* slot: _slots){
                    
                    Math_Object* object_copy=Math_Pool::getInstance()->getObject();
                    object_copy= slot->getLinker()->clone();
                    
                    object_copy->setImageVisible(false);
                    object_copy->setValueText(cocos2d::StringUtils::toString(object_copy->getImageSource()->getComparisionValue("number")));
                    object_copy->getLabelValue()->setFontSize(object_size.width*1.5);
                    object_copy->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
                    //  object_copy->getLabelValue()->setAnchorPoint(cocos2d::Vec2(0.5, 0.6));
                    object_copy->showValueTextWithAnimation(false);
                    copy_obj.push_back(object_copy);
                }
                
            }
            counting++;
        }
        
        this->onExplanation(true);
       // _answers.at(0)->onAnimationClick(true);
        
    }
}

int BA_SumAndSubQuiz::checkResult(){
    
    int correctAnswer =0;
    
    for(int i = 0; i < number_slot; i++){
        
        if(_slots[i]->getLinker()->getImageSource()->getComparisionValue("number") != _groups[i]->getNumberObjects() ){
            
            _slots[i]->getLinker()->backToStart();
            _slots[i]->removeLinker();
        }
        else{
            
            correctAnswer++;
        }
        
    }
    
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
        enableControlQuiz();
    }), nullptr));
    
    return correctAnswer;
    
}

#define auto_testing_delay 2

void BA_SumAndSubQuiz::autoTesting(){
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_SumAndSubQuiz::autoTestingRun(float delaytime){
    
    //Choose answer
    bool done=true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed()) {
            done=false;
        }
    }
    
    if (done) {
        //Tat ca slot duoc keo object
        bool is_correct=true;
        for (Math_Slot* slot : _slots) {
            if (!slot->isCorrect(sort_key)) {
                is_correct=false;
            }
        }
        
        if (!is_correct) {
            //keo tha lai
            this->scheduleOnce([=](float dt) {
                this->autoTestingRun(delaytime);
            }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
            
        } else {
            //Ket thuc quiz
        }
        
        Math_Answer* answer=math::func::getRandomNode(_answers);
        answer->onAutoClick();
        
    } else {
        //Chua keo tha xong, tiep tuc keo tha
        //Chon 1 obj chua keo tha.
        std::vector<Math_Object*> object_list;
        bool finded;
        for (Math_Object* object : _dragers) {
            finded=false;
            for (Math_Slot* slot : _slots) {
                if (object->isSame(slot->getLinker())) {
                    finded=true;
                    break;
                }
            }
            if (!finded) {
                object_list.push_back(object);
            }
        }
        
        if(object_list.size() != 0){
            Math_Object* m_object=math::func::getRandomNode(object_list);
            //Chon danh sach slot chua keo tha
            std::vector<Math_Slot*> slot_list;
            for (Math_Slot* slot : _slots) {
                if (!slot->isUsed()) {
                    slot_list.push_back(slot);
                }
            }
            Math_Slot* m_slot=math::func::getRandomNode(slot_list);
            
            //move object to slot
            m_slot->setLinker(m_object);
            m_object->runAction(cocos2d::MoveTo::create(0.4, m_slot->getPosition()));
            
            this->scheduleOnce([=](float dt) {
                this->autoTestingRun(delaytime);
            }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
        }
        else {
            
            this->scheduleOnce([=](float dt) {
                this->autoTestingRun(delaytime);
            }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
            
        }
    }
}

