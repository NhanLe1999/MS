//
//  CountRowAndDragQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/19/17.
//
//

#include "CountRowAndDragQuiz.h"

CountRowAndDragQuiz* CountRowAndDragQuiz::createQuiz()
{
    CountRowAndDragQuiz* quiz=new CountRowAndDragQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CountRowAndDragQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CountRowAndDragQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    rapidjson::Value& json_game_count = json_game.FindMember("count")->value;
    counting = getJsonBool(json_game_count, "enable", false);
    count_sequence = getJsonBool(json_game_count, "sequence", true);
    number_group = getJsonInt(json_game, "group", 2);
    slot_position = getJsonString(json_game, "slot", "right");
    align_group = getJsonBool(json_game, "align", true);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    auto number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    object_size = getSizeJson(json_object, "size");
    object_value = json_object.FindMember("value")->value.FindMember("type")->value.GetBool();
    object_value_position = json_object.FindMember("value")->value.FindMember("position")->value.GetString();
    number_object = cocos2d::random(number_min, number_max);
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    object_align = getJsonString(json_group, "align", "bottom_right");
    group_arrange = getJsonBool(json_group, "arrange", true);
    group_column = getJsonInt(getJsonMember(json_group, "frame"), "column", 5);
    group_row = getJsonInt(getJsonMember(json_group, "frame"), "row", 1);
    group_type = getJsonString(json_group, "type", "frame");
    group_size = getSizeJson(json_group, "size");
    
    number_kind=number_group;
    auto n_list=randomArrayIndexes(number_group, number_max-number_min+1);
    for (int i=0; i<number_group; i++) {
        number_list.push_back(n_list[i]+number_min);
    }
    
    //TODO: DRAGER CONFIG
    rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
    number_drager = randomRangeJson(json_drager, "number");
    drager_size = getSizeJson(json_drager, "size");
    int value_min = getJsonInt(getJsonMember(json_drager, "value"), "minimum", 1);
    int value_max = getJsonInt(getJsonMember(json_drager, "value"), "maximum", 3);
    dragger_value = getJsonBool(json_drager, "display", true);
    
    for (int i=0; i<number_group; i++) {
        drag_values.push_back(number_list[i]);
    }
    while (drag_values.size()<number_drager) {
        auto rand_val=cocos2d::random(value_min, value_max);
        bool add=true;
        for (int val : drag_values) {
            if (val==rand_val) {
                add=false;
                break;
            }
        }
        if (add) {
            drag_values.push_back(rand_val);
        }
    }
    math::func::shuffle(drag_values);
    
    //TODO:
    std::string icon_filter = json_data.FindMember("icon")->value.GetString();
    auto all_imgs = getImageList("icons");
    img_list = getImageList(all_imgs, number_kind, icon_filter);
    img_list = getImageListNumber(all_imgs, img_list, number_list, icon_filter);
    
    /*---> Lay xong data cua game*/
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    img_number_list = getNumbers(value_min, value_max);
    //Gen doi tuong
    math::common::TouchPermission touch_per=math::common::deny;
    if (counting) {
        touch_per=math::common::touch;
    }
    int obj_id=0;
    
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id++, touch_per, img_list[i], NULL);
            object->setObjectSize(object_size);
            object->setValueTextPosition(object_value_position);
            object->setTouchCallback(CC_CALLBACK_1(CountRowAndDragQuiz::onTouchedObject, this));
            _objects.push_back(object);
        }
    }
    
    math::resource::Image* frame_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
    math::resource::Image* group_bg = groupBG();

    math::group::GroupType gr_type=math::group::custom;
    int group_cap = number_max;
    if (group_type.compare("queue")==0) {
        gr_type = math::group::queue;
        group_cap = group_column*group_row;
        group_size = cocos2d::Size(group_column * object_size.width * 1.2 + object_size.width * 0.6, group_row * object_size.width * 1.2 + object_size.width * 0.6);
    } else if (group_type.compare("frame")==0) {
        group_cap = group_column*group_row;
        gr_type=math::group::frame;
    }
    int index=0;
    for (int i=0; i<number_kind; i++) {
        std::vector<Math_Object*> _group_list;
        for (int j=0; j<number_list[i]; j++) {
            _group_list.push_back(_objects[index++]);
        }
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        if (gr_type==math::group::frame) {
            group->setGroupFrames(group_column, group_row, frame_bg);
        } else if (gr_type==math::group::queue) {
            group->setGroupQueue(group_size);
            group->setBackground(group_bg);
        } else {
            group->setGroupSize(group_size);
            group->setBackground(group_bg);
        }
        _groups.push_back(group);
        _index_counting.push_back(0);
    }
    
    //Gen Drager
    math::resource::Image* drag_bg = objectBG();
    for (int i=0; i<number_drager; i++) {
        math::resource::Image* drag_img = NULL;
        for (math::resource::Image* img : img_number_list) {
            if (img->getComparisionValue("number")==drag_values[i]) {
                drag_img=img;
                break;
            }
        }
        Math_Object* object=mpool->getObject();
        object->setEnable(obj_id++, math::common::deny, drag_img, drag_bg);
        object->setBackground(drag_bg, 0.4);
        object->setObjectSize(drager_size);
        
        if (!dragger_value) {
            object->setValueTextByKey("name");
        } else {
            object->setValueTextByKey("number");
            object->getLabelValue()->setFontSize(drager_size.height*0.75);
        }
        object->showValueTextWithAnimation(false);
        object->setImageVisible(false);
        object->setMoveCallback(nullptr, CC_CALLBACK_2(CountRowAndDragQuiz::onMoveObjectEnded, this));
        _dragers.push_back(object);
    }
    
    //Gen Slot
    math::resource::Image* slot_bg = slotBG();
    for (int i=0; i<number_kind; i++) {
        math::resource::Image* slot_img;
        for (math::resource::Image* img : img_number_list) {
            if (img->getComparisionValue("number")==number_list[i]) {
                slot_img=img;
                break;
            }
        }
        Math_Object* object=mpool->getObject();
        object->setEnable(obj_id++, math::common::deny, slot_img, nullptr);
        
        Math_Slot* slot=mpool->getSlot();
        slot->setEndble(i, slot_bg);
        slot->setSlotSize(drager_size);
        slot->setComparisionValue(object->getComparisionValue());
        _slots.push_back(slot);
    }
}

void CountRowAndDragQuiz::onStart() {
    math::func::shuffle(_dragers);
    group_size = _groups.front()->getContentSize();
    drager_size = _dragers.front()->getContentSize();
    
    if (slot_position == "bottom") {
        float delta_gr = (quiz_size.width - group_size.width * number_group) / (number_group+1);
        delta_gr = delta_gr > 100 ? 100 : delta_gr;
        float delta_dr = (quiz_size.width - 330 - number_drager * drager_size.width) / (number_drager + 1);
        delta_dr = delta_dr > 150 ? 150 : delta_dr;
        
        math::func::smartSetPosition(_dragers, drager_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height*0.275 - 30), delta_dr);
        if (counting) {
            quizBegan(0, false);
            
            math::func::smartSetPosition(_groups, group_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height-getQuestion()->getContentSize().height-20), delta_gr);
            for (int i=0; i<number_group; i++) {
                addChildNode(_groups[i]);
                _groups[i]->onShow(i*0.1);
            }
        } else {
            onShowBottomBar(0, 0.275, true, CC_CALLBACK_1(CountRowAndDragQuiz::onAnswerDone, this));
            quizBegan(0, false);
            
            for (int i=0; i<_dragers.size(); i++) {
                addChildNodeToBottomPanel(_dragers[i]);
                _dragers[i]->onShow(0.5 + i*0.1, math::animation::SlideUp);
            }
            
            //groupsize
            float item_h = group_size.height + drager_size.height;
            float padding = (quiz_size.height * 0.725 - getQuestion()->getContentSize().height - 20 - item_h) / 3;
            float delta = padding > 100 ? 100 : padding;
            padding = (padding * 3 - delta) / 2;
            
            math::func::smartSetPosition(_groups, group_size, cocos2d::Rect(0, padding + delta + drager_size.height, quiz_size.width, group_size.height), delta_gr);
            for (int i=0; i<number_group; i++) {
                _slots[i]->setPosition(cocos2d::Vec2(_groups[i]->getPosition().x, padding + drager_size.height/2));
                addChildNode(_slots[i]);
                _slots[i]->onShow(i*0.1);
                addChildNode(_groups[i]);
                _groups[i]->onShow(i*0.1);
            }
        }
    } else {
        float bar_w = 265.0/quiz_size.width;
        math::func::smartSetPosition(_dragers, drager_size, cocos2d::Rect(50, 150, 165, quiz_size.height - 180), _dragers.size() == 2 ? 60 : 30, false);
        if (counting) {
            quizBegan(0, true);
            
            math::func::smartSetPosition(_groups, group_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height - getQuestion()->getContentSize().height-20), number_group == 2 ? 80 : 50, false);
            for (int i=0; i<number_group; i++) {
                addChildNode(_groups[i]);
                _groups[i]->onShow(i*0.1, i%2==0? math::animation::SlideRight : math::animation::SlideLeft);
            }

        } else {
            onShowRightBar(0, bar_w, CC_CALLBACK_1(CountRowAndDragQuiz::onAnswerDone, this));
            quizBegan(0, true);
            
            for (int i=0; i<_dragers.size(); i++) {
                addChildNodeToRightPanel(_dragers[i]);
                _dragers[i]->onShow(0.5 + i*0.1, math::animation::SlideLeft);
            }
            
            //groupsize
            float item_w = group_size.width + drager_size.width;
            float padding = (quiz_size.width * (1-bar_w) - item_w) / 3;
            float delta = padding > 100 ? 100 : padding;
            padding = (padding * 3 - delta) / 2;
            
            math::func::smartSetPosition(_groups, group_size, cocos2d::Rect(padding, 0, group_size.width, quiz_size.height - getQuestion()->getContentSize().height-20), number_group == 2 ? 80 : 50, false);
            for (int i=0; i<number_group; i++) {
                _slots[i]->setPosition(cocos2d::Vec2(padding + delta + group_size.width + drager_size.width/2, _groups[i]->getPosition().y));
                addChildNode(_slots[i]);
                _slots[i]->onShow(i*0.1);
                addChildNode(_groups[i]);
                _groups[i]->onShow(i*0.1);
            }
        }
    }
}

void CountRowAndDragQuiz::startQuestion() {
     
    enableControlQuiz();
    
    if (slot_position == "bottom") {
        if (counting) {
            if (this->getQuestion()->getData().index==1) {
                onShowBottomBar(0, 0.275, true, CC_CALLBACK_1(CountRowAndDragQuiz::onAnswerDone, this));
                for (int i=0; i<_dragers.size(); i++) {
                    addChildNodeToBottomPanel(_dragers[i]);
                    _dragers[i]->onShow(0.5 + i*0.1, math::animation::SlideUp);
                }
                scheduleOnce([=](float){
                    for (Math_Object* object :  _dragers) {
                        object->enableDrag(_panel_bot);
                    }
                }, 1.5, "drag ready");

                //groupsize
                float item_h = group_size.height + drager_size.height;
                float padding = (quiz_size.height * 0.725 - getQuestion()->getContentSize().height - 20 - item_h) / 3;
                float delta = padding > 100 ? 100 : padding;
                padding = (padding * 3 - delta) / 2;
                
                float group_y = padding + delta + drager_size.height + group_size.height/2;
                for (int i=0; i<number_group; i++) {
                    _groups[i]->setPosition(cocos2d::Vec2(_groups[i]->getPosition().x, _groups[i]->getPosition().y - quiz_size.height*0.275));
                    _groups[i]->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(_groups[i]->getPosition().x, group_y)));
                }

                for (int i=0; i<number_group; i++) {
                    _slots[i]->setPosition(cocos2d::Vec2(_groups[i]->getPosition().x, padding + drager_size.height/2));
                    addChildNode(_slots[i]);
                    _slots[i]->onShow(0.5+i*0.1);
                }

            }
        } else {
            if (this->getQuestion()->getData().index==0) {
                scheduleOnce([=](float){
                    for (Math_Object* object :  _dragers) {
                        object->enableDrag(_panel_bot);
                    }
                }, 1.0, "drag ready");
            }
        }
    } else {
        if (counting) {
            if (this->getQuestion()->getData().index==1) {
                float bar_w = 265.0/quiz_size.width;
                onShowRightBar(0, bar_w, CC_CALLBACK_1(CountRowAndDragQuiz::onAnswerDone, this));
                for (int i=0; i<_dragers.size(); i++) {
                    addChildNodeToRightPanel(_dragers[i]);
                    _dragers[i]->onShow(0.5 + i*0.1, math::animation::SlideLeft);
                }
                scheduleOnce([=](float){
                    for (Math_Object* object : _dragers) {
                        object->enableDrag(_panel_right);
                    }
                }, 1.5, "drag ready");
                
                //groupsize
                float item_w = group_size.width + drager_size.width;
                float padding = (quiz_size.width * (1-bar_w) - item_w) / 3;
                float delta = padding > 100 ? 100 : padding;
                padding = (padding * 3 - delta) / 2;
                
                float group_x = padding + group_size.width/2;
                for (int i=0; i<number_group; i++) {
                    _groups[i]->runAction(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(group_x, _groups[i]->getPosition().y)));
                }
                
                for (int i=0; i<number_group; i++) {
                    _slots[i]->setPosition(cocos2d::Vec2(padding + delta + group_size.width + drager_size.height/2, _groups[i]->getPosition().y));
                    addChildNode(_slots[i]);
                    _slots[i]->onShow(0.5+i*0.1);
                }
            }
            
        } else {
            if (this->getQuestion()->getData().index==0) {
                scheduleOnce([=](float){
                    for (Math_Object* object : _dragers) {
                        object->enableDrag(_panel_right);
                    }
                }, 1.0, "drag ready");
            }
        }
    }
}

void CountRowAndDragQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
        //return wrong drager
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() && !slot->isCorrect(key_compare)) {
                slot->getLinker()->backToStart();
                slot->removeLinker();
            }
        }
    } else {
        disableControlQuiz();
        nextQuestion(0.75);
    }
}

void CountRowAndDragQuiz::onComplete() {
    auto ans_slide = math::animation::SlideUp;
    if (slot_position == "right") {
        ans_slide = math::animation::SlideLeft;
    }
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, ans_slide);
    }
    for (Math_Group* group : _groups) {
        group->setDisable(0.25);
    }
    
    for (Math_Slot* slot : _slots) {
        slot->onHide(0.5);
    }
    for (Math_Object* object : _dragers) {
        object->onHide(0.5);
    }
    
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
void CountRowAndDragQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=(Math_Object*) node;
    Math_Object* object_focus=NULL;
    bool count=true;
    
    if (count_sequence) {
        Math_Group* group;
        for (Math_Group* _group : _groups) {
            if (_group->hasObject(object)) {
                group=_group;
                break;
            }
        }

        if (_group_counting==NULL) {
            if (group->getIndexOfObject(object)==0) {
                _group_counting=group;
            } else {
                object_focus=group->getObjectAtIndex(0);
                count=false;
            }
        } else {
            int index_group=_group_counting->getId();
            int number_count=_index_counting[index_group];
            object_focus=_group_counting->getObjectAtIndex(number_count);
            if (_group_counting==group) {
                if (!object->isSame(object_focus)) {
                    count=false;
                }
            } else {
                count=false;
            }
        }
    } else {
        for (Math_Group* group : _groups) {
            if (group->hasObject(object)) {
                _group_counting=group;
                break;
            }
        }
    }
    
    if (count) {
        //Show count
        int index_group=_group_counting->getId();
        int number_count=_index_counting[index_group]+1;
        _index_counting.erase(_index_counting.begin()+index_group);
        _index_counting.insert(_index_counting.begin()+index_group, number_count);
        
        if (number_count==_group_counting->getNumberObjects()) {
            _group_counting=NULL;
        }
        
        if (object_value) {
            object->setValueText(cocos2d::StringUtils::format("%d", number_count));
        } else {
            std::string name_count="";
            for (math::resource::Image* img : img_number_list) {
                if (img->getComparisionValue("number") == number_count) {
                    name_count=img->getText("name_1");
                }
            }
            object->setValueText(name_count);
        }

        object->showValueTextWithAnimation(true);
        object->setLocked(true);
        
        number_counting++;
        if (number_counting==_objects.size()) {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0), cocos2d::CallFunc::create([=](){
                this->onExplanation(true);
            }), NULL));
        }
    } else {
        //forcus count
        if (object_focus!=NULL) {
            object_focus->runAction(cocos2d::Repeat::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.1, 1.1), cocos2d::ScaleTo::create(0.1, 1)), 2));
        }
    }
}

void CountRowAndDragQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
    
    Math_Object* object=(Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    
    //Doi tuong keo tha nam trong 1 slot nao do.
    Math_Slot* m_slot = NULL;
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            m_slot = slot;
            break;
        }
    }
    
    Math_Slot* t_slot = NULL;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    
    if (m_slot!=NULL) {
        if (t_slot != NULL) {
            //swap
            if (m_slot == t_slot) {
                object->backToLast(2);
            } else {
                if (t_slot->isUsed()) {
                    auto linker = t_slot->getLinker();
                    m_slot->removeLinker();
                    t_slot->removeLinker();
                    
                    t_slot->setLinker(object);
                    object->removeFromParentAndCleanup(false);
                    object->setPosition(position);
                    addChildNode(object, 100);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    
                    m_slot->setLinker(linker);
                    linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);
                    object->removeFromParentAndCleanup(false);
                    object->setPosition(position);
                    addChildNode(object, 100);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                }
            }
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->backToStart();
            }
            t_slot->setLinker(object);
            object->removeFromParentAndCleanup(false);
            object->setPosition(position);
            addChildNode(object, 100);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            //move start
            object->backToStart();
        }
    }
}

void CountRowAndDragQuiz::onAnswerDone(Node* node)
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
void CountRowAndDragQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CountRowAndDragQuiz::autoTestingRun(float delaytime)
{
    int index_ques=this->getQuestion()->getData().index;
    
    if (counting && index_ques==0) {
        //counting
        if (number_counting < _objects.size()) {
            Math_Object* object=math::func::getRandomNode(_objects);
            while (object->isLocked() || object->isSame(last_object_touch)) {
                object=math::func::getRandomNode(_objects);
            }
            last_object_touch=object;
            object->onTouched();
        }
        this->scheduleOnce([=](float dt) {
            this->autoTestingRun(delaytime);
        }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
        
    } else {
        //Choose answer
        bool done=true;
        for (Math_Slot* slot : _slots) {
            if (!slot->isUsed()) {
                done=false;
            }
        }
        
        if (done) {
            //Tat ca slot duoc keo object
            bool is_correct=true;
            for (Math_Slot* slot : _slots) {
                if (!slot->isCorrect(key_compare)) {
                    is_correct=false;
                }
            }
            
            if (!is_correct) {
                //keo tha lai
                this->scheduleOnce([=](float dt) {
                    this->autoTestingRun(delaytime);
                }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
                
            } else {
                //Ket thuc quiz
            }
            
            Math_Answer* answer=math::func::getRandomNode(_answers);
            answer->onAutoClick();
        } else {
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
    }
}

