//
//  MoveObjectToRightPlaceQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "MoveObjectToRightPlaceQuiz.h"

MoveObjectToRightPlaceQuiz* MoveObjectToRightPlaceQuiz::createQuiz()
{
    MoveObjectToRightPlaceQuiz* quiz=new MoveObjectToRightPlaceQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MoveObjectToRightPlaceQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void MoveObjectToRightPlaceQuiz::initQuiz(mj::model::Game game)
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
    
    number_object = 3;
    fix_ask_pos = getJsonInt(json_game, "fix_ask_pos", number_object);
    if (fix_ask_pos > 1) {
        fix_ask_pos = cocos2d::random(0, 1);
    }
    
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
    std::string icon_same = getJsonString(json_icon, "same", "none");
    std::string icon_diff = getJsonString(json_icon, "different", "id");
    auto all_imgs = getImageList("icons");
    all_imgs = math::resource::ResHelper::filterImages(all_imgs, icon_same);
    
    std::vector<math::resource::Image*> img_question;
    
    object_size = cocos2d::Size(150, 150);
    if (question_type == QT_Above_Below || question_type == QT_Top_Bottom) {
        object_size = cocos2d::Size(150, 150);
    }
    
    //img làm mốc: index = 1, img kéo thả: index = 0
    img_list = getImageList(all_imgs, 2, icon_diff);
    if (question_type == QT_NextTo) {
        if (fix_ask_pos==0) {
            index_pivot = 0;
            index_correct = 1;
        } else {
            index_pivot = 1;
            index_correct = 2;
        }
    } else {
        index_pivot = 1;
        if (fix_ask_pos==0) {
            index_correct = 0;
        } else {
            index_correct = 2;
        }
    }
    img_question = img_list;
    
    auto img_slot = slotBG();
    auto img_drag = objectBG();

    for (int i2=0; i2 < number_object; i2++) {
        auto object = mpool->getObject();
        object->setEnable(i2, math::common::deny, img_list.back(), nullptr);
        object->setObjectSize(object_size);
        _objects.push_back(object);
        if (i2 != index_pivot) {
            object->setImageVisible(false);
            
            auto slot = mpool->getSlot();
            slot->setEndble(i2, img_slot);
            slot->setSlotSize(object_size);
            _slots.push_back(slot);
        }
    }
    
    auto object = mpool->getObject();
    object->setEnable(0, math::common::touch, img_list.front(), img_drag);
    object->setObjectSize(object_size);
    object->setMoveCallback(nullptr, CC_CALLBACK_2(MoveObjectToRightPlaceQuiz::onMovedObject, this));
    _dragers.push_back(object);

    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_question);
    }
    getQuestion()->setKeywordValue(fix_ask_pos);
}

void MoveObjectToRightPlaceQuiz::onStart() {
    
    if (question_type == QT_Left_Right || question_type == QT_NextTo) {
        float bar_height = 0.3;
        onShowBottomBar(0, bar_height, true, CC_CALLBACK_1(MoveObjectToRightPlaceQuiz::onMoveDone, this));
        Math_Quiz::onStart();
        
        cocos2d::Vec2 center_view = cocos2d::Vec2(quiz_size.width , quiz_size.height * (1-bar_height)- getQuestion()->getContentSize().height - 20) / 2;
        
        float delta = 150;
        float start_x = center_view.x - object_size.width - delta;
        for (int i=0; i<number_object; i++) {
            _objects[i]->setPosition(cocos2d::Vec2(start_x + (object_size.width + delta) * i, center_view.y));
            addChildNode(_objects[i]);
            _objects[i]->onShow(i*0.1);
        }
        
        auto drag = _dragers.front();
        addChildNodeToBottomPanel(drag);
        drag->setPosition(cocos2d::Vec2((quiz_size.width - 200) * 0.5, (quiz_size.height * bar_height - 30) * 0.5));
        drag->onShow(0.5, math::animation::SlideUp);
    } else {
        float bar_width = 275.0 / quiz_size.width;
        onShowRightBar(0, bar_width, CC_CALLBACK_1(MoveObjectToRightPlaceQuiz::onMoveDone, this));
        Math_Quiz::onStart();
        
        cocos2d::Vec2 center_view = quiz_size/2;
        center_view = cocos2d::Vec2(quiz_size.width * (1-bar_width), quiz_size.height - getQuestion()->getContentSize().height - 20) / 2;
        
        float delta = 50;
        float start_y = center_view.y + object_size.height + delta;
        for (int i=0; i<number_object; i++) {
            _objects[i]->setPosition(cocos2d::Vec2(center_view.x, start_y - (object_size.height + delta) * i));
            addChildNode(_objects[i]);
            _objects[i]->onShow(i*0.1);
        }
        
        auto drag = _dragers.front();
        drag->setPosition(cocos2d::Vec2(quiz_size.width * bar_width * 0.5, (quiz_size.height + 100) * 0.5));
        addChildNodeToRightPanel(drag);
        drag->onShow(0.5, math::animation::SlideLeft);
    }
    
    for (Math_Slot* slot : _slots) {
        slot->setPosition(_objects[slot->getId()]->getPosition());
        addChildNode(slot);
        slot->onShow(slot->getId()*0.1);
    }
}

void MoveObjectToRightPlaceQuiz::startQuestion() {
    enableControlQuiz();
    
    scheduleOnce([=](float){
        if (question_type == QT_Left_Right || question_type == QT_NextTo) {
            _dragers.front()->enableDrag(_panel_bot);
        } else {
            _dragers.front()->enableDrag(_panel_right);
        }
    }, 0.5, "complete");
    
}

void MoveObjectToRightPlaceQuiz::onExplanation(bool correct) {
    if (!correct) {
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed()) {
                auto object = slot->getLinker();
                slot->removeLinker();
                object->backToStart();
                break;
            }
        }
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void MoveObjectToRightPlaceQuiz::onComplete() {
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(i*0.1, math::animation::ScaleUp);
    }
    
    for (Math_Slot* slot : _slots) {
        slot->onHide(slot->getId() * 0.1, math::animation::ScaleUp);
    }

    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void MoveObjectToRightPlaceQuiz::onMovedObject(cocos2d::Node *node, cocos2d::Vec2 position) {
    Math_Object* object=(Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    //Doi tuong keo tha nam trong 1 slot nao do.
    Math_Slot* m_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            m_slot = slot;
            break;
        }
    }
    
    Math_Slot* t_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    
    if (m_slot!=nullptr) {
        if (t_slot != nullptr) {
            //swap
            if (m_slot == t_slot) {
                object->backToLast(2);
            } else {
                object->removeFromParentAndCleanup(false);
                addChildNode(object);
                object->setPosition(position);
                m_slot->removeLinker();
                t_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            }
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
        }
    } else {
        if (t_slot != nullptr) {
            object->removeFromParentAndCleanup(false);
            addChildNode(object);
            object->setPosition(position);
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            //move start
            object->backToStart();
        }
    }
}

void MoveObjectToRightPlaceQuiz::onMoveDone(Node* node) {
    auto answer = static_cast<Math_Answer*>(node);
    
    bool correct = false;
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed() && slot->getId() == index_correct) {
            correct = true;
            break;
        }
    }
    
    answer->onAnimationClick(correct);
    
    scheduleOnce([=](float){
        this->onExplanation(correct);
    }, 0.5, "wait explain");    
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MoveObjectToRightPlaceQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MoveObjectToRightPlaceQuiz::autoTestingRun(float delaytime) {
    
}
