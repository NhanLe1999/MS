//
//  Nam_ColorObject
//  MJQuiz
//
//  Created by HuyPhuong on 13/9/17.
//
//

#include "Nam_ColorObject.h"

Nam_ColorObject* Nam_ColorObject::createQuiz()
{
    Nam_ColorObject* quiz=new Nam_ColorObject();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_ColorObject::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_ColorObject::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    pickColor = json_game.FindMember("pick_color")->value.GetBool();
    typeCompare = json_game.FindMember("type_compare")->value.GetString();
    keyCompare = json_game.FindMember("key_compare")->value.GetString();
    int number_request = 1;
    if (typeCompare == "Ordinal") {
        number_request = randomRangeJson(json_game, "number_request");
    }
    objectSize = cocos2d::Size(200, 200);
    if (json_game.HasMember("object_size")) {
        objectSize = getSizeJson(json_game, "object_size");
    }
    
    keyGame = typeCompare;
    if (keyGame == "BigOrSmall") {
        if (cocos2d::random(0, 1)==0) {
            keyGame = "Small";
        } else {
            keyGame = "Big";
        }
    }
    
    if (keyCompare == "number") {
        compare_key = math::comparision::number;
    } else if (keyCompare == "size") {
        compare_key = math::comparision::size;
    } else {
        compare_key = math::comparision::exactly;
    }
    
    typeNumber = randomRangeJson(json_game, "number_kind");
    colorNumber = typeNumber + randomRangeJson(json_game, "color");
    
    int number_object_min = json_game.FindMember("number_object")->value.FindMember("minimum")->value.GetInt();
    int number_object_max = json_game.FindMember("number_object")->value.FindMember("maximum")->value.GetInt();
    int number_value_min = json_game.FindMember("number_value")->value.FindMember("minimum")->value.GetInt();
    int number_value_max = json_game.FindMember("number_value")->value.FindMember("maximum")->value.GetInt();
    
    //TODO: ICONS
    rapidjson::Value& json_icon = json_data.FindMember("icon")->value;
    std::string icon_same = getJsonString(json_icon, "same", "none");
    std::string icon_different = getJsonString(json_icon, "different", "id");
    
    auto allImages = getImageList("icons");
    
    std::vector <math::resource::Image*> all_color_imgs;

    for (auto img : allImages){
        if (img->getText("color") == "white"){
            white_imgs.push_back(img);
        } else {
            all_color_imgs.push_back(img);
        }
    }
    math::func::shuffle(white_imgs);
    while (white_imgs.size() > typeNumber) {
        white_imgs.pop_back();
    }
    if (white_imgs.size() > typeNumber) {
        typeNumber = static_cast<int>(white_imgs.size());
    }
    
    // Gen color
    math::func::shuffle(all_color_imgs);
    if (pickColor) {
        std::string color = "";
        for (auto img : all_color_imgs) {
            if (img->getText() == white_imgs.front()->getText()) {
                color = img->getText("color");
                colors.push_back(color);
                if (colors.size() >= colorNumber) {
                    break;
                }
            }
        }
        math::func::shuffle(colors);
    } else {
        std::string color = "";
        for (auto img : all_color_imgs) {
            if (img->getText() == white_imgs.front()->getText()) {
                color = img->getText("color");
                break;
            }
        }
        colors.push_back(color);
        for (auto img1 : white_imgs) {
            for (auto img2 : all_color_imgs) {
                if (img2->getText() == img1->getText() && img2->getText("color") == color) {
                    color_imgs.push_back(img2);
                    break;
                }
            }
        }
    }
    
    //Number list
    std::vector<int> numbers_list;
    for (int i=0; i<typeNumber; i++) {
        if (keyGame == "Same") {
            if (i==0) {
                numbers_list.push_back(cocos2d::random(number_object_min, number_object_max));
            } else {
                numbers_list.push_back(1);
            }
        } else if (keyGame == "Different") {
            if (i==0) {
                numbers_list.push_back(1);
            } else {
                numbers_list.push_back(cocos2d::random(number_object_min, number_object_max));
            }
        } else {
            numbers_list.push_back(cocos2d::random(number_object_min, number_object_max));
        }
    }
    
    std::vector<int> numbers;
    if (compare_key == math::comparision::number) {
        int total_icon = 0;
        for (int i1 = 0; i1 < typeNumber; i1++) {
            total_icon += numbers_list[i1];
        }
        if (keyGame == "Ordinal") {
            for (int i1 = 0; i1 < total_icon; i1++) {
                ordinal_imgs.push_back(math::ordinal::getOrdinal(i1+1));
            }
            first_img = ordinal_imgs.front();
            math::func::shuffle(ordinal_imgs);
        } else {
            if (keyGame == "Same") {
                sameNumber = cocos2d::random(2, total_icon-1);
                numbers = randomArrayIndexes(total_icon - sameNumber + 1, number_value_max - number_value_min);
                while (numbers.size() < total_icon) {
                    numbers.push_back(numbers.back());
                }
            } else if (keyGame == "Different") {
                sameNumber = total_icon - 1;
                numbers = randomArrayIndexes(2, number_value_max - number_value_min);
                while (numbers.size() < total_icon) {
                    numbers.push_back(numbers.back());
                }
            } else {
                numbers = randomArrayIndexes(total_icon, number_value_max - number_value_min);
            }
            math::func::shuffle(numbers);
            for (int num : numbers) {
                number_imgs.push_back(math::number::getNumber(num + number_value_min));
            }
        }
    }
    
    std::vector<int> sizes_value;
    if (compare_key == math::comparision::size) {
        numbers = randomArrayIndexes(numbers_list[0], numbers_list[0]);
        math::func::shuffle(numbers);
        for (auto val : numbers){
            sizes_value.push_back(val);
        }
    }
    
    
    //TODO: QUESTION CONFIG
    for (std::string color : colors) {
        imagesColor.push_back(getColor(color));
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "color") {
            std::vector<math::resource::Text> texts;
            for (auto img : imagesColor) {
                math::resource::Text text = img->getTextValue("color");
                texts.push_back(text);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "ordinal") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : ordinal_imgs) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, white_imgs);
        }
    }
    getQuestion()->setKeywordValue(keyGame == "Big"?1:0);
    
    //TODO: GENERATE ALL COMPONENTS
    int index=0;
    for (int i1 = 0; i1 < typeNumber; i1++) {
        for (int i2 = 0; i2 < numbers_list[i1]; i2++) {
            Math_Object* obj = mpool->getObject();
            obj->setEnable(index, math::common::TouchPermission::touch, white_imgs[i1], nullptr);
            obj->setTouchCallback(CC_CALLBACK_1(Nam_ColorObject::onTouchedObject, this));
            obj->setObjectSize(objectSize);
            obj->setColoring(true);
            obj->getLabelValue()->enableOutline(cocos2d::Color4B::WHITE, 5);
            obj->getLabelValue()->setFontSize(objectSize.height * 0.4);
            obj->setValueTextPosition("bottom");
            
            if (compare_key == math::comparision::number) {
                if (keyGame == "Ordinal") {
                    obj->setComparisionValue("number", ordinal_imgs[index]->getComparisionValue("number"));
                    if (index==0) {
                        obj->setValueText(ordinal_imgs[index]->getText("name_n"));
                        obj->showValueTextWithAnimation(false);
                    }
                } else {
                    obj->setComparisionValue("number", number_imgs[index]->getComparisionValue("number"));
                    obj->setValueText(number_imgs[index]->getText());
                    obj->showValueTextWithAnimation(false);
                }
            }
            if (compare_key == math::comparision::size) {
                obj->setComparisionValue(sizes_value[index]);
            }
            _objects.push_back(obj);
            index++;
        }
    }
}

void Nam_ColorObject::onStart() {
    if (keyGame != "Ordinal") {
        math::func::shuffle(_objects);
    }
    if (pickColor) {
        math::func::shuffle(colors);
        showColorsPad(colors, CC_CALLBACK_1(Nam_ColorObject::onTouchedColor, this), nullptr, 0, false);
        math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height * 0.35, quiz_size.width, quiz_size.height * 0.65-questionHeight()));
    } else {
        math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height * 0.05, quiz_size.width, quiz_size.height * 0.95-questionHeight()));
    }
    quizBegan(0, true);
    
    for (Math_Object* object : _objects) {
        object->fitObjectSize();
        addChildNode(object);
    }
    randomShow(_objects);
}

void Nam_ColorObject::startQuestion() {
    enableControlQuiz();
    keyColor = imagesColor[questionIndex()]->getText("color");
    if (!pickColor) {
        currentColor = keyColor;
    }
}

void Nam_ColorObject::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
        for (Math_Object* obj : _objects) {
            if (obj->getComparisionValue().color.compare("white") != 0) {
                obj->setImageColor("white");
            }
        }
    } else {
        disableControlQuiz();
        nextQuestion(1.0);
    }
}

void Nam_ColorObject::onComplete() {
    randomHide(_objects);
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback
void Nam_ColorObject::onTouchedObject(cocos2d::Node* node){
    Math_Object* object = static_cast<Math_Object*>(node);
    if (currentColor == "" || currentColor != keyColor) {
        object->onTouchObject(false);
        return;
    }
    
    bool correct = true;
    if (keyGame == "Small") {
        for (Math_Object* obj : _objects) {
            if (object->compare(obj, compare_key) > 0) {
                correct = false;
                break;
            }
        }
    } else if (keyGame == "Big") {
        for (Math_Object* obj : _objects) {
            if (object->compare(obj, compare_key) < 0) {
                correct = false;
                break;
            }
        }
    } else if (keyGame == "Different") {
        for (Math_Object* obj : _objects) {
            if (!object->isSame(obj) && object->compare(obj, compare_key) == 0) {
                correct = false;
                break;
            }
        }
    } else if (keyGame == "Ordinal") {
        auto ordinal = ordinal_imgs[questionIndex()];
        if (object->getComparisionValue().number != ordinal->getComparisionValue("number")) {
            correct = false;
        }
    } else if (keyGame == "Same") {
        correct = false;
        for (Math_Object* obj : _objects) {
            if (!object->isSame(obj) && object->compare(obj, compare_key) == 0) {
                correct = true;
                break;
            }
        }
        if (correct) {
            colored_objects.push_back(object);
        }
    }
    
    object->onTouchObject(correct);
    if (correct) {
        //TODO: fill color
        math::resource::Image* img_color_fill = nullptr;
        for (math::resource::Image* img : color_imgs) {
            if (img->getText("color") == currentColor && img->getText() == object->getComparisionValue().name) {
                img_color_fill = img;
                break;
            }
        }
        if (img_color_fill) {
            object->setImage(img_color_fill);
            object->setComparisionValue("color", img_color_fill->getText("color"));
        }
        if (keyGame == "Same") {
            if (colored_objects.size() == sameNumber) {
                onExplanation(true);
            }
        } else {
            onExplanation(true);
        }
    }
}

void Nam_ColorObject::onTouchedColor(cocos2d::Node* node){
    currentColor = static_cast<Math_Object*>(node)->getComparisionValue().color;
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_ColorObject::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Nam_ColorObject::autoTestingRun(float delaytime)
{
    
}

