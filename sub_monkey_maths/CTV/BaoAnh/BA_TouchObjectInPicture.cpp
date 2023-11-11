//
//  BA_TouchObjectInPicture.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 3/8/18.
//

#include "BA_TouchObjectInPicture.hpp"

BA_TouchObjectInPicture* BA_TouchObjectInPicture::createQuiz()
{
    BA_TouchObjectInPicture* quiz=new BA_TouchObjectInPicture();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_TouchObjectInPicture::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void BA_TouchObjectInPicture::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    number_question = getJsonInt(getJsonMember(json_data, "game_mode"), "number", 2);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    int number_kind = getJsonInt(json_object, "number_kind", 3);
    json_object.FindMember("number_kind")->value.GetInt();
    object_size=cocos2d::Size(json_object.FindMember("size")->value.FindMember("width")->value.GetInt(), json_object.FindMember("size")->value.FindMember("heigth")->value.GetInt());


    int number_object = number_kind*2;
    
    auto all_imgs = getImageList("icons");
    int mpair = math::func::getRandomNode(all_imgs)->getComparisionValue("pair");
    
    math::resource::Image* image;
    std::vector<math::resource::Image*> bg_obj_list;
    
    for (math::resource::Image* img : all_imgs) {
        int role = img->getComparisionValue("role");
        int pair = img->getComparisionValue("pair");
        if (pair == mpair) {
            if (role == 0) {
                image =img;
            } else {
                icon_obj_list.push_back(img);
            }
        }
    }
    
    std::sort(icon_obj_list.begin(), icon_obj_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
        return a->getComparisionValue("role") < b->getComparisionValue("role");
    });
    
    while (icon_obj_list.size() > number_object) {
        icon_obj_list.pop_back();
    }
    
//   img_list.push_back(image);
    /*---- PARSE DATA -----*/
    rapidjson::Document doc_tracing;
    doc_tracing.Parse<rapidjson::kParseStopWhenDoneFlag>(image->tracing.c_str());
    rapidjson::Value& trace_positions = getJsonMember(doc_tracing, "position");
    for (rapidjson::SizeType i = 0; i<trace_positions.Size(); i++) {
        rapidjson::Value& json_point = trace_positions[i];
        Dot point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
//    std::sort(points.begin(), points.end(), [=](Dot a, Dot b)->bool{
//        return a.index < b.index;
//    });
    
    image_center = cocos2d::Vec2(quiz_size.width * 0.5, quiz_size.height * 0.5);
    image_size = cocos2d::Size(600, 600);
    
    //----------------------------------
    //Gen objects
    //TODO: GENERATE QUIZ COMPONENTS
    template_object = mpool->getObject();
    template_object->setEnable(0, math::common::deny, image, nullptr);
    template_object->setObjectSize(image_size);
    template_object->fixHeightObject();
    template_object->setPosition(image_center);
    addChildNode(template_object);
    image_size = template_object->getContentSize();
    
    for (Dot dot : points) {
        cocos2d::Vec2 position = getLocation(dot.index);
        math::resource::Image* number_img= icon_obj_list.at(dot.index-1);
        Math_Object* obj_dot = mpool->getObject();
        obj_dot->setEnable(dot.index, math::common::touch, number_img, nullptr);
        obj_dot->setObjectSize(object_size);
        obj_dot->setPosition(position);

        obj_dot->setTouchCallback(CC_CALLBACK_1(BA_TouchObjectInPicture::onTouchedObject, this));

        addChildNode(obj_dot, 7);
        _objects.push_back(obj_dot);
    }
    
    for (int i=0; i<number_object; i++) {
        bg_obj_list.push_back(image);
    }
    math::func::shuffle(icon_obj_list);
    
    if (number_question > number_object) {
        number_question = number_object;
    }
    //PARSING QUESTION
    parseQuestion(true, number_question);
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if(key == "name_n"){
            getQuestion()->setOptionalTexts(key, bg_obj_list);

        }else{
            getQuestion()->setOptionalTexts(key, icon_obj_list);
        }
    }
}

void BA_TouchObjectInPicture::onStart() {
    
    //---------------------------------
    //Gen question
//    float bar_h =220.0/quiz_size.height;
//    onShowBottomBar(0, bar_h, true, CC_CALLBACK_1(BA_TouchObjectInPicture::onAnswerDone, this));
    showQuestionBox(0) ;
    
    image_center = cocos2d::Vec2(quiz_size.width * 0.5, (quiz_size.height - 20 - getQuestion()->getContentSize().height) * 0.5);
    
    scheduleUpdate();
    template_object->setPosition(image_center);
    addChildNode(template_object);
    template_object->onShow(0, math::animation::No_Effect);
    
    for (int i=0; i<_objects.size(); i++) {
        _objects[i]->onShow(i*0.1);
        CCLOG("index: %i", _objects.at(i)->getID());
    }
    
}

//--------------------------------------
//start Question
void BA_TouchObjectInPicture::startQuestion() {
    //Bat dau cau hoi
    enableControlQuiz();
    q_id = getQuestion()->getData().index;
}

void BA_TouchObjectInPicture::onExplanation(bool correct) {
    
    if (!correct){
        this->enableControlQuiz();
//        touched_obj->onTouched();
    }
    else {
        this->nextQuestion();
    }
}

void BA_TouchObjectInPicture::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    // //Math_Pool::getInstance()->resetAnswers();
//    for (Math_Answer* node : _answers) {
//        if (node->isEnable()) {
//            node->setDisable();
//        }
//    }
    unscheduleUpdate();
    float delay = showConratulation(0.3);
    
    for(Math_Object* obj: _objects){
        obj->onHide(delay);
        delay+= 0.1;
    }
    
    template_object->onHide(delay, math::animation::ScaleUpAndRotation);
    
    scheduleOnce([=](float) {
        Math_Quiz::onComplete();
    }, 1, "change complete img");
//    template_object->onHide(delay);
//
//    scheduleOnce([=](float dt) {
//        hideQuestionBox();
//    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void BA_TouchObjectInPicture::onTouchedObject(cocos2d::Node *node){
    
    touched_obj = (Math_Object*) node;
    int index = touched_obj->getID();
    int correct_index = icon_obj_list[q_id]->getComparisionValue("role");
    bool done = index == correct_index;;
    touched_obj->onTouchObject(done);
    onExplanation(done);
}

cocos2d::Vec2 BA_TouchObjectInPicture::getLocation(int point_index) {
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

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void BA_TouchObjectInPicture::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_TouchObjectInPicture::autoTestingRun(float delaytime)
{
    Math_Answer* answer=math::func::getRandomNode(_answers);
    answer->onAutoClick();
    autoTesting();
}

