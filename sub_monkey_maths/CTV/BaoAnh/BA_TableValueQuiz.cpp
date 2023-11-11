//
//  BA_TableValueQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/4/17.
//
//completed

#include "BA_TableValueQuiz.hpp"
BA_TableValueQuiz* BA_TableValueQuiz::createQuiz()
{
    BA_TableValueQuiz* quiz=new BA_TableValueQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_TableValueQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_TableValueQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    rapidjson::Value& total_object = json_game.FindMember("value_range")->value;
    number_max= total_object.FindMember("maximum")->value.GetInt();
    number_min= total_object.FindMember("minimum")->value.GetInt();
//     display_level = json_game.FindMember("display_level")->value.GetInt();
    int obj_number = cocos2d::random(number_min, number_max);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    
    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    //title_chart = json_chart.FindMember("chart_title")->value.GetBool();
    //chart_type =  json_chart.FindMember("chart_type")->value.GetString();
    vertical =  json_chart.FindMember("vertical")->value.GetBool();
    item_title = json_chart.FindMember("item_title")->value.GetBool();
    item_icon = json_chart.FindMember("icon_item")->value.GetBool();
    chart_size = json_chart.FindMember("title_size")->value.GetDouble();
    rapidjson::Value& chart_names=json_chart.FindMember("name")->value;
    std::string chart_title = chart_names.FindMember("chart_title")->value.GetString() ;
    std::string chart_icon_title = chart_names.FindMember("chart_icon_title")->value.GetString();
    
    
    //PARSING QUESTION
    parseQuestion();
    
    
    //----------------------------------
    
    std::vector<math::resource::Image*> name_list;
    auto obj_list=math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
    display_level = 3;
    if(obj_number > 100 ){
        display_level = 4;
    }
//    auto img_bg== math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"));
    //----------------------------------
    //Gen objects
    int obj_id=0;
    
    general_object = Math_Pool::getInstance()->getObject();
    general_object->setEnable(obj_id, math::common::deny, obj_list.at(0), NULL);
    general_object->setObjectSize(object_size*1.5);
    general_object->setImageVisible(false);
    general_object->setValueText(cocos2d::StringUtils::toString(obj_number));
    general_object->showValueTextWithAnimation(false);
//    object->getLabelValue()->setFontSize(object->getContentSize().height*0.8);
//    _objects.push_back(general_object);
    //object->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
  //  object->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*0.5));
//    addChildNode(object);
//    object->onShow(0);
    
    //number_list
    while(obj_number) {
        number_obj.push_back(obj_number % 10);
        obj_number /= 10;
    }

    
    for(int i = 0; i < display_level; i++){
        name_list.push_back(obj_list.at(i));
    }
    std::reverse(name_list.begin(), name_list.end());
    
  
    //--------------------
    //Gen graph
   
    math::resource::Image* obj_bg;
//    auto graph_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));
    std::vector<math::resource::Image*> icons_bg = {
        math::resource::ResHelper::createImage(getUIPath("common/frame/red.png")),
        math::resource::ResHelper::createImage(getUIPath("common/frame/yellow.png")),
        math::resource::ResHelper::createImage(getUIPath("common/frame/green.png")),
        math::resource::ResHelper::createImage(getUIPath("common/frame/blue.png"))
    };
    
    while (object_size.width*chart_size*display_level > quiz_size.width - object_size.width*2) {
        object_size.width -= 10;
        object_size.height -=10;
    }
    

    cocos2d::Size cell_sz = cocos2d::Size(object_size.width*chart_size+5, object_size.height+12);
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < display_level; j++){
            if(i == 0){
                obj_bg = icons_bg.at(j);
            }else{
                obj_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square3.png")) ;
                cell_sz = cocos2d::Size(object_size.width*chart_size, object_size.height+10);
            }
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(j, math::common::deny, name_list.at(j), obj_bg);
            obj->setImageVisible(false);
            obj->setBackgroundVisible(true);
            if(i ==0){
                obj->setValueText(name_list.at(j)->getText());
                obj->showValueTextWithAnimation(false);
            }
            obj->setObjectSize(cell_sz);
            _objects.push_back(obj);
        }
    }

//    table = math::Table::createGraph();
//    for(int i = 0 ; i < display_level; i++){
//        Math_Object* obj = Math_Pool::getInstance()->getObject();
//        obj->setEnable(i, math::common::deny, name_list.at(i), NULL);
//        obj->setImageVisible(false);
//        table->addItem(obj, 0);
//    }
//
//    table->setGraphCap(2);
//    table->setItemSize(object_size);
////    table->seItemScale(chart_size);
//    table->setItemTitleScale(chart_size);
//    table->setDirectionVertical(vertical);
//    table->showItemTitleAsIcon(item_title);
////    table->setChartTitleSize(chart_size);
//    table->setChartBackground(graph_bg);
//    table->setTitleChar(title_chart);
//    if(title_chart){
//        table->setTitleNumber(chart_icon_title);
//        table->setTitleItem(chart_title);
//    }
//  //  table->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height* 0.26));
//    table->setColorIconTitle(true, icons_bg);
//
//    addChildNode(table);
//    table->math::Table::reload();
    
    
    auto slot_img = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    auto dis_slot_img = math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"));
    //--------------------
    //Gen slot
    auto slot_size = object_size*0.9;
    for(int i = 0; i < number_obj.size(); i++){
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i,slot_img );
        slot->setSlotSize(slot_size);
        _slots.push_back(slot);
       
    }
    
    //Gen disabled slot
    for(int i = 0; i< display_level; i++){
        Math_Object* bg_obj = mpool->getObject();
        bg_obj->setEnable(i, math::common::deny, dis_slot_img, dis_slot_img);
        bg_obj->setObjectSize(slot_size);
        dis_slots.push_back(bg_obj);
    }
    //--------------------
    //Gen dragers
    number_list = getNumbers(0, 9);
    drag_bg_img = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));
    
    while(number_obj.size() < display_level){
        int numb = cocos2d::random(0, 9);
        bool add = true;
        for(int i: number_obj){
            if(i == numb){
                add = false;
            }
        }
        if(add){
            number_obj.push_back(numb);
        }
    }
    
    for(int i = 0; i < display_level; i++){
        math::resource::Image* drag_img;
        for (math::resource::Image* img : number_list){
            if(number_obj.at(i) == img->getComparisionValue("number")){
                drag_img = img;
            }
        }
        
        Math_Object* drager = Math_Pool::getInstance()->getObject();
        drager->setEnable(number_obj.at(i), math::common::deny, drag_img, drag_bg_img);
        drager->setImageVisible(false);
        drager->setObjectSize(object_size*0.9);
        drager->setValueTextByKey("number");
        drager->showValueTextWithAnimation(false);
        drager->getLabelValue()->setFontSize(object_size.height*0.8);
       // drager->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
        
        drager->setMoveCallback(nullptr, CC_CALLBACK_2(BA_TableValueQuiz::onMoveObjectEnded, this));
        dragers.push_back(drager);
    }
    
    math::func::shuffle(dragers);
    
    
  
}

void BA_TableValueQuiz::onStart() {
  
    //---------------------------------
    //Gen question
    float bar_h =220.0/quiz_size.height;
    onShowBottomBar(0, bar_h, true, CC_CALLBACK_1(BA_TableValueQuiz::onAnswerDone, this));
    showQuestionBox(0) ;
    
    for(Math_Object* obj: dragers){
        addChildNodeToBottomPanel(obj);
        obj->onShow(0);
    }
    
    general_object->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*(1-bar_h)- getQuestion()->getContentSize().height - general_object->getContentSize().height/2 - 60));
    addChildNode(general_object,3);
    general_object->onShow(0);
    
    
    math::func::setPositionListOnFrame(_objects, display_level, 2, _objects.front()->getContentSize(), cocos2d::Vec2(quiz_size.width / 2, (quiz_size.height * (1-bar_h) - getQuestion()->getContentSize().height - general_object->getContentSize().height) / 2 ), 0);
    
    for(Math_Object* obj: _objects){
        addChildNode(obj,2);
        obj->onShow(0);
    }
    
    background = cocos2d::ui::ImageView::create(getUIPath("common/frame/square.png"));
    background->setScale9Enabled(true);
    background->setContentSize(cocos2d::Size((object_size.width*chart_size) * display_level +20, (object_size.height + 1)*2 + 30));
    background->setPosition(cocos2d::Vec2(quiz_size.width / 2, (quiz_size.height * (1-bar_h) - getQuestion()->getContentSize().height - general_object->getContentSize().height) / 2 - 4 ));
    _panel->addChild(background, 1);
    
//    table->setAnchorPoint(cocos2d::Point(0.5, 0.5));
//    table->setPosition(cocos2d::Vec2(object->getPosition().x, (object->getPosition().y - object->getContentSize().height/2 )/2));
//     addChildNode(table);

    
//     auto dis_slot_size = cocos2d::Size(table->getContentSize().width/4, table->getContentSize().height/2);
    
    
    int j =0;
    for(Math_Object* bg_obj: dis_slots){
        bg_obj->setPosition(_objects.at(display_level+j)->getPosition());
        addChildNode(bg_obj,3);
        bg_obj->onShow(0);
        j++;
    }
    
    int i = (int)_objects.size()-1;
    for(Math_Slot* slot : _slots){
        slot->setPosition(_objects.at(i)->getPosition());
        addChildNode(slot,3);
        slot->onShow(0);
        i--;
    }
    
    math::func::setPositionList(dragers, (int)dragers.size(), cocos2d::Rect(quiz_size.width*0.05, quiz_size.height*0, quiz_size.width*0.65, quiz_size.height*bar_h));
}

//--------------------------------------
//start Question
void BA_TableValueQuiz::startQuestion() {
    //Bat dau cau hoi
    
    for(Math_Object* obj: dragers){
        obj->enableDrag(_panel_bot);
    }
    enableControlQuiz();
  //  _answers[0]->onShowInParent(_panel_bot, 2);
}

void BA_TableValueQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    
    bool done = false;
    int count = 0;
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
            if(slot->getLinker()->getID() == number_obj.at(i)){
                check ++;
            }
        }
        if(check == _slots.size()){
            done = true;
        }
    }
    
    
    
   // _answers.at(0)->onAnimationClick(done);
    this->onExplanation(done);
}

void BA_TableValueQuiz::onExplanation(bool correct) {
    
    if (!correct){
        this->enableControlQuiz();
        //return wrong drager
        //&& !slot->isCorrect(math::comparision::number)
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed()) {
                slot->getLinker()->onHide(0);
                slot->removeLinker();
            }
        }
    }
    else {
        this->nextQuestion();
    }
}

void BA_TableValueQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    // //Math_Pool::getInstance()->resetAnswers();
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    general_object->onHide(0.2);
    
    float delay = 0;
    
    for(Math_Slot* slot: _slots){
        slot->onHide(delay);
        delay+= 0.1;
    }
    
    for(Math_Object* slot: dis_slots){
        slot->onHide(delay);
        delay+= 0.1;
    }
    
    for(Math_Object* obj: _objects){
        obj->onHide(delay);
        delay+= 0.1;
    }
    
    background->runAction(cocos2d::FadeOut::create(delay));
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void BA_TableValueQuiz::onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 position)
{
    disableControlQuiz(0.3);
    bool create = false;
    Math_Object* object=(Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 4);
    
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
            //object->backToStart();
            object->setPosition(cocos2d::Vec2(0, -1000));
            object->onHide(0);
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->setPosition(cocos2d::Vec2(0, -1000));
                linker->onHide(0);
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            create = true;
        } else {
            //move start
            object->backToStart();
        }
    }
    
    if(create){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(object->getID(), math::common::move, object->getImageSource(), drag_bg_img);
        obj->setImageVisible(false);
        obj->setValueTextByKey("number");
        obj->setObjectSize(object_size);
        obj->showValueTextWithAnimation(false);
        obj->getLabelValue()->setFontSize(object_size.height*0.8);
        obj->setPosition(object->getStartPosition());
        obj->setMoveCallback(nullptr, CC_CALLBACK_2(BA_TableValueQuiz::onMoveObjectEnded, this));
        obj->onShow(0);
        addChildNodeToBottomPanel(obj);
        obj->enableDrag(_panel_bot);
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_TableValueQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_TableValueQuiz::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    autoTesting();
}

