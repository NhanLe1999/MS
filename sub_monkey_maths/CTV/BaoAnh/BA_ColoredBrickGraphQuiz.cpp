//
//  BA_ColoredBrickGraphQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 10/9/17.
//

#include "BA_ColoredBrickGraphQuiz.hpp"
#include <algorithm>

BA_ColoredBrickGraphQuiz* BA_ColoredBrickGraphQuiz::createQuiz()
{
    BA_ColoredBrickGraphQuiz* quiz=new BA_ColoredBrickGraphQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_ColoredBrickGraphQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_ColoredBrickGraphQuiz::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);
    
//    _game = game;
//    _game_path = math_statistic->getGamePath(_game);

    Math_Quiz::initQuiz(game);
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_kind = json_object.FindMember("kind")->value.GetInt();
    display_mode = json_object.FindMember("display_mode")->value.GetBool();
    number_object_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    number_object_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();

    
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    

    //TODO: Chon icon
    //lay tat ca icon trong media_set
  //  std::string icon_filter = "name";
    img_quantity  = getImageList("icons", number_kind+1, "color");

    //random so luong moi loai obj
    std::vector<int> quantity;
    while(quantity.size() < number_kind){
        bool add = true;
        int number = cocos2d::random(number_object_min, number_object_max);
        for (int j = 0; j < quantity.size(); j++) {
            
            if (quantity[j] == number) {
                add = false;
                break;
            }
        }
        if (add){
            quantity.push_back(number);
        }
    }
 

    //sap xep so luong tu lon den nho
    std::sort(quantity.begin(), quantity.end());
    std::reverse(quantity.begin(), quantity.end());
    
    //tao list img voi loai icon va so luong moi loai
    std::pair<math::resource::Image*,  int> img_number;
    
    for(int i = 0; i < quantity.size(); i++){
        int number = quantity[i];
        img_number.second = number ;
        img_number.first = img_quantity[i];
        img_number_list.push_back(img_number);
    }
    
    right_color = img_number_list.back().first->getText("color");
 
    //gen obj
    for(int i = 0; i <img_number_list.size(); i++){
        std::vector<Math_Object*> objects;
        for(int j = 0; j < img_number_list[i].second; j++){
            Math_Object* object = Math_Pool::getInstance()->getObject();
            object->setEnable(obj_id, math::common::deny, img_quantity[i], NULL);
            object->setObjectSize(object_size);
            objects.push_back(object);
            obj_id++;
            _objects.push_back(object);
        }
        object_list.push_back(objects);
    }
    
    //parsing question
    auto number_imglist = getNumbers(number_object_min, number_object_max);
    this->parseQuestion(true, number_kind);
    auto opt_keys = getQuestion()->getOptionalKeys();
  
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : number_imglist) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : number_imglist) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_quantity);
        }
    }
    
    //----------------------------------
    //Gen answers
    std::string obj_key;
    
    // right answer value list
    for(int i = 0 ; i < getQuestion()->getNumberQuestions(); i++){
        if(i < 3){
            count =0;
            for(Math_Object* obj: _objects){
                obj_key = obj->getComparisionValue().color;
                if(obj_key == img_quantity[i]->getText ("color") ){
                    count++;
                }
            }
             answers_list.push_back(count);
            
//        }else if (i == 3){
//            count = 0;
//            for(int j = 0 ; j < opt_keys.size(); j++){
//                for(int k = 0; k < img_number_list.size(); k++){
//                    if(img_number_list[k].first->getText("color") == opt_keys[j]){
//                        count += img_number_list[k].second;
//
//                    }
//                }
//            }
//
//            answers_list.push_back(count);
//        }else if(i == 4){
//            count = 0;
//            int temp;
//            for(int j = 0 ; j < opt_keys.size(); j++){
//                for(int k = 0; k < img_number_list.size(); k++){
//                    if(img_number_list[k].first->getText("color") == opt_keys[0]){
//                        temp = img_number_list[k].second;
//                    }else if(img_number_list[k].first->getText("color") == opt_keys[2]) {
//                        count = img_number_list[k].second - temp;
//                    }
//                }
//            }
//            answers_list.push_back(count);
//            CCLOG("%i",count);
//        }else{
//            for(int i = 0; i < 4; i++){
//
//                answer_list.push_back(img_quantity[i]->getText("color"));
//            }
//            //answer_list.push_back(img_quantity[]->getText("color"));
        }
    }
    
    
    //create pair question index - answer list
    for(int i = 0; i < answers_list.size(); i++){
        std::pair<int, std::vector<int>> answer_pair;
        answer_pair.first = i;
        answer_pair.second.push_back(answers_list[i]);
        
        //create wrong answers
        while(answer_pair.second.size()<4){
            bool add = true;
            int delta = cocos2d::random(1, 3);
            int min = answers_list[i] - delta;
            int max = answers_list[i] + delta;
            int fake_ans ;
            if(answers_list[i] > delta){
                fake_ans = rand()%(max-min)+min;
            }else{
                fake_ans = max;
            }
            
            for(int j : answer_pair.second){
                if(j == fake_ans){
                    add = false;
                }
            }
            
            if(add){
                answer_pair.second.push_back(fake_ans);
            }
        }
        
        answer_vector.push_back(answer_pair);
    }
    
}

void BA_ColoredBrickGraphQuiz::onStart() {
    CCLOG("%s override", __func__);
    
    //get text of number
    std::vector<std::pair<int, std::vector<std::string>>> number_name_list;
    
    if(display_mode){
        auto number_text = getNumbers(0, 20);
        
        for (int i = 0; i < answer_vector.size(); i++) {
            std::pair<int, std::vector<std::string>> number_name;
            number_name.first = i;
            for(int j = 0; j < answer_vector.at(i).second.size(); j++){
                std::string name;
                // CCLOG("%i", temp);
                for(auto text : number_text){
                    
                    if( answer_vector.at(i).second.at(j) == text->getComparisionValue("number")){
                        name = text->getText("name_1", m_lang->getLanguage());
                        break;
                    }
                    
                }
                
                number_name.second.push_back(name);
            }
            number_name_list.push_back(number_name);
        }
    }
    //---------------------------
    //gen math_answers
    for(int i = 0; i < answer_vector.size(); i++){
        std::pair<int, std::vector<Math_Answer*>> answer_pair;
        answer_pair.first = i;
        for(int j = 0; j < 4; j++){
            Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
            if(!display_mode ){
                ans->setEnable(cocos2d::StringUtils::toString(answer_vector[i].second[j]), math::common::answer, CC_CALLBACK_1(BA_ColoredBrickGraphQuiz::onTouchedAnswer, this), this->getAnswerSize(0));
            }else{
                ans->setEnable(number_name_list[i].second[j], math::common::answer, CC_CALLBACK_1(BA_ColoredBrickGraphQuiz::onTouchedAnswer, this), this->getAnswerSize(0));
            }
            
            if(j == 0){
                ans->setCorrect(true);
            }
            
            answer_pair.second.push_back(ans);
        }
        answer_btn_vector.push_back(answer_pair);
    }
    
    std::pair<int, std::vector<Math_Answer*>> answer_pair;
    answer_pair.first = (int) answer_btn_vector.size();
    for(int i = 0; i < answer_list.size(); i++){
        
        Math_Answer* ans = Math_Pool::getInstance()->getAnswer();
        ans->setEnable(answer_list[i], math::common::answer, CC_CALLBACK_1(BA_ColoredBrickGraphQuiz::onTouchedAnswer, this), this->getAnswerSize(0));
        
        if( answer_list[i] == right_color){
            ans->setCorrect(true);
        }
        answer_pair.second.push_back(ans);
        
    }
    answer_btn_vector.push_back(answer_pair);
    //---------------------------------
    //Gen question
    onShowRightBar(0, 250.0/quiz_size.width);
    //    onShowBottomBar();
    showQuestionBox(0);

    for (int i = 0; i < object_list.size(); i++){
        for(int j = 0; j < object_list.at(i).size(); j++){
            auto obj = object_list.at(i).at(j);
            cocos2d::Vec2 pos (quiz_size.width*0.4 - object_size.width*number_kind/2 + i* object_size.width*1.3, (quiz_size.height-getQuestion()->getContentSize().height - object_size.height*0.9*img_number_list[0].second)/2 + (j * object_size.height*0.9 ));
            obj->setPosition(pos);
            addChildNode(obj,3);
            obj->onShow(0);
            
        }
    }
}


//--------------------------------------
//start Question
void BA_ColoredBrickGraphQuiz::startQuestion() {
    CCLOG("%s override", __func__);
    //Bat dau cau hoi
 
    int index = getQuestion()->getData().index;
    
    //----------------------------------
    //create answers for each question
    _answers.clear();

    for(int i = 0; i < 4; i++){
        Math_Answer* ans = answer_btn_vector.at(index).second.at(i);
        addChildNodeToRightPanel(ans,3);
        _answers.push_back(ans);
    }
    
//    ---------------------------------
//    show answers
    math::func::shuffle(_answers);
    math::func::setPositionList(_answers, 1, cocos2d::Rect(quiz_size.width* 0, quiz_size.height* 0.1, 250.0, quiz_size.height* 0.8));

    int i = 0;
    for(Math_Answer* ans: _answers){
        ans->onShowInParent(_panel_right, 2, i*0.1, math::animation::SlideLeft) ;
        i++;
    }
    
    
}

void BA_ColoredBrickGraphQuiz::onTouchedAnswer(cocos2d::Node* node)
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


void BA_ColoredBrickGraphQuiz::onExplanation(bool correct) {
    
    if (!correct){
        
        
    }
    else {
        
        for(Math_Answer* ans: _answers){
            if (ans->isEnable()) {
                ans->setDisable();
            }
        }
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=](){
            this->nextQuestion();
        }), NULL));
        
    }
    
}

void BA_ColoredBrickGraphQuiz::onComplete() {
    CCLOG("%s override", __func__);
    
    //TODO: Thuc hien them cac lenh o day
    
    float delay = showConratulation(0.3);
    std::reverse(_objects.begin(), _objects.end());
    for(Math_Object* obj: _objects){
        obj->onHide(delay);
        delay+= 0.1;
    }

    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_ColoredBrickGraphQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void BA_ColoredBrickGraphQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}
