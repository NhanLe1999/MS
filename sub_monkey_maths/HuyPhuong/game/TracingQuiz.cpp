//
//  TracingQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "TracingQuiz.h"

TracingQuiz* TracingQuiz::createQuiz()
{
    TracingQuiz* quiz=new TracingQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool TracingQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
#define detect_size cocos2d::Size(100, 100)
void TracingQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    std::string choose_icon = getJsonString(json_data, "icon", "random");
    std::string choose_icon_name = getJsonString(json_data, "icon_choose");
    tracing_size = getSizeJson(json_data, "size");
    
    std::vector<std::string> colors;
    colors.push_back("red");
    colors.push_back("pink");
    colors.push_back("purple");
    colors.push_back("orange");
    color_tracing = math::func::getRandomNode(colors);
    
    //TODO: GAME CONFIG
    std::vector<math::resource::Image*> image_list;
    auto all_images = getImageList("icons");
    int pair_value = -1;
    
    if (choose_icon == "name") {
        for (math::resource::Image* img : all_images) {
            if (img->getText() == choose_icon_name) {
                pair_value = img->getComparisionValue("pair");
                break;
            }
        }
    }
    if (pair_value == -1) {
        pair_value = math::func::getRandomNode(all_images)->getComparisionValue("pair");
    }
    for (math::resource::Image* img : all_images) {
        if (img->getComparisionValue("pair") == pair_value) {
            int role = img->getComparisionValue("role");
            if (role == -1) {
                img_item = img;
            } else if (role == 0) {
                img_tracing = img;
            } else {
                image_list.push_back(img);
            }
        }
    }
    
    std::sort(image_list.begin(), image_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
        return a->getComparisionValue("role") < b->getComparisionValue("role");
    });
    
    std::vector<math::resource::Image*> imgs;
    imgs.push_back(img_tracing);
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, imgs);
    }
    
    /*---- PARSE DATA -----*/
    rapidjson::Document doc_tracing;
    doc_tracing.Parse<rapidjson::kParseStopWhenDoneFlag>(img_tracing->tracing.c_str());
    rapidjson::Value& trace_positions = getJsonMember(doc_tracing, "position");
    for (rapidjson::SizeType i = 0; i<trace_positions.Size(); i++) {
        rapidjson::Value& json_point = trace_positions[i];
        TracingPoint point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
    
    rapidjson::Value& trace_paths = getJsonMember(doc_tracing, "paths");
    for (rapidjson::SizeType i = 0; i<trace_paths.Size(); i++) {
        rapidjson::Value& json_path = trace_paths[i];
        TracingPath path;
        path.index = getJsonInt(json_path, "path_number", 1);
        std::string point_list = getJsonString(json_path, "point");
        
        bool stop = true;
        do {
            auto a1 = point_list.find(",");
            auto sub = point_list.substr(0, a1);
            point_list = point_list.substr(a1+1);
            stop = (sub == point_list);
            
            math::func::trim(sub);
            path.points.push_back(std::atoi(sub.c_str()));
            
        } while (!stop);
        paths.push_back(path);
    }
    
    std::sort(points.begin(), points.end(), [=](TracingPoint a, TracingPoint b)->bool{
        return a.index < b.index;
    });
    std::sort(paths.begin(), paths.end(), [=](TracingPath a, TracingPath b)->bool{
        return a.index < b.index;
    });
    
    //Sinh object
    setBackGroundQuiz("gameui/tracing/bg.jpg");
    img_item = math::resource::ResHelper::createImage(getUIPath("gameui/tracing/dot.png"));
    
    /*---- GEN OBJECT -----*/
    tracing_center = quiz_size/2;
    
    game_object = mpool->getObject();
    game_object->setEnable(0, math::common::deny, img_tracing, nullptr);
    game_object->setObjectSize(tracing_size);
    game_object->setPosition(tracing_center);
    game_object->setImageColor(cocos2d::Color3B(102, 51, 0));
    addChildNode(game_object);
    
    for (int i=0; i<paths.size(); i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::deny, image_list[i], nullptr);
        object->setTouchDetectedType(math::common::nontrans);
        object->setObjectSize(tracing_size);
        object->setPosition(tracing_center);
        object->setImageVisible(false);
        addChildNode(object);
        path_objects.push_back(object);
    }
    
    for (int i=0; i<points.size(); i++) {
        auto object = mpool->getObject();
        object->setEnable(points[i].index, math::common::deny, img_item, nullptr);
        object->setObjectSize(detect_size);
        object->setImageVisible(false);
        cocos2d::Vec2 location = getLocation(points[i].index);
        object->setPosition(location);
        addChildNode(object);
        point_objects.push_back(object);
    }
}

void TracingQuiz::onStart() {
    startGuide();

    quizBegan(0, false);
    audio_helper->play("magic_dust.mp3");
    game_object->onShow(0, math::animation::SlideUp);
    for (Math_Object* object : path_objects) {
        object->onShow(0);
    }
    addEvents();
}

void TracingQuiz::startQuestion() {
    enableControlQuiz();
    startTracing(1);
}

void TracingQuiz::onExplanation(bool correct) {
    disableControlQuiz();
    this->nextQuestion(2.25);
}

void TracingQuiz::onComplete() {
    game_object->onHide(0, math::animation::ScaleUpAndRotation);
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    quizEnded(0.5, false);
}

void TracingQuiz::completeTrace() {
    for (Math_Object* object : point_objects) {
        object->onHide(0);
    }
    for (Math_Object* object : path_objects) {
        object->onHide(0);
    }
    scheduleOnce([=](float){
        for (Node* gaf : gaf_list) {
			if (gaf)
			{
				gaf->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=]() {
					gaf->removeFromParentAndCleanup(true);
				}), NULL));
			}

            game_object->setImageColor(this->getColor3B(color_tracing));
        }
        audio_helper->play("star_dust.mp3");
    }, 1.0, "remove gaf");
    
    scheduleOnce([=](float){
        auto audios = img_tracing->getAudios();
        audio_helper->play_sequence(audios, nullptr, "");
    }, 1.25, "play audio name");
    this->onExplanation(true);
}

void TracingQuiz::onGuideStarted() {
    disableControlQuiz();
    std::vector<Math_Object*> not_actives;
    for (Math_Object* object : item_list) {
        if (!object->isColoring()) {
            not_actives.push_back(object);
        }
    }
    
    if (not_actives.empty()) {
        GuideCenter->guideNone();
    } else {
        if (not_actives.size() == 1) {
            GuideCenter->guideByTouch(not_actives.front()->getWorldPosition());
        } else {
            std::vector<cocos2d::Vec2> vec2points;
            for (Math_Object* object : not_actives) {
                vec2points.push_back(object->getWorldPosition());
            }
            GuideCenter->guideBySwipe(vec2points);
        }
    }
}

void TracingQuiz::onGuideDone() {
    enableControlQuiz();
}


#pragma mark - object touch events callback
bool TracingQuiz::isInPath(cocos2d::Vec2 point) {
    cocos2d::Vec2 position = path_object->convertToNodeSpace(point);
    cocos2d::Rect rect = path_object->getBoundingBox();
    rect.origin = cocos2d::Vec2::ZERO;
    
    if (rect.containsPoint(position)) {
        if (path_object->isDetected(position)) {
            return true;
        }
    }
    return false;
}

void TracingQuiz::doCheckPath() {
    if (isTraceDone()) {
        //Tracing complete
        index_path++;
        if (index_path < paths.size()) {
            item_list.clear();
            startTracing(1.5);
        } else {
            completeTrace();
        }
    } else {
        //Tracing not complete
        startTracing();
    }
}

void TracingQuiz::startTracing(float dt) {
    path_object = path_objects[index_path];
    auto points = paths[index_path].points;
    
    float delay = 0;
    if (item_list.empty()) {
        disableControlQuiz(0.5+0.1*(int)points.size());
        audio_helper->play("corlorusekey/pop_out_3.mp3");
        
        for (int i : points) {
            for (Math_Object* item : point_objects) {
                if (item->getID() == i) {
                    item->setImageVisible(true);
                    item_list.push_back(item);
                    item->onShow(delay);
                    delay+=0.1;
                    break;
                }
            }
        }
    }
    
    GuideCenter->setGuideDisabled();
    scheduleOnce([=](float){
        GuideCenter->setGuideEnabled();
    }, dt + delay, "start tracing now");
}

Math_Object* TracingQuiz::nextItem() {
    if (item_list.empty()) {
        return nullptr;
    }
    for (Math_Object* item : item_list) {
        if (!item->isColoring()) {
            return item;
        }
    }
    return nullptr;
}

void TracingQuiz::activeItem(Math_Object* object) {
    if (object == NULL) {
        return;
    }
    if (object->isColoring()) {
        return;
    }
    object->setColoring(true);
    object->onTouchObject(true, 0, 0);
    GuideCenter->resetIncorrectCount();

    //show flowers
    int flower = cocos2d::random(1, 7);
    auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgameui/tracing/tracing%d/tracing%d.gaf", m_theme->getThemePath().c_str(), flower, flower));
    auto anim = gaf_asset->createObjectAndRun(false);
    anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    anim->setPosition(object->getPosition());
    anim->setSequenceDelegate([=](gaf::GAFObject* object, const std::string& sequenceName) {
        if (sequenceName == "grow") {
            object->playSequence("idle", true);
        }
    });
    
    object->getParent()->addChild(anim, 20);
    gaf_list.push_back(anim);
}

cocos2d::Vec2 TracingQuiz::getLocation(int point_index) {
    TracingPoint point;
    for (TracingPoint mpoint : points) {
        if (mpoint.index == point_index) {
            point = mpoint;
            break;
        }
    }
    cocos2d::Vec2 origin = cocos2d::Vec2(tracing_center.x - tracing_size.width/2, tracing_center.y - tracing_size.height/2);
    float padding = 60;
    
    float fix_x = (0.5 - (float)point.x/1000.0) * padding + point.x;
    float fix_y = (0.5 - (float)point.y/1000.0) * padding + point.y;

    cocos2d::Vec2 point_in_object = cocos2d::Vec2(fix_x * tracing_size.width / 1000, fix_y * tracing_size.height / 1000);
    return  origin + point_in_object;
}

void TracingQuiz::GAFSequenceCallback(gaf::GAFObject *object, const std::string &sequenceName) {
    if (sequenceName == "grow") {
        object->playSequence("idle", true);
    }
}

void TracingQuiz::addEvents()
{
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(TracingQuiz::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(TracingQuiz::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(TracingQuiz::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(TracingQuiz::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
}

Math_Object* TracingQuiz::touchOnObject(cocos2d::Vec2 point) {
    for (Math_Object* object : item_list) {
        if (!object->isColoring() && object->isTouchedAt(point)) {
            return object;
        }
    }
    return nullptr;
}

bool TracingQuiz::isTraceDone() {
    for (Math_Object* object : item_list) {
        if (!object->isColoring()) {
            return false;
        }
    }
    return true;
}

bool TracingQuiz::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event){
    if (!isInPath(touch->getLocation())) {
        GuideCenter->increamentIncorrect();
        return false;
    }
    moveable = true;
    auto t_object = touchOnObject(touch->getLocation());
    if (t_object) {
        if (t_object != nextItem()) {
            moveable = false;
            GuideCenter->increamentIncorrect();
        } else {
            activeItem(t_object);
            //check trace done
            if (isTraceDone()) {
                moveable = false;
                doCheckPath();
            }
        }
    }
    return moveable;
}

void TracingQuiz::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
    if (moveable) {
        if (!isInPath(touch->getLocation())) {
            GuideCenter->increamentIncorrect();
            moveable = false;
        } else {
            auto t_object = touchOnObject(touch->getLocation());
            if (t_object) {
                if (t_object != nextItem()) {
                    GuideCenter->increamentIncorrect();
                    moveable = false;
                } else {
                    activeItem(t_object);
                    //check trace done
                    if (isTraceDone()) {
                        moveable = false;
                    }
                }
            }
        }
    }
}

void TracingQuiz::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    doCheckPath();
}

void TracingQuiz::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    doCheckPath();
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void TracingQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        //this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", 1));
}

