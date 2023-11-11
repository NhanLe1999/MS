//
//  AnswerPositionOfObjectQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "AnswerPositionOfObjectQuiz.h"

AnswerPositionOfObjectQuiz* AnswerPositionOfObjectQuiz::createQuiz()
{
    AnswerPositionOfObjectQuiz* quiz=new AnswerPositionOfObjectQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool AnswerPositionOfObjectQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void AnswerPositionOfObjectQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string ask_pos = getJsonString(json_game, "position", "left_right");
    if (ask_pos == "left_right") {
        question_type = QT_Left_Right;
    } else if (ask_pos == "top_bottom") {
        question_type = QT_Top_Bottom;
    } else if (ask_pos == "next_to") {
        question_type = QT_NextTo;
    } else if (ask_pos == "above_below") {
        question_type = QT_Above_Below;
    } else {
        question_type = QT_UsePicture;
    }
    
    std::vector<math::resource::Image*> img_question;
    if (question_type == QT_UsePicture) {
        auto all_imgs = getImageList("icons");
        img_list.push_back(math::func::getRandomNode(all_imgs));
        img_question.push_back(img_list.front());
        
        //correct answer
        auto img_correct = math::resource::ResHelper::createImage("");
        std::string text_correct = img_list.front()->getText("kind");
        auto audios_correct = img_list.front()->getAudios("kind");
        img_correct->addProperty("name_1", text_correct, audios_correct);
        img_answer.push_back(img_correct);
        
        //Wrong answer
        auto img_wrong = math::resource::ResHelper::createImage("");
        std::string text_wrong = img_list.front()->getText("family");
        auto audios_wrong = img_list.front()->getAudios("family");
        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
        img_answer.push_back(img_wrong);
        
        auto object = mpool->getObject();
        object->setEnable(0, math::common::deny, img_list.front(), nullptr);
        object->setObjectSize(cocos2d::Size(400, 400));
        object->fixHeightObject();
        _objects.push_back(object);
        
    } else {
        number_object = randomRangeJson(json_game, "number_object");
        fix_ask_pos = getJsonInt(json_game, "fix_ask_pos", number_object);
        
        if (question_type == QT_Left_Right || question_type == QT_Top_Bottom) {
            if (number_object == 2) {
                if (fix_ask_pos >= number_object) {
                    fix_ask_pos = cocos2d::random(0, 1);
                }
            } else {
                if (fix_ask_pos == 3) {
                    fix_ask_pos = cocos2d::random(0, 1);
                } else if (fix_ask_pos>3) {
                    fix_ask_pos = cocos2d::random(0, 2);
                }
            }
        } else if (question_type == QT_Above_Below) {
            if (fix_ask_pos > 1) {
                fix_ask_pos = cocos2d::random(0, 1);
            }
        } else if (question_type == QT_NextTo) {
            number_object = 3;
            fix_ask_pos = cocos2d::random(0, 1);
        }
        
        rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
        std::string icon_same = getJsonString(json_icon, "same", "none");
        std::string icon_diff = getJsonString(json_icon, "different", "id");
        auto all_imgs = getImageList("icons");
        all_imgs = math::resource::ResHelper::filterImages(all_imgs, icon_same);
        
        object_size = cocos2d::Size(200, 200);
        if ((question_type == QT_Above_Below || question_type == QT_Top_Bottom) && number_object == 3) {
            object_size = cocos2d::Size(150, 150);
        }
        
        img_list = getImageList(all_imgs, number_object, icon_diff);
        for (int i2=0; i2 < number_object; i2++) {
            auto object = mpool->getObject();
            object->setEnable(i2, math::common::deny, img_list[i2], nullptr);
            object->setObjectSize(object_size);
            _objects.push_back(object);
        }
        
        if (question_type == QT_NextTo) {
            if (fix_ask_pos==0) {
                img_question.push_back(img_list[1]);
                img_question.push_back(img_list[0]);
                img_question.push_back(img_list[2]);
            } else if (fix_ask_pos == 1) {
                img_question.push_back(img_list[2]);
                img_question.push_back(img_list[1]);
                img_question.push_back(img_list[0]);
            }
            
            auto img_nextto = getTextByTextGroup("next to");
            
            //correct answer
            auto img_correct = math::resource::ResHelper::createImage("");
            std::string text_correct = cocos2d::StringUtils::format("%s %s", img_nextto->getText().c_str(), img_question[1]->getText().c_str());
            auto audios_correct = img_nextto->getAudios();
            for (std::string audio : img_question[1]->getAudios()) {
                audios_correct.push_back(audio);
            }
            img_correct->addProperty("name_1", text_correct, audios_correct);
            img_answer.push_back(img_correct);
            
            //Wrong answer
            auto img_wrong = math::resource::ResHelper::createImage("");
            std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_nextto->getText().c_str(), img_question[1]->getText().c_str());
            auto audios_wrong = img_nextto->getAudios();
            for (std::string audio : img_question[1]->getAudios()) {
                audios_wrong.push_back(audio);
            }
            img_wrong->addProperty("name_1", text_wrong, audios_wrong);
            img_answer.push_back(img_wrong);
            
        } else if (question_type == QT_Above_Below) {
            math::resource::Image* img_correct_pos = nullptr;
            math::resource::Image* img_wrong_post = nullptr;
            
            if (fix_ask_pos==0) {
                img_question.push_back(img_list[0]);
                img_question.push_back(img_list[1]);
                
                img_correct_pos = getTextByTextGroup("above");
                img_wrong_post = getTextByTextGroup("below");
            } else {
                img_correct_pos = getTextByTextGroup("below");
                img_wrong_post = getTextByTextGroup("above");
                
                if (number_object==2) {
                    img_question.push_back(img_list[1]);
                    img_question.push_back(img_list[0]);
                } else {
                    img_question.push_back(img_list[2]);
                    img_question.push_back(img_list[1]);
                }
            }
            
            //correct answer
            auto img_correct = math::resource::ResHelper::createImage("");
            std::string text_correct = cocos2d::StringUtils::format("%s %s", img_correct_pos->getText().c_str(), img_question[1]->getText().c_str());
            auto audios_correct = img_correct_pos->getAudios();
            for (std::string audio : img_question[1]->getAudios()) {
                audios_correct.push_back(audio);
            }
            img_correct->addProperty("name_1", text_correct, audios_correct);
            img_answer.push_back(img_correct);
            
            //Wrong answer
            auto img_wrong = math::resource::ResHelper::createImage("");
            std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_correct_pos->getText().c_str(), img_question[1]->getText().c_str());
            auto audios_wrong = img_correct_pos->getAudios();
            for (std::string audio : img_question[1]->getAudios()) {
                audios_wrong.push_back(audio);
            }
            img_wrong->addProperty("name_1", text_wrong, audios_wrong);
            img_answer.push_back(img_wrong);
        } else if (question_type == QT_Left_Right) {
            if (number_object == 2) {
                math::resource::Image* img_correct_pos = nullptr;
                math::resource::Image* img_wrong_post = nullptr;
                
                if (fix_ask_pos==0) {
                    img_question.push_back(img_list[0]);
                    img_question.push_back(img_list[1]);
                    
                    img_correct_pos = getTextByTextGroup("on the left");
                    img_wrong_post = getTextByTextGroup("on the right");
                } else {
                    img_question.push_back(img_list[1]);
                    img_question.push_back(img_list[0]);
                    
                    img_correct_pos = getTextByTextGroup("on the right");
                    img_wrong_post = getTextByTextGroup("on the left");
                }
                
                //correct answer
                img_answer.push_back(img_correct_pos);
                img_answer.push_back(img_wrong_post);
                
//                auto img_correct = math::resource::ResHelper::createImage("");
//                std::string text_correct = cocos2d::StringUtils::format("%s %s", img_correct_pos->getText().c_str(), img_question[1]->getText().c_str());
//                auto audios_correct = img_correct_pos->getAudios();
//                for (std::string audio : img_question[1]->getAudios()) {
//                    audios_correct.push_back(audio);
//                }
//                img_correct->addProperty("name_1", text_correct, audios_correct);
//                img_answer.push_back(img_correct);
//
//                //Wrong answer
//                auto img_wrong = math::resource::ResHelper::createImage("");
//                std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_wrong_post->getText().c_str(), img_question[1]->getText().c_str());
//                auto audios_wrong = img_wrong_post->getAudios();
//                for (std::string audio : img_question[1]->getAudios()) {
//                    audios_wrong.push_back(audio);
//                }
//                img_wrong->addProperty("name_1", text_wrong, audios_wrong);
//                img_answer.push_back(img_wrong);
            } else {
                math::resource::Image* img_correct_pos = nullptr;
                math::resource::Image* img_wrong_post = nullptr;
                math::resource::Image* img_middle = getTextByTextGroup("in the middle");
                
                if (fix_ask_pos < 2) {
                    if (fix_ask_pos==0) {
                        img_question.push_back(img_list[0]);
                        img_question.push_back(img_list[1]);
                        img_question.push_back(img_list[2]);
                        
                        img_correct_pos = getTextByTextGroup("on the left");
                        img_wrong_post = getTextByTextGroup("on the right");
                    } else if (fix_ask_pos == 1) {
                        img_question.push_back(img_list[2]);
                        img_question.push_back(img_list[1]);
                        img_question.push_back(img_list[0]);
                        
                        img_correct_pos = getTextByTextGroup("on the right");
                        img_wrong_post = getTextByTextGroup("on the left");
                    }
                    
                    //correct answer
//                    auto img_correct = math::resource::ResHelper::createImage("");
//                    std::string text_correct = cocos2d::StringUtils::format("%s %s", img_correct_pos->getText().c_str(), img_question[1]->getText().c_str());
//                    auto audios_correct = img_correct_pos->getAudios();
//                    for (std::string audio : img_question[1]->getAudios()) {
//                        audios_correct.push_back(audio);
//                    }
//                    img_correct->addProperty("name_1", text_correct, audios_correct);
                    img_answer.push_back(img_correct_pos);
                    
                    //Wrong answer
                    if (cocos2d::random(0, 2) < 2) {
//                        auto img_wrong = math::resource::ResHelper::createImage("");
//                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_wrong_post->getText().c_str(), img_question[1]->getText().c_str());
//                        auto audios_wrong = img_wrong_post->getAudios();
//                        for (std::string audio : img_question[1]->getAudios()) {
//                            audios_wrong.push_back(audio);
//                        }
//                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
                        img_answer.push_back(img_wrong_post);
                    } else {
                        img_answer.push_back(img_middle);
                    }
                } else {
                    img_question.push_back(img_list[1]);
                    img_question.push_back(img_list[0]);
                    img_question.push_back(img_list[2]);
                    auto img_right = getTextByTextGroup("on the left");
                    auto img_left = getTextByTextGroup("on the right");
                    
                    //correct
                    img_answer.push_back(img_middle);
                    
                    //Wrong answer
                    if (cocos2d::random(0, 1) == 0) {
                        img_answer.push_back(img_right);
//                        auto img_wrong = math::resource::ResHelper::createImage("");
//                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_left->getText().c_str(), img_question[1]->getText().c_str());
//                        auto audios_wrong = img_left->getAudios();
//                        for (std::string audio : img_question[1]->getAudios()) {
//                            audios_wrong.push_back(audio);
//                        }
//                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
//                        img_answer.push_back(img_wrong);
                    } else {
                        img_answer.push_back(img_left);
//                        auto img_wrong = math::resource::ResHelper::createImage("");
//                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_right->getText().c_str(), img_question[2]->getText().c_str());
//                        auto audios_wrong = img_right->getAudios();
//                        for (std::string audio : img_question[1]->getAudios()) {
//                            audios_wrong.push_back(audio);
//                        }
//                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
//                        img_answer.push_back(img_wrong);
                    }
                }
            }
        }  else if (question_type == QT_Top_Bottom) {
            if (number_object == 2) {
                math::resource::Image* img_correct_pos = nullptr;
                math::resource::Image* img_wrong_post = nullptr;
                
                if (fix_ask_pos==0) {
                    img_question.push_back(img_list[0]);
                    img_question.push_back(img_list[1]);
                    
                    img_correct_pos = getTextByTextGroup("in the top");
                    img_wrong_post = getTextByTextGroup("in the bottom");
                } else {
                    img_question.push_back(img_list[1]);
                    img_question.push_back(img_list[0]);
                    
                    img_correct_pos = getTextByTextGroup("in the bottom");
                    img_wrong_post = getTextByTextGroup("in the top");
                }
                
                //correct answer
                img_answer.push_back(img_correct_pos);
                img_answer.push_back(img_wrong_post);
            } else {
                math::resource::Image* img_correct_pos = nullptr;
                math::resource::Image* img_wrong_post = nullptr;
                math::resource::Image* img_middle = getTextByTextGroup("in the middle");
                
                if (fix_ask_pos < 2) {
                    if (fix_ask_pos==0) {
                        img_question.push_back(img_list[0]);
                        img_question.push_back(img_list[1]);
                        img_question.push_back(img_list[2]);
                        
                        img_correct_pos = getTextByTextGroup("top of the");
                        img_wrong_post = getTextByTextGroup("bottom of the");
                    } else if (fix_ask_pos == 1) {
                        img_question.push_back(img_list[2]);
                        img_question.push_back(img_list[1]);
                        img_question.push_back(img_list[0]);
                        
                        img_correct_pos = getTextByTextGroup("bottom of the");
                        img_wrong_post = getTextByTextGroup("top of the");
                    }
                    
                    //correct answer
                    auto img_correct = math::resource::ResHelper::createImage("");
                    std::string text_correct = cocos2d::StringUtils::format("%s %s", img_correct_pos->getText().c_str(), img_question[1]->getText().c_str());
                    auto audios_correct = img_correct_pos->getAudios();
                    for (std::string audio : img_question[1]->getAudios()) {
                        audios_correct.push_back(audio);
                    }
                    img_correct->addProperty("name_1", text_correct, audios_correct);
                    img_answer.push_back(img_correct);
                    
                    //Wrong answer
                    if (cocos2d::random(0, 2) < 2) {
                        auto img_wrong = math::resource::ResHelper::createImage("");
                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_wrong_post->getText().c_str(), img_question[1]->getText().c_str());
                        auto audios_wrong = img_wrong_post->getAudios();
                        for (std::string audio : img_question[1]->getAudios()) {
                            audios_wrong.push_back(audio);
                        }
                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
                        img_answer.push_back(img_wrong);
                    } else {
                        img_answer.push_back(img_middle);
                    }
                } else {
                    img_question.push_back(img_list[1]);
                    img_question.push_back(img_list[0]);
                    img_question.push_back(img_list[2]);
                    auto img_top = getTextByTextGroup("top of the");
                    auto img_bottom = getTextByTextGroup("bottom of the");
                    
                    //correct
                    img_answer.push_back(img_middle);
                    
                    //Wrong answer
                    if (cocos2d::random(0, 1) == 0) {
                        auto img_wrong = math::resource::ResHelper::createImage("");
                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_top->getText().c_str(), img_question[1]->getText().c_str());
                        auto audios_wrong = img_top->getAudios();
                        for (std::string audio : img_question[1]->getAudios()) {
                            audios_wrong.push_back(audio);
                        }
                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
                        img_answer.push_back(img_wrong);
                    } else {
                        auto img_wrong = math::resource::ResHelper::createImage("");
                        std::string text_wrong = cocos2d::StringUtils::format("%s %s", img_bottom->getText().c_str(), img_question[2]->getText().c_str());
                        auto audios_wrong = img_bottom->getAudios();
                        for (std::string audio : img_question[1]->getAudios()) {
                            audios_wrong.push_back(audio);
                        }
                        img_wrong->addProperty("name_1", text_wrong, audios_wrong);
                        img_answer.push_back(img_wrong);
                    }
                }
            }
        }
    }
    
    for (int i=0; i<2; i++) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(AnswerPositionOfObjectQuiz::onAnswered, this), getAnswerSize(8));
        answer->setIcon(img_answer[i]);
        answer->setTitleWithIcon("name_1");
        _answers.push_back(answer);
    }
    _answers.front()->setCorrect(true);
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_question);
    }
}

void AnswerPositionOfObjectQuiz::onStart() {
    onShowBottomBar(0, 0.3);
    quizBegan(0.5, true);
    
    cocos2d::Vec2 center_view = cocos2d::Vec2(quiz_size.width, quiz_size.height*1.3 - questionHeight()) / 2;
    if (question_type == QT_UsePicture) {
        _objects.front()->setPosition(center_view);
        addChildNode(_objects.front());
        _objects.front()->onShow(0);
    } else {
        if (question_type == QT_Left_Right || question_type == QT_NextTo) {
            float delta = 150;
            float start_x = center_view.x - object_size.width - delta;
            if (number_object == 2) {
                start_x = center_view.x - (object_size.width + delta)/2;
            }
            for (int i=0; i<number_object; i++) {
                _objects[i]->setPosition(cocos2d::Vec2(start_x + (object_size.width + delta) * i, center_view.y));
                addChildNode(_objects[i]);
                _objects[i]->onShow(i*0.1);
            }
        } else {
            float delta = 30;
            float start_y = center_view.y + object_size.height + delta;
            if (number_object == 2) {
                delta = 80;
                start_y = center_view.y + (object_size.width + delta)/2;
            }
            for (int i=0; i<number_object; i++) {
                _objects[i]->setPosition(cocos2d::Vec2(center_view.x, start_y - (object_size.height + delta) * i));
                addChildNode(_objects[i]);
                _objects[i]->onShow(i*0.1);
            }
        }
    }
    
    math::func::shuffle(_answers);
    fixAnswerSize(quiz_size.width * 0.39, 200);
    math::func::setPositions(_answers, cocos2d::Rect(30, 0, quiz_size.width-60, quiz_size.height*0.3 - 30));
}

void AnswerPositionOfObjectQuiz::startQuestion() {
    enableControlQuiz();
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, i*0.1, math::animation::SlideUp);
    }
}

void AnswerPositionOfObjectQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void AnswerPositionOfObjectQuiz::onComplete() {
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(i*0.1, math::animation::ScaleUp);
    }
    for (int i=0; i < _answers.size(); i++) {
        Math_Answer* answer = _answers[i];
        answer->setDisable(i*0.1, math::animation::SlideUp);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void AnswerPositionOfObjectQuiz::onAnswered(Node* node) {
    disableControlQuiz(0.5);
    Math_Answer* answer = (Math_Answer*) node;
    bool done = answer->isCorrect();
    answer->onAnimationClick(done);
    
    scheduleOnce([=](float){
        this->onExplanation(done);
    }, 0.5, "wait explain");
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void AnswerPositionOfObjectQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void AnswerPositionOfObjectQuiz::autoTestingRun(float delaytime) {
    
}
