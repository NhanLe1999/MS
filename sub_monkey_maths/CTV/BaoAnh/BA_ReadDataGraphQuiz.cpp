
//
//  BA_ReadDataGraphQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/23/17.
//
//

#include "BA_ReadDataGraphQuiz.hpp"

BA_ReadDataGraphQuiz* BA_ReadDataGraphQuiz::createQuiz()
{
    BA_ReadDataGraphQuiz* quiz=new BA_ReadDataGraphQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_ReadDataGraphQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_ReadDataGraphQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    
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
    std::string same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    std::string diff = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::vector<math::resource::Image*> kind_list = getImageList("icons", number_kind+1, diff, same);
    
    auto back_img = kind_list.back();
    kind_list.pop_back();
    
    bar_height = 250.0/quiz_size.height;
    
    //--------------------
    //Gen graph
    auto graph_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"));
    auto title_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/brown1.png"));
    auto icon_bg = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));

   
    if(chart_type == "picto"){
        graph = math::PictoGraph::createGraph();
        for(int i = 0; i < number_kind; i++){
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(i, math::common::deny, kind_list[i], NULL);
            graph->addItem(obj, number_list [i]);
        }
        graph->setGraphCap(number_object_max);
        graph->setItemSize(object_size*1.2);
        graph->showDataAsIcon(item_icon);
        graph->showItemTitleAsIcon(item_title);
        graph->setTitleChar(title_chart);
        if(title_chart){
            graph->setTitleNumber(chart_icon_title);
            graph->setTitleItem(chart_title);
        }
        graph->setDirectionVertical(vertical);
        graph->setChartBackground(graph_bg);
        graph->setTitleBackground(title_bg);
        graph->setIconBackground(icon_bg);
       // graph->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height - getQuestion()->getContentSize().height - 250/quiz_size.height));
        
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
        //tally_graph->showDataAsIcon(item_icon);
        tally_graph->setTitleChar(title_chart);
        if(title_chart){
            tally_graph->setTitleNumber(chart_icon_title);
            tally_graph->setTitleItem(chart_title);
        }
        tally_graph->showItemTitleAsIcon(item_title);
        tally_graph->setDirectionVertical(vertical);
       // tally_graph->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height - getQuestion()->getContentSize().height - 250/quiz_size.height));
        tally_graph->setChartBackground(graph_bg);
        tally_graph->setIconBackground(title_bg);
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
        table->setDirectionVertical(vertical);
        table->showItemTitleAsIcon(item_title);
        table->setTitleChar(title_chart);
        if(title_chart){
            table->setTitleNumber(chart_icon_title);
            table->setTitleItem(chart_title);
        }
        table->setChartBackground(graph_bg);
       // table->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height - getQuestion()->getContentSize().height - 250/quiz_size.height));
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
    
    std::vector<math::resource::Image*> opt_imgs;
    std::vector<int> opts;
    parseQuestion();
    int keyword_index = cocos2d::random(0, 1);
    // std::string keyword_str;
    if(keyword_index == 0){
        this->getQuestion()->setKeywordValue(0);
    }else{
        this->getQuestion()->setKeywordValue(1);
    }
    
    auto opt_keys = getQuestion()->getOptionalKeys();
    
    if(getQuestion()->getNumberQuestions() >1 ){
        //nhieu question
        int index_1 = cocos2d::random(1, (int) kind_list.size()-2);
        int index_2 = index_1;
        while(index_1 == index_2){
            if(keyword_index == 1){
                index_2 = cocos2d::random(index_1, (int) kind_list.size()-1);
            }else{
                index_2 = cocos2d::random(0, index_1);
            }
        }
        opt_imgs.push_back(graph_items.at(index_1).object->getImageSource());
        opt_imgs.push_back(graph_items.at(index_2).object->getImageSource());
        opts.push_back(index_1);
        opts.push_back(index_2);
    }else{
        //1 question
        for(int i = 0; i < graph_items.size(); i++){
            if(kind_list.at(0) == graph_items.at(i).object->getImageSource()){
                opts.push_back(i);
            }
        }
        opt_imgs = kind_list;

    }
    
    auto img_number_list = getNumbers(graph_items.at(opts.back()).value, graph_items.at(opts.back()).value);
    
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
            getQuestion()->setOptionalTexts(key, opt_imgs);
        
        }
    }
    
    for(int i = 0 ; i < 2; i++){
        if(i == 0){
            //how many-answer
            for(int i1 = 0; i1< getQuestion()->getNumberQuestions(); i1++){
                AnswersList temp_list;
                temp_list.id = i;
                
                std::vector<int> ans_number;
                temp_list.q_id = i1;
                int right_number ;
                std::string right_text;
                if(i1 == 0){
                    right_number = graph_items.at(opts.front()).value;
                }
                else if(i1 == 1){
                    if(keyword_index == 0){
                        //the most
                        right_number = graph_items.at(graph_items.size()-1).value;
                    }else {
                        //the least
                         right_number = graph_items.at(0).value;
                    }
                    
                }
                else{
                     right_number = abs(graph_items.at(opts.front()).value - graph_items.at(opts.back()).value);
                }
               
                //add right answer
                Math_Answer* right_ans = Math_Pool::getInstance()->getAnswer();
                right_ans->setEnable(cocos2d::StringUtils::toString(right_number), math::common::answer , CC_CALLBACK_1(BA_ReadDataGraphQuiz::onTouchedAnswer, this), this->getAnswerSize());
                right_ans->setCorrect(true);
                temp_list.answers.push_back(right_ans);
                
                //add wrong answer
                ans_number.push_back(right_number);
                //mở rộng khoảng giá trị random
                if(number_object_max-number_object_min <3){
                    number_object_min--;
                    number_object_max+=3;
                }
                while(ans_number.size() < 4){
                    
                    int wrong_number = cocos2d::random(number_object_min, number_object_max);
                    bool add = true;
                    for(int i2 = 0; i2 < ans_number.size(); i2++){
                        if(wrong_number == ans_number.at(i2) ){
                            add = false;
                        }
                    }
                    
                    if(add){
                        ans_number.push_back(wrong_number);
                        Math_Answer* wrong_ans = Math_Pool::getInstance()->getAnswer();
                        wrong_ans->setEnable(cocos2d::StringUtils::toString(wrong_number), math::common::answer , CC_CALLBACK_1(BA_ReadDataGraphQuiz::onTouchedAnswer, this), this->getAnswerSize());
                        temp_list.answers.push_back(wrong_ans);
                    }
                }
                answers_list.push_back(temp_list);
            }
            
        }else{
            // which - answer
            kind_list.push_back(back_img);
            for(int i1 = 0; i1< getQuestion()->getNumberQuestions(); i1++){
                AnswersList temp_list;
                temp_list.id = i;
                temp_list.q_id = i1;
                std::vector<math::resource::Image*> ans_img;
                math::resource::Image* right_img;
                if(i == 0){
                    right_img = graph_items.at(opts.back()).object->getImageSource();
                }
                else if(i == 1){
                    if(keyword_index == 0){
                        //the most
                        right_img = graph_base.at(0)->getMostItem().object->getImageSource();
                        
                    }else {
                        //the least
                        right_img = graph_base.at(0)->getLeastItem().object->getImageSource();
                    }
                }
                else{
                    //more/less than
                    right_img = graph_items.at(opts.front()).object->getImageSource();
                }
                
                Math_Answer* right_ans_text = Math_Pool::getInstance()->getAnswer();
                right_ans_text->setEnable(right_img->getText("name_1"), math::common::answer , CC_CALLBACK_1(BA_ReadDataGraphQuiz::onTouchedAnswer, this), this->getAnswerSize(2));
                right_ans_text->setCorrect(true);
                temp_list.answers.push_back(right_ans_text);
                ans_img.push_back(right_img);
                //add wrong answer
                while(ans_img.size() < 4){
                    auto wrong_img = kind_list.at(cocos2d::random(0, (int)kind_list.size()-1));
                    
                    bool add = true;
                    for(int i2 = 0; i2 < ans_img.size(); i2++){
                        if(wrong_img == ans_img.at(i2)){
                            add = false;
                        }
                    }
                    
                    if(add){
                        Math_Answer* wrong_ans = Math_Pool::getInstance()->getAnswer();
                        wrong_ans->setEnable(wrong_img->getText("name_1"), math::common::answer , CC_CALLBACK_1(BA_ReadDataGraphQuiz::onTouchedAnswer, this), this->getAnswerSize(2));
                        temp_list.answers.push_back(wrong_ans);
                        ans_img.push_back(wrong_img);
                    }
                }
                answers_list.push_back(temp_list);
            }
        }
        
    }
    
}


void BA_ReadDataGraphQuiz::onStart() {
    CCLOG("%s override", __func__);
    
    
    onShowBottomBar(0, bar_height);
    showQuestionBox(0);
    graph_base.front()->setPosition(cocos2d::Vec2(quiz_size.width*0.5, ((quiz_size.height*(1-bar_height) - getQuestion()->getContentSize().height)/2)));
}


//--------------------------------------
//start Question
void BA_ReadDataGraphQuiz::startQuestion() {
    CCLOG("%s override", __func__);
    //Bat dau cau hoi

    enableControlQuiz();
    answers.clear();
    std::vector<AnswersList> list;
    
    
    if(getQuestion()->getNumberQuestions()>1){
        if(getQuestion()->getData().index == 0){
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(5), cocos2d::CallFunc::create([=]() {
                this->onExplanation(true);
            }), NULL));
        }else{
            std::string q_text = getQuestion()->getQuestionTextValue();
            if(getQuestion()->getQuestionTextValue() .find("How many") != std::string::npos){
                for (int i =0 ; i< answers_list.size(); i++) {
                    if(answers_list.at(i).id == 0){
                        list.push_back(answers_list.at(i));
                        
                    }
                }
                
            }else{
                for (int i =0 ; i< answers_list.size(); i++) {
                    if(answers_list.at(i).id == 1){
                        list.push_back(answers_list.at(i));
                    }
                }
            }
            
            for (int i =0; i<list.size(); i++) {
                if(list.at(i).q_id == getQuestion()->getData().index-1){
                    answers = list.at(i).answers;
                }
                
            }
            
            math::func::shuffle(answers);
            
            math::func::setPositionList(answers, 4, cocos2d::Rect(quiz_size.width * 0.05, quiz_size.height* 0.0, quiz_size.width* 0.9, quiz_size.height*bar_height) );
            
            for(Math_Answer* ans: answers){
                addChildNodeToBottomPanel(ans);
                ans->onShowInParent(_panel_bot, 3);
            }
        }
    }else{
        std::string q_text = getQuestion()->getQuestionTextValue();
        if(getQuestion()->getQuestionTextValue() .find("How many") != std::string::npos){
            for (int i =0 ; i< answers_list.size(); i++) {
                if(answers_list.at(i).id == 0){
                    list.push_back(answers_list.at(i));
                    
                }
            }
            
        }else{
            for (int i =0 ; i< answers_list.size(); i++) {
                if(answers_list.at(i).id == 1){
                    list.push_back(answers_list.at(i));
                }
            }
        }
        
        for (int i =0; i<list.size(); i++) {
            if(list.at(i).q_id == getQuestion()->getData().index){
                answers = list.at(i).answers;
            }
            
        }
        
        math::func::shuffle(answers);
        
        math::func::setPositionList(answers, 4, cocos2d::Rect(quiz_size.width * 0.05, quiz_size.height* 0.0, quiz_size.width* 0.9, quiz_size.height*bar_height));
        
        for(Math_Answer* ans: answers){
            addChildNodeToBottomPanel(ans);
            ans->onShowInParent(_panel_bot, 3);
        }
    }
 
}

//void BA_ReadDataGraphQuiz::onAnswersDone(bool correct){
//   // disableControlQuiz();
//    bool done = true;
//
//    this->onExplanation(done);
//    //_answers.at(0)->onAnimationClick(done);
//}


void BA_ReadDataGraphQuiz::onTouchedAnswer(cocos2d::Node *node){
   // disableControlQuiz();
    Math_Answer* ans = (Math_Answer*) node;
    bool done = false;
    
    if(clickable){
        clickable = false;
        if(ans->isCorrect() == true){
            done = true;
        }

        this->onExplanation(done);
        ans->onAnimationClick(done);
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::CallFunc::create([=]() {
            clickable = true;
        }), NULL));
    }
}

void BA_ReadDataGraphQuiz::onExplanation(bool correct) {
    
    if (!correct){
        this->enableControlQuiz();
        
    }
    else {
        for (Math_Answer* ans: answers) {
            ans->setDisable();
        }
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.35), cocos2d::CallFunc::create([=]() {
            
            this->nextQuestion();
        }), NULL));
    }
    
}

void BA_ReadDataGraphQuiz::onComplete() {
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

bool BA_ReadDataGraphQuiz::sameNumber(std::vector<int> vector , int number){
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
void BA_ReadDataGraphQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void BA_ReadDataGraphQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}
