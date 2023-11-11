//
//  CompleteSymbolsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "CompleteSymbolsQuiz.h"

CompleteSymbolsQuiz* CompleteSymbolsQuiz::createQuiz()
{
    CompleteSymbolsQuiz* quiz=new CompleteSymbolsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CompleteSymbolsQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void CompleteSymbolsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");

    is_addition = getJsonBool(json_game, "operator", true);
    
    number_total = randomRangeJson(json_game, "total");
    number_1 = cocos2d::random(1, number_total-1);
    number_2 = number_total-number_1;
    std::vector<int> numbers;
    
    if (is_addition) {
        img_number_list.push_back(math::number::getNumber(number_1));
        img_number_list.push_back(math::number::getNumber(number_2));
        img_number_list.push_back(math::number::getNumber(number_total));
        numbers.push_back(number_1);
        numbers.push_back(0);
        img_operator_list.push_back(getOperator("+"));
        img_operator_list.push_back(getOperator("="));
        img_operator_list.push_back(getOperator("-"));
    } else {
        img_number_list.push_back(math::number::getNumber(number_total));
        img_number_list.push_back(math::number::getNumber(number_1));
        img_number_list.push_back(math::number::getNumber(number_2));
        numbers.push_back(number_total);
        numbers.push_back(0);
        img_operator_list.push_back(getOperator("-"));
        img_operator_list.push_back(getOperator("="));
        img_operator_list.push_back(getOperator("+"));
    }

    bool same_icon = getJsonBool(json_game, "same_icon", true);
    auto all_imgs = getImageList("icons");
    if (same_icon) {
        img_list.push_back(math::func::getRandomNode(all_imgs));
        img_list.push_back(img_list.front());
    } else {
        std::string icon_same = getJsonString(json_icon, "same", "none");
        std::string icon_diff = getJsonString(json_icon, "different", "none");
        img_list = getImageList(all_imgs, 2, icon_diff, icon_same);
    }

    auto item_size = cocos2d::Size(75, 75);
    object_size = cocos2d::Size(130, 130);
    auto gr_img = groupBG();
    group_size = cocos2d::Size(6*item_size.width + 50, 100);
    
    for (int i = 0; i<2; i++) {
        int number = numbers[i];
        
        auto img = img_list[i];
        std::vector<Math_Object*> objs_gr;
        for (int i2=0; i2<number; i2++) {
            auto object = mpool->getObject();
            if (is_addition) {
                object->setEnable(i2, math::common::deny, img, nullptr);
            } else {
                object->setEnable(i2, math::common::move, img, nullptr);
                object->setMoveCallback(CC_CALLBACK_1(CompleteSymbolsQuiz::onTouchIcon, this), CC_CALLBACK_2(CompleteSymbolsQuiz::onMovedIcon, this));
            }
            object->setObjectSize(item_size);
            objs_gr.push_back(object);
        }
        
        if (i==1) {
            number=number_total;
        }
        auto group = mpool->getGroup();
        group->setEnable(i, objs_gr, math::group::custom, 10);
        group->setObjectSize(item_size);
        group->setGroupQueue(group_size);
        if (i==0) {
            group->setAlignObjects(math::group::right);
        }
        group->setBackground(gr_img, 0.3);
        _groups.push_back(group);
        
        auto object = mpool->getObject();
        object->setEnable(i, math::common::deny, img_number_list[i], nullptr);
        object->setObjectSize(object_size);
        object->setImageVisible(false);
        object->setValueTextByKey("name_1");
        object->showValueTextWithAnimation(false);
        object->getLabelValue()->setFontSize(object_size.height*0.8);
        _objects.push_back(object);

        auto op_object = mpool->getObject();
        op_object->setEnable(i, math::common::deny, img_operator_list[i], nullptr);
        op_object->setObjectSize(object_size);
        op_object->setImageVisible(false);
        op_object->setValueTextByKey("name_1");
        op_object->showValueTextWithAnimation(false);
        _objects.push_back(op_object);
    }
    
    auto object = mpool->getObject();
    object->setEnable(4, math::common::deny, img_number_list[2], nullptr);
    object->setObjectSize(object_size);
    object->setImageVisible(false);
    object->setValueTextByKey("name_1");
    object->showValueTextWithAnimation(false);
    object->getLabelValue()->setFontSize(object_size.height*0.8);
    _objects.push_back(object);
    
    if (is_addition) {
        item_object = mpool->getObject();
        item_object->setEnable(0, math::common::touch, img_list[1], nullptr);
        item_object->setMoveCallback(CC_CALLBACK_1(CompleteSymbolsQuiz::onTouchObject, this), CC_CALLBACK_2(CompleteSymbolsQuiz::onMovedObject, this));
        item_object->setObjectSize(item_size);
        _objects.push_back(item_object);
    }
    
    auto img_drag = objectBG();
    auto img_drop = slotBG();
    for (int i=0; i<3; i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::touch, img_operator_list[i], img_drag);
        object->setMoveCallback(CC_CALLBACK_1(CompleteSymbolsQuiz::onTouchObject, this), CC_CALLBACK_2(CompleteSymbolsQuiz::onMovedObjectToSlot, this));
        object->setObjectSize(object_size);
        object->setImageVisible(false);
        object->setValueTextByKey("name_1");
        object->showValueTextWithAnimation(false);
        object->getLabelValue()->setFontSize(object_size.height*0.8);
        _dragers.push_back(object);
        if (i<2) {
            auto slot = mpool->getSlot();
            slot->setEndble(i, img_drop);
            slot->setSlotSize(object_size);
            slot->setComparisionValue(object->getComparisionValue());
            _slots.push_back(slot);
        }
    }
    
    //Question
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
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
        } else if (key == "name_n") {
            if (img_number_list.front()->getComparisionValue("number")<2) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : img_list) {
                    auto img_txt = img->getTextValue("name_1");
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_list);
            }
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
}

void CompleteSymbolsQuiz::onStart() {
    if (is_addition) {
        onShowBottomBar(0, 0.25, true, CC_CALLBACK_1(CompleteSymbolsQuiz::onAnswerDone, this));
    } else {
        onShowBottomBar(0, 0.25, false, CC_CALLBACK_1(CompleteSymbolsQuiz::onAnswerDone, this));
    }
    Math_Quiz::onStart();
    
    for (int i=0; i<2; i++) {
        auto group = _groups[i];
        group->setPosition(cocos2d::Vec2((quiz_size.width - 50 - group_size.width) * 0.5 + (50 + group_size.width)*i, quiz_size.height * 0.42));
        addChildNode(group);
        group->onShow(0, math::animation::SlideUp);
    }
    
    _objects[0]->setPosition(cocos2d::Vec2((quiz_size.width - 50 - group_size.width) * 0.5, quiz_size.height * 0.12));
    addChildNode(_objects[0]);
    _objects[0]->onShow(0);
    
    _objects[2]->setPosition(cocos2d::Vec2((quiz_size.width + 50 + group_size.width) * 0.5, quiz_size.height * 0.12));
    addChildNode(_objects[2]);
    _objects[2]->onShow(0);
    
    if (is_addition) {
        item_object->setPosition(cocos2d::Vec2((quiz_size.width - 270)/2, (quiz_size.height*0.25-30)/2));
        addChildNodeToBottomPanel(item_object);
        item_object->onShow(0, math::animation::SlideUp);
    }
    
}

void CompleteSymbolsQuiz::startQuestion() {
    enableControlQuiz();
    
    int question = getQuestion()->getData().index;
    
    if (question == 0) {
        //move number object
        if (is_addition) {
            item_object->enableDrag(_panel_bot);
        } else {
            //_groups.front()->setSuperNode(_groups.front()->super_node);
            for (int i=0; i<_groups.front()->getNumberObjects(); i++) {
                auto object = _groups.front()->getObjectAtIndex(i);
                object->enableDrag(object->getParent());
            }
        }
    } else if (question == 1) {
        //drag plus: show drager and slot for plus
        for (Math_Object* object : _objects) {
            object->setLocked(true);
        }
        float delay = 0;
        if (is_addition) {
            item_object->onHide(0, math::animation::ScaleUpAndRotation);
        } else {
            delay = onHideBottomBar()+0.5;
            scheduleOnce([=](float){
                onShowBottomBar(0, 0.25, true, CC_CALLBACK_1(CompleteSymbolsQuiz::onAnswerDone, this));
            }, delay, "re show bottom bar");
        }
        disableControlQuiz(delay+0.5);
        scheduleOnce([=](float){
            float drag_x = 40;
            math::func::shuffle(_dragers);
            math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(drag_x, 0, quiz_size.width-300-drag_x, quiz_size.height*0.25-35));
            int i=0;
            for (Math_Object* object : _dragers) {
                addChildNodeToBottomPanel(object);
                object->onShow(i * 0.1 + 0.5, math::animation::SlideUp);
                i++;
            }
            scheduleOnce([=](float){
                for (Math_Object* object : _dragers) {
                    object->enableDrag(_panel_bot);
                }
            }, 0.5, "enable_drag");
            
            auto slot = _slots.front();
            slot->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height*0.12));
            addChildNode(slot);
            slot->onShow(0.25);
        }, delay, "add_dragger");        
    } else {
        //drag equal: merger sentence and group, show slot for equal
        _slots.front()->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            _slots[0]->setPosition(cocos2d::Vec2(-500, 0));
        })));
        _slots.front()->getLinker()->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            _slots[0]->getLinker()->setPosition(cocos2d::Vec2(-500, 0));
        })));
        _objects[2]->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            _objects[2]->setPosition(cocos2d::Vec2(-500, 0));
        })));
        
        _objects.front()->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            if (is_addition) {
                _objects.front()->setValueText(cocos2d::StringUtils::format("%d + %d", number_1, number_2));
            } else {
                _objects.front()->setValueText(cocos2d::StringUtils::format("%d - %d", number_total, number_1));
            }
            
            auto slot = _slots.back();
            this->addChildNode(slot);
            this->addChildNode(_objects[4]);

            float delta = 30;
            float width_1 = _objects.front()->getLabelValue()->getContentSize().width + delta;
            float width_2 = _objects[4]->getLabelValue()->getContentSize().width + delta;
            float tota_width = width_1+width_2+_slots.back()->getContentSize().width;

            float start_pos = (quiz_size.width-tota_width) / 2 + width_1/2;
            _objects.front()->setPosition(cocos2d::Vec2(start_pos, quiz_size.height*0.12));
            start_pos = start_pos + width_1/2+delta+object_size.width/2;
            _slots.back()->setPosition(cocos2d::Vec2(start_pos, quiz_size.height*0.12));
            start_pos = start_pos + width_2/2+delta+object_size.width/2;
            _objects[4]->setPosition(cocos2d::Vec2(start_pos, quiz_size.height*0.12));
            
            slot->onShow(0);
            _objects[4]->onShow(0, math::animation::No_Effect);
        }), cocos2d::FadeIn::create(0.25), NULL));
       
        if (is_addition) {
            disableControlQuiz(0.5);
            _groups.front()->runAction(cocos2d::MoveBy::create(0.25, cocos2d::Vec2(40, 0)));
            _groups.back()->runAction(cocos2d::MoveBy::create(0.25, cocos2d::Vec2(-40, 0)));
            _groups.front()->clippedBackgroundEdge(0);
            _groups.back()->clippedBackgroundEdge(1);
        } else {
            disableControlQuiz(1);
            _groups.front()->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(1), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(quiz_size.width*0.5, _groups.front()->getPosition().y)))));
            _groups.back()->onHide(0, math::animation::SlideLeft);
            _groups.pop_back();
        }
    }
}

void CompleteSymbolsQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1, "complete");
    }
}

void CompleteSymbolsQuiz::onComplete() {
    for (Math_Group* group : _groups) {
        group->onHide(0, math::animation::SlideUp);
    }
    
    for (Math_Slot* slot : _slots) {
        slot->onHide(0);
    }
    
    _objects.front()->onHide(0);
    _objects[4]->onHide(0);
    
    int i=0;
    for (Math_Object* object : _dragers) {
        object->onHide(i*0.1, math::animation::SlideUp);
        i++;
    }
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void CompleteSymbolsQuiz::onAnswerDone(Node* node) {
    int question = getQuestion()->getData().index;
    Math_Answer* answer = static_cast<Math_Answer*>(node);
    
    bool correct = false;
    if (question == 0) {
        if (is_addition) {
            //move number object
            correct = (_groups.back()->getNumberObjects() == number_2);
            if (!correct) {
                //remove all
                audio_helper->play("movenumber/wrong.mp3");
                stopSpeakQuestion();
                speakQuestion();
                for (int i=_groups.back()->getNumberObjects()-1; i>=0; i--) {
                    auto object = _groups.back()->getObjectAtIndex(i);
                    auto wposition = object->getWorldPosition();
                    _groups.back()->removeObject(object);
                    object->setPosition(_panel->convertToNodeSpace(wposition));
                    addChildNode(object, 10);
                    
                    for (int i=0; i<_objects.size(); i++) {
                        if (_objects[i]->isSame(object)) {
                            _objects.erase(_objects.begin()+i);
                            break;
                        }
                    }
                    object->onHide(0, math::animation::ScaleUpAndRotation);
                }
            }
        } else {
            correct = (_groups.back()->getNumberObjects() == number_1);
            if (!correct) {
                //remove all
                audio_helper->play("movenumber/wrong.mp3");
                stopSpeakQuestion();
                speakQuestion();
                Math_Group* group = _groups.back();
                std::vector<Math_Object*> list_back;
                int index=0;
                while (group->getNumberObjects() > 0) {
                    auto wposition = group->getObjectAtIndex(group->getNumberObjects()-1)->getWorldPosition();
                    auto object = group->popBackObject();
                    list_back.push_back(object);
                    object->setPosition(_panel->convertToNodeSpace(wposition));
                    addChildNode(object, 10);
                    
                    //Last location
                    auto position = _groups.front()->getPositionAtIndex(_groups.front()->getNumberObjects()+index);
                    index++;
                    position = _groups.front()->convertToWorldSpace(position);
                    position = _panel->convertToNodeSpace(position);
                    object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.25, position)), cocos2d::CallFunc::create([=](){
//                        _groups.front()->addObject(object, object->getWorldPosition());
                        _groups.front()->addBackObject(object);
                    })));
                }
            }
        }
    } else if (question == 1) {
        //drag plus
        correct = _slots.front()->isCorrect(math::comparision::name);
        if (!correct) {
            //remove from slot
            if (_slots.front()->isUsed()) {
                auto linker = _slots.front()->getLinker();
                _slots.front()->removeLinker();
                linker->onHide(0, math::animation::ScaleUpAndRotation);
            }
        }
    } else {
        //drag equal
        correct = _slots.back()->isCorrect(math::comparision::name);
        if (!correct) {
            //remove from slot
            if (_slots.back()->isUsed()) {
                auto linker = _slots.back()->getLinker();
                _slots.back()->removeLinker();
                linker->onHide(0, math::animation::ScaleUpAndRotation);
            }
        }
    }
    
    answer->onAnimationClick(correct);
    if (correct) {
        this->onExplanation(true);
    }
}

void CompleteSymbolsQuiz::onTouchObject(Node* node) {
    Math_Object* object=(Math_Object*)node;
    //Chon object dau tien cham phai
    if (selected_object==nullptr) {
        selected_object=object;
        audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drag.mp3");

        copying = true;
        if (getQuestion()->getData().index==0) {
            Math_Group* gr_source = nullptr;
            for (Math_Group* _group : _groups) {
                if (_group->hasObject(object)) {
                    gr_source = _group;
                    break;
                }
            }
            if (gr_source!=nullptr) {
                gr_source->removeObject(object, true);
            }
            
            //Kiem tra object o dock hay o group
            if (gr_source!=nullptr) {
                copying = false;
            }
        } else if (getQuestion()->getData().index==1) {
            if (object->isSame(_slots.front()->getLinker())) {
                copying = false;
            }
        } else {
            if (object->isSame(_slots.back()->getLinker())) {
                copying = false;
            }
        }
        
        if (copying) {
            Math_Object* object_copy = object->clone();
            object_copy->onShow(0, math::animation::No_Effect);
            if (getQuestion()->getData().index==0) {
                item_object = object_copy;
                _objects.push_back(object_copy);
            } else {
                //TODO: FIX HERE BECAUSE RESHUFFLE
                int index = 0;
                for (Math_Object* obj : _dragers) {
                    if (obj->isSame(object)) {
                        _dragers.erase(_dragers.begin()+index);
                        break;
                    }
                    index++;
                }
                _dragers.insert(_dragers.begin()+index, object_copy);
            }
        }
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
    
}

void CompleteSymbolsQuiz::onMovedObject(Node* node, cocos2d::Vec2 position) {
    Math_Object* object = (Math_Object*)node;
    auto point = _panel->convertToNodeSpace(position);
    
    Math_Group* group=nullptr;
    if (_groups.back()->isTouchOnGroup(point)) {
        group=_groups.back();
    }
    
    if (group!=nullptr) {
        if (group->addObject(object, position)) {
            object->onDropObject(0.1);
        } else {
            audio_helper->play("movenumber/wrong.mp3");
            object->onHide(0, math::animation::ScaleUpAndRotation);
        }
    } else {
        audio_helper->play("movenumber/wrong.mp3");
        object->onHide(0, math::animation::ScaleUpAndRotation);
    }
    selected_object=nullptr;
}

void CompleteSymbolsQuiz::onMovedObjectToSlot(Node* node, cocos2d::Vec2 position) {
    Math_Object* object = (Math_Object*)node;
    auto point = _panel->convertToNodeSpace(position);

    Math_Slot* t_slot = nullptr;
    Math_Slot* m_slot = nullptr;
    if (getQuestion()->getData().index==1) {
        if (_slots.front()->isAvailable(point)) {
            t_slot = _slots.front();
        }
        if (object->isSame(_slots.front()->getLinker())) {
            m_slot = _slots.front();
        }
    } else {
        if (_slots.back()->isAvailable(point)) {
            t_slot = _slots.back();
        }
        if (object->isSame(_slots.back()->getLinker())) {
            m_slot = _slots.back();
        }
    }
    
    if (m_slot != nullptr) {
        if (t_slot != nullptr) {
            object->removeFromParentAndCleanup(false);
            object->setPosition(point);
            addChildNode(object, 2);

            object->backToLast(2);
        } else {
            m_slot->removeLinker();
            object->onHide(0, math::animation::ScaleUpAndRotation);
        }
    } else {
        if (t_slot != nullptr) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->onHide(0, math::animation::No_Effect);
            }
            object->removeFromParentAndCleanup(false);
            object->setPosition(point);
            addChildNode(object, 2);

            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            object->onHide(0, math::animation::ScaleUpAndRotation);
        }
    }
    selected_object=nullptr;
}


void CompleteSymbolsQuiz::onTouchIcon(Node* node) {
    Math_Object* object=(Math_Object*)node;
    //Chon object dau tien cham phai
    if (selected_object==nullptr) {
        selected_object=object;
        audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drag.mp3");
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void CompleteSymbolsQuiz::onMovedIcon(Node* node, cocos2d::Vec2 position) {
    Math_Object* object = (Math_Object*)node;
    auto point = _panel->convertToNodeSpace(position);
    
    Math_Group* stgroup=nullptr;
    for (Math_Group* _group : _groups) {
        if (_group->hasObject(object)) {
            stgroup = _group;
            break;
        }
    }

    Math_Group* group=nullptr;
    for (Math_Group* _group : _groups) {
        if (_group->isTouchOnGroup(point)) {
            group=_group;
            break;
        }
    }
    
    if (stgroup == group || group == nullptr) {
        object->backToLast(2);
        selected_object=nullptr;
        audio_helper->play("movenumber/wrong.mp3");
    } else {
        stgroup->removeObject(object);
        
        object->removeFromParentAndCleanup(false);
        object->setPosition(point);
        addChildNode(object, 2);
        
        if (group->addObject(object, position)) {
            //Check answer
            selected_object=nullptr;
            if (cocos2d::random(0, 1)==0) {
                audio_helper->play(cocos2d::random(0, 1)==0?"movenumber/drop.mp3":"movenumber/drop2.mp3");
            } else {
                audio_helper->play_sequence(new math::SequenceAudio(), cocos2d::random(0, 1)==0?"movenumber/drop.mp3":"movenumber/drop2.mp3", "girl_smile.mp3");
            }
        }
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void CompleteSymbolsQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CompleteSymbolsQuiz::autoTestingRun(float delaytime) {
    
}
