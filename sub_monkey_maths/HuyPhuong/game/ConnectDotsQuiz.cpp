//
//  ConnectDotsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "ConnectDotsQuiz.h"

ConnectDotsQuiz* ConnectDotsQuiz::createQuiz()
{
    ConnectDotsQuiz* quiz=new ConnectDotsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ConnectDotsQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
#define detect_size  cocos2d::Size(100, 100)
#define line_size    8

void ConnectDotsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    
    connect_number = getJsonString(json_game, "type", "number") == "number";
    if (connect_number) {
        rapidjson::Value& json_number=json_game.FindMember("number")->value;
        
        number_skip = getJsonInt(json_number, "skip", 1);
        is_asc = getJsonString(json_number, "count_style", "ASC") == "ASC";

        rapidjson::Value& json_number_start=json_number.FindMember("start")->value;
        bool is_beaty = getJsonBool(json_number_start, "beauty_number", true);
        number_start = randomRangeJson(json_number_start, "range");
        if (is_beaty) {
            while (number_start % number_skip != 0) {
                 number_start = randomRangeJson(json_number_start, "range");
            }
        }
    } else {
        rapidjson::Value& json_character=json_game.FindMember("character")->value;

        is_asc = getJsonString(json_character, "count_style", "ASC") == "ASC";
        lower_case = getJsonBool(json_character, "lower_case", true);
        number_start = randomRangeJson(json_character, "start");
    }
    
    color_string = math::func::getRandomNode(getColors())->getText();
    
    std::vector<math::resource::Image*> image_list;
    auto all_images = getImageList("icons");
    math::resource::Image* image = math::func::getRandomNode(all_images);
    img_list.push_back(image);
    
    /*---- PARSE DATA -----*/
    rapidjson::Document doc_tracing;
    doc_tracing.Parse<rapidjson::kParseStopWhenDoneFlag>(img_list.front()->tracing.c_str());
    rapidjson::Value& trace_positions = getJsonMember(doc_tracing, "position");
    for (rapidjson::SizeType i = 0; i<trace_positions.Size(); i++) {
        rapidjson::Value& json_point = trace_positions[i];
        Dot point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
    std::sort(points.begin(), points.end(), [=](Dot a, Dot b)->bool{
        return a.index < b.index;
    });
    
    image_center = cocos2d::Vec2(quiz_size.width * 0.5, quiz_size.height * 0.5);
    image_size = cocos2d::Size(600, 600);
    
    //TODO: GENERATE QUIZ COMPONENTS
    template_object = mpool->getObject();
    template_object->setEnable(0, math::common::deny, img_list.front(), nullptr);
    template_object->setObjectSize(image_size);
    template_object->fixHeightObject();
    template_object->setPosition(image_center);
    image_size = template_object->getContentSize();
    template_object->setImageVisible(false);
    
    auto img_item = math::resource::ResHelper::createImage(getUIPath("gameui/connectdot/dot2.png"));
    auto img_item_start = math::resource::ResHelper::createImage(getUIPath("gameui/connectdot/star.png"));
    
    int i=0;
    for (Dot dot : points) {
        math::resource::Image* number_img= nullptr;
        if (connect_number) {
            int number = number_start + i * number_skip;
            number_img = math::number::getNumber(number);
        } else {
            int index = number_start + i;
            number_img = math::character::getCharacter(index, lower_case);
        }
        if (is_asc) {
            i++;
        } else {
            i--;
        }
        
        cocos2d::Vec2 position = getLocation(dot.index);
        Math_Object* obj_dot = mpool->getObject();
        obj_dot->setEnable(dot.index, math::common::touch, number_img, img_item);
        if (dot.index == 1) {
            obj_dot->setBackground(img_item_start);
        }
        obj_dot->setObjectSize(detect_size);
        obj_dot->setPosition(position);
        
        if (connect_number) {
            obj_dot->setValueTextByKey("name_1");
            obj_dot->showValueTextWithAnimation(false);
            obj_dot->setImageVisible(false);
            obj_dot->getLabelValue()->setFontSize(detect_size.height * 0.45);
            obj_dot->getLabelValue()->disableEffect();
            obj_dot->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
        }
        
        obj_dot->setTouchCallback(CC_CALLBACK_1(ConnectDotsQuiz::onTouchObject, this));
        
        addChildNode(obj_dot, 7);
        _objects.push_back(obj_dot);
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
}

void ConnectDotsQuiz::onStart() {
    Math_Quiz::onStart();
    image_center = cocos2d::Vec2(quiz_size.width * 0.5, (quiz_size.height - 20 - getQuestion()->getContentSize().height) * 0.5);
    
    scheduleUpdate();
    template_object->setPosition(image_center);
    addChildNode(template_object);
    template_object->onShow(0, math::animation::No_Effect);
    
    for (int i=0; i<_objects.size(); i++) {
        cocos2d::Vec2 position = getLocation(points[i].index);
        _objects[i]->setPosition(position);
        _objects[i]->onShow(i*0.1);
    }
    
    draw_parent = cocos2d::Node::create();
    draw_parent->setPosition(cocos2d::Vec2::ZERO);
    addChildNode(draw_parent, 5);
}

void ConnectDotsQuiz::startQuestion() {
    index_path=0;
    
    drawer = cocos2d::DrawNode::create();
    drawer->setLineWidth(line_size);
    //addChildNode(drawer, 5);
    draw_parent->addChild(drawer);
    drawer_list.push_back(drawer);
    
    scheduleOnce([=](float) {
        enableControlQuiz();
        this->activeObject(_objects.front());
    }, (int)_objects.size() * 0.1, "start connect");
}

void ConnectDotsQuiz::onExplanation(bool correct) {
    unscheduleUpdate();

    audio_helper->play_sequence(template_object->getImageSource()->getAudios(), nullptr, "");
    
    //animation done
    cocos2d::Vec2 out_point = cocos2d::Vec2(0, 1000);
    cocos2d::Vec2 in_point = cocos2d::Vec2(0, 0);

    draw_parent->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0), cocos2d::CallFunc::create([=](){
        for (cocos2d::DrawNode* draw : drawer_list) {
            draw->clear();
            draw->removeFromParentAndCleanup(true);
        }
    }), NULL));
    for (int i=0; i<_objects.size(); i++) {
        _objects[i]->onHide(1.0, math::animation::No_Effect);
    }
    
    template_object->setOpacity(0);
    template_object->setImageVisible(true);
    template_object->runAction(cocos2d::FadeIn::create(0.3));
    
    float skew_time = 0.15;
    float skew_delta = 7;
    scheduleOnce([=](float){
        auto action = cocos2d::Repeat::create(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::SkewTo::create(skew_time, skew_delta, 1), cocos2d::SkewTo::create(skew_time*2, -skew_delta, 1), cocos2d::SkewTo::create(skew_time, 1, 1), NULL), 3);
        template_object->runAction(cocos2d::Sequence::create(cocos2d::FadeTo::create(0.15, 100), cocos2d::FadeIn::create(0.15), cocos2d::FadeTo::create(0.15, 100), cocos2d::FadeIn::create(0.15), action, NULL));
    }, 0.5, "object animation");
    
    scheduleOnce([=](float dt){
        this->onComplete();
    }, 5.0, "compete quiz here");

}

void ConnectDotsQuiz::onComplete() {
     
    //TODO: Thuc hien them cac lenh o day
    template_object->onHide(0, math::animation::ScaleUpAndRotation);
    
    scheduleOnce([=](float) {
        Math_Quiz::onComplete();
    }, 1, "change complete img");
}

#pragma mark - object touch events callback
#define line_weight 10
void ConnectDotsQuiz::onTouchObject(cocos2d::Node* node)
{
    auto object = static_cast<Math_Object*>(node);
    
    bool done = false;
    if (object->isSame(_objects[index_path])) {
        done=activeObject(object);
    }
    
    if (done) {
        for (Math_Object* object : _objects) {
            object->setLocked(true);
        }
        
        if (auto_draw_last) {
            scheduleOnce([=](float) {
                complete = true;
                this->drawLine(item_list.back(), _objects.front());
            }, 1.0, "draw last line");
        }
    }
}

cocos2d::Vec2 ConnectDotsQuiz::getLocation(int point_index) {
    Dot point;
    for (Dot mpoint : points) {
        if (mpoint.index == point_index) {
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

bool ConnectDotsQuiz::activeObject(Math_Object* object) {
    //TODO: active here

    auto img_active = math::resource::ResHelper::createImage(getUIPath("gameui/connectdot/dot1.png"));
    object->setBackground(img_active);
    object->onTouchObject(true);
    
    index_path++;
    
    bool done = false;
    if (!item_list.empty()) {
        disableControlQuiz();
        if (object->isSame(_objects.back())) {
            done = true;
        }
        if (!auto_draw_last) {
            complete = done;
        }
        drawLine(item_list.back(), object);
    }
    item_list.push_back(object);
    return done;
}

void ConnectDotsQuiz::drawLine(Math_Object* from, Math_Object* to) {
    start_position = from->getPosition();
    target_position = to->getPosition();
    current_position = start_position;
    
    float length = target_position.distance(current_position);
    delta = (target_position-current_position)*15/length;
    
    drawing = true;
}

void ConnectDotsQuiz::update (float dt) {
    if (drawing) {
        drawer->clear();
        current_position = current_position+delta;
        drawer->drawSegment(start_position, current_position, line_weight, getColor4F(color_string));
        if (current_position.distance(target_position) < 15) {
            drawer->clear();
            drawer->drawSegment(start_position, target_position, line_weight, getColor4F(color_string));
            
            drawLineDone();
        }
    }
}

void ConnectDotsQuiz::drawLineDone() {
    drawer = cocos2d::DrawNode::create();
    drawer->setLineWidth(line_size);
    //addChildNode(drawer, 5);
    draw_parent->addChild(drawer);
    drawer_list.push_back(drawer);
    
    drawing = false;
    
    if (complete) {
        scheduleOnce([=](float){
            this->onExplanation(true);
        }, 2.0, "complete");
    } else {
        enableControlQuiz();
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ConnectDotsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        //this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}

