//
//  DragBetweenTwoGroupsQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/24/17.
//
//

#include "DragBetweenTwoGroupsQuiz.h"

DragBetweenTwoGroupsQuiz* DragBetweenTwoGroupsQuiz::createQuiz()
{
    DragBetweenTwoGroupsQuiz* quiz=new DragBetweenTwoGroupsQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return nullptr;
}

bool DragBetweenTwoGroupsQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void DragBetweenTwoGroupsQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    number_group = 2;
    reorder = getJsonBool(json_game, "reorder", true);
    align = getJsonBool(json_game, "align", true);
    request = getJsonString(json_game, "request", "equal");
    if (request.compare("equal")==0) {
        drag_type=DragType::make_equal;
    } else if (request.compare("more")==0) {
        drag_type=DragType::make_more;
    } else if (request.compare("less")==0) {
        drag_type=DragType::make_less;
    } else if (request.compare("more_or_less")==0) {
        if (cocos2d::random(0, 1)==0) {
            drag_type=DragType::make_more;
        } else {
            drag_type=DragType::make_less;
        }
    } else if (request.compare("enough")==0) {
        drag_type=DragType::make_at_least;
    } else if (request.compare("quantity")==0) {
        drag_type=DragType::by_quantity;
    } else {
        drag_type=DragType::compare;
    }
    quantity = randomRangeJson(json_game, "quantity");
    
    std::string compare_text = getJsonString(json_game, "compare", "id");
    compare_key = getComparisionKey(compare_text);
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object = json_data.FindMember("object")->value;
    auto number_max = getJsonInt(getJsonMember(json_object, "number"), "maximum", 3);
    auto number_min = getJsonInt(getJsonMember(json_object, "number"), "minimum", 1);
    object_size = getSizeJson(json_object, "size");
    
    std::vector<int> array_range;
    rapidjson::Value& range_list = json_object.FindMember("range")->value;
    if (!range_list.Empty()) {
        for (rapidjson::SizeType i=0; i<range_list.Size(); i++) {
            array_range.push_back(range_list[i].GetInt());
        }
    }
    
    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    
    std::string name_key = getJsonString(json_group, "name", "none");
    object_align = getJsonString(json_group, "align", "left");
    object_arrange = getJsonBool(json_group, "arrange", true);
    group_type = getJsonString(json_group, "type", "custom");
    group_column = getJsonInt(getJsonMember(json_group, "frame"), "column", 5);
    group_row = getJsonInt(getJsonMember(json_group, "frame"), "row", 2);
    group_size = getSizeJson(json_group, "size");
    
    //Random số lượng từng đối tượng
    int number_kind = 2;

    number_object=0;
    if (drag_type==DragType::by_quantity) {
        number_list.push_back(cocos2d::random(number_min, number_max));
        number_list.push_back(0);
    } else if (drag_type==DragType::compare) {
        number_list.push_back(cocos2d::random(number_min, number_max));
        int dif = cocos2d::random(number_min, number_max);
        for (int i=0; i<dif; i++) {
            number_list.push_back(1);
        }
        number_kind = dif+1;
    } else {
        if (array_range.size()>0) {
            int range=math::func::getRandomNode(array_range);
            int number_1=cocos2d::random(number_min+range, number_max);
            
            if (drag_type==DragType::make_less) {
                number_list.push_back(number_1-range);
                number_list.push_back(number_1);
            } else {
                number_list.push_back(number_1);
                number_list.push_back(number_1-range);
            }
        }
    }
    
    //TODO: Chon icon
    //lay tat ca icon trong media_set
    std::string icon_filter = json_data.FindMember("icon")->value.GetString();
    auto all_imgs = getImageList("icons");
    auto img_kind = getImageList(all_imgs, number_kind, icon_filter);
    
    img_number_list = getNumbers(quantity, quantity);
    
    //Group name:
    if (name_key == "people_name") {
        group_name_img = getPeopleNames(number_group);
    } if (name_key == "color") {
        group_name_img = getColors();
        math::func::shuffle(group_name_img);
        while (group_name_img.size() > number_group) {
            group_name_img.pop_back();
        }
    } else {
        
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
        } else if (key == "name_n") {
            if (img_number_list.front()->getComparisionValue("number")==1) {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : img_kind) {
                    auto img_txt = img->getTextValue("name_1");
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, img_kind);
            }
        } else {
            getQuestion()->setOptionalTexts(key, img_kind);
        }
    }
    
    if (drag_type==DragType::compare) {
        img_list = getImageListNumber(all_imgs, img_kind, number_list, compare_text);
        int total_icon = (int)img_list.size();
        number_list.clear();
        number_list.push_back(total_icon);
        number_list.push_back(0);
    } else {
        img_list = getImageListNumber(all_imgs, img_kind, number_list, icon_filter);
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen object & group
    math::group::GroupType gr_type=math::group::custom;
    math::resource::Image* group_bg = nullptr;
    
    group_cap=number_max+number_min+1;
    group_bg = groupBG();
    auto group_frame = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
    if (group_type.compare("queue")==0) {
        gr_type=math::group::queue;
        group_cap = group_column * group_row;
        group_size = cocos2d::Size(group_column*1.2*object_size.width + object_size.width*0.6, group_row*1.2*object_size.height + object_size.width*0.6);
    } else if (group_type.compare("frame")==0) {
        gr_type=math::group::frame;
        group_cap = group_column * group_row;
    }
    
    std::vector<Math_Object*> group_value;
    for (math::resource::Image* img : img_kind) {
        auto object = mpool->getObject();
        object->setEnable(0, math::common::deny, img, nullptr);
        group_value.push_back(object);
    }
    
    int obj_id=0;
    for (int i=0; i<number_group; i++) {
        std::vector<Math_Object*> _group_list;
        
        for (int j=0; j<number_list[i]; j++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(obj_id, math::common::move, img_list[obj_id], img_bg);
            object->setComparisionValue();
            object->setObjectSize(object_size);
            object->setMoveCallback(CC_CALLBACK_1(DragBetweenTwoGroupsQuiz::onTouchedObject, this), CC_CALLBACK_2(DragBetweenTwoGroupsQuiz::onMoveObjectEnded, this));
            _objects.push_back(object);
            obj_id++;
            _group_list.push_back(object);
        }
        
        Math_Group* group=mpool->getGroup();
        group->setEnable(i, _group_list, gr_type, group_cap);
        group->setObjectSize(object_size);
        if (drag_type == DragType::compare) {
            group->setComparisionKey(compare_key);
            if (i==0) {
                group->setComparisionValue(group_value.front()->getComparisionValue());
            } else {
                group->setComparisionValue(group_value.back()->getComparisionValue());
            }
        }
        
        if (gr_type==math::group::frame) {
            group->setAlignObjects(math::group::random);
            group->setGroupFrames(group_column, group_row, group_frame);
            group->setAlignObjects(math::group::random);
        } else if (gr_type==math::group::custom) {
            group->setGroupSize(group_size);
            group->setBackground(group_bg);
        } else {
            group->setGroupQueue(group_size);
            group->setBackground(group_bg);
            group->setAlignObjects(math::group::random);
        }
        
        if (group_name_img.size()>0) {
            group->setGroupName(group_name_img[i]->getText());
            group->showNameText(true);
        }
        _groups.push_back(group);
    }
}

void DragBetweenTwoGroupsQuiz::onStart() {
    onShowBottomBar(0, 0.2, false, CC_CALLBACK_1(DragBetweenTwoGroupsQuiz::onAnswerDone, this), nullptr, true);
    Math_Quiz::onStart();
    
    //show
    float delta = (quiz_size.width - _groups.front()->getContentSize().width * number_group) / (number_group+1);
    delta = delta > 100 ? 100 : delta;
    math::func::smartSetPosition(_groups, _groups.front()->getContentSize(), cocos2d::Rect(0, 0, quiz_size.width, quiz_size.height * 0.8 - getQuestion()->getContentSize().height - 20), delta);
    
    for (int i=0; i < _groups.size(); i++) {
        this->addChildNode(_groups[i]);
        _groups[i]->onShow(i*0.1);
    }
}

void DragBetweenTwoGroupsQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
        scheduleOnce([=](float){
            for (Math_Object* obj : _objects) {
                obj->enableDrag(obj->getParent());
            }
        }, 1.0, "enable drag");
    }
}

void DragBetweenTwoGroupsQuiz::onExplanation(bool correct) {
    if (!correct) {
        _groups.back()->removeAllObjects();
        for (Math_Object* obj : _objects) {
            obj->backStartGroup();
        }
    } else {
        this->nextQuestion();
    }
}

void DragBetweenTwoGroupsQuiz::onComplete() {
    float delay = 1.0;
    for (Math_Group* group : _groups) {
        group->setDisable(delay);
        delay += 0.1;
    }
    scheduleOnce([=](float dt) {
        Math_Quiz::onComplete();
    }, 1.5, "complete_quiz");
}

#pragma mark - object touch events callback
void DragBetweenTwoGroupsQuiz::onTouchedObject(cocos2d::Node* node)
{
    Math_Object* object = (Math_Object*)node;

    //Chon object dau tien cham phai
    if (selected_object==nullptr) {
        selected_object=object;
        audio_helper->play("movenumber/drag.mp3");
    } else {
        object->setAllowMove(false);
        object->backToLast(-1);
    }
}

void DragBetweenTwoGroupsQuiz::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point)
{
    Math_Object* object = (Math_Object*)node;
    point = _panel->convertToNodeSpace(point);
    
    Math_Group* start_group=nullptr;
    for (Math_Group* _group : _groups) {
        if (_group->hasObject(object)) {
            start_group = _group;
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
    
    start_group->removeObject(object);
    if (group!=nullptr && group != start_group) {
        object->removeFromParentAndCleanup(false);
        object->setPosition(point);
        addChildNode(object, 100);
        if (group->addObject(object, point)) {
            object->onDropObject(0.1);
        }
    } else {
        audio_helper->play("movenumber/wrong.mp3");
        object->backLastGroup();
    }
    selected_object=nullptr;
}

void DragBetweenTwoGroupsQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool is_correct=true;
    if (drag_type==DragType::by_quantity) {
        is_correct=_groups[1]->getNumberObjects()==quantity;
    } else if (drag_type==DragType::make_equal) {
        for (int i=0; i<number_group-1; i++) {
            if (_groups[i]->getNumberObjects()!=_groups[i+1]->getNumberObjects()) {
                is_correct=false;
                break;
            }
        }
    } else if (drag_type==DragType::make_at_least) {
        for (int i=0; i<number_group-1; i++) {
            if (_groups[i]->getNumberObjects()<_groups[i+1]->getNumberObjects()) {
                is_correct=false;
                break;
            }
        }
    } else if (drag_type==DragType::make_more) {
        for (int i=0; i<number_group-1; i++) {
            if (_groups[i]->getNumberObjects()>=_groups[i+1]->getNumberObjects()) {
                is_correct=false;
                break;
            }
        }
    } else if (drag_type==DragType::make_less) {
        for (int i=0; i<number_group-1; i++) {
            if (_groups[i]->getNumberObjects()<=_groups[i+1]->getNumberObjects()) {
                is_correct=false;
                break;
            }
        }
    } else if (drag_type==DragType::make_equal_less) {
        for (int i=0; i<number_group-1; i++) {
            if (_groups[i]->getNumberObjects()>_groups[i+1]->getNumberObjects()) {
                is_correct=false;
                break;
            }
        }
    } else if (drag_type==DragType::compare) {
        //Thoa man 2 dk:
        //1. co 1 group goi laf group A, ma tat ca cac obj bang nhau.
        is_correct = false;
        Math_Group* groupA = nullptr;
        for (Math_Group* group : _groups) {
            if (group->isMatchGroupWithOther()) {
                groupA=group;
                is_correct = true;
                break;
            }
        }
        //2. group con lai, group b, k co obj nao match voi A
        if (is_correct) {
            for (Math_Group* group : _groups) {
                if (group != groupA) {
                    for (int i=0; i<group->getNumberObjects(); i++) {
                        if (groupA->isMatchWithOther(group->getObjectAtIndex(i))) {
                            is_correct = false;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    disableControlQuiz(0.5);
    answer->onAnimationClick(is_correct);
    this->onExplanation(is_correct);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void DragBetweenTwoGroupsQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void DragBetweenTwoGroupsQuiz::autoTestingRun(float delaytime)
{
}

