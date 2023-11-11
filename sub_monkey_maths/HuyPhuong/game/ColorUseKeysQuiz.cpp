//
//  ColorUseKeysQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "ColorUseKeysQuiz.h"

ColorUseKeysQuiz* ColorUseKeysQuiz::createQuiz()
{
    ColorUseKeysQuiz* quiz=new ColorUseKeysQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();//this line is importance !
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ColorUseKeysQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void ColorUseKeysQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data = getJsonMember(document, "data");
    
	
	//get lang
	rapidjson::Document document_icons;
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	math::lang::MathLanguage lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
	

    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    compare_key = getComparisionKey(getJsonString(json_game, "color_by", "id"));
    int over_color = randomRangeJson(json_game, "over_color");
    int number_key_max = getJsonInt(getJsonMember(json_game, "number_key"), "maximum", 2);
    int number_key_min = getJsonInt(getJsonMember(json_game, "number_key"), "minimum", 2);

    if (json_game.HasMember("color_story")) {
        is_story_config = true;
        rapidjson::Value& color_story = getJsonMember(json_game, "color_story");
        color_one = getJsonBool(color_story, "color_one", false);
        rand_color = getJsonBool(color_story, "rand_color", false);
        show_table = getJsonBool(color_story, "show_table", true);
        rapidjson::Value& colors = getJsonMember(color_story, "list");
        for (rapidjson::SizeType i = 0; i < colors.Size(); i++) {
            selected_color.push_back(colors[i].GetString());
        }
    }

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object = getJsonMember(json_data, "object");
    int number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 4);
    int number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 4);
    int kind_max = getJsonInt(getJsonMember(json_object, "kind"), "maximum", 2);
    int kind_min = getJsonInt(getJsonMember(json_object, "kind"), "minimum", 2);
    object_size = getSizeJson(json_object, "size");
    fit_object = getJsonString(json_object, "fit_size", "none");
    sort_in_line = getJsonBool(json_object, "sort_in_line", true);

    number_kind = cocos2d::random(kind_min, kind_max);
    number_object = cocos2d::random(number_min, number_max);
    
    if (compare_key == math::comparision::number) {
        img_number_list = getNumbers(number_key_min, number_key_max);
        math::func::shuffle(img_number_list);
        while (img_number_list.size() > number_kind) {
            img_number_list.pop_back();
        }
    }

    //TODO: ICON CONFIG
    rapidjson::Value& json_icon = getJsonMember(json_data, "icon");
    std::string icon_diff = getJsonString(json_icon, "different", "id");
    std::string icon_same = getJsonString(json_icon, "same", "none");
    
    auto all_imgs = getImageList("icons");
	for (auto img : all_imgs) {
		auto p = img->properties.front();
		if (p.lang == math::lang::vietnamese) {
			auto c = p;
			c.lang = math::lang::english_us;
			img->properties.push_back(c);
		}
	}
    std::vector<math::resource::Image*> white_imgs;
    std::vector<math::resource::Image*> colorful_imgs;
    for (math::resource::Image* img : all_imgs) {
        if (img->getText("color",lang) == "white") {
            white_imgs.push_back(img);
        } else {
            colorful_imgs.push_back(img);
        }
    }
    img_list = getImageList(white_imgs, number_kind, icon_diff, icon_same);
    
    int colorNumber = number_kind+over_color;
    auto allImageColor = getColors();
    //Bỏ màu trắng
    for (int i = 0; i<allImageColor.size(); i++) {
        if (allImageColor[i]->getText("color") == "white") {
            allImageColor.erase(allImageColor.begin()+i);
            break;
        }
    }
    //Chon những màu được config
    if (is_story_config && !selected_color.empty()) {
        std::vector<math::resource::Image*> choose_color_img;
        for (std::string color_select : selected_color) {
            for (math::resource::Image* img : allImageColor) {
                if (color_select == img->getText("color")) {
                    choose_color_img.push_back(img);
                    break;
                }
            }
        }
        allImageColor = choose_color_img;
    }
    math::func::shuffle(allImageColor);
    
    if (rand_color) {
        while (img_color.size() < colorNumber) {
            auto img = allImageColor.back();
            allImageColor.pop_back();
            img_color.push_back(img);
        }
        
        for (int i=0; i<number_kind; i++) {
            auto icon_name = img_list[i]->getText("name_1");
            auto icon_color = img_color[i]->getText("color");
            object_color_list.push_back(std::make_pair(icon_name, icon_color));
            
            for (math::resource::Image* img : colorful_imgs) {
                if (img->getText("name_1", lang) == icon_name) {
                    auto mcolor = img->getText("color", lang);
                    if (mcolor == icon_color) {
                        key_imgs.push_back(img);
                        for (math::resource::Image* i_color : img_color) {
                            if (mcolor == i_color->getText("color")) {
                                img_list_color.push_back(img);
                            }
                        }
                        break;
                    }
                }
            }
        }
        for (int i=0; i<over_color; i++) {
            auto icon_color = img_color[i+number_kind]->getText("color");
            for (math::resource::Image* img2 : img_list) {
                auto icon_name = img2->getText("name_1" , lang);
                for (math::resource::Image* img : colorful_imgs) {
                    if (img->getText("name_1", lang) == icon_name) {
                        auto mcolor = img->getText("color");
                        if (mcolor == icon_color) {
                            key_imgs.push_back(img);
                            for (math::resource::Image* i_color : img_color) {
                                if (mcolor == i_color->getText("color")) {
                                    img_list_color.push_back(img);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    } else {
        //getcolor for object
        for (math::resource::Image * icon : img_list) {
            auto img_name = icon->getText("name_1");
            for (math::resource::Image* img : colorful_imgs) {
                if (img->getText("name_1") == img_name) {
                    object_color_list.push_back(std::make_pair(img_name, img->getText("color")));
                    img_list_color.push_back(img);
                    break;
                }
            }
        }
        key_imgs = img_list_color;
        
        for (math::resource::Image* img : img_list_color) {
            auto mcolor = img->getText("color");
            for (math::resource::Image* img2 : allImageColor) {
                if (mcolor == img2->getText()) {
                    img_color.push_back(img2);
                    break;
                }
            }
        }
        
        //Bổ sung màu thừa ra gây nhiễu
        for (std::string color_name : selected_color) {
            bool find_color = false;
            for (math::resource::Image* m_color : img_list_color) {
                if (color_name == m_color->getText("color")) {
                    find_color = true;
                    break;
                }
            }
            if (!find_color) {
                for (math::resource::Image* img : allImageColor) {
                    if (color_name == img->getText()) {
                        img_color.push_back(img);
                        break;
                    }
                }
            }
            if (img_color.size() == colorNumber) {
                break;
            }
        }
    }
    
    //TODO: Question
    img_number_list = getNumbers(number_key_min, number_key_max);
    math::func::shuffle(img_number_list);
    while (img_number_list.size() > number_kind) {
        img_number_list.pop_back();
    }
    
    if (color_one) {
        parseQuestion(true, number_kind);
    } else {
        parseQuestion();
    }
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
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
        } else if (key == "ordinal") {
            img_number_list = getOrdinals(number_key_min, number_key_max);
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_n");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "color") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_color) {
                auto img_txt = img->getTextValue("name_vi");
				if (lang == math::lang::MathLanguage::english_us) {
					img_txt = img->getTextValue("name_1");
				}
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }

    //Random so luong tung kind
    std::vector<int>number_list;
    for (int i=0; i<number_kind; i++) {
        number_list.push_back(cocos2d::random(number_min, number_max));
    }
    //img_list = getImageListNumber("icons", img_list, number_list, icon_diff);
    if (compare_key==math::comparision::size) {
        //TODO: size_list
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    int size_fix = 0;
    int obj_id=0;
    
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id, math::common::touch, img_list[i], nullptr);
            if (compare_key==math::comparision::size) {
                object->setComparisionValue(size_list[obj_id]+size_fix);
            }
            object->setObjectSize(object_size);
            if (compare_key == math::comparision::number) {
                object->setComparisionValue("number", img_number_list[i]->getComparisionValue("number"));
                //object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
                object->setValueText(img_number_list[i]->getText("name_1"));
                object->getLabelValue()->setFontSize(object_size.height*0.5);
                object->showValueTextWithAnimation(false);
            }
            object->setColoring(true);
            object->setTouchCallback(CC_CALLBACK_1(ColorUseKeysQuiz::onTouchedObject, this));
            if (fit_object.compare("width")==0) {
                object->fixWidthObject();
            } else if (fit_object.compare("height")==0) {
                object->fixHeightObject();
            }
            _objects.push_back(object);
            obj_id++;
        }
    }

    /*--- key objects box ---*/
    //Tao box color
    colorBox = cocos2d::ui::ImageView::create(getUIPath("common/frame.png"));
    colorBox->setScale9Enabled(true);
    auto render_size = colorBox->getVirtualRendererSize();
    colorBox->setCapInsets(cocos2d::Rect(render_size.width*0.2, render_size.height*0.2, render_size.width*0.6, render_size.height*0.6));
    
    float max_w = 100;
    for (int i = 0; i < number_kind; i++) {
        Math_Object* obj = mpool->getObject();
        obj->setEnable(i, math::common::deny, key_imgs[i], nullptr);
        obj->setObjectSize(cocos2d::Size(70, 70));
        obj->getLabelValue()->setTextColor(getColor4B(img_color[i]->getText("color")));
        if (compare_key == math::comparision::number) {
            obj->setComparisionValue("color", img_color[i]->getText("color"));
            obj->setComparisionValue("number", img_number_list[i]->getComparisionValue("number"));
            obj->setValueText(cocos2d::StringUtils::format("%d - %s", obj->getComparisionValue().number, img_color[i]->getText("color").c_str()));
            obj->setImageVisible(false);
        } else {
            obj->setValueText(img_color[i]->getText("color"));
            obj->setValueTextPosition("right");
            obj->getLabelValue()->setPosition(cocos2d::Vec2(obj->getLabelValue()->getPosition().x + obj->getLabelValue()->getContentSize().width * 0.6, obj->getLabelValue()->getPosition().y));
        }
        obj->getLabelValue()->enableOutline(cocos2d::Color4B::WHITE, 2);
        obj->showValueTextWithAnimation(false);
        objectsKey.push_back(obj);
        colorBox->addChild(obj);
        
        auto obj_w = obj->getLabelValue()->getContentSize().width + 70;
        max_w = max_w < obj_w ? obj_w : max_w;
    }
    
    if (show_table) {
        colorBox->setContentSize(cocos2d::Size(max_w*number_kind, 80));
        float start_x = max_w/2;;
        for (int i = 0; i < number_kind; i++) {
            auto object = objectsKey[i];
            object->setPosition(cocos2d::Vec2(start_x + max_w * i, 40));
        }
        this->addChildNode(colorBox, 2);
    }
}

void ColorUseKeysQuiz::onStart() {
    startGuide();

    std::vector<std::string> color_no_repeat;
    for (int i = 0; i < img_color.size(); i++) {
        std::string c_color = img_color[i]->getText("name_1");
        bool is_valid = true;
        for (std::string n_color : color_no_repeat) {
            if (c_color == n_color) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            color_no_repeat.push_back(c_color);
        }
    }
    showColorsPad(color_no_repeat, [=](cocos2d::Node* node){
        current_color = static_cast<Math_Object*>(node)->getComparisionValue().color;
    }, nullptr);
    
    quizBegan(0, false);

    math::func::shuffle(_objects);
    float bar_width = 250.0/quiz_size.width;
    cocos2d::Rect rect_box = cocos2d::Rect(0, 0, quiz_size.width*(1 - bar_width), quiz_size.height);
    if (show_table) {
        rect_box = cocos2d::Rect(0, 100, quiz_size.width*(1 - bar_width), quiz_size.height - 80);
        colorBox->setPosition(cocos2d::Vec2(quiz_size.width * 0.5 * (1-bar_width), 50));
        colorBox->setOpacity(0);
        colorBox->runAction(cocos2d::FadeIn::create(0.4));
        for (Math_Object* object : objectsKey) {
            object->onShow(0, math::animation::SlideUp);
        }
    }

    if (sort_in_line) {
        math::func::setSmartPositions(_objects, rect_box);
    } else {
        math::func::setPositionsRandom(_objects, rect_box);
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->fitObjectSize();
        addChildNode(object, 2);
    }
    randomShow(_objects);
}

void ColorUseKeysQuiz::startQuestion() {
    enableControlQuiz();
    if (color_one) {
        ask_color = img_color[questionIndex()]->getText("name_1");
    }
}

void ColorUseKeysQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        for (Math_Object* object : _objects) {
            if (object->getComparisionValue().color != "white") {
                object->setColoring(false);
            }
        }
        this->nextQuestion(0.75);
    }
}

void ColorUseKeysQuiz::onComplete() {
    if (show_table) {
        colorBox->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0), cocos2d::FadeOut::create(0.5)));
        for (cocos2d::ui::ImageView* line : line_boxes) {
            line->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0), cocos2d::FadeOut::create(0.5)));
        }
        for (Math_Object* object : objectsKey) {
            object->onHide(0);
        }
    }
    randomHide(_objects);
    quizEnded(0.5, false);
}

void ColorUseKeysQuiz::onGuideStarted() {
    disableControlQuiz();
    std::string color_name = "";

    if (color_one) {
        //get pen
        color_name = ask_color;
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
        for (std::pair<std::string, std::string> pair : object_color_list) {
            if (object_name == pair.first) {
                color_name = pair.second;
                break;
            }
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

    //get object
    std::vector<Math_Object*> objects;
    for (Math_Object* _object : _objects) {
        if (_object->getComparisionValue().color == "white") {
            if (compare_key == math::comparision::number) {
                int number = _object->getComparisionValue().number;
                for (Math_Object* key : objectsKey) {
                    if (key->getComparisionValue().number == number && key->getComparisionValue().color == color_name) {
                        objects.push_back(_object);
                        break;
                    }
                }
            } else if (compare_key == math::comparision::size) {
                
            } else {
                auto name = _object->getComparisionValue().name;
                for (Math_Object* key : objectsKey) {
                    if (key->getComparisionValue().name == name && key->getComparisionValue().color == color_name) {
                        objects.push_back(_object);
                        break;
                    }
                }
            }
        }
    }
    if (objects.empty()) {
        GuideCenter->guideNone();
    } else {
        math::func::shuffle(objects);
        GuideCenter->guideByColor(nullptr, cocos2d::Vec2(quiz_size.width-75, pen->getWorldPosition().y), math::func::getRandomNode(objects)->getWorldPosition());
    }
}

void ColorUseKeysQuiz::onGuideDone() {
    enableControlQuiz();
}


#pragma mark - object touch events callback
void ColorUseKeysQuiz::onTouchedObject(cocos2d::Node* node) {
    Math_Object* object = static_cast<Math_Object*>(node);
    if (current_color == object->getComparisionValue().color) {
        return;
    }

    //color lock
    if (!object->isColoring()) {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        return;
    }
    
    //Select color check
    if (current_color=="") {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        return;
    }
    if (color_one && current_color != ask_color) {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        return;
    }
    
    //Color and object check
    math::resource::Image* img_color_fill = nullptr;
    bool color_correct = true;
    if (compare_key == math::comparision::number) {
        int number = object->getComparisionValue().number;
        for (Math_Object* key : objectsKey) {
            if (key->getComparisionValue().number == number) {
                if (key->getComparisionValue().color != current_color) {
                    color_correct = false;
                }
            }
        }
        for (math::resource::Image* img : img_list_color) {
            if (img->getText("color") == current_color) {
                img_color_fill = img;
                break;
            }
        }
    } else if (compare_key == math::comparision::size) {
        
    } else {
        auto name = object->getComparisionValue().name;
        for (Math_Object* key : objectsKey) {
            if (key->getComparisionValue().name == name) {
                if (key->getComparisionValue().color != current_color) {
                    color_correct = false;
                }
            }
        }
        for (math::resource::Image* img : img_list_color) {
            if (img->getText("color") == current_color && img->getText("name_1") == name) {
                img_color_fill = img;
                break;
            }
        }
    }
    
    if (rand_color || color_correct) {
        object->onTouchObject(true);
        GuideCenter->resetIncorrectCount();
        
        //get image to set
        object->setImage(img_color_fill);
        object->setComparisionValue("color", img_color_fill->getText("color"));
        
        bool check_done = true;
        if (!color_one) {
            for (Math_Object* obj : _objects) {
                if (obj->getComparisionValue().color == "white") {
                    check_done = false;
                    break;
                }
            }
        }
        
        if (check_done) {
            bool correct = checkAnswer();
            if (correct) {
                onExplanation(true);
            } else {
                disableControlQuiz(0.25);
            }
        } else {
            disableControlQuiz(0.25);
        }
    } else {
        disableControlQuiz(0.25);
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
    }
}

bool ColorUseKeysQuiz::checkAnswer() {
    std::vector<Math_Object*> wrong_list;

    if (color_one) {
        math::resource::Image* img_white = nullptr;
        int index_question = questionIndex();
        std::string k_color = objectsKey[index_question]->getComparisionValue().color;
        
        if (compare_key==math::comparision::number) {
            int k_number = objectsKey[index_question]->getComparisionValue().number;
            for (Math_Object* object : _objects) {
                if (object->isColoring()) {
                    int number = object->getComparisionValue().number;
                    std::string color = object->getComparisionValue().color;
                    if (number == k_number) {
                        if (color != k_color) {
                            wrong_list.push_back(object);
                        }
                    } else {
                        if (color != "white") {
                            wrong_list.push_back(object);
                        }
                    }
                }
            }
            img_white = img_list.front();
        } else if (compare_key==math::comparision::size) {
            
        } else {
            std::string k_name = objectsKey[index_question]->getComparisionValue().name;
            for (Math_Object* object : _objects) {
                if (object->isColoring()) {
                    std::string name = object->getComparisionValue().name;
                    std::string color = object->getComparisionValue().color;
                    if (name == k_name) {
                        if (color != k_color) {
                            wrong_list.push_back(object);
                        }
                    } else {
                        if (color != "white") {
                            wrong_list.push_back(object);
                        }
                    }
                }
            }
            for (math::resource::Image* img : img_list) {
                if (img->getText("name_1") == k_name) {
                    img_white = img;
                    break;
                }
            }
        }
        if (wrong_list.empty()) {
            for (Math_Object* obj : _objects) {
                if (obj->getComparisionValue().color != "white") {
                    obj->setColoring(false);
                }
            }
        } else {
            for (Math_Object* obj : wrong_list) {
                obj->setImage(img_white);
                obj->setComparisionValue("color", "white");
            }
        }
    } else {
        if (compare_key==math::comparision::number) {
            for (Math_Object* object : _objects) {
                int number = object->getComparisionValue().number;
                std::string color = object->getComparisionValue().color;
                
                for (Math_Object* object_key : objectsKey) {
                    int number_key = object_key->getComparisionValue().number;
                    if (number == number_key) {
                        std::string color_key = object_key->getComparisionValue().color;
                        if (color_key.compare(color)!=0) {
                            wrong_list.push_back(object);
                            break;
                        }
                    }
                }
            }
        } else if (compare_key==math::comparision::size) {
            
        } else {
            for (Math_Object* object : _objects) {
                std::string name = object->getComparisionValue().name;
                std::string color = object->getComparisionValue().color;
                
                for (Math_Object* object_key : objectsKey) {
                    std::string name_key = object_key->getComparisionValue().name;
                    if (name == name_key) {
                        std::string color_key = object_key->getComparisionValue().color;
                        if (color_key.compare(color)!=0) {
                            wrong_list.push_back(object);
                            break;
                        }
                    }
                }
            }
        }
        
        if (!wrong_list.empty()) {
            for (Math_Object* obj : wrong_list) {
                auto name = obj->getComparisionValue().name;
                math::resource::Image* white_img = nullptr;
                for (math::resource::Image* img : img_list) {
                    if (img->getText("name_1") == name) {
                        white_img = img;
                        break;
                    }
                }
                obj->setImage(white_img);
                obj->setComparisionValue("color", "white");
            }
        }
    }
    return wrong_list.empty();
}

void ColorUseKeysQuiz::onAnswerDone(cocos2d::Node *node) {
    Math_Answer* answer = (Math_Answer*)node;
    bool is_correct = checkAnswer();
    answer->onAnimationClick(is_correct);
    if (!is_correct) {
        speakQuestion();
    }
    this->onExplanation(is_correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ColorUseKeysQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ColorUseKeysQuiz::autoTestingRun(float delaytime)
{
    
    
}

