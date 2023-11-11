//
//  BA_GroupInGroupQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/1/17.
//


//set position
// 3 groups

#include "BA_GroupInGroupQuiz.hpp"

BA_GroupInGroupQuiz* BA_GroupInGroupQuiz::createQuiz()
{
    BA_GroupInGroupQuiz* quiz=new BA_GroupInGroupQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_GroupInGroupQuiz::init() {
    return cocos2d::ui::Layout::init();
}



#pragma mark - Quiz Life-cycle
void BA_GroupInGroupQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    
    number_group = cocos2d::random(json_game.FindMember("group")->value.FindMember("minimum")->value.GetInt(), json_game.FindMember("group")->value.FindMember("maximum")->value.GetInt());
    
    number_min = json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    number_max = json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object = cocos2d::random(number_min, number_max);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    std::string obj_display_value = json_object.FindMember("display_value")->value.GetString();
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    //TODO: OUTER GROUP CONFIG
    rapidjson::Value& json_outer_group=json_data.FindMember("outer_group")->value;
    outer_group_type=json_outer_group.FindMember("type")->value.GetString();
    outer_group_column=json_outer_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
    outer_group_row=json_outer_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
    outer_group_size=cocos2d::Size(json_outer_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_outer_group.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    //TODO: INNER GROUP CONFIG
    rapidjson::Value& json_inner_group=json_data.FindMember("inner_group")->value;
    inner_group_type=json_inner_group.FindMember("type")->value.GetString();
    inner_group_column=json_inner_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
    inner_group_row=json_inner_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
    inner_group_size=cocos2d::Size(json_inner_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_inner_group.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    // list images for object
    auto img_numbers = getNumbers(number_object, number_object);
    img_obj_list = getImageList("icons", 1, "none");
    
    //---------------------------------
    //Gen Object
    number_list.push_back(number_object);
    
    while(number_list.size() < number_group){
        int wrong_number = cocos2d::random(number_object-3, number_object+5);
        bool add = true;
        for(int i : number_list){
            if (wrong_number == i){
                add = false;
                break;
            }
        }
        if(add){
            number_list.push_back(wrong_number);
        }
    }
    
    //---------------------------------
    //group background
    math::resource::Image* group_bg = groupBG();
    math::resource::Image* inner_group_bg =  math::resource::ResHelper::createImage(getUIPath("common/frame.png") );
    
    //outer group type
    math::group::GroupType outer_gr_type=math::group::custom;
    if (outer_group_type.compare("queue")==0) {
        outer_gr_type=math::group::queue;
    } else if (outer_group_type.compare("frame")==0) {
        outer_gr_type=math::group::frame;
    }
    
    //inner group type
    math::group::GroupType inner_gr_type=math::group::custom;
    if (inner_group_type.compare("queue")==0) {
        inner_gr_type=math::group::queue;
    } else if (inner_group_type.compare("frame")==0) {
        inner_gr_type=math::group::frame;
    }
    
    //find max in number_list
    int max =0;
    for(int i = 0; i< number_list.size(); i++){
        if(number_list.at(i) > max){
            max = number_list.at(i);
        }
    }
    
    // number of inner group
    int number_inner_group;
    int s = inner_group_row * inner_group_column; //full obj in each inner group
    int div = max / s;
    int mod = max % s;
    if(mod > 0){
        number_inner_group = div + 1;
    }else {
        number_inner_group = div;
    }
    int temp_number;
    
    //create outer groups vector
    for(int i = 0; i < number_list.size(); i++){
        
        //number of obj of each outer group
        int number_obj_gr = number_list[i];
        
        std::vector<Math_Group*> inner_gr;
        std::vector<Math_Object*> group_object_list;
        mod = number_obj_gr % s;
        div = number_obj_gr /s;
        
        if(mod >= 0 && div !=0 ){
            temp_number = s;
        }else{
            temp_number = mod;
        }
        
        //create inner groups vector
        for(int j = 0; j < number_inner_group; j++){
            
            std::vector<Math_Object*> object_list;
            group_cap = inner_group_column * number_inner_group ;
            
            if(temp_number > s){
                temp_number = s;
            }
            for(int k = 0; k < temp_number; k++){
                Math_Object* object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, img_obj_list[0], NULL);
                object->setObjectSize(object_size);
                object_list.push_back(object);
            }
            temp_number = number_obj_gr - temp_number;
            number_obj_gr = temp_number;
            
            //setup inner group
            Math_Group* group = Math_Pool::getInstance()->getGroup();
            group->setEnable(j, object_list, inner_gr_type, group_cap);
            group->setObjectSize(object_size);
            
            if (inner_gr_type==math::group::frame) {
                group->setGroupFrames(inner_group_column, inner_group_row, inner_group_bg);
                group->setAlignObjects(math::group::left);
            }
            //set up group with custom config
            else {
                group->setGroupSize(inner_group_size);
                group->setBackground(inner_group_bg);
            }
            group->setBackgroundVisiable(false);
            inner_gr.push_back(group);
            
            inner_group_size = group->getContentSize();
            //setup objects to getPos in outer group
            Math_Object* temp_obj = Math_Pool::getInstance()->getObject();
            
            temp_obj->setEnable(i, math::common::deny, img_obj_list[0], group_bg);
            temp_obj->setImageVisible(false);
            temp_obj->setBackgroundVisible(false);
            temp_obj->setObjectSize(inner_group_size);
            group_object_list.push_back(temp_obj);
           
            
        }
        
        inner_groups.push_back(inner_gr);
        
        //setup outer group
        Math_Group* group = Math_Pool::getInstance()->getGroup();
        group->setEnable(i, group_object_list, outer_gr_type, (int)inner_groups.front().size());
        
        group->setName(cocos2d::StringUtils::toString(i));
        group->setObjectSize(inner_group_size);
        group->setTouchEnabled(true);
        group->enableToch(CC_CALLBACK_1(BA_GroupInGroupQuiz::onTouchOnGroup, this));
        
        if (outer_gr_type==math::group::frame) {
            group->setGroupFrames(outer_group_column, outer_group_row, group_bg);
            group->setAlignObjects(math::group::left);
        }
        //set up group with custom config
        else {
            group->setGroupQueue(inner_group_size*1.25);
            group->setBackground(group_bg);
        }
        
        _groups.push_back(group);
    }
    
    
    //ICON CONFIG
    //PARSING QUESTION
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_numbers) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_numbers) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "name_n") {
            if (img_numbers.front()->getComparisionValue("number")==1) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : img_list) {
                    auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_list);
            }
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }

    //Math_Pool::getInstance()->resetImages();
}

//--------------------------------------------------
void BA_GroupInGroupQuiz::onStart(){
    
    //---------------------------------
    //Gen question
    showQuestionBox(0);
//    disableControlQuiz(5);
//    this->getQuestion()->onStart();
    math::func::setPositionList(_groups, 2, cocos2d::Rect(quiz_size.width* 0, quiz_size.height* 0, quiz_size.width* 1,quiz_size.height - getQuestion()->getContentSize().height));
    //_groups.at(0)->setPosition(cocos2d::Vec2(quiz_size.width/2, (quiz_size.height-getQuestion()->getContentSize().height)/2));
    
    for(Math_Group* gr: _groups){
        addChildNode(gr,2);
        gr->onShow();
        
    }
    
    math::func::shuffle(inner_groups);
    for(int i = 0 ; i < inner_groups.size(); i++){
        for(int j = 0; j < inner_groups.at(i).size(); j++){
            Math_Group* gr_obj = inner_groups.at(i).at(j);
              Math_Object* obj = _groups.at(i)->getObjectAtIndex(j);
            
            cocos2d::Vec2 pos (_panel->convertToNodeSpace(_groups.at(i)->convertToWorldSpace(cocos2d::Vec2(obj->getPosition().x+obj->getContentSize().width/3.5 , obj->getPosition().y))));
            gr_obj->setPosition(pos);
            addChildNode(gr_obj,3);
            gr_obj->onShow();
            
        }
    }
}

void BA_GroupInGroupQuiz::startQuestion() {
    enableControlQuiz();
}

void BA_GroupInGroupQuiz::onExplanation(bool correct){
    if (!correct) {
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=](){
            enableControlQuiz();
        }), NULL));
        
    }
    else {
        this->stopAllActions();
        this->nextQuestion();
    }
    
}

void BA_GroupInGroupQuiz::onComplete(){
    //TODO: Thuc hien them cac lenh o day
    
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    
    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }
    
    scheduleOnce([=](float dt) {
        for (int i = 0 ; i < inner_groups.size(); i++) {
            for(Math_Group* gr : inner_groups.at(i)){
                gr->setDisable();
            }
        }
    }, m_delay, "hide_groups");
    
    scheduleOnce([=](float dt) {
        for (Math_Group* group : _groups) {
            group->setDisable();
        }
    }, m_delay, "hide_groups");
    
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void BA_GroupInGroupQuiz::onTouchOnGroup(Node* node){
    
    disableControlQuiz();
    this->stopAllActions();
    
    int sum = 0;
    bool correct = false;
    
    node->runAction(cocos2d::Repeat::create(cocos2d::Sequence::create( cocos2d::FadeOut::create(0.35), cocos2d::FadeIn::create(0.35), NULL),2));
    
    
    std::vector<Math_Group*> in_gr = inner_groups.at(atoi(node->getName().c_str()));
    for(int i = 0; i < in_gr.size(); i++ ){
        sum += in_gr.at(i)->getNumberObjects();
    }
    
    if(sum == number_object){
        correct = true;
    }
    
    this->onExplanation(correct);
    
}


#define auto_testing_delay 2

void BA_GroupInGroupQuiz::autoTesting(){
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_GroupInGroupQuiz::autoTestingRun(float delaytime){
    
    //Choose answer
    //bool done=true;
}


