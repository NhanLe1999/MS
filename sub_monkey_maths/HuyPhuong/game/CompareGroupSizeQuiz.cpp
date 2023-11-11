//
//  CompareGroupSizeQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#include "CompareGroupSizeQuiz.h"

CompareGroupSizeQuiz* CompareGroupSizeQuiz::createQuiz()
{
    CompareGroupSizeQuiz* quiz=new CompareGroupSizeQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool CompareGroupSizeQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void CompareGroupSizeQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);

    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    number_group = getJsonInt(json_game, "group", 2);
    align = getJsonBool(json_game, "align", true);
    reorder = getJsonBool(json_game, "reorder", true);
    game_type = getJsonString(json_game, "type", "choose");
    gr_same_kind = getJsonBool(json_game, "group_same_kind", true);
    
    rapidjson::Value& relation_list = json_game.FindMember("relation")->value;
    if (relation_list.Empty()) {
        relation = "equal";
    } else {
        relation = relation_list[cocos2d::random(0, (int)relation_list.Size()-1)].GetString();
    }
    
    if (relation == "equal") {
        relation_type = RelationType::Equal;
    } else if (relation == "more") {
        relation_type = RelationType::More;
    } else if (relation == "less") {
        relation_type = RelationType::Less;
    } else {
        relation_type = RelationType::Enough;
    }
    
    if (game_type == "yes_no") {
        gameplay_type = GamePlayType::Yes_No;
    } else if (game_type == "answer") {
        gameplay_type = GamePlayType::Answer;
    } else {
        gameplay_type = GamePlayType::Choose;
    }
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max=json_object.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    auto number_min=json_object.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    object_size = getSizeJson(json_object, "size");
    group_cap=number_max*number_group;
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    rapidjson::Value& group_names=json_group.FindMember("name")->value;
    std::vector<std::string> list_name;
    for (rapidjson::SizeType i=0; i<group_names.Size(); i++) {
        list_name.push_back(group_names[i].GetString());
    }
    kind_in_group = randomRangeJson(json_group, "kind");
    object_align = getJsonString(json_group, "align", "left");
    object_arrange = getJsonBool(json_group, "arrange", false);
    sort_type = getJsonString(json_group, "sort", "kind");
    group_type = getJsonString(json_group, "type", "queue");
    group_column = json_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
    group_row = json_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
    group_size = getSizeJson(json_group, "size");
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    std::string icon_filter = json_data.FindMember("icon")->value.FindMember("different")->value.GetString();
    std::string icon_same = json_data.FindMember("icon")->value.FindMember("same")->value.GetString();

    auto all_imgs = getImageList("icons");
    int kind_img = kind_in_group;
    if (!gr_same_kind) {
        kind_img *=2;
    }
    if (relation_type == RelationType::Enough) {
        img_list = getImageList(all_imgs, 2, icon_filter, icon_same);

        std::vector<math::resource::Image*> temp_list;
        int index = 0;
        while (temp_list.size()<2) {
            for (math::resource::Image* img : img_list) {
                if (index==0) {
                    if (img->getComparisionValue("role") == 0) {
                        temp_list.push_back(img);
                        index++;
                        break;
                    }
                } else {
                    if (img->getComparisionValue("role") == 1) {
                        temp_list.push_back(img);
                        index++;
                        break;
                    }
                }
            }
        }
        img_list.clear();
        for (math::resource::Image* img : temp_list) {
            img_list.push_back(img);
        }
    } else {
        img_list = getImageList(all_imgs, kind_img, icon_filter, icon_same);
    }
        
    //Group name:
    if (list_name.size()>0) {
        //Group name:
        if (list_name.size()>0) {
            if (list_name.front() == "people_name") {
                group_name_img = getPeopleNames(number_group);
                for (math::resource::Image* img : group_name_img) {
                    img->used = true;
                }
            } if (list_name.front() == "color") {
                group_name_img = getColors();
                math::func::shuffle(group_name_img);
                while (group_name_img.size() > number_group) {
                    group_name_img.pop_back();
                }
                for (math::resource::Image* img : group_name_img) {
                    img->used = true;
                }
            } else {
                auto random_name = randomArrayIndexes(number_group, (int)list_name.size());
                for (int i=0; i<number_group; i++) {
                    list_groupname.push_back(list_name[random_name[i]]);
                }
            }
        }
    }
    
    std::vector<int> total_numbers;
    switch (relation_type) {
        case RelationType::Equal :
            {
                //Cách chơi: Chọn yes/no: group a có số lượng bằng (khác) group B k?
                index_answer = cocos2d::random(0, 1);
                if (index_answer==0) {
                    //Choose true
                    int number=cocos2d::random(number_min, number_max);
                    for (int i=0; i<number_group; i++) {
                        total_numbers.push_back(number);
                    }
                } else {
                    //choose false
                    auto rand_numbers=randomArrayIndexes(number_group, number_max-number_min+1);
                    for (int i=0; i<number_group; i++) {
                        total_numbers.push_back(rand_numbers[i] + number_min);
                    }
                }
            }
            break;
        case RelationType::More :
        {
            //Cách chơi: touch group: Chọn group có số lượng lớn/bé hơn.
            //cách chơi: chọn đáp án: Group nào có số lượng lớn hơn, A or B?
            index_answer=0;
            auto rand_numbers=randomArrayIndexes(number_group, number_max-number_min+1);
            for (int i=0; i<number_group; i++) {
                total_numbers.push_back(rand_numbers[i] + number_min);
            }
            std::sort(total_numbers.begin(), total_numbers.end(), [](int a, int b)->bool{return a>b;});
        }
            break;
        case RelationType::Less :
        {
            //Cách chơi: touch group: Chọn group có số lượng lớn/bé hơn.
            //cách chơi: chọn đáp án: Group nào có số lượng lớn hơn, A or B?
            index_answer=0;
            auto rand_numbers=randomArrayIndexes(number_group, number_max-number_min+1);
            for (int i=0; i<number_group; i++) {
                total_numbers.push_back(rand_numbers[i] + number_min);
            }
            std::sort(total_numbers.begin(), total_numbers.end(), [](int a, int b)->bool{return a<b;});

        }
            break;
        default:
        {
            //enough
            //Cách chơi: Chọn Yes/no: Group A có đủ cho mỗi obj trong B k?
            index_answer = cocos2d::random(0, 1);
            if (index_answer==0) {
                int _val = cocos2d::random(number_min, number_max);
                total_numbers.push_back(_val);
                total_numbers.push_back(_val);
            } else {
                do {
                    int _val_1 = cocos2d::random(number_min, number_max);
                    int _val_2 = cocos2d::random(number_min, number_max);
                    if (_val_1!=_val_2) {
                        total_numbers.push_back(_val_1);
                        total_numbers.push_back(_val_2);
                    }
                } while (total_numbers.size() < 2);
                std::sort(total_numbers.begin(), total_numbers.end(), [](int a, int b)->bool{return a<b;});
            }
        }
            break;
    }

    for (int i=0; i<number_group; i++) {
        int _remain = total_numbers[i];
        if (kind_in_group==1) {
            number_list.push_back(_remain);
        } else {
            int _div = _remain/kind_in_group;
            int _mod = _remain%kind_in_group;
            for (int i=0; i<kind_in_group; i++) {
                if (i<_mod) {
                    number_list.push_back(_div+1);
                } else {
                    number_list.push_back(_div);
                }
            }
        }
    }
    
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "group_name") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : group_name_img) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    getQuestion()->setKeywordValue((int)relation_type);
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object & group
    math::resource::Image* group_bg = groupBG();

    math::group::GroupType gr_type=math::group::custom;
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
        group_cap = group_column * group_row;
        group_size = cocos2d::Size((group_column * 1.2 + 0.6) * object_size.width, (group_row * 1.2 + 0.6) * object_size.height);
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
        group_cap = group_column * group_row;
        group_size = cocos2d::Size((group_column * 1.2 + 0.6) * object_size.width, (group_row * 1.2 + 0.6) * object_size.height);
    }
    
    int obj_id=0;
    int number_id=0;
    int img_id=-1;
    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        for (int i2=0; i2<kind_in_group; i2++) {
            if (gr_same_kind) {
                img_id=i2;
            } else {
                img_id++;
            }
            
            math::resource::Image* img = img_list[img_id];
            if (relation_type == RelationType::Enough) {
                img = img_list[i];
            }
            
            for (int i3=0; i3<number_list[number_id]; i3++) {
                Math_Object* object=mpool->getObject();
                object->setEnable(obj_id, math::common::deny, img, nullptr);
                object->setObjectSize(object_size);
                _objects.push_back(object);
                _group_list.push_back(object);
                obj_id++;
            }
            number_id++;
        }
        if (sort_type.compare("mix")==0) {
            math::func::shuffle(_group_list);
        }
        
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        if (gameplay_type == GamePlayType::Choose) {
            group->enableToch(CC_CALLBACK_1(CompareGroupSizeQuiz::onTouchedGroup, this));
        }
        if (gr_type==math::group::frame) {
            group->setGroupFrames(group_column, group_row, group_bg);
        } else if (gr_type == math::group::custom) {
            group->setGroupSize(group_size);
            group->setBackground(group_bg, 0.3);
        } else {
            group->setGroupQueue(group_size);
            group->setBackground(group_bg, 0.3);
        }
        if (list_groupname.size()>0) {
            group->setGroupName(list_groupname[i]);
            group->showNameText(true);
        }
        _groups.push_back(group);
    }
    
    if (gameplay_type == GamePlayType::Yes_No) {
        std::vector<math::resource::Image*> list_answer;
        list_answer.push_back(getTextByTextGroup("yes"));
        list_answer.push_back(getTextByTextGroup("no"));
        for (int i=0; i<2; i++) {
            Math_Answer* answer = mpool->getAnswer();
            answer->setEnable("", math::common::answer, CC_CALLBACK_1(CompareGroupSizeQuiz::onAnswered, this), getAnswerSize(2));
            answer->setIcon(list_answer[i]);
            answer->setTitleWithIcon("name_1");
            answer->setCorrect(i==index_answer);
            _answers.push_back(answer);
        }
        math::func::shuffle(_answers);
    } else if (gameplay_type == GamePlayType::Answer) {
        //Thêm thông tin định nghĩa đáp án. Để tạm là group name.
        std::vector<std::string> list_answer=list_groupname;
        for (int i=0; i<list_answer.size(); i++) {
            Math_Answer* answer = mpool->getAnswer();
            answer->setEnable(list_answer[i], math::common::answer, CC_CALLBACK_1(CompareGroupSizeQuiz::onAnswered, this), getAnswerSize(2));
            answer->setCorrect(i==index_answer);
            _answers.push_back(answer);
        }
        math::func::shuffle(_answers);
    }
}

void CompareGroupSizeQuiz::onStart() {

    math::func::shuffle(_groups);
    cocos2d::Rect rect_box = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height);
    if (gameplay_type == Choose) {
        Math_Quiz::onStart();
        rect_box = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height - getQuestion()->getContentSize().height-30);
    } else {
        onShowBottomBar();
        Math_Quiz::onStart();
        rect_box = cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height*0.75 - getQuestion()->getContentSize().height-30);
    }
    
    float delta = (quiz_size.width - group_size.width * number_group) / (number_group + 1);
    delta = delta > 100? 100 : delta;
    math::func::smartSetPosition(_groups, group_size, rect_box, delta);
    for (Math_Group* group : _groups) {
        this->addChildNode(group);
        group->onShow();
    }
    
    if (!_answers.empty()) {
        int number_ans = (int) _answers.size();
        auto answer_size = _answers.front()->getContentSize();
        float delta = (quiz_size.width * 0.9 - number_ans * answer_size.width) / (number_ans + 1);
        delta = delta > 120 ? 120 : delta;
        math::func::smartSetPosition(_answers, answer_size, cocos2d::Rect(quiz_size.width * 0.05, 0, quiz_size.width*0.9, quiz_size.height * 0.25 - 30), delta);
    }
}

void CompareGroupSizeQuiz::startQuestion() {
    //#2. add btn done
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (Math_Answer* answer : _answers) {
            answer->onShowInParent(_panel_bot, 2);
        }
    }
}

void CompareGroupSizeQuiz::onExplanation(bool correct) {
    if (!correct) {
        
    } else {
        disableControlQuiz();
        scheduleOnce([=](float dt) {
            this->nextQuestion();
        }, 1.0, "next question");
    }
}

void CompareGroupSizeQuiz::onComplete() {
    for (int i=0; i<_answers.size(); i++) {
        _answers[i]->setDisable(0.5 + i*0.1);
    }
    for (int i=0; i<_groups.size(); i++) {
        _groups[i]->setDisable(i*0.1);
    }
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.0, "complete_quiz");
}

#pragma mark - object touch events callback
void CompareGroupSizeQuiz::onAnswered(Node* node) {
    disableControlQuiz(0.25);
    Math_Answer* answer = (Math_Answer*)node;
    bool correct = answer->isCorrect();
    answer->onAnimationClick(correct);
    this->onExplanation(correct);
}

void CompareGroupSizeQuiz::onTouchedGroup(Node* node) {
    Math_Group* group = (Math_Group*)node;
    bool correct = group->getId()==index_answer;
    
    group->onTouchedAnimation(correct);
    this->onExplanation(correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void CompareGroupSizeQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void CompareGroupSizeQuiz::autoTestingRun(float delaytime) {
    if (game_type.compare("choose")==0) {
        //Choose one group
        Math_Group* group=math::func::getRandomNode(_groups);
        while (group->getId() == last_index) {
            group=math::func::getRandomNode(_groups);
        }
        last_index=group->getId();
        group->onTouchOnGroup();
        
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

