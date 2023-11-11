//
//  BA_ChooseTwoMakeOneQuiz.cpp
//  MJQuiz
//
//  Created by Admin on 8/23/17.
//
//

//status: Completed
//game co 2 che do kiem tra ket qua (check_inline)
//da hoan thanh ca 2 che do kiem tra kq
//check_inline = true ->autocheck
//              = false-> chọn Done để check

#include "BA_ChooseTwoMakeOneQuiz.hpp"
#include <string>

BA_ChooseTwoMakeOneQuiz* BA_ChooseTwoMakeOneQuiz::createQuiz()
{
    BA_ChooseTwoMakeOneQuiz* quiz=new BA_ChooseTwoMakeOneQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_ChooseTwoMakeOneQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_ChooseTwoMakeOneQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_object_list = json_game.FindMember("object_list")->value.GetInt();
    autoCheck = true;// json_game.FindMember("autoCheck")->value.GetBool();
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max = json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min = json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    value_object_max = json_object.FindMember("value")->value.FindMember("maximum")->value.GetInt();
    value_object_min = json_object.FindMember("value")->value.FindMember("minimum")->value.GetInt();
    if (value_object_min<3) {
        value_object_min=3;
    }
    if (value_object_max<value_object_min) {
        value_object_max = value_object_min;
    }
    object_size = getSizeJson(json_object, "size");
    result_size = getSizeJson(json_object, "result_size");
    
    rapidjson::Value& json_icon_config=json_data.FindMember("icon_config")->value;
    std::string filter = json_icon_config.FindMember("filter")->value.GetString();
    std::string bg_prop = json_icon_config.FindMember("bg_prop")->value.GetString();
    std::string bg_result_prop = json_icon_config.FindMember("bg_result_prop")->value.GetString();
    
    number_object = 4;// cocos2d::random(number_object_min, number_object_max);
    number_right = 2;//(int) number_object/2;
    
    //----------------------------------
    //Gen objects
    icon_list = getImageList("icons");
    
    auto bg_result_list= math::resource::ResHelper::filterImages(icon_list , filter, cocos2d::Value(bg_result_prop));
    auto bg_list=math::resource::ResHelper::filterImages(icon_list, filter, cocos2d::Value(bg_prop));

    for(int i = 0; i < number_object_list; i++) {
        std::vector<Math_Object*> objects;
        std::vector<int> value_list = createValueList();
        
        int random = rand()%bg_result_list.size();
        int rand_img = rand()%bg_list.size();
        
        Math_Object* result = mpool->getObject();
        result->setEnable(0, math::common::deny,bg_result_list[random] , nullptr);
        result->setValueText(cocos2d::StringUtils::toString(value_list[0]));
        result->setObjectSize(result_size);
        result->showValueTextWithAnimation(false);
        objects.push_back(result);
        
        for(int j = 0; j < number_object ; j++){
            Math_Object* object = mpool->getObject();
            object->setEnable(obj_id++, math::common::touch, bg_list[rand_img], nullptr);
            object->setValueText(cocos2d::StringUtils::toString(value_list[j+1]));
            object->showValueTextWithAnimation(false);
            //list id
            object->setTag(i);
            object->setObjectSize(object_size);
            object->setTouchCallback(CC_CALLBACK_1(BA_ChooseTwoMakeOneQuiz::onTouchedObject, this));
            objects.push_back(object);
        }
        object_list.push_back(objects);
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, bg_list);
    }
}


void BA_ChooseTwoMakeOneQuiz::onStart() {
    
    quizBegan(0, true);
    
    //----------------------------------
    // set position lists
    float line_height = (quiz_size.height - getQuestion()->getContentSize().height-40)/((int)object_list.size());
    
    for(int i = 0; i < object_list.size(); i++) {
        auto ret_obj = object_list[i].front();
        std::vector<Math_Object*> list_coms;
        for(int i2=1; i2<object_list[i].size(); i2++){
            list_coms.push_back(object_list[i][i2]);
        }        
        math::func::setPositionList(list_coms, (int) list_coms.size() , cocos2d::Rect(quiz_size.width*0.25+20, 10 + line_height*i , quiz_size.width*0.75-40, line_height));
        ret_obj->setPosition(cocos2d::Vec2(quiz_size.width*0.125, 10 + line_height*(i+0.5)));
        for (Math_Object* object : object_list[i]) {
            addChildNode(object);
            object->onShow(0.1*i);
        }
    }
    
    auto line_x = quiz_size.width * 0.25;
    auto line = cocos2d::ui::ImageView::create(getUIPath("common/line/line_1.png"));
    line->setPosition(cocos2d::Vec2(line_x, (quiz_size.height - getQuestion()->getContentSize().height-20) * 0.5));
    addChildNode(line);
    line->setOpacity(0);
    line->runAction(cocos2d::FadeIn::create(0.5));
    
    ui_elements.push_back(line);
}

//--------------------------------------
//start Question
void BA_ChooseTwoMakeOneQuiz::startQuestion() {
    enableControlQuiz();
}

//--------------------------------------
//BA data creation
std::vector<int> BA_ChooseTwoMakeOneQuiz::createValueList(){
    
    std::vector<int> value_list;
    std::pair<int, std::vector<int>> pair;
    std::vector<int> right_answer;
    
    int result = cocos2d::random(value_object_min, value_object_max);
    
    while (value_list.empty()) {
        for (int i=0; i<3; i++) {
            value_list.push_back(cocos2d::random(1, value_object_max));
        }
        for (int i1=0; i1<value_list.size()-1; i1++) {
            for (int i2 = i1+1; i2<value_list.size(); i2++) {
                if (value_list[i1] + value_list[i2] == result) {
                    value_list.crend();
                    break;
                }
            }
        }    
    }
    
    
    
    int stNumber = cocos2d::random(1, result);
    int ndNumber = result-stNumber;
    
    right_answer.push_back(stNumber);
    right_answer.push_back(ndNumber);
    
    pair.first = result;
    pair.second= right_answer;
    answers_lists.push_back(pair);
    
    value_list.push_back(result);
    value_list.push_back(stNumber);
    value_list.push_back(ndNumber);
    
    for (int i = 0; i < number_object - number_right; i++){
        int wrong_number = cocos2d::random(1, value_object_max);
        
        if(checkSameValue(wrong_number, value_list) == true){
            i--;
        }else {
            value_list.push_back(wrong_number);
        }
    }
    
    
    
    return value_list;
}

//--------------------------------------
//BA data creation
bool BA_ChooseTwoMakeOneQuiz::checkSameValue(int number, std::vector<int> value_list){
    for(int i = 0; i < value_list.size(); i++){
        if(number == value_list[i]){
            return true;
        }
        if(i > 0){
            if(value_list[0] == number + value_list[i]){
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------
//BA logic processing
int BA_ChooseTwoMakeOneQuiz::checkSelectedObject(int i){
    int count =0;
    
    for(Math_Object* obj : object_list[i]){
        if(obj->isSelected() == true){
            count ++;
        }
    }
    return count;
}

//--------------------------------------
//BA logic processing
void BA_ChooseTwoMakeOneQuiz::lockObj(int i, bool locked){
    for(Math_Object* obj: object_list[i]){
        if(obj -> isSelected() == false){
            obj->setLocked(locked);
        }
    }
}

#pragma mark - object touch events callback
void BA_ChooseTwoMakeOneQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = (Math_Object*) node;
    bool selected = false;
    bool locked = false;
    
    if(object->isSelected() == false){
        selected = true;
    }
    
    object->setSelected(selected);
    CCLOG("selected");
    
    int count = checkSelectedObject(object->getTag());
    
    //-----------------------
    //check inline
    if(autoCheck){
        if (count == number_right) {
            checkInline(object);
        }
        else{
            lockObj(object->getTag(), locked);
        }
    }else{
        if (count == number_right) {
            locked = true;
        }
        lockObj(object->getTag(), locked);
    }
}

//--------------------------------------
//check inline
void BA_ChooseTwoMakeOneQuiz::checkInline(Math_Object *object){
    if(checkResult(object->getTag()) != true){
        for(Math_Object* obj : object_list[object->getTag()]){
            
            if(obj->isSelected() == true){
                obj->setLocked(false);
                obj->onTouched();
                obj->setSelected(false);
            }
        }
    }else {
        autoCountList++;
        for(Math_Object* obj : object_list[object->getTag()]){
            obj->setLocked(true);
        }
        
        if(autoCountList == number_object_list){
            this->onExplanation(true);
            //CCLOG("%i", autoCountList);
            
        }
    }
}

void BA_ChooseTwoMakeOneQuiz::onAnswerDone(bool correct)
{
    disableControlQuiz();
    int count, countList =0;
    bool done  = false;
    
    for(int i = 0; i < number_object_list; i++){
        count = checkSelectedObject(i);
        if(count == number_right){
            if (checkResult(i) == true){
                countList ++;
                
            }
        }
    }
    
    if(countList == number_object_list){
        done = true;
    }
    
    this->onExplanation(done);
    _answers.at(0)->onAnimationClick(done);
    
}

bool BA_ChooseTwoMakeOneQuiz::checkResult(int i){
    
    int sum = 0;
    int check = 0;
    
    std::vector<Math_Object*> obj_list = object_list[i];
    for(Math_Object* obj : obj_list){
        if(obj->isSelected() == true){
            sum += atoi(obj->getValueText().c_str());
            check++;
        }
        
        if(check == number_right){
            if(sum == answers_lists[i].first){
                return true;
            }
        }
    }
    return false;
}

void BA_ChooseTwoMakeOneQuiz::onExplanation(bool correct) {
    std::vector<int> check;
    if (!correct){
        for(int i = 0; i < number_object_list; i++ ){
            int count =0;
            
            for(Math_Object* obj : object_list[i]){
                if(obj->isSelected()){
                    for(int k : answers_lists[i].second){
                        if(atoi(obj->getValueText().c_str()) == k){
                            count++;
                        }
                    }
                }
            }
            if(count !=2){
                check.push_back(i);
            }
        }
        
        for(int k : check){
            for(Math_Object* obj : object_list[k]){
                if(obj->isSelected()){
                    obj->onTouched();
                }

            }
        }

        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create([=] {
            enableControlQuiz();
        }), nullptr));
        
    }
    else {
        nextQuestion(0.5);
    }
    
}

void BA_ChooseTwoMakeOneQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    
    float delay = 0;
    for(std::vector<Math_Object*> Obj_list: object_list){
        for(Math_Object* obj: Obj_list){
            obj->onHide(delay);
            delay+= 0.1;
        }
    }
        
    for (auto ui_element : ui_elements) {
        ui_element->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            ui_element->removeFromParentAndCleanup(true);
        })));
    }
    
    quizEnded(0.5, true);
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_ChooseTwoMakeOneQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_ChooseTwoMakeOneQuiz::autoTestingRun(float delaytime)
{
    
    autoCheck = false;
    
    Math_Object* object = (Math_Object*) math::func::getRandomNode(object_list[rand()%object_list.size()]);
    
    
    while (object->getTouch() == math::common::deny || object->isLocked()== true || object->isSelected()== true) {
        
        object = math::func::getRandomNode(object_list[rand()%object_list.size()]);
    }
    
    
    onTouchedObject(object);
    
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}



