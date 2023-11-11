//
//  SortObjectInOrderQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "SortObjectInOrderQuiz.h"

SortObjectInOrderQuiz* SortObjectInOrderQuiz::createQuiz()
{
    SortObjectInOrderQuiz* quiz=new SortObjectInOrderQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return nullptr;
}

bool SortObjectInOrderQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void SortObjectInOrderQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    compare_key = getComparisionKey(getJsonString(json_game, "compare_key", "height"));
    std::string str_order = json_game.FindMember("order")->value.GetString();
    if (str_order.compare("asc")==0) {
        sort_asc = true;
    } else if (str_order.compare("dec")==0) {
        sort_asc = false;
    } else {
        sort_asc = cocos2d::random(0, 1)==0;
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    int number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    int number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    object_size = getSizeJson(json_object, "size");
    std::string background_key = getJsonString(json_object, "background", "none");
    fit_object = getJsonString(json_object, "fit_size", "height");
    
    number_object = cocos2d::random(number_min, number_max);
    
    img_bg = nullptr;
    if (background_key != "none") {
        img_bg = objectBG();
    }
    
    if (compare_key == math::comparision::size) {
        for (int i=0; i<number_object; i++) {
            size_list.push_back(i);
        }
        math::func::shuffle(size_list);
    } else {
        for (int i=0; i<number_object; i++) {
            size_list.push_back(0);
        }
    }
    
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = getJsonString(dislay_json, "type", "line");
    max_in_line = getJsonInt(dislay_json, "max_line", number_object);
    
    //TODO: ICON CONFIG
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    
    auto all_imgs = getImageList("icons");
    
    if (compare_key == math::comparision::size) {
        auto icon = math::func::getRandomNode(all_imgs);
        for (int i=0; i<number_object; i++) {
            img_list.push_back(icon);
        }
    } else {
        img_list = getImageList(all_imgs, number_object, icon_filter, icon_same);
        img_number_list = getNumbers(1, number_object);
    }
    
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
        } else if (key == "ordinal") {
            img_number_list = getOrdinals(1, number_object);
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue(sort_asc?0:1);
    
    //TODO: GENERATE QUIZ COMPONENTS
    slot_size = object_size;
    for (int i=0; i<number_object; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, img_list[i], img_bg);
        object->setComparisionValue(size_list[i]);
        object->setObjectSize(object_size, true);
        object->setMoveCallback(nullptr, CC_CALLBACK_2(SortObjectInOrderQuiz::onMoveObjectEnded, this));
        if (fit_object.compare("width")==0) {
            object->fixWidthObject();
            if (slot_size.height < object->getContentSize().height) {
                slot_size.height = object->getContentSize().height;
            }
        } else if (fit_object.compare("height")==0) {
            object->fixHeightObject();
            if (slot_size.width < object->getContentSize().width) {
                slot_size.width = object->getContentSize().width;
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
    }
    if (fit_object == "none") {
        fit_object = "width";
    }
    
    if (sort_asc) {
        std::sort(_objects.begin(), _objects.end(), [=](Math_Object* a, Math_Object* b)->bool {
            return a->compare(b, compare_key)<0;
        });
    } else {
        std::sort(_objects.begin(), _objects.end(), [=](Math_Object* a, Math_Object* b)->bool {
            return a->compare(b, compare_key)>0;
        });
    }
    
    math::resource::Image* slot_bg=slotBG();
    
    for (int i=0; i<number_object; i++) {
        Math_Slot* slot = mpool->getSlot();
        slot->setEndble(0, slot_bg);
        slot->setSlotSize(slot_size);
        slot->setComparisionValue(_objects[i]->getComparisionValue());
        slot->setBackgroundVisible(false);
        _slots.push_back(slot);
    }
}

void SortObjectInOrderQuiz::onStart() {
    max_in_line = (int)_slots.size();

    if (fit_object.compare("width")==0) {
        onShowBottomBar(0, 0.2, false, [=](Node* node){
            Math_Answer* answer = (Math_Answer*) node;
            bool correct = true;
            for (Math_Slot* slot : _slots) {
                if (!slot->isCorrect(compare_key)) {
                    correct = false;
                    break;
                }
            }
            answer->onAnimationClick(correct);
            this->onExplanation(correct);
        });
        Math_Quiz::onStart();
        float delta = (quiz_size.width - max_in_line * object_size.width) / (max_in_line+1);
        delta = delta > 100 ? 100 : delta;
        math::func::smartSetPosition(_slots, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.8 - getQuestion()->getContentSize().height-20), delta);
    } else if (fit_object.compare("height")==0) {
        onShowRightBar(0, 0.25, [=](Node* node){
            Math_Answer* answer = (Math_Answer*) node;
            bool correct = true;
            for (Math_Slot* slot : _slots) {
                if (!slot->isCorrect(compare_key)) {
                    correct = false;
                    break;
                }
            }
            answer->onAnimationClick(correct);
            this->onExplanation(correct);
        });
        Math_Quiz::onStart();
        float delta = (quiz_size.height - getQuestion()->getContentSize().height-20 - max_in_line * object_size.height) / (max_in_line+1);
        delta = delta > 100 ? 100 : delta;
        math::func::smartSetPosition(_slots, object_size, cocos2d::Rect(0, 0, quiz_size.width*0.75, quiz_size.height - getQuestion()->getContentSize().height-20), delta, false);
    }

    math::func::shuffle(_objects);
    for (int i=0; i<number_object; i++) {
        _slots[i]->setLinker(_objects[i]);
        this->addChildNode(_slots[i]);
        _slots[i]->onShow(0);
        
        Math_Object* object = _objects[i];
        if (fit_object.compare("width")==0) {
            object->setMoveDirection(1);
            auto delta_height = slot_size.height - object->getContentSize().height;
            object->setPosition(cocos2d::Vec2(_slots[i]->getPosition().x, _slots[i]->getPosition().y - delta_height/2));
        } else if (fit_object.compare("height")==0) {
            object->setMoveDirection(2);
            auto delta_width = slot_size.width - object->getContentSize().width;
            object->setPosition(cocos2d::Vec2(_slots[i]->getPosition().x - delta_width/2, _slots[i]->getPosition().y));
        }
        addChildNode(object, 2);
        object->onShow(i*0.1);
    }
}

void SortObjectInOrderQuiz::startQuestion() {
     
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (Math_Object* object : _objects) {
            object->enableDrag(_panel);
        }
    }
}

void SortObjectInOrderQuiz::onExplanation(bool correct) {
     
    disableControlQuiz();
    scheduleOnce([=](float dt){
        if (!correct) {
            for (Math_Object* object : answer_list) {
                object->setSelected(false);
            }
            answer_list.clear();
            enableControlQuiz();
        } else {
            this->nextQuestion();
        }
    }, 1.0, "selected_anim");
}

void SortObjectInOrderQuiz::onComplete() {
    for (int i=0; i < _slots.size(); i++) {
        Math_Slot* slot = _slots[i];
        slot->onHide(i*0.1);
    }
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 0.5, "complete_quiz");
}

#pragma mark - object touch events callback
void SortObjectInOrderQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
    //Danh cho game sap xep lai vi tri roi chon dap an
    Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    
    Math_Slot* t_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    
    if (t_slot == nullptr) {
        //Move ve vi tri cu
        object->backToLast(2);
    } else {
        Math_Slot* m_slot = nullptr;
        for (Math_Slot* slot : _slots) {
            if (object->isSame(slot->getLinker())) {
                m_slot = slot;
                break;
            }
        }

        if (m_slot == t_slot) {
            object->backToLast(2);
        } else {
            //swap
            Math_Object* object_2 = t_slot->getLinker();
            
            m_slot->removeLinker();
            t_slot->removeLinker();
            
            t_slot->setLinker(object);
            m_slot->setLinker(object_2);
            
            if (fit_object.compare("width")==0) {
                object->runAction(cocos2d::MoveTo::create(0.1, cocos2d::Vec2(t_slot->getPosition().x, object->getPosition().y)));
                object_2->runAction(cocos2d::MoveTo::create(0.25, cocos2d::Vec2(m_slot->getPosition().x, object_2->getPosition().y)));
            } else if (fit_object.compare("height")==0) {
                object->runAction(cocos2d::MoveTo::create(0.1, cocos2d::Vec2(object->getPosition().x, t_slot->getPosition().y)));
                object_2->runAction(cocos2d::MoveTo::create(0.25, cocos2d::Vec2(object_2->getPosition().x, m_slot->getPosition().y)));
            }
        }
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void SortObjectInOrderQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void SortObjectInOrderQuiz::autoTestingRun(float delaytime)
{
    
}

