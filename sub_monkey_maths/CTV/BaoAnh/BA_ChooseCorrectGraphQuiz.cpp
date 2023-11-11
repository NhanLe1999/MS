//
//  BA_ChooseCorrectGraphQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/3/17.
//

#include "BA_ChooseCorrectGraphQuiz.hpp"
BA_ChooseCorrectGraphQuiz* BA_ChooseCorrectGraphQuiz::createQuiz()
{
    BA_ChooseCorrectGraphQuiz* quiz=new BA_ChooseCorrectGraphQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_ChooseCorrectGraphQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_ChooseCorrectGraphQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_kind = getJsonInt(json_game, "kind", 3);
    chart_kind = getJsonString(json_game, "chart_kind", "none");
    
    rapidjson::Value& total_object = json_game.FindMember("total_object")->value;
    number_total_max = getJsonInt(total_object, "maximum", 3);
    number_total_min = getJsonInt(total_object, "minimum", 3);
    
    //compare_key = json_game.FindMember("compare_key")->value.GetString();
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 5);
    number_object_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 1);
    
    object_size = getSizeJson(json_object, "size");
    
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = getJsonString(dislay_json, "type", "line");
    max_in_line = getJsonInt(dislay_json, "max_line", 5);
    
    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    title_chart = getJsonBool(json_chart, "chart_title");
    chart_type = getJsonString(json_chart, "chart_type", "picto");
    vertical = getJsonBool(json_chart, "vertical");
    item_title = getJsonBool(json_chart, "item_title");
    item_icon = getJsonBool(json_chart, "icon_item");
    item_size = getSizeJson(json_chart, "item_size");
    chart_cap = getJsonInt(json_chart, "chart_cap", 5);
    title_size = getJsonInt(json_chart, "title_size", 2);
    same_no = getJsonBool(json_chart, "different_number", true);
    
    rapidjson::Value& chart_names=json_chart.FindMember("name")->value;
    std::string chart_title = getJsonString(chart_names, "chart_title", "");
    std::string chart_icon_title = getJsonString(chart_names, "chart_icon_title", "");
    
    //random kind and number of each kind
    int total_number;
    if(number_kind == 0){
        total_number = cocos2d::random(number_total_min, number_total_min);
        number_object = 0;
        while (number_object < total_number){
            int number = cocos2d::random(number_object_min, number_object_max);
            if(same_no == true){
                if(sameNumber(number_list, number) == true){
                    number_list.push_back(number);
                    number_object += number;
                }
            }else{
                number_list.push_back(number);
                number_object += number;
            }
            
        }
        number_kind = (int) number_list.size();
    }else{
        while (number_list.size()< number_kind) {
            int number = cocos2d::random(number_object_min, number_object_max);
            if(same_no == true){
                if(sameNumber(number_list, number) == true){
                    number_list.push_back(number);
                    number_object += number;
                }
            }else{
                number_list.push_back(number);
                number_object += number;
            }
            
        }
        total_number = number_object;
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    rapidjson::Value& json_icon =json_data.FindMember("icon")->value;
    std::string icon_same = getJsonString(json_icon, "same", "none");
    std::string icon_diff = getJsonString(json_icon, "different", "id");
    
    kind_list = getImageList("icons", number_kind, icon_diff, icon_same);
    
    //tao list img voi loai icon va so luong moi loai
    for(int i = 0 ; i < number_kind; i++){
        for(int j = 0 ; j < number_list[i]; j++){
            img_list.push_back(kind_list[i]);
        }
    }
    math::func::shuffle(img_list);
    
    //----------------------------------
    //Gen objects
    for(int i = 0 ; i < img_list.size(); i++){
        Math_Object* object = mpool->getObject();
        object->setEnable(i, math::common::deny, img_list[i], NULL);
        object->setObjectSize(object_size);
        object->setComparisionValue();
        _objects.push_back(object);
    }
    
    //set bar_h
    if(number_kind < 3){
        bar_h = 250.0/quiz_size.height;
    }else{
        if(item_size.width <= 60){
            bar_h = 250.0/quiz_size.height;
        }else{
            bar_h = 300.0/quiz_size.height;
        }
    }
    //---------------------------------
    //group background
    math::resource::Image* group_bg = groupBG();
    
    cocos2d::Size gr_sz;
    float padding = 80/3;
    
    //group_type
    math::group::GroupType gr_type = math::group::custom;
    if(display_type.compare("line") == 0){
        gr_type = math::group::queue;
        gr_sz = cocos2d::Size((object_size.width+ padding) * max_in_line, object_size.height * (total_number/max_in_line + 1));
        
    }else{
        gr_sz = cocos2d::Size(800, 270);
    }
    
    gr_cap = total_number ;
 
    //--------------------
    //Gen group
    Math_Group* group = mpool->getGroup();
    group->setEnable(0, _objects, gr_type, gr_cap);
    group->setObjectSize(object_size);
    
    if(gr_type == math::group::queue){
        group->setGroupQueue(gr_sz);
        group->setBackground(group_bg, 0.3);
    }else{
        group->setGroupSize(gr_sz);
        group->setBackground(group_bg, 0.3);
    }
    _groups.push_back(group);
    
    //--------------------
    //Gen graph
    int chart_kindy;
    if(chart_kind == "none"){
        chart_kindy = number_kind;
    }else{
        chart_kindy = cocos2d::random(2, 3);
    }
    
    cocos2d::Size answer_size;
    for(int j = 0; j < 2; j++){
        if(j != 0){
            number_list.push_back(number_list.at(0));
            number_list.erase(number_list.begin());
        }
        
        if(chart_type == "picto"){
            graph = math::PictoGraph::createGraph();
            for(int i = 0; i < chart_kindy; i++){
                Math_Object* obj = mpool->getObject();
                obj->setEnable(i, math::common::deny, kind_list[i], NULL);
                graph->addItem(obj, number_list.at(i));
            }
            
            graph->setGraphCap(chart_cap);
            graph->setItemSize(item_size);
            graph->showDataAsIcon(item_icon);
            graph->showItemTitleAsIcon(item_title);
            graph->setChartTitleSize(title_size);
            graph->setTitleChar(title_chart);
            if(title_chart){
                graph->setTitleNumber(chart_icon_title);
                graph->setTitleItem(chart_title);
            }
            graph->setDirectionVertical(vertical);
            graph->math::PictoGraph::reload();
            graph_list.push_back(graph);
            answer_size = graph->getContentSize();
            graph->disableTouchAll();
        }else if(chart_type == "tally"){
            tally_graph = math::TallyChart::createGraph();
            for(int i = 0; i < chart_kindy; i++){
                Math_Object* obj = mpool->getObject();
                obj->setEnable(i, math::common::deny, kind_list[i], NULL);
                tally_graph->addItem(obj, number_list[i]);
                
            }
            
            tally_graph->setGraphCap(1);
            tally_graph->setItemSize(item_size);
            tally_graph->showItemTitleAsIcon(item_title);
            tally_graph->setTitleChar(title_chart);
            if(title_chart){
                tally_graph->setTitleNumber(chart_icon_title);
                tally_graph->setTitleItem(chart_title);
            }
            tally_graph->setDirectionVertical(vertical);
            tally_graph->math::TallyChart::reload();
            
            graph_list.push_back(tally_graph);
            answer_size = tally_graph->getContentSize();
        }else{
            
            table_graph = math::Table::createGraph();
            for(int i = 0; i < chart_kindy; i++){
                Math_Object* obj = mpool->getObject();
                obj->setEnable(i, math::common::deny, kind_list[i], NULL);
                table_graph->addItem(obj, number_list[i]);
            }
            
            table_graph->setGraphCap(1);
            table_graph->setItemSize(item_size);
            table_graph->setTitleChar(title_chart);
            if(title_chart){
                table_graph->setTitleItem(chart_title);
                table_graph->setTitleNumber(chart_icon_title);
                table_graph->setChartTitleSize(title_size);
            }
            table_graph->showItemTitleAsIcon(item_title);
            table_graph->setDirectionVertical(vertical);
            table_graph->math::Table::reload();
            graph_list.push_back(table_graph);
            answer_size = table_graph->getContentSize();
        }
        
        Math_Answer* answ = mpool->getAnswer();
        answ->setEnable("", math::common::answer , CC_CALLBACK_1(BA_ChooseCorrectGraphQuiz::onTouchedAnswer, this), answer_size);
        if(j==0){
            answ->setCorrect(true);
        }
        
        graph_list[j]->setPosition(answer_size/2);
        answ->addChild(graph_list[j]);
        
        _answers.push_back(answ);
    }
    
    math::func::shuffle(_answers);
    
    //parsing question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, kind_list);
    }
}

void BA_ChooseCorrectGraphQuiz::onStart() {
    
    onShowBottomBar(0, 0.35, false);
    quizBegan(0, true);
    
    _groups.front()->setPosition(cocos2d::Vec2(quiz_size.width, quiz_size.height*0.65-getQuestion()->getContentSize().height-20)*0.5);
    addChildNode(_groups.front());
    _groups.front()->onShow();
    
    float delta = (quiz_size.width - 20 - _answers.front()->getContentSize().width * 2) / 3;
    if (delta>100) {
        delta = 100;
    }
    
    math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(10, 0, quiz_size.width-20, quiz_size.height*0.35), delta);
}

//--------------------------------------
//start Question
void BA_ChooseCorrectGraphQuiz::startQuestion() {
    enableControlQuiz();
    for (int i = 0; i<_answers.size(); i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
    }
}

void BA_ChooseCorrectGraphQuiz::onTouchedAnswer(cocos2d::Node *node){
    Math_Answer* answer = (Math_Answer*) node;
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}

void BA_ChooseCorrectGraphQuiz::onExplanation(bool correct) {
    
    if (!correct){
        disableControlQuiz(0.25);
    }
    else {
        disableControlQuiz();
        nextQuestion(0.5);
    }
    
}

void BA_ChooseCorrectGraphQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (int i = 0; i<_answers.size(); i++) {
        _answers[i]->setDisable(0.1*i, math::animation::SlideUp);
    }

    _groups.front()->onHide();
    quizEnded(0.5, true);
}

bool BA_ChooseCorrectGraphQuiz::sameNumber(std::vector<int> vector , int number){
    bool add = true;
    for(int i = 0 ; i < vector.size(); i++){
        if(number == vector.at(i)){
            add = false;
        }
    }
    return add;
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_ChooseCorrectGraphQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_ChooseCorrectGraphQuiz::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    autoTesting();
}

