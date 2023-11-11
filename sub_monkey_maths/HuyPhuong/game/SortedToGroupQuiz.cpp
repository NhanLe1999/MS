//
//  SortedToGroupQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#include "SortedToGroupQuiz.h"

SortedToGroupQuiz* SortedToGroupQuiz::createQuiz()
{
    SortedToGroupQuiz* quiz=new SortedToGroupQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return nullptr;
}

bool SortedToGroupQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void SortedToGroupQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_group = getJsonInt(json_game, "group", 2);
    sort_by = getJsonString(json_game, "sort", "id");
    sorted = getJsonString(json_game, "sorted", "none");
    align_group_object = getJsonString(json_game, "align", "left_right");
    remain_object = getJsonString(json_game, "remain", "fit_quantity");
    dinamic_match = getJsonBool(json_game, "dinamic_match", false);
    sort_key = getComparisionKey(sort_by);
    only_correct_move = true;// getJsonBool(json_game, "only_correct_move", true);
    position_name = getJsonString(json_game, "name_position", "top");
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 2);
    auto number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 2);
    object_size = getSizeJson(json_object, "size");
    
    group_cap = number_max * 2;
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    rapidjson::Value& group_names=json_group.FindMember("name")->value;
    std::vector<std::string> list_name;
    list_name.clear();
    /*for (rapidjson::SizeType i=0; i<group_names.Size(); i++) {
        list_name.push_back(group_names[i].GetString());
    }*/
    object_align=json_group.FindMember("align")->value.GetString();
    group_type=json_group.FindMember("type")->value.GetString();
    group_column=json_group.FindMember("frame")->value.FindMember("column")->value.GetInt();
    group_row=json_group.FindMember("frame")->value.FindMember("row")->value.GetInt();
    group_size=cocos2d::Size(json_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("size")->value.FindMember("height")->value.GetInt());
    
    //Random số lượng từng đối tượng
    number_object=0;
    number_kind=number_group;
    if (sort_by == "tens") {
        remain_object = "fit_quantity";
        only_correct_move = false;
        dinamic_match = false;
        sorted = "none";
    }
    
    if (remain_object.compare("fit_quantity")!=0) {
        number_kind+=1;
        dinamic_match=true;
    }
    
    int minimum_object = getJsonInt(json_game, "minimum", number_kind * number_min);
    int maximum_object = getJsonInt(json_game, "maximum", number_kind * number_max);

    int while_count = 0;
    while (number_object<minimum_object || number_object > maximum_object) {
        while_count++;
        if (while_count == 50) {
            while_count = 0;
            if (remain_object.compare("fit_quantity")!=0) {
                number_kind = number_group + cocos2d::random(1, 2);
            }
        }
        
        number_object=0;
        number_list.clear();
        //Random so luong trong group
        for (int i=0; i<number_group; i++) {
            int number=cocos2d::random(number_min, number_max);
            number_list.push_back(number);
            number_object += number;
        }
        //Random so luong ngoai group
        if (number_kind-number_group==1) {
            int number=cocos2d::random(number_min, number_max);
            number_list.push_back(number);
            number_object += number;
        } else if (number_kind-number_group==2) {
            int number_1=0;
            int number_2=0;
            do {
                number_1=cocos2d::random(1, number_max);
                number_2=cocos2d::random(1, number_max);
            } while (number_1+number_2 <= number_min && number_1+number_2 >= number_max);
            
            number_list.push_back(number_1);
            number_list.push_back(number_2);
            number_object += (number_1+number_2);
        }
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    rapidjson::Value& ison_json = json_data.FindMember("icon")->value;
    std::string icon_filter = getJsonString(ison_json, "different", "id");
    std::string icon_same = getJsonString(ison_json, "same", "none");
    rapidjson::Value& icon_select = getJsonMember(ison_json, "select");
    
    std::vector<math::resource::Image*> kind_images;

    if (sort_by == "tens") {
        auto all_images = getImageList("icons");
        int number_max = all_images.front()->getComparisionValue("number")-1;
        int max_tens = number_max / 10 + 1;
        auto tens_list = randomArrayIndexes(number_group, max_tens);
        
        for (int i=0; i<number_group; i++) {
            int tens = tens_list[i];
            auto ones = randomArrayIndexes(number_list[i], 10);
            for (int i2=0; i2<number_list[i]; i2++) {
                int number = tens*10 + ones[i2];
                auto img_number = math::number::getNumber(number);
                img_list.push_back(img_number);
            }
        }
    } else {
        auto all_images = getImageList("icons");
        all_images = math::resource::ResHelper::filterImages(all_images, icon_same);
        
        if (icon_select == ison_json || icon_select.IsNull()) {
            kind_images = getImageList(all_images, number_kind, icon_filter);
        } else {
            std::vector<math::resource::Image*> key_images;
            for (rapidjson::SizeType i =0; i<icon_select.Size(); i++) {
                int index = icon_select[i].GetInt();
                if (index >= all_images.size()) {
                    key_images.push_back(math::func::getRandomNode(all_images));
                } else {
                    key_images.push_back(all_images[index]);
                }
            }
            math::func::shuffle(all_images);
            
            if (icon_filter == "id" || icon_filter == "none") {
                for (math::resource::Image* img : key_images) {
                    kind_images.push_back(img);
                }
                auto img_same = math::func::getRandomNode(all_images);
                for (int i=0; i<number_kind; i++) {
                    kind_images.push_back(img_same);
                }
            } else {
                if (icon_filter == "name") {
                    icon_filter = "name_1";
                }
                
                bool filter_prop = true;
                for (math::resource::Property prop : all_images.front()->comparisions) {
                    if (prop.key == icon_filter) {
                        filter_prop = false;
                        break;
                    }
                }
                
                if (!filter_prop) {
                    for (math::resource::Image* image: key_images) {
                        int key_value = image->getComparisionValue(icon_filter);
                        for (math::resource::Image* img2 : all_images) {
                            if (key_value == img2->getComparisionValue(icon_filter)) {
                                kind_images.push_back(img2);
                                break;
                            }
                        }
                    }
                    
                    while (kind_images.size() < number_kind) {
                        for (math::resource::Image* img : all_images) {
                            int prop_value = img->getComparisionValue(icon_filter);
                            bool valid = true;
                            for (math::resource::Image* img2 : kind_images) {
                                if (img2->getComparisionValue(icon_filter) == prop_value) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) {
                                kind_images.push_back(img);
                            }
                        }
                    }
                } else {
                    for (math::resource::Image* image: key_images) {
                        std::string key_value = image->getText(icon_filter);
                        for (math::resource::Image* img2 : all_images) {
                            if (key_value == img2->getText(icon_filter)) {
                                kind_images.push_back(img2);
                                break;
                            }
                        }
                    }
                    
                    while (kind_images.size() < number_kind) {
                        for (math::resource::Image* img : all_images) {
                            std::string prop_value = img->getText(icon_filter);
                            bool valid = true;
                            for (math::resource::Image* img2 : kind_images) {
                                if (img2->getText(icon_filter) == prop_value) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) {
                                kind_images.push_back(img);
                            }
                        }
                    }
                }
            }
        }
        img_list = getImageListNumber(all_images, kind_images, number_list, sort_by);
    }
    
    //TODO: Xu ly sort by size
    bool isBig = true;
    if (sort_key==math::comparision::size) {
        isBig = cocos2d::random(0, 1)==0;
        math::func::shuffle(img_list);
        number_group=number_group>2?2:number_group;
        number_kind=2;
        number_object=0;
        number_list.clear();
        while (number_object<minimum_object) {
            number_object=0;
            //Random so luong trong group
            for (int i=0; i<number_kind; i++) {
                int number=cocos2d::random(number_min, number_max);
                number_list.push_back(number);
                number_object+=number;
            }
        }
        while (img_list.size()>number_object) {
            img_list.pop_back();
        }
        while (img_list.size()<number_object) {
            img_list.push_back(img_list.back());
        }
        
        size_list.clear();
        for (int i=0; i<number_kind; i++) {
            for (int i2=0; i2<number_list[i]; i2++) {
                if (isBig) {
                    size_list.push_back(i*2);
                } else {
                    size_list.push_back(2-i*2);
                }
            }
        }
    } else {
        size_list.clear();
        for (int i=0; i<number_kind; i++) {
            for (int i2=0; i2<number_list[i]; i2++) {
                size_list.push_back(0);
            }
        }
    }
    
    //Group name
    if (remain_object.compare("add_last_group")==0) {
        //Name group
        if (list_name.size()>0) {
            if (number_group==1) {
                list_name.push_back("not");
            } else {
                list_name.push_back("remain");
            }
        }
        number_group++;
    }
    
    bool color_group_bg = false;
    if (list_name.size()>0) {
        for (int i=0; i<number_group; i++) {
            if (list_name[i].compare("key")==0) {
                std::string key_sort = sort_by;
                if (key_sort == "id" || key_sort == "name") {
                    key_sort = "name_1";
                }
                if (key_sort == "color") {
                    color_group_bg = true;
                }
                list_groupname.push_back(math::func::upperFirstLetter(kind_images[i]->getText(key_sort)));
                continue;
            } if (list_name[i].compare("not")==0) {
                list_groupname.push_back(cocos2d::StringUtils::format("Not %s", list_groupname[0].c_str()));
                continue;
            } else {
                list_groupname.push_back(list_name[i]);
            }
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object
    int obj_id=0;
    for (int i=0; i<number_kind; i++) {
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id, math::common::move, img_list[obj_id], img_bg);
            object->setComparisionValue(size_list[obj_id]);
            object->setObjectSize(object_size);
            object->setMoveCallback(CC_CALLBACK_1(SortedToGroupQuiz::onTouchedObject, this), CC_CALLBACK_2(SortedToGroupQuiz::onMoveObjectEnded, this));
            if (sort_by == "tens") {
                object->setImageVisible(false);
                object->setValueTextByKey("name_1");
                object->showValueTextWithAnimation(false);
                object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
                object->getLabelValue()->setFontSize(object_size.height *0.6);
            }
            _objects.push_back(object);
            obj_id++;
        }
    }
    
    int group_name_position = 0;
    if (position_name == "top") {
        group_name_position = 0;
    } else if (position_name == "bottom") {
        group_name_position = 1;
    } else if (position_name == "left") {
        group_name_position = 2;
    } else if (position_name == "right") {
        group_name_position = 3;
    }
    
    //Gen group
    math::resource::Image* group_bg = groupBG();
    math::resource::Image* group_frame = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
    math::group::GroupType gr_type=math::group::custom;
    if (group_type == "queue") {
        gr_type=math::group::queue;
        group_cap = group_column * group_row;
        float padding = object_size.width * 0.8 > 60 ? 60 : object_size.width * 0.8;
        group_size = cocos2d::Size(object_size.width * 1.2 * group_column + padding, 100);
    } else if (group_type == "frame") {
        gr_type=math::group::frame;
        group_cap = group_column * group_row;
    }
    
    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        group->setComparisionKey(sort_key);
        if (dinamic_match) {
            //Co dinh loai object <-> group
            int index_obj=0;
            for (int i2=0; i2<i; i2++) {
                index_obj += number_list[i2];
            }
            group->setComparisionValue(_objects[index_obj]->getComparisionValue());
        }
        if (gr_type==math::group::frame) {
            group->setGroupFrames(group_column, group_row, group_frame);
        } else if (gr_type==math::group::queue) {
            group->setGroupQueue(group_size);
            if (color_group_bg) {
                group->setBackground(groupBG(list_groupname[i]), 0.3);
            } else {
                group->setBackground(group_bg, 0.3);
            }
        } else {
            group->setGroupSize(group_size);
            if (color_group_bg) {
                group->setBackground(groupBG(list_groupname[i]), 0.3);
            } else {
                group->setBackground(group_bg, 0.3);
            }
        }
        if (list_groupname.size()>0) {
            group->setGroupName(list_groupname[i], group_name_position);
            group->showNameText(true);
            if (color_group_bg) {
                group->getNameText()->setTextColor(getColor4B(list_groupname[i]));
            }
        }
        _groups.push_back(group);
    }
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    std::vector<math::resource::Image*> fix;
    for (int i = 0; i < number_group; i++)
        fix.push_back(kind_images[i]);
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, fix);
    }
}

void SortedToGroupQuiz::onStart() {
    startGuide();

    if (align_group_object == "left_right") {
        int total_objects = (int)_objects.size();
        int max_in_line = (total_objects + 4) / 5;
        float width_bar = (object_size.width * max_in_line + 140)/quiz_size.width;
        width_bar = (width_bar < 0.2) ? 0.2 : width_bar;
        onShowRightBar(0, width_bar);
        quizBegan(0.5, false);
        
        math::func::shuffle(_objects);
        math::func::setPositions(_objects, cocos2d::Rect(40, 30, quiz_size.width * width_bar - 80, quiz_size.height-60));
        
        for (int i=0; i< _objects.size(); i++) {
            _objects[i]->fitObjectSize();
            addChildNodeToRightPanel(_objects[i]);
        }
        randomShow(_objects);
        
        math::func::shuffle(_groups);
        auto size = _groups.front()->getContentSize();
        math::func::setPositions(_groups, cocos2d::Rect((quiz_size.width * (1-width_bar) - size.width * 1.5)/2, 0, size.width * 1.5, quiz_size.height));
        for (int i=0; i<_groups.size(); i++) {
            addChildNode(_groups[i]);
            _groups[i]->onShow(i*0.1, i%2==0?math::animation::SlideLeft : math::animation::SlideRight);
        }
    } else {
        float bar_height = 0.3;
        int total_objects = (int)_objects.size();
        if (total_objects > 7) {
            bar_height = 0.35;
        }
        onShowBottomBar(0, bar_height);
        quizBegan(0.5, false);
        
        math::func::shuffle(_objects);
        math::func::setPositions(_objects, cocos2d::Rect(40, 0, quiz_size.width - 80, quiz_size.height * bar_height - 30));
        
        for (int i=0; i< _objects.size(); i++) {
            _objects[i]->fitObjectSize();
            addChildNodeToBottomPanel(_objects[i]);
        }
        randomShow(_objects);
        
        math::func::shuffle(_groups);
        math::func::setPositions(_groups, cocos2d::Rect(0, quiz_size.height*bar_height, quiz_size.width, quiz_size.height*(1-bar_height)));
        for (int i=0; i<_groups.size(); i++) {
            addChildNode(_groups[i]);
            _groups[i]->onShow(i*0.1, i%2==0?math::animation::SlideUp : math::animation::SlideDown);
        }
    }
}

void SortedToGroupQuiz::startQuestion() {
    enableControlQuiz();
    if (questionIndex()==0) {
        if (align_group_object == "left_right") {
            for (Math_Object* obj : _objects) {
                obj->enableDrag(_panel_right);
            }
        } else {
            for (Math_Object* obj : _objects) {
                obj->enableDrag(_panel_bot);
            }
        }

        if (sorted.compare("all_sort_one")==0) {
            disableControlQuiz(2.5);
            scheduleOnce([=](float dt){
                this->onSortedObjects();
            }, 2, "sorted");
        }
    }
}

void SortedToGroupQuiz::onExplanation(bool correct) {
    if (!correct) {
        bool moveback = false;
        for (int i=0; i<_groups.size(); i++) {
            Math_Group* group = _groups[i];
            std::vector<Math_Object*> list_wrong;
            
            if (i==group->getNumberObjects()-1 && remain_object == "add_last_group") {
                for (int i2=0; i2<group->getNumberObjects(); i2++) {
                    auto object =group->getObjectAtIndex(i2);
                    
                    for (int i3=0; i3<i; i3++) {
                        Math_Group* group2 = _groups[i3];
                        if (group2->isMatchWithGroup(object)) {
                            list_wrong.push_back(object);
                            break;
                        }
                    }
                }
            } else {
                if (group->getNumberObjects() != 0) {
                    auto obj_1st = group->getObjectAtIndex(0);
                    int tens = obj_1st->getComparisionValue().number / 10;
                    
                    for (int i2=0; i2<group->getNumberObjects(); i2++) {
                        auto object =group->getObjectAtIndex(i2);
                        if (dinamic_match) {
                            if (!group->isMatchWithGroup(object)) {
                                list_wrong.push_back(object);
                            }
                        } else {
                            if (sort_by == "tens") {
                                if ((object->getComparisionValue().number / 10) != tens) {
                                    list_wrong.push_back(object);
                                }
                            } else {
                                if (!group->isMatchWithOther(object)) {
                                    list_wrong.push_back(object);
                                }
                            }
                        }
                    }
                }
            }
            
            for (Math_Object* object : list_wrong) {
                group->removeObject(object);
                auto point = group->convertToWorldSpace(object->getPosition());
                object->setPosition(point);
            }
            for (Math_Object* object : list_wrong) {
                object->backToStart();
                moveback = true;
            }
        }
        if (moveback) {
            audio_helper->play("audio_moveback.mp3");
        }
        speakQuestion(true);
    } else {
        disableControlQuiz();
        this->nextQuestion(1.0);
    }
}

void SortedToGroupQuiz::onComplete() {
    for (Math_Group* group : _groups) {
        group->onHide();
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

void SortedToGroupQuiz::onSortedObjects() {
    if (sorted.compare("all_sort_one")==0) {
        Math_Group* last_group = _groups[number_group-1];
        
        if (dinamic_match) {
            for (Math_Group* group : _groups) {
                for (Math_Object* object : _objects) {
                    if (remain_object.compare("add_last_group")==0) {
                        if (group != last_group) {
                            if (group->isMatchWithGroup(object)) {
                                //auto matching object
                                sortedToGroup(object, group);
                                break;
                            }
                        }
                    } else {
                        if (group->isMatchWithGroup(object)) {
                            //auto matching object
                            sortedToGroup(object, group);
                            break;
                        }
                    }
                }
            }
        } else {
            //chỉ fit_quantity
            std::vector<Math_Object*> sorted_objs;
            sorted_objs.push_back(math::func::getRandomNode(_objects));
            
            while (sorted_objs.size() < number_group) {
                Math_Object *object = math::func::getRandomNode(_objects);
                bool find = false;
                for (Math_Object * obj : sorted_objs) {
                    if (obj->isEqual(object, sort_key)) {
                        find = true;
                        break;
                    }
                }
                if (!find) {
                    sorted_objs.push_back(object);
                }
            }
            
            for (int i=0; i<number_group; i++) {
                Math_Object* object = sorted_objs[i];
                Math_Group* group = _groups[i];
                sortedToGroup(object, group);
            }
        }
    }
}


void SortedToGroupQuiz::onGuideStarted() {
    disableControlQuiz();
    
    Math_Object* object = nullptr;
    Math_Group* group = nullptr;

    for (Math_Object* _object : _objects) {
        //object chua trong group nao
        bool valid = true;
        for (Math_Group* _group : _groups) {
            if (_group->hasObject(_object)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            for (Math_Group* _group : _groups) {
                if (dinamic_match) {
                    if (_group->isMatchWithGroup(_object)) {
                        valid = true;
                        object = _object;
                        group = _group;
                        break;
                    }
                } else {
                    if (_group->isMatchWithOther(_object)) {
                        valid = true;
                        object = _object;
                        group = _group;
                        break;
                    }
                }
            }
        }
    }
    if (group && object) {
        GuideCenter->guideByDragDrop(object->getWorldPosition(), group->getWorldPosition());
    } else {
        GuideCenter->guideNone();
    }
}

void SortedToGroupQuiz::onGuideDone() {
    enableControlQuiz();
}

void SortedToGroupQuiz::sortedToGroup (Math_Object* object, Math_Group* group) {
    auto position = group->getWorldPosition();
    position = _panel_right->convertToNodeSpace(position);
    position = cocos2d::Vec2 (cocos2d::random(position.x - group->getContentSize().width * 0.5 + object_size.width, position.x + group->getContentSize().width * 0.5 - object_size.width), cocos2d::random(position.y - group->getContentSize().height * 0.5 + object_size.height, position.y + group->getContentSize().height * 0.5 - object_size.height));

    object->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)), cocos2d::CallFunc::create([=](){
        auto point = object->getWorldPosition();
        group->addObject(object, point);
    }))); 
    object->setLocked(true);
}

#pragma mark - object touch events callback
void SortedToGroupQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = static_cast<Math_Object*>(node);
    //Chon object dau tien cham phai
    if (selected_object==nullptr) {
        selected_object=object;
        audio_helper->play("audio_drag.mp3");
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void SortedToGroupQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
    Math_Object* object = static_cast<Math_Object*>(node);
    //remove object khoi group hien tai
    Math_Group* gr_source = nullptr;
    for (Math_Group* _group : _groups) {
        if (_group->hasObject(object)) {
            gr_source = _group;
            break;
        }
    }
    Math_Group* group=nullptr;
    for (Math_Group* _group : _groups) {
        if (_group->isTouchOnGroup(point)) {
            group=_group;
            break;
        }
    }
    if (gr_source!=nullptr) {
        if (gr_source==group) {
            //object->backToLast(1);
            gr_source->removeObject(object, true);
            group->addObject(object, point);
            selected_object=nullptr;
            object->onDropObject(0.1);
            return;
        } else {
            gr_source->removeObject(object, true);
        }
    }
    bool can_add=true;
    if (group!=nullptr) {
        if (only_correct_move && can_add) {
            //Check object và group
            if (!dinamic_match) {
                if (sort_by == "tens") {
                    can_add = false;
                    auto obj_1st = group->getObjectAtIndex(0);
                    int tens = obj_1st->getComparisionValue().number / 10;
                    if ((object->getComparisionValue().number / 10) == tens) {
                        can_add = true;
                    }
                } else {
                    can_add = group->isMatchWithOther(object);
                }
            } else {
                can_add = group->
                isMatchWithGroup(object);
            }
        }
        if (can_add) {
            can_add = group->addObject(object, point);
        }
        if (can_add) {
            object->onDropObject(0.1);
            selected_object=nullptr;
        }
    } else {
        can_add = false;
    }
    
    if (can_add) {
        //TODO: check done
        std::vector<Math_Object*> remain_objs;
        for (Math_Object* object : _objects) {
            bool find = false;
            for (Math_Group* group : _groups) {
                if (group->hasObject(object)) {
                    find = true;
                    break;
                }
            }
            if (!find) {
                remain_objs.push_back(object);
            }
        }
        //check k còn obj thừa và chế độ khác skip remain.
        if (!(remain_object == "skip_remain" || remain_objs.empty())) {
            return;
        }
        bool is_correct = true;
        if (remain_object == "skip_remain") {
            for (Math_Group* group : _groups) {
                if (!group->isMatchGroup()) {
                    is_correct = false;
                    break;
                }
            }
            for (Math_Object* object : remain_objs) {
                for (Math_Group* group : _groups) {
                    if (group->isMatchWithGroup(object)) {
                        is_correct = false;
                        break;
                    }
                }
                if (!is_correct) {
                    break;
                }
            }
        }
        if (is_correct) {
            onExplanation(is_correct);
        }
    } else {
        audio_helper->play("audio_moveback.mp3");
        object->backToStart();
        selected_object=nullptr;
    }
}


void SortedToGroupQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool is_correct=true;
    std::vector<Math_Object*> wrong_objs;

    //Check object chua duoc keo => false
    std::vector<Math_Object*> remain_objs;
    for (Math_Object* object : _objects) {
        bool find = false;
        for (Math_Group* group : _groups) {
            if (group->hasObject(object)) {
                find = true;
                break;
            }
        }
        if (!find) {
            remain_objs.push_back(object);
        }
    }
    
    if (remain_object != "skip_remain") {
        if (remain_objs.size() > 0) {
            is_correct = false;
        }
    }
    
    //có ít nhất 1 group chưa được xếp => false
    if (is_correct) {
        for (Math_Group* group : _groups) {
            if (group->getNumberObjects()==0) {
                is_correct = false;
                break;
            }
        }
    }
    
    //Check object đã kéo
    if (is_correct) {
        if (remain_object == "fit_quantity") {
            for (Math_Group* group : _groups) {
                if (dinamic_match) {
                    if (!group->isMatchGroup()) {
                        is_correct = false;
                        break;
                    }
                } else {
                    if (sort_by == "tens") {
                        auto obj_1st = group->getObjectAtIndex(0);
                        int tens = obj_1st->getComparisionValue().number / 10;
                        for (int i=1; i<group->getNumberObjects(); i++) {
                            if ((group->getObjectAtIndex(i)->getComparisionValue().number / 10) != tens) {
                                is_correct = false;
                                break;
                            }
                        }
                        
                    } else {
                        if (!group->isMatchGroupWithOther()) {
                            is_correct = false;
                            break;
                        }
                    }
                }
            }
        } else if (remain_object == "add_last_group") {
            Math_Group* last_group = _groups.back();
            for (Math_Group* group : _groups) {
                if (group != last_group) {
                    if (!group->isMatchGroup()) {
                        is_correct = false;
                        break;
                    }
                }
            }
            for (int i=0; i<last_group->getNumberObjects(); i++) {
                Math_Object* object = last_group->getObjectAtIndex(i);
                for (Math_Group* group : _groups) {
                    if (group != last_group) {
                        if (group->isMatchWithGroup(object)) {
                            is_correct = false;
                            break;
                        }
                    }
                }
            }
        } else {
            //skip remain object
            for (Math_Group* group : _groups) {
                if (!group->isMatchGroup()) {
                    is_correct = false;
                    break;
                }
            }
            for (Math_Object* object : remain_objs) {
                for (Math_Group* group : _groups) {
                    if (group->isMatchWithGroup(object)) {
                        is_correct = false;
                        break;
                    }
                }
                if (!is_correct) {
                    break;
                }
            }
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
void SortedToGroupQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void SortedToGroupQuiz::autoTestingRun(float delaytime)
{
}

