//
//  ChooseCorrectGroupQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/25/17.
//
//

#include "ChooseCorrectGroupQuiz.h"

ChooseCorrectGroupQuiz* ChooseCorrectGroupQuiz::createQuiz()
{
    ChooseCorrectGroupQuiz* quiz=new ChooseCorrectGroupQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool ChooseCorrectGroupQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void ChooseCorrectGroupQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_group = getJsonInt(json_game, "group", 2);
    align = getJsonBool(json_game, "align", true);
    game_type = getJsonString(json_game, "type", "total_number");
    quantity = randomRangeJson(json_game, "quantity");
    std::string comparision =  getJsonString(json_game, "comparision", "id");
    compare_key = getComparisionKey(comparision);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    auto number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 3);
    auto number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 1);
    object_size = getSizeJson(json_object, "size");
    fit_object = getJsonString(json_object, "fit_size", "none");
    
    group_cap=number_max*number_group;
    img_number_list = getNumbers(quantity, quantity);
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    rapidjson::Value& group_names=json_group.FindMember("name")->value;
    std::vector<std::string> list_name;
    for (rapidjson::SizeType i=0; i<group_names.Size(); i++) {
        list_name.push_back(group_names[i].GetString());
    }
    kind_in_group = randomRangeJson(json_group, "kind");
    object_align = getJsonString(json_group, "align", "left");
    object_arrange = getJsonBool(json_group, "arrange", true);
    sort_type = getJsonString(json_group, "sort", "kind");
    group_type = getJsonString(json_group, "type", "frame");
    group_column = getJsonInt(getJsonMember(json_group, "frame"), "column", 5);
    group_row = getJsonInt(getJsonMember(json_group, "frame"), "row", 2);
    group_size = getSizeJson(json_group, "size");
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    rapidjson::Value& json_icon=json_data.FindMember("icon")->value;
    std::string icon_filter = getJsonString(json_icon, "different", "id");
    std::string icon_same = getJsonString(json_icon, "same", "none");
    
    auto all_imgs = getImageList("icons");
    
    img_list = getImageList(all_imgs, kind_in_group, icon_filter, icon_same);
    
    if (game_type.compare("enough")==0) {
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
    }
    
    //Group name:
    if (list_name.size()>0) {
        auto random_name = randomArrayIndexes(number_group, (int)list_name.size());
        for (int i=0; i<number_group; i++) {
            list_groupname.push_back(list_name[random_name[i]]);
        }
    }
    if (game_type.compare("enough")==0) {
        //So sánh về số lượng trong cùng group.
        int number = cocos2d::random(number_min, number_max);
        for (int i=0; i<number_group; i++) {
            std::vector<int> number_in_group;
            number_in_group.push_back(number);
            if (i==0) {
                number_in_group.push_back(number);
            } else {
                int number2 = cocos2d::random(number_min, number_max);
                while (number2 == number) {
                    number2 = cocos2d::random(number_min, number_max);
                }
                number_in_group.push_back(number2);
            }
            math::func::shuffle(number_in_group);
            
            for (int _val : number_in_group) {
                number_list.push_back(_val);
            }
        }
    } else if (game_type.compare("total_number")==0) {
        //Tính tổng số đối tượng có trong group
        std::vector<int> number_total_group;
        for (int i=0; i<number_group; i++) {
            std::vector<int> number_in_group;
            if (i==0) {
                number_total_group.push_back(quantity);
                if (quantity<=kind_in_group) {
                    for (int i2=0; i2<kind_in_group; i2++) {
                        if (i2<quantity) {
                            number_in_group.push_back(1);
                        } else {
                            number_in_group.push_back(0);
                        }
                    }
                } else {
                    int _div = quantity/kind_in_group;
                    int _mod = quantity%kind_in_group;
                    for (int i2=0; i2<kind_in_group; i2++) {
                        if (i2<_mod) {
                            number_in_group.push_back(_div+1);
                        } else {
                            number_in_group.push_back(_div);
                        }
                    }
                }
            } else {
                int _number;
                bool ret;
                do {
                    ret = false;
                    _number = cocos2d::random(number_min, number_max);
                    for (int total : number_total_group) {
                        if (_number == total) {
                            ret = true;
                            break;
                        }
                    }
                } while (ret);
                number_total_group.push_back(_number);
                
                int _div = _number/kind_in_group;
                int _mod = _number%kind_in_group;
                for (int i2=0; i2<kind_in_group; i2++) {
                    if (i2<_mod) {
                        number_in_group.push_back(_div+1);
                    } else {
                        number_in_group.push_back(_div);
                    }
                }
            }
            
            for (int _val : number_in_group) {
                number_list.push_back(_val);
            }
            
        }
    } else if (game_type.compare("spec_number")==0) {
        //Tính tổng số đối tượng 1 loại trong group. group có ít nhất 2 loại icon
        for (int i=0; i<number_group; i++) {
            std::vector<int> number_in_group;
            if (i==0) {
                number_in_group.push_back(quantity);
            } else {
                int _num=cocos2d::random(number_min, number_max);
                while (_num==quantity) {
                    _num=cocos2d::random(number_min, number_max);
                }
                number_in_group.push_back(_num);
            }
            for (int i2=1; i2<kind_in_group; i2++) {
                number_in_group.push_back(cocos2d::random(number_min, number_max));
            }
            
            for (int _val : number_in_group) {
                number_list.push_back(_val);
            }
        }
    } else {
        //So sánh các object trong cùng 1 group. 2 object. về giống nhau hoặc khác nhau.
        //ex: Same length, same size... or not
        for (int i=0; i<number_group * kind_in_group; i++) {
            number_list.push_back(1);
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
        } else if (key == "name_n") {
            if (img_number_list.front()->getComparisionValue("number")==1) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : img_list) {
                    auto img_txt = img->getTextValue("name_1");
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_list);
            }
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object & group
    math::resource::Image* group_bg = groupBG();
    math::resource::Image* group_frame = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));

    math::group::GroupArange gr_arrange = math::group::mix;
    math::group::GroupType gr_type=math::group::custom;
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
        int max = number_list.front();
        for (int number : number_list) {
            max = max < number ? number : max;
        }
        group_cap = max * kind_in_group;
        int fix_size = max < 5 ? 5 : max;
        
        if (game_type == "enough") {
            group_size = cocos2d::Size(object_size.width * (fix_size * 1.2 + 0.6), object_size.height * (kind_in_group * 1.2 + 0.6));
            gr_arrange = math::group::kind;
        } else {
            group_size = cocos2d::Size(object_size.width * 6.6, object_size.height * (kind_in_group*1.2+0.6));
            gr_arrange = math::group::kind;
        }
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
    }
    
    int obj_id=0;
    int number_id=0;
    cocos2d::Size item_size = object_size;
    std::vector<std::vector<Math_Object*>> objects_groups_list;

    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        for (int i2=0; i2<kind_in_group; i2++) {
            for (int i3=0; i3<number_list[number_id]; i3++) {
                Math_Object* object=mpool->getObject();
                
                if (game_type.compare("comparision")==0) {
                    if (i==0) {
                        object->setEnable(obj_id, math::common::deny, img_list[0], img_bg);
                        object->setObjectSize(object_size);
                    } else {
                        object->setEnable(obj_id, math::common::deny, img_list[i2], img_bg);
                        object->setObjectSize(object_size);
                    }
                } else {
                    object->setEnable(obj_id, math::common::deny, img_list[i2], img_bg);
                    object->setObjectSize(object_size);
                }
                
                if (fit_object.compare("width")==0) {
                    object->fixWidthObject();
                    if (item_size.height < object->getContentSize().height) {
                        item_size=object->getContentSize();
                    }
                } else if (fit_object.compare("height")==0) {
                    object->fixHeightObject();
                    if (item_size.width < object->getContentSize().width) {
                        item_size=object->getContentSize();
                    }
                }
                _objects.push_back(object);
                _group_list.push_back(object);
                obj_id++;
                
            }
            number_id++;
        }
        if (sort_type.compare("mix")==0) {
            math::func::shuffle(_group_list);
        }
        objects_groups_list.push_back(_group_list);
    }
        
    for (int i=0; i<number_group; i++) {
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, objects_groups_list[i], gr_type, group_cap);
        group->setObjectSize(item_size);
        group->enableToch(CC_CALLBACK_1(ChooseCorrectGroupQuiz::onTouchedGroup, this));
        if (gr_type==math::group::frame) {
            group->setGroupFrames(group_column, group_row, group_frame);
        } else if (gr_type==math::group::queue) {
            group->setGroupQueue(group_size);
            group->setBackground(group_bg, 1);
        } else {
            group->setGroupSize(group_size);
            group->setBackground(group_bg, 1);
        }
        if (sort_type.compare("kind")==0) {
            group->setArrangeObjects(math::group::kind);
        }
        if (list_groupname.size()>0) {
            group->setGroupName(list_groupname[i]);
            group->showNameText(true);
        }
        group->setArrangeObjects(gr_arrange);
        
        if (fit_object == "height") {
            group->setItemsAlign(Math_Group::ItemAlignLeft);
        } else if (fit_object == "width") {
            group->setItemsAlign(Math_Group::ItemAlignBottom);
        }
        _groups.push_back(group);
    }
}

void ChooseCorrectGroupQuiz::onStart() {
    startGuide();
    quizBegan(0, false);
    
    math::func::shuffle(_groups);
    group_size = _groups.front()->getContentSize();
    if (align) {
        math::func::setPositions(_groups, cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height));
    } else {
        auto wgroup = _groups.front()->getContentSize().width + 80;
        math::func::setPositions(_groups, cocos2d::Rect((quiz_size.width - wgroup)/2, 0, wgroup, quiz_size.height));
    }
    for (int i=0; i<number_group; i++) {
        Math_Group* group = _groups[i];
        addChildNode(group);
        if (align) {
            if (i%2 == 0) {
                group->onShow(0, math::animation::SlideUp);
            } else {
                group->onShow(0, math::animation::SlideDown);
            }
        } else {
            if (i%2 == 0) {
                group->onShow(0, math::animation::SlideRight);
            } else {
                group->onShow(0, math::animation::SlideLeft);
            }
        }
    }
}

void ChooseCorrectGroupQuiz::startQuestion() {
    enableControlQuiz();
}

void ChooseCorrectGroupQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion(0.5);
    }
}

void ChooseCorrectGroupQuiz::onComplete() {
    for (int i=0; i<number_group; i++) {
        Math_Group* group = _groups[i];
        if (align) {
            group->setDisable(0, math::animation::SlideRight);
        } else {
            if (i%2 == 0) {
                group->setDisable(0, math::animation::SlideRight);
            } else {
                group->setDisable(0, math::animation::SlideLeft);
            }
        }
    }
    quizEnded(0.5, true);
}

void ChooseCorrectGroupQuiz::onGuideStarted() {
    disableControlQuiz();
    
    for (Math_Group* group : _groups) {
        if (group->getId() == 0) {
            GuideCenter->guideByTouch(group->getWorldPosition());
            return;
        }
    }
    GuideCenter->guideNone();
}

void ChooseCorrectGroupQuiz::onGuideDone() {
    enableControlQuiz();
}

#pragma mark - object touch events callback
void ChooseCorrectGroupQuiz::onTouchedGroup(Node* node) {
    Math_Group* group = (Math_Group*)node;
    bool correct = group->getId() == 0;
    group->onTouchedAnimation(correct);
    this->onExplanation(correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void ChooseCorrectGroupQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void ChooseCorrectGroupQuiz::autoTestingRun(float delaytime) {
}

