//
//  MoveCorrectObjectToBoxQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "MoveCorrectObjectToBoxQuiz.h"

MoveCorrectObjectToBoxQuiz* MoveCorrectObjectToBoxQuiz::createQuiz()
{
    MoveCorrectObjectToBoxQuiz* quiz=new MoveCorrectObjectToBoxQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MoveCorrectObjectToBoxQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MoveCorrectObjectToBoxQuiz::initQuiz(mj::model::Game game)
{
     
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    compare_key = getComparisionKey(getJsonString(json_game, "compare_key", "id"));
    had_temp = getJsonBool(getJsonMember(json_game, "template"), "enable", false);
    show_temp = getJsonBool(getJsonMember(json_game, "template"), "show", false);
    
    isBigger = true;
    if (compare_key == math::comparision::size) {
        isBigger = cocos2d::random(0, 1)==0;
    }
    
    int value_keyword = 0;
    rapidjson::Value& _request=json_game.FindMember("request")->value;
    if (_request.Size()>0) {
        value_keyword = cocos2d::random(0, (int)_request.Size()-1);
        question_req.push_back(_request[value_keyword].GetString());
    }
    
    for (std::string req : question_req) {
        if (req.compare("different")==0) {
            different_req=true;
        }
        if (req.compare("ordinal")==0) {
            ordinal_req=true;
        }
    }

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    number_object = randomRangeJson(json_object, "number");
    number_kind = randomRangeJson(json_object, "kind");
    object_size = getSizeJson(json_object, "size");
    std::string background_key = getJsonString(json_object, "background", "none");
    show_icon = getJsonString(json_object, "show", "icon");
    fit_object = getJsonString(json_object, "fit_size", "none");

    if (number_kind>number_object) {
        number_kind=number_object;
    }
   
    img_bg = nullptr;
    if (background_key != "none") {
        img_bg = slotBG();
    }
    display_type = getJsonString(json_object, "display", "vertical");
    
    //TODO: ICON CONFIG
    icon_filter = getJsonString(getJsonMember(json_data, "icon"), "different", "id");
    icon_same = getJsonString(getJsonMember(json_data, "icon"), "same", "none");
    
    auto all_imgs = getImageList("icons");
    img_list = getImageList(all_imgs, number_kind, icon_filter, icon_same);
    
    //chưa có img_number_list
    if (compare_key == math::comparision::number) {
        if (ordinal_req) {
            for (math::resource::Image* img : img_list) {
                int number = img->getComparisionValue("number");
                img_number_list.push_back(getOrdinals(number, number).front());
            }
        } else {
            for (math::resource::Image* img : img_list) {
                int number = img->getComparisionValue("number");
                img_number_list.push_back(getNumbers(number, number).front());
            }
        }
    }
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue(value_keyword);
    
    //Random so luong tung kind
    if (different_req) {
        int number_same = number_object-1;
        number_kind = 2;
        int index_same=0;
        for (int i=0; i<number_kind; i++) {
            if (i==index_same) {
                number_list.push_back(number_same);
            } else {
                number_list.push_back(1);
            }
        }
        for (int i=0; i<number_kind; i++) {
            if (compare_key == math::comparision::size) {
                if (i==index_same) {
                    for (int i2=0; i2<number_same; i2++) {
                        size_list.push_back(i);
                    }
                } else {
                    size_list.push_back(i);
                }
            } else {
                size_list.push_back(0);
            }
        }
    } else {
        //Random list number & size
        int _div = number_object / number_kind;
        int _mod = number_object % number_kind;
        for (int i=0; i<number_kind; i++) {
            if (i<_mod) {
                number_list.push_back(_div+1);
            } else {
                number_list.push_back(_div);
            }
        }
        math::func::shuffle(number_list);
        
        //Check compare size.
        for (int i=0; i<number_kind; i++) {
            if (compare_key == math::comparision::size) {
                if (isBigger) {
                    size_list.push_back(i);
                } else {
                    size_list.push_back(number_kind-1-i);
                }
            } else {
                size_list.push_back(0);
            }
        }
        if (compare_key != math::comparision::size) {
            math::func::shuffle(size_list);
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    int size_fix = 0;
    int obj_id=0;
    auto imgs_gen = getImageListNumber(all_imgs, img_list, number_list, icon_filter);
    
    max_object_size = object_size;
    
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id, math::common::deny, imgs_gen[obj_id], img_bg);
            object->setComparisionValue(size_list[i]*2+size_fix);
            object->setObjectSize(object_size);
            object->setMoveCallback(CC_CALLBACK_1(MoveCorrectObjectToBoxQuiz::onTouchedObject, this), CC_CALLBACK_2(MoveCorrectObjectToBoxQuiz::onMoveObjectEnded, this));
            if (fit_object.compare("width")==0) {
                object->fixWidthObject();
                display_type = "horizontal";
                if (object->getContentSize().height > max_object_size.height) {
                    max_object_size = object->getContentSize();
                }
            } else if (fit_object.compare("height")==0) {
                object->fixHeightObject();
                display_type = "vertical";
                if (object->getContentSize().width > max_object_size.width) {
                    max_object_size = object->getContentSize();
                }
            }
            
            if (show_icon.compare("icon")!=0) {
                object->setImageVisible(false);
                object->setValueTextByKey(show_icon);
                object->showValueTextWithAnimation(false);
                if (show_icon.compare("number")==0) {
                    object->getLabelValue()->setFontSize(object_size.height*0.75);
                }
            }
            _objects.push_back(object);
            obj_id++;
        }
    }
    
    if (had_temp) {
        temp_object = mpool->getObject();
        temp_object->setEnable(0, math::common::deny, img_list[0], img_bg);
        temp_object->setComparisionValue(size_list[0]+size_fix);
        temp_object->setObjectSize(object_size);
    }
    
    math::resource::Image* group_bg = groupBG();
    
    Math_Slot* slot = mpool->getSlot();
    slot->setEndble(0, group_bg);
    if (fit_object == "width") {
        slot->setSlotSize(cocos2d::Size(350, max_object_size.height+50)/0.9);
    } else {
        slot->setSlotSize(cocos2d::Size(max_object_size.width +50, max_object_size.height+50)/0.9);
    }
    slot->setBackground(group_bg, 0.3);
    _slots.push_back(slot);
    std::string question_request = question_req[0];
    Math_Object* correct_obj = _objects[0];
    if (question_request.compare("small")==0) {
        for (Math_Object* obj : _objects) {
            if (correct_obj->compare(obj, compare_key) > 0) {
                correct_obj = obj;
            }
        }
    } else if (question_request.compare("big")==0) {
        for (Math_Object* obj : _objects) {
            if (correct_obj->compare(obj, compare_key) < 0) {
                correct_obj = obj;
            }
        }
    } else if (question_request.compare("compare")==0) {
        for (Math_Object * obj : _objects) {
            if (obj->isEqual(temp_object, compare_key)) {
                correct_obj=obj;
                break;
            }
        }
    } else if (question_request.compare("different")==0) {
        for (int i1=0; i1<number_object-1; i1++) {
            Math_Object* obj1 = _objects[i1];
            bool correct = true;
            for (int i2=i1+1; i2<number_object; i2++) {
                if (obj1->isEqual(_objects[i2], compare_key)) {
                    correct = false;
                    break;
                }
            }
            if (correct) {
                correct_obj = obj1;
                break;
            }
        }
    } else if (question_request.compare("ordinal")==0) {
        
    }
    slot->setComparisionValue(correct_obj->getComparisionValue());
}

void MoveCorrectObjectToBoxQuiz::onStart() {
    
    onShowBottomBar(0, 0.2, false, [=](Node* node){
        Math_Answer* answer = (Math_Answer*) node;
        bool correct = _slots[0]->isCorrect(compare_key);
        answer->onAnimationClick(correct);
        this->onExplanation(correct);
    }, nullptr, true);
    
    Math_Quiz::onStart();

    cocos2d::Vec2 center = cocos2d::Vec2(quiz_size.width, quiz_size.height * 0.8 - getQuestion()->getContentSize().height-20) /2;
    
    auto quiz_height = quiz_size.height * 0.8 - getQuestion()->getContentSize().height-20;
    if (fit_object == "height") {
        _slots.front()->setPosition(cocos2d::Vec2(quiz_size.width * 0.25, center.y));
        this->addChildNode(_slots.front());
        _slots.front()->onShow(0.5, math::animation::SlideRight);
        
        float distance = (quiz_height - number_object * max_object_size.height) / (number_object+1);
        if (distance > 100) {
            distance = 100;
        }
        math::func::smartSetPosition(_objects, max_object_size, cocos2d::Rect(quiz_size.width/2, 0, quiz_size.width/2, quiz_height), distance, false);
        
        for (int i=0; i<_objects.size(); i++) {
            Math_Object* object = _objects[i];
            auto obj_size = object->getContentSize();
            auto obj_position = object->getPosition();
            object->setPosition(cocos2d::Vec2(obj_position.x - (max_object_size.width - obj_size.width)/2, obj_position.y));
            addChildNode(object, 2);
            object->onShow(0.5, math::animation::SlideRight);
        }
    } else if (fit_object == "width") {
        _slots.front()->setPosition(cocos2d::Vec2(quiz_size.width * 0.25, center.y));
        this->addChildNode(_slots.front());
        _slots.front()->onShow(0.5, math::animation::SlideUp);

        float distance = (quiz_size.width/2 - number_object * max_object_size.width) / (number_object+1);
        if (distance > 100) {
            distance = 100;
        }
        math::func::smartSetPosition(_objects, max_object_size, cocos2d::Rect(quiz_size.width/2, 0, quiz_size.width/2, quiz_height), distance);
        
        for (int i=0; i<_objects.size(); i++) {
            Math_Object* object = _objects[i];
            auto obj_size = object->getContentSize();
            auto obj_position = object->getPosition();
            object->setPosition(cocos2d::Vec2(obj_position.x, obj_position.y - (max_object_size.height - obj_size.height)/2));
            addChildNode(object, 2);
            object->onShow(0.5, math::animation::SlideUp);
        }
    } else {
        if (display_type == "vertical") {
            _slots.front()->setPosition(cocos2d::Vec2(quiz_size.width * 0.5, _slots.front()->getContentSize().height * 0.5 + 30));
            
            math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, _slots.front()->getContentSize().height+60, quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height-20 - _slots.front()->getContentSize().height-60), 100);

        } else {
            _slots.front()->setPosition(cocos2d::Vec2(quiz_size.width * 0.25, center.y));
            
            math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(quiz_size.width*0.5, 0, quiz_size.width*0.5, quiz_size.height*0.75 - getQuestion()->getContentSize().height-20), 100);
        }
        
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            addChildNode(object, 2);
            object->onShow(0.5);
        }
        
        this->addChildNode(_slots[0]);
        _slots[0]->onShow(0.5);

    }
}

void MoveCorrectObjectToBoxQuiz::startQuestion() {
     
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        if (ordinal_req) {
            std::string start_ordinal = "1";
            for (math::resource::Image* img : img_number_list) {
                if (img->getComparisionValue("number") == 1) {
                    start_ordinal = img->getText("name_n");
                    break;
                }
            }
            _objects[0]->setValueText(start_ordinal);
            _objects[0]->showValueTextWithAnimation(true);
        }

        for (Math_Object* object : _objects) {
            object->enableDrag(_panel);
        }
        
    }
}

void MoveCorrectObjectToBoxQuiz::onExplanation(bool correct) {
     
    disableControlQuiz();
    scheduleOnce([=](float dt){
        if (!correct) {
            if (_slots.front()->isUsed()) {
                _slots.front()->getLinker()->backToStart();
                _slots.front()->removeLinker();
            }
            enableControlQuiz();
        } else {
            this->nextQuestion();
        }
    }, 1.0, "selected_anim");

}

void MoveCorrectObjectToBoxQuiz::onComplete() {
     
    //TODO: Thuc hien them cac lenh o day
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(0.5);
    }
    _slots[0]->onHide(0.5);

    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.5, "complete_quiz");
}

#pragma mark - object touch events callback
void MoveCorrectObjectToBoxQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = (Math_Object*)node;
    
    //Chon object dau tien cham phai
    if (selected_object==NULL) {
        selected_object=object;
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void MoveCorrectObjectToBoxQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
    //Danh cho game sap xep lai vi tri roi chon dap an
    Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    Math_Slot* m_slot = _slots[0];

    if (m_slot->isAvailable(position)) {
        object->removeFromParentAndCleanup(false);
        addChildNode(object, 10);
        object->setPosition(position);
        
        
        if (m_slot->isUsed()) {
            auto linker = m_slot->getLinker();
            if (object->isSame(linker)) {
                object->runAction(cocos2d::MoveTo::create(0.2, m_slot->getPosition()));
            } else {
                m_slot->removeLinker();
                m_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, m_slot->getPosition()));
                linker->backToStart();
            }
        } else {
            m_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, m_slot->getPosition()));
        }
    } else {
        if (m_slot->isUsed() && object->isSame(m_slot->getLinker())) {
            m_slot->removeLinker();
        }
        object->backToStart();
    }
    selected_object = NULL;
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MoveCorrectObjectToBoxQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MoveCorrectObjectToBoxQuiz::autoTestingRun(float delaytime)
{
    
}

