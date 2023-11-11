//
//  BA_PictoGraphQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/23/17.
//
//logic chua ok
//khong che so loai 2-3-4 loai
//
#include "BA_PictoGraphQuiz.hpp"

BA_PictoGraphQuiz* BA_PictoGraphQuiz::createQuiz()
{
    BA_PictoGraphQuiz* quiz=new BA_PictoGraphQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_PictoGraphQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_PictoGraphQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_kind = randomRangeJson(json_game, "number_kind");
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    object_size = getSizeJson(json_object, "size");
    
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = dislay_json.FindMember("type")->value.GetString();
    max_in_line = dislay_json.FindMember("max_line")->value.GetInt();
    
    //random kind and number of each kind
    int total_number=0;
    while (number_list.size()< number_kind) {
        int number = cocos2d::random(number_object_min, number_object_max);
        total_number+= number;
        number_list.push_back(number);
    }
    
    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    int chart_cap = json_chart.FindMember("chart_cap")->value.GetInt();
    auto item_size = getSizeJson(json_chart, "item_size");
    title_chart = json_chart.FindMember("chart_title")->value.GetBool();
    chart_type =  json_chart.FindMember("chart_type")->value.GetString();
    vertical =  json_chart.FindMember("vertical")->value.GetBool();
    item_title = json_chart.FindMember("item_title")->value.GetBool();
    item_icon = json_chart.FindMember("icon_item")->value.GetBool();
    title_size = json_chart.FindMember("title_size")->value.GetDouble();
    chart_title = json_chart.FindMember("name")->value.FindMember("chart_title")->value.GetString();
    chart_icon_title = json_chart.FindMember("name")->value.FindMember("chart_icon_title")->value.GetString();
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    std::string iconsame = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    kind_list = getImageList("icons", number_kind, icon_filter, iconsame);
    
    //tao list img voi loai icon va so luong moi loai
    for(int i = 0 ; i < number_kind; i++){
        for(int j = 0 ; j < number_list[i]; j++){
            img_list.push_back(kind_list[i]);
        }
    }
    
    //PARSING QUESTION
    parseQuestion();
    auto img_number_list = getNumbers(0, number_kind);
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, kind_list);
        }
    }
    
    //----------------------------------
    //Gen objects
    for(int i = 0 ; i < img_list.size(); i++) {
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(i, math::common::deny, img_list[i], NULL);
        object->setObjectSize(object_size);
        _objects.push_back(object);
    }
    math::func::shuffle(_objects);
    
    //---------------------------------
    //group background
    math::resource::Image* group_bg = groupBG();
   
    //group_type
    cocos2d::Size gr_sz;
    float padding = 80/3;
    
    math::group::GroupType gr_type = math::group::custom;
    if(display_type.compare("line") == 0){
        gr_type = math::group::queue;
        gr_sz = cocos2d::Size((object_size.width+ padding) * max_in_line, object_size.height * (total_number/max_in_line + 1));

    }else{
        gr_sz = cocos2d::Size(800, 270);
    }
    
    gr_cap = total_number ;
//    if (vertical) {
//        gr_sz = cocos2d::Size(430, 500);
//    }
    //--------------------
    //Gen group
    Math_Group* group = Math_Pool::getInstance()->getGroup();
    group->setEnable(0, _objects, gr_type, gr_cap);
    group->setObjectSize(object_size);
    if(gr_type == math::group::queue){
        group->setGroupQueue(gr_sz);
        group->setBackground(group_bg);
    }else{
        group->setBackground(group_bg);
         group->setGroupSize(gr_sz);
    }
    // group->setBackgroundVisiable(true);
    _groups.push_back(group);
    
    //--------------------
    //Gen graph
//    auto graph_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"));
//    auto title_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"));
//    auto icon_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));
    std::vector<math::GraphBase*> graphs;
    graph = math::PictoGraph::createGraph();
    
    if(number_kind < 3){
        bar_h = 250.0/quiz_size.height;
    }else{
        if(item_size.width <= 60){
            bar_h = 250.0/quiz_size.height;
        }else{
            bar_h = 300.0/quiz_size.height;
        }
    }
    
    for(int i = 0; i < number_kind; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(i, math::common::touch, kind_list[i], NULL);
        item_list.push_back(obj);
        graph->addItem(obj, 0);
    }
    
    graph->setGraphCap(chart_cap);
    graph->setItemSize(item_size);
    graph->showDataAsIcon(item_icon);
    graph->setChartTitleSize(title_size);
    graph->showItemTitleAsIcon(item_title);
    graph->setTitleChar(title_chart);
    if(title_chart){
        graph->setTitleNumber(chart_icon_title);
        graph->setTitleItem(chart_title);
        
    }
//    graph->setTitleBackground(title_bg);
//    graph->setIconBackground(icon_bg);
    graph->setDirectionVertical(vertical);
    cocos2d::Vec2 center = cocos2d::Vec2((quiz_size.width - 250)/2, (quiz_size.height* bar_h)/2);
    graph->setPosition(center);
//    graph->setChartBackground(graph_bg);
    addChildNodeToBottomPanel (graph,2);
    graph->math::PictoGraph::reload();
    graph->enableTouchAll();
    graphs.push_back(graph);
    
    //---------------------
    //Gen button done
//    Math_Answer* btn=Math_Pool::getInstance()->getAnswer();
//    btn->setEnable("Done", math::common::done, CC_CALLBACK_1(BA_PictoGraphQuiz::onAnswerDone, this), this->getAnswerSize(1));
//    addChildNodeToBottomPanel(btn,2);
//    _answers.push_back(btn);

    
//     math::func::setPositionList(graphs, (int)graphs.size(), cocos2d::Rect(quiz_size.width*0.02, quiz_size.height*0.0, quiz_size.width*0.7, 250.0));
}

void BA_PictoGraphQuiz::onStart() {
    
    //----------------------------------
    // set position lists
//    if (!vertical) {
//        math::func::setPositionList(_groups, (int)_groups.size(), cocos2d::Rect(quiz_size.width* 0.05, quiz_size.height* 0.15, quiz_size.width* 0.9,quiz_size.height* 0.25));
//    } else {
//        math::func::setPositionList(_groups, (int)_groups.size(), cocos2d::Rect(quiz_size.width*0.05,quiz_size.height* 0.15,quiz_size.width* 0.9,quiz_size.height* 0.25));
//    }
    
    //---------------------------------
    //Gen question
    onShowBottomBar(0, bar_h, false, CC_CALLBACK_1(BA_PictoGraphQuiz::onAnswerDone, this));
    _groups.front()->setPosition(cocos2d::Vec2(quiz_size.width/2, (quiz_size.height*(1-bar_h)- getQuestion()->getContentSize().height)/2));
    
    for(Math_Group* gr: _groups){
        addChildNode(gr,2);
        gr->onShow();
    }
    showQuestionBox(0);
}

//--------------------------------------
//start Question
void BA_PictoGraphQuiz::startQuestion() {
    //Bat dau cau hoi
    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();
    }
   //
  
}

void BA_PictoGraphQuiz::onAnswerDone(bool correct){
    disableControlQuiz();
    
    bool done = false;
    
    int count = 0;
    std::vector<int> indexes;
    for(int i = 0; i < graph->getNumberItems(); i++){
        int value = graph->getGraphItems().at(i).value;
        if(value == number_list.at(i)){
            count++;
        }else{
            indexes.push_back(i);
        }
    }
    
    for(int i = 0; i < indexes.size(); i++){
        graph->changeValueForItem(indexes.at(i), 0);
        graph->updateItem(indexes.at(i));
    }
    
    if(count == graph->getNumberItems() ){
        done = true;
    }
    
    this->onExplanation(done);
    
}


void BA_PictoGraphQuiz::onExplanation(bool correct) {
    
    if (!correct){
        this->enableControlQuiz();
    }
    else {
        this->nextQuestion();
    }
    
}

void BA_PictoGraphQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    // //Math_Pool::getInstance()->resetAnswers();
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    for(Math_Object* obj: _objects){
        obj->onHide(delay);
        delay+= 0.1;
    }
    scheduleOnce([=](float dt) {
        for (Math_Group* group : _groups) {
            group->setDisable();
        }
    }, delay, "hide_groups");

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_PictoGraphQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_PictoGraphQuiz::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    autoTesting();
}

