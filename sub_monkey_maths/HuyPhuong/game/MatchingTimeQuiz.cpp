//
//  MatchingTimeQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "MatchingTimeQuiz.h"
#include "AnalogClock.h"
#include "DigitalClock.h"

MatchingTimeQuiz* MatchingTimeQuiz::createQuiz()
{
    MatchingTimeQuiz* quiz=new MatchingTimeQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MatchingTimeQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MatchingTimeQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string game_type_str = getJsonString(json_game, "game_type", "name_picture");
    if (game_type_str == "name_picture") {
        game_type = GT_Name_picture;
    } else if (game_type_str == "time_clock") {
        game_type = GT_Time_Clock;
    } else if (game_type_str == "clock_clock") {
        game_type = GT_Clock_Clock;
    } else {
        game_type = GT_Picture_Picture;
    }
    
    number_pair = randomRangeJson(json_game, "number_pair");
    
    //Time config
    rapidjson::Value& json_time = getJsonMember(json_game, "time");
    std::string show_str = getJsonString(json_time, "show", "text");
    if (show_str == "text") {
        show_type = ST_Text;
    } else if (show_str == "semi_text") {
        show_type = ST_Semi_Text;
    } else {
        show_type = ST_Number;
    }
    time_24 = getJsonBool(json_time, "moed_24", true);

    //Clock config
    rapidjson::Value& json_clock = getJsonMember(json_game, "clock");
    is_same_clock = getJsonBool(json_clock, "is_same", false);
    rapidjson::Value& json_clock_select = getJsonMember(json_clock, "select");
    if (json_clock_select.Empty()) {
        is_analog = true;
    } else {
        is_analog = (std::string)json_clock_select[cocos2d::random(0, (int)json_clock_select.Size()-1)].GetString() == "analog";
    }
    is_time_right = getJsonBool(json_clock, "type", true);
    
    //
    auto img_bg = objectBG();
    cocos2d::Size object_size = cocos2d::Size(175, 175);
    is_time_game = (game_type == GT_Time_Clock) || (game_type == GT_Clock_Clock);
    if (is_time_game) {
        //time & clock
        std::vector<int> time_list = randomArrayIndexes(number_pair, 12);
        
        if (is_analog) {
            for (int value : time_list) {
                auto clock = math::AnalogClock::createClock(1);
                clock->setTime(value+1, is_time_right?0:30);
                clock->setClockSize(object_size.height);
                clock_list.push_back(clock);
            }
        } else {
            for (int value : time_list) {
                auto clock = math::DigitalClock::createClock(1);
                clock->setTime(value+1, is_time_right?0:30);
                clock->setClockSize(object_size.height);
                clock_list.push_back(clock);
            }
        }
        
        if (game_type == GT_Time_Clock) {
            for (int i=0; i<number_pair; i++) {
                auto clock = clock_list[i];
                auto img = clock->getImgOfClock();
                
                auto object = mpool->getObject();
                object->setEnable(i, math::common::deny, img, nullptr);
                object->setObjectSize(cocos2d::Size(274, 84));
                object->setImageVisible(false);
                object->setValueTextByKey("name_1");
                object->showValueTextWithAnimation(false);
                object->setBackground(img_bg, 0.3);
                object->setMoveCallback(nullptr, CC_CALLBACK_2(MatchingTimeQuiz::onMovedObject, this));
                
                auto txt_val = object->getLabelValue();
                txt_val->setTextColor(cocos2d::Color4B::BLACK);
                //txt_val->enableOutline(cocos2d::Color4B::BLACK, 3);
                
                _dragers.push_back(object);
                
                auto slot = mpool->getSlot();
                slot->setEndble(i, img_bg);
                slot->setSlotSize(object->getContentSize());
                slot->setComparisionValue(object->getComparisionValue());
                _slots.push_back(slot);
            }
        } else {
            if (!is_analog) {
                for (int value : time_list) {
                    auto clock = math::AnalogClock::createClock(1);
                    clock->setTime(value+1, is_time_right?0:30);
                    clock->setClockSize(object_size.height);
                    clock_drag_list.push_back(clock);
                }
            } else {
                for (int value : time_list) {
                    auto clock = math::DigitalClock::createClock(1);
                    clock->setTime(value+1, is_time_right?0:30);
                    clock->setClockSize(object_size.height);
                    clock_drag_list.push_back(clock);
                }
            }
            
            for (math::ClockBase * clock : clock_drag_list) {
                auto object = mpool->getObject();
                object->setEnable(clock->getHour(), math::common::deny, clock->getImgOfClock(), nullptr);
                object->setObjectSize(object_size);
                object->setImageVisible(false);
                clock->setPosition(object_size/2);
                object->addChild(clock);
                object->setMoveCallback(nullptr, CC_CALLBACK_2(MatchingTimeQuiz::onMovedObject, this));
                _dragers.push_back(object);
                
                auto slot = mpool->getSlot();
                slot->setEndble(0, img_bg);
                slot->setSlotSize(object->getContentSize());
                slot->setComparisionValue(object->getComparisionValue());
                _slots.push_back(slot);
            }
        }
    } else {
        if (game_type == GT_Name_picture) {
            auto all_imgs = getImageList("icons");
            img_list = getImageList(all_imgs, number_pair, "name_1");
        } else {
            auto all_imgs = getImageList("icons");
            img_list = getImageList(all_imgs, number_pair, "name_1");
            for (int i=0; i<number_pair; i++) {
                for (math::resource::Image* img : all_imgs) {
                    if (img != img_list[i] && img->getText() == img_list[i]->getText()) {
                        img_list.push_back(img);
                        break;
                    }
                }
            }
        }
        
        //Gen object
        for (int i=0; i<number_pair; i++) {
            auto object = mpool->getObject();
            object->setEnable(i, math::common::deny, img_list[i], nullptr);
            object->setObjectSize(object_size);
            _objects.push_back(object);
        }
        
        //Gen dragger
        if (game_type == GT_Name_picture) {
            for (int i=0; i<number_pair; i++) {
                auto object = mpool->getObject();
                object->setEnable(i, math::common::deny, img_list[i], nullptr);
                object->setObjectSize(cocos2d::Size(274, 84));
                object->setImageVisible(false);
                object->setValueTextByKey("name_1");
                object->showValueTextWithAnimation(false);
                object->setBackground(img_bg, 0.3);
                object->setMoveCallback(nullptr, CC_CALLBACK_2(MatchingTimeQuiz::onMovedObject, this));
                
                auto txt_val = object->getLabelValue();
                txt_val->setTextColor(cocos2d::Color4B::BLACK);
//                txt_val->enableOutline(cocos2d::Color4B::BLACK, 3);
                
                _dragers.push_back(object);
                
                auto slot = mpool->getSlot();
                slot->setEndble(i, img_bg);
                slot->setSlotSize(object->getContentSize());
                slot->setComparisionValue(object->getComparisionValue());
                _slots.push_back(slot);
            }
        } else {
            for (int i=0; i<number_pair; i++) {
                auto object = mpool->getObject();
                object->setEnable(i, math::common::deny, img_list[i+number_pair], nullptr);
                object->setObjectSize(object_size);
                object->setMoveCallback(nullptr, CC_CALLBACK_2(MatchingTimeQuiz::onMovedObject, this));
                _dragers.push_back(object);
                
                auto slot = mpool->getSlot();
                slot->setEndble(i, img_bg);
                slot->setSlotSize(object->getContentSize());
                slot->setComparisionValue(object->getComparisionValue());
                _slots.push_back(slot);
            }
        }
    }
    math::func::shuffle(_dragers);
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue((int)game_type);
    
}

void MatchingTimeQuiz::onStart() {
    float bar_height = 0.3;
    if (game_type == GT_Name_picture || game_type == GT_Time_Clock) {
        bar_height = 0.25;
    }
    onShowBottomBar(0, bar_height);
    Math_Quiz::onStart();
    
    cocos2d::Vec2 center_view = cocos2d::Vec2(quiz_size.width, quiz_size.height*(1-bar_height) - getQuestion()->getContentSize().height - 20) / 2;

    float delta = 110;
    float object_width = 300;
    float delta_answer = 0;
    
    float total_width = object_width*number_pair + delta*(number_pair-1);
    while (total_width > quiz_size.width - 50) {
        delta-=10;
        total_width = object_width*number_pair + delta*(number_pair-1);
        delta_answer = 15;
    }
    
    float start_x = center_view.x - object_width - delta;
    if (number_pair == 2) {
        start_x = center_view.x - (object_width + delta)/2;
    }
    
    float padding = 12;
    cocos2d::Size box_size = cocos2d::Size(object_width, 400);
    cocos2d::Size box_top_size = cocos2d::Size(object_width-padding*2, 199-padding);
    cocos2d::Size box_bot_size = cocos2d::Size(object_width-padding*2, 200 - padding);
    if (game_type == GT_Name_picture || game_type == GT_Time_Clock) {
        box_size = cocos2d::Size(object_width, 300);
        box_bot_size = cocos2d::Size(object_width-padding*2, 100-padding);
    }
    for (int i=0; i<number_pair; i++) {
        auto view_box = cocos2d::ui::ImageView::create(getUIPath("common/frame/square.png"));
        view_box->setScale9Enabled(true);
        auto render_size = view_box->getVirtualRendererSize();
        view_box->setCapInsets(cocos2d::Rect(render_size.width*0.3, render_size.height*0.3, render_size.width*0.4, render_size.height*0.4));
        view_box->setContentSize(box_size);
        view_box->setPosition(cocos2d::Vec2(start_x + (object_width + delta)*i, center_view.y));
        addChildNode(view_box);
        
        auto view_box_1 = cocos2d::ui::ImageView::create(getUIPath("common/frame/square2.png"));
        view_box_1->setScale9Enabled(true);
        auto render_size_1 = view_box_1->getVirtualRendererSize();
        view_box_1->setCapInsets(cocos2d::Rect(render_size_1.width*0.3, render_size_1.height*0.3, render_size_1.width*0.4, render_size_1.height*0.4));
        view_box_1->setContentSize(box_top_size);
        view_box_1->setPosition(cocos2d::Vec2(box_size.width/2, box_size.height - box_top_size.height/2 - padding));
        view_box->addChild(view_box_1);
        
        auto view_box_2 = cocos2d::ui::ImageView::create(getUIPath("common/frame/brown1.png"));
        view_box_2->setScale9Enabled(true);
        auto render_size_2 = view_box_2->getVirtualRendererSize();
        view_box_2->setCapInsets(cocos2d::Rect(render_size_2.width*0.3, render_size_2.height*0.3, render_size_2.width*0.4, render_size_2.height*0.4));
        view_box_2->setContentSize(box_bot_size);
        view_box_2->setPosition(cocos2d::Vec2(box_size.width/2, box_bot_size.height/2+padding));
        view_box->addChild(view_box_2);
        
        view_box->setOpacity(0);
        view_box->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.1*i), cocos2d::FadeIn::create(0.5)));
        view_box_1->setOpacity(0);
        view_box_1->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.1*i), cocos2d::FadeIn::create(0.5)));
        view_box_2->setOpacity(0);
        view_box_2->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.1*i), cocos2d::FadeIn::create(0.5)));
        
        ui_elements.push_back(view_box);
    }
    
    if (is_time_game) {
        for (int i=0; i<number_pair; i++) {
            auto clock = clock_list[i];
            clock->setPosition(cocos2d::Vec2(start_x + (object_width + delta)*i, center_view.y + box_size.height/2 - box_top_size.height/2 - padding));
            addChildNode(clock);
            clock->show();
        }
    } else {
        for (int i=0; i<number_pair; i++) {
            auto picture = _objects[i];
            picture->setPosition(cocos2d::Vec2(start_x + (object_width + delta)*i, center_view.y + box_size.height/2 - box_top_size.height/2 - padding));
            addChildNode(picture);
            picture->onShow(0);
        }
    }
    
    for (int i2=0; i2<number_pair; i2++) {
        auto slot = _slots[i2];
        slot->setPosition(cocos2d::Vec2(start_x + (object_width + delta)*i2, center_view.y - box_size.height/2 + box_bot_size.height/2+padding));
        slot->setVisible(false);
        addChildNode(slot);
        slot->onShow(0, math::animation::No_Effect);
    }
    
    for (int i2=0; i2<number_pair; i2++) {
        auto drager = _dragers[i2];
        drager->setPosition(cocos2d::Vec2(start_x + (object_width + delta)*i2 + (1-i2) * 15, (quiz_size.height * bar_height - 30)/2));
        addChildNodeToBottomPanel(drager);
        drager->onShow(0.3+i2*0.1, math::animation::SlideUp);
    }
}

void MatchingTimeQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    scheduleOnce([=](float){
        for (Math_Object* drager : _dragers) {
            drager->enableDrag(_panel_bot);
        }
    }, 0.5, "enable drag");
}

void MatchingTimeQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            this->nextQuestion();
        }, 1.5, "go complete");
    }
}

void MatchingTimeQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (int i=0; i<number_pair; i++) {
        _slots[i]->onHide(i*0.1);
    }
    
    if (is_time_game) {
        for (int i=0; i<number_pair; i++) {
            clock_list[i]->destroy(0.1*i);
        }
    } else {
        for (int i=0; i<number_pair; i++) {
            _objects[i]->onHide(0.1*i);
        }
    }
    
    for (int i=0; i<number_pair; i++) {
        auto box = ui_elements[i];
        box->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5+0.1*i), cocos2d::FadeOut::create(0.25)));
        for (auto child : box->getChildren()) {
            child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5+0.1*i), cocos2d::FadeOut::create(0.25)));
        }
    }
    
    scheduleOnce([=](float){
        Math_Quiz::onComplete();
    }, 1.0, "on complete");
}

#pragma mark - object touch events callback
void MatchingTimeQuiz::onTouchedObject(cocos2d::Node *node) {
    //auto object = static_cast<Math_Object*>(node);
    
    audio_helper->play_sequence(new math::SequenceAudio(), "movenumber/drag.mp3");

}


void MatchingTimeQuiz::onMovedObject(Node* node, cocos2d::Vec2 position) {
    auto object = static_cast<Math_Object*>(node);
    
    position = _panel->convertToNodeSpace(position);
    //Doi tuong keo tha nam trong 1 slot nao do.
    
    Math_Slot* t_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    
    bool complete = false;
    if (t_slot != nullptr) {
        if (t_slot->isUsed()) {
            //move start
            object->backToStart();
            audio_helper->play("movenumber/wrong.mp3");
        } else {
            //Check true
            t_slot->setLinker(object);

            if (t_slot->isCorrect(math::comparision::name)) {
                object->removeFromParentAndCleanup(false);
                addChildNode(object);
                object->setPosition(position);
                t_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                object->setTouch(math::common::deny);
                
                object->getBackground()->runAction(cocos2d::FadeOut::create(0.25));
                auto txt_val = object->getLabelValue();
                txt_val->disableEffect();
                txt_val->setTextColor(cocos2d::Color4B::WHITE);
                
                object->onTouchObject(true, 0.25);
                
                complete = true;
                for (Math_Slot* slot : _slots) {
                    if (!slot->isUsed()) {
                        complete = false;
                        break;
                    }
                }
            } else {
                t_slot->removeLinker();
                object->backToStart();
                audio_helper->play("movenumber/wrong.mp3");
            }
        }
    } else {
        //move start
        object->backToStart();
        audio_helper->play("movenumber/wrong.mp3");
    }
    
    if (complete) {
        this->onExplanation(true);
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MatchingTimeQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MatchingTimeQuiz::autoTestingRun(float delaytime) {
    
}

