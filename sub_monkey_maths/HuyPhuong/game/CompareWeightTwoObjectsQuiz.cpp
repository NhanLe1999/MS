//
//  CompareWeightTwoObjectsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "CompareWeightTwoObjectsQuiz.h"

CompareWeightTwoObjectsQuiz* CompareWeightTwoObjectsQuiz::createQuiz()
{
    CompareWeightTwoObjectsQuiz* quiz=new CompareWeightTwoObjectsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CompareWeightTwoObjectsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CompareWeightTwoObjectsQuiz::initQuiz(mj::model::Game game)
{
     
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    drag_object = json_game.FindMember("game_play")->value.GetBool();
    rapidjson::Value& compare_json = json_game.FindMember("compare")->value;
    std::string compare_string = (compare_json[cocos2d::random(0, (int)compare_json.Size()-1)]).GetString();
    isBig = compare_string.compare("Big")==0;
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("height")->value.GetInt());
    background_key = json_object.FindMember("background")->value.GetString();
    fit_object = json_object.FindMember("fit_size")->value.GetString();
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string img_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    
    img_list = getImageList("icons", 2, icon_filter, img_same);
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue(isBig?1:0);
    
    //TODO: GENERATE QUIZ COMPONENTS
    float balance_y = 180;
    if (drag_object) {
        balance_y = 155;
    }
    
    balance=cocos2d::ui::ImageView::create(getUIPath("gameui/scale/body.png"));
    _panel->addChild(balance, 2);
    balance_scales=cocos2d::ui::ImageView::create(getUIPath("gameui/scale/line.png"));
    _panel->addChild(balance_scales, 1);
    balance_hand=cocos2d::ui::ImageView::create(getUIPath("gameui/scale/hand.png"));
    _panel->addChild(balance_hand, 3);
    
    for (int i=0; i<2; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::touch, img_list[i], nullptr);
        object->setObjectSize(object_size);
        if (fit_object.compare("width")==0) {
            object->fixWidthObject();
        } else if (fit_object.compare("height")==0) {
            object->fixHeightObject();
        }
        object->setTouchCallback(CC_CALLBACK_1(CompareWeightTwoObjectsQuiz::onTouchObject, this));
        object->setMoveCallback(nullptr, CC_CALLBACK_2(CompareWeightTwoObjectsQuiz::onMoveObjectEnded, this));
        _objects.push_back(object);
        
        Math_Slot* slot = mpool->getSlot();
        slot->setEndble(i, slotBG());
        slot->setSlotSize(object_size);
        slot->setBackgroundVisible(false);
        _slots.push_back(slot);
    }
}

void CompareWeightTwoObjectsQuiz::onStart() {
    
    Math_Quiz::onStart();
    math::func::shuffle(_objects);
    cocos2d::Vec2 balance_center = cocos2d::Vec2(quiz_size.width/2, 200);
    
    for (int i = 0; i<2; i++) {
        auto slot = _slots[i];
        slot->setPosition(cocos2d::Vec2(100 + i* 677, 135));
        balance_scales->addChild(slot);
        slot->onShow(0, math::animation::No_Effect);
    }

    if (drag_object) {
        float width = _objects[0]->getContentSize().width*2/900 + 0.1;
        math::func::setPositionList(_objects, 2, cocos2d::Rect((1-width)/2, 0.6, width, 0.3));
        for (int i=0; i<2; i++) {
            this->addChildNode(_objects[i]);
            _objects[i]->onShow(0.3);
        }
    } else {
        balance_center = cocos2d::Vec2(quiz_size.width/2, 250);
        for (int i=0; i<2; i++) {
            _slots[i]->setLinker(_objects[i]);
            _objects[i]->setPosition(_slots[i]->getPosition());
            balance_scales->addChild(_objects[i]);
            _objects[i]->onShow(0.5);
        }
    }
    
    balance->setPosition(balance_center);
    balance_scales->setPosition(cocos2d::Vec2(balance_center.x, balance_center.y+153));
    balance_hand->setPosition(cocos2d::Vec2(balance_center.x, balance_center.y-65));
}

void CompareWeightTwoObjectsQuiz::startQuestion() {
     

    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        if (drag_object) {
            for (Math_Object* object : _objects) {
                object->enableDrag(_panel);
            }
        } else {
            //CHECK BALANCE
            int A=0;
            int B=0;
            if (_slots[0]->isUsed()) {
                A=1;
            }
            if (_slots[1]->isUsed()) {
                B=1;
            }
            
            int balance=_slots[0]->getLinker()->compare(_slots[1]->getLinker(), math::comparision::weight);
            balance_scales->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, -10*balance)));
            balance_hand->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, -10*balance)));
        }
    }
}

void CompareWeightTwoObjectsQuiz::onExplanation(bool correct) {
     
    scheduleOnce([=](float){
        for (Math_Object* object : _objects) {
            object->setSelected(false);
        }
    }, 1.0, "un selected");

    if (!correct) {
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}

void CompareWeightTwoObjectsQuiz::onComplete() {
    
    float m_delay= 1.0;
    
    balance->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(m_delay), cocos2d::FadeOut::create(0.2)));
    balance_scales->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(m_delay), cocos2d::FadeOut::create(0.2)));
    balance_hand->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(m_delay), cocos2d::FadeOut::create(0.2)));

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
        object->onHide(m_delay+(random_hide[i]/number_div)*0.1);
    }
    
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.5, "complete_quiz");
}

#pragma mark - object touch events callback
void CompareWeightTwoObjectsQuiz::onTouchObject (Node* node) {
    Math_Object* object = (Math_Object*) node;
    bool is_correct=true;
    object->setSelected(true);
    
    if (!isBig) {
        if (is_correct) {
            for (Math_Object* obj : _objects) {
                if (!object->isSame(obj) && object->compare(obj, math::comparision::weight) >= 0) {
                    is_correct=false;
                    break;
                }
            }
        }
        onExplanation(is_correct);
        return;
    } else {
        //So dap an =1
        if (is_correct) {
            for (Math_Object* obj : _objects) {
                if (!object->isSame(obj) && object->compare(obj, math::comparision::weight) <= 0) {
                    is_correct=false;
                    break;
                }
            }
        }
        onExplanation(is_correct);
        return;
    }
}

void CompareWeightTwoObjectsQuiz::onMoveObjectEnded(Node* node, cocos2d::Vec2 position) {
    
    Math_Object* object=(Math_Object*)node;
    
    cocos2d::Vec2 balance_position=balance_scales->convertToNodeSpace(_panel->convertToWorldSpace(position));
    bool detect_slot = false;
    
    Math_Slot *m_slot = nullptr;
    
    for (Math_Slot *slot : _slots) {
        if (slot->isAvailable(balance_position)) {
            m_slot = slot;
            detect_slot = true;
            break;
        }
    }
    
    if (detect_slot) {
        if (m_slot->isUsed()) {
            Math_Object* linker = m_slot->getLinker();
            linker->backToStart();
            m_slot->removeLinker();
        }
        m_slot->setLinker(object);
        object->removeFromParentAndCleanup(false);
        object->setPosition(balance_position);
        balance_scales->addChild(object);
        object->runAction(cocos2d::MoveTo::create(0.1, m_slot->getPosition()));
    } else {
        //move back
        object->removeFromParentAndCleanup(false);
        object->setPosition(position);
        _panel->addChild(object);
        object->backToStart();
        
        for (Math_Slot *slot : _slots) {
            if (object->isSame(slot->getLinker())) {
                slot->removeLinker();
                break;
            }
        }
    }
    
    //CHECK BALANCE
    int A=0;
    int B=0;
    if (_slots[0]->isUsed()) {
        A=1;
    }
    if (_slots[1]->isUsed()) {
        B=1;
    }
    
    if (A*B>0) {
        int balance=_slots[0]->getLinker()->compare(_slots[1]->getLinker(), math::comparision::weight);
        balance_scales->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, -10*balance)));
        balance_hand->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, -10*balance)));
        
        for (Math_Object* c_obj : _objects) {
            c_obj->setTouch(math::common::touch);
        }
                            
        this->scheduleOnce([=](float dt) {
            this->onExplanation(true);
        }, 2.0, "switch_choose");
    } else {
        balance_scales->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, 15*(B-A))));
        balance_hand->runAction(cocos2d::EaseBounceOut::create(cocos2d::RotateTo::create(1.5, 15*(B-A))));
    }
}
#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void CompareWeightTwoObjectsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}



void CompareWeightTwoObjectsQuiz::autoTestingRun(float delaytime) {
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

