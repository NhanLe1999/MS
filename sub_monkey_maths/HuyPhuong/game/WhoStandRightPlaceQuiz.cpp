//
//  WhoStandRightPlaceQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "WhoStandRightPlaceQuiz.h"

WhoStandRightPlaceQuiz* WhoStandRightPlaceQuiz::createQuiz()
{
    WhoStandRightPlaceQuiz* quiz=new WhoStandRightPlaceQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool WhoStandRightPlaceQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void WhoStandRightPlaceQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    std::vector<math::resource::Image*> img_question;
    
    auto all_imgs = getImageList("icons");
    while (img_list.size() < 2) {
        img_list.clear();
        int m_pair = math::func::getRandomNode(all_imgs)->getComparisionValue("pair");
        for (math::resource::Image* img : all_imgs) {
            if (img->getComparisionValue("pair") == m_pair) {
                img_list.push_back(img);
            }
        }
    }
    
    for (int i=0; i<img_list.size(); i++) {
        if (img_list[i]->getComparisionValue("role") == 0) {
            img_question.push_back(img_list[i]);
            img_list.erase(img_list.begin() + i);
            break;
        }
    }
    math::func::shuffle(img_list);
    for (int i=0; i<2; i++) {
        img_question.push_back(img_list[i]);
    }
    
    //Gen object
    object_size = cocos2d::Size(350, 350);
    auto object = mpool->getObject();
    object->setEnable(0, math::common::deny, img_question.front(), nullptr);
    object->setObjectSize(object_size);
    object->fixHeightObject();
    _objects.push_back(object);
    addChildNode(object);
    
    
    //Gen answer
    for (int i=1; i<3; i++) {
        auto ans = mpool->getAnswer();
        ans->setEnable("", math::common::icon_name, CC_CALLBACK_1(WhoStandRightPlaceQuiz::onTouchObject, this), getAnswerSize());
        ans->setIcon(img_question[i]);
        ans->setTitleWithIcon("name_1");
        if (i==1) {
            ans->setCorrect(true);
        }
        _answers.push_back(ans);
    }
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_question);
    }
}

void WhoStandRightPlaceQuiz::onStart() {
    onShowBottomBar(0, 0.3);
    quizBegan(0.5, true);
    
    _objects.front()->setPosition(cocos2d::Vec2(quiz_size.width*0.5, (quiz_size.height*1.3-questionHeight())*0.5));
    _objects.front()->onShow(0);
    
    fixAnswerSize(quiz_size.width * 0.3, quiz_size.width*0.2);
    math::func::shuffle(_answers);
    math::func::setPositions(_answers, cocos2d::Rect(40, 0, quiz_size.width - 80, quiz_size.height*0.3 - 30));
}

void WhoStandRightPlaceQuiz::startQuestion() {
    enableControlQuiz();
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, i*0.1, math::animation::SlideUp);
    }
}

void WhoStandRightPlaceQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void WhoStandRightPlaceQuiz::onComplete() {
    _objects.front()->onHide(0);
    
    for (int i=0; i < _dragers.size(); i++) {
        Math_Object* object = _dragers[i];
        object->onHide(i*0.1, math::animation::SlideUp);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void WhoStandRightPlaceQuiz::onTouchObject(Node* node) {
    disableControlQuiz(0.5);
    Math_Answer* object = static_cast<Math_Answer*>(node);
    bool correct = object->isCorrect();
    
    object->onAnimationClick(correct);
    if (correct) {
        for (Math_Object* obj : _objects) {
            obj->setLocked(true);
        }
        this->onExplanation(true);
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void WhoStandRightPlaceQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void WhoStandRightPlaceQuiz::autoTestingRun(float delaytime) {
    
}
