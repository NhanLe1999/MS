//
//  PatternCopyQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#include "PatternCopyQuiz.h"

PatternCopyQuiz* PatternCopyQuiz::createQuiz()
{
    PatternCopyQuiz* quiz=new PatternCopyQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

PatternCopyQuiz::~PatternCopyQuiz()
{
    if(pattern)
    {
        CC_SAFE_DELETE(pattern);
    }
}

bool PatternCopyQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void PatternCopyQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_object = randomRangeJson(json_game, "number");
    number_slot = number_object;
    
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
            pattern->addPatternSize(i);
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
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen doi tuong
    math::resource::Image* slot_bg = slotBG();
    img_bg = objectBG();
    if (object_bg == "none") {
        img_bg = nullptr;
    }
    auto drag_bg = objectBG();
    
    for (int i=0; i<number_object; i++) {
        Math_Object* object=mpool->getObject();
        object->setEnable(i, math::common::deny, pattern->getImageAt(i), img_bg);
        object->setComparisionValue(pattern->getSizeLevelAt(i));
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
        object->setObjectSize(object_size * .8, object_align_bot);
        _objects.push_back(object);
        
        Math_Slot* slot = mpool->getSlot();
        slot->setEndble(i, slot_bg);
        slot->setSlotSize(object_size * 0.95);
        slot->setComparisionValue(object->getComparisionValue());
        _slots.push_back(slot);
    }
    
    for (int i=0; i<pattern->piece; i++) {
        Math_Object* dragger=mpool->getObject();
        dragger->setEnable(i+number_object, math::common::move, pattern->match_icon[i], drag_bg);
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
        dragger->setMoveCallback(CC_CALLBACK_1(PatternCopyQuiz::onTouchedObject, this), CC_CALLBACK_2(PatternCopyQuiz::onMoveObjectEnded, this));
        _dragers.push_back(dragger);
    }
    math::func::shuffle(_dragers);    
}

void PatternCopyQuiz::onStart() {
    int number_dr = (int)_dragers.size();
    onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(PatternCopyQuiz::onAnswerDone, this));
    quizBegan(0, true);
    
    float delta_dr = (quiz_size.width - 330 - number_dr * object_size.width) /(number_dr+1);
    delta_dr = delta_dr > 100 ? 100 : delta_dr;
    math::func::smartSetPosition(_dragers, object_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height*0.3-30), delta_dr);
    for (int i=0; i<number_dr; i++) {
        addChildNodeToBottomPanel(_dragers[i]);
        _dragers[i]->onShow(0.5+i*0.1, math::animation::SlideUp);
    }
    
    float delta = (quiz_size.width - number_object * object_size.width) / (number_object+1);
    delta = delta > 100 ? 100 : delta;
    math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.7 - getQuestion()->getContentSize().height-20), delta);
    
    for (int i=0; i<number_object; i++) {
        auto pos = _objects[i]->getPosition();
        _objects[i]->setPosition(cocos2d::Vec2(pos.x, pos.y + object_size.width*0.7));
        addChildNode(_objects[i]);
        _objects[i]->onShow(0.1*i);
        
        _slots[i]->setPosition(cocos2d::Vec2(pos.x, pos.y - object_size.width*0.7));
        addChildNode(_slots[i]);
        _slots[i]->onShow(0.1*i, math::animation::SlideUp);
    }
}

void PatternCopyQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        scheduleOnce([=](float){
            for (Math_Object* object : _dragers) {
                object->enableDrag(_panel_bot);
            }
        }, 0.5, "start quiz");
    }
}

void PatternCopyQuiz::onExplanation(bool correct) {
    if (!correct) {
        //return wrong drager
    } else {
        this->nextQuestion(1.0);
    }
}

void PatternCopyQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    float delay = 1.0;
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }

    for (Math_Object* object : _objects) {
        object->onHide(delay-0.5);
    }
    for (Math_Slot* slot :  _slots) {
        slot->onHide(delay-0.25);
    }
    for (Math_Object* object : _dragers) {
        object->onHide(delay);
    }
    
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
void PatternCopyQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=(Math_Object*)node;
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

void PatternCopyQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
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

void PatternCopyQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool done=true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isCorrect(key_compare)) {
            done=false;
        }
    }
    answer->onAnimationClick(done);
    if (done) {
        disableControlQuiz();
        this->onExplanation(done);
    } else {
        disableControlQuiz(0.25);
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() && !slot->isCorrect(key_compare)) {
                slot->getLinker()->onHide(0, math::animation::ScaleUpAndRotation);
                slot->removeLinker();
            }
        }
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void PatternCopyQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void PatternCopyQuiz::autoTestingRun(float delaytime)
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

