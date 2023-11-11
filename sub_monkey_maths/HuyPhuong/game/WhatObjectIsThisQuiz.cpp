//
//  WhatObjectIsThisQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "WhatObjectIsThisQuiz.h"

WhatObjectIsThisQuiz* WhatObjectIsThisQuiz::createQuiz()
{
    WhatObjectIsThisQuiz* quiz=new WhatObjectIsThisQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool WhatObjectIsThisQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void WhatObjectIsThisQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    game_type = getJsonString(json_game, "game_mode", "yes_no");
    std::string str_rotation = getJsonString(json_game, "rotation", "none");
    rotation = str_rotation.compare("none") != 0;
    random_size = getJsonBool(json_game, "random_size", false);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    background_key = json_object.FindMember("background")->value.GetString();
    fit_object = json_object.FindMember("fit_size")->value.GetString();
    display = json_object.FindMember("display")->value.FindMember("type")->value.GetString();
    max_in_line = json_object.FindMember("display")->value.FindMember("max_line")->value.GetInt();
    number_object = cocos2d::random(json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt(), json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt());
    if (number_object==1) {
        random_size = false;
    }
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_dif = getJsonString( getJsonMember(json_data, "icon") , "different", "id");
    std::string icon_same = getJsonString( getJsonMember(json_data, "icon") , "same", "none");

    auto all_imgs = getImageList("icons");
    int number_answer=2;
    std::vector<math::resource::Image*> img_gens;
    if (game_type.compare("answer")==0) {
        number_answer = randomRangeJson(json_game, "number_answer");
        img_list = getImageList(all_imgs, number_answer, icon_dif, icon_same);
        for (math::resource::Image* img : img_list) {
            answer_list.push_back(img);
        }
        for (int i=0; i<number_answer; i++) {
            number_list.push_back(1);
        }
        img_gens = img_list;
    } else {
        answer_true = cocos2d::random(0, 1)==0;
        img_gens = getImageList(all_imgs, 2, icon_dif, icon_same);
        answer_list.push_back(getTextByTextGroup("yes"));
        answer_list.push_back(getTextByTextGroup("no"));
        if (!answer_true) {
            for (math::resource::Image* img : img_gens) {
                img_list.insert(img_list.begin(), img);
            }
        } else {
            img_list = img_gens;
        }
    }
    
    auto img_corect = img_gens.front();
    img_gens.clear();
    
    if (number_object > 1) {
        img_gens.push_back(img_corect);
    } else {
        std::vector<math::resource::Image*> imgs_same;
        if (icon_dif == "none") {
            for (int i=0; i<number_object; i++) {
                img_gens.push_back(img_corect);
            }
        } else {
            std::string same_val = img_corect->getText(icon_dif);
            for (math::resource::Image* img : all_imgs) {
                if (img->getText(icon_dif) == same_val && img != img_corect) {
                    imgs_same.push_back(img);
                }
            }
            while (imgs_same.size() < number_object-1) {
                imgs_same.push_back(math::func::getRandomNode(imgs_same));
            }
            math::func::shuffle(imgs_same);
            
            img_gens.push_back(img_corect);
            for (int i=0; i<number_object-1; i++) {
                img_gens.push_back(imgs_same[i]);
            }
        }
    }
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    for (int i=0; i<number_object; i++) {
        if (random_size) {
            if (i==0) {
                size_list.push_back(0);
            } else {
                size_list.push_back(cocos2d::random(0, 2));
            }
        } else {
            size_list.push_back(0);
        }
        
        if (rotation) {
            rotate_list.push_back(30*cocos2d::random(-3, 3));
        } else {
            rotate_list.push_back(0);
        }
    }

    //TODO: GENERATE QUIZ COMPONENTS
    img_bg = nullptr;
    
    for (int i=0; i<number_object; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, img_gens[i], img_bg);
        object->setComparisionValue(size_list[i]);
        object->setObjectSize(object_size);
        object->getImage()->setRotation(rotate_list[i]);
        if (fit_object.compare("width")==0) {
            object->fixWidthObject();
        } else if (fit_object.compare("height")==0) {
            object->fixHeightObject();
        }
        _objects.push_back(object);
    }
    
    for (int i=0; i<number_answer; i++) {
        Math_Answer* answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(WhatObjectIsThisQuiz::onAnswered, this), getAnswerSize(2));
        answer->setIcon(answer_list[i]);
        answer->setTitleWithIcon("name_1");
        _answers.push_back(answer);
    }
    if (game_type.compare("yes_no")==0) {
        _answers[answer_true?0:1]->setCorrect(true);
    } else {
        _answers.front()->setCorrect(true);
    }
    math::func::shuffle(_answers);
}

void WhatObjectIsThisQuiz::onStart() {
    onShowBottomBar(0, 0.25, true);
    quizBegan(0, true);
    
    math::func::shuffle(_objects);
    if (display.compare("line")==0) {
        math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.75-20-getQuestion()->getContentSize().height), 100);
    } else {
        //random
        cocos2d::Rect rect_box = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.7-20-getQuestion()->getContentSize().height);
        auto list_points = randomPositionsInRect(object_size, rect_box, (int)_objects.size());
        for (int i=0; i<_objects.size(); i++) {
            _objects[i]->setPosition(list_points[i]);
        }
    }
    for (int i=0; i<_objects.size(); i++) {
        this->addChildNode(_objects[i]);
        _objects[i]->onShow(0.1*i, math::animation::ScaleUp);
    }

    fixAnswerSize(quiz_size.width * 0.3, quiz_size.width*0.2);
    float delta = (quiz_size.width - 80 - (int)_answers.size() * _answers.front()->getContentSize().width) / ((int)_answers.size() + 1);
    delta = delta > 100 ? 100 : delta;
    math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(40, 0, quiz_size.width-80, quiz_size.height*0.25-30), delta);
}

void WhatObjectIsThisQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (int i=0; i<_answers.size(); i++) {
            _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
        }
    }
}

void WhatObjectIsThisQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion(0.75);
    }
}

void WhatObjectIsThisQuiz::onComplete() {
         
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(0.1*i, math::animation::SlideUp);
    }
    
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
    
    quizEnded(0.5, true);
}

#pragma mark - object touch events callback
void WhatObjectIsThisQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    bool correct = answer->isCorrect();
    answer->onAnimationClick(correct);
    this->onExplanation(correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void WhatObjectIsThisQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void WhatObjectIsThisQuiz::autoTestingRun(float delaytime) {
    //Choose answer
    Math_Answer* answer;
    bool choose_ans=true;
    while (choose_ans) {
        choose_ans=false;
        answer=math::func::getRandomNode(_answers);
        for (Math_Answer* _ans : answer_tested) {
            if (answer==_ans) {
                choose_ans=true;
            }
        }
    }
    answer_tested.push_back(answer);
    
    if (!answer->isCorrect()) {
        this->scheduleOnce([=](float dt) {
            this->autoTestingRun(delaytime);
        }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
    }
    answer->onAutoClick();
}

