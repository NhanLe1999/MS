//
//  MakeEnoughQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "MakeEnoughQuiz.h"

MakeEnoughQuiz* MakeEnoughQuiz::createQuiz()
{
    MakeEnoughQuiz* quiz=new MakeEnoughQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MakeEnoughQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MakeEnoughQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    game_mode = getJsonString(json_game, "game_mode", "match");
    int number_cupple = randomRangeJson(json_game, "number");
    align_pair = getJsonBool(json_game, "align_pair", false);
    
    int numberover_max = getJsonInt(getJsonMember(json_game, "over_number"), "maximum", 2);
    int numberover_min = getJsonInt(getJsonMember(json_game, "over_number"), "minimum", 1);
    int number_over = cocos2d::random(numberover_min, numberover_max);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    std::string display = getJsonString(json_object, "display", "icon");
    std::string fit_object = getJsonString(json_object, "fit_size", "none");

    //TODO: ICON CONFIG
    std::string icon_diff = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    
    auto all_imgs = getImageList("icons");
    if (game_mode.compare("color")==0)  {
        if (icon_same == "none" || icon_same == "name") {
            icon_same = "name_1";
        }
        auto img_select = math::func::getRandomNode(all_imgs);
        bool filter_prop = true;
        for (math::resource::Property prop : img_select->comparisions) {
            if (prop.key == icon_same) {
                filter_prop = false;
                break;
            }
        }
        if (filter_prop) {
            auto value = img_select->getText(icon_same);
            for (math::resource::Image* img : all_imgs) {
                if (img->getText(icon_same) == value) {
                    if (img->getText("color") == "white") {
                        white_img = img;
                    } else {
                        color_imgs.push_back(img);
                    }
                }
            }
        } else {
            auto value = img_select->getComparisionValue(icon_same);
            for (math::resource::Image* img : all_imgs) {
                if (img->getComparisionValue(icon_same) == value) {
                    if (img->getText("color") == "white") {
                        white_img = img;
                    } else {
                        color_imgs.push_back(img);
                    }
                }
            }
        }
        
        math::func::shuffle(color_imgs);
        while (color_imgs.size() > 3) {
            color_imgs.pop_back();
        }
        color_img = color_imgs.front();
        img_list = color_imgs;

        float object_w = quiz_size.width / (number_cupple+number_over);
        if (object_w > 140) {
            object_size = cocos2d::Size(120, 120);
        } else {
            object_size = cocos2d::Size(object_w-20, object_w-20);
        }
        
        for (int i=0; i<number_cupple; i++) {
            auto object = mpool->getObject();
            object->setEnable(i, math::common::deny, color_imgs.front(), nullptr);
            object->setObjectSize(object_size);
            _objects.push_back(object);
            if (display != "icon") {
                object->setImageVisible(false);
                object->setValueTextByKey(display);
                object->showValueTextWithAnimation(false);
            }
            
            auto object2 = mpool->getObject();
            object2->setEnable(i, math::common::touch, white_img, nullptr);
            object2->setObjectSize(object_size);
            object2->setColoring(true);
            color_objects.push_back(object2);
            if (display != "icon") {
                object2->setImageVisible(false);
                object2->setValueTextByKey(display);
                object2->showValueTextWithAnimation(false);
            }
        }
        
        for (int i=0; i<number_over; i++) {
            auto object2 = mpool->getObject();
            object2->setEnable(i, math::common::touch, white_img, nullptr);
            object2->setObjectSize(object_size);
            object2->setColoring(true);
            color_objects.push_back(object2);
            if (display != "icon") {
                object2->setImageVisible(false);
                object2->setValueTextByKey(display);
                object2->showValueTextWithAnimation(false);
            }
        }
    } else if (game_mode.compare("match")==0) {
        bg_img = groupBG();
        img_list = getImageList("icons", 2, icon_diff, icon_same);
        std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image*b)->bool{
            return a->getComparisionValue("role") < b->getComparisionValue("role");
        });
        
        int number_gr = 2;
        std::vector<int> list_numbers;
        list_numbers.push_back(number_cupple);
        int value = number_cupple +  cocos2d::random(1, numberover_max) * (cocos2d::random(0, 1) * 2 - 1);
        list_numbers.push_back(value);
        
        //Gen groups to matching
        object_size = cocos2d::Size(100, 100);
        cocos2d::Size group_size = cocos2d::Size(object_size.width*6.5, object_size.height*3);
        
        std::vector<Math_Object*> drag_list;
        for (int i=0; i<number_cupple; i++) {
            auto object = mpool->getObject();
            object->setEnable(i, math::common::deny, img_list.front(), nullptr);
            object->setObjectSize(object_size);
            
            if (display != "icon") {
                object->setImageVisible(false);
                object->setValueTextByKey(display);
                object->showValueTextWithAnimation(false);
            }
            drag_list.push_back(object);
        }
        
        auto group = mpool->getGroup();
        group->setEnable(0, drag_list, math::group::queue, 10);
        group->setObjectSize(object_size);
        group->setGroupQueue(group_size);
        group->setBackground(bg_img, 0.3);
        _groups.push_back(group);
        
        float object_w = (quiz_size.height * 0.4) / 4;
        cocos2d::Size object_size2 = cocos2d::Size(object_w, object_w);
        cocos2d::Size group_size2 = cocos2d::Size(object_size2.width*6.5, object_size2.height*3);
        for (int i=0; i < number_gr; i++) {
            std::vector<Math_Object*> drag_list;
            for (int j=0; j<list_numbers[i]; j++) {
                auto object = mpool->getObject();
                object->setEnable(j, math::common::deny, img_list[1], nullptr);
                object->setObjectSize(object_size2);
                
                if (display != "icon") {
                    object->setImageVisible(false);
                    object->setValueTextByKey(display);
                    object->showValueTextWithAnimation(false);
                }
                drag_list.push_back(object);
            }
            
            auto group = mpool->getGroup();
            group->setEnable(0, drag_list, math::group::queue, 10);
            group->setObjectSize(object_size2);
            group->setGroupQueue(group_size2);
            group->setBackground(bg_img, 0.3);
            _groups.push_back(group);
            group->enableToch(CC_CALLBACK_1(MakeEnoughQuiz::onTouchedGroup, this));
        }
    } else {
        bg_img = slotBG();
        img_list = getImageList("icons", 2, icon_diff, icon_same);
        std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image*b)->bool{
            return a->getComparisionValue("role") < b->getComparisionValue("role");
        });
        
        float object_w = quiz_size.width / (number_cupple+number_over);
        if (object_w > 140) {
            object_size = cocos2d::Size(120, 120);
        } else {
            object_size = cocos2d::Size(object_w-20, object_w-20);
        }
        for (int i=0; i<number_cupple; i++) {
            auto object = mpool->getObject();
            object->setEnable(i, math::common::deny, img_list[0], nullptr);
            object->setObjectSize(object_size * 0.9);
            _objects.push_back(object);
            
            if (display.compare("icon")!=0) {
                object->setImageVisible(false);
                object->setValueTextByKey(display);
                object->showValueTextWithAnimation(false);
            }
        }
        
        for (int i=0; i<number_over+number_cupple; i++) {
            auto slot = mpool->getSlot();
            slot->setEndble(i, bg_img);
            slot->setSlotSize(object_size);
            _slots.push_back(slot);
        }
        
        auto c_object = mpool->getObject();
        c_object->setEnable(0, math::common::deny, img_list[1], nullptr);
        c_object->setObjectSize(object_size * 0.9);
        c_object->setMoveCallback(CC_CALLBACK_1(MakeEnoughQuiz::onMoveBegan, this), CC_CALLBACK_2(MakeEnoughQuiz::onMovedObject, this));
        if (display.compare("icon")!=0) {
            c_object->setImageVisible(false);
            c_object->setValueTextByKey(display);
            c_object->showValueTextWithAnimation(false);
        }
        _dragers.push_back(c_object);
    }
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
}

void MakeEnoughQuiz::onStart() {
    
    if (game_mode.compare("color")==0) {
        int max_in_line = (int)color_objects.size();
        std::vector<std::string> colors;
        for (math::resource::Image* img : color_imgs) {
            colors.push_back(img->getText("color"));
        }
        showColorsPad(colors, [=](cocos2d::Node* node){
            current_color = static_cast<Math_Object*>(node)->getComparisionValue().color;
            CCLOG("%s", current_color.c_str());
        }, CC_CALLBACK_1(MakeEnoughQuiz::onAnswerDone,this), 0, false);
        
        quizBegan(0, true);
        
        math::func::setPositions(color_objects, cocos2d::Rect(0, quiz_size.height*0.3, quiz_size.width, quiz_size.height*0.7 - questionHeight()));
        for (int i=0; i < max_in_line; i++) {
            auto point = color_objects[i]->getPosition();
            color_objects[i]->setPosition(cocos2d::Vec2(point.x, point.y - object_size.height*0.8));
        }

        if (align_pair) {
            for (int i=0; i < _objects.size(); i++) {
                auto point = color_objects[i]->getPosition();
                _objects[i]->setPosition(cocos2d::Vec2(point.x, point.y + object_size.height*1.6));
            }
        } else {
            math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height*0.3, quiz_size.width, quiz_size.height*0.7 - questionHeight()));
            for (int i=0; i < _objects.size(); i++) {
                auto point = _objects[i]->getPosition();
                _objects[i]->setPosition(cocos2d::Vec2(point.x, point.y + object_size.height*0.8));
            }
        }
        
        float dt=0;
        for (Math_Object* object : _objects) {
            addChildNode(object, 2);
            object->onShow(dt);
            dt+=0.1;
        }
        
        dt=0;
        for (Math_Object* object : color_objects) {
            addChildNode(object, 2);
            object->onShow(dt);
            object->setTouchCallback(CC_CALLBACK_1(MakeEnoughQuiz::onTouchedObject, this));
            dt+=0.1;
        }
    } else if (game_mode.compare("match")==0)  {
        onShowBottomBar(0, 0.4, false, nullptr, nullptr);
        Math_Quiz::onStart();
        
        _groups[0]->setPosition(cocos2d::Vec2(quiz_size.width/2, (quiz_size.height *0.6 - getQuestion()->getContentSize().height-20)/2));
        addChildNode(_groups[0]);
        _groups[0]->onShow(0, math::animation::ScaleUp);
        
        auto gr_size = _groups.back()->getContentSize();
        float delta = ((quiz_size.width - 100) - gr_size.width*2) / 3;
        delta = delta > 100 ? 100 : delta;
        std::vector<Math_Group*> groups_touch;
        groups_touch.push_back(_groups[1]);
        groups_touch.push_back(_groups[2]);
        
        math::func::smartSetPosition(groups_touch, gr_size, cocos2d::Rect(10, 10, quiz_size.width-20, quiz_size.height*0.4-20), delta);
        for (int  i=0; i<groups_touch.size(); i++) {
            addChildNodeToBottomPanel(groups_touch[i]);
            groups_touch[i]->onShow(0.5 + i*0.1, math::animation::SlideUp);
        }
        
    } else {
        int max_in_line = (int)_slots.size();

        onShowBottomBar(0, 0.25, true, CC_CALLBACK_1(MakeEnoughQuiz::onAnswerDone, this), nullptr);
        Math_Quiz::onStart();
        
        float delta = (quiz_size.width - max_in_line * object_size.width) / (max_in_line+1);
        delta = delta > 80 ? 80 : delta;
        math::func::smartSetPosition(_slots, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height - 20), delta);
        for (int i=0; i < max_in_line; i++) {
            auto point = _slots[i]->getPosition();
            _slots[i]->setPosition(cocos2d::Vec2(point.x, point.y - object_size.height*0.8));
        }
        
        if (align_pair) {
            for (int i=0; i < _objects.size(); i++) {
                auto point = _slots[i]->getPosition();
                _objects[i]->setPosition(cocos2d::Vec2(point.x, point.y + object_size.height*1.6));
            }
        } else {
            math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height - 20), delta);
            for (int i=0; i < _objects.size(); i++) {
                auto point = _objects[i]->getPosition();
                _objects[i]->setPosition(cocos2d::Vec2(point.x, point.y + object_size.height*0.8));
            }
        }
        
        float dt=0;
        for (Math_Object* object : _objects) {
            addChildNode(object, 2);
            object->onShow(dt);
            dt+=0.1;
        }
        
        dt=0;
        for (Math_Slot* slot : _slots) {
            addChildNode(slot, 2);
            slot->onShow(dt);
            dt+=0.1;
        }
        
        _dragers.front()->setPosition(cocos2d::Vec2((quiz_size.width - 140)/2, (quiz_size.height *0.25 - 30)/2));
        addChildNodeToBottomPanel(_dragers.front());
        _dragers.front()->onShow(0.5, math::animation::SlideUp);
    }
}

void MakeEnoughQuiz::startQuestion() {
     
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        
        if (game_mode.compare("copy")==0) {
            for (Math_Object* object : _dragers) {
                object->enableDrag(_panel_bot);
                object->setCopied(true);
            }
        }
    } else {
       
    }
}

void MakeEnoughQuiz::onExplanation(bool correct) {
    disableControlQuiz();
    scheduleOnce([=](float dt){
        this->nextQuestion();
    }, 1.0, "delay explain");
}

void MakeEnoughQuiz::onComplete() {
    
    if (game_mode.compare("color")==0) {
        int number_color = (int)color_objects.size();
        int number_tempt = (int)_objects.size();
        
        for (int i=number_color-1; i >=0; i--) {
            if (i<number_tempt) {
                Math_Object* object = _objects[i];
                object->onHide((number_color-i)*0.1);
            }
            Math_Object* m_object = color_objects[i];
            m_object->onHide((number_color-i)*0.1);
        }
    } else if (game_mode.compare("match")==0) {
        _groups[0]->setDisable(0, math::animation::ScaleUp);
        for (int  i=1; i<_groups.size(); i++) {
            _groups[i]->setDisable(0.4+0.1*i, math::animation::SlideUp);
        }
    } else {
        for (int i=0 ; i<_slots.size(); i++) {
            if (i<_objects.size()) {
                _objects[i]->onHide(i*0.1);
            }
            _slots[i]->onHide(i*0.1);
        }
        
        for (Math_Object* object : _dragers) {
            object->onHide(0.5);
        }
    }
    
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.0, "complete_quiz");
}

#pragma mark - object touch events callback
void MakeEnoughQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = static_cast<Math_Object*>(node);
    if (current_color == "" || current_color != color_img->getText("color")) {
        object->onTouchObject(false);
        return;
    }
    
    bool is_color = true;
    int index=0;
    for (Math_Object* obj : select_list) {
        if (obj->isSame(object)) {
            is_color = false;
            break;
        }
        index++;
    }
    if (is_color) {
        object->onTouchObject(true);
        object->setImage(color_img);
        object->setComparisionValue("color", color_img->getText("color"));
        select_list.push_back(object);
    } else {
        object->setImage(white_img);
        object->setComparisionValue("color", "white");
        select_list.erase(select_list.begin()+index);
    }
}

void MakeEnoughQuiz::onTouchedGroup(cocos2d::Node* node)
{
    Math_Group* group = (Math_Group*)node;
    bool is_correct = group->getNumberObjects() == _groups[0]->getNumberObjects();
    group->onTouchedAnimation(is_correct);
    if (is_correct) {
        onExplanation(is_correct);
    }
}

void MakeEnoughQuiz::onMoveBegan(Node* node) {
    Math_Object* object=(Math_Object*)node;
    make_copy=true;
    //Kiem tra object o dock hay o group
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            make_copy = false;
        }
    }
    
    if (make_copy) {
        Math_Object* object_copy = object->clone();
        object_copy->onShow(0, math::animation::No_Effect);
        _dragers.clear();
        _dragers.push_back(object_copy);
    }
}

void MakeEnoughQuiz::onMovedObject(Node* node, cocos2d::Vec2 position){
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
        object->onHide(0, math::animation::ScaleUpAndRotation);
    } else {
        if (m_slot == nullptr) {
            if (t_slot->isUsed()) {
                object->onHide(0, math::animation::ScaleUpAndRotation);
            } else {
                object->removeFromParentAndCleanup(false);
                object->setPosition(position);
                addChildNode(object, 10);
                
                t_slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            }
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

void MakeEnoughQuiz::onAnswerDone(cocos2d::Node *node) {
    Math_Answer* answer = static_cast<Math_Answer*>(node);
    bool is_correct = true;
    if (game_mode.compare("color")==0) {
        is_correct = select_list.size() == _objects.size();
        
        answer->onAnimationClick(is_correct);
        if (is_correct) {
            onExplanation(true);
        } else {
            scheduleOnce([=](float){
                for (Math_Object* object : select_list) {
                    object->setImage(white_img);
                    object->setComparisionValue("color", "white");
                }
                select_list.clear();
            }, 0.5, "replay");            
            disableControlQuiz(0.5);
        }
    } else {
        int count =0;
        for (Math_Slot *slot : _slots) {
            if (slot->isUsed()) {
                count++;
            }
        }
        
        is_correct = count == _objects.size();
        
        if (is_correct) {
            scheduleOnce([=](float){
                answer->onAnimationClick(true);
                onExplanation(true);
            }, 0.5, "complete");
        } else {
            scheduleOnce([=](float){
                for (Math_Slot* slot : _slots) {
                    if (slot->isUsed()) {
                        auto linker = slot->getLinker();
                        slot->removeLinker();
                        linker->onHide(0);
                    }
                }
                select_list.clear();
            }, 0.5, "complete");
            answer->onAnimationClick(false);
            disableControlQuiz(0.5);
        }
    }
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MakeEnoughQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MakeEnoughQuiz::autoTestingRun(float delaytime)
{
    
    
}

