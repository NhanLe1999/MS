//
//  MoveFruitToBagQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#include "MoveFruitToBagQuiz.h"

MoveFruitToBagQuiz* MoveFruitToBagQuiz::createQuiz()
{
    MoveFruitToBagQuiz* quiz=new MoveFruitToBagQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MoveFruitToBagQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void MoveFruitToBagQuiz::initQuiz(mj::model::Game game)
{
     
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    game_mode = getJsonString(json_game, "game_mode", "number");
    number_group = getJsonInt(json_game, "group", 2);
    int number_drag_min = getJsonInt(getJsonMember(json_game, "number_drag"), "minimum", 1);
    int number_drag_max = getJsonInt(getJsonMember(json_game, "number_drag"), "maximum", 3);
    check_move = getJsonBool(json_game, "check_move", false);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    bool kind_fit = getJsonBool(json_object, "kind", true);
    if (kind_fit) {
        number_kind = number_group;
    } else {
        number_kind = 1;
    }
    
    auto over_min = getJsonInt(getJsonMember(json_object, "over_icon"), "minimum", 1);
    auto over_max = getJsonInt(getJsonMember(json_object, "over_icon"), "maximum", 3);
    
    object_size = getSizeJson(json_object, "size");
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    rapidjson::Value& group_names=json_group.FindMember("name")->value;
    names_enable = group_names.FindMember("enable")->value.GetBool();
    std::vector<std::string> list_name;
    if (group_names.HasMember("value")) {
        rapidjson::Value& json_names=group_names.FindMember("value")->value;
        for (rapidjson::SizeType i=0; i<json_names.Size(); i++) {
            list_name.push_back(json_names[i].GetString());
        }
        math::func::shuffle(list_name);
        if (list_name.size() > 0) {
            if (list_name.front() == "people_name") {
                group_name_img = getPeopleNames(number_group);
            } if (list_name.front() == "color") {
                group_name_img = getColors();
                math::func::shuffle(group_name_img);
                while (group_name_img.size() > number_group) {
                    group_name_img.pop_back();
                }
            } else {
                auto random_name = randomArrayIndexes(number_group, (int)list_name.size());
                for (int i=0; i<number_group; i++) {
                    list_groupname.push_back(list_name[random_name[i]]);
                }
            }
        }
    }
    group_name_type = getJsonString(group_names, "name_type", "");
    group_size = getSizeJson(json_group, "size");
    
    //Random số lượng từng đối tượng
    group_cap = number_drag_max+over_max;
    auto random_no = randomArrayIndexes(number_group, number_drag_max-number_drag_min+1);
    for (int value : random_no) {
        list_number_drag.push_back(value+number_drag_min);
    }
    
    if (game_mode.compare("number")==0) {
        if (number_kind == 1) {
            int total = 0;
            for (int value : list_number_drag) {
                total+=value;
            }
            number_list.push_back(total + cocos2d::random(over_min, over_max));
        } else {
            for (int value : list_number_drag) {
                number_list.push_back(value + cocos2d::random(over_min, over_max));
            }
        }
    } else {
        for (int value : list_number_drag) {
            number_list.push_back(value + cocos2d::random(over_min, over_max));
        }
    }
    
    //TODO: Chon icon
    std::string icon_filter = getJsonString(getJsonMember(json_data, "icon"), "different", "id");
    std::string icon_same = getJsonString(getJsonMember(json_data, "icon"), "same", "none");
    
    for (int value : list_number_drag) {
        img_number_list.push_back(math::number::getNumber(value));
    }
    
    //Group name
    if (names_enable) {
        if (group_name_type.compare("exactly")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(list_name[i]);
            }
        } else if (group_name_type.compare("color")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(list_name[i]);
            }
        } else if (group_name_type.compare("number")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(cocos2d::StringUtils::format("%d", list_number_drag[i]));
            }
        } else if (group_name_type.compare("icon")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back("");
            }
        } else {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back("");
            }
        }
    }
    
    math::resource::Image* fruit_img = nullptr;
    math::resource::Image* tree_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/tree.png", m_theme->getThemePath().c_str()));
    math::resource::Image* tree_fg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/tree.png", m_theme->getThemePath().c_str()));
    math::resource::Image* tree_check = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/tree_touch.png", m_theme->getThemePath().c_str()));

    math::resource::Image* bag_bg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/bag_bg.png", m_theme->getThemePath().c_str()));
    math::resource::Image* bag_fg = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/bag.png", m_theme->getThemePath().c_str()));
    math::resource::Image* bag_check = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%sgameui/tree/bag_touch.png", m_theme->getThemePath().c_str()));

    img_list = getImageList("icons", 1, "none");
    fruit_img = img_list.front();
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object
    std::vector<Math_Object*> comparision_list;
    if (game_mode.compare("number")==0) {
        int obj_id=0;
        for (int i=0; i<number_kind; i++) {
            for (int j=0; j<number_list[i]; j++) {
                Math_Object* object=mpool->getObject();
                object->setEnable(obj_id, math::common::move, img_list[0], img_bg);
                object->setObjectSize(object_size);
                object->setMoveCallback(CC_CALLBACK_1(MoveFruitToBagQuiz::onTouchedObject, this), CC_CALLBACK_2(MoveFruitToBagQuiz::onMoveObjectEnded, this));
                _objects.push_back(object);
                obj_id++;
            }
        }
    } else {
        int obj_id=0;
        for (int i=0; i<number_group; i++) {
            int number_object_kind =  cocos2d::random(2, 4);
            for (int j=0; j<number_object_kind; j++) {
                Math_Object* object=mpool->getObject();
                object->setEnable(obj_id, math::common::move, img_list[0], img_bg);
                object->setObjectSize(object_size);
                object->setMoveCallback(CC_CALLBACK_1(MoveFruitToBagQuiz::onTouchedObject, this), CC_CALLBACK_2(MoveFruitToBagQuiz::onMoveObjectEnded, this));
                _objects.push_back(object);
                object->setComparisionValue("number", number_list[i]);
                object->setValueTextByKey("number");
                object->showValueTextWithAnimation(false);
                obj_id++;
            }
            comparision_list.push_back(_objects.back());
        }
    }

    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
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
        } else if (key == "group_name") {
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
    
    tree=mpool->getGroup();
    tree->setEnable(0, _objects, math::group::custom, number_object);
    tree->setObjectSize(object_size);
    tree->setGroupSize(cocos2d::Size(1274, 800));
    tree->setPosition(cocos2d::Vec2(quiz_size.width*0.5, quiz_size.height*0.57));
    tree->setBackground(tree_bg);
    tree->setForeground(tree_fg);
    tree->setCheckTouchImage(tree_check);
    tree->removeCustomPosition();
    
    //Gen group
    cocos2d::ui::ImageView* img_bag = cocos2d::ui::ImageView::create(bag_bg->path);
    group_size = img_bag->getVirtualRendererSize()*(group_size.width/img_bag->getVirtualRendererSize().width);
    
    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, math::group::custom, group_cap);
        group->setObjectSize(object_size);
        group->setGroupSize(group_size);
        group->setBackground(bag_bg);
        group->setForeground(bag_fg);
        group->setCheckTouchImage(bag_check);
        if (names_enable) {
            group->setGroupName(list_groupname[i]);
            group->showNameText(true);
            group->getNameText()->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            group->getNameText()->setPosition(cocos2d::Vec2(group->getContentSize().width*.5, group->getContentSize().height*0.3));
        }
        _groups.push_back(group);
        
        if (game_mode.compare("number")!=0) {
            group->setComparisionValue(comparision_list[i]->getComparisionValue());
            group->setComparisionKey(math::comparision::number);
        }
    }
    if (game_mode.compare("number")!=0) {
        math::func::shuffle(_groups);
    }
}

void MoveFruitToBagQuiz::onStart() {

    math_statistic->setCurrentGame(_game);
    std::string pop_out = cocos2d::StringUtils::format("movenumber/pop_out_%d.mp3", cocos2d::random(1, 2));
    audio_helper->play_sequence(new math::SequenceAudio(), pop_out, pop_out, "lalala.mp3");
    
    showQuestionBox(0);
    
    this->addChildNode(tree);
    tree->onShow();

    math::func::setPositionList(_groups, number_group, cocos2d::Rect(0, 0.05, 1, 0.15));
    //Show
    for (Math_Group* group : _groups) {
        this->addChildNode(group);
        group->onShow();
    }
}

void MoveFruitToBagQuiz::startQuestion() {
     
    //#1. add group
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        for (Math_Object* obj : _objects) {
            obj->enableDrag(_panel);
        }
    }
}

void MoveFruitToBagQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        if (game_mode.compare("number")==0) {
            for (int i=0 ; i < _groups.size(); i++) {
                Math_Group* group = _groups[i];
                if (group->getNumberObjects()!=list_number_drag[i]) {
                    while (group->getNumberObjects() > 0) {
                        auto object=group->popBackObject();
                        auto point = group->convertToWorldSpace(object->getPosition());
                        object->setPosition(point);
                        object->backToStart();
                        count_move--;
                    }
                }
            }
        } else {
            for (int i=0 ; i < _groups.size(); i++) {
                Math_Group* group = _groups[i];
                int index_obj=0;
                while (index_obj < group->getNumberObjects()) {
                    auto object = group->getObjectAtIndex(index_obj);
                    if (!group->isMatchWithGroup(object)) {
                        group->removeObject(object);
                        auto point = group->convertToWorldSpace(object->getPosition());
                        object->setPosition(point);
                        object->backToStart();
                        count_move--;
                    } else {
                        index_obj++;
                    }
                }
            }
        }
        enableControlQuiz();
    } else {
        for (int i=0 ; i < _groups.size(); i++) {
            Math_Group* group = _groups[i];
            for (int i=0; i<group->getNumberObjects(); i++) {
                group->getObjectAtIndex(i)->setLocked(true);
            }
        }
        
        disableControlQuiz();
        this->nextQuestion();
    }
}

void MoveFruitToBagQuiz::onComplete() {
     
    //TODO: Thuc hien them cac lenh o day
    float delay = showConratulation(0.3);
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }

    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }
    scheduleOnce([=](float dt) {
        for (Math_Group* group : _groups) {
            group->setDisable();
        }
    }, m_delay, "hide_groups");
    
    m_delay= delay-0.3;
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
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - object touch events callback
void MoveFruitToBagQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object=(Math_Object*)node;
    //*
    //Chon object dau tien cham phai
    if (selected_object==NULL) {
        selected_object=object;
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void MoveFruitToBagQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
    Math_Object* object = (Math_Object*)node;
    
    //remove object khoi group hien tai
    Math_Group* gr_source = NULL;
    for (Math_Group* _group : _groups) {
        if (_group->hasObject(object)) {
            gr_source = _group;
            _group->removeObject(object, true);
            count_move--;
            break;
        }
    }
    
    Math_Group* group=NULL;
    for (Math_Group* _group : _groups) {
        if (_group->isTouchOnGroup(point)) {
            group=_group;
            break;
        }
    }
    
    if (group!=NULL) {
        bool can_add=true;
        if (can_add) {
            if (group->addObject(object, point)) {
                //Check answer
                selected_object=NULL;
                count_move++;
                if (tree->getNumberObjects()==count_move) {
                    onAnswerDone(nullptr);
                }
                return;
            }
        }
    }
    
    object->backToStart();
    selected_object=NULL;
}


void MoveFruitToBagQuiz::onAnswerDone(Node* node)
{
    disableControlQuiz();
    bool is_correct=true;
    if (game_mode.compare("number")==0) {
        for (int i=0; i<number_group; i++) {
            Math_Group* group=_groups[i];
            if (group->getNumberObjects()!=list_number_drag[i]) {
                is_correct=false;
                break;
            }
        }
    } else {
        for (int i=0; i<number_group; i++) {
            Math_Group* group=_groups[i];
            if (!group->isMatchGroup()) {
                is_correct=false;
                break;
            }
        }
    }
    scheduleOnce([=](float){
        this->onExplanation(is_correct);
    }, 1.0, "done");
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MoveFruitToBagQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MoveFruitToBagQuiz::autoTestingRun(float delaytime)
{
}

