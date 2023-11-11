//
//  BA_TapToAdd.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/18/17.
//

//status: da hoan thanh

#include "BA_TapToAddQuiz.hpp"
#include <algorithm>

using namespace cocos2d;

BA_TapToAddQuiz* BA_TapToAddQuiz::createQuiz()
{
    BA_TapToAddQuiz* quiz=new BA_TapToAddQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_TapToAddQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_TapToAddQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    game_mode = getJsonBool(json_data, "game_mode",true) ;
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    object_size = getSizeJson(json_object, "size");
    
//    range_max = json_object.FindMember("range")->value.FindMember("maximum")->value.GetInt();
//    range_min = json_object.FindMember("range")->value.FindMember("minimum")->value.GetInt();
    
    //random total number object
    number_object = cocos2d::random(number_object_min, number_object_max);
    standard_object = cocos2d::random(number_object_min, number_object_max);
    total_object = number_object + standard_object;
    
    //------------------------------
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = dislay_json.FindMember("type")->value.GetString();
    max_in_line = dislay_json.FindMember("max_line")->value.GetInt();
    
    //---------------------------------
    //image list
    auto img_number_list = getNumbers(number_object, number_object);
    
    //============================
    //Icon config
//    number_kind=1;
//    std::string icon_filter = json_data.FindMember("icon")->value.GetString();
    img_list = getImageList("icons", 1, "id");
    /*---> Lay xong data cua game*/
    
    //PARSING QUESTION
    parseQuestion();
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
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "name_n") {
            if (number_object==1) {
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
    
    if(!game_mode){
        number_object += cocos2d::random(1, 4);
    }
    //resize obj
    if((number_object+standard_object)/7 >= 4){
        object_size = object_size*0.8;
    }
    
    //----------------------------------
    //Gen objects
    for (int i = 0; i <standard_object; i++) {
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(obj_id++, math::common::deny, img_list.front(), NULL);
        object->setLocked(true);
        object->setObjectSize(object_size);
        object->getImage()->setOpacity(255);
        
        _objects.push_back(object);
    }
    
    
    for(int i = 0; i < number_object; i++){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(obj_id++, math::common::deny, img_list.front(), NULL);
        obj->getImage()->setOpacity(1);
        if(game_mode){
            if(i < total_object - standard_object){
                obj->getImage()->setOpacity(80);
            }
        }
        
        obj->setObjectSize(object_size);
       
        extra_object.push_back(obj);
    }
    
    //event listener
    auto listener1 = cocos2d:: EventListenerTouchOneByOne::create();
    listener1->setSwallowTouches(true);
    // trigger when you push down
    listener1->onTouchBegan = CC_CALLBACK_2(BA_TapToAddQuiz::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, this);
    
    //---------------------------------
    //Gen line
    line = mpool->getObject();
    line->setEnable(0, math::common::touch, img_list.front(), nullptr);
    line->getImage()->setColor(cocos2d::Color3B::BLACK);
    line->getImage()->setOpacity(150);
    line->setObjectSize(object_size);
    if(game_mode){
        line->setVisible(false);
    }
    
}

void BA_TapToAddQuiz::onStart() {
    
    float bar_h = 200/quiz_size.height;
    if(!game_mode){
        onShowBottomBar(0, bar_h , false, CC_CALLBACK_0(BA_TapToAddQuiz::onAnswerDone, this),nullptr, true);
    }
    showQuestionBox(0);
    
    //----------------------------------
    // set position lists
    if (display_type.compare("line")==0) {
        if(game_mode){
            _objects.insert( _objects.end(), extra_object.begin(), extra_object.end() );
            math::func::setPositionList(_objects, max_in_line, cocos2d::Rect (0.1, 0.2, 0.8, 0.65));
          
        }else{
            float pos_y = 0;
            setObjectPos(_objects, pos_y + 0.25, 0.0);
            setObjectPos(extra_object, pos_y, 0);
            
            _objects.insert( _objects.end(), extra_object.begin(), extra_object.end() );
        }
    } else {
        //random
        _objects.insert( _objects.end(), extra_object.begin(), extra_object.end() );
        
        cocos2d::Rect rect_box;
        if (_objects.size() < 5) {
            rect_box = cocos2d::Rect(200, 100, 500, 500);
        } else if (_objects.size() < 10) {
            rect_box = cocos2d::Rect(150, 100, 700, 500);
        } else {
            rect_box = cocos2d::Rect(0, 100, 900, 500);
        }
        auto list_points = randomPositionsInRect(object_size, rect_box, (int)_objects.size());
        for (int i=0; i<_objects.size(); i++) {
            _objects[i]->setPosition(list_points[i]);
        }
    }
   
//    disableControlQuiz(5);
//    this->getQuestion()->onStart();
}

void BA_TapToAddQuiz::setObjectPos(std::vector<Math_Object *> object_list, float pos_y, float t){
    
    int number_objs = (int)object_list.size();
    
    if (number_objs<=5) {
        float objs_width = number_objs*(object_size.width + 50);
        float pos_x = ((900-objs_width) /2.0)/768.0;
        if(t != 0){
            math::func::setPositionList(object_list, number_objs, cocos2d::Rect(pos_x, pos_y - t, 1-pos_x*2, pos_y +0.1));
        } else {
            math::func::setPositionList(object_list, number_objs, cocos2d::Rect(pos_x, pos_y + 0.05, 1-pos_x*2, pos_y +0.1));
        }
        
    } else if (number_objs <= 10) {
        int max_in_line = (number_objs+1)/2;
        int delta = 50;
        float objs_width = max_in_line*(object_size.width + delta);
        while (objs_width>900) {
            delta-=5;
            objs_width = max_in_line*(object_size.width + delta);
        }
        float pos_x = ((900-objs_width) /2.0)/768.0;
        
        int number_line = number_objs/max_in_line;
        number_line+=(number_objs%max_in_line)==0?0:1;
        
        math::func::setPositionList(object_list, max_in_line, cocos2d::Rect(pos_x, pos_y, 1-pos_x*2, number_line *0.15));
        
    } else {
        int max_in_line=(number_objs+1)/2;
        if (max_in_line>7) {
            max_in_line =(number_objs+2)/3;
        }
        int delta = 50;
        float objs_width = max_in_line*(object_size.width + delta);
        while (objs_width>900) {
            delta-=5;
            objs_width = max_in_line*(object_size.width + delta);
        }
        float pos_x = ((900-objs_width) /2.0)/768.0;
        
        int number_line = number_objs/max_in_line;
        number_line+=(number_objs%max_in_line)==0?0:1;
        
        math::func::setPositionList(object_list, max_in_line, cocos2d::Rect(pos_x, pos_y, 1-pos_x*2, number_line*0.15));
    }
}

//--------------------------------------
//start Question
void BA_TapToAddQuiz::startQuestion() {
    //Bat dau cau hoi
    enableControlQuiz();
    
    //----------------------------
    //show obj
    float delay=0;

    for(Math_Object* obj: _objects){
        addChildNode(obj);
        obj->onShow(delay);
        CCLOG("pos: %f, %f", obj->getPosition().x, obj->getPosition().y);
        delay+=0.05;
    }
    
//    if(!game_mode){
        line->setPosition(_objects[standard_object]->getPosition());
        addChildNode(line);
        line->onShow(0.5);
//    }
    
    
    
    for(Math_Answer* ans: _answers){
        addChildNode(ans);
        ans->onShowInParent(_panel, 2);
    }
}


#pragma mark - object touch events callback
bool BA_TapToAddQuiz::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent){
    
    //tr hop click dup
    if(!game_mode){
        disableControlQuiz();
    }
    
    bool touch_screen = true;
    
    for(int i = standard_object ; i < _objects.size(); i++){
        Math_Object* obj = _objects[i];
        if(obj->getBoundingBox().containsPoint(obj->getParent()->convertToNodeSpace(touch->getLocation())))
        {
            onTouchedObject(_objects[i]);
            touch_screen = false;
        }
    }
    
    if(touch_screen){
        for(int i =0 ; i < extra_object.size(); i++){
            if(extra_object[i]->getImage()->getOpacity() != 255 ){
                extra_object[i]->getImage()->setOpacity(255);
                break;
            }
        }
    }
    
    if(line != NULL){
        setLinePos(true);
    }
    
    if(game_mode){
        onAnswerDone();
    }
    
    return true;
}

void BA_TapToAddQuiz::onTouchedObject(Node* node){
    
    int opacity =1 ;
    if(game_mode){
        opacity = 80;
    }
    
    Math_Object* obj = (Math_Object*) node;
    if(obj->getPosition() == line->getPosition()){
        opacity = 255;
        
    }
   
    if(!game_mode){
        if(obj->getImage()->getOpacity() !=255){
            for(int i = 0; i < _objects.size(); i++){
                if(_objects.at(i)->getPosition() == line->getPosition()){
                    opacity = 255;
                    obj = _objects.at(i);
                }
            }
        }
    }
    
    obj->getImage()->setOpacity(opacity);
}

void BA_TapToAddQuiz::setLinePos(bool go){
    
    line_pos = line->getPosition();
    
    for(int i = standard_object ; i < _objects.size(); i++){
        if(_objects.at(i)->getImage()->getOpacity()!= 255){
            line_pos = _objects.at(i)->getPosition();
            break;
        }
        else{
            if(line_pos == _objects[i]->getPosition()){
                
                if(i+1 < _objects.size()){
                    line_pos = _objects[i+1]->getPosition();
                    
                }else{
                    line_pos = Vec2(2000, 2000);
                }
                
            }
        }
    }
    
    line->runAction(cocos2d::MoveTo::create(0, line_pos));
    
    if(game_mode){
        if(line_pos == Vec2(2000, 2000)){
            onExplanation(true);
            disableControlQuiz();
        }
    }
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
        enableControlQuiz();
    }), nullptr));
}

void BA_TapToAddQuiz::onAnswerDone()
{
    disableControlQuiz();
    bool done = false;
    int count =0;
    for(Math_Object* obj : _objects){
        if (obj->getImage()->getOpacity() == 255) {
            count++;
        }
    }
    
    if(count == total_object){
        done = true;
    }
    
    this->onExplanation(done);
//    if(_answers.size() != 0){
//        _answers[0]->onAnimationClick(done);
//    }
}


void BA_TapToAddQuiz::onExplanation(bool correct) {
    
    if (!correct){
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
            enableControlQuiz();
        }), nullptr));
    }
    else {
        this->nextQuestion();
    }
}

void BA_TapToAddQuiz::onComplete() {
    
    //TODO: Thuc hien them cac lenh o day
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    
    if(line != NULL){
        line->removeFromParentAndCleanup(true);
    }
    
    for(Math_Object* obj:_objects){
        obj->onHide(delay);
    }
    for(Math_Answer* ans: _answers){
        ans->setDisable();
    }
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_TapToAddQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_TapToAddQuiz::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    autoTesting();
}



