//
//  CompareNumberOfObjectsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "CompareNumberOfObjectsQuiz.h"

CompareNumberOfObjectsQuiz* CompareNumberOfObjectsQuiz::createQuiz()
{
    CompareNumberOfObjectsQuiz* quiz=new CompareNumberOfObjectsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CompareNumberOfObjectsQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CompareNumberOfObjectsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    game_type = getJsonString(json_game, "game_type", "answer");
    game_rule = getJsonString(json_game, "game_rule", "more");
    number_kind = getJsonInt(json_game, "number_kind", 1);
    arange_kind = getJsonBool(json_game, "object_arange", true);
    answer_icon = getJsonBool(json_game, "answer_icon", false);
    max_line = getJsonInt(json_game, "max_line", 5);
    
    if (game_rule == "more") {
        gamerule_type = GameRuleType::More;
    } else if (game_rule == "few") {
        gamerule_type = GameRuleType::Fewer;
    } else {
        gamerule_type = GameRuleType::Enough;
    }

    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    auto number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    object_size = getSizeJson(json_object, "size");
    background_key = getJsonString(json_object, "background", "none");
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();
    auto all_imgs = getImageList("icons");
    
    img_list = getImageList(all_imgs, number_kind, icon_filter, icon_same);

    //Gen number list
    if (gamerule_type == GameRuleType::Enough) {
        for (int i=0; i<number_kind; i++) {
            number_list.push_back(cocos2d::random(number_min, number_max));
        }
        game_type = "yes_no";
    } else {
        std::vector<int> _numbers = randomArrayIndexes(number_kind, number_max-number_min+1);
        for (int number : _numbers) {
            number_list.push_back(number+number_min);
        }
        math::func::shuffle(number_list);
    }
    
    index_answer = 0;
    if (gamerule_type == GameRuleType::More) {
        int i_max = 0;
        int n_max = number_list[0];
        for (int i=0; i < number_list.size(); i++) {
            if (number_list[i] > n_max) {
                n_max = number_list[i];
                i_max = i;
            }
        }
        if (game_type.compare("answer")==0) {
            index_answer = i_max;
        } else {
            if (i_max>0) {
                index_answer=1;
            }
        }
    } else if (gamerule_type == GameRuleType::Fewer) {
        int i_min = 0;
        int n_min = number_list[0];
        for (int i=0; i < number_list.size(); i++) {
            if (number_list[i] < n_min) {
                n_min = number_list[i];
                i_min = i;
            }
        }
        if (game_type.compare("answer")==0) {
            index_answer = i_min;
        } else {
            if (i_min>0) {
                index_answer=1;
            }
        }
    } else {
        //enough
        if (number_list[0]<number_list[1]) {
            index_answer=1;
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    getQuestion()->setKeywordValue((int)gamerule_type);
    
    //TODO: GENERATE QUIZ COMPONENTS
    img_bg=objectBG();

    //Gen object
    max_numbericon=0;
    for (int i=0; i<number_kind; i++) {
        std::vector<Math_Object*> m_objects;
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(i, math::common::deny, img_list[i], img_bg);
            object->setObjectSize(object_size);
            _objects.push_back(object);
            m_objects.push_back(object);
        }
        all_objects.push_back(m_objects);
        if (m_objects.size()>max_numbericon) {
            max_numbericon=(int)m_objects.size();
        }
    }
    
    if (game_type.compare("yes_no")==0) {
        std::vector<math::resource::Image*> list_answer;
        list_answer.push_back(getTextByTextGroup("yes"));
        list_answer.push_back(getTextByTextGroup("no"));
        for (int i=0; i<2; i++) {
            Math_Answer* answer = mpool->getAnswer();
            answer->setEnable("", math::common::answer, CC_CALLBACK_1(CompareNumberOfObjectsQuiz::onAnswered, this), getAnswerSize(2));
            answer->setIcon(list_answer[i]);
            answer->setTitleWithIcon("name_1");
            answer->setCorrect(i==index_answer);
            _answers.push_back(answer);
        }
        math::func::shuffle(_answers);
    } else if (game_type.compare("answer")==0) {
        //Thêm thông tin định nghĩa đáp án. Để tạm là group name.
        for (int i=0; i<number_kind; i++) {
            Math_Answer* answer = mpool->getAnswer();
            answer->setEnable("", math::common::icon_name, CC_CALLBACK_1(CompareNumberOfObjectsQuiz::onAnswered, this), getAnswerSize());
            answer->setIcon(img_list[i]);
            answer->setTitleWithIcon("name_1");
            answer->setCorrect(i==index_answer);
            _answers.push_back(answer);
        }
        math::func::shuffle(_answers);
    }
}

void CompareNumberOfObjectsQuiz::onStart() {
    onShowBottomBar(0, 0.3);
    quizBegan(0, true);
    
    if (arange_kind) {
        int delta = (quiz_size.width - max_numbericon*object_size.width) / (max_numbericon+1);
        delta = delta > 50 ? 50 : delta;
        
        float objs_width = max_numbericon*(object_size.width + delta)-delta;
        while (objs_width>quiz_size.width) {
            delta-=5;
            objs_width = max_numbericon*(object_size.width + delta)-delta;
        }

        float position_y = (quiz_size.height*0.7-questionHeight()) * 0.5 + (number_kind - 1) * object_size.height * 0.5 + 60 * (number_kind - 1) * 0.5;
        
        for (int i=0; i<number_kind; i++) {
            auto m_objects = all_objects[i];
            math::func::smartSetPosition(m_objects, object_size, cocos2d::Rect(0, quiz_size.height*0.3, quiz_size.width, quiz_size.height*0.7-questionHeight()), delta);
            
            for (int k=0; k < m_objects.size(); k++) {
                Math_Object* object = m_objects[k];
                object->setPosition(cocos2d::Vec2(object->getPosition().x, position_y));
                addChildNode(object, 2);
                object->onShow(k*0.1);
            }
            position_y -= (60 + object_size.height);
        }
    } else {
        math::func::shuffle(_objects);
        math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(0, quiz_size.height*0.3, quiz_size.width, quiz_size.height*0.7-questionHeight()), 50);
        
        std::vector<int> random_show;
        for (int i=0; i<_objects.size(); i++) {
            random_show.push_back(i);
        }
        int number_div = ((int)_objects.size()+4)/5;
        if (number_div>1) {
            math::func::shuffle(random_show);
        }
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            addChildNode(object, 2);
            object->onShow((random_show[i]/number_div)*0.15);
        }
    }
    
    fixAnswerSize((quiz_size.width-200)/3, 200);
    float delta_ans = (quiz_size.width - 100 - _answers.front()->getContentSize().width * ((int)_answers.size()))/((int)_answers.size()+1);
    delta_ans = delta_ans > 150 ? 150:delta_ans;
    math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.25-30), delta_ans);
}

void CompareNumberOfObjectsQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (Math_Answer* answer : _answers) {
            answer->onShowInParent(_panel_bot, 2);
        }
    }
}

void CompareNumberOfObjectsQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}

void CompareNumberOfObjectsQuiz::onComplete() {
     
    float delay = 1;
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }

    float m_delay= delay-0.3;
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
        object->onHide(m_delay+(random_hide[i]/number_div)*0.15);
    }
    
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void CompareNumberOfObjectsQuiz::onAnswered(Node* node) {
    disableControlQuiz(0.25);
    Math_Answer* answer = (Math_Answer*)node;
    bool correct = answer->isCorrect();
    answer->onAnimationClick(correct);
    this->onExplanation(correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void CompareNumberOfObjectsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CompareNumberOfObjectsQuiz::autoTestingRun(float delaytime) {
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

