//
//  BA_CountBlockQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/19/17.
//

//status: Completed


#include "BA_CountBlockQuiz.hpp"

BA_CountBlockQuiz* BA_CountBlockQuiz::createQuiz()
{
    BA_CountBlockQuiz* quiz=new BA_CountBlockQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_CountBlockQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_CountBlockQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    game_mode = getJsonBool(json_data, "game", false);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    
    //============================
    //Icon config
    
    std::string icon_same = "color";
    std::string icon_different = "id";

    img_list = getImageList("icons", 3, icon_different, icon_same);
    total_object = cocos2d::random(number_object_min, number_object_max);
   
    if(game_mode){
        bar_h = 250.0/quiz_size.height;
    }else{
        bar_h = 220.0/quiz_size.height;
    }
    bar_w = 200.0/quiz_size.width;
    
    //-------------------------------------
    // calculate row and column
    int number_tens = total_object / 10;
    int mod = total_object % 10;
    int number_fives = mod / 5;
    int number_ones = mod % 5;

    if (number_ones > 0) {
        column = number_tens + number_fives + 1;
    } else {
        column = number_tens + number_fives;
    }
    std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
        return a->getComparisionValue("number") < b->getComparisionValue("number");
    });
    
    object_size = cocos2d::Size (65, 65);
    if (number_tens > 0) {
        object_size = cocos2d::Size(45, 45);
    }
    
    float position_y_tens = 215;
    float position_x = (quiz_size.width - column * object_size.width * 1.5 + object_size.width*1.5) / 2;
    
    for (int i=0; i<number_tens; i++) {
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(obj_id, math::common::deny, img_list[2], nullptr);
        obj->setObjectSize(object_size);
        obj->fixWidthObject();
        obj->setPosition(cocos2d::Vec2(position_x, position_y_tens));
        position_x+=(object_size.width*1.5);
        _objects.push_back(obj);
        blocks_object.push_back(obj);
    }
    
    float position_y_fives = 60;
    if (blocks_object.size()>0) {
        position_y_fives = position_y_tens- (blocks_object.front()->getContentSize().height)/2;
    }
    for (int i=0; i<number_fives; i++) {
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(obj_id, math::common::deny, img_list[1], nullptr);
        obj->setObjectSize(object_size);
        obj->fixWidthObject();
        obj->setPosition(cocos2d::Vec2(position_x, position_y_fives+obj->getContentSize().height/2));
        position_x+=(object_size.width*1.5);
        _objects.push_back(obj);
        blocks_object.push_back(obj);
    }

    int position_y_ones = position_y_fives;
    for (int i=0; i<number_ones; i++) {
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(obj_id, math::common::deny, img_list[0], nullptr);
        obj->setObjectSize(object_size);
        obj->fixWidthObject();
        obj->setPosition(cocos2d::Vec2(position_x, position_y_ones+ obj->getContentSize().height/2 ));
        position_y_ones+=(object_size.height);
        _objects.push_back(obj);
        ones_object.push_back(obj);
    }
    
    //create answer value list
    answer_list.push_back(total_object);
    std::vector<int> wrong_answer;
    if (number_tens > 1) {
        wrong_answer.push_back(total_object-10);
    }
    wrong_answer.push_back(total_object+10);
    if (total_object>5) {
        wrong_answer.push_back(total_object-5);
    }
    wrong_answer.push_back(total_object+5);
    wrong_answer.push_back(total_object-1);
    wrong_answer.push_back(total_object+1);

    math::func::shuffle(wrong_answer);
    
    while (answer_list.size() < 4) {
        answer_list.push_back(wrong_answer.back());
        wrong_answer.pop_back();
    }
    int min = number_object_max;
    int max = 0;
    for (int value : answer_list) {
        if (value > max) {
            max = value;
        }
        if (value < min) {
            min = value;
        }
    }
    
    auto img_numbers = getNumbers(min, max);
    std::vector<math::resource::Image*> img_number_list;
    for (int value : answer_list) {
        for (math::resource::Image* img : img_numbers) {
            if (value == img->getComparisionValue("number")) {
                img_number_list.push_back(img);
                break;
            }
        }
    }
    
    //---------------------------------
    //Gen answers
    for (int i=0; i<4; i++) {
        Math_Answer* btn=Math_Pool::getInstance()->getAnswer();

        if(game_mode){
            std::string numb = cocos2d::StringUtils::toString( img_number_list.at(i)->getComparisionValue("number"));
            if(numb.size() == 1){
                numb.insert(numb.front(), "0");
            }
            std::string key = cocos2d::StringUtils::toString( numb.at(0)) +" tens " +" + " +cocos2d::StringUtils::toString( numb.at(1))+ " ones";
            btn->setEnable(key, math::common::answer, CC_CALLBACK_1(BA_CountBlockQuiz::onTouchedAnswer, this), this->getAnswerSize(6));
           // btn->setTitleWithIconProp(key);
        }else{
            btn->setEnable("", math::common::answer, CC_CALLBACK_1(BA_CountBlockQuiz::onTouchedAnswer, this), this->getAnswerSize(0));
            btn->setIcon(img_number_list[i]);
            btn->setTitleWithIconProp("number");
        }
        
        if(i == 0){
            btn->setCorrect(true);
        }
       
        _answers.push_back(btn);
    }
    
    math::func::shuffle(_answers);
    
    this->parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
}

void BA_CountBlockQuiz::onStart() {
    for(int i = 0; i < _objects.size(); i++){
        Math_Object* obj = _objects[i];
        addChildNode(obj);
        obj->onShow(1);
    }
    
    //---------------------------------
    onShowBottomBar(0, bar_h);
    showQuestionBox(0);

    //----------------------------------
    // set position lists
    if(game_mode){
        math::func::setPositionList(_answers, 2, cocos2d::Rect(quiz_size.width * 0.1, quiz_size.height* 0.0, quiz_size.width* 0.8, quiz_size.height*bar_h));
    }else{
        math::func::setPositionList(_answers, (int)_answers.size(), cocos2d::Rect(quiz_size.width * 0.1, quiz_size.height* 0.0, quiz_size.width* 0.8, quiz_size.height*bar_h));
    }
    
}

//--------------------------------------
//start Question
void BA_CountBlockQuiz::startQuestion() {
    //Bat dau cau hoi
    enableControlQuiz();
    if(getQuestion()->getData().index == 0){
        
        for(Math_Answer* ans: _answers){
            ans->onShowInParent(_panel_bot, 2);
        }
    }
}


#pragma mark - object touch events callback
void BA_CountBlockQuiz::onTouchedAnswer(cocos2d::Node* node)
{
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


void BA_CountBlockQuiz::onExplanation(bool correct) {
    
    if (!correct){
        
    }
    else {
        this->nextQuestion();
    }
}

void BA_CountBlockQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    
    for(Math_Object* obj : _objects){
        obj->onHide(delay);
        
    }
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_CountBlockQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_CountBlockQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}

