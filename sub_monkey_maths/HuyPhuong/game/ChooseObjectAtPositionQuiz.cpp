//
//  ChooseObjectAtPositionQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "ChooseObjectAtPositionQuiz.h"

ChooseObjectAtPositionQuiz* ChooseObjectAtPositionQuiz::createQuiz()
{
    ChooseObjectAtPositionQuiz* quiz=new ChooseObjectAtPositionQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ChooseObjectAtPositionQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void ChooseObjectAtPositionQuiz::initQuiz(mj::model::Game game)
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
    } else {
        question_type = QT_Above_Below;
    }
    
    coloring = getJsonBool(json_game, "color", false);
    number_object = randomRangeJson(json_game, "number_object");

    random_icon = getJsonString(json_game, "icon", "random") == "random";
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
        fix_ask_pos = cocos2d::random(0, 2);
    }
    
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
    std::string icon_same = getJsonString(json_icon, "same", "none");
    std::string icon_diff = getJsonString(json_icon, "different", "id");
    auto all_imgs = getImageList("icons");
    all_imgs = math::resource::ResHelper::filterImages(all_imgs, icon_same);
    
    std::vector<math::resource::Image*> img_question;
    
    object_size = cocos2d::Size(200, 200);
    if ((question_type == QT_Above_Below || question_type == QT_Top_Bottom) && number_object == 3) {
        object_size = cocos2d::Size(150, 150);
    }
    if (coloring) {
        std::vector<math::resource::Image*> white_imgs;
        for (math::resource::Image* img : all_imgs) {
            if (img->getText("color") == "white") {
                white_imgs.push_back(img);
            }
        }
        
        auto img_select = math::func::getRandomNode(white_imgs);
        std::string name_img = img_select->getText();
        for (math::resource::Image* img : all_imgs) {
            auto color = img->getText("color");
            if (color != "white" && img->getText() == name_img) {
                img_color.push_back(img);
                color_correct = color;
                break;
            }
        }
        
        math::resource::Image* thirt_img=nullptr;
        if (number_object == 2) {
            img_list.push_back(img_select);
            img_list.push_back(img_select);
            img_question.push_back(img_select);
            img_question.push_back(img_select);
        } else {
            math::func::shuffle(all_imgs);
            for (math::resource::Image* img : all_imgs) {
                if (img->getText() != name_img && img->getText("color") != "white") {
                    thirt_img = img;
                    break;
                }
            }
            img_question.push_back(img_select);
            img_question.push_back(thirt_img);

            if (question_type == QT_NextTo) {
                if (fix_ask_pos==0) {
                    img_list.push_back(thirt_img);
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                } else if (fix_ask_pos==1) {
                    img_list.push_back(img_select);
                    img_list.push_back(thirt_img);
                    img_list.push_back(img_select);
                } else {
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                    img_list.push_back(thirt_img);
                }
            } else if (question_type == QT_Top_Bottom) {
                if (random_icon) {
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                } else {
                    img_list.push_back(img_select);
                    img_list.push_back(thirt_img);
                    img_list.push_back(img_select);
                }
            } else if (question_type == QT_Left_Right) {
                if (fix_ask_pos == 2) {
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                    img_list.push_back(img_select);
                } else {
                    img_list.push_back(img_select);
                    img_list.push_back(thirt_img);
                    img_list.push_back(img_select);
                }
            } else {
                img_list.push_back(img_select);
                img_list.push_back(thirt_img);
                img_list.push_back(img_select);
            }
        }
        
        for (int i2=0; i2 < number_object; i2++) {
            auto object = mpool->getObject();
            object->setEnable(i2, math::common::touch, img_list[i2], nullptr);
            object->setObjectSize(object_size);
            if (img_list[i2] != thirt_img) {
                object->setTouchCallback(CC_CALLBACK_1(ChooseObjectAtPositionQuiz::onTouchObjectColor, this));
            }
            _objects.push_back(object);
        }
        
        color_list.push_back(color_correct);
        
        auto colors = getColors();
        math::func::shuffle(colors);
        
        for (math::resource::Image* img : colors) {
            auto m_color = img->getText();
            if (m_color != "black" && m_color != "white" && m_color != color_correct) {
                color_list.push_back(m_color);
            }
            if (color_list.size() >= 3) {
                break;
            }
        }
        
    } else {
        if (number_object == 2) {
            img_list = getImageList(all_imgs, 2, icon_diff);
            img_question = img_list;
        } else {
            if (question_type == QT_NextTo) {
                if (random_icon) {
                    img_list = getImageList(all_imgs, 3, icon_diff);
                    img_question.push_back(img_list[fix_ask_pos]);
                } else {
                    img_list = getImageList(all_imgs, 2, icon_diff);
                    img_question = img_list;
                    if (fix_ask_pos==0) {
                        img_list.push_back(img_list.back());
                    } else if (fix_ask_pos==1) {
                        img_list.insert(img_list.begin(), img_list.back());
                    } else {
                        auto imgs = img_list;
                        img_list.clear();
                        img_list.push_back(imgs.back());
                        img_list.push_back(imgs.back());
                        img_list.push_back(imgs.front());
                    }
                }
            } else if (question_type == QT_Top_Bottom) {
                if (fix_ask_pos == 2) {
                    img_list = getImageList(all_imgs, 3, icon_diff);
                    img_question = img_list;
                } else {
                    if (random_icon) {
                        img_list = getImageList(all_imgs, 3, icon_diff);
                        img_question=img_list;
                    } else {
                        img_list = getImageList(all_imgs, 2, icon_diff);
                        
                        img_question.push_back(img_list.front());
                        img_question.push_back(img_list.back());
                        img_list.push_back(img_list.front());
                    }
                }
//                img_list = getImageList(all_imgs, 3, icon_diff);
//                img_question = img_list;
            } else if (question_type == QT_Left_Right) {
                if (fix_ask_pos == 2) {
                    img_list = getImageList(all_imgs, 3, icon_diff);
                    img_question = img_list;
                } else {
                    if (random_icon) {
                        img_list = getImageList(all_imgs, 3, icon_diff);
                        img_question=img_list;
                    } else {
                        img_list = getImageList(all_imgs, 2, icon_diff);
                        
                        img_question.push_back(img_list.front());
                        img_question.push_back(img_list.back());
                        img_list.push_back(img_list.front());
                    }
                }
            } else { //Above - Below
                if (random_icon) {
                    img_list = getImageList(all_imgs, 3, icon_diff);
                    img_question = img_list;
                } else {
                    img_list = getImageList(all_imgs, 2, icon_diff);
                    img_question.push_back(img_list.front());
                    img_question.push_back(img_list.back());
                    img_list.push_back(img_list.front());
                }
            }
        }
        
        for (int i2=0; i2 < number_object; i2++) {
            auto object = mpool->getObject();
            object->setEnable(i2, math::common::touch, img_list[i2], nullptr);
            object->setObjectSize(object_size);
            object->setTouchCallback(CC_CALLBACK_1(ChooseObjectAtPositionQuiz::onTouchObject, this));
            _objects.push_back(object);
        }
    }
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "color") {
            getQuestion()->setOptionalTexts(key, img_color);
        } else {
            getQuestion()->setOptionalTexts(key, img_question);
        }
    }
    getQuestion()->setKeywordValue(fix_ask_pos);
}

void ChooseObjectAtPositionQuiz::onStart() {
    
    cocos2d::Vec2 center_view = quiz_size/2;
    
    if (coloring) {
        showColorsPad(color_list, [=](Node* node){
            auto object = static_cast<Math_Object*>(node);
            color_selected = object->getImageSource()->getText();
        }, nullptr);
        Math_Quiz::onStart();

        center_view = cocos2d::Vec2(quiz_size.width-250, quiz_size.height - getQuestion()->getContentSize().height - 20) / 2;
    } else {
        Math_Quiz::onStart();
        
        center_view = cocos2d::Vec2(quiz_size.width, quiz_size.height - getQuestion()->getContentSize().height - 20) / 2;
    }
    
    if (question_type == QT_Left_Right || question_type == QT_NextTo) {
        float delta = 150;
        if (coloring && number_object == 3) {
            delta = 80;
        }
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
        float delta = 50;
        float start_y = center_view.y + object_size.height + delta;
        if (number_object == 2) {
            delta = 100;
            start_y = center_view.y + (object_size.width + delta)/2;
        }
        for (int i=0; i<number_object; i++) {
            _objects[i]->setPosition(cocos2d::Vec2(center_view.x, start_y - (object_size.height + delta) * i));
            addChildNode(_objects[i]);
            _objects[i]->onShow(i*0.1);
        }
    }
}

void ChooseObjectAtPositionQuiz::startQuestion() {
    enableControlQuiz();
}

void ChooseObjectAtPositionQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void ChooseObjectAtPositionQuiz::onComplete() {
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(i*0.1, math::animation::ScaleUp);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void ChooseObjectAtPositionQuiz::onTouchObject(Node* node) {
    disableControlQuiz(0.5);
    Math_Object* object = static_cast<Math_Object*>(node);
    
    bool correct = true;
    bool correct_half = false;
    if (question_type == QT_NextTo) {
        if (fix_ask_pos == 0 || fix_ask_pos == 2) {
            correct = object->getID()==1;
        } else {
            if (object->getID()==0 || object->getID()==2) {
                next_to_selected.push_back(object);
                object->setLocked(true);
                correct_half = true;
            }
            correct = next_to_selected.size()==2;
        }
    } else {
        if (fix_ask_pos == 0) {
            correct = object->getID()==0;
        } else if (fix_ask_pos == 1) {
            if (number_object==2) {
                correct = object->getID()==1;
            } else {
                correct = object->getID()==2;
            }
        } else {
            correct = object->getID()==1;
        }
    }
    
    object->onTouchObject(correct || correct_half, 0, 100);
    if (correct) {
        for (Math_Object* obj : _objects) {
            obj->setLocked(true);
        }
        this->onExplanation(true);
    }
}

void ChooseObjectAtPositionQuiz::onTouchObjectColor(Node* node) {
    disableControlQuiz(0.5);
    Math_Object* object = static_cast<Math_Object*>(node);
    
    bool correct = true;
    if (color_selected != color_correct) {
        correct = false;
    }
    
    bool correct_coloring = false;
    if (correct) {
        if (question_type == QT_NextTo) {
            if (fix_ask_pos == 0 || fix_ask_pos == 2) {
                correct = object->getID()==1;
            } else {
                if (object->getID()==0 || object->getID()==2) {
                    next_to_selected.push_back(object);
                    object->setLocked(true);
                    correct_coloring = true;
                }
                correct = next_to_selected.size()==2;
            }
        } else {
            if (fix_ask_pos == 0) {
                correct = object->getID()==0;
            } else if (fix_ask_pos == 1) {
                if (number_object==2) {
                    correct = object->getID()==1;
                } else {
                    correct = object->getID()==2;
                }
            } else {
                correct = object->getID()==1;
            }
        }
    }
    
    object->onTouchObject(correct || correct_coloring);
    if (correct || correct_coloring) {
        object->setImage(img_color.front());
    }
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
void ChooseObjectAtPositionQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ChooseObjectAtPositionQuiz::autoTestingRun(float delaytime) {
    
}
