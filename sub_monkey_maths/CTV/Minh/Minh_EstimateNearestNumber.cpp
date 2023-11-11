////////
//        Minh_EstimateNearestNumber.cpp
//        MJ_Game
//        Create By MinhTzy on 08/11/2017
//


#include "Minh_EstimateNearestNumber.h"

Minh_EstimateNearestNumber * Minh_EstimateNearestNumber::createQuiz()
{
    Minh_EstimateNearestNumber * quiz = new Minh_EstimateNearestNumber();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return nullptr;
}
bool Minh_EstimateNearestNumber::init()
{
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Minh_EstimateNearestNumber::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value & json_data = document.FindMember("data")->value;
    
    //TODO : Game Config
    rapidjson::Value & game_data = getJsonMember(json_data, "game");
    
    rapidjson::Value & groups_data = getJsonMember(game_data,"groups");
    num_groups = randomRangeJson(groups_data, "num_groups");
    group_type = getJsonString(groups_data, "group_type","custom");
    group_size = getSizeJson(groups_data, "group_size");
    object_size = getSizeJson(groups_data, "object_size");
    answer = getJsonString(game_data, "answer", "about");
    
    // TODO : GET ICONS
    obj_img = math::func::getRandomNode(getImageList("icons"));
    list_icons.push_back(obj_img);
    //----> Lay xong data cua game
    
    math::group::GroupType gr_type = math::group::custom;
    math::group::GroupAlign gr_align = math::group::random;
    if (group_type.compare("custom") != 0) {
        gr_type = math::group::queue;
    }
    auto g_bg = groupBG();
    
    if (num_groups > 3) {
        num_groups=3;
    }
    int condition = getJsonInt(game_data, "condition", 1);
    
    if (num_groups==1) {
        int number_1 = randomRangeJson(game_data, "est_number") * condition;
        int number_2 = randomRangeJson(game_data, "est_number") * condition;
        number_about = (number_1 + number_2) * 0.1;
        while (abs(number_1 - number_2) < number_about*2 + 1) {
            number_1 = randomRangeJson(game_data, "est_number") * condition;
            number_2 = randomRangeJson(game_data, "est_number") * condition;
            number_about = (number_1 + number_2) * 0.1;
        }
        
        number_est_ques.push_back(number_1);
        number_est_ques.push_back(number_2);
        
        number_est.push_back(number_1 + cocos2d::random(-number_about, number_about));
        number_est.push_back(number_2 + cocos2d::random(-number_about, number_about));
        math::func::shuffle(number_est);
    } else {
        est_num = randomRangeJson(game_data, "est_number") * condition;
        number_est_ques.push_back(est_num);
        number_about = est_num * 0.2;
        number_est.push_back(est_num + cocos2d::random(-number_about, number_about));
        
        int hafl = est_num/2;
        int lower_number = (est_num - number_about);
        if (hafl < lower_number) {
            lower_number = cocos2d::random(hafl, lower_number);
        }
        number_est.push_back(lower_number);
        
        if (hafl > 10) {
            hafl = 10;
        }
        int upper_number = est_num + number_about + cocos2d::random(0, hafl);
        number_est.push_back(upper_number);
        
        if (num_groups == 2) {
            number_est.erase(number_est.begin() + cocos2d::random(1, 2));
        }
    }
    
    // #1. Gen Group
    for (int i=0; i<num_groups; i++) {
        Math_Group * group = mpool->getGroup();
        group->setEnable(i, GenObjects(number_est[i]), gr_type, number_est[i]);
        group->setObjectSize(object_size);
        if (gr_type == math::group::queue) {
            group->setGroupQueue(group_size);
            group->setBackground(g_bg, 0.3);
        } else {
            group->setGroupSize(group_size);
            group->setBackground(g_bg, 0.3);
        }
        group->setAlignObjects(gr_align);
        if (num_groups > 1) {
            group->enableToch(CC_CALLBACK_1(Minh_EstimateNearestNumber::onTouchGroup, this));
        }
        _groups.push_back(group);
    }
    math::func::shuffle(_groups);
    
    // #2. Gen Answer
    std::vector<math::resource::Image*> compare_images;
    compare_images.push_back(getTextByTextGroup("about"));
    compare_images.push_back(getTextByTextGroup("more than"));
    compare_images.push_back(getTextByTextGroup("fewer than"));
    
    std::vector<math::resource::Image*> number_images;
    for (int num : number_est_ques) {
        number_images.push_back(math::number::getNumber(num));
    }
    
    if (num_groups == 1) {
        // Fix answer option
        if (answer == "more-less-about") {
            int index_ans = 0;
            if (number_est.front() - number_est_ques.front() > number_about) {
                index_ans = 1;
            } else if (number_est.front() - number_est_ques.front() < -number_about) {
                index_ans = 2;
            }
            for (int i=0; i<3; i++) {
                auto m_answer = mpool->getAnswer();
                std::string txt_ans = cocos2d::StringUtils::format("%s %s", compare_images[i]->getText().c_str(), number_images.front()->getText().c_str());
                m_answer->setEnable(txt_ans, math::common::answer, CC_CALLBACK_1(Minh_EstimateNearestNumber::onTouchedAnswer, this), getAnswerSize());
                if (i==index_ans) {
                    m_answer->setCorrect(true);
                }
                _answers.push_back(m_answer);
            }
        } else {
            int index_ans = 0;
            if (abs(number_est.front() - number_est_ques.front()) > number_about) {
                index_ans = 1;
            }
            for (int i=0; i<2; i++) {
                auto m_answer = mpool->getAnswer();
                std::string txt_ans = cocos2d::StringUtils::format("%s %s", compare_images.front()->getText().c_str(), number_images[i]->getText().c_str());
                m_answer->setEnable(txt_ans, math::common::answer, CC_CALLBACK_1(Minh_EstimateNearestNumber::onTouchedAnswer, this), getAnswerSize());
                if (i==index_ans) {
                    m_answer->setCorrect(true);
                }
                _answers.push_back(m_answer);
            }
        }
    }
    math::func::shuffle(_answers);
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image*img : number_images) {
                texts.push_back(img->getTextValue("name_1"));
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        else {
            getQuestion()->setOptionalTexts(key, list_icons);
        }
    }
}

void Minh_EstimateNearestNumber::onStart()
{
    float quiz_per = 1;
    if (num_groups == 1) {
        quiz_per = 0.75;
        onShowBottomBar();
        
        fixAnswerSize(quiz_size.width * 0.25, quiz_size.width * 0.2);
        float delta = (quiz_size.width - 40 - _answers.front()->getContentSize().width * (int)_answers.size()) / ((int)_answers.size() + 1);\
        if (delta > 100) {
            delta = 100;
        }
        math::func::smartSetPosition(_answers, _answers.front()->getContentSize(), cocos2d::Rect(20, 0, quiz_size.width - 20, quiz_size.height*0.25-30), delta);
    }
    quizBegan(0, true);
    
    math::func::smartSetPosition(_groups, group_size, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*quiz_per - getQuestion()->getContentSize().height-20), 50);
    
    for (int i = 0; i<num_groups; i++) {
        addChildNode(_groups[i]);
        _groups[i]->onShow(i*0.1, math::animation::ScaleUp);
    }
}

void Minh_EstimateNearestNumber::startQuestion()
{
    enableControlQuiz();
    // Start Question
    if (num_groups==1) {
        for (int i = 0; i < _answers.size(); ++i) {
            _answers[i]->onShowInParent(_panel_bot, 2, 0.1*i, math::animation::SlideUp);
        }
    }
}

void Minh_EstimateNearestNumber::onExplanation(bool correct)
{
    if (!correct) {
        disableControlQuiz(0.25);
    }
    else {
        disableControlQuiz();
        this->nextQuestion(0.75);
    }
}

void Minh_EstimateNearestNumber::onComplete()
{
    //TODO: Thuc hien them cac lenh o day
    if (num_groups==1) {
        for (int i = 0; i < _answers.size(); ++i) {
            _answers[i]->setDisable(0.1*i, math::animation::SlideUp);
        }
    }
    
    for (int i = 0; i<num_groups; i++) {
        _groups[i]->onHide(i*0.1 + 0.25, math::animation::ScaleUp);
    }
    
    quizEnded(0.5, true);
}


////
// Danh rieng cho game nay

std::vector<Math_Object*> Minh_EstimateNearestNumber::GenObjects(int num)
{
    std::vector<Math_Object*> objects;
    for (int i = 0; i < num; ++i) {
        Math_Object * object = Math_Pool::getInstance()->getObject();
        object->setEnable(obj_id++, math::common::TouchPermission::deny, obj_img, NULL);
        object->setObjectSize(object_size);
        objects.push_back(object);
    }
    
    return objects;
    
}
void Minh_EstimateNearestNumber::onTouchGroup(Node * node)
{
    Math_Group * group = (Math_Group *)node;
    bool correct = abs(group->getNumberObjects() - number_est_ques.front()) <= number_about;
    
    group->onTouchedAnimation(correct);
    this->onExplanation(correct);
}

#pragma mark - object touch events callback

void Minh_EstimateNearestNumber::onTouchedAnswer(cocos2d::Node * node)
{
    Math_Answer* m_answer = (Math_Answer*)node;
    bool done = m_answer->isCorrect();
    m_answer->onAnimationClick(done);
    this->onExplanation(done);
}


// #debug mode
#define auto_testing_delay 2
void Minh_EstimateNearestNumber::autoTesting()
{
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(3.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}


void Minh_EstimateNearestNumber::autoTestingRun(float dt)
{
    if (num_groups == 1) {
        Math_Answer* answer = math::func::getRandomNode(_answers);
        answer->onAutoClick();
    }
    else {
        Math_Group * group = math::func::getRandomNode(_groups);
        group->onTouchOnGroup();
    }
    autoTesting();
}



