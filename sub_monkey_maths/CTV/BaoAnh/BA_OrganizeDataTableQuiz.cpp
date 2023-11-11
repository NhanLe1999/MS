//
//  BA_OrganizeDataTableQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/5/17.
//

#include "BA_OrganizeDataTableQuiz.hpp"

BA_OrganizeDataTableQuiz* BA_OrganizeDataTableQuiz::createQuiz()
{
    BA_OrganizeDataTableQuiz* quiz=new BA_OrganizeDataTableQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_OrganizeDataTableQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_OrganizeDataTableQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    rapidjson::Value& json_kind = json_game.FindMember("kind")->value;
    kind_max = json_kind.FindMember("maximum")->value.GetInt();
    kind_min = json_kind.FindMember("minimum")->value.GetInt();
    chart_kind = json_game.FindMember("chart_kind")->value.GetString();
    answer_checked = json_game.FindMember("answer_checked")->value.GetBool();
    
    number_kind = cocos2d::random(kind_min, kind_max);
    
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = dislay_json.FindMember("type")->value.GetString();
    max_in_line = dislay_json.FindMember("max_line")->value.GetInt();
    
    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    title_chart = json_chart.FindMember("chart_title")->value.GetBool();
    chart_type =  json_chart.FindMember("chart_type")->value.GetString();
    vertical =  json_chart.FindMember("vertical")->value.GetBool();
    item_title = json_chart.FindMember("item_title")->value.GetBool();
    item_icon = json_chart.FindMember("icon_item")->value.GetBool();
    chart_cap = json_chart.FindMember("chart_cap")->value.GetInt();
    same_no = json_chart.FindMember("different_number")->value.GetBool();
    title_size = json_chart.FindMember("title_size")->value.GetDouble();
    
    rapidjson::Value& chart_names=json_chart.FindMember("name")->value;
    std::string chart_title = chart_names.FindMember("chart_title")->value.GetString() ;
    std::string chart_icon_title = chart_names.FindMember("chart_icon_title")->value.GetString();
    
    //random kind and number of each kind
    
    while (number_list.size()< number_kind) {
        int number = cocos2d::random(number_object_min, number_object_max);
        number_list.push_back(number);
        number_object += number;
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    rapidjson::Value& json_icon =json_data.FindMember("icon")->value;
    std::string icon_filter = json_icon.FindMember("icon_choice")->value.GetString();
    std::vector<math::resource::Image*> kind_list = getImageList("icons", number_kind, icon_filter);

    //tao list img voi loai icon va so luong moi loai
    for(int i = 0 ; i < number_kind; i++){
        for(int j = 0 ; j < number_list[i]; j++){
            img_list.push_back(kind_list[i]);
        }
    }
    
    if(number_kind >2){
        vertical = true;
    }
    //----------------------------------
    //Gen objects
    int obj_id=0;
    for(int i = 0 ; i < img_list.size(); i++){
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(obj_id, math::common::deny, img_list[i], NULL);
        object->setObjectSize(object_size);
        object->setComparisionValue();
        object->onShow(0);
        _objects.push_back(object);

        obj_id++;
    }

    bar_w = 220.0/quiz_size.width;
    //---------------------------------
    //group background
    auto img_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"));

    //group_type
    int gr_cap = number_object ;
    cocos2d::Size gr_sz;
    float padding = 70/3;
    
    math::group::GroupType gr_type = math::group::custom;
    if(display_type.compare("line") == 0){
        gr_type = math::group::queue;
        gr_sz = cocos2d::Size((object_size.width+ padding) * max_in_line, object_size.height * (number_object/max_in_line));
        
    }else{
        gr_sz = cocos2d::Size(700, 200);
    }
//    int group_column = max_in_line;
//    int group_row = (int)_objects.size()/max_in_line;
//
//    float padding = object_size.width *0.8 > 60 ? 60 : object_size.width *0.8;
//    cocos2d::Size gr_sz = cocos2d::Size(object_size.width *1.2 *group_column + padding, 100);
//    cocos2d::Size gr_sz (700, 200);
//
//    if(display_type.compare("line") == 0){
//        gr_type = math::group::queue;
//    }else{
//        gr_type = math::group::custom;
//    }
   
    
    //--------------------
    //Gen group
    Math_Group* group = Math_Pool::getInstance()->getGroup();
    group->setEnable(0, _objects, gr_type, gr_cap);
    group->setObjectSize(object_size);
    
    if(gr_type == math::group::queue){
        
        group->setGroupQueue(gr_sz);
        group->setBackground(img_bg);
    }else{
        group->setBackground(img_bg);
        group->setGroupSize(gr_sz);
    }
    _groups.push_back(group);

    
    auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    //--------------------
    //Gen graph
    graph_items_sz = object_size*1.5;
    table_graph = math::Table::createGraph();
    for(int i = 0; i < number_kind; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(i, math::common::touch, kind_list[i], slot_bg);
        table_graph->addItem(obj, number_list[i]);
    }
    
    table_graph->setGraphCap(chart_cap);
    table_graph->setItemSize(graph_items_sz);
    
    table_graph->showItemTitleAsIcon(item_title);
    table_graph->setTitleChar(title_chart);
    if(title_chart){
        table_graph->setTitleNumber(chart_icon_title);
        table_graph->setTitleItem(chart_title);
    }
    table_graph->setChartBackground(math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png")));
    table_graph->setDirectionVertical(vertical);
    if(vertical){
//        table_graph->setPosition(cocos2d::Vec2(group->getPosition().x, quiz_size.height*0.28));
        title_size = 1;
    }else{
//        table_graph->setPosition(cocos2d::Vec2(group->getPosition().x, quiz_size.height*0.3));
    }
    table_graph->setChartTitleSize(title_size);
    
    addChildNode(table_graph,2);
    table_graph->math::Table::reload();
    table_graph->setVisible(false);
    table_list.push_back(table_graph);
    
    
    //PARSING QUESTION
    math::func::shuffle(img_list);
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    for(int i = 0; i < kind_list.size(); i++){
        if(kind_list.at(i) == img_list.at(0)){
            k_index = i;
        }
    }
    
    //--------------------
    //Gen slot
    for(int i = 0; i < number_kind; i++){
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i, slot_bg);
        slot->setSlotSize(object_size);
        _slots.push_back(slot);
    }
   
    
    //--------------------
    //Gen dragers
    drag_bg = objectBG();
    number_img_list = getNumbers(number_object_min , number_object_max);
    
    for(int i = number_object_min; i < number_object_max+1; i++){
        math::resource::Image* drag_img;
        for (math::resource::Image* img : number_img_list){
            if(i == img->getComparisionValue("number")){
                drag_img = img;
            }
        }
        
        Math_Object* drager = Math_Pool::getInstance()->getObject();
        drager->setEnable(i, math::common::deny, drag_img, drag_bg);
        drager->setImageVisible(false);
        drager->setObjectSize(object_size);
        drager->setValueTextByKey("number");
        drager->showValueTextWithAnimation(false);
        drager->getLabelValue()->setFontSize(object_size.height*0.8);
        drager->setMoveCallback(nullptr, CC_CALLBACK_2(BA_OrganizeDataTableQuiz::onMoveObjectEnded, this));
        addChildNodeToRightPanel(drager,3);
        dragers.push_back(drager);
    }
    math::func::setPositionList(dragers, 1, cocos2d::Rect(quiz_size.width*0, quiz_size.height*0.25, quiz_size.width*bar_w, quiz_size.height*0.7));

}


void BA_OrganizeDataTableQuiz::onStart() {
    for(Math_Object* dr: dragers){
        dr->onShow(0);
    }
     onShowRightBar(0, bar_w, CC_CALLBACK_1(BA_OrganizeDataTableQuiz::onAnswerDone,this));
    showQuestionBox(0);
    
    auto gr = _groups.at(0);
    if(vertical){
        gr->setPosition(cocos2d::Vec2( quiz_size.width*(1-bar_w)/2, quiz_size.height-getQuestion()->getContentSize().height-gr->getContentSize().height/2 -75));
    }else{
        gr->setPosition(cocos2d::Vec2( quiz_size.width*(1-bar_w)/2, quiz_size.height-getQuestion()->getContentSize().height-gr->getContentSize().height/2 -150));

    }
 
    addChildNode(gr);
    gr->onShow();
    
    auto label_img = math::resource::ResHelper::createImage(getUIPath("common/frame/red2.png"));
    gr_label = cocos2d::ui::ImageView::create(label_img->path);
    gr_label->setContentSize(cocos2d::Size(gr->getContentSize().width+10, 55));
    gr_label->setScale9Enabled(true);
    gr_label->setPosition(cocos2d::Vec2(quiz_size.width*(1-bar_w)/2, gr->getPosition().y + gr->getContentSize().height*0.58));
    label_text = cocos2d::ui::Text::create("Items in the box", m_text_font, 40);
    label_text->setTextColor(cocos2d::Color4B::WHITE);
    label_text->setPosition(gr_label->getPosition());

    addChildNode(gr_label,2);
    addChildNode(label_text,2);

    table_graph->setPosition(cocos2d::Vec2(quiz_size.width*(1-bar_w)/2,( gr->getPosition().y - gr->getContentSize().height/2)/2));
    table_graph->setVisible(true);
   
    int i =0;
    for(Math_Slot* slot: _slots){
        if(vertical){
            //chieu doc
            slot->setPosition(cocos2d::Vec2(table_graph->getPosition().x + graph_items_sz.width, table_graph->getPosition().y - table_graph->getContentSize().height/2+ graph_items_sz.height/2+ graph_items_sz.height*(i) ));
        }else {
            //chieu ngang
            slot->setPosition(cocos2d::Vec2(table_graph->getPosition().x - table_graph->getContentSize().width/2+ graph_items_sz.width*title_size/2+graph_items_sz.width*title_size*(i+1), table_graph->getPosition().y - + graph_items_sz.height/2));
        }
        addChildNode(slot,3);
        slot->onShow(0);
        i++;
    }
    if(answer_checked){
        for(int i = 0; i < number_kind; i++){
            if(i != k_index){
                _slots.at(i)->setVisible(false);
                _slots.at(i)->onHide(0);

            }
        }
    }

}

//--------------------------------------
//start Question
void BA_OrganizeDataTableQuiz::startQuestion() {
    //Bat dau cau hoi
    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();
    }
    
    for(Math_Object* obj: dragers){
        obj->enableDrag(_panel_right);
    }
    
}

void BA_OrganizeDataTableQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    
    bool done = false;
    int count = 0;
    
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed() || !slot->isVisible()){
            count++;
        }
    }
    
    if(count != _slots.size()){
        done = false;
        
    }else{
        int check = 0;
        for(int i = 0 ; i < _slots.size(); i++){
            Math_Slot* slot = _slots.at(i);
            if(!slot->isVisible()){
                check++;
            }
            else{
                if(slot->getLinker()->getID() == number_list.at(i)){
                    check ++;
                }
            }
            
        }
        if(check == _slots.size()){
            done = true;
        }
    }
    if(done == false){
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed()){
                slot->getLinker()->setDisable();
                slot->removeLinker();
            }
        }
    }
    
    this->onExplanation(done);
   // _answers.at(0)->onAnimationClick(done);
}

void BA_OrganizeDataTableQuiz::onExplanation(bool correct) {
    
    if (!correct){
        this->enableControlQuiz();
        
    }
    else {
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.1), cocos2d::CallFunc::create([=](){
            this->nextQuestion();
        }), NULL));
    }
    
}

void BA_OrganizeDataTableQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = 0.1;
    
    for(Math_Object* obj: _objects){
        obj->onHide(delay);
        delay+= 0.1;
    }
    scheduleOnce([=](float dt) {
        _groups.at(0)->setDisable();
    }, delay, "disable_group");
    
    gr_label->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::FadeOut::create(delay), NULL) );
    label_text->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::FadeOut::create(delay), NULL));
    
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void BA_OrganizeDataTableQuiz::onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 position)
{
    disableControlQuiz();
    bool create = false;
    
    Math_Object* object=(Math_Object*)node;
   
    
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
            //object->backToStart();
            object->setDisable();
            //create = true;
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                //linker->backToLast(2);
                linker->setDisable();
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            
        } else {
            //move start
            object->backToStart();
        }
    }
    
    if(create){
        
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(object->getID(), math::common::move, object->getImageSource(), drag_bg);
        obj->setImageVisible(false);
        obj->setValueTextByKey("number");
        obj->setObjectSize(object_size);
        obj->showValueTextWithAnimation(false);
        obj->getLabelValue()->setFontSize(object_size.height*0.75);
       // obj->getLabelValue()->setTextColor(cocos2d::Color4B(255,0,0, 255));
        obj->setPosition(_panel_right->convertToNodeSpace(object->getStartPosition()));
        obj->setMoveCallback(nullptr, CC_CALLBACK_2(BA_OrganizeDataTableQuiz::onMoveObjectEnded, this));
        
        addChildNodeToRightPanel(obj,3);
        obj->onShow(0);
        obj->enableDrag(_panel_right);
    }
    
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
        enableControlQuiz();
    }), nullptr));
    
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_OrganizeDataTableQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_OrganizeDataTableQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}

