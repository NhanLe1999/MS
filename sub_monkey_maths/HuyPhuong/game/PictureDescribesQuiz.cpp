//
//  PictureDescribesQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "PictureDescribesQuiz.h"

PictureDescribesQuiz* PictureDescribesQuiz::createQuiz()
{
    PictureDescribesQuiz* quiz=new PictureDescribesQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool PictureDescribesQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void PictureDescribesQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string ask_pos = getJsonString(json_game, "position", "use_picture");
    if (ask_pos == "use_picture") {
        question_type = QT_Picture;
    } else if (ask_pos == "left_right") {
        question_type = QT_Left_Right;
    } else if (ask_pos == "top_bottom") {
        question_type = QT_Top_Bottom;
    } else {
        question_type = QT_Above_Below;
    }
    number_object = randomRangeJson(json_game, "number_object");

    std::vector<math::resource::Image*> img_question;
    
    if (question_type == QT_Picture) {
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
        
        fix_ask_pos = getJsonInt(json_game, "fix_ask_pos", 0);
        if (fix_ask_pos > 1) {
            fix_ask_pos = cocos2d::random(0, 1);
        }
        std::vector<math::resource::Image*> imgs_selected;
        std::vector<math::resource::Image*> imgs_diff;
        for (math::resource::Image* img : img_list) {
            if (img->getComparisionValue("role") == fix_ask_pos) {
                imgs_selected.push_back(img);
            } else {
                imgs_diff.push_back(img);
            }
        }
        if (!imgs_selected.empty()) {
            img_list.clear();
            img_list.push_back(math::func::getRandomNode(imgs_selected));
            img_list.push_back(math::func::getRandomNode(imgs_diff));
        } else {
            img_list.clear();
            math::func::shuffle(all_imgs);
            while (img_list.size() < 2) {
                img_list.clear();
                int m_pair = math::func::getRandomNode(all_imgs)->getComparisionValue("pair");
                for (math::resource::Image* img : all_imgs) {
                    if (img->getComparisionValue("pair") == m_pair) {
                        img_list.push_back(img);
                    }
                }
            }
        }
        
        //Gen object
        object_size = cocos2d::Size(450, 450);
        for (int i=0; i<2; i++) {
            auto object = mpool->getObject();
            object->setEnable(i, math::common::touch, img_list[i], nullptr);
            object->setObjectSize(object_size);
            object->fixWidthObject();
            object->setTouchCallback(CC_CALLBACK_1(PictureDescribesQuiz::onTouchObject, this));
            _objects.push_back(object);
        }
        math::func::shuffle(_objects);
        img_question = img_list;
    } else {
        random_icon = getJsonString(json_game, "icon", "random") == "random";
        fix_ask_pos = getJsonInt(json_game, "fix_ask_pos", -1);
        if (fix_ask_pos >= number_object) {
            fix_ask_pos = cocos2d::random(1, number_object)-1;
        }
        if (question_type == QT_Above_Below && fix_ask_pos > 1) {
            fix_ask_pos = cocos2d::random(0, 1);
        }
        
        rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
        std::string icon_same = getJsonString(json_icon, "same", "none");
        std::string icon_diff = getJsonString(json_icon, "different", "id");
        
        auto all_imgs = getImageList("icons");
        all_imgs = math::resource::ResHelper::filterImages(all_imgs, icon_same);
        
        if (random_icon) {
            img_list = getImageList(all_imgs, number_object, icon_diff);
            math::func::shuffle(img_list);
        } else {
            img_list = getImageList(all_imgs, 2, icon_diff);
            math::func::shuffle(img_list);
            if (number_object > 2) {
                img_list.push_back(img_list.back());
            }
        }
        
        auto group_bg = groupBG();
        if (number_object == 2) {
            cocos2d::Size item_size = cocos2d::Size(180, 180);

            for (int i=0; i<2; i++) {
                std::vector<Math_Object*> list;
                for (int i2=0; i2 < number_object; i2++) {
                    auto object = mpool->getObject();
                    object->setEnable(i2, math::common::deny, img_list[abs(i-i2)], nullptr);
                    object->setObjectSize(item_size);
                    list.push_back(object);
                }
                
                int column = 1;
                int row = number_object;
                if (question_type == QT_Left_Right) {
                    column = number_object;
                    row = 1;
                }
                
                group_size = cocos2d::Size(item_size.width*1.2*column + 60, item_size.height*1.2*row + 60);
                auto group = mpool->getGroup();
                group->setEnable(i, list, math::group::queue, number_object);
                group->setObjectSize(item_size);
                group->setGroupQueue(group_size);
                group->setBackground(group_bg, 0.3);
                group->enableToch(CC_CALLBACK_1(PictureDescribesQuiz::onTouchGroup, this));
                _groups.push_back(group);
            }
            if (fix_ask_pos == 0) {
                img_question = img_list;
            } else {
                img_question.push_back(img_list.back());
                img_question.push_back(img_list.front());
            }
        } else {
            cocos2d::Size item_size = cocos2d::Size(120, 120);
            std::vector<math::resource::Image*> temp_imgs;
            for (math::resource::Image* img : img_list) {
                temp_imgs.push_back(img);
            }
            for (int i=0; i<2; i++) {
                if (i==1) {
                    temp_imgs.clear();
                    if (fix_ask_pos==2) {
                        //Mid
                        auto indexes = randomArrayIndexes(3, 3);
                        while (indexes[1]==1) {
                            indexes = randomArrayIndexes(3, 3);
                        }
                        for (int index : indexes) {
                            temp_imgs.push_back(img_list[index]);
                        }
                    } else {
                        temp_imgs.push_back(img_list.back());
                        temp_imgs.push_back(img_list[1]);
                        temp_imgs.push_back(img_list.front());
                    }
                }
                
                std::vector<Math_Object*> list;
                for (int i2=0; i2 < number_object; i2++) {
                    auto object = mpool->getObject();
                    object->setEnable(i2, math::common::deny, temp_imgs[i2], nullptr);
                    object->setObjectSize(item_size);
                    list.push_back(object);
                }
                
                int column = 1;
                int row = number_object;
                if (question_type == QT_Left_Right) {
                    column = number_object;
                    row = 1;
                }
                group_size = cocos2d::Size(item_size.width*1.2*column + 60, item_size.height*1.2*row + 60);
                
                auto group = mpool->getGroup();
                group->setEnable(i, list, math::group::queue, number_object);
                group->setObjectSize(item_size);
                group->setGroupQueue(group_size);
                group->setBackground(group_bg);
                group->enableToch(CC_CALLBACK_1(PictureDescribesQuiz::onTouchGroup, this));
                _groups.push_back(group);
            }
            
            if (fix_ask_pos == 0) {
                img_question = img_list;
            } else if (fix_ask_pos == 1) {
                img_question.push_back(img_list.back());
                img_question.push_back(img_list[1]);
                img_question.push_back(img_list.front());
            } else {
                img_question.push_back(img_list[1]);
                img_question.push_back(img_list.back());
                img_question.push_back(img_list.front());
            }
        }
        math::func::shuffle(_groups);
    }
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_question);
    }
    if (question_type != QT_Picture) {
        getQuestion()->setKeywordValue(fix_ask_pos);
    }
    
}

void PictureDescribesQuiz::onStart() {
    Math_Quiz::onStart();
    
    if (question_type == QT_Picture) {
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->setPosition(cocos2d::Vec2((quiz_size.width - object_size.width - 50) * 0.5 + (object_size.width + 100)*i, (quiz_size.height-getQuestion()->getContentSize().height-20)*0.5));
            addChildNode(object, 2);
            object->onShow(i*0.1, math::animation::SlideUp);
        }
    } else {
        for (int i=0; i < _groups.size(); i++) {
            Math_Group* group = _groups[i];
            group->setPosition(cocos2d::Vec2((quiz_size.width - group_size.width - 50) * 0.5 + (group_size.width + 100)*i, (quiz_size.height-getQuestion()->getContentSize().height-20)*0.5));
            addChildNode(group, 2);
            group->onShow(i*0.1, math::animation::SlideUp);
        }
    }
}

void PictureDescribesQuiz::startQuestion() {
    enableControlQuiz();
}

void PictureDescribesQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void PictureDescribesQuiz::onComplete() {
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(i*0.1, math::animation::SlideUp);
    }
    
    for (int i=0; i < _groups.size(); i++) {
        Math_Group* group = _groups[i];
        group->onHide(i*0.1, math::animation::SlideUp);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void PictureDescribesQuiz::onTouchObject(Node* node) {
    disableControlQuiz(0.5);
    Math_Object* object = static_cast<Math_Object*>(node);
    bool correct = (object->getID() == 0);
    
    object->onTouchObject(correct);
    if (correct) {
        for (Math_Object* obj : _objects) {
            obj->setLocked(true);
        }
        this->onExplanation(true);
    }
}

void PictureDescribesQuiz::onTouchGroup(Node* node) {
    disableControlQuiz(0.5);
    Math_Group* group = static_cast<Math_Group*>(node);
    bool correct = group->getId()==0;
    group->onTouchedAnimation(correct);
    scheduleOnce([=](float){
        this->onExplanation(correct);
    }, 0.5, "wait explain");
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void PictureDescribesQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void PictureDescribesQuiz::autoTestingRun(float delaytime) {
    
}
