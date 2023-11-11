//
//  ColorPictureQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "ColorPictureQuiz.h"

ColorPictureQuiz* ColorPictureQuiz::createQuiz()
{
    ColorPictureQuiz* quiz=new ColorPictureQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ColorPictureQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void ColorPictureQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data = getJsonMember(document, "data");
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    color_one = getJsonBool (json_game, "color_one", true);
    
    rapidjson::Value& colors = getJsonMember(json_game, "color_list");
    for (rapidjson::SizeType i =0 ; i < colors.Size(); i++) {
        selected_color.push_back(colors[i].GetString());
    }
    int over_color = randomRangeJson(json_game, "over_color");
    
    rapidjson::Value& json_object = getJsonMember(json_data, "object");
    object_size = getSizeJson(json_object, "size");
    
    //Get Icons
    auto all_images = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
    auto img_selected = math::func::getRandomNode(all_images);
    int pair_selected = img_selected->getComparisionValue("pair");
    for (math::resource::Image* img : all_images) {
        int pair = img->getComparisionValue("pair");
        if (pair_selected == pair) {
            int role = img->getComparisionValue("role");
            if (role== 0) {
                img_background = img;
                img_background->used = true;
            } else if (role == 1)  {
                img_list.push_back(img);
                img->used = true;
            } else {
                auto color = img->getText("color");
                img_list_color.push_back(img);
                img->used = true;
                object_color_list.push_back(std::make_pair(img->getText(), color));
            }
        }
    }
    number_object = (int)img_list.size();
    math::func::shuffle(img_list);
    
    /*---- PARSE DATA -----*/
    rapidjson::Document doc_location;
    doc_location.Parse<rapidjson::kParseStopWhenDoneFlag>(img_background->tracing.c_str());
    rapidjson::Value& trace_positions = getJsonMember(doc_location, "position");
    for (rapidjson::SizeType i = 0; i<trace_positions.Size(); i++) {
        rapidjson::Value& json_point = trace_positions[i];
        PicturePoint point;
        point.index = getJsonInt(json_point, "order", 1);
        point.x = getJsonInt(json_point, "horizontal_axis", 1);
        point.y = getJsonInt(json_point, "vertical_axis", 1);
        points.push_back(point);
    }
    
    for (std::pair<std::string, std::string> pair : object_color_list) {
        auto color = pair.second;
        bool avail = true;
        for (std::string m_color : list_colors) {
            if (color == m_color) {
                avail = false;
                break;
            }
        }
        if (avail) {
            list_colors.push_back(color);
        }
    }
    
    if (over_color > 0) {
        int total_color = (int)list_colors.size() + over_color;
        math::func::shuffle(selected_color);
        for (std::string m_color : selected_color) {
            bool avail = true;
            for (std::string color : list_colors) {
                if (color == m_color) {
                    avail = false;
                    break;
                }
            }
            if (avail) {
                list_colors.push_back(m_color);
                if (list_colors.size() >= total_color) {
                    break;
                }
            }
        }
    }
    
    auto all_colors = getColors();
    for (std::string color : list_colors) {
        for (math::resource::Image* img : all_colors) {
            auto m_color = img->getText();
            if (color == m_color) {
                img_color.push_back(img);
                break;
            }
        }
    }
    
    //Gen objects
    cocos2d::Vec2 position = cocos2d::Vec2(quiz_size.width * 0.375, quiz_size.height * 0.5);
    auto obj_background = mpool->getObject();
    obj_background->setEnable(0, math::common::deny, img_background, nullptr);
    obj_background->setObjectSize(object_size);
    obj_background->setPosition(position);
    addChildNode(obj_background);
    _objects.push_back(obj_background);
    
    for (int i=0; i<img_list.size(); i++) {
        auto object = mpool->getObject();
        object->setEnable(i+1, math::common::touch, img_list[i], nullptr);
        object->setObjectSize(object_size);
        object->setColoring(true);
        object->setTouchDetectedType(math::common::nontrans);
        object->setTouchCallback(CC_CALLBACK_1(ColorPictureQuiz::onTouchedObject, this));
        object->setPosition(position);
        addChildNode(object, 3);
        _objects.push_back(object);
    }
    
    //Parse Question
    if (color_one) {
        parseQuestion(true, number_object);
    } else {
        parseQuestion();
    }
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "color") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto name = img->getText();
                auto color = list_colors.front();
                for (std::pair<std::string, std::string> pair : object_color_list) {
                    if (name == pair.first) {
                        color = pair.second;
                        break;
                    }
                }
                for (math::resource::Image* m_color : img_color) {
                    if (m_color->getText() == color) {
                        texts.push_back(m_color->getTextValue("name_1"));
                        break;
                    }
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
}

void ColorPictureQuiz::onStart() {
    startGuide();
    //Gen color object
    showColorsPad(list_colors, [=](cocos2d::Node* node){
        Math_Object* m_object = (Math_Object*)node;
        current_color = m_object->getComparisionValue().color;
    }, nullptr);
    quizBegan(0, false);
    
    //Fix object size and position
    auto img_size = _objects.front()->getImage()->getVirtualRendererSize();
    auto view_size = cocos2d::Size(quiz_size.width*0.7, quiz_size.height*0.9);
    float scalar_x = view_size.width / img_size.width;
    float scalar_y = view_size.height / img_size.height;
    float scalar = scalar_x < scalar_y ? scalar_x : scalar_y;
    
    picture_size = cocos2d::Size(img_size.width * scalar, img_size.height * scalar);
    picture_center = cocos2d::Vec2(quiz_size.width * 0.375, quiz_size.height * 0.5);
    
    for (Math_Object* object : _objects) {
        object->setPosition(picture_center);
        object->setObjectSize(picture_size);
    }
    
    //show
    for (int i=0; i<_objects.size(); i++) {
        _objects[i]->onShow(0, math::animation::No_Effect);
    }
   
    current_color = "";
    colored_object = 0;
}

void ColorPictureQuiz::startQuestion() {
    enableControlQuiz();
    if (color_one) {
        ask_object = img_list[questionIndex()]->getText();
        for (std::pair<std::string, std::string> pair : object_color_list) {
            if (ask_object == pair.first) {
                ask_color = pair.second;
                break;
            }
        }
    }
}

void ColorPictureQuiz::onExplanation(bool correct) {
    disableControlQuiz();
    this->nextQuestion(1.0);
}

void ColorPictureQuiz::onComplete() {
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide(0.5, math::animation::No_Effect);
    }
    quizEnded(0.75, false);
}

void ColorPictureQuiz::onGuideStarted() {
    disableControlQuiz();
    if (color_one) {
        //get pen
        Math_Object* pen = nullptr;
        for (Math_Object* _pen : _color_pens) {
            if (_pen->getComparisionValue().color == ask_color) {
                pen = _pen;
                break;
            }
        }

        //get object
        Math_Object* object = nullptr;
        for (Math_Object* _object : _objects) {
            if (_object->getComparisionValue().name == ask_object) {
                object = _object;
                break;
            }
        }
        
        GuideCenter->guideByColor(nullptr, cocos2d::Vec2(quiz_size.width-75, pen->getWorldPosition().y), getLocation(object->getComparisionValue().number));
    } else {
        //get object
        Math_Object* object = nullptr;
        for (Math_Object* _object : _objects) {
            if (_object->isColoring()) {
                object = _object;
                break;
            }
        }
        
        auto object_name = object->getComparisionValue().name;
        std::string color_name = "";
        for (std::pair<std::string, std::string> pair : object_color_list) {
            if (object_name == pair.first) {
                color_name = pair.second;
                break;
            }
        }
        
        //get pen
        Math_Object* pen = nullptr;
        for (Math_Object* _pen : _color_pens) {
            if (_pen->getComparisionValue().color == color_name) {
                pen = _pen;
                break;
            }
        }
        
        GuideCenter->guideByColor(nullptr, cocos2d::Vec2(quiz_size.width-75, pen->getWorldPosition().y), getLocation(object->getComparisionValue().number));
    }
}

void ColorPictureQuiz::onGuideDone() {
    enableControlQuiz();
}

#pragma mark - object touch events callback
void ColorPictureQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = (Math_Object*) node;
    if (!object->isColoring()) {
        return;
    }
   
    if (current_color=="") {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        return;
    }
    
    auto name = object->getComparisionValue().name;
    if (color_one) {
        if (current_color != ask_color) {
            disableControlQuiz(0.25);
            GuideCenter->increamentIncorrect();
            object->onTouchObject(false);
            return;
        }
        if (name != ask_object) {
            disableControlQuiz(0.25);
            GuideCenter->increamentIncorrect();
            object->onTouchObject(false);
            return;
        }
    }

    std::string color = "";
    for (std::pair<std::string, std::string> pair : object_color_list) {
        if (name == pair.first) {
            color = pair.second;
            break;
        }
    }
    
    if (color != current_color) {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        return;
    }
    
    //get img color
    math::resource::Image* img_color_find = nullptr;
    for (math::resource::Image* img : img_list_color) {
        if (img->getText() == name && img->getText("color") == color) {
            img_color_find = img;
            break;
        }
    }
    
    if (img_color_find != nullptr) {
        object->setImage(img_color_find);
        object->setComparisionValue("color", color);
        object->setColoring(false);
        colored_object ++;
    }
    
    std::string gaf = "click1";
    auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
    auto anim = gaf_asset->createObjectAndRun(false);
    anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    auto position_gaf = getLocation(object->getComparisionValue().number);
    anim->setPosition(position_gaf);
    object->getParent()->addChild(anim, 20);
    scheduleOnce([=](float){
        anim->removeFromParentAndCleanup(true);
    }, 0.5, "remove anim");
    
    if (cocos2d::random(0, 1)==1) {
        audio_helper->play("colorusekey/click5.mp3");
    } else {
        audio_helper->play_sequence(new math::SequenceAudio(), "colorusekey/click4.mp3", "heyyy.mp3");
    }
    GuideCenter->resetIncorrectCount();
    
    if (color_one) {
        onExplanation(true);
    } else {
        if (colored_object == number_object) {
            onExplanation(true);
        }
    }
}

cocos2d::Vec2 ColorPictureQuiz::getLocation(int point_index) {
    PicturePoint point;
    for (PicturePoint mpoint : points) {
        if (mpoint.index == point_index) {
            point = mpoint;
            break;
        }
    }
    float padding = 60;
    float fix_x = (0.5 - (float)point.x/1000.0) * padding + point.x;
    float fix_y = (0.5 - (float)point.y/1000.0) * padding + point.y;
    
    cocos2d::Vec2 origin = cocos2d::Vec2(picture_center.x - picture_size.width/2, picture_center.y - picture_size.height/2);
    cocos2d::Vec2 point_in_object = cocos2d::Vec2(fix_x * picture_size.width / 1000, fix_y * picture_size.height / 1000);
    return  origin + point_in_object;
}

void ColorPictureQuiz::onAnswerDone(cocos2d::Node *node) {
    
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ColorPictureQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ColorPictureQuiz::autoTestingRun(float delaytime)
{
    
    
}

