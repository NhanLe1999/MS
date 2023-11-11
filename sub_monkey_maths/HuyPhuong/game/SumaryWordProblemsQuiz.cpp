//
//  SumaryWordProblemsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "SumaryWordProblemsQuiz.h"

SumaryWordProblemsQuiz* SumaryWordProblemsQuiz::createQuiz()
{
    SumaryWordProblemsQuiz* quiz=new SumaryWordProblemsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool SumaryWordProblemsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void SumaryWordProblemsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    is_time_ask = getJsonBool(json_game, "time_ask", false);
    is_onething = getJsonBool(json_game, "one_thing", false);
    multiple = getJsonInt(json_game ,"multiple", 1);

    rapidjson::Value& drag_drop_type_json = getJsonMember(json_game, "drag_drop_type");
    std::string drag_drop_type_str="result";
    if (!drag_drop_type_json.Empty()) {
        drag_drop_type_str = drag_drop_type_json[cocos2d::random(0, (int)drag_drop_type_json.Size()-1)].GetString();
    }
    if (drag_drop_type_str == "result") {
        dragdrop_type = DT_result;
    } else if (drag_drop_type_str == "operator") {
        dragdrop_type = DT_operator;
    } else {
        dragdrop_type = DT_all_number;
    }
    
    std::string game_type_str = getJsonString(json_game, "game_type", "addition");
    if (game_type_str == "addition") {
        is_addition = true;
    } else if (game_type_str == "subtraction") {
        is_addition = false;
    } else {
        is_addition = cocos2d::random(0, 1) == 0;
    }
    
    //Gen number img
    int total_number = randomRangeJson(json_game, "total");
    if (total_number < 2) {
        total_number = 2;
    }
    int number_small = cocos2d::random(1, total_number-1);
    total_number*=multiple;
    number_small*=multiple;
    
    if (is_addition) {
        img_number.push_back(getNumbers(number_small, number_small).front());
        img_number.push_back(getNumbers(total_number - number_small, total_number - number_small).front());
        img_number.push_back(getNumbers(total_number, total_number).front());
    } else {
        img_number.push_back(getNumbers(total_number, total_number).front());
        img_number.push_back(getNumbers(number_small, number_small).front());
        img_number.push_back(getNumbers(total_number - number_small, total_number - number_small).front());
    }
    
    //Gen img time
    for (int i=0; i<2; i++) {
        auto clock = new math::ClockBase();
        clock->setTime(img_number[i]->getComparisionValue("number"), 0);
        img_time.push_back(clock->getImgOfClock());
    }
    
    //All image
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
    std::string same = getJsonString(json_icon, "same", "none");
    std::string diff = getJsonString(json_icon, "different", "id");

    auto all_img = getImageList("icons");
    for (math::resource::Image* img : all_img) {
        int role = img->getComparisionValue("role");
        if (role == 0) {
            img_thing.push_back(img);
        } else if (role == 1) {
            img_person.push_back(img);
        } else {
            img_unit.push_back(img);
        }
    }
    img_thing = math::resource::ResHelper::filterImages(img_thing, same);
    math::func::shuffle(img_unit);
    math::func::shuffle(img_thing);
    math::func::shuffle(img_person);
    
    //One thing
    if (is_onething) {
        auto img_select = math::func::getRandomNode(img_thing);
        img_thing.clear();
        img_thing.push_back(img_select);
        img_thing.push_back(img_select);
        img_thing.push_back(img_select);
    } else {
        while (img_thing.size() > 2) {
            img_thing.pop_back();
        }
    }
    
    //Tobe
    for (int i=0; i<img_number.size(); i++) {
        int number = img_number[i]->getComparisionValue("number");
        if (i==2) {
            number = 2;
        }
        if (number > 1) {
            auto _tobe = math::resource::ResHelper::createImage("");
            std::vector<std::string> audios;
            _tobe->addProperty("tobe", "are", audios);
            img_tobe.push_back(_tobe);
        } else {
            auto _tobe = math::resource::ResHelper::createImage("");
            std::vector<std::string> audios;
            _tobe->addProperty("tobe", "is", audios);
            img_tobe.push_back(_tobe);
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "time_24") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_time) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "time_12") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_time) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "person_name") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "people_name") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "the") {
            getQuestion()->setOptionalTexts(key, img_person);
        } else if (key == "tobe") {
            getQuestion()->setOptionalTexts(key, img_tobe);
        } else if (key == "use") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<img_number.size(); i++) {
                auto number = img_number[i]->getComparisionValue("number");
                if (i==2) {
                    number = 2;
                }
                if (number < 2) {
                    auto img_txt = img_unit[0]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_unit[0]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "name_n") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<img_number.size(); i++) {
                auto number = img_number[i]->getComparisionValue("number");
                if (i==2) {
                    number = 2;
                }
                if (number < 2) {
                    auto img_txt = img_thing[i]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_thing[i]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_thing);
        }
    }
    getQuestion()->setKeywordValue(is_addition?0:1);
    
    auto img_equal = getOperator("=");
    auto img_plus = getOperator("+");
    auto img_minus = getOperator("-");
    //Gen answer
    if (dragdrop_type == DT_result) {
        int min_number = number_small>(total_number-number_small)?(total_number-number_small):number_small;
        int max_number = total_number;
        if (is_addition) {
            max_number+=multiple;
        } else {
            if (min_number > multiple) {
                min_number-=multiple;
            }
        }
        auto numbers = getNumbers(min_number, max_number);
        img_answer.push_back(img_number.back());
        math::func::shuffle(numbers);
        
        int first_number = img_answer.front()->getComparisionValue("number");
        for (math::resource::Image* img : numbers) {
            int number = img->getComparisionValue("number");
            if (number != first_number && abs(number-first_number)%multiple==0) {
                img_answer.push_back(img);
            }
            if (img_answer.size() == 4) {
                break;
            }
        }
    } else if (dragdrop_type == DT_all_number) {
        int min_number = number_small>(total_number-number_small)?(total_number-number_small):number_small;
        int max_number = total_number;
        if (is_addition) {
            max_number+=multiple;
        } else {
            if (min_number > multiple) {
                min_number-=multiple;
            }
        }
        auto numbers = getNumbers(min_number, max_number);
        math::func::shuffle(numbers);
        for (math::resource::Image*img : img_number) {
            img_answer.push_back(img);
        }
        
        for (math::resource::Image* img : numbers) {
            int number = img->getComparisionValue("number");
            bool valid = true;
            for (math::resource::Image*img2 : img_number) {
                int number2 = img2->getComparisionValue("number");
                if (number == number2 || abs(number-number2)%multiple!=0) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                img_answer.push_back(img);
                break;
            }
        }
    } else {
        img_answer.push_back(img_plus);
        img_answer.push_back(img_minus);
        img_answer.push_back(img_equal);
    }
    
    //Generate phep toan
    std::vector<math::resource::Image*> sentence_img;
    sentence_img.push_back(img_number[0]);
    if (is_addition) {
        sentence_img.push_back(img_plus);
    } else {
        sentence_img.push_back(img_minus);
    }
    sentence_img.push_back(img_number[1]);
    sentence_img.push_back(img_equal);
    sentence_img.push_back(img_number[2]);
    
    object_size = cocos2d::Size(150, 150);
    auto img_drag = objectBG();
    auto img_drop = slotBG();
    for (int i=0; i<sentence_img.size(); i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::deny, sentence_img[i], img_drag);
        object->setObjectSize(object_size);
        object->setValueTextByKey("name_1");
        object->getLabelValue()->setFontSize(object_size.height*0.6);
        object->setImageVisible(false);
        object->showValueTextWithAnimation(false);
        _objects.push_back(object);
        
        bool is_visible = true;
        if (dragdrop_type == DT_result) {
            if (i==4) {
                is_visible = false;
            }
        } else if (dragdrop_type == DT_operator) {
            if (i==1) {
                is_visible = false;
            }
        } else {
            if (i%2==0) {
                is_visible = false;
            }
        }
        
        object->setVisible(is_visible);
        if (!is_visible) {
            auto slot = mpool->getSlot();
            slot->setEndble(i, img_drop);
            slot->setComparisionValue(object->getComparisionValue());
            slot->setSlotSize(object_size);
            _slots.push_back(slot);
        }
    }
    
    for (int i=0; i<img_answer.size(); i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::deny, img_answer[i], img_drag);
        object->setObjectSize(object_size);
        object->setValueTextByKey("name_1");
        object->getLabelValue()->setFontSize(object_size.height*0.6);
        object->setImageVisible(false);
        object->showValueTextWithAnimation(false);
        object->setMoveCallback(CC_CALLBACK_1(SumaryWordProblemsQuiz::onTouchObjectBegan, this), CC_CALLBACK_2(SumaryWordProblemsQuiz::onTouchObjectEnded, this));
        _dragers.push_back(object);
    }
    math::func::shuffle(_dragers);
}

void SumaryWordProblemsQuiz::onStart() {
    onShowBottomBar(0, 0.3, true, CC_CALLBACK_1(SumaryWordProblemsQuiz::onAnswerDone, this));
    Math_Quiz::onStart();
    
    int number_inline = 5;
    int delta = 50;
    float objs_width = number_inline*(object_size.width + delta) - delta;
    while (objs_width>quiz_size.width * 0.95) {
        delta-=5;
        objs_width = number_inline*(object_size.width + delta) - delta;
    }
    float start_x = (quiz_size.width-objs_width) /2.0;
    math::func::setPositionList(_objects, number_inline, cocos2d::Rect(start_x, 0, quiz_size.width - start_x*2, quiz_size.height*0.7-getQuestion()->getContentSize().height-20));
    
    if (dragdrop_type == DT_result) {
        _slots.front()->setPosition(_objects[4]->getPosition());
    } else if (dragdrop_type == DT_operator) {
        _slots.front()->setPosition(_objects[1]->getPosition());
    } else {
        for (int i=0; i<_slots.size(); i++) {
            _slots[i]->setPosition(_objects[i*2]->getPosition());
        }
    }
    
    for (Math_Object* object : _objects) {
        addChildNode(object);
        object->onShow(object->getID() * 0.1);
    }
    for (Math_Slot* slot : _slots) {
        addChildNode(slot);
        slot->onShow(slot->getId() * 0.1);
    }
    
    float drag_x = 40;
    math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(drag_x, 0, quiz_size.width-300-drag_x, quiz_size.height*0.3-30));
    
    int i=0;
    for (Math_Object* object : _dragers) {
        addChildNodeToBottomPanel(object);
        object->onShow(i * 0.1 + 0.5, math::animation::SlideUp);
        i++;
    }
}

void SumaryWordProblemsQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    
    scheduleOnce([=](float){
        for (Math_Object* object : _dragers) {
            object->enableDrag(_panel_bot);
        }
    }, 0.5, "enable_drag");
}

void SumaryWordProblemsQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}


void SumaryWordProblemsQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (Math_Object* object : _objects) {
        object->onHide(object->getID() * 0.1);
    }
    for (Math_Slot* slot : _slots) {
        slot-> onHide(slot->getId() * 0.1);
    }
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 0.5, "on complete");
}

#pragma mark - object touch events callback
void SumaryWordProblemsQuiz::onAnswerDone(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    math::comparision::Key cmp_key = math::comparision::number;
    if (dragdrop_type == DT_operator) {
        cmp_key = math::comparision::name;
    }
    
    bool correct = true;
    for (Math_Slot * slot : _slots) {
        if (slot->isUsed()) {
            auto linker = slot->getLinker();
            if (!slot->isCorrect(cmp_key)) {
                correct = false;
                slot->removeLinker();
                linker->backToStart();
            }
        } else {
            correct = false;
        }
    }
    
    answer->onAnimationClick(correct);
    if (correct) {
        scheduleOnce([=](float){
            this->onExplanation(true);
        }, 1.5, "wait explain");
    }
}

void SumaryWordProblemsQuiz::onTouchObjectBegan(Node* node) {
    audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drag.mp3");
}

void SumaryWordProblemsQuiz::onTouchObjectEnded(Node* node, cocos2d::Vec2 position) {
    Math_Object* object=(Math_Object*)node;
    //Doi tuong keo tha nam trong 1 slot nao do.
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
    
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
                audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drop.mp3", cocos2d::random(0, 1)==0?"girl_smile.mp3":"");
            } else {
                if (t_slot->isUsed()) {
                    auto linker = t_slot->getLinker();
                    m_slot->removeLinker();
                    t_slot->removeLinker();
                    
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    
                    m_slot->setLinker(linker);
                    linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
                    audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drop2.mp3", cocos2d::random(0, 1)==0?"girl_smile.mp3":"");
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drop2.mp3", cocos2d::random(0, 1)==0?"girl_smile.mp3":"");
                }
            }
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
            audio_helper->play("howmany/pop_out10.mp3");
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->backToStart();
                audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drop2.mp3", cocos2d::random(0, 1)==0?"girl_smile.mp3":"");
            } else {
                audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drop.mp3", cocos2d::random(0, 1)==0?"girl_smile.mp3":"");
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
        } else {
            //move start
            audio_helper->play("howmany/pop_out10.mp3");
            object->backToStart();
        }
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void SumaryWordProblemsQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void SumaryWordProblemsQuiz::autoTestingRun(float delaytime) {
    
}

