//
//  PatternComeNextQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#include "PatternComeNextQuiz.h"

PatternComeNextQuiz* PatternComeNextQuiz::createQuiz()
{
    PatternComeNextQuiz* quiz=new PatternComeNextQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

PatternComeNextQuiz::~PatternComeNextQuiz()
{
    if(pattern)
    {
        CC_SAFE_DELETE(pattern);
    }
}

bool PatternComeNextQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void PatternComeNextQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    touch_game = getJsonBool(json_game, "game_play", true);
    comenext_game = getJsonBool(json_game, "missing_type", true);
    number_object = randomRangeJson(json_game, "number");
    number_slot = randomRangeJson(json_game, "missing");
    
    if (number_slot > 1) {
        touch_game=false;
    }
    
    if (comenext_game) {
        for (int i=number_object-number_slot; i<number_object; i++) {
            missing_index.push_back(i);
        }
    } else {
        missing_index = randomArrayIndexes(number_slot, number_object);
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_size = getSizeJson(json_object, "size");
    object_bg = getJsonString(json_object, "background", "none");
    
    //TODO: PATTERN CONFIG
    rapidjson::Value& json_pattern=json_data.FindMember("pattern")->value;
    int pattern_piece = randomRangeJson(json_pattern, "piece");
    int pattern_size = randomRangeJson(json_pattern, "size");
    std::string pattern_type = getJsonString(json_pattern, "type", "icon");
    std::string pattern_kind = getJsonString(json_pattern, "class", "normal");
    if (pattern_kind == "growing") {
        pattern_type = pattern_kind;
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();

    img_list = getImageList("icons", pattern_piece, icon_filter, icon_same);
    pattern=new PatternCreator(pattern_piece, pattern_size, pattern_type);
    if (pattern_type.compare("size")==0) {
        key_compare = math::comparision::size;
        object_align_bot = true;
    } else if (pattern_type == "position") {
        key_compare = math::comparision::position;
    }
    
    is_flip_x = cocos2d::random(0, 1) == 0;
    for (int i=0; i<pattern_piece; i++) {
        pattern->addPatternImage(img_list[i]);
        if (pattern_type.compare("size")==0) {
            pattern->addPatternSize(i*2);
        } else if (pattern_type.compare("position")==0) {
            pattern->addPatternSize(0);
            pattern->addPatternPosition(i);
        } else {
            pattern->addPatternSize(0);
        }
    }
    if (pattern_kind == "growing") {
        pattern->setGrowingType(PatternCreator::PatternGrowingType::Pattern_3);
    }
    pattern->setPatternListSize(number_object);
    /*---> Lay xong data cua game*/
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //TODO: GENEEATE QUIZ COMPONENTS
    //Gen doi tuong
    math::resource::Image* slot_bg = slotBG();
    img_bg = objectBG();
    if (object_bg == "none") {
        img_bg = nullptr;
    }
    auto dragger_bg = objectBG();

    
    for (int i=0; i<number_object; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, pattern->getImageAt(i), img_bg);
        
        bool is_missing = false;
        for (int index : missing_index) {
            if (i==index) {
                is_missing=true;
                break;
            }
        }
        
        if (key_compare == math::comparision::size) {
            object->setComparisionValue(pattern->getSizeLevelAt(i));
        }
        if (key_compare == math::comparision::position) {
            if (pattern->getPositionAt(i)!=0) {
                if (is_flip_x) {
                    object->getImage()->setFlippedX(true);
                } else {
                    object->getImage()->setFlippedY(true);
                }
            }
            object->setComparisionValue("position", cocos2d::StringUtils::toString(pattern->getPositionAt(i)));
        }
        
        if (!is_missing) {
            object->setObjectSize(object_size * .8, object_align_bot);
            _objects.push_back(object);
            _slots_objects.push_back(object);
        } else {
            Math_Slot* slot = mpool->getSlot();
            slot->setEndble(i, slot_bg);
            slot->setSlotSize(object_size);
            slot->setComparisionValue(object->getComparisionValue());
            _slots.push_back(slot);
            _slots_objects.push_back(slot);
        }
    }
    
    for (int i=0; i<pattern->piece; i++) {
        Math_Object* dragger=mpool->getObject();
        dragger->setEnable(i+number_object, math::common::touch, pattern->match_icon[i], dragger_bg);
        if (key_compare == math::comparision::size) {
            dragger->setComparisionValue(pattern->match_size[i]);
        }
        if (key_compare == math::comparision::position) {
            if (pattern->match_position[i]!=0) {
                if (is_flip_x) {
                    dragger->getImage()->setFlippedX(true);
                } else {
                    dragger->getImage()->setFlippedY(true);
                }
            }
            dragger->setComparisionValue("position", cocos2d::StringUtils::toString(pattern->match_position[i]));
        }
        
        dragger->setObjectSize(object_size, object_align_bot);
        
        if (touch_game) {
            dragger->setTouchCallback(CC_CALLBACK_1(PatternComeNextQuiz::onTouchedObject, this));
        } else {
            dragger->setMoveCallback(CC_CALLBACK_1(PatternComeNextQuiz::onTouchedObject, this), CC_CALLBACK_2(PatternComeNextQuiz::onMoveObjectEnded, this));
        }
        _dragers.push_back(dragger);
    }
    math::func::shuffle(_dragers);
}

void PatternComeNextQuiz::onStart() {
    int number_dr = (int)_dragers.size();
    if (touch_game) {
        onShowBottomBar(0, 0.3, true);
        float delta_dr = (quiz_size.width - number_dr * object_size.width) /(number_dr+1);
        delta_dr = delta_dr > 100 ? 100 : delta_dr;
        math::func::smartSetPosition(_dragers, object_size, cocos2d::Rect(40, 0, quiz_size.width-80, quiz_size.height*0.3-30), delta_dr);
    } else {
        onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(PatternComeNextQuiz::onAnswerDone, this));
        float delta_dr = (quiz_size.width - 330 - number_dr * object_size.width) /(number_dr+1);
        delta_dr = delta_dr > 100 ? 100 : delta_dr;
        math::func::smartSetPosition(_dragers, object_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height*0.3-30), delta_dr);
    }
    quizBegan(0, true);
    
    for (int i=0; i<number_dr; i++) {
        addChildNodeToBottomPanel(_dragers[i]);
        _dragers[i]->onShow(0.5+i*0.1, math::animation::SlideUp);
    }
    
    float delta = (quiz_size.width - number_object * object_size.width) / (number_object+1);
    delta = delta > 100 ? 100 : delta;
    math::func::smartSetPosition(_slots_objects, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.7 - getQuestion()->getContentSize().height-20), delta);
    
    for (Math_Object* object : _objects) {
        addChildNode(object);
        object->onShow(0.2, math::animation::SlideUp);
    }
    for (Math_Slot* slot :  _slots) {
        this->addChildNode(slot, 2);
        slot->onShow(0.2, math::animation::SlideUp);
    }
}

void PatternComeNextQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        if (!touch_game) {
            scheduleOnce([=](float){
                for (Math_Object* object : _dragers) {
                    object->enableDrag(_panel_bot);
                }
            }, 0.5, "srart game");
        }
    }
}

void PatternComeNextQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        nextQuestion(0.75);
    }
}

void PatternComeNextQuiz::onComplete() {
    
    float delay = 0;
    for (Math_Object* object : _dragers) {
        object->onHide(delay, math::animation::SlideUp);
        delay += 0.1;
    }
    for (Math_Object* object : _objects) {
        object->onHide(0.2, math::animation::SlideUp);
    }
    for (Math_Slot* slot :  _slots) {
        slot->onHide(0.2, math::animation::SlideUp);
    }
    
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
void PatternComeNextQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=(Math_Object*)node;
    if (touch_game) {
        _slots[0]->setLinker(object);
        bool correct = _slots.front()->isCorrect(key_compare);
        object->onTouchObject(correct);
        if (correct) {
            disableControlQuiz();
            this->onExplanation(true);
        } else {
            disableControlQuiz(0.5);
        }
    } else {
        make_copy=true;
        for (Math_Slot* slot : _slots) {
            if (object->isSame(slot->getLinker())) {
                make_copy = false;
                break;
            }
        }
        
        if (make_copy) {
            for (int i=0; i<_dragers.size(); i++) {
                if (object->isSame(_dragers[i])) {
                    _dragers.erase(_dragers.begin()+1);
                    break;
                }
            }
            auto cloned = object->clone();
            cloned->setObjectSize(object_size, object_align_bot);
            cloned->onShow(0, math::animation::No_Effect);
            _dragers.push_back(cloned);
        }
    }
}

void PatternComeNextQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
    Math_Object* object=(Math_Object*)node;
    
    position = _panel->convertToNodeSpace(position);
    
    Math_Slot* m_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed() && object->isSame(slot->getLinker())) {
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
    
    if (t_slot == nullptr) {
        if (m_slot != nullptr) {
            m_slot->removeLinker();
        }
        object->onHide(0, math::animation::ScaleUpAndRotation);
    } else {
        if (m_slot == nullptr) {
            object->removeFromParentAndCleanup(false);
            object->setPosition(position);
            addChildNode(object, 10);
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->onHide(0, math::animation::ScaleUpAndRotation);
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            object->removeFromParentAndCleanup(false);
            object->setPosition(position);
            addChildNode(object, 10);
            
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                m_slot->removeLinker();
                t_slot->removeLinker();
                
                t_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                
                m_slot->setLinker(linker);
                linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
            } else {
                m_slot->removeLinker();
                t_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            }
        }
    }
}

void PatternComeNextQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool done=true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isCorrect(key_compare)) {
            done=false;
        }
    }
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void PatternComeNextQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void PatternComeNextQuiz::autoTestingRun(float delaytime)
{
    //int index_ques=this->getQuestion()->getData().index;
    
    //Chua keo tha xong, tiep tuc keo tha
    //Chon 1 obj chua keo tha.
    std::vector<Math_Object*> object_list;
    bool finded;
    for (Math_Object* object : _dragers) {
        finded=false;
        for (Math_Slot* slot : _slots) {
            if (object->isSame(slot->getLinker())) {
                finded=true;
                break;
            }
        }
        if (!finded) {
            object_list.push_back(object);
        }
    }
    Math_Object* m_object=math::func::getRandomNode(object_list);
    
    //Chon danh sach slot chua keo tha
    std::vector<Math_Slot*> slot_list;
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed()) {
            slot_list.push_back(slot);
        }
    }
    Math_Slot* m_slot=math::func::getRandomNode(slot_list);
    
    //move object to slot
    m_slot->setLinker(m_object);
    m_object->runAction(cocos2d::MoveTo::create(0.4, m_slot->getPosition()));
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(delaytime);
    }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
}

