//
//  CountAndDragQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#include "CountAndDragQuiz.h"

CountAndDragQuiz* CountAndDragQuiz::createQuiz()
{
    CountAndDragQuiz* quiz=new CountAndDragQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CountAndDragQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CountAndDragQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    rapidjson::Value& json_game_count = json_game.FindMember("count")->value;
    counting = getJsonBool(json_game_count, "enable", true);
    count_sequence = getJsonBool(json_game_count, "sequence", false);

    display_number = getJsonBool(json_game, "display", true);
    number_slot = getJsonInt(json_game, "slot", 3);
    skip_count = randomRangeJson(json_game, "skip_count");
    ordinal = getJsonBool(json_game, "display_ordinal", false);
    if (ordinal) {
        counting = false;
        count_sequence = true;
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 3);
    auto number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 1);
    object_size = getSizeJson(json_object, "size");
    object_bg = getJsonString(json_object, "background", "none");
    object_value = getJsonBool(getJsonMember(json_object, "value"), "type", true);
    object_value_position = getJsonString(getJsonMember(json_object, "value"), "position", "top");
    number_object = cocos2d::random(number_min, number_max);
    if (object_bg == "none") {
        img_bg = nullptr;
    } else {
        img_bg = objectBG();
    }

    //TODO: DRAGER CONFIG
    rapidjson::Value& json_drager = json_data.FindMember("drager")->value;
    number_drager = randomRangeJson(json_drager, "number");
    drager_size = getSizeJson(json_drager, "size");
    int value_min=json_drager.FindMember("value")->value.FindMember("minimum")->value.GetInt();
    int value_max=json_drager.FindMember("value")->value.FindMember("maximum")->value.GetInt();
    
    slot_index=randomArrayIndexes(number_slot, number_object);
    for (int i=0; i<slot_index.size(); i++) {
        drag_values.push_back(slot_index[i]+ skip_count);
    }
    while (drag_values.size()<number_drager) {
        auto rand_val=cocos2d::random(value_min, (skip_count+number_object+1) > value_max? value_max : skip_count+number_object+1);
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
    //Con resource. game chi co 1 loai obj
    number_kind=1;
    number_list.push_back(number_object);
    
    std::string icon_filter = json_data.FindMember("icon")->value.GetString();
    img_list = getImageList("icons", number_kind, icon_filter);
    
    auto number_imgs = getNumbers(value_min, (skip_count+number_object+1) > value_max? value_max : skip_count+number_object+1);
    if (ordinal) {
        number_imgs = getOrdinals(value_min, (skip_count+number_object+1) > value_max? value_max : skip_count+number_object+1);
    }
    for (math::resource::Image* img : number_imgs) {
        int _number = img->getComparisionValue("number");
        if (_number == skip_count) {
            img_number_list.insert(img_number_list.begin(), img);
        } else if (_number > skip_count) {
            img_number_list.push_back(img);
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
        }  else if (key == "name_n") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<img_number_list.size(); i++) {
                if (img_number_list[i]->getComparisionValue("number") < 2) {
                    auto img_txt = img_list[i]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_list[i]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen doi tuong
    math::common::TouchPermission touch_per=math::common::deny;
    if (counting) {
        touch_per=math::common::touch;
    }
    int obj_id=0;
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id++, touch_per, img_list[i], img_bg);
            object->setObjectSize(object_size);
            object->setValueTextPosition(object_value_position);
            object->setTouchCallback(CC_CALLBACK_1(CountAndDragQuiz::onTouchedObject, this));
            _objects.push_back(object);
        }
    }
    
    //Gen Drager
    math::resource::Image* drag_bg = objectBG();
    math::resource::Image* slot_bg = slotBG();

    for (int i=0; i<number_drager; i++) {
        math::resource::Image* drag_img;
        for (math::resource::Image* img : number_imgs) {
            if (img->getComparisionValue("number")==drag_values[i]) {
                drag_img=img;
                break;
            }
        }
        Math_Object* object=mpool->getObject();
        object->setEnable(obj_id++, math::common::deny, drag_img, drag_bg);
        object->setObjectSize(drager_size);
        object->setBackground(drag_bg, 0.3);
        object->setValueTextPosition("middle");
        if (object_value) {
            object->setValueTextByKey("name_1");
            object->getLabelValue()->setFontSize(drager_size.height*0.7);
        } else {
            object->setValueTextByKey("name_n");
        }
        object->showValueTextWithAnimation(false);
        object->setImageVisible(false);
        object->fitValueTextSize();
        
        object->setMoveCallback(nullptr, CC_CALLBACK_2(CountAndDragQuiz::onMoveObjectEnded, this));
        _dragers.push_back(object);
    }
    
    //Gen Slot
    for (int i=0; i<_objects.size(); i++) {
        bool is_slot=false;
        for (int index : slot_index) {
            if (i==index) {
                is_slot=true;
                break;
            }
        }
        math::resource::Image* slot_img;
        for (math::resource::Image* img : number_imgs) {
            if (img->getComparisionValue("number")==i+skip_count) {
                slot_img=img;
                break;
            }
        }
        Math_Object* object=mpool->getObject();
        object->setEnable(obj_id++, math::common::deny, slot_img, drag_bg);
        if (is_slot) {
            Math_Slot* slot=mpool->getSlot();
            slot->setEndble(obj_id++, slot_bg);
            slot->setSlotSize(drager_size*0.95);
            slot->setBackground(slot_bg, 0.3);
            slot->setComparisionValue(object->getComparisionValue());
            _slots.push_back(slot);
            _slot_object.push_back(slot);
        } else {
            object->setObjectSize(drager_size);
            object->setBackground(drag_bg, 0.3);
            object->setTag(100);
            if (object_value) {
                object->getLabelValue()->setFontSize(drager_size.height*0.7);
                object->setValueTextByKey("name_1");
            } else {
                object->setValueTextByKey("name_n");
            }
            object->showValueTextWithAnimation(false);
            object->setImageVisible(false);
            object->fitValueTextSize();
            
            _slot_object.push_back(object);
            _added_objects.push_back(object);
        }
    }
}

void CountAndDragQuiz::onStart() {
    //Set position and setup qui
    float delta_dr = (quiz_size.width - 330 - drager_size.width * number_drager) / (number_drager+1);
    delta_dr = delta_dr > 100 ? 100 : delta_dr;
    math::func::smartSetPosition(_dragers, drager_size, cocos2d::Rect(40, 0, quiz_size.width - 330, quiz_size.height * 0.3 - 30), delta_dr);
    
    float delta_obj = (quiz_size.width - object_size.width * number_object) / (number_object+1);
    while (delta_obj < 20) {
        delta_obj = (quiz_size.width - drager_size.width * number_object) / (number_object+1);
    }
    delta_obj = delta_obj > 100 ? 100 : delta_obj;
    
    math::func::smartSetPosition(_slot_object, drager_size, cocos2d::Rect(0, 0, quiz_size.width, (quiz_size.height * 0.7 - getQuestion()->getContentSize().height-20)/2), delta_obj);

    if (counting) {

        quizBegan(0.5, true);
        
        math::func::smartSetPosition(_objects, drager_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height - getQuestion()->getContentSize().height-20), delta_obj);

        for (int i=0; i<_objects.size(); i++) {
            _objects[i]->setObjectSize(object_size);
            addChildNode(_objects[i], 2);
            _objects[i]->onShow(i*0.1);
        }
    } else {
        onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(CountAndDragQuiz::onAnswerDone, this));
        quizBegan(0.0, true);

        math::func::smartSetPosition(_objects, drager_size, cocos2d::Rect(0, (quiz_size.height * 0.7 - getQuestion()->getContentSize().height-20)/2, quiz_size.width, (quiz_size.height * 0.7 - getQuestion()->getContentSize().height-20)/2), delta_obj);
        for (int i=0; i<_objects.size(); i++) {
            _objects[i]->setObjectSize(object_size);
            addChildNode(_objects[i], 2);
            _objects[i]->onShow(i*0.1);
        }
        
        //#2. show slot
        for (Math_Slot* slot : _slots) {
            addChildNode(slot, 2);
            slot->onShow(0.5);
        }
        for (Math_Object* object : _added_objects) {
            addChildNode(object, 2);
            object->onShow(0.5);
        }
        
        //#3. show drager
        for (int i=0; i<_dragers.size(); i++) {
            addChildNodeToBottomPanel(_dragers[i], 2);
            _dragers[i]->onShow(0.5 + i*0.1, math::animation::SlideUp);
        }
        scheduleOnce([=](float){
            for (int i=0; i<_dragers.size(); i++) {
                _dragers[i]->enableDrag(_panel_bot);
            }
        }, 1.5, "enablde drag");
    }
}

void CountAndDragQuiz::startQuestion() {
    enableControlQuiz();
    //Bat dau cau hoi
    if (counting) {
        if (this->getQuestion()->getData().index==0) {
        } else {
            //#1. move object
            for (Math_Object* object : _objects) {
                object->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
                    object->hideValueTextWithAnimation(false);
                }), cocos2d::DelayTime::create(0.1), cocos2d::FadeIn::create(0.5), NULL));
            }
            scheduleOnce([=](float){
                math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, (quiz_size.height * 0.7 - getQuestion()->getContentSize().height-20)/2, quiz_size.width, (quiz_size.height * 0.7 - getQuestion()->getContentSize().height-20)/2), 100);
                onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(CountAndDragQuiz::onAnswerDone, this));
                //#2. show slot
                for (Math_Slot* slot : _slots) {
                    addChildNode(slot, 2);
                    slot->onShow(0.5);
                }
                for (Math_Object* object : _added_objects) {
                    addChildNode(object, 2);
                    object->onShow(0.5);
                }
                
                //#3. show drager
                for (int i=0; i<_dragers.size(); i++) {
                    addChildNodeToBottomPanel(_dragers[i], 2);
                    _dragers[i]->onShow(i*0.1, math::animation::SlideUp);
                }
            }, 0.5, "move up objects");
            scheduleOnce([=](float){
                for (int i=0; i<_dragers.size(); i++) {
                    _dragers[i]->enableDrag(_panel_bot);
                }
            }, 1.5, "enablde drag");
        }
    }
}

void CountAndDragQuiz::onExplanation(bool correct) {
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
        this->nextQuestion(0.75);
    }
}

void CountAndDragQuiz::onComplete() {
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }
    for (int i = 0; i< _objects.size(); i++) {
        _objects[i]->onHide(i*0.1);
    }
    for (Math_Object* object : _dragers) {
        object->onHide(0.5);
    }
    for (Math_Slot* slot :  _slots) {
        slot->onHide(0.5);
    }
    for (Math_Object* object :  _added_objects) {
        object->onHide(0.5);
    }
    
    quizEnded(0.5, true);
}

#pragma mark - object touch events callback
void CountAndDragQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=(Math_Object*) node;
    bool count=true;
    if (count_sequence) {
        int index=0;
        for (; index<number_object; index++) {
            if (_objects[index]->isSame(object)) {
                break;
            }
        }
        if (index==count_index) {
            count_index++;
        } else {
            count=false;
        }
    }
    if (count) {
        //Show count
        number_counting++;
        
        math::resource::Image* img_number = getNumbers(number_counting+skip_count-1, number_counting+skip_count-1).front();
        if (img_number != nullptr) {
            if (object_value) {
                object->setValueText(img_number->getText("name_1"));
                audio_helper->play_sequence(img_number->getAudios(), nullptr, "");
            } else {
                object->setValueText(img_number->getText("name_n"));
                audio_helper->play_sequence(img_number->getAudios(), nullptr, "");
            }
        }
        object->showValueTextWithAnimation(true);
        object->setLocked(true);
        
        if (number_counting==_objects.size()) {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0), cocos2d::CallFunc::create([=](){
                this->onExplanation(true);
            }), NULL));
        }
    } else {
        //forcus count
        _objects[count_index]->runAction(cocos2d::Repeat::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.1, 1.1), cocos2d::ScaleTo::create(0.1, 1)), 2));
    }
}

void CountAndDragQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
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
                    addChildNode(object, 10);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    
                    m_slot->setLinker(linker);
                    linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);
                    object->removeFromParentAndCleanup(false);
                    object->setPosition(position);
                    addChildNode(object, 10);
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
            object->removeFromParentAndCleanup(false);
            object->setPosition(position);
            addChildNode(object, 10);
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            //move start
            object->backToStart();
        }
    }
}

void CountAndDragQuiz::onAnswerDone(Node* node)
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
void CountAndDragQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CountAndDragQuiz::autoTestingRun(float delaytime)
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

