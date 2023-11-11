#include "Huy_ReadingNumber.h"

Huy_ReadingNumber* Huy_ReadingNumber::createQuiz()
{
    Huy_ReadingNumber* quiz=new Huy_ReadingNumber();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Huy_ReadingNumber::init() {
	return cocos2d::ui::Layout::init();
}

void Huy_ReadingNumber::initQuiz(mj::model::Game game) {
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());

	// Config
    rapidjson::Value& json_config = getJsonMember(document, "data");
    
    rapidjson::Value& game_config = getJsonMember(json_config, "game");
    int number_question = randomRangeJson(game_config, "number_question");
    int number_kind = randomRangeJson(game_config, "number_kind");
    int number_answer = randomRangeJson(game_config, "number_answer");
    
    int answer_value_min = getJsonInt(getJsonMember(game_config, "answer_value"), "minimum", 1);
    int answer_value_max = getJsonInt(getJsonMember(game_config, "answer_value"), "maximum", number_question+number_answer);

    // image
    rapidjson::Value& json_image = getJsonMember(json_config, "icon");
    std::string same = getJsonString(json_image, "same", "none");
    std::string diff = getJsonString(json_image, "same", "none");
    std::vector<math::resource::Image*> all_images = getImageList("icons", number_kind, diff, same);
    
    std::vector<int> number_in_kind;
    if (number_kind > 1) {
        int remain = number_question;
        int count = number_kind;
        while (number_in_kind.size() < number_kind-1) {
            if (count > remain) {
                count = remain;
            }
            if (count > 0) {
                int value = cocos2d::random(1, count);
                number_in_kind.push_back(value);
                count--;
                remain-=value;
            } else {
                number_in_kind.push_back(0);
            }
        }
        number_in_kind.push_back(remain);
    } else {
        number_in_kind.push_back(number_question);
    }

	// Random number for answer
	std::vector<int> _int_temp;
    _int_temp.push_back(number_question);
    while (_int_temp.size() < number_answer) {
        int value = cocos2d::random(answer_value_min, answer_value_max);
        bool can_add = true;
        for (int _val : _int_temp) {
            if (value == _val) {
                can_add = false;
                break;
            }
        }
        if (can_add) {
            _int_temp.push_back(value);
        }
    }
    math::func::shuffle(_int_temp);
    
    math::resource::Image* image_question = math::number::getNumber(number_question);
	// Add image for answer
    for (int value : _int_temp) {
        _image_answer.push_back(math::number::getNumber(value));
    }
    
    cocos2d::Size object_size = cocos2d::Size(140,  140);
    if (number_question > 10) {
        object_size = cocos2d::Size(100, 100);
    } else if (number_question > 5) {
        object_size = cocos2d::Size(110, 110);
    }
    for (int i = 0; i < number_kind; i++) {
        for (int i2 = 0; i2 < number_in_kind[i]; i2++) {
            Math_Object* object_images = mpool->getObject();
            object_images->setEnable(i, math::common::deny, all_images[i], NULL);
            object_images->setObjectSize(object_size);
            _object_images.push_back(object_images);
        }
    }
    
    // Gen object number
    Math_Object* object_number = mpool->getObject();
    object_number->setEnable(1, math::common::TouchPermission::deny, image_question, NULL);
    object_number->setObjectSize(cocos2d::Size(200, 200));
    object_number->setValueTextByKey("name_n");
    object_number->setImageVisible(false);
    object_number->showValueTextWithAnimation(false);
    object_number->getLabelValue()->enableOutline(cocos2d::Color4B::WHITE, 5);
    _objects.push_back(object_number);
    
    // Gen answer
    auto img_ans = objectBG();
    for (int i = 0; i < _image_answer.size(); i++) {
        Math_Answer* answer = mpool->getAnswer();
        answer->setEnable("", math::common::answer, CC_CALLBACK_1(Huy_ReadingNumber::onAnswerDone, this), getAnswerSize());
        answer->setIcon(_image_answer[i]);
        answer->setTitleWithIconProp("number");
        answer->setCustomAnswer(img_ans, cocos2d::Size(140,  140), 80);
        _answers.push_back(answer);
        if (_image_answer[i]->getComparisionValue("number") == number_question) {
            answer->setCorrect(true);
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "name_n") {
            if (number_question == 1) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : all_images) {
                    auto img_txt = img->getTextValue("name_1");
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, all_images);
            }
        } else {
            getQuestion()->setOptionalTexts(key, all_images);
        }
    }
}

void Huy_ReadingNumber::onStart() {
    startGuide();

    onShowBottomBar(0, 0.3);
    quizBegan(0, false);
    
    float view_h = (quiz_size.height * 0.7);
    math::func::setPositions(_object_images, cocos2d::Rect(0, quiz_size.height * 0.3 + view_h*0.4, quiz_size.width, view_h*0.6));
    
    std::vector<int> random_show;
    for (int i=0; i<_object_images.size(); i++) {
        random_show.push_back(i);
    }
    int number_div = ((int)_object_images.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < _object_images.size(); i++) {
        Math_Object* object = _object_images[i];
        object->fitObjectSize();
        addChildNode(object, 2);
        object->onShow((random_show[i]/number_div)*0.1);
    }
    
	// Add object number
    _objects.front()->setPosition(cocos2d::Vec2(quiz_size.width/2, (_object_images.back()->getPosition().y - _object_images.back()->getContentSize().height/2 + quiz_size.height*0.3)/2));
    this->addChildNode(_objects.front(), 2);
    _objects.front()->onShow(0.5, math::animation::SlideUp);
    
    math::func::shuffle(_answers);
    math::func::setPositions(_answers, cocos2d::Rect(30, 0, quiz_size.width-60, quiz_size.height * 0.3-30));
}

void Huy_ReadingNumber::startQuestion() {
	enableControlQuiz();
    for (int i=0; i < _answers.size(); i++) {
        _answers[i]->fitAnswerSize();
        _answers[i]->onShowInParent(_panel_bot, 2, i*0.1, math::animation::SlideUp);
    }
}

void Huy_ReadingNumber::onAnswerDone(cocos2d::Node* node) {
	Math_Answer* answer = (Math_Answer*)node;
    bool done = answer->isCorrect();
    if (!done) {
        GuideCenter->increamentIncorrect();
    }
    answer->onAnimationClick(done);
    answer->playAnswerAudio("name_1", 0.2);
    this->onExplanation(done);
}

void Huy_ReadingNumber::onExplanation(bool correct) {
    if (correct) {
        disableControlQuiz();
        this->nextQuestion(0.5);
    } else {
        disableControlQuiz(0.25);
    }
}

void Huy_ReadingNumber::onComplete() {
    int i = 0;
    for (Math_Answer* node : _answers) {
        node->disableClickAnswer();
        node->setDisable(i*0.1, math::animation::SlideUp);
        i++;
    }
    
    _objects.front()->onHide(0.5, math::animation::SlideUp);
    
    std::vector<int> random_show;
    for (int i=0; i<_object_images.size(); i++) {
        random_show.push_back(i);
    }
    int number_div = ((int)_object_images.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < _object_images.size(); i++) {
        Math_Object* object = _object_images[i];
        object->onHide((random_show[i]/number_div)*0.1, math::animation::ScaleUp);
	}
    
    quizEnded(0.5, true);
}

void Huy_ReadingNumber::onGuideStarted() {
    disableControlQuiz();
    
    for (Math_Answer* answer : _answers) {
        if (answer->isCorrect()) {
            GuideCenter->guideByTouch(answer->getWorldPosition());
            return;
        }
    }
    GuideCenter->guideNone();
}

void Huy_ReadingNumber::onGuideDone() {
    enableControlQuiz();
}

void Huy_ReadingNumber::autoTesting() {

}

void Huy_ReadingNumber::autoTestingRun(float delaytime) {

}
