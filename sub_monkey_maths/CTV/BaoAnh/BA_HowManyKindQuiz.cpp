//
//  BA_HowManyKindQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 10/8/17.
//

//status: Completed

#include "BA_HowManyKindQuiz.hpp"

BA_HowManyKindQuiz* BA_HowManyKindQuiz::createQuiz()
{
    BA_HowManyKindQuiz* quiz=new BA_HowManyKindQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_HowManyKindQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_HowManyKindQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_kind = randomRangeJson(json_game, "object_kind");
    
    total_object = randomRangeJson(json_game, "total_number");
    bool one_question = getJsonBool(json_game, "type", true);
    compare_key = getJsonString(json_game, "compare_key", "none");
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 6);
    number_object_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 6);
    object_size = getSizeJson(json_object, "size");
    
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = getJsonString(dislay_json, "type", "line");
    max_in_line = getJsonInt(dislay_json, "max_line", 4);

    int number_obj = 0;
    while (number_object_min * number_kind >  total_object) {
        number_kind--;
    }
    while(number_list.size() < number_kind) {
        if (number_object_min+number_obj > total_object) {
            number_list.clear();
        }
        int number = cocos2d::random(number_object_min, number_object_max);
        if (number_obj + number < total_object) {
            number_list.push_back(number);
        }
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    rapidjson::Value& icon_json = json_data.FindMember("icon")->value;
    std::string icon_filter = getJsonString(icon_json, "different", "id");
    std::string icon_same = getJsonString(icon_json, "same", "none");

    auto all_images = getImageList("icons");
    
    //dành riêng cho game hỏi cố định 1 loại obj
    if(compare_key != "none"){
        std::string key_cmp = (compare_key == "sym") ? "kind" : "name_1";
        for(math::resource::Image* key: all_images){
            if(key->getText(key_cmp) == compare_key){
                kind_list.push_back(key);
                break;
            }
        }
        while (kind_list.size() < number_kind) {
            auto img = math::func::getRandomNode(all_images);
            if (img->getText(key_cmp) != compare_key) {
                kind_list.push_back(img);
            }
        }
        
    }else{
        kind_list = getImageList(all_images, number_kind, icon_filter, icon_same);
    }
    
    img_list = getImageListNumber(all_images, kind_list, number_list, icon_filter);
    math::func::shuffle(img_list);
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    if (one_question) {
        parseQuestion();
    } else {
        parseQuestion(true, number_kind);
    }
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, kind_list);
    }
    
    //----------------------------------
    //Gen objects
    for(int i = 0 ; i < img_list.size(); i++){
        Math_Object* object = mpool->getObject();
        object->setEnable(i, math::common::deny, img_list[i], NULL);
        object->setObjectSize(object_size);
        object->setComparisionValue();
        _objects.push_back(object);
    }

    //----------------------------------
    //Gen answers
    std::string obj_name;
    
    // right answer value list
    for(int i = 0 ; i < number_kind; i++){
        answer_list.push_back(number_list[i]);
    }
    std::vector<math::resource::Image*> answer_list_img;
    for (int ans_val : answer_list) {
        auto img = math::number::getNumber(ans_val);
        answer_list_img.push_back(img);
    }
    
    //create pair question index - answer list
    for(int i = 0; i < answer_list.size(); i++){
        std::pair<int, std::vector<math::resource::Image*>> answer_pair;
        answer_pair.first = i;
        answer_pair.second.push_back(answer_list_img[i]);
        
        //create wrong answers
        while(answer_pair.second.size()<4){
            bool add = true;
            int delta = cocos2d::random(1, 3);
            int min = answer_list[i] - delta;
            int max = answer_list[i] + delta;
            int fake_ans ;
            if(answer_list[i] > delta){
                fake_ans = rand()%(max-min)+min;
            }else{
                fake_ans = max;
            }
            for(math::resource::Image* j : answer_pair.second){
                if(j->getComparisionValue("number") == fake_ans){
                    add = false;
                }
            }
            if(add){
                auto fake_img = math::number::getNumber(fake_ans);
                answer_pair.second.push_back(fake_img);
            }
        }
        answer_vector.push_back(answer_pair);
    }
    
    //---------------------------
    //gen math_answers
    for(int i = 0; i < answer_vector.size(); i++){
        for(int j = 0; j < 4; j++){
            Math_Answer* ans = mpool->getAnswer();
            ans->setEnable("", math::common::answer, CC_CALLBACK_1(BA_HowManyKindQuiz::onTouchedAnswer, this), cocos2d::Size(140, 140));
            ans->setIcon(answer_vector[i].second[j]);
            ans->setTitleWithIconProp("number");
            if(j == 0){
                ans->setCorrect(true);
            }
            ans->getValueText()->enableOutline(cocos2d::Color4B::WHITE, 4);
            _answers.push_back(ans);
        }
    }    
}

void BA_HowManyKindQuiz::onStart() {
    startGuide();
    float bar_width = 250.0/quiz_size.width;
    onShowRightBar(0, bar_width);
    quizBegan(0, false);
    auto rect_box = cocos2d::Rect(10, 10, quiz_size.width - 270, quiz_size.height-20);
    
    math::func::shuffle(_objects);
    if (display_type.compare("line")==0) {
        math::func::setPositions(_objects, rect_box);
    } else {
        math::func::setPositionsRandom(_objects, rect_box);
    }
    for (int i=0; i<_objects.size(); i++) {
        _objects[i]->fitObjectSize();
        addChildNode(_objects[i], 2);
    }
    randomShow(_objects);
}

//--------------------------------------
//start Question
void BA_HowManyKindQuiz::startQuestion() {
    enableControlQuiz();
    
    answers.clear();
    for(int i = 0; i < 4; i++){
        Math_Answer* ans = _answers[i];
        answers.push_back(ans);
    }
    _answers.erase(_answers.begin(), _answers.begin()+4);
    math::func::shuffle(answers);
    math::func::setPositions(answers, cocos2d::Rect(30, 30, 190, quiz_size.height-60));
    for(int i=0; i<answers.size(); i++){
        answers[i]->onShowInParent(_panel_right, 2, i*0.1, math::animation::SlideLeft);
    }
}

void BA_HowManyKindQuiz::onTouchedAnswer(cocos2d::Node* node) {
    Math_Answer* answer = (Math_Answer*) node;
    bool done = answer->isCorrect();
    if (!done) {
        GuideCenter->increamentIncorrect();
    }
    answer->onAnimationClick(done);
    answer->playAnswerAudio("name_1", 0.2);
    this->onExplanation(done);
}

void BA_HowManyKindQuiz::onExplanation(bool correct) {
    if (!correct){
        disableControlQuiz(0.25);
    }
    else {
        disableControlQuiz();
        if (getQuestion()->getData().index < getQuestion()->getNumberQuestions()-1) {
            if (!answers.empty()) {
                for(int i=0; i<answers.size(); i++){
                    answers[i]->setDisable(0.25 + i*0.1, math::animation::SlideLeft);
                }
            }
            this->nextQuestion(0.75);
        } else {
            this->nextQuestion(0.5);
        }
    }
}

void BA_HowManyKindQuiz::onComplete() {
    for(int i=0; i<answers.size(); i++){
        answers[i]->setDisable(i*0.1, math::animation::SlideLeft);
    }
    if (number_object <= 5) {
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->onHide(i*0.1);
        }
    } else {
        std::vector<int> random_hide;
        for (int i=0; i<_objects.size(); i++) {
            random_hide.push_back(i);
        }
        int number_div = ((int)_objects.size()+4)/5;
        if (number_div>1) {
            math::func::shuffle(random_hide);
        }
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->onHide((random_hide[i]/number_div)*0.1);
        }
    }
    quizEnded(0.5, 0);
}

void BA_HowManyKindQuiz::onGuideStarted() {
    disableControlQuiz();
    for (Math_Answer* answer : answers) {
        if (answer->isCorrect()) {
            GuideCenter->guideByTouch(answer->getWorldPosition());
            return;
        }
    }
    GuideCenter->guideNone();
}

void BA_HowManyKindQuiz::onGuideDone() {
    enableControlQuiz();
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_HowManyKindQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_HowManyKindQuiz::autoTestingRun(float delaytime)
{
    
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    
    autoTesting();
    
}
