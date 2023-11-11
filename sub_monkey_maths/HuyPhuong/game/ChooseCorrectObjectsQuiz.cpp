//
//  ChooseCorrectObjectsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#include "ChooseCorrectObjectsQuiz.h"

ChooseCorrectObjectsQuiz* ChooseCorrectObjectsQuiz::createQuiz()
{
    ChooseCorrectObjectsQuiz* quiz=new ChooseCorrectObjectsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ChooseCorrectObjectsQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void ChooseCorrectObjectsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    basic_type = getJsonBool(json_game, "type", true);
    compare_key = getComparisionKey(getJsonString(json_game, "compare_key", "id"));
    size_request = getJsonBool(json_game, "size_reques", false);
    had_temp = getJsonBool(getJsonMember(json_game, "template"), "enable", false);
    show_temp = getJsonBool(getJsonMember(json_game, "template"), "show", false);
    move_align = getJsonBool(json_game, "move_align", false);
    align = getJsonBool(json_game, "align", true);
    
    isBigger = true;
    bool use_first = getJsonBool(json_game, "use_first_icon", false);

    rapidjson::Value& _request=json_game.FindMember("request")->value;
    int value_keyword = 0;
    if (_request.Size() > 0) {
        value_keyword = cocos2d::random(0, (int)_request.Size()-1);
        question_req.push_back(_request[value_keyword].GetString());
        if (compare_key == math::comparision::size || size_request) {
            isBigger = question_req.front() == "big";
        }
    }
    
    same_req = false;
    for (std::string req : question_req) {
        if (req.compare("same")==0) {
            same_req=true;
        }
        if (req.compare("different")==0) {
            different_req=true;
        }
        if (req.compare("ordinal")==0) {
            ordinal_req=true;
        }
    }

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    int number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 4);
    int number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 4);
    int kind_max = getJsonInt(getJsonMember(json_object, "kind"), "maximum", 2);
    int kind_min = getJsonInt(getJsonMember(json_object, "kind"), "minimum", 2);
    object_size = getSizeJson(json_object, "size");
    std::string background_key = getJsonString(json_object, "background", "none");
    show_icon = getJsonString(json_object, "show", "icon");
    fit_object = getJsonString(json_object, "fit_size", "none");
    rapidjson::Value& dislay_json = json_object.FindMember("display")->value;
    display_type = getJsonString(dislay_json, "type", "line");
    max_in_line = getJsonInt(dislay_json, "max_line", 4);

    number_object = cocos2d::random(number_min, number_max);
    if (kind_max>number_object) {
        kind_max=number_object;
    }
    if (kind_min>kind_max) {
        kind_min=kind_max;
    }
    number_kind = cocos2d::random(kind_min, kind_max);
    
    auto all_images = getImageList("icons");
    if (background_key.compare("custom")==0) {
        std::vector<math::resource::Image*> bg_imgs;
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("role") == 1) {
                bg_imgs.push_back(img);
            } else {
                img_list.push_back(img);
            }
        }
        if (bg_imgs.size()>0) {
            img_bg=math::func::getRandomNode(bg_imgs);
            img_bg->path = image_prefix+img_bg->path;
            img_bg->used=true;
        }
    } else {
        img_bg = nullptr;
        if (background_key != "none") {
            img_bg = objectBG();
        }
    }
    
    //TODO: ICON CONFIG
    rapidjson::Value& icon_json = json_data.FindMember("icon")->value;
    icon_filter = getJsonString(icon_json, "different", "id");
    icon_same = getJsonString(icon_json, "same", "none");

    if (use_first) {
        img_list.push_back(all_images.front());
        all_images = math::resource::ResHelper::filterImages(all_images, icon_same, cocos2d::Value(img_list.front()->getText(icon_same)));
        int number_kind2 = number_kind-1;
        
        if (icon_filter.compare("none")==0) {
            auto image = math::func::getRandomNode(all_images);
            for (int i=0; i<number_kind2; i++) {
                img_list.push_back(image);
            }
        } else if (icon_filter.compare("id")==0) {
            auto dif_random=randomArrayIndexes(number_kind2, (int)all_images.size()-1);
            for (int i=0; i<number_kind2; i++) {
                auto image=all_images[dif_random[i]+1];
                img_list.push_back(image);
            }
        } else {
            if (icon_filter == "name") {
                icon_filter = "name_1";
            }
            //check filter is prop
            bool filter_prop = true;
            for (math::resource::Property prop : all_images.front()->comparisions) {
                if (prop.key == icon_filter) {
                    filter_prop = false;
                    break;
                }
            }
            
            if (filter_prop) {
                std::vector<std::string>choose_imgs;
                choose_imgs.push_back(img_list.front()->getText(icon_filter));
                bool add_img;
                do {
                    add_img=true;
                    std::string m_kind=math::func::getRandomNode(all_images)->getText(icon_filter);
                    for (std::string kind: choose_imgs) {
                        if (kind.compare(m_kind)==0) {
                            add_img=false;
                            break;
                        }
                    }
                    if (add_img) {
                        choose_imgs.push_back(m_kind);
                    }
                } while (choose_imgs.size()<number_kind);
                
                for (int i=1; i<number_kind; i++) {
                    std::string kind=choose_imgs[i];
                    while (true) {
                        auto image=math::func::getRandomNode(all_images);
                        if (image->getText(icon_filter).compare(kind)==0) {
                            img_list.push_back(image);
                            break;
                        }
                    }
                }
            } else {
                std::vector<int>choose_imgs;
                bool add_img;
                do {
                    add_img=true;
                    int m_kind=math::func::getRandomNode(all_images)->getComparisionValue(icon_filter);
                    for (int kind: choose_imgs) {
                        if (kind == m_kind) {
                            add_img=false;
                            break;
                        }
                    }
                    if (add_img) {
                        choose_imgs.push_back(m_kind);
                    }
                } while (choose_imgs.size()<number_kind2);
                
                for (int i=1; i<number_kind; i++) {
                    int kind = choose_imgs[i];
                    while (true) {
                        auto image=math::func::getRandomNode(all_images);
                        if (image->getComparisionValue(icon_filter)==kind) {
                            img_list.push_back(image);
                            break;
                        }
                    }
                }
            }
        }
    } else {
        if (background_key.compare("custom")==0) {
            img_list = getImageList(img_list, number_kind, icon_filter, icon_same);
        } else {
            img_list = getImageList(all_images, number_kind, icon_filter, icon_same);
        }
    }

    //Random so luong tung kind
    if (same_req) {
        if (number_kind==number_object) {
            number_kind=number_object-1;
        }
        number_same = number_object-number_kind+1;
        int index_same=0;
        index_same = cocos2d::random(0, number_kind-1);

        for (int i=0; i<number_kind; i++) {
            if (i==index_same) {
                number_list.push_back(number_same);
            } else {
                number_list.push_back(1);
            }
        }
        for (int i=0; i<number_kind; i++) {
            if (compare_key == math::comparision::size || size_request) {
                if (i==index_same) {
                    for (int i2=0; i2<number_same; i2++) {
                        size_list.push_back(i);
                    }
                } else {
                    size_list.push_back(i);
                }
            } else {
                size_list.push_back(0);
            }
        }
    } else if (different_req) {
        number_same = number_object-1;
        number_kind = 2;
        //int index_same=0;
        int index_same=cocos2d::random(0, 1);

        for (int i=0; i<number_kind; i++) {
            if (i==index_same) {
                number_list.push_back(number_same);
            } else {
                number_list.push_back(1);
            }
        }
        for (int i=0; i<number_kind; i++) {
            if (compare_key == math::comparision::size || size_request) {
                if (i==index_same) {
                    for (int i2=0; i2<number_same; i2++) {
                        size_list.push_back(i);
                    }
                } else {
                    size_list.push_back(i);
                }
            } else {
                size_list.push_back(0);
            }
        }
    } else {
        //Random list number & size
        int _div = number_object / number_kind;
        int _mod = number_object % number_kind;
        for (int i=0; i<number_kind; i++) {
            if (i<_mod) {
                number_list.push_back(_div+1);
            } else {
                number_list.push_back(_div);
            }
        }
        math::func::shuffle(number_list);
        
        //Check compare size.
        if (compare_key == math::comparision::size || size_request) {
            for (int i=0; i<number_kind; i++) {
                for (int i2=0; i2 < number_list[i]; i2++) {
                    if (isBigger) {
                        size_list.push_back(i);
                    } else {
                        size_list.push_back(number_kind-1-i);
                    }
                }
            }
        }
    }

    if (ordinal_req) {
        ordinal_number = cocos2d::random(1, number_object);
        img_number_list = getOrdinals(ordinal_number, ordinal_number);
    } else {
        for (int number : number_list) {
            img_number_list.push_back(math::number::getNumber(number));
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object
    int obj_id=0;
    auto imgs_gen = getImageListNumber(all_images, img_list, number_list, icon_filter);
    
    if (size_list.empty()) {
        std::vector<int> temp_size;
        if (compare_key == math::comparision::weight) {
            for (int i=0; i<imgs_gen.size(); i++) {
                temp_size.push_back((5-imgs_gen[i]->getComparisionValue("weight"))*2);
            }
        } else if (compare_key == math::comparision::hold) {
            for (int i=0; i<imgs_gen.size(); i++) {
                temp_size.push_back((10-imgs_gen[i]->getComparisionValue("hold"))*2);
            }
        } else {
            for (int i=0; i<number_kind; i++) {
                temp_size.push_back(0);
            }
        }
        
        int min_value = 5;
        for (int size_value : temp_size) {
            if (min_value > size_value) {
                min_value = size_value;
            }
        }
        
        std::vector<int> fix_size;
        int max_value_size = 0;
        for (int size_value : temp_size) {
            int value = size_value - min_value;
            if (value > max_value_size) {
                max_value_size = value;
            }
            fix_size.push_back(value);
        }
        //fix size value
        int size_value_next = 0;
        for (int i=0; i<=max_value_size; i++) {
            bool find = false;
            for (int size_value : fix_size) {
                if (i == size_value) {
                    size_list.push_back(size_value_next);
                    find = true;
                }
            }
            if (find) {
                size_value_next++;
            }
        }
    }
    
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id, math::common::touch, imgs_gen[obj_id], img_bg);
            if (compare_key == math::comparision::size || size_request) {
                object->setComparisionValue(size_list[obj_id]);
            }
            object->setObjectSize(object_size, true);
            object->setTouchCallback(CC_CALLBACK_1(ChooseCorrectObjectsQuiz::onTouchedObject, this));
            if (fit_object.compare("width")==0) {
                object->fixWidthObject();
            } else if (fit_object.compare("height")==0) {
                object->fixHeightObject();
            }
            
            if (move_align) {
                //object->enableDrag(_panel);
                if (max_in_line==1) {
                    object->setMoveDirection(1);
                } else {
                    object->setMoveDirection(2);
                }
                object->setMoveCallback(CC_CALLBACK_1(ChooseCorrectObjectsQuiz::onMoveObjectBegan, this), CC_CALLBACK_2(ChooseCorrectObjectsQuiz::onMoveObjectEnded, this));
            }
            if (show_icon.compare("icon")!=0) {
                object->setImageVisible(false);
                object->setValueTextByKey(show_icon);
                object->showValueTextWithAnimation(false);
                if (show_icon.compare("number")==0) {
                    object->getLabelValue()->setFontSize(object_size.height*0.75);
                }
            }
            _objects.push_back(object);
            obj_id++;
        }
    }
    
    if (had_temp) {
        temp_object = mpool->getObject();
        temp_object->setEnable(0, math::common::deny, img_list[0], img_bg);
        temp_object->setComparisionValue(size_list[0]);
        temp_object->setObjectSize(object_size);
    }
    //mpool->resetImages();
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    if (basic_type) {
        parseQuestion();
    } else {
        parseQuestion(true, number_kind);
    }
    auto opt_keys = getQuestion()->getOptionalKeys();
    bool fix_name = img_list.front()->getComparisionValue("role") >= 0 && img_list.front()->getComparisionValue("number") >= 0;
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            if (fix_name) {
                for (math::resource::Image* img : img_list) {
                    auto img_txt = img->getTextValue("name_1");
                    img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                    texts.push_back(img_txt);
                }
            } else {
                for (math::resource::Image* img : img_number_list) {
                    auto img_txt = img->getTextValue("name_1");
                    img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            if (fix_name) {
                for (math::resource::Image* img : img_list) {
                    auto img_txt = img->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            } else {
                for (math::resource::Image* img : img_number_list) {
                    auto img_txt = img->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "ordinal") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_number_list) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "name_n") {
            if (fix_name) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : img_list) {
                    if (img->getComparisionValue("number") < 2) {
                        texts.push_back(img->getTextValue("name_1"));
                    } else {
                        texts.push_back(img->getTextValue("name_n"));
                    }
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_list);
            }
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue(value_keyword);

}

void ChooseCorrectObjectsQuiz::onStart() {
    startGuide();
    quizBegan(0, false);
    
    math::func::shuffle(_objects);
    if (fit_object == "height") {
        auto max_size = object_size;
        for (int i=0; i<number_object; i++) {
            if (_objects[i]->getContentSize().width > max_size.width) {
                max_size = _objects[i]->getContentSize();
            }
        }
        math::func::setPositions(_objects, cocos2d::Rect((quiz_size.width - max_size.width - 50)/2, 0, max_size.width + 50, quiz_size.height), math::func::middle, math::func::left);
    } else if (fit_object == "width") {
        math::func::setPositions(_objects, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height), math::func::bottom, math::func::center);
    } else {
        if (display_type == "line") {
            math::func::setPositions(_objects, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height), math::func::bottom, math::func::center);
        } else {
            //random
            cocos2d::Rect rect_box;
            if (number_object < 5) {
                rect_box = cocos2d::Rect(quiz_size.width * 0.2, quiz_size.height * 0.075, quiz_size.width * 0.6, quiz_size.height*0.85);
            } else if (number_object < 10) {
                rect_box = cocos2d::Rect(quiz_size.width * 0.1, quiz_size.height * 0.05, quiz_size.width * 0.8, quiz_size.height*0.9);
            } else {
                rect_box = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height);
            }
            math::func::setPositionsRandom(_objects, rect_box);
        }
    }
    
    if (ordinal_req) {
        auto arrow_position = _objects.front()->getPosition() + cocos2d::Vec2(0, -object_size.height);
        line = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
        line->setPosition(arrow_position);
        addChildNode(line);
        
        auto arrow = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
        arrow->setPosition(cocos2d::Vec2(line->getContentSize().width, line->getContentSize().height*0.5));
        line->addChild(arrow);
    }
    
    if (move_align) {
        line = cocos2d::ui::ImageView::create(getUIPath("common/line/line_1.png"));
        addChildNode(line);

        if (max_in_line==1) {
            move_align_target = 250;
            float max_width = _objects.front()->getContentSize().width;
            for (int i=1; i<_objects.size(); i++) {
                if (max_width < _objects[i]->getContentSize().width) {
                    max_width = _objects[i]->getContentSize().width;
                }
            }
            move_align_target = (quiz_size.width - max_width) / 2;
            line->setPosition(cocos2d::Vec2(move_align_target, (quiz_size.height)/2));
            
            //fix position
            for (Math_Object* object : _objects) {
                object->setPosition(cocos2d::Vec2(object->getPosition().x+cocos2d::random(2, 10)*10, object->getPosition().y));
            }
            
        } else {
            move_align_target = 75;
            float width = (900-(_objects[0]->getContentSize().width +90) * max_in_line)/2;
            if (width<0) {
                width=0;
            }

            line->setPosition(cocos2d::Vec2(quiz_size.width/2, move_align_target));
            line->setRotation(90);

            //fix position
            for (Math_Object* object : _objects) {
                object->setPosition(cocos2d::Vec2(object->getPosition().x, object->getPosition().y+cocos2d::random(10, 50)));
            }
        }
        line->setOpacity(0);
        line->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(1), cocos2d::FadeIn::create(0.4)));
    }

    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->fitObjectSize();
        addChildNode(object, 2);
    }
    randomShow(_objects);
    
    if (had_temp && show_temp) {
        temp_object->setPosition(cocos2d::Vec2(quiz_size.width * 0.5, quiz_size.height - getQuestion()->getContentSize().height-40 - temp_object->getContentSize().height*0.5));
        addChildNode(temp_object);
        temp_object->onShow(1, math::animation::ScaleUp);
    }
}

void ChooseCorrectObjectsQuiz::startQuestion() {
    enableControlQuiz();
    if (questionIndex()==0) {
        if (move_align) {
            for (Math_Object* object : _objects) {
                object->enableDrag(_panel);
            }
        }
    } else {
        for (Math_Object* object : answer_list) {
            object->setSelected(false);
        }
        answer_list.clear();
        
        if (move_align) {
            for (Math_Object* object : _objects) {
                object->setTouch(math::common::touch);
            }
            line->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
                line->removeFromParentAndCleanup(true);
            }), NULL));
        }
    }
}

void ChooseCorrectObjectsQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion(0.5);
    }
}

void ChooseCorrectObjectsQuiz::onComplete() {

    if (line) {
        line->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            line->removeFromParentAndCleanup(true);
        }), NULL));
    }
    std::vector<int> random_hide;
    for (int i=0; i<_objects.size(); i++) {
        random_hide.push_back(i);
    }
    int number_div = ((int)_objects.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_hide);
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        object->onHide((random_hide[i]/number_div)*0.1);
    }
    
    quizEnded(0.75, true);
}

void ChooseCorrectObjectsQuiz::onGuideStarted() {
    disableControlQuiz();
    if (GuideCenter->guideCount() == 1) {
        speakQuestion(true);
        GuideCenter->guideNone();
    } else {
        if (move_align && questionIndex() == 0) {
            Math_Object* object = nullptr;
            for (Math_Object* i_object : _objects) {
                if (i_object->getTouch() != math::common::move) {
                    object = i_object;
                    break;
                }
            }
            if (object) {
                if (max_in_line==1) {
                    //move ngang
                    float target = move_align_target+object->getContentSize().width/2;
                    GuideCenter->guideBySwipe(nullptr, object->getWorldPosition(), cocos2d::Vec2(target, object->getWorldPosition().y));
                } else {
                    float target = move_align_target+object->getContentSize().height/2;
                    GuideCenter->guideBySwipe(nullptr, object->getWorldPosition(), cocos2d::Vec2(object->getWorldPosition().x, target));
                }
            } else {
                GuideCenter->guideNone();
            }
        } else {
            Math_Object* object = nullptr;
            for (Math_Object* i_object : _objects) {
                if (checkObject(i_object)) {
                    object = i_object;
                    break;
                }
            }
            if (object) {
                GuideCenter->guideByTouch(object->getWorldPosition());
            } else {
                GuideCenter->guideNone();
            }
        }
    }
}

void ChooseCorrectObjectsQuiz::onGuideDone() {
    enableControlQuiz();
}


#pragma mark - object touch events callback
bool ChooseCorrectObjectsQuiz::checkObject(Math_Object* object) {
    
    std::string question_request = "";
    if (!basic_type || move_align) {
        question_request = question_req[0];
    } else {
        question_request = question_req[questionIndex()];
    }
    
    /*--- Add vao answer list ---*/
    if (!basic_type) {
        //chi touch chon duoc nhung icon dung
        for (int i = 0; i < answer_list.size(); i++) {
            auto obj = answer_list[i];
            if (object->isSame(obj)) {
                return false;
            }
        }
        
        bool is_correct = false;
        if (object->isEqual(img_list[questionIndex()], compare_key)) {
            is_correct = true;
        }
        if (is_correct && size_request) {
            if (isBigger) {
                if (!object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                    is_correct=false;
                }
            } else {
                if (object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                    is_correct=false;
                }
            }
        }
        if (!is_correct) {
            return false;
        }
    } else {
        for (int i = 0; i < answer_list.size(); i++) {
            auto obj = answer_list[i];
            if (object->isSame(obj)) {
                return false;
            }
        }
    }
    
    bool is_correct=true;
    if (question_request.compare("small")==0) {
        //So dap an = 1;
        //check color
        if (is_correct) {
            for (Math_Object* obj : _objects) {
                if (!object->isSame(obj) && object->compare(obj, compare_key) >= 0) {
                    is_correct=false;
                    break;
                }
            }
        }
        return is_correct;
    }
    
    if (question_request.compare("big")==0) {
        //So dap an =1
        if (is_correct) {
            for (Math_Object* obj : _objects) {
                if (!object->isSame(obj) && object->compare(obj, compare_key) <= 0) {
                    is_correct=false;
                    break;
                }
            }
        }
        return is_correct;
    }
    
    if (question_request.compare("different")==0) {
        //So dap an = 1
        if (is_correct) {
            for (Math_Object* obj : _objects) {
                if (!object->isSame(obj) && object->isEqual(obj, compare_key)) {
                    is_correct=false;
                    break;
                }
            }
        }
        return is_correct;
    }
    
    if (question_request.compare("ordinal")==0) {
        //So dap an = 1
        if (is_correct) {
            for (int k=0; k<_objects.size(); k++) {
                if (object->isSame(_objects[k])) {
                    if ((k+1) != ordinal_number) {
                        is_correct=false;
                    }
                    break;
                }
            }
        }
        return is_correct;
    }
    
    if (question_request.compare("compare")==0) {
        //So dap an = number_list[0]
        if (had_temp) {
            if (!object->isEqual(temp_object, compare_key)) {
                return false;
            }
            if (size_request) {
                if (isBigger) {
                    if (!object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                        is_correct=false;
                    }
                } else {
                    if (object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                        is_correct=false;
                    }
                }
            }
            return is_correct;
            
        } else {
            int index_ques= questionIndex();
            if (!object->isEqual(img_list[index_ques], compare_key)) {
                return false;
            }
            if (is_correct && size_request) {
                if (isBigger) {
                    if (!object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                        is_correct=false;
                    }
                } else {
                    if (object->isEqual(cocos2d::Value(0), math::comparision::size)) {
                        is_correct=false;
                    }
                }
            }
            return is_correct;
        }
    }
    
    if (question_request.compare("same")==0) {
        //So dap an = number_same
        bool valid = false;
        for (Math_Object* obj : _objects) {
            if (!object->isSame(obj)) {
                if (object->isEqual(obj, compare_key)) {
                    if (size_request) {
                        if ((isBigger && object->isEqual(cocos2d::Value(0), math::comparision::size)) || (!isBigger && !object->isEqual(cocos2d::Value(0), math::comparision::size))) {
                            valid = true;
                            break;
                        }
                    } else {
                        valid = true;
                        break;
                    }
                }
            }
        }
        return valid;
    }
    return false;
}

void ChooseCorrectObjectsQuiz::onTouchedObject(cocos2d::Node* node)
{
    GuideCenter->resetGuideCount();
    Math_Object* object = (Math_Object*) node;
    bool is_correct = checkObject(object);
    if (is_correct) {
        GuideCenter->resetIncorrectCount();
        std::string question_request = "";
        if (!basic_type || move_align) {
            question_request = question_req[0];
        } else {
            question_request = question_req[questionIndex()];
        }
        
        object->onTouchObject(true, 0, 100);
        if (question_request == "same") {
            answer_list.push_back(object);
            onExplanation(answer_list.size() >= number_same);
        } else if (question_request == "compare") {
            answer_list.push_back(object);
            onExplanation(answer_list.size()>= number_list[questionIndex()]);
        } else {
            onExplanation(true);
        }
    } else {
        GuideCenter->increamentIncorrect();
        object->onTouchObject(false);
        onExplanation(false);
    }
}

void ChooseCorrectObjectsQuiz::onMoveObjectBegan(cocos2d::Node* node)
{
    GuideCenter->resetGuideCount();
}

void ChooseCorrectObjectsQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
    //Danh cho game sap xep lai vi tri roi chon dap an
    Math_Object* object = (Math_Object*)node;

    if (max_in_line==1) {
        //move ngang
        float target = move_align_target+object->getContentSize().width/2;
        if (abs((point.x - target))< 20) {
            object->runAction(cocos2d::MoveTo::create(0.15, cocos2d::Vec2(target, point.y)));
            object->setTouch(math::common::deny);
            move_aligned++;
        }
    } else {
        float target = move_align_target+object->getContentSize().height/2;
        if (abs((point.y - target))< 20) {
            object->runAction(cocos2d::MoveTo::create(0.15, cocos2d::Vec2(point.x, target)));
            object->setTouch(math::common::deny);
            move_aligned++;
        }
    }
    
    if (move_aligned >= number_object) {
        onExplanation(true);
    }
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ChooseCorrectObjectsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ChooseCorrectObjectsQuiz::autoTestingRun(float delaytime)
{
    
}

