//
//  CountAndAnswerQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/15/17.
//
//

#include "CountAndAnswerQuiz.h"

CountAndAnswerQuiz* CountAndAnswerQuiz::createQuiz()
{
    CountAndAnswerQuiz* quiz=new CountAndAnswerQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CountAndAnswerQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CountAndAnswerQuiz::initQuiz(mj::model::Game game)
{
	//get lang
	rapidjson::Document document_icons;
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
	//
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;

    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    
    rapidjson::Value& json_count=json_game.FindMember("count")->value;
    counting = getJsonBool(json_count, "enable");
    if (counting) {
        count_sequence = getJsonBool(json_count, "sequence");
    }
    missing_type = getJsonString(json_game, "missing", "none");
    
    rapidjson::Value& json_frame = json_game.FindMember("frame")->value;
    frame_show = getJsonBool(json_frame, "enable", true);
    frame_column=5;
    frame_row=2;
    if (frame_show) {
        frame_row = getJsonInt(json_frame, "row");
        frame_column = getJsonInt(json_frame, "column");
    }
    
    arrange_line = getJsonBool(json_game, "arrange", true);

    start_number = randomRangeJson(json_game, "skip_count");
    ordinal = getJsonBool(json_game, "ordinal", false);
    skip_counting = getJsonInt(json_game, "skip_counting", 1);
    
    rapidjson::Value& revert_json = getJsonMember(json_game, "revert");
    count_revert = getJsonBool(revert_json, "enable", false);
    if (count_revert) {
        revert_total = getJsonInt(revert_json, "total", 10);
        start_number = 0;
        counting = false;
        ordinal = false;
        skip_counting = 1;
        missing_type = "none";
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    
    number_object = randomRangeJson(json_object, "number");
    object_size = getSizeJson(json_object, "size");
    object_bg = getJsonString(json_object, "background", "none");
    
    rapidjson::Value& json_object_value = json_object.FindMember("value")->value;
    object_value = getJsonBool(json_object_value, "type", true);
    object_value_position = getJsonString(json_object_value, "position", "middle");
    
    img_bg = nullptr;
    if (object_bg != "none") {
        img_bg = objectBG();
    }

    //TODO: ANSWER CONFIG
    rapidjson::Value& json_answer=json_data.FindMember("answer")->value;
    number_answer = randomRangeJson(json_answer, "number");
    int answer_min = getJsonInt(getJsonMember(json_answer, "value"), "minimum", 1);
    int answer_max = getJsonInt(getJsonMember(json_answer, "value"), "maximum", 4);
    answer_size = getSizeJson(json_answer, "size");
    answer_display_number = getJsonBool(json_answer, "display", true);

    //TODO:
    //Missing game
    index_missing=0;
    if (missing_type.compare("come_next")==0) {
        index_missing=number_object-1;
    } else if (missing_type.compare("come_before")==0) {
        index_missing=0;
    } else if (missing_type.compare("missing")==0) {
        index_missing=cocos2d::random(0, number_object-1);
    }
    
    //Con resource. game chi co 1 loai obj
    number_kind=1;
    number_list.push_back(number_object);
    
    std::string icon_filter = getJsonString(json_data, "icon", "id");
    img_list = getImageList("icons", number_kind, icon_filter);
        
    //TODO: Xử lý đáp án
    std::vector<int>list_answer_value;
    int correct_answer=0;
    if (missing_type.compare("none")==0) {
        correct_answer=number_object*skip_counting + start_number;
        if (count_revert) {
            correct_answer = revert_total - correct_answer;
        }
    } else {
        correct_answer=(index_missing + 1) *skip_counting + start_number;
    }
    list_answer_value.push_back(correct_answer);
    
    while (list_answer_value.size()<number_answer) {
        int answer_val=cocos2d::random(answer_min, answer_max)*skip_counting + start_number;
        bool new_val=true;
        for (int answer : list_answer_value) {
            if (answer_val==answer) {
                new_val=false;
                break;
            }
        }
        if (new_val) {
            list_answer_value.push_back(answer_val);
        }
    }
    
    if (ordinal) {
        img_number_list = getOrdinals(answer_min*skip_counting + start_number, answer_max*skip_counting + start_number);
    } else {
		//img_number_list = getNumbers(answer_min * skip_counting + start_number, answer_max * skip_counting + start_number);
        img_number_list = getNumbers(answer_min*skip_counting + start_number, answer_max*skip_counting + start_number,lang);
    }

    for (int value : list_answer_value) {
        if (ordinal) {
            list_answer_image.push_back(getOrdinals(value, value).front());
        } else {
			//list_answer_image.push_back(getNumbers(value, value).front());
            list_answer_image.push_back(getNumbers(value, value, lang).front());
        }
    }
    /*---> Lay xong data cua game*/
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen doi tuong
    math::common::TouchPermission touch_per=math::common::deny;
    if (counting) {
        touch_per=math::common::touch;
    }
    int obj_id=0;
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id++, touch_per, img_list[i], img_bg);
            object->setObjectSize(object_size);
            object->getLabelValue()->setFontSize(object_size.height * 0.275);
            object->getLabelValue()->enableOutline(cocos2d::Color4B::WHITE, 6);
            object->setValueTextPosition(object_value_position);
            if (missing_type.compare("none")!=0) {
                if (obj_id==index_missing+1) {
                    object->setValueText("?");
                } else {
                    math::resource::Image* img_number = NULL;
                    for (math::resource::Image* img : img_number_list) {
                        if (img->getComparisionValue("number") == obj_id*skip_counting+start_number) {
                            img_number = img;
                            break;
                        }
                    }
                    if (img_number != NULL) {
                        if (ordinal) {
                            object->setValueText(img_number->getText("name_n"));
                        } else {
                            if (object_value) {
                                object->setValueText(cocos2d::StringUtils::format("%d", img_number->getComparisionValue("number")));
                            } else {
                                object->setValueText(img_number->getText("name_n"));
                            }
                        }
                    }
                }
            }
            object->setTouchCallback(CC_CALLBACK_1(CountAndAnswerQuiz::onTouchedObject, this));
            _objects.push_back(object);
        }
    }
    
    if (frame_show) {
        math::resource::Image* frame_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
        if (_objects.size() > 10) {
            int number_obj = (int)_objects.size();
            int index = 0;
            while (number_obj > 0) {
                std::vector<Math_Object*> frame_objs;
                int number_in_frame = number_obj > 10 ? 10 : number_obj;
                for (int i = 0; i < number_in_frame; i++) {
                    frame_objs.push_back(_objects[i+index]);
                }
                Math_Group* group=mpool->getGroup();
                group->setEnable(index, frame_objs, math::group::frame, 10);
                group->setObjectSize(object_size);
                group->setGroupFrames(5, 2, frame_bg);
                if (!arrange_line) {
                    group->setAlignObjects(math::group::GroupAlign::random);
                }
                group->updatePosition(false, false);
                cocos2d::Vec2 center=cocos2d::Vec2(450, 375);
                _groups.push_back(group);

                index += number_in_frame;
                number_obj -= number_in_frame;
            }
        } else {
            Math_Group* group=mpool->getGroup();
            group->setEnable(0, _objects, math::group::frame, 10);
            group->setObjectSize(object_size);
            group->setGroupFrames(5, 2, frame_bg);
            if (!arrange_line) {
                group->setAlignObjects(math::group::GroupAlign::random);
            }
            group->updatePosition(false, false);
            _groups.push_back(group);
        }
    }

    //Gen answer
    auto answer_bg = objectBG();
    for (int i=0; i<number_answer; i++) {
        Math_Answer* answer=mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(CountAndAnswerQuiz::onAnswerDone, this), getAnswerSize());
        answer->setIcon(list_answer_image[i]);
        if (answer_display_number) {
            if (ordinal) {
                answer->setTitleWithIcon("name_1");
            } else {
                answer->setTitleWithIconProp("number");
            }
            answer->setCustomAnswer(answer_bg, cocos2d::Size(140, 140), 70, m_text_color, 0.3);
        } else {
            answer->setTitleWithIcon("name_n");
            answer->setCustomAnswer(answer_bg, cocos2d::Size(140, 140), 60, m_text_color, 0.3);
        }
        if (list_answer_image[i]->getComparisionValue("number") == correct_answer) {
            answer->setCorrect(true);
        }
        _answers.push_back(answer);
    }
}

void CountAndAnswerQuiz::onStart() {
    startGuide();
    if (!counting) {
        onShowBottomBar(0, 0.3);
    }
    quizBegan(0, false);
    if (counting) {
        if (frame_show) {
            math::func::smartSetPosition(_groups, _groups.front()->getContentSize(), cocos2d::Rect(0, quiz_size.height*0.175, quiz_size.width, quiz_size.height*0.65));
        } else {
            if (arrange_line) {
                math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height*0.175, quiz_size.width, quiz_size.height*0.65));
            } else {
                cocos2d::Rect random_rect = cocos2d::Rect(quiz_size.width*0.05, quiz_size.height*0.175, quiz_size.width*0.9, quiz_size.height*0.65);
                math::func::setPositionsRandom(_objects, random_rect);
            }
        }
    } else {
        if (frame_show) {
            math::func::smartSetPosition(_groups, _groups.front()->getContentSize(), cocos2d::Rect(0, quiz_size.height*0.35, quiz_size.width, quiz_size.height*0.65));
        } else {
            if (arrange_line) {
                math::func::setPositions(_objects, cocos2d::Rect(0, quiz_size.height*0.35, quiz_size.width, quiz_size.height*0.65));
            } else {
                cocos2d::Rect random_rect = cocos2d::Rect(quiz_size.width*0.05, quiz_size.height*0.35, quiz_size.width*0.9, quiz_size.height*0.65);
                math::func::setPositionsRandom(_objects, random_rect);
            }
        }
    }
    
    if (frame_show) {
        for (Math_Group* group : _groups) {
            addChildNode(group, 2);
            group->onShow();
        }
    } else {
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->fitObjectSize();
            addChildNode(object, 2);
        }
        randomShow(_objects);
    }
    
    math::func::shuffle(_answers);
    if (ordinal || !answer_display_number) {
        fixAnswerSize(quiz_size.width*0.22, quiz_size.width*0.2);
    }
    math::func::setPositions(_answers, cocos2d::Rect(30, 0, quiz_size.width-60, quiz_size.height*0.3-30));
}

void CountAndAnswerQuiz::startQuestion() {
    scheduleOnce([=](float){
        enableControlQuiz();
    }, 2, "delay_for_audio_question");
    if (counting) {
        if (questionIndex()==1) {
            onShowBottomBar(0, 0.3, true);
            scheduleOnce([=](float){
                audio_helper->play("howmany/pop_out10.mp3");
            }, 0.25, "audio out");
            scheduleOnce([=](float){
                audio_helper->play("howmany/pop_out10.mp3");
            }, 0.5, "audio out");
            
            float delta = quiz_size.height * 0.175;
            if (frame_show) {
                for (Math_Group* group : _groups) {
                    group->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, delta))));
                }
            } else {
                for (Math_Object* object : _objects) {
                    object->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveBy::create(0.5, cocos2d::Vec2(0, delta))));
                }
            }
            
            for (int i=0; i<_answers.size(); i++) {
                Math_Answer* answer = _answers[i];
                answer->fitAnswerSize();
                answer->onShowInParent(_panel_bot, 2, 0.5 + 0.1 * i);
            }
        }
    } else {
        if (missing_type.compare("none")!=0) {
            for (Math_Object* object : _objects) {
                object->showValueTextWithAnimation(true);
            }
        }
        for (int i=0; i<_answers.size(); i++) {
            Math_Answer* answer = _answers[i];
            answer->fitAnswerSize();
            answer->onShowInParent(_panel_bot, 2, 0.1 * i);
        }
    }
}

void CountAndAnswerQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion(0.75);
    }
}

void CountAndAnswerQuiz::onComplete() {
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(i*0.1, math::animation::SlideUp);
    }
    if (frame_show) {
        for (int i=0; i<_groups.size(); i++) {
            _groups[i]->onHide(0, (i%2==0 ? math::animation::SlideRight : math::animation::SlideLeft));
        }
    } else {
        randomHide(_objects);
    }
    quizEnded(0.75, true);
}

void CountAndAnswerQuiz::onGuideStarted() {
    disableControlQuiz();
    if (counting && questionIndex() == 0) {
        for (Math_Object* object : _objects) {
            if (!object->isLocked()) {
                GuideCenter->guideByTouch(object->getWorldPosition());
                return;
            }
        }
    } else {
        for (Math_Answer* answer : _answers) {
            if (answer->isCorrect()) {
                GuideCenter->guideByTouch(answer->getWorldPosition());
                return;
            }
        }
    }
    GuideCenter->guideNone();
}

void CountAndAnswerQuiz::onGuideDone() {
    enableControlQuiz();
}

#pragma mark - object touch events callback
void CountAndAnswerQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=static_cast<Math_Object*>(node);
    bool count=true;
    
    if (count_sequence) {
        int index=0;
        for (; index<number_object; index++) {
            if (_objects[index]->isSame(object)) {
                break;
            }
        }
        if (index==count_index) {
            count_index++;
        } else {
            count=false;
        }
    }
    
    if (count) {
        //Show count
        number_counting ++;
        math::resource::Image* img_number = getNumbers(number_counting * skip_counting + start_number, number_counting * skip_counting + start_number,lang).front();
        if (img_number != nullptr) {
            if (object_value) {
                object->setValueText(img_number->getText("name_1"));
                audio_helper->play_sequence(img_number->getAudios(), nullptr, "");
            } else {
                object->setValueText(img_number->getText("name_n"));
                audio_helper->play_sequence(img_number->getAudios(), nullptr, "");
            }
        }
        
        object->showValueTextWithAnimation(true);
        object->setLocked(true);
        
        if (number_counting==_objects.size()) {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0), cocos2d::CallFunc::create([=](){
                this->onExplanation(true);
            }), NULL));
        }
    } else {
        //forcus count
        _objects[count_index]->runAction(cocos2d::Repeat::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.1, 1.1), cocos2d::ScaleTo::create(0.1, 1)), 2));
    }
}

void CountAndAnswerQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool done = answer->isCorrect();
    if (!done) {
        GuideCenter->increamentIncorrect();
    }
    answer->onAnimationClick(done);
    answer->playAnswerAudio("name_1", 0.2);
    this->onExplanation(done);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 4
void CountAndAnswerQuiz::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(2.5);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CountAndAnswerQuiz::autoTestingRun(float delaytime)
{
    int index_ques=this->getQuestion()->getData().index;
    if (counting && index_ques==0) {
        //counting
        if (number_counting < _objects.size()) {
            Math_Object* object=math::func::getRandomNode(_objects);
            while (object->isLocked() || object->isSame(last_object_touch)) {
                object=math::func::getRandomNode(_objects);
            }
            last_object_touch=object;
            object->onTouched();
        }
        this->scheduleOnce([=](float dt) {
            this->autoTestingRun(delaytime);
        }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));

    } else {
        //Choose answer
        Math_Answer* answer;
        bool choose_ans=true;
        while (choose_ans) {
            choose_ans=false;
            answer=math::func::getRandomNode(_answers);
            for (Math_Answer* _ans : answer_tested) {
                if (answer==_ans) {
                    choose_ans=true;
                }
            }
        }
        answer_tested.push_back(answer);
        
        if (!answer->isCorrect()) {
            this->scheduleOnce([=](float dt) {
                this->autoTestingRun(delaytime);
            }, delaytime, cocos2d::StringUtils::format("autoTestingRun-%d", testing_step++));
        }
        answer->onAutoClick();
    }
}

