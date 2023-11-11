//
//  BA_InterpretDataQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/2/17.
//

#include "BA_InterpretDataQuiz.hpp"

BA_InterpretDataQuiz* BA_InterpretDataQuiz::createQuiz()
{
    BA_InterpretDataQuiz* quiz=new BA_InterpretDataQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_InterpretDataQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_InterpretDataQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    rapidjson::Value& total_object = json_game.FindMember("obj_kind")->value;
    kind_max= total_object.FindMember("maximum")->value.GetInt();
    kind_min= total_object.FindMember("minimum")->value.GetInt();
    
    number_kind = cocos2d::random(kind_min , kind_max);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    
    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    title_chart = json_chart.FindMember("chart_title")->value.GetBool();
    chart_type =  json_chart.FindMember("chart_type")->value.GetString();
    vertical =  json_chart.FindMember("vertical")->value.GetBool();
    item_title = json_chart.FindMember("item_title")->value.GetBool();
    item_icon = json_chart.FindMember("icon_item")->value.GetBool();
    different = json_chart.FindMember("different_number")->value.GetBool();
    title_size = json_chart.FindMember("title_size")->value.GetInt();
    //json_chart.FindMember("different_number")->value.GetBool();
    chart_title = json_chart.FindMember("name")->value.FindMember("chart_title")->value.GetString();
    chart_icon_title = json_chart.FindMember("name")->value.FindMember("chart_icon_title")->value.GetString();
    
    
    number_object = 0;
    while (number_list.size()< number_kind) {
        int number = cocos2d::random(number_object_min, number_object_max);
        if(different == true){
            if(sameNumber(number_list, number) == true){
                number_list.push_back(number);
                number_object += number;
            }
        }else{
            number_list.push_back(number);
            number_object += number;
        }
    }
    
    //TODO: Chon icon
    std::vector<math::resource::Image*> kind_list = getImageList("icons", number_kind, "name","");
    
    //--------------------
    //Gen graph
//    auto graph_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));
//    auto title_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"));
   
    if(chart_type == "picto"){
        graph = math::PictoGraph::createGraph();
        for(int i = 0; i < number_kind; i++){
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::deny, kind_list[i], NULL);
            graph->addItem(obj, number_list [i]);
        }
        graph->setGraphCap(number_object_max);
        graph->setItemSize(object_size*1.2);
        graph->setChartTitleSize(title_size);
        graph->showDataAsIcon(item_icon);
        graph->showItemTitleAsIcon(item_title);
        graph->setDirectionVertical(vertical);
        graph->setTitleChar(title_chart);
        if(title_chart){
            graph->setTitleNumber(chart_icon_title);
            graph->setTitleItem(chart_title);
        }
//        graph->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*0.25));
//        graph->setChartBackground(graph_bg);
//        graph->setTitleBackground(title_bg);
        addChildNode(graph,2);
        graph->math::PictoGraph::reload();
        graph_base.push_back(graph);
    }
    else if(chart_type =="tally"){
        tally_graph = math::TallyChart::createGraph();
        
        for(int i = 0; i < number_kind; i++){
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::deny, kind_list[i], NULL);
            tally_graph->addItem(obj, number_list[i]);
        }
        tally_graph->setGraphCap(number_object_max);
        tally_graph->setItemSize(object_size*1.2);
        tally_graph->setChartTitleSize(title_size);
        //tally_graph->showDataAsIcon(item_icon);
        tally_graph->showItemTitleAsIcon(item_title);
        tally_graph->setDirectionVertical(vertical);
        tally_graph->setTitleChar(title_chart);
        if(title_chart){
            tally_graph->setTitleNumber(chart_icon_title);
            tally_graph->setTitleItem(chart_title);
        }
//        tally_graph->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*0.25));
//        tally_graph->setChartBackground(graph_bg);
//        tally_graph->setIconBackground(title_bg);
        addChildNode(tally_graph,2);
        tally_graph->math::TallyChart::reload();
        graph_base.push_back(tally_graph);
    }else{
        table = math::Table::createGraph();
        for(int i = 0; i < number_kind; i++){
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::deny, kind_list[i], NULL);
            table->addItem(obj, number_list[i]);
        }
        table->setGraphCap(1);
        table->setItemSize(object_size);
        table->setChartTitleSize(title_size);
        table->setDirectionVertical(vertical);
        table->showItemTitleAsIcon(item_title);
        table->setTitleChar(title_chart);
        if(title_chart){
            table->setTitleNumber(chart_icon_title);
            table->setTitleItem(chart_title);
        }
//        table->setChartBackground(graph_bg);
//        table->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_size.height*0.25));
        addChildNode(table);
        table->math::Table::reload();
        graph_base.push_back(table);
    }
    
    graph_items = graph_base.at(0)->getGraphItems();
    
    //sap xep tang dan graph_item list
    math::GraphBase::GraphItem temp;
    for(int i=0;i<graph_items.size()-1;i++)
    {
        for(int j=i+1;j<graph_items.size();j++)
        {
            if(graph_items[i].value > graph_items[j].value)
            {
                temp = graph_items[i];
                graph_items[i]=graph_items[j];
                graph_items[j]=temp;
                
            }
        }
    }
    
    std::vector<int> quantity_list;
    int a = 0;
    std::vector<math::resource::Image*> img_numbers;
    while(quantity_list.size()<number_kind){
        quantity_list.push_back(graph_items.at(a).value);
        img_numbers.push_back(math::number::getNumber( graph_items.at(a).value ));
        a++;
    }
    
    math::func::shuffle(kind_list);
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    std::vector<int> opt_index;


    for (std::string key : opt_keys) {
        
        if (key == "number") {
//            if(cocos2d::random(0, 1) == 1){
//                not_change = false;
//                math::func::shuffle(kind_list);
//            }
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_numbers) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
//            CCLOG("text: %s", texts.at(0).text.c_str());
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_numbers) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
          
            getQuestion()->setOptionalTexts(key, kind_list);
        }
    }
    
    
        for(int j = 0; j < kind_list.size(); j++){
            for(int i = 0; i < graph_items.size(); i++){
                if(graph_items.at(i).object->getImageSource() == kind_list.at(j)){
                    opt_index.push_back(i);
                    CCLOG("index: %i", i);
                }
            }
        }
    
    
    
    int keyword_index = cocos2d::random(0, 1);
   // std::string keyword_str;
    if(keyword_index == 0){
        this->getQuestion()->setKeywordValue(0);
    }else{
        this->getQuestion()->setKeywordValue(1);
    }
    
//    std::vector<int> k_index;
//    k_index.push_back(k)
    
    //create answer list
    int q_number = getQuestion()->getNumberQuestions() ;
    for(int i = 0; i < q_number; i++){

        int add = 0;
        if(i == 0){
            if(keyword_index == 0){
                //the most
                int ind_1 = opt_index.at(0);
                if(ind_1 == graph_items.size()-1){
                    add = 1;
                }
            }else{
                //the less
                    int ind_1 = opt_index.at(0);
                    if(ind_1 == 0){
                        add = 1;
                    }
                }
        }else if(i ==1){
            if(keyword_index == 0){
                //more than
                int ind_1 = opt_index.at(1);
                int ind_2 = opt_index.at(2);
                CCLOG("ind_1: %i, ind_2: %i", ind_1, ind_2);
                if(ind_1 > ind_2){
                    add = 1;
                }

            }else{
                //less than
                int ind_1 = opt_index.at(1);;
                int ind_2 = opt_index.at(2);;
                CCLOG("ind_1: %i, ind_2: %i", ind_1, ind_2);
                if(ind_1 < ind_2){
                    add = 1;
                }
            }
        }
//        else if(i == 2){
////            int ind_1 = opt_index.at(0);
////            int ind_2 = opt_index.at(0);
//            if(not_change){
//                add = 1;
//            }
//
//           // kind_list.at(0);
//
//            //int quantity = img_numbers.at(0)->getComparisionValue("number");
//            //graph_items.at(opt_index.at(1)).value;
//           // int value = graph_items.at(ind_1).value;
////                        if(ind_2 == ind_1){
////                            add = 1;
////                        }
//
////            if(value == graph_items.at(ind_2).value ){
////                add = 1;
////            }
//        }else{
//            add = 1;
//        }
//        std::string q_text = getQuestionTexts();
//        int add = 0;
//        //keyword
//        if(q_text.find("least") != std::string::npos){
//
//            int ind_1 = opt_index.at(0);
//            if(ind_1 == 0){
//                add = 1;
//            }
//        }else if(q_text.find("most") != std::string::npos){
//            int ind_1 = opt_index.at(0);
//            if(ind_1 == graph_items.size()-1){
//                add = 1;
//            }
//        }else if(q_text.find("less") != std::string::npos){
//            int ind_1 = opt_index.at(0);;
//            int ind_2 = opt_index.at(1);;
//            CCLOG("ind_1: %i, ind_2: %i", ind_1, ind_2);
//            if(ind_1 < ind_2){
//                add = 1;
//            }
//        }else if(q_text.find("more") != std::string::npos){
//            int ind_1 = opt_index.at(0);;
//            int ind_2 = opt_index.at(1);;
//            CCLOG("ind_1: %i, ind_2: %i", ind_1, ind_2);
//            if(ind_2 < ind_1){
//                add = 1;
//            }
//        }
        right_answer.push_back(add);
    }
}


void BA_InterpretDataQuiz::onStart() {
    CCLOG("%s override", __func__);
    
    //---------------------------------
    //Gen question
//    Math_Question* question=Math_Pool::getInstance()->getQuestion();
//    question->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
//    question->setPosition(cocos2d::Vec2(0, 768));
//    for (math::common::QuestionData* data : list_questiondata) {
//        question->pushData(data);
//    }
//    question->setEnable([=](){
//        this->speakQuestion();
//        this->startQuestion();
//    });
//    this->setQuestion(question);

 //   getQuestion()->onStart();
    bar_h = 200.0/quiz_size.height;
    onShowBottomBar(0, bar_h, false);
    graph_base.front()->setPosition(cocos2d::Vec2(quiz_size.width/2, (quiz_size.height*(1-bar_h)-getQuestion()->getContentSize().height)/2 - 40));
    showQuestionBox(0);

}


//--------------------------------------
//start Question
void BA_InterpretDataQuiz::startQuestion() {
    CCLOG("%s override", __func__);
    //Bat dau cau hoi
    
    enableControlQuiz();
    clickable = true;
    answers.clear();
    Math_Answer* yes_btn = Math_Pool::getInstance()->getAnswer();
    yes_btn->setEnable("Yes", math::common::done , CC_CALLBACK_1(BA_InterpretDataQuiz::onTouchedAnswer, this),this->getAnswerSize());
    yes_btn->setName(cocos2d::StringUtils::toString(1)) ;
    addChildNodeToBottomPanel(yes_btn);
    
    Math_Answer* no_btn = Math_Pool::getInstance()->getAnswer();
    no_btn->setEnable("No", math::common::done, CC_CALLBACK_1(BA_InterpretDataQuiz::onTouchedAnswer, this),this->getAnswerSize());
    no_btn->setName(cocos2d::StringUtils::toString(0));
    addChildNodeToBottomPanel(no_btn);
    
    answers.push_back(yes_btn);
    answers.push_back(no_btn);
    
    //----------------------------------
    // set position lists
    math::func::setPositionList(answers, 2, cocos2d::Rect(quiz_size.width* 0.1, quiz_size.height* 0.0, quiz_size.width*0.8, quiz_size.height* bar_h) );
    yes_btn->onShowInParent(_panel_bot, 3);
    no_btn->onShowInParent(_panel_bot, 3);
   
    
   
}

void BA_InterpretDataQuiz::onTouchedAnswer(cocos2d::Node *node){
    // disableControlQuiz();
    Math_Answer* ans = (Math_Answer*) node;
    bool done = false;
    int q_id = getQuestion()->getData().index;
    if(clickable){
        clickable = false;
        std::string name = ans->getName();
        if(atoi(ans->getName().c_str()) == right_answer.at(q_id)){
            done = true;
        }
        this->onExplanation(done);
        ans->onAnimationClick(done);
        //
    }
}

void BA_InterpretDataQuiz::onExplanation(bool correct) {
    
    if (!correct){
        enableControlQuiz();
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::CallFunc::create([=]() {
            clickable = true;
        }), NULL));
    }
    else {
        for (Math_Answer* ans: answers) {
            ans->setDisable();
        }
        disableControlQuiz();
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::CallFunc::create([=]() {
            
            this->nextQuestion();
        }), NULL));

    }
    
}

void BA_InterpretDataQuiz::onComplete() {
    CCLOG("%s override", __func__);
    
    //TODO: Thuc hien them cac lenh o day
    // Math_Pool::getInstance()->resetAnswers();
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    
    
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

bool BA_InterpretDataQuiz::sameNumber(std::vector<int> vector , int number){
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
void BA_InterpretDataQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void BA_InterpretDataQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}
