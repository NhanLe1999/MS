//
//  BA_MoreOrLessDragQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/14/17.
//
//

//status: Completed


#include "BA_MoreOrLessDragQuiz.hpp"

BA_MoreOrLessDragQuiz* BA_MoreOrLessDragQuiz::createQuiz()
{
    BA_MoreOrLessDragQuiz* quiz=new BA_MoreOrLessDragQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_MoreOrLessDragQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void BA_MoreOrLessDragQuiz::initQuiz(mj::model::Game game)
{
     
    Math_Quiz::initQuiz(game);    
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    type = json_game.FindMember("kind")->value.GetString();
    value_range = randomRangeJson(json_game, "range");
    number_group = json_game.FindMember("group")->value.GetInt();

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    std::string obj_display_value = json_object.FindMember("display_value")->value.GetString();
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    object_align=json_group.FindMember("align")->value.GetString();
    group_type=json_group.FindMember("type")->value.GetString();
    
    //TODO: DRAGER CONFIG
    rapidjson::Value& json_drager=json_data.FindMember("drager")->value;
    number_drager = 4;
    value_min=json_drager.FindMember("value")->value.FindMember("minimum")->value.GetInt();
    value_max=json_drager.FindMember("value")->value.FindMember("maximum")->value.GetInt();
    display_value = json_drager.FindMember("display_value")->value.GetString();
    
    //Icon config
    number_kind=1;
    img_list = getImageList("icons", 1, "id");
    
    //color4b vector
    while(result <= value_range){
        result =  cocos2d::random(number_min, number_max);
    }
    std::vector<int> oprt {result - value_range, result + value_range};
    
    if(result == number_min){
        result2 = result + value_range;
    }else if(result == number_max){
        result2 = result - value_range;
    }else{
        if (type == "more") {
            result2 = result + value_range;
        } else if (type == "less") {
            result2 = result - value_range;
        } else if (type == "equal") {
            result2 = result ;
        } else if (type == "moreless") {
            result2 = oprt[rand()%2];
        } else {
            oprt.push_back(result);
            result2 = oprt[rand()%3];
        }
    }
    
    //---------------------------------
    //Gen Object
    math::resource::Image* group_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png")) ;
    
    //group type
    math::group::GroupType gr_type=math::group::custom;
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
    }
    //Gen group
    int number_max = result > result2?result:result2;
    if (number_max<=5) {
        group_cap = 5;
        
        object_size =cocos2d::Size(100, 100);
        if (gr_type==math::group::frame) {
            object_size =cocos2d::Size(70, 70);
        }
        group_size =cocos2d::Size(610, 100);
        slot_size = cocos2d::Size(120, 120);
        
    } else if (number_max<=10) {
        group_cap = 10;
        object_size =cocos2d::Size(70, 70);
        group_size =cocos2d::Size(430, 100);
        slot_size = cocos2d::Size(120, 120);
    } else {
        group_cap = 10;
        object_size =cocos2d::Size(60, 60);
        group_size =cocos2d::Size(370, 100);
        slot_size = cocos2d::Size(120, 120);
    }
    
    std::vector<int> numbers;
    if (number_max<=10) {
        numbers.push_back(result);
        numbers.push_back(result2);
    } else {
        if (result<=10) {
            numbers.push_back(result);
            numbers.push_back(0);
        } else {
            numbers.push_back(10);
            numbers.push_back(result-10);
        }

        if (result2<=10) {
            numbers.push_back(result2);
            numbers.push_back(0);
        } else {
            numbers.push_back(10);
            numbers.push_back(result2-10);
        }
    }
    
    for (int i=0; i<numbers.size(); i++) {
        std::vector<Math_Object*> object_list;
        for (int j=0; j<numbers[i]; j++) {
            Math_Object* object = mpool->getObject();
            object->setEnable(i*10+j, math::common::deny, img_list[0], NULL);
            object->setObjectSize(object_size);
            object_list.push_back(object);
        }
        
        Math_Group* group = mpool->getGroup();
        group->setEnable(i, object_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        
        if (group_type.compare("frame")==0) {
            group->setGroupFrames(5, 2, group_bg);
        } else if (group_type.compare("queue")==0) {
            group->setGroupQueue(group_size);
            group->setBackground(group_bg);
        }else{
            group->setBackground(group_bg);
            group->setGroupSize(group_size);
        }
       
        _groups.push_back(group);
    }
    
    //image list
    auto number_imgs = getNumbers(value_min - 1, value_max + 1);
    
    //--------------------
    //Gen Drager
    std::vector<int> drag_value;
     drag_bg_img = objectBG();
    //set dragers's value
    if(result == result2){
        if(result == number_min){
            drag_value = {result, result2 + value_range, result2+ value_range *2, result2+ value_range*3};
        } else if (result == number_max){
            drag_value = {result-value_range*3, result- value_range*2, result-value_range, result};
        } else {
            if(result / value_range >2){
                drag_value = {result - value_range*2 ,result - value_range, result, result2+ value_range};
            }else{
                drag_value = {result - value_range, result, result2+ value_range, result + value_range*2 };
            }
        }
    }else if(result < result2){
        if(result == number_min){
            drag_value = {result, result2, result2 + value_range, result2+ value_range *2};
        }else if (result2 == number_max){
            drag_value = {result- value_range*2, result-value_range, result, result2};
        } else {
            drag_value = {result - value_range, result, result2, result2+ value_range};
        }
    }else{
        if(result == number_max){
            drag_value = {result2- value_range*2, result2-value_range, result2, result};
        }else if (result2 == number_min){
            drag_value = {result2, result, result+ value_range, result + value_range*2};
        }else{
            drag_value = {result2- value_range, result2, result, result + value_range};
        }
        
    }
    
    for(int i = 0 ; i < 4; i++){
        math::resource::Image* drag_img;
        for (math::resource::Image* img : number_imgs){
            if(drag_value[i] == img->getComparisionValue("number")){
                drag_img = img;
            }
        }
        
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, drag_img, drag_bg_img);
        object->setObjectSize(slot_size);
        if (display_value == "icon") {
            
        } else {
            if(display_value == "number"){
                object->setValueTextByKey("number");
                object->getLabelValue()->setFontSize(slot_size.height*0.8);
                
            }else if(display_value == "name"){
                object->setValueTextByKey("name");
                object->getLabelValue()->setFontSize(slot_size.height*0.5);
            }
            object->setImageVisible(false);
            object->showValueTextWithAnimation(false);
            //object->getLabelValue()->setTextColor(color_list[i]);
        }
        
        object->setMoveCallback(nullptr, CC_CALLBACK_2(BA_MoreOrLessDragQuiz::onMoveObjectEnded, this));
        addChildNodeToBottomPanel(object,3);
        _dragers.push_back(object);
    }
    
    //---------------------------------
    //Gen Slot
    math::resource::Image* obj_img;
    for (math::resource::Image* img : number_imgs){
        if(result == img->getComparisionValue("number")){
            obj_img = img;
        }
    }
    
    example_obj = mpool->getObject();
    example_obj->setEnable(0, math::common::deny, obj_img, drag_bg_img);
    if(display_value == "name"){
        example_obj->setObjectSize(cocos2d::Size(180, 50));
    }else {
        example_obj->setObjectSize(slot_size);
    }
    if(display_value == "icon"){
        
    }else{
        if(display_value == "number"){
            example_obj->setValueTextByKey("number");
            example_obj->getLabelValue()->setFontSize(slot_size.height*0.8);
            
        }else if(display_value == "name"){
            example_obj->setValueTextByKey("name");
            example_obj->getLabelValue()->setFontSize(slot_size.height*0.5);
        }
        example_obj->setImageVisible(false);
        example_obj->showValueTextWithAnimation(false);
    }
    _results.push_back(example_obj);
    
    
    auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    Math_Slot* slot=mpool->getSlot();
    slot->setEndble(1, slot_bg);
    if(display_value == "name"){
        slot->setSlotSize(cocos2d::Size(200, 65));
    }else {
        slot->setSlotSize(slot_size);
    }
    _slots.push_back(slot);
    
    math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(quiz_size.width*0.02, quiz_size.height*0.0, quiz_size.width*0.7, quiz_size.height* 200.0/quiz_size.height - 20));
    
//    Math_Answer* btn=mpool->getAnswer();
//    btn->setEnable("Done", math::common::done, CC_CALLBACK_1(BA_MoreOrLessDragQuiz::onAnswerDone, this), getAnswerSize(2));
//    addChildNodeToBottomPanel(btn);
//    //btn->setPosition(cocos2d::Vec2(450, 55));
//    _answers.push_back(btn);
//
//    math::func::setPositionList(_answers, 1, cocos2d::Rect(quiz_size.width*0.4, quiz_size.height*0.05, quiz_size.width*0.8, quiz_size.height*0.15));
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    std::vector<math::resource::Image*> img_number_list;
    for (math::resource::Image* img : number_imgs) {
        if (img->getComparisionValue("number")==result) {
            img_number_list.push_back(img);
        }
    }    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    //mpool->resetImages();
}

//--------------------------------------------------
void BA_MoreOrLessDragQuiz::onStart(){
    float delay=0;

    //---------------------------------
    //Gen question
    float bar_h = 200.0/quiz_size.height;
    onShowBottomBar(0, bar_h, true, CC_CALLBACK_1(BA_MoreOrLessDragQuiz::onAnswerDone, this));
    showQuestionBox(delay);
    
    //---------------------------------
    //set position of groups and objects
//    group_size = _groups.front()->getContentSize();
//   // auto space_size = quiz_size.height-(getQuestion()->getContentSize().height+quiz_size.height*bar_h);
//    if (number_max > 10) {
//        float total_width = group_size.width*2 + slot_size.width + 5;
//        float delta = 40;
//        while (total_width+delta > 900) {
//            delta-=5;
//        }
//
//        float group_x = (quiz_size.width - (total_width+delta) + group_size.width)/2;
//        float slot_x = group_x + delta + group_size.width + 5 + (group_size.width + slot_size.width)/2;
//
//        _groups[0]->setPosition(cocos2d::Vec2(group_x, 280));
//        _groups[1]->setPosition(cocos2d::Vec2(group_x+5+group_size.width, 300));
//
//        _groups[2]->setPosition(cocos2d::Vec2(group_x, 120));
//        _groups[3]->setPosition(cocos2d::Vec2(group_x+5+group_size.width, 120));
//
//        example_obj->setPosition(cocos2d::Vec2(slot_x, 300));
//        _slots[0]->setPosition(cocos2d::Vec2(slot_x, 120));
//
//    } else {
//        float total_width = group_size.width + slot_size.width;
//        float delta = 40;
//        while (total_width+delta > quiz_size.width) {
//            delta-=5;
//        }
//
//        float group_x = (quiz_size.width - (total_width+delta) + group_size.width)/2;
//        float slot_x = group_x + delta + (group_size.width + slot_size.width)/2;
//
//        _groups[0]->setPosition(cocos2d::Vec2(group_x, 260));
//        _groups[1]->setPosition(cocos2d::Vec2(group_x, 60));
//        example_obj->setPosition(cocos2d::Vec2(slot_x, 300));
//        _slots[0]->setPosition(cocos2d::Vec2(slot_x, 90));
//    }
    
    math::func::setPositionList(_groups, (int)_groups.size()/2, cocos2d::Rect(quiz_size.width*0.02, quiz_size.height*0.0, quiz_size.width*0.78, quiz_size.height*(1-bar_h)- getQuestion()->getContentSize().height-20));
    
    example_obj->setPosition(cocos2d::Vec2(quiz_size.width*0.9,_groups.front()->getPosition().y));
    _slots.front()->setPosition(cocos2d::Vec2(quiz_size.width*0.9,_groups.back()->getPosition().y));
    
    
    for(Math_Group* group : _groups){
        addChildNode(group,2);
        group->onShow();
    }
    
    for(Math_Object*  obj : _results){
        addChildNode(obj);
        obj->onShow(delay);
    }
    
    for(Math_Slot*  obj : _slots){
        addChildNode(obj);
        obj->onShow(delay);
    }
    
   
    

    for(Math_Object*  obj : _dragers){
        obj->onShow(delay);
    }
    
}

void BA_MoreOrLessDragQuiz::startQuestion() {
     
    enableControlQuiz();
//    for(Math_Answer*  ans : _answers){
//       // ans->setPosition(cocos2d::Vec2(quiz_size.width * 0.125, quiz_size.height*0.05));
//        ans->onShowInParent(_panel_bot, 2);
//    }
    
    for(Math_Object*  obj : _dragers){
        obj->enableDrag(_panel_bot);
    }
}


void BA_MoreOrLessDragQuiz::onExplanation(bool correct){
    
    if (!correct) {
        enableControlQuiz();
        //return wrong drager
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() && !slot->isCorrect(math::comparision::number)) {
                slot->getLinker()->backToStart();
                slot->removeLinker();
            }
            else{
                
            }
        }
    }
    else {
        this->nextQuestion();
    }
    
}

void BA_MoreOrLessDragQuiz::onComplete(){
     
    //TODO: Thuc hien them cac lenh o day
    
    float delay = showConratulation(0.3);
//    for (Math_Answer* node : _answers) {
//        if (node->isEnable()) {
//            node->setDisable();
//        }
//    }

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
    for (Math_Object* object : _results) {
        object->onHide(m_delay);
        m_delay+=0.15;
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
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");

}

void BA_MoreOrLessDragQuiz::onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 position){
    
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

void BA_MoreOrLessDragQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    bool done=false;
    
    if (_slots.front()->isUsed()) {
        if(_slots.front()->getLinker()->getImageSource()->getComparisionValue("number") == result2){
            done = true;
        }
    }
    
    this->onExplanation(done);
   // _answers.at(0)->onAnimationClick(done);
}


#define auto_testing_delay 2

void BA_MoreOrLessDragQuiz::autoTesting(){
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_MoreOrLessDragQuiz::autoTestingRun(float delaytime){
    
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
        if (_slots.front()->getLinker()->getComparisionValue().number == result2) {
            is_correct = true;
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
