////////
//        Minh_FillTimeShowAtClockQuiz.cpp
//        MJ_Game
//        Dev By MinhTzy on 5/3/2018
//

#include "Minh_FillTimeShowAtClockQuiz.h"

#define text_size 70

Minh_FillTimeShowAtClockQuiz * Minh_FillTimeShowAtClockQuiz::createQuiz()
{
    Minh_FillTimeShowAtClockQuiz * quiz = new Minh_FillTimeShowAtClockQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return nullptr;
}
bool Minh_FillTimeShowAtClockQuiz::init()
{
    return cocos2d::ui::Layout::init();
}
void Minh_FillTimeShowAtClockQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value & json_data = document.FindMember("data")->value;
    // TODO : Game Config
    
    rapidjson::Value & game_data = getJsonMember(json_data, "game");
    // Config clock
    use_text = getJsonBool(getJsonMember(game_data, "clock"), "use_text", true);
    use_roman = getJsonBool(getJsonMember(game_data, "clock"), "use_roman", false);
    
    // Config game
    fill_hour = getJsonBool(getJsonMember(game_data, "fill"), "fill_hour", true);
    fill_minute = getJsonBool(getJsonMember(game_data, "fill"), "fill_minute", false);
    fill_random = getJsonBool(getJsonMember(game_data, "fill"), "random", false);
    
    round_minute = getJsonInt(game_data, "round_minute", 5);
    level = getJsonString(game_data, "level", "easy");
    //------> Lay xong data cua game
    
    // TODO : GEN QUIZ COMPONENTS
    int obj_id = 0;
    math::resource::Image * slot_bg = slotBG();
    math::resource::Image * drag_bg = objectBG();
    // GEN TIME
    
    int hour = cocos2d::random<int>(1, 12);
    int minute = (cocos2d::random<int>(0, 59) / round_minute) * round_minute;
    
    if (level.compare("easy") == 0) {
        drager_size = cocos2d::Size(120, 120);
        num_slot_fill = 1;
        is_clone_drag = false;
        if (fill_random) {
            bool r = cocos2d::random(0, 1) == 1;
            fill_hour = r;
            fill_minute = !r;
        }
        fill_hour ? _hours_show.push_back(0) : _hours_show.push_back(1);
        fill_minute ? _minutes_show.push_back(0) : _minutes_show.push_back(1);
    }
    else {
        num_slot_fill = 2;
        is_clone_drag = true;
        
        fill_hour ? _hours_show.push_back(0) : _hours_show.push_back(1);
        fill_hour ? _hours_show.push_back(0) : _hours_show.push_back(1);
        fill_minute ? _minutes_show.push_back(0) : _minutes_show.push_back(1);
        fill_minute ? _minutes_show.push_back(0) : _minutes_show.push_back(1);
        
        // Fix cho th random
        if (fill_random) {
            fill_hour = true;
            fill_minute = true;
            _hours_show.clear();
            _minutes_show.clear();
            int r = cocos2d::random((hour < 10 ? 1 : 0), 3);
            r == 0 ? _hours_show.push_back(0) : _hours_show.push_back(1);
            r == 1 ? _hours_show.push_back(0) : _hours_show.push_back(1);
            r == 2 ? _minutes_show.push_back(0) : _minutes_show.push_back(1);
            r == 3 ? _minutes_show.push_back(0) : _minutes_show.push_back(1);
        }
    }
    
    // Gen time object
    hour_object = mpool->getObject();
    hour_object->setEnable(obj_id++, math::common::deny, getNumbers(hour, hour)[0], drag_bg);
    hour_object->setObjectSize(drager_size);
    hour_object->getLabelValue()->setFontSize(text_size);
    hour_object->getLabelValue()->setString(cocos2d::StringUtils::format("%02d", hour));
    hour_object->setImageVisible(false);
    hour_object->showValueTextWithAnimation(true);
    _hour_objects.push_back(hour_object);
    _objects.push_back(hour_object);
    
    minute_object = mpool->getObject();
    minute_object->setEnable(obj_id++, math::common::deny, getNumbers(minute, minute)[0], drag_bg);
    minute_object->setObjectSize(drager_size);
    minute_object->getLabelValue()->setFontSize(text_size);
    minute_object->getLabelValue()->setString(cocos2d::StringUtils::format("%02d", minute));
    minute_object->setImageVisible(false);
    minute_object->showValueTextWithAnimation(true);
    _minute_objects.push_back(minute_object);
    _objects.push_back(minute_object);
    
    device_object = mpool->getObject();
    device_object->setEnable(obj_id++, math::common::deny, getOperator(":"), NULL);
    device_object->setObjectSize(drager_size);
    device_object->getLabelValue()->setFontSize(text_size);
    device_object->getLabelValue()->setString(":");
    device_object->setImageVisible(false);
    device_object->showValueTextWithAnimation(true);
    _objects.push_back(device_object);
    
    // Gen time fill;
    _hours_fill.clear();
    _minutes_fill.clear();
    if (fill_hour) {
        _hours_fill.push_back(hour);
        while (_hours_fill.size() < 4) {
            int r = cocos2d::random<int>(1, 12);
            bool add = true;
            for (int n : _hours_fill) {
                
                if (r == n) {
                    add = false;
                    break;
                }
            }
            if (add) _hours_fill.push_back(r);
        }
        math::func::shuffle(_hours_fill);
    }
    if (fill_minute) {
        _minutes_fill.push_back(minute);
        while (_minutes_fill.size() < 4) {
            int r = (cocos2d::random<int>(0, 59) / round_minute) * round_minute;
            bool add = true;
            for (int n : _minutes_fill) {
                if (r == n) {
                    add = false;
                    break;
                }
            }
            if (add) _minutes_fill.push_back(r);
        }
        math::func::shuffle(_minutes_fill);
    }
    
    // Gen Clock
    clock = math::AnalogClock::createClock(1);
    clock->setClockSize(clock_size);
    clock->setNumberAsText(use_text);
    clock->setNumberRoman(use_roman);
    clock->setTime(hour, minute);
    
    // Gen Slot
    for (int i = 0; i < num_slot_fill; ++i) {
        Math_Slot * slot = mpool->getSlot();
        slot->setEndble(obj_id++, slot_bg);
        slot->setSlotSize(drager_size*0.95);
        _hour_slots.push_back(slot);
        if(fill_hour && !_hours_show[i]) _slots.push_back(slot);
    }
    
    for (int i = 0; i < num_slot_fill; ++i) {
        Math_Slot * slot = mpool->getSlot();
        slot->setEndble(obj_id++, slot_bg);
        slot->setSlotSize(drager_size*0.95);
        _minute_slots.push_back(slot);
        if(fill_minute && !_minutes_show[i]) _slots.push_back(slot);
    }
    
    ///////////////
    // Level Easy
    if (level.compare("easy") == 0) {
        // Set Comparison Slots
        if (fill_hour) {
            _hour_slots[0]->setComparisionValue(hour_object->getComparisionValue());
        }
        if (fill_minute) {
            _minute_slots[0]->setComparisionValue(minute_object->getComparisionValue());
        }
        // Gen Drager
        for (int h : _hours_fill) {
            math::resource::Image * obj_img = getNumbers(h, h)[0];
            Math_Object * object = mpool->getObject();
            object->setEnable(obj_id++, math::common::move, obj_img, drag_bg);
            object->setObjectSize(drager_size);
            object->getLabelValue()->setFontSize(text_size);
            object->getLabelValue()->setString(cocos2d::StringUtils::format("%02d", h));
            object->setImageVisible(false);
            object->showValueTextWithAnimation(true);
            object->setMoveCallback(CC_CALLBACK_1(Minh_FillTimeShowAtClockQuiz::onTouchedObject, this), CC_CALLBACK_2(Minh_FillTimeShowAtClockQuiz::onMoveObjectEnded, this));
            _hour_dragers.push_back(object);
            if(fill_hour) _dragers.push_back(object);
        }
        
        for (int m : _minutes_fill) {
            math::resource::Image * obj_img = getNumbers(m, m)[0];
            Math_Object * object = mpool->getObject();
            object->setEnable(obj_id++, math::common::move, obj_img, drag_bg);
            object->setObjectSize(drager_size);
            object->getLabelValue()->setFontSize(text_size);
            object->getLabelValue()->setString(cocos2d::StringUtils::format("%02d", m));
            object->setImageVisible(false);
            object->showValueTextWithAnimation(true);
            object->setMoveCallback(CC_CALLBACK_1(Minh_FillTimeShowAtClockQuiz::onTouchedObject, this), CC_CALLBACK_2(Minh_FillTimeShowAtClockQuiz::onMoveObjectEnded, this));
            _minute_dragers.push_back(object);
            if(fill_minute) _dragers.push_back(object);
        }
        
    }
    ///////////
    // Level Hard
    else {
        
        // Set Comparision Value For Slots
        
        int pos = 0;
        _hour_objects.clear();
        for (char c : cocos2d::StringUtils::format("%02d", hour)) {
            Math_Object * object = mpool->getObject();
            object->setEnable(obj_id++, math::common::move, getNumbers(c - '0', c - '0')[0], drag_bg);
            object->setObjectSize(drager_size);
            object->getLabelValue()->setFontSize(text_size);
            object->setValueTextByKey("number");
            object->setImageVisible(false);
            object->showValueTextWithAnimation(true);
            _hour_objects.push_back(object);
            _objects.push_back(object);
            if (fill_hour) _hour_slots[pos++]->setComparisionValue(object->getComparisionValue());
        }
        
        _minute_objects.clear();
        pos = 0;
        for (char c : cocos2d::StringUtils::format("%02d", minute)) {
            Math_Object * object = mpool->getObject();
            object->setEnable(obj_id++, math::common::move, getNumbers(c - '0', c - '0')[0], drag_bg);
            object->setObjectSize(drager_size);
            object->getLabelValue()->setFontSize(text_size);
            object->setValueTextByKey("number");
            object->setImageVisible(false);
            object->showValueTextWithAnimation(true);
            _minute_objects.push_back(object);
            _objects.push_back(object);
            if (fill_minute) _minute_slots[pos++]->setComparisionValue(object->getComparisionValue());
        }
        
        // Gen dragers
        for (auto img : getNumbers(0, 9)) {
            Math_Object * object = mpool->getObject();
            object->setEnable(obj_id++, math::common::move, img, drag_bg);
            object->setObjectSize(drager_size);
            object->getLabelValue()->setFontSize(text_size);
            object->setValueTextByKey("number");
            object->setImageVisible(false);
            object->showValueTextWithAnimation(true);
            object->setMoveCallback(CC_CALLBACK_1(Minh_FillTimeShowAtClockQuiz::onTouchedObject, this), CC_CALLBACK_2(Minh_FillTimeShowAtClockQuiz::onMoveObjectEnded, this));
            _minute_dragers.push_back(object);
            _dragers.push_back(object);
        }
    }
    
    // Gen Question
    parseQuestion();
}

void Minh_FillTimeShowAtClockQuiz::onStart()
{
    float right_width = 330;
    if (level == "easy") {
        right_width = 250;
    }
    int number_slot = (fill_hour ? 1 : 0) + (fill_minute ? 1 : 0);
    int total_node = (is_clone_drag ? 10 : 4 * number_slot);
    int max_col = (is_clone_drag || number_slot == 2) ? 2 : 1;
    onShowRightBar(0, right_width / quiz_size.width, CC_CALLBACK_1(Minh_FillTimeShowAtClockQuiz::onAnswerDone, this));
    
    quizBegan(0.5, true);
    
    float total_height = clock_size + drager_size.height;
    float delta = (quiz_size.height - getQuestion()->getContentSize().height - 50 - total_height)/2;
    
    // Set Postiton Clock
    clock->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2, quiz_size.height - getQuestion()->getContentSize().height - 20 - clock_size/2 - delta));
    addChildNode(clock, 2);
    clock->show(0.25f, math::animation::SlideRight);
    
    float slot_y = clock->getPosition().y - 30 - clock_size/2 - drager_size.height/2;
    
    // Set position slot and time
    float dt = 0;
    // Show hour
    for (int i = 0; i < _hour_slots.size(); ++i) {
        _hour_slots[i]->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2 - (_hour_slots.size() - i - 0.5) *  drager_size.width - 25, slot_y));
        if (!_hours_show[i]) {
            addChildNode(_hour_slots[i], 2);
            _hour_slots[i]->onShow(dt, math::animation::SlideUp);
            dt += 0.1;
        }
    }
    for (int i = 0; i < _hour_objects.size(); ++i) {
        _hour_objects[i]->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2 - (_hour_objects.size() - i - 0.5) *  drager_size.width - 25, slot_y));
        if (_hours_show[i]) {
            addChildNode(_hour_objects[i], 2);
            _hour_objects[i]->onShow(dt, math::animation::SlideUp);
            dt += 0.1;
        }
    }
    
    // Show divison
    device_object->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2, slot_y));
    addChildNode(device_object, 2);
    device_object->onShow(dt, math::animation::SlideUp);
    dt += 0.1;
    
    // Show minute;
    for (int i = 0; i < _minute_slots.size(); ++i) {
        _minute_slots[i]->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2 + (i + 0.5) *  drager_size.width + 25, slot_y));
        if (!_minutes_show[i]) {
            addChildNode(_minute_slots[i], 2);
            _minute_slots[i]->onShow(dt, math::animation::SlideUp);
            _minute_slots[i]->setParent(_panel);
            dt += 0.1;
        }
    }
    
    for (int i = 0; i < _minute_objects.size(); ++i) {
        _minute_objects[i]->setPosition(cocos2d::Vec2((quiz_size.width - right_width) / 2 + (i + 0.5) *drager_size.width + 25, slot_y));
        if (_minutes_show[i]) {
            addChildNode(_minute_objects[i], 2);
            _minute_objects[i]->onShow(dt, math::animation::SlideUp);
            dt += 0.1;
        }
    }
    
    auto locations = math::func::getLocations(total_node, max_col, cocos2d::Rect(0.15, 0.22, 0.7, 0.7), _panel_right->getContentSize());
    int pos = 0;
    dt = 0.5f;
    for (Math_Object * object : _dragers) {
        object->setPosition(locations[pos]);
        addChildNodeToRightPanel(object, 3);
        object->onShow(dt, math::animation::SlideLeft);
        dt += 0.1;
        ++pos;
    }
}

void Minh_FillTimeShowAtClockQuiz::startQuestion()
{
    enableControlQuiz();
    for (Math_Object * object : _dragers) {
        object->enableDrag(_panel_right);
    }
}

void Minh_FillTimeShowAtClockQuiz::onExplanation(bool correct)
{
    if (!correct) {
        disableControlQuiz(0.25);
        //return wrong drager
        for (int i = 0; i < _slots.size(); ++i) {
            if (_slots[i]->isUsed() && !_slots[i]->isCorrect(key_compare)) {
                auto linker = _slots[i]->getLinker();
                _slots[i]->removeLinker();
                if (level == "hard") {
                    linker->onHide(0, math::animation::ScaleUpAndRotation);
                } else {
                    linker->backToStart();
                }
            }
        }
    } else {
        disableControlQuiz();
        this->nextQuestion(0.75);
    }
}

void Minh_FillTimeShowAtClockQuiz::onComplete()
{
    for (Math_Slot* slot :_slots) {
        slot->onHide(0, math::animation::No_Effect);
    }
    for (Math_Object* object : _hour_dragers) {
        object->onHide(0, math::animation::No_Effect);
    }
    for (Math_Object* object : _minute_dragers) {
        object->onHide(0, math::animation::No_Effect);
    }
    std::string txt = "";
    for (Math_Object* object : _hour_objects) {
        txt = cocos2d::StringUtils::format("%s%s", txt.c_str(), object->getValueText().c_str());
        object->onHide(0, math::animation::No_Effect);
    }
    txt = cocos2d::StringUtils::format("%s : ", txt.c_str());
    for (Math_Object* object : _minute_objects) {
        txt = cocos2d::StringUtils::format("%s%s", txt.c_str(), object->getValueText().c_str());
        object->onHide(0, math::animation::No_Effect);
    }
    
    device_object->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.35), cocos2d::CallFunc::create([=](){
        device_object->setValueText(txt);
    }), cocos2d::FadeIn::create(0.5), NULL));
    
    device_object->onTouchObject(true, 0.5);
    
    clock->destroy(2.0, math::animation::SlideLeft);
    
    runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(2.0), cocos2d::CallFunc::create([=](){
        device_object->onHide(0, math::animation::ScaleUp);
    })));
    quizEnded(2.0, true);
}

void Minh_FillTimeShowAtClockQuiz::onTouchedObject(cocos2d::Node * node)
{
    is_clone_drag = false;
    if (level == "hard") {
        Math_Object* object = (Math_Object*)node;
        bool clone = true;
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() && slot->getLinker()->isSame(object)) {
                clone = false;
                break;
            }
        }
        if (clone) {
            is_clone_drag = true;
            for (int i=0; i<_dragers.size(); i++) {
                if (object->isSame(_dragers[i])) {
                    _dragers.erase(_dragers.begin()+i);
                    break;
                }
            }
            
            Math_Object * clone_object = object->clone();
            clone_object->onShow(0, math::animation::No_Effect);
            _dragers.push_back(clone_object);
        }
    }
}

void Minh_FillTimeShowAtClockQuiz::onMoveObjectEnded(cocos2d::Node * node, cocos2d::Vec2 position)
{
    disableControlQuiz(0.25);
    Math_Slot * current_slot = NULL;
    Math_Slot * olded_slot = NULL;
    Math_Object* object = (Math_Object*)node;
    
    //Doi tuong keo tha nam trong 1 slot nao do.
    for (Math_Slot* slot : _slots) {
        if (slot->isUsed() && slot->getLinker()->isSame(object)) {
            olded_slot = slot;
            break;
        }
    }
    //Check slot dich.
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(_panel->convertToNodeSpace(position))) {
            current_slot = slot;
            break;
        }
    }
    
    // Neu khong nam trong slot nao
    if (current_slot == NULL) {
        // Neu truoc do nam trong 1 slot -> xoa object
        if (olded_slot != NULL) {
            olded_slot->removeLinker();
        }
        if (is_clone_drag) {
            object->onHide(0, math::animation::ScaleUpAndRotation);
        } else {
            object->backToStart();
        }
    }
    // Neu nam trong 1 slot nao do
    else {
        // Neu truoc do k nam trong slot nao
        if (olded_slot == NULL) {
            // neu slot hien tai dang duoc su dung
            if (current_slot->isUsed()) {
                if (is_clone_drag) {
                    current_slot->getLinker()->onHide(0, math::animation::ScaleUpAndRotation);
                }
                else {
                    current_slot->getLinker()->backToStart();
                }
                current_slot->removeLinker();
            }
            current_slot->setLinker(object);
            
            object->removeFromParentAndCleanup(false);
            addChildNode(object, 10);
            object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
        }
        // Neu truoc do nam o slot nao do
        // Doi cho 2 slot
        else {
            if (current_slot == olded_slot) {
                object->removeFromParentAndCleanup(false);
                addChildNode(object, 10);
                object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
            }
            else {
                Math_Object * ocs = current_slot->getLinker();
                current_slot->removeLinker();
                olded_slot->removeLinker();
                
                current_slot->setLinker(object);
                object->removeFromParentAndCleanup(false);
                addChildNode(object, 10);
                object->runAction(cocos2d::MoveTo::create(0.2, current_slot->getPosition()));
                
                if (ocs != NULL) {
                    olded_slot->setLinker(ocs);
                    ocs->runAction(cocos2d::MoveTo::create(0.2, olded_slot->getPosition()));
                }
            }
        }
    }
}

void Minh_FillTimeShowAtClockQuiz::onAnswerDone(cocos2d::Node * node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool  done = true;
    for (Math_Slot* slot : _slots) {
        if (!slot->isCorrect(key_compare)) {
            done = false;
        }
    }
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

void Minh_FillTimeShowAtClockQuiz::autoTesting()
{
}


void Minh_FillTimeShowAtClockQuiz::autoTestingRun(float dt)
{
}

void Minh_FillTimeShowAtClockQuiz::playAnimGaf(Node * object)
{
    std::string gaf = "click1";
    auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
    auto anim = gaf_asset->createObjectAndRun(false);
    anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    anim->setPosition(object->getPosition());
    object->getParent()->addChild(anim, 20);
    scheduleOnce([=](float) {
        anim->removeFromParentAndCleanup(true);
    }, 0.5, "remove anim");
}

