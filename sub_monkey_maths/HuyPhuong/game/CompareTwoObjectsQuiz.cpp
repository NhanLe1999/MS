//
//  CompareTwoObjectsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "CompareTwoObjectsQuiz.h"

CompareTwoObjectsQuiz* CompareTwoObjectsQuiz::createQuiz()
{
    CompareTwoObjectsQuiz* quiz=new CompareTwoObjectsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CompareTwoObjectsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CompareTwoObjectsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    rapidjson::Value& compare_list = json_game.FindMember("compare")->value;
    int index_compare = cocos2d::random(0, (int)compare_list.Size()-1);
    compare_type = compare_list[index_compare] .GetString();
    
    compare = getJsonString(json_game, "key", "id");
    sub_compare = getJsonString(json_game, "sub_key", "none");
    game_play = getJsonString(json_game, "game_play", "yes_no");
    align = getJsonBool(json_game, "align", true);
    re_order = getJsonBool(json_game, "reorder", true);
    
    rapidjson::Value& ans_data = json_game.FindMember("answer")->value;
    for (rapidjson::SizeType i=0; i<ans_data.Size(); i++) {
        answer_list.push_back(getTextByTextGroup(ans_data[i].GetString()));
    }
    if (game_play.compare("yes_no_clause")==0) {
        rapidjson::Value& clauses = json_game.FindMember("clause")->value;
        clause_string = clauses[index_compare].GetString();
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    background_key = json_object.FindMember("background")->value.GetString();
    fit_object = json_object.FindMember("fit_size")->value.GetString();
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    img_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    
    auto all_imgs = getImageList("icons");

    answer_true = cocos2d::random(0, 1)==0;
    if (compare_type.compare("equal")==0) {
        img_list = getImageList(all_imgs, 2, icon_filter, img_same);
        if (answer_true) {
            img_list.pop_back();
            img_list.push_back(img_list[0]);
        } else {
            if (compare.compare("size")==0) {
                if (cocos2d::random(0, 1)) {
                    size_list.push_back(0);
                    size_list.push_back(2);
                } else {
                    size_list.push_back(2);
                    size_list.push_back(0);
                }
            }
        }
    } else if (compare_type.compare("different")==0) {
        img_list = getImageList(all_imgs, 2, icon_filter, img_same);
        number_list.push_back(1);
        number_list.push_back(1);
        img_list = getImageListNumber(all_imgs, img_list, number_list, img_same);
        if (!answer_true) {
            img_list.pop_back();
            img_list.push_back(img_list[0]);
        } else {
            if (compare.compare("size")==0) {
                if (cocos2d::random(0, 1)) {
                    size_list.push_back(0);
                    size_list.push_back(2);
                } else {
                    size_list.push_back(2);
                    size_list.push_back(0);
                }
            }
        }
    } else if (compare_type.compare("less")==0) {
        number_list.push_back(1);
        number_list.push_back(1);
        img_list = getImageList(all_imgs, 2, icon_filter, img_same);
        if (answer_true) {
            std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                return a->getComparisionValue(compare) < b->getComparisionValue(compare);
            });
            if (compare.compare("size")==0) {
                size_list.push_back(2);
                size_list.push_back(0);
            }
        } else {
            std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                return a->getComparisionValue(compare) > b->getComparisionValue(compare);
            });
            if (compare.compare("size")==0) {
                size_list.push_back(0);
                size_list.push_back(2);
            }
        }
    } else if (compare_type.compare("more")==0) {
        number_list.push_back(1);
        number_list.push_back(1);
        img_list = getImageList("icons", 2, icon_filter, img_same);
        if (!answer_true) {
            std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                return a->getComparisionValue(compare) < b->getComparisionValue(compare);
            });
            
            if (compare.compare("size")==0) {
                size_list.push_back(2);
                size_list.push_back(0);
            }
        } else {
            std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                return a->getComparisionValue(compare) > b->getComparisionValue(compare);
            });
            if (compare.compare("size")==0) {
                size_list.push_back(0);
                size_list.push_back(2);
            }
        }
    } else {
    
    }
    
    if (size_list.size()==0) {
        size_list.push_back(0);
        size_list.push_back(0);
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue(compare_type == "more"?1:0);

    //TODO: GENERATE QUIZ COMPONENTS
    //img_bg = getBackground(background_key);
    
    for (int i=0; i<2; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, img_list[i], nullptr);
        object->setComparisionValue(size_list[i]);
        object->setObjectSize(object_size);
        if (fit_object.compare("width")==0) {
            object->fixWidthObject();
        } else if (fit_object.compare("height")==0) {
            object->fixHeightObject();
        }
        _objects.push_back(object);
    }
    
    for (int i=0; i<answer_list.size(); i++) {
        Math_Answer* answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(CompareTwoObjectsQuiz::onAnswered, this), getAnswerSize(2));
        answer->setIcon(answer_list[i]);
        answer->setTitleWithIcon("name_1");
        _answers.push_back(answer);
    }
    _answers[answer_true?0:1]->setCorrect(true);
}

void CompareTwoObjectsQuiz::onStart() {
    onShowBottomBar();
    Math_Quiz::onStart();
    
    if (game_play == "yes_no_clause") {
        cocos2d::Vec2 center = cocos2d::Vec2(quiz_size.width, quiz_size.height * 0.75 - getQuestion()->getContentSize().height-20) /2;
        
        clause = cocos2d::ui::Text::create(clause_string, "Maths_Bold.ttf", 48);
        clause->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        clause->setTextColor(cocos2d::Color4B::BLUE);
        clause->setPosition(center);
        addChildNode(clause, 3);
        clause->setOpacity(0);
        clause->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::FadeIn::create(0.5), NULL));
        
        float clause_width = clause->getContentSize().width;
        while (clause_width > quiz_size.width * 0.55) {
            clause->setFontSize(clause->getFontSize() - 2);
            clause_width = clause->getContentSize().width;
        }
        
        if (fit_object == "height") {
            auto object_size_max = _objects.front()->getContentSize();
            if (_objects.back()->getContentSize().width > object_size_max.width) {
                object_size_max = _objects.back()->getContentSize();
            }
            
            for (int i=0; i<_objects.size(); i++) {
                auto obj_size = _objects[i]->getContentSize();
                _objects[i]->setPosition(cocos2d::Vec2(center.x - (object_size_max.width - obj_size.width) / 2, center.y + (1-i*2) * object_size.height*0.9));
            }
            
        } else if (fit_object == "width") {
            auto object_size_max = _objects.front()->getContentSize();
            if (_objects.back()->getContentSize().height > object_size_max.height) {
                object_size_max = _objects.back()->getContentSize();
            }
            
            for (int i=0; i<_objects.size(); i++) {
                auto obj_size = _objects[i]->getContentSize();
                _objects[i]->setPosition(cocos2d::Vec2(center.x - (1-i*2) * (clause_width + 50)/2, center.y - (object_size_max.height - obj_size.height) / 2));
            }
        } else {
            for (int i=0; i<_objects.size(); i++) {
                _objects[i]->setPosition(cocos2d::Vec2(center.x - (1-i*2) * (clause_width + 50)/2, center.y));
            }
        }
    } else {
        math::func::shuffle(_objects);
        cocos2d::Vec2 center = cocos2d::Vec2(quiz_size.width, quiz_size.height * 0.75 - getQuestion()->getContentSize().height-20) /2;
        
        if (fit_object == "height") {
            auto object_size_max = _objects.front()->getContentSize();
            if (_objects.back()->getContentSize().width > object_size_max.width) {
                object_size_max = _objects.back()->getContentSize();
            }
            
            for (int i=0; i<_objects.size(); i++) {
                auto obj_size = _objects[i]->getContentSize();
                _objects[i]->setPosition(cocos2d::Vec2(center.x - (object_size_max.width - obj_size.width) / 2, center.y + (1-i*2) * object_size.height*0.7));
            }
        } else if (fit_object == "width") {
            auto object_size_max = _objects.front()->getContentSize();
            if (_objects.back()->getContentSize().height > object_size_max.height) {
                object_size_max = _objects.back()->getContentSize();
            }
            
            for (int i=0; i<_objects.size(); i++) {
                auto obj_size = _objects[i]->getContentSize();
                _objects[i]->setPosition(cocos2d::Vec2(center.x - (1-i*2) * object_size.width, center.y - (object_size_max.height - obj_size.height) / 2));
            }
        } else {
            math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height-20), 100);
        }
    }
    
    for (int i=0; i<2; i++) {
        this->addChildNode(_objects[i]);
        _objects[i]->onShow(0.1*i);
    }
    
    math::func::shuffle(_answers);
    math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(40, 0, quiz_size.width-80, quiz_size.height*0.25-30), 100);
}

void CompareTwoObjectsQuiz::startQuestion() {
     
    enableControlQuiz();
    for (int i = 0; i<_answers.size(); i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, i*0.1, math::animation::SlideUp);
    }
}

void CompareTwoObjectsQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}

void CompareTwoObjectsQuiz::onComplete() {
     
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }

    if (clause != nullptr) {
        clause->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5), cocos2d::FadeOut::create(0.2)));
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(0.5+i*0.1);
    }
    
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.0, "complete_quiz");
}

#pragma mark - object touch events callback
void CompareTwoObjectsQuiz::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void CompareTwoObjectsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CompareTwoObjectsQuiz::autoTestingRun(float delaytime) {
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

