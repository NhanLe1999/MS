//
//  MoveObjectMakePictureQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "MoveObjectMakePictureQuiz.h"

MoveObjectMakePictureQuiz* MoveObjectMakePictureQuiz::createQuiz()
{
    MoveObjectMakePictureQuiz* quiz=new MoveObjectMakePictureQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MoveObjectMakePictureQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void MoveObjectMakePictureQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    
    auto all_images = getImageList("icons");
    auto pair_selected = math::func::getRandomNode(all_images)->getComparisionValue("pair");
    
    for (math::resource::Image* img : all_images) {
        if (img->getComparisionValue("pair") == pair_selected) {
            int role = img->getComparisionValue("role");
            if (role == 0) {
                img_complete = img;
            } else if (role == 1) {
                img_suggess = img;
            } else if (role == 2) {
                img_target = img;
            } else if (role == 3) {
                img_start = img;
            } else {
                img_list.push_back(img);
            }
        }
    }
    std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
        return a->getComparisionValue("number") < b->getComparisionValue("number");
    });
    
    std::vector<math::resource::Image*> imgs;
    imgs.push_back(img_complete);
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, imgs);
    }
    
    if (getJsonBool(json_game, "suggess_front", false)) {
        suggess_index = 100;
    }
    
    //Gen object
    image_size = getSizeJson(json_game, "picture_size");
    image_center = cocos2d::Vec2(quiz_size.width *0.25, quiz_size.height * 0.475);
    float scale_value = 1;
    
    start_size = cocos2d::Size(quiz_size.width * 0.4, quiz_size.height*0.85);
    start_center = cocos2d::Vec2(quiz_size.width *0.75, quiz_size.height * 0.475);
    
    comp_object = mpool->getObject();
    comp_object->setEnable(0, math::common::deny, img_complete, nullptr);
    comp_object->setObjectSize(image_size);
    comp_object->fixWidthObject();
    image_size = comp_object->getContentSize();
    comp_object->setPosition(image_center);
    
    scale_value = image_size.width / comp_object->getImage()->getVirtualRendererSize().width;
    
    sugg_object = mpool->getObject();
    sugg_object->setEnable(1, math::common::deny, img_suggess, nullptr);
    sugg_object->setObjectSize(image_size);
    sugg_object->fixWidthObject();
    sugg_object->setPosition(image_center);
    
    /*---- PARSE DATA -----*/
    rapidjson::Document doc_suggess;
    doc_suggess.Parse<rapidjson::kParseStopWhenDoneFlag>(img_target->tracing.c_str());
    rapidjson::Value& piece_positions = getJsonMember(doc_suggess, "position");
    for (rapidjson::SizeType i = 0; i<piece_positions.Size(); i++) {
        rapidjson::Value& json_point = piece_positions[i];
        PiecePoint point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
    
    rapidjson::Value& families_ison = getJsonMember(doc_suggess, "paths");
    for (rapidjson::SizeType i = 0; i<families_ison.Size(); i++) {
        rapidjson::Value& json_path = families_ison[i];
        PieceFamily family;
        std::string point_list = getJsonString(json_path, "point");
        bool stop = true;
        do {
            auto a1 = point_list.find(",");
            auto sub = point_list.substr(0, a1);
            point_list = point_list.substr(a1+1);
            stop = (sub == point_list);
            
            math::func::trim(sub);
            family.points.push_back(std::atoi(sub.c_str()));
        } while (!stop);
        families.push_back(family);
    }
    
    /*---- PARSE DATA START POSITION -----*/
    rapidjson::Document doc_start;
    doc_start.Parse<rapidjson::kParseStopWhenDoneFlag>(img_start->tracing.c_str());
    rapidjson::Value& start_positions = getJsonMember(doc_start, "position");
    for (rapidjson::SizeType i = 0; i<start_positions.Size(); i++) {
        rapidjson::Value& json_point = start_positions[i];
        PiecePoint point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        start_points.push_back(point);
    }
    
    cocos2d::Size slot_size = cocos2d::Size(80, 80);
    for (int i=0; i<points.size(); i++) {
        int index = points[i].index;
        math::resource::Image* image = nullptr;
        for (math::resource::Image* img : img_list) {
            if (img->getComparisionValue("number") == index) {
                image = img;
            }
        }

        auto object = mpool->getObject();
        object->setEnable(index, math::common::deny, image, nullptr);
        auto object_size = object->getImage()->getVirtualRendererSize() * scale_value;
        object->setObjectSize(object_size);
        object->setTouchDetectedType(math::common::nontrans);
        object->setMoveCallback(CC_CALLBACK_1(MoveObjectMakePictureQuiz::onMoveObjectBegan, this), CC_CALLBACK_2(MoveObjectMakePictureQuiz::onMoveObjectEnded, this));
        object->setPosition(getStartLocation(index));
        _objects.push_back(object);

        auto slot = mpool->getSlot();
        slot->setEndble(index, nullptr);
        slot->setSlotSize(slot_size);
        slot->setPosition(getLocation(index));
        _slots.push_back(slot);
    }
}

void MoveObjectMakePictureQuiz::onStart() {
     
    quizBegan(0.5, true);
    
    auto line = cocos2d::ui::ImageView::create(getUIPath("common/line/line_1.png"));
    line->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height*0.5));
    addChildNode(line);
    ui_elements.push_back(line);
    
    addChildNode(sugg_object, suggess_index);
    sugg_object->onShow(0);

    math::func::shuffle(_objects);
    float delay = 0;
    for (Math_Object* obj : _objects) {
        addChildNode(obj, obj->getID());
        obj->onShow(delay+=0.1);
    }
    
    for (Math_Slot* slot : _slots) {
        addChildNode(slot, 0);
        slot->onShow(0);
    }
}

void MoveObjectMakePictureQuiz::startQuestion() {
    enableControlQuiz();
    
    for (Math_Object* obj : _objects) {
        obj->enableDrag(_panel);
    }
}

void MoveObjectMakePictureQuiz::onExplanation(bool correct) {
    disableControlQuiz();
    if (correct) {
        for (Math_Object* object : _objects) {
            object->onHide(0, math::animation::No_Effect);
        }
        sugg_object->onHide(0, math::animation::No_Effect);
        comp_object->setPosition(image_center);
        addChildNode(comp_object);
        comp_object->onShow(0, math::animation::No_Effect);
        
        scheduleOnce([=](float dt) {
            for (auto child : ui_elements) {
                child->runAction(cocos2d::FadeOut::create(0.25));
            }
            comp_object->runAction(cocos2d::Spawn::createWithTwoActions(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(quiz_size.width*0.5, image_center.y)), cocos2d::ScaleTo::create(0.5, 1.15)));
        }, 0.5, "go complete");
        
        this->nextQuestion(2.5);
    }
}

void MoveObjectMakePictureQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    comp_object->onHide(0);

    quizEnded(0.5, true);
}

#pragma mark - object touch events callback
void MoveObjectMakePictureQuiz::onMoveObjectBegan(cocos2d::Node *node)
{
    auto object = static_cast<Math_Object*>(node);
    //Chon object dau tien cham phai
    if (selected_object==NULL) {
        selected_object=object;
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void MoveObjectMakePictureQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position)
{
    //Danh cho game sap xep lai vi tri roi chon dap an
    auto object = static_cast<Math_Object*>(node);
    auto object_index = object->getID();
    
    bool addable = false;
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed() && slot->isAvailable(position)) {
            //In slot not use
            int slot_id = slot->getId();
            if (slot_id == object_index) {
                addable = true;
            } else {
                PieceFamily family;
                for (int i = 0; i<families.size(); i++) {
                    for (int index : families[i].points) {
                        if (index == object_index) {
                            family = families[i];
                            break;
                        }
                    }
                }
                for (int value : family.points) {
                    if (slot_id == value) {
                        addable = true;
                    }
                }
            }
            
            if (addable) {
                object->removeFromParentAndCleanup(false);
                addChildNode(object, object_index);
                slot->setLinker(object);
                object->runAction(cocos2d::MoveTo::create(0.2, slot->getPosition()));
                object->setTouch(math::common::deny);
                break;
            }
        }
    }
    
    if (!addable) {
        //Move ve vi tri ban dau
        object->backToStart();
    }
    selected_object=nullptr;

    bool move_done = true;
    for (Math_Slot * slot : _slots) {
        if (!slot->isUsed()) {
            move_done = false;
            break;
        }
    }
    
    if (move_done) {
        scheduleOnce([=](float){
            this->onExplanation(true);
        }, 1.0, "explain");
    }
}

cocos2d::Vec2 MoveObjectMakePictureQuiz::getLocation(int index) {
    PiecePoint point;
    for (PiecePoint mpoint : points) {
        if (mpoint.index == index) {
            point = mpoint;
            break;
        }
    }
    cocos2d::Vec2 origin = cocos2d::Vec2(image_center.x - image_size.width/2, image_center.y - image_size.height/2);
    float padding = 60;
    
    float fix_x = (0.5 - (float)point.x/1000.0) * padding + point.x;
    float fix_y = (0.5 - (float)point.y/1000.0) * padding + point.y;
    
    cocos2d::Vec2 point_in_object = cocos2d::Vec2(fix_x * image_size.width / 1000, fix_y * image_size.height / 1000);
    return  origin + point_in_object;
}

cocos2d::Vec2 MoveObjectMakePictureQuiz::getStartLocation(int index) {
    PiecePoint point;
    for (PiecePoint mpoint : start_points) {
        if (mpoint.index == index) {
            point = mpoint;
            break;
        }
    }
    cocos2d::Vec2 origin = cocos2d::Vec2(start_center.x - start_size.width/2, start_center.y - start_size.height/2);
    float padding = 60;
    
    float fix_x = (0.5 - (float)point.x/1000.0) * padding + point.x;
    float fix_y = (0.5 - (float)point.y/1000.0) * padding + point.y;
    
    cocos2d::Vec2 point_in_object = cocos2d::Vec2(fix_x * start_size.width / 1000, fix_y * start_size.height / 1000);
    return  origin + point_in_object;
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MoveObjectMakePictureQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MoveObjectMakePictureQuiz::autoTestingRun(float delaytime)
{
    
}

