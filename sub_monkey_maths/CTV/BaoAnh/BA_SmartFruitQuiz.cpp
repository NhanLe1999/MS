//
//  BA_SmartFruitQuiz2.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 8/31/17.
//
//

//status: Completed

#include "BA_SmartFruitQuiz.hpp"
BA_SmartFruitQuiz* BA_SmartFruitQuiz::createQuiz()
{
    BA_SmartFruitQuiz* quiz=new BA_SmartFruitQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_SmartFruitQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void BA_SmartFruitQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    value_object_max = json_object.FindMember("value")->value.FindMember("maximum")->value.GetInt();
    value_object_min = json_object.FindMember("value")->value.FindMember("minimum")->value.GetInt();
    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
   
    group_cap=number_object_max*2;
    group_size = object_size;
    display_mode = json_object.FindMember("display")->value.GetInt();
    
    //GEN QUESTION
    parseQuestion();
    
    bar_h = 200.0/quiz_size.height;
     img_list = getImageList("icons", 1, "id");
    //-------------------------------------
    //Gen result
    number_object = cocos2d::random(number_object_min, number_object_max);
    
    number_right = (int) number_object/2;
    
    result = cocos2d::random(value_object_min, value_object_max );
    
    auto number_imgs = getNumbers(0, 20);
    
    math::resource::Image* result_img;
    
    for (math::resource::Image* img : number_imgs ) {
        
        if (img->getComparisionValue("number")== result) {
            result_img=img;
            break;
        }
    }
    
    Math_Object* object=Math_Pool::getInstance()->getObject();
    object->setEnable(0, math::common::deny, result_img, NULL);
    object->setObjectSize(object_size);
    object->setImageVisible(false);
   // object->setValueTextByKey("number");
    object->setValueText(cocos2d::StringUtils::toString(result));
    object->getLabelValue()->setFontSize(object_size.height);
    object->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
    object->showValueTextWithAnimation(false);
    
    resultList.push_back(object);
    // _panel->addChild(object,2);
    
    //-------------------------------------
    //Gen objects
    
    fruitList = createFruitList(result);
    
    //-------------------------------------
    //Gen groups
    //fruit group bg
    
    math::resource::Image* group_bg;
   
//    auto bg_list=math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
  //  int rand_img = rand()%bg_list.size();
    
    group_bg = img_list[0];
    
    //group type
    math::group::GroupType gr_type=math::group::frame;
    
    for(int i = 0; i < expr_list.size(); i++){
        
        std::vector<Math_Object*> obj_list;
        obj_list.push_back(fruitList[i]);
        
        Math_Group* group = Math_Pool::getInstance()->getGroup();
        
        group->setEnable(i, obj_list, gr_type, group_cap);
        group->enableToch(CC_CALLBACK_1(BA_SmartFruitQuiz::onTouchedGroup, this));
        group->setGroupFrames(1, 1, group_bg);
        group->setGroupSize(object_size);
        group->setObjectSize(object_size);
        group->setGroupName(cocos2d::StringUtils::toString(result));
        group->showNameText(true);
        _groups.push_back(group);
    }
    
    
    //----------------------------------
    // set position lists
    
   
    
    
//    if (img_bg!=NULL) {
//        img_bg->used=true;
//    }
//    for (math::resource::Image* img : img_list) {
//        img->used=true;
//    }
//
    //Math_Pool::getInstance()->resetImages();
    
}


void BA_SmartFruitQuiz::onStart() {
    //---------------------------------
    //Gen button done
//    Math_Answer* btn_done=Math_Pool::getInstance()->getAnswer();
//    btn_done->setEnable("Done", math::common::answer, CC_CALLBACK_1(BA_SmartFruitQuiz::onAnswerDone, this), this->getAnswerSize(2));
//    btn_done->setPosition(cocos2d::Vec2(450, 55));
//    _answers.push_back(btn_done);
    
    
    //---------------------------------
    //Gen question
    onShowBottomBar(0, bar_h, false, CC_CALLBACK_1(BA_SmartFruitQuiz::onAnswerDone,this),CC_CALLBACK_1(BA_SmartFruitQuiz::DoNothing, this),true);
    if(display_mode ==1 ){
        resultList.at(0)->setPosition(cocos2d::Vec2(quiz_size.width/2, (quiz_size.height - getQuestion()->getContentSize().height)/2));
        // math::func::setPositionList(resultList, 1, cocos2d::Rect(0.3, 0.15, 0.4, 0.4));
        math::func::setPositionList(fruitList, (int) fruitList.size(), cocos2d::Rect(quiz_size.width*0.05, quiz_size.height*0, quiz_size.width*0.9,quiz_size.height* 0.35));
    }
    else{
        math::func::setPositionList(_groups, (int) _groups.size(), cocos2d::Rect(quiz_size.width*0.05, quiz_size.height*0, quiz_size.width*0.9,(quiz_size.height-bar_h)/2));
    }
    
    if(display_mode == 1){
        float delay=0.5;
        for(Math_Object* Obj: resultList){
            addChildNode(Obj, 2);
            Obj->onShow(delay);
            
        }
        
        for(Math_Object* Obj: fruitList){
            addChildNode(Obj, 2);
            //addChildNodeToBottomPanel(Obj,2);
            Obj->onShow(delay);
            Obj->showValueTextWithAnimation(true);
            delay+=0.1;
        }
    } else {
        for (Math_Group* gr : _groups) {
            addChildNode(gr,2);
            //addChildNodeToBottomPanel(gr,2);
            gr->onShow();
        }
    }
    showQuestionBox(0);
//    disableControlQuiz(5);
//    this->getQuestion()->onStart();
}


//--------------------------------------
//start Question
void BA_SmartFruitQuiz::startQuestion() {
    //Bat dau cau hoi
    if(getQuestion()->getData().index == 0){
        
        enableControlQuiz();
        for (Math_Answer* answer: _answers){
            addChildNode(answer);
            answer->onShowInParent(_panel, 2);
        }

    }
}


//--------------------------------------
//BA_function data creation
std::vector<Math_Object*> BA_SmartFruitQuiz::createFruitList(int result){
    
    std::vector<Math_Object*> fruit_list;
    
    //----------------------------------
    //gen phep tinh
    auto img_bg = img_list.front();
    //img_bg = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%sbackground.json", _game_path.c_str()), 3);
   // img_bg->used=true;
    
    std::vector<std::pair<std::string, int>> expr_list;
    
    expr_list = createExprList(result);
    
    std::random_shuffle(expr_list.begin(), expr_list.end() );
    int obj_id =0;
    for(int i = 0; i < number_object; i++){
        
        Math_Object* object=Math_Pool::getInstance()->getObject();
        
        if(display_mode ==1 ){
            
            object->setEnable(obj_id, math::common::touch, img_bg, NULL);
            object->setBackgroundVisible(false);
            object->setTouchCallback(CC_CALLBACK_1(BA_SmartFruitQuiz::onTouchedObject, this));
        }
        else {
            object->setEnable(obj_id, math::common::touch, img_bg, NULL);
        }
        
        object->setObjectSize(object_size*1.2);
        object->setImageVisible(false);
        object->setValueText(expr_list[i].first);
        object->fitValueTextSize();
        object->showValueTextWithAnimation(false);
        //object->onShow(0);
        object->setTag(expr_list[i].second);
        
        obj_id++;
        
        CCLOG("phep tinh %s", expr_list[i].first.c_str());
        fruit_list.push_back(object);
       // _panel->addChild(object);
    }
    CCLOG("%lu", expr_list.size());
    return fruit_list;
}

//---------------------------------
//BA_function data creation
bool BA_SmartFruitQuiz::checkSameExpr(std::string expr){
    if(expr_list.size() == 0){
        return false;
    }
    else{
        for(int i = 0; i < expr_list.size(); i++){
            if(expr == expr_list[i].first){
                return true;
            }
        }
    }
    return false;
}

//---------------------------------
//BA_function data creation
std::vector<std::pair<std::string, int>> BA_SmartFruitQuiz::createExprList(int result){
    
    expr_list.clear();
    
    std::pair<std::string, int> expr;
    int count = 0;
    
    int i;
    for(i = 0; i < number_object; i++){
        
        expr = randomExpression(result, count);
        
        if (checkSameExpr(expr.first) == true){
            i--;
        }else {
            expr_list.push_back(expr);
            count++;
        }
        
    }
    
    return expr_list;
}

//---------------------------------
//BA_function data creation
std::pair<std::string, int> BA_SmartFruitQuiz::randomExpression(int result, int count){
    std::pair<std::string, int> expression;
    std::string oprt ;
    
    int stNumb = cocos2d::random(value_object_min,value_object_max);
    int ndNumb = 0;
    int wrongResult = randomWrongResult(result);
    int tempResult;
    
    if(count < number_right){
        tempResult = result;
    }else{
        
        tempResult = wrongResult;
    }
    
    if(tempResult < stNumb){
        oprt = "-";
    }else {
        oprt = "+";
    }
    
    if(oprt.compare("+") == 0){
        ndNumb = tempResult - stNumb;
        
    }else {
        ndNumb = tempResult + stNumb;
        
    }
    
    std::string expr = cocos2d::StringUtils::toString(ndNumb) + oprt + cocos2d::StringUtils::toString(stNumb);
    
    expression.first = expr;
    expression.second = tempResult;
    
    return expression;
}

//---------------------------------
//BA_function data creation
int BA_SmartFruitQuiz::randomWrongResult(int result){
    int wrongResult = cocos2d::random(value_object_min, value_object_max);
    if(wrongResult == result){
        wrongResult = randomWrongResult(result);
    }
    return wrongResult;
}

//---------------------------------
//BA_function logic procession
void BA_SmartFruitQuiz::lockedObj(bool lock){
    
    for(Math_Object* obj: fruitList){
        if(obj->isSelected() == false){
            obj->setLocked(lock);
        }
    }
}

void BA_SmartFruitQuiz::onTouchedGroup(cocos2d::Node *node){
    
    Math_Group* group = (Math_Group*) node;
  
    Math_Object* obj = group->getObjectAtIndex(0);
    
    onTouchedObject(obj);
    
    int count = 0;
    
    if(obj->isSelected()){
        group->onTouchedAnimation(false);
    }
    else {
        node->stopAllActions();
    }
   
    for (Math_Object* obj: fruitList) {
        if(obj->isSelected()) count++;
    }
    
    if(count == number_right){
        for(Math_Group* gr: _groups){
            gr->enableToch(CC_CALLBACK_1(BA_SmartFruitQuiz::DoNothing, this));
        }
    }
}

void BA_SmartFruitQuiz::DoNothing(cocos2d::Node* node){
    
}

#pragma mark - object touch events callback
void BA_SmartFruitQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = (Math_Object*) node;
    
    bool selected = false;
    bool locked = false;
    
    if(object->isSelected() == false){
        selected = true;
    }
    
    object->setSelected(selected);
    
    if(checkSelectedObj() == true){
        locked = true;
    }
    
    lockedObj(locked);
}

bool BA_SmartFruitQuiz::checkSelectedObj(){
    int count =0;
    
    for(Math_Object* obj : fruitList){
        if(obj->isSelected() == true){
            count ++;
        }
    }
    
    if(count == number_right){
        return true;
    }
    
    return false;
}

void BA_SmartFruitQuiz::onAnswerDone(bool correct)
{
    int count = 0;
    bool done = false;
    
    for(Math_Object* obj: fruitList){
        if(obj->isSelected() == true){
            if(obj->getTag() == result){
                count ++;
            };
        }
    }
    
    
    if(count == number_right){
        done = true;
    }else {
        for(Math_Group* gr: _groups){
            gr->enableToch(CC_CALLBACK_1(BA_SmartFruitQuiz::onTouchedGroup, this));
        }
    }
    
    this->onExplanation(done);
   // _answers.at(0)->onAnimationClick(done);
}

void BA_SmartFruitQuiz::onExplanation(bool correct) {
    
    if (!correct){
        for(Math_Object* obj: fruitList){
            if(obj->isSelected() == true){
                onTouchedObject(obj);
            }
            obj->setLocked(false);
        }
        
    }
    else {
        
        this->nextQuestion();
    }
    
}

void BA_SmartFruitQuiz::onComplete() {
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
        for (Math_Group* group : _groups) {
            group->onHide();
        }
        
        for (Math_Object* result: resultList) {
            result->onHide(0);
        }
        
        for (Math_Object* object : fruitList) {
            object->onHide(0);
        }

    }, m_delay, "hide_groups");
    
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_SmartFruitQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_SmartFruitQuiz::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}
