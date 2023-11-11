//
//  MoveNumberObjectToGroupQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#include "MoveNumberObjectToGroupQuiz.h"

MoveNumberObjectToGroupQuiz* MoveNumberObjectToGroupQuiz::createQuiz()
{
    MoveNumberObjectToGroupQuiz* quiz=new MoveNumberObjectToGroupQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool MoveNumberObjectToGroupQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void MoveNumberObjectToGroupQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    copy_object = getJsonBool(json_game, "copy", false);

    total_number = randomRangeJson(json_game, "number_total");
    number_group = getJsonInt(json_game, "group", 1);
    check_move = getJsonBool(json_game, "check_move", false);
    drag_allinone = getJsonBool(json_game, "drag_type", true);
    use_first = getJsonBool(json_game, "use_first", false);
    
    int number_drag_min = getJsonInt(getJsonMember(json_game, "number_drag"), "minimum", 1);
    int number_drag_max = getJsonInt(getJsonMember(json_game, "number_drag"), "maximum", 2);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    bool kind_fit = getJsonBool(json_object, "kind", false);
    if (kind_fit) {
        number_kind = number_group;
        if (copy_object) {
            number_kind = number_group + 2;
            number_kind = number_kind > 4 ? 4 : number_kind;
        }
    } else {
        number_kind = 1;
    }
    object_size = getSizeJson(json_object, "size");
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group = json_data.FindMember("group")->value;
    rapidjson::Value& group_names = json_group.FindMember("name")->value;
    names_enable = getJsonBool(group_names, "enable", false);

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
                    list_groupname.push_back(getTextByTextGroup(list_name[random_name[i]]));
                }
            }
        }
    }
    group_name_type = getJsonString(group_names, "name_type", "exactly");
    
    object_align = getJsonString(json_group, "align");
    group_type = getJsonString(json_group, "type", "queue");
    group_column = getJsonInt(getJsonMember(json_group, "frame"), "column", 5);
    group_row = getJsonInt(getJsonMember(json_group, "frame"), "row", 5);
    group_size = getSizeJson(json_group, "size");
    
    //Random số lượng từng đối tượng
    group_cap = total_number;
    if (group_type != "custom") {
        group_cap = group_row*group_column;
    }
    
    int fix_number = total_number;
    int fix_group = number_group > number_kind ? number_group : number_kind;
    if (copy_object) {
        fix_number = number_drag_max * number_group;
    }
    int total_icon = fix_number+1;
    if (copy_object) {
        total_icon=0;
        for (int i=0; i<fix_group; i++) {
            int value = cocos2d::random(number_drag_min, number_drag_max);
            total_icon+=value;
            list_number_drag.push_back(value);
        }
    } else {
        while (total_icon > fix_number) {
            total_icon = 0;
            list_number_drag.clear();
            for (int i=0; i<fix_group; i++) {
                int value = cocos2d::random(number_drag_min, number_drag_max);
                total_icon+=value;
                list_number_drag.push_back(value);
            }
        }
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    rapidjson::Value& icon_json = getJsonMember(json_data, "icon");
    
    std::string icon_diff = getJsonString(icon_json, "different", "none");
    std::string icon_same = getJsonString(icon_json, "same", "none");
    if (!copy_object) {
        icon_diff = "none";
        icon_same = "none";
    }
    
    auto all_images = getImageList("icons");
    if (use_first) {
        img_list.push_back(all_images.front());
        while (img_list.size() < number_kind) {
            auto img = math::func::getRandomNode(all_images);
            bool avail = true;
            for (math::resource::Image* image : img_list) {
                if (icon_diff == "id") {
                    if (image->code == img->code) {
                        avail = false;
                        break;
                    }
                } else {
                    if (img->getText(icon_diff) == image->getText(icon_diff)) {
                        avail = false;
                        break;
                    }
                }
            }
            if (avail) {
                img_list.push_back(img);
            }
        }
    } else {
        img_list = getImageList(all_images, number_kind, icon_diff, icon_same);
    }
    
    if (!kind_fit) {
        while (img_list.size() < number_group) {
            img_list.push_back(img_list.front());
        }
    }

    for (int value : list_number_drag) {
        img_number_list.push_back(math::number::getNumber(value)); 
    }
    
    //Group name
    bool group_color_bg = false;
    if (names_enable) {
        if (group_name_type.compare("exactly")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(getTextByTextGroup(list_name[i]));
            }
        } else if (group_name_type.compare("color")==0) {
            group_color_bg = true;
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(getColor(list_name[i]));
            }
        } else if (group_name_type.compare("number")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back(getNumbers(list_number_drag[i], list_number_drag[i]).front());
            }
        } else if (group_name_type.compare("icon")==0) {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back((getTextByTextGroup(list_name[i])));
            }
        } else {
            for (int i=0; i<number_group; i++) {
                list_groupname.push_back((getTextByTextGroup(list_name[i])));
            }
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    if (copy_object) {
        for (int i=0; i<number_kind; i++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(i, math::common::move, img_list[i], img_bg);
            object->setObjectSize(object_size);
            object->setMoveCallback(CC_CALLBACK_1(MoveNumberObjectToGroupQuiz::onTouchedObject, this), CC_CALLBACK_2(MoveNumberObjectToGroupQuiz::onMoveObjectEnded, this));
            _objects.push_back(object);
        }
    } else {
        for (int i=0; i<total_number; i++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(i, math::common::move, img_list[0], img_bg);
            object->setObjectSize(object_size);
            object->setMoveCallback(CC_CALLBACK_1(MoveNumberObjectToGroupQuiz::onTouchedObject, this), CC_CALLBACK_2(MoveNumberObjectToGroupQuiz::onMoveObjectEnded, this));
            _objects.push_back(object);
        }
    }
    
    std::vector<math::resource::Image*> group_bgs;
    if (group_name_type.compare("color")==0 && !list_groupname.empty()) {
        for (int i=0; i<number_group; i++) {
            auto bg_img = groupBG(list_groupname[i]->getText());
            group_bgs.push_back(bg_img);
        }
    } else {
        math::resource::Image* group_bg = groupBG();
        for (int i=0; i<number_group; i++) {
            group_bgs.push_back(group_bg);
        }
    }

    //Gen group
    math::group::GroupType gr_type=math::group::custom;
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
        group_size = cocos2d::Size((group_column * 1.2 + 0.6) * object_size.width, (group_row * 1.2 + 0.6) * object_size.height);
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
    }
    math::resource::Image* group_frame = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));

    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        if (gr_type==math::group::frame) {
            group->setGroupFrames(group_column, group_row, group_frame);
        } else if (gr_type==math::group::queue) {
            group->setGroupQueue(group_size);
            group->setBackground(group_bgs[i], 1);
        } else {
            group->setGroupSize(group_size);
            group->setBackground(group_bgs[i], 1);
        }
        if (names_enable) {
            group->setGroupName(list_groupname[i]->getText());
            group->showNameText(true);
            if (group_color_bg) {
                group->getNameText()->setTextColor(getColor4B(list_groupname[i]->getText()));
            }
        }
        if (copy_object) {
            group->setComparisionKey(math::comparision::exactly);
            if (number_kind == 1) {
                group->setComparisionValue(_objects[0]->getComparisionValue());
            } else {
                group->setComparisionValue(_objects[i]->getComparisionValue());
            }
        }
        _groups.push_back(group);
    }
    
    if (drag_allinone) {
        parseQuestion();
    } else {
        parseQuestion(true, number_group);
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
        } else if (key == "group_name") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : list_groupname) {
                auto img_txt = img->getTextValue("name_1");
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
            
        } else if (key == "name_n") {
            std::vector<math::resource::Text> texts;
            for (int i=0; i<number_group; i++) {
                if (img_number_list[i]->getComparisionValue("number")==1) {
                    auto img_txt = img_list[i]->getTextValue("name_1");
                    texts.push_back(img_txt);
                } else {
                    auto img_txt = img_list[i]->getTextValue("name_n");
                    texts.push_back(img_txt);
                }
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
}

void MoveNumberObjectToGroupQuiz::onStart() {
    startGuide();

    int number_icon = (int)_objects.size();
    if (number_icon > 10) {
        onShowBottomBar(0, 0.4);
        quizBegan(0.5, false);
        math::func::shuffle(_objects);
        math::func::setPositions(_objects, cocos2d::Rect(30, 0, quiz_size.width-60, quiz_size.height * 0.4 - 30));
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->fitObjectSize();
            addChildNodeToBottomPanel(object, 2);
        }
        randomShow(_objects, 0, math::animation::SlideUp);
        
        math::func::shuffle(_groups);
        math::func::setPositions(_groups, cocos2d::Rect(0, quiz_size.height*0.4, quiz_size.width, quiz_size.height * 0.6));
        for (int i=0; i<_groups.size(); i++) {
            addChildNode(_groups[i]);
            _groups[i]->onShow();
        }
    } else {
        float bar_size_w = 300;
        float delta = 50;
        if (number_icon > 4) {
            bar_size_w = 350;
            delta = 40;
        }
        float bar_w = bar_size_w / quiz_size.width;
        onShowRightBar(0, bar_w);
        quizBegan(0.5, false);
        math::func::shuffle(_objects);
        math::func::setPositions(_objects, cocos2d::Rect(40, 30, bar_size_w-80, quiz_size.height - 60));
        
        for (int i=0; i < _objects.size(); i++) {
            Math_Object* object = _objects[i];
            object->fitObjectSize();
            addChildNodeToRightPanel(object, 2);
        }
        randomShow(_objects, 0, math::animation::SlideLeft);
        
        math::func::shuffle(_groups);
        math::func::setPositions(_groups, cocos2d::Rect(0, 0, quiz_size.width - bar_size_w, quiz_size.height));

        for (int i=0; i<_groups.size(); i++) {
            addChildNode(_groups[i]);
            _groups[i]->onShow();
        }
    }
}

void MoveNumberObjectToGroupQuiz::startQuestion() {
    enableControlQuiz();
    if (questionIndex()==0) {
        int number_icon = (int)_objects.size();
        if (number_icon > 10) {
            for (Math_Object* obj : _objects) {
                obj->enableDrag(_panel_bot);
            }
        } else {
            for (Math_Object* obj : _objects) {
                obj->enableDrag(_panel_right);
            }
        }
    }
}

void MoveNumberObjectToGroupQuiz::onExplanation(bool correct) {    
    if (!correct) {
        disableControlQuiz(0.25);
        
        if (copy_object) {
            
        } else {
            audio_helper->play("audio_moveback.mp3");
        }
        speakQuestion(true);
        
        for (int i=0 ; i < _groups.size(); i++) {
            Math_Group* group = _groups[i];
            if (group->getNumberObjects()!=list_number_drag[i]) {
                while (group->getNumberObjects() > 0) {
                    auto point = group->getObjectAtIndex(group->getNumberObjects()-1)->getWorldPosition();
                    auto object = group->popBackObject();
                    if (copy_object) {
                        for (int i=0; i<_objects.size(); i++) {
                            if (_objects[i]->isSame(object)) {
                                _objects.erase(_objects.begin()+i);
                                break;
                            }
                        }
                        object->setPosition(point);
                        addChildNode(object, 10);
                        object->onHide(0, math::animation::ScaleUpAndRotation);
                    } else {
                        auto point = group->convertToWorldSpace(object->getPosition());
                        object->setPosition(point);
                        object->backToStart();
                    }
                }
            }
        }
    } else {
        disableControlQuiz();
        for (int i=0 ; i < _groups.size(); i++) {
            Math_Group* group = _groups[i];
            for (int i=0; i<group->getNumberObjects(); i++) {
                group->getObjectAtIndex(i)->setLocked(true);
            }
        }
        nextQuestion(1.0);
    }
}

void MoveNumberObjectToGroupQuiz::audioEnding(float dt) {
    if (index_group < (int)_groups.size()) {
        std::vector<std::string> audios;
        audios.push_back(img_number_list[index_group]->getAudio());
        if (img_number_list[index_group]->getComparisionValue("number") < 2) {
            audios.push_back(img_list.front()->getAudio());
        } else {
            audios.push_back(img_list.front()->getAudio("name_n"));
        }
        audio_helper->play_sequence(audios, nullptr, "");
        index_group++;
        scheduleOnce([=](float){
            this->audioEnding(dt);
        }, dt, cocos2d::StringUtils::format("audioEnding %d", index_group));
    } else {
        disableControlQuiz();
        this->nextQuestion();
    }
}

void MoveNumberObjectToGroupQuiz::onComplete() {
    for (Math_Group* group : _groups) {
        group->setDisable(0);
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
    quizEnded(0.5, true);
}

void MoveNumberObjectToGroupQuiz::onGuideStarted() {
    disableControlQuiz();
    
    Math_Group* group = nullptr;
    
    if (drag_allinone) {
        for (int i=0; i<number_group; i++) {
            Math_Group* gr = _groups[i];
            if (gr->getNumberObjects()!=list_number_drag[i]) {
                group = gr;
                break;
            }
        }
    } else {
        int index = questionIndex();
        for (Math_Group* gr : _groups) {
            if (gr->getId() == index) {
                group = gr;
                break;
            }
        }
    }
    if (group) {
        Math_Object* object = nullptr;
        for (Math_Object* obj : _objects) {
            bool unused = true;
            for (Math_Group* gr : _groups) {
                if (gr->hasObject(obj)) {
                    unused = false;
                    break;
                }
            }
            if (unused) {
                if ((copy_object && group->isMatchWithGroup(obj)) || !copy_object) {
                    object = obj;
                    break;
                }
            }
        }
        if (object) {
            GuideCenter->guideByDragDrop(object->getWorldPosition(), group->getWorldPosition());
        } else {
            GuideCenter->guideNone();
        }
    } else {
        GuideCenter->guideNone();
    }
}

void MoveNumberObjectToGroupQuiz::onGuideDone() {
    enableControlQuiz();
}

#pragma mark - object touch events callback
void MoveNumberObjectToGroupQuiz::onTouchedObject(cocos2d::Node* node)
{
    GuideCenter->stopGuide();
    Math_Object* object=(Math_Object*)node;
    //Chon object dau tien cham phai
    if (selected_object==NULL) {
        selected_object=object;
        audio_helper->play("audio_drag.mp3");
        
        Math_Group* gr_source = NULL;
        for (Math_Group* _group : _groups) {
            if (_group->hasObject(object)) {
                gr_source = _group;
                break;
            }
        }
        if (gr_source!=NULL) {
            gr_source->removeObject(object, true);
        }
        
        if (copy_object) {
            //Kiem tra object o dock hay o group
            copying = gr_source==NULL;
            if (copying) {
                for (int i=0; i<_objects.size(); i++) {
                    if (_objects[i]->isSame(object)) {
                        _objects.erase(_objects.begin()+i);
                        break;
                    }
                }
                Math_Object* object_copy = object->clone();
                object_copy->onShow(0, math::animation::No_Effect);
                _objects.push_back(object_copy);
            }
        }
        
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void MoveNumberObjectToGroupQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
    startGuide();
    Math_Object* object = (Math_Object*)node;
    point = _panel->convertToNodeSpace(point);
    Math_Group* group=NULL;
    for (Math_Group* _group : _groups) {
        if (_group->isTouchOnGroup(point)) {
            group=_group;
            break;
        }
    }
    
    if (group!=NULL) {
        bool can_add=true;
        if (group->getId() == this->getQuestion()->getData().index && copy_object) {
            can_add = group->isMatchWithGroup(object);
        }
        if (!drag_allinone) {
            if (group->getId() != this->getQuestion()->getData().index) {
                can_add = false;
            }
        }
        if (check_move && can_add) {
            can_add = group->isMatchGroupWithOther();
        }
        if (can_add) {
            can_add = group->addObject(object, point);
        }
        if (can_add) {
            object->onDropObject(0.1);
            GuideCenter->resetIncorrectCount();
        } else {
            GuideCenter->increamentIncorrect();
            audio_helper->play("audio_moveback.mp3");
            if (copy_object) {
                object->onHide(0, math::animation::ScaleUpAndRotation);
            } else {
                object->backToStart();
            }
        }
    } else {
        GuideCenter->increamentIncorrect();
        audio_helper->play("audio_moveback.mp3");
        if (copy_object) {
            object->onHide(0, math::animation::ScaleUpAndRotation);
        } else {
            object->backToStart();
        }
    }
    selected_object=NULL;
    
    //check done
    bool is_correct=true;
    if (drag_allinone) {
        for (int i=0; i<number_group; i++) {
            Math_Group* group = nullptr;
            for (Math_Group* gr : _groups) {
                if (gr->getId() == i) {
                    group = gr;
                    break;
                }
            }
            if (group->getNumberObjects()!=list_number_drag[i]) {
                is_correct=false;
                break;
            }
        }
    } else {
        int index = questionIndex();
        Math_Group * group = nullptr;
        for (Math_Group* gr : _groups) {
            if (gr->getId() == index) {
                group = gr;
                break;
            }
        }
        if (group->getNumberObjects()!=list_number_drag[index]) {
            is_correct=false;
        }
    }
    if (is_correct) {
        onExplanation(true);
    }
}


void MoveNumberObjectToGroupQuiz::onAnswerDone(Node* node)
{
    disableControlQuiz(1);
    Math_Answer* answer = (Math_Answer*)node;
    bool is_correct=true;
    if (drag_allinone) {
        for (int i=0; i<number_group; i++) {
            Math_Group* group = nullptr;
            for (Math_Group* gr : _groups) {
                if (gr->getId() == i) {
                    group = gr;
                    break;
                }
            }
            if (group->getNumberObjects()!=list_number_drag[i]) {
                is_correct=false;
                break;
            }
        }
    } else {
        int index = this->getQuestion()->getData().index;
        Math_Group * group = nullptr;
        for (Math_Group* gr : _groups) {
            if (gr->getId() == index) {
                group = gr;
                break;
            }
        }
        if (group->getNumberObjects()!=list_number_drag[index]) {
            is_correct=false;
        }
    }
    answer->onAnimationClick(is_correct);
    scheduleOnce([=](float){
        this->onExplanation(is_correct);
    }, 0.5, "wait explain");
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void MoveNumberObjectToGroupQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void MoveNumberObjectToGroupQuiz::autoTestingRun(float delaytime)
{
}

