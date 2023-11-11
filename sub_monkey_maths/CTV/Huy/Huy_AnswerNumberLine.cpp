//
//  Huy_AnswerNumberLine.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "Huy_AnswerNumberLine.h"

Huy_AnswerNumberLine* Huy_AnswerNumberLine::createQuiz()
{
    Huy_AnswerNumberLine* quiz=new Huy_AnswerNumberLine();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Huy_AnswerNumberLine::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Huy_AnswerNumberLine::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = getJsonMember(json_data, "game");
    
    std::string game_type_str = "come_next";
    rapidjson::Value& game_type_json = getJsonMember(json_game, "game_play");
    if (!game_type_json.Empty()) {
        game_type_str = game_type_json[cocos2d::random(0, (int)game_type_json.Size()-1)].GetString();
    }
    if (game_type_str == "come_next") {
        game_type = come_next;
    } else if (game_type_str == "come_before") {
        game_type = come_before;
    } else {
        game_type = between;
    }
    
    number_line = getJsonBool(json_game, "number_line", true);
    number_object = randomRangeJson(json_game, "number_object");
    number_start = randomRangeJson(json_game, "number_start");
    
    all_images = getNumbers(number_start, number_start+number_object-1);
    int index_ask = 0;
    if (game_type == come_next) {
        index_ask = cocos2d::random(1, number_object-1);
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == number_start + index_ask-1) {
                img_list.push_back(img);
                break;
            }
        }
    } else if (game_type == come_before) {
        index_ask = cocos2d::random(0, number_object-2);
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == number_start + index_ask+1) {
                img_list.push_back(img);
                break;
            }
        }
    } else {
        index_ask = cocos2d::random(1, number_object-2);
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == number_start + index_ask-1) {
                img_list.push_back(img);
                break;
            }
        }
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("number") == number_start + index_ask+1) {
                img_list.push_back(img);
                break;
            }
        }
    }
    
    number_question = number_start+index_ask;
    for (math::resource::Image* img : all_images) {
        if (img->getComparisionValue("number") == number_question) {
            img_answer.push_back(img);
            break;
        }
    }
    
    int start_answer = number_start > 0 ? number_start-1 : number_start;
    int end_answer = (number_start+number_object-start_answer) < 5 ? number_start+number_object : number_start+5;
    auto all_imgs_ans = getNumbers(start_answer, end_answer);
    math::func::shuffle(all_imgs_ans);
    for (math::resource::Image* img : all_imgs_ans) {
        if (img->getComparisionValue("number") != number_question) {
            img_answer.push_back(img);
        }
        if (img_answer.size() >= 4) {
            break;
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_1");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_n");
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue((int)game_type);
    
    //Gen number inline
    auto bg_img = objectBG();
    auto bg_img2 = slotBG();

    object_size = cocos2d::Size(150, 150);
    for (int i=0; i<number_object; i++) {
        auto object = mpool->getObject();
        object->setEnable(i, math::common::deny, all_images[i], bg_img);
        object->setObjectSize(object_size);
        object->setImageVisible(false);
        object->setValueTextByKey("number");
        object->showValueTextWithAnimation(false);
        if (i==index_ask) {
            object->setObjectSize(object_size*0.95);
            object->setValueText("");
            object->setBackground(bg_img2);
        }
        object->getLabelValue()->setFontSize(object_size.height*0.55);
        _objects.push_back(object);
    }
    
    math::func::shuffle(img_answer);
    for (math::resource::Image * img : img_answer) {
        auto answer = mpool->getAnswer();
        answer->setEnable("", math::common::custom_bg, CC_CALLBACK_1(Huy_AnswerNumberLine::onAnswered, this), getAnswerSize());
        answer->setIcon(img);
        answer->setTitleWithIconProp("number");
        if (img->getComparisionValue("number") == number_question) {
            answer->setCorrect(true);
        }
        answer->setCustomAnswer(bg_img, object_size, object_size.height*0.55);
        _answers.push_back(answer);
    }
}

void Huy_AnswerNumberLine::onStart() {
    onShowBottomBar(0, 0.3);
    quizBegan(0, true);
    
    math::func::setPositionList(_answers, 4, cocos2d::Rect(quiz_size.width *0.05, 0, quiz_size.width*0.9, quiz_size.height * 0.3 - 40));
    
    int delta = 180;
    float objs_width = number_object*(object_size.width + delta) - delta;
    while (objs_width>quiz_size.width * 0.9) {
        delta-=5;
        objs_width = number_object*(object_size.width + delta) - delta;
    }
    float pos_x = (quiz_size.width-objs_width) /2.0;
    if (number_line) {
        math::func::setPositionList(_objects, number_object, cocos2d::Rect(pos_x, 0, quiz_size.width-pos_x*2, quiz_size.height * 0.625 - 30 -getQuestion()->getContentSize().height));
        
        //number line
        float start_line = _objects[0]->getPosition().x;
        float delta_line = _objects[1]->getPosition().x - _objects[0]->getPosition().x;
        float line_y = _objects[0]->getPosition().y + object_size.height *0.5 + 30;
        
        //line
        auto img_line = cocos2d::ui::ImageView::create(getUIPath("common/numberline/line.png"));
        img_line->setScale9Enabled(true);
        auto line_render = img_line->getVirtualRendererSize();
        img_line->setContentSize(cocos2d::Size(delta_line*(number_object), line_render.height));
        img_line->setPosition(cocos2d::Vec2(quiz_size.width *0.5, line_y));
        addChildNode(img_line);
        ui_elements.push_back(img_line);
        
        //arrows
        auto img_arrow = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
        img_arrow->setPosition(cocos2d::Vec2((quiz_size.width + delta_line*number_object) * 0.5 - 7, line_y));
        addChildNode(img_arrow);
        ui_elements.push_back(img_arrow);
        
        auto img_arrow2 = cocos2d::ui::ImageView::create(getUIPath("common/numberline/arrow.png"));
        img_arrow2->setPosition(cocos2d::Vec2((quiz_size.width - delta_line*number_object) * 0.5 + 7, line_y));
        img_arrow2->setFlippedX(true);
        addChildNode(img_arrow2);
        ui_elements.push_back(img_arrow2);
        
        //sub lines
        for (int i=0; i<number_object; i++) {
            auto img_subline = cocos2d::ui::ImageView::create(getUIPath("common/numberline/point.png"));
            img_subline->setPosition(cocos2d::Vec2(start_line + delta_line*i, line_y));
            addChildNode(img_subline);
            ui_elements.push_back(img_subline);
        }
        
    } else {
        math::func::setPositionList(_objects, number_object, cocos2d::Rect(pos_x, 0, quiz_size.width-pos_x*2, quiz_size.height * 0.7 - 30 -getQuestion()->getContentSize().height));
        
        float delta_line = _objects[1]->getPosition().x - _objects[0]->getPosition().x;
        float start_line = _objects[0]->getPosition().x + delta_line * 0.5;
        float line_y = _objects[0]->getPosition().y;
        
        for (int i=0; i<number_object-1; i++) {
            auto txt_comma = cocos2d::ui::Text::create(",", _objects.front()->getLabelValue()->getFontName(), _objects.front()->getLabelValue()->getFontSize());
            txt_comma->setPosition(cocos2d::Vec2(start_line + delta_line*i, line_y));
            txt_comma->setTextColor(_objects.front()->getLabelValue()->getTextColor());
            addChildNode(txt_comma);
            ui_elements.push_back(txt_comma);
        }
    }

    for (int i=0; i<number_object; i++) {
        addChildNode(_objects[i]);
        _objects[i]->onShow(i*0.1);
    }
    
    for (Node* child : ui_elements) {
        child->setOpacity(0);
        child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.5), cocos2d::FadeIn::create(0.25)));
    }
}

void Huy_AnswerNumberLine::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    for (int i=0; i<4; i++) {
        _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
    }
}

void Huy_AnswerNumberLine::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        scheduleOnce([=](float){
            for (Math_Object* object : _objects) {
                if (object->getComparisionValue().number == number_question) {
                    object->hideValueTextWithAnimation(false);
                    object->setValueTextByKey("number");
                    object->showValueTextWithAnimation(true);
                    object->onTouchObject(true);
                }
            }
        }, 0.5, "show answer");
        this->nextQuestion(2.0);
    }
}


void Huy_AnswerNumberLine::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(0.1*i, math::animation::SlideUp);
    }
    
    for (int i=0; i<number_object; i++) {
        _objects[i]->onHide(i*0.1);
    }
    
    for (Node* child : ui_elements) {
        child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::FadeOut::create(0.25), cocos2d::CallFunc::create([=](){
            child->removeFromParentAndCleanup(true);
        })));
    }
    
    quizEnded(0.5, true);
}

#pragma mark - object touch events callback
void Huy_AnswerNumberLine::onAnswered(Node* node) {
    Math_Answer* answer = (Math_Answer*)node;
    answer->onAnimationClick(answer->isCorrect());
    this->onExplanation(answer->isCorrect());
}


#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Huy_AnswerNumberLine::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Huy_AnswerNumberLine::autoTestingRun(float delaytime) {
    
}


