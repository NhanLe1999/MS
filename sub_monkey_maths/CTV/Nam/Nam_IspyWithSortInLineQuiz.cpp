//
//  Nam_IspyWithSortInLineQuiz
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//
#include "Nam_IspyWithSortInLineQuiz.h"

Nam_IspyWithSortInLineQuiz* Nam_IspyWithSortInLineQuiz::createQuiz()
{
    Nam_IspyWithSortInLineQuiz* quiz=new Nam_IspyWithSortInLineQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Nam_IspyWithSortInLineQuiz::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Nam_IspyWithSortInLineQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    rapidjson::Value& json_size = json_game.FindMember("object_size")->value;
    shape_size = Size(json_size.FindMember("width")->value.GetInt(),json_size.FindMember("height")->value.GetInt());
    yeu_cau_1 = json_game.FindMember("yeucau1")->value.GetString();
    yeu_cau_2 = json_game.FindMember("yeucau2")->value.GetString();
    //yeu_cau_2 = "size";
    rapidjson::Value& json_number_shape = json_game.FindMember("number_object")->value;
    number_shape = RandomHelper::random_int(json_number_shape.FindMember("minimum")->value.GetInt(), json_number_shape.FindMember("maximum")->value.GetInt());
    number_shape = (yeu_cau_2 == "size" && number_shape%2 == 1) ? (number_shape+1):number_shape;
    std::string check_number_line = json_game.FindMember("number_line")->value.GetString();
    number_line = (check_number_line == "bang_so_shape") ? number_shape : RandomHelper::random_int(std::min(2,number_shape-1), number_shape-1);

    // GET IMAGES
    auto all_images_shape = getImageList("icons");
    std::string key1 = (yeu_cau_1== "name") ? "name_1" : "like";
    if (yeu_cau_2 == "none"){
        all_images_shape = getImageList("icons", number_shape, key1);
    }else{
        if (yeu_cau_2 == "size"){
            all_images_shape = getImageList("icons", number_shape/2, key1);
        }
    }
    auto image_line = math::resource::ResHelper::createImage(getUIPath("common/line/line_3.png"));
    
    
    // gen cac shape
    auto shapes_pos = Math_Quiz::randomPositionsInRect(shape_size, cocos2d::Rect(50, 0, quiz_size.width-100, 0.27*quiz_size.height), number_shape);
    for (int i = 0 ; i < number_shape; i++){
        // tao shapes
        auto obj = Math_Pool::getInstance()->getObject();
        if (yeu_cau_2 == "size"){
            obj->setEnable(i, math::common::TouchPermission::move, all_images_shape[i%(number_shape/2)], NULL);
            if (i < (number_shape/2)){
                obj->setComparisionValue(0);
            }else{
                obj->setComparisionValue(1);
            }
        }
        else{
            obj->setEnable(i, math::common::TouchPermission::move, all_images_shape[i], NULL);
        }
         obj->setObjectSize(shape_size);
        _objects.push_back(obj);
        shapes_question.push_back(obj);
        obj->setPosition(shapes_pos[i]);
        obj->setMoveCallback(CC_CALLBACK_1(Nam_IspyWithSortInLineQuiz::onTouchedObject, this), CC_CALLBACK_2(Nam_IspyWithSortInLineQuiz::onMoveObjectEnded, this));
    }
    for (int i = 0 ; i < number_line; i++){
        // tao slot
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i, image_line);
        slot->setSlotSize(shape_size*1.2);
        slot->getBackground()->setContentSize(Size(shape_size.width,20));
        lines.push_back(slot);
    }
    math::func::shuffle(shapes_question);
    math::func::setPositionList(lines, (number_line < 5) ? number_line : (number_line+1)/2, Rect(0.1,0,0.8,0.5));
   
    
    current_index = 0;
    for (auto obj : _objects){
        this->addChildNodeToBottomPanel(obj,2);
        obj->onShow(0);
    }
    // GET QUESTION
    this->parseQuestion(true,number_line);
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "color") {
            std::vector<math::resource::Text> texts;
            for (auto obj : shapes_question){
                auto img = obj->getImageSource();
                math::resource::Text t = img->getTextValue("color", m_lang->getLanguage());
                texts.push_back(t);
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        if (key == "name_1") {
            std::vector<math::resource::Text> texts;
            for (auto obj : shapes_question){
                auto img = obj->getImageSource();
                math::resource::Text t = img->getTextValue("name_1", m_lang->getLanguage());
                texts.push_back(t);
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        if (key == "size"){
            std::vector<math::resource::Text> texts;
            for (auto obj : shapes_question){
                std::string text = (obj->getComparisionValue().size == 0)? "big":"small";
                math::resource::Text t;
                t.text = text;
                texts.push_back(t);
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        if (key == "like"){
            std::vector<math::resource::Text> texts;
            for (auto obj : shapes_question){
                auto img = obj->getImageSource();
                math::resource::Text t = img->getTextValue("like", m_lang->getLanguage());
                texts.push_back(t);

            }
            getQuestion()->setOptionalTexts(key, texts);
        }
        if (key == "number"){
            std::vector<math::resource::Text> texts;
            for (int i = 0; i < shapes_question.size(); i++){
                std::string text = (i == 0)? "first":"next";
                math::resource::Text t;
                t.text = text;
                texts.push_back(t);
                
            }
            getQuestion()->setOptionalTexts(key, texts);
        }
    }
}

void Nam_IspyWithSortInLineQuiz::onStart() {
    math_statistic->setCurrentGame(_game);
    //Gen question
    onShowBottomBar(0,0.3);
    showQuestionBox();
}

void Nam_IspyWithSortInLineQuiz::startQuestion() {
    this->scheduleUpdate();
    for (auto obj : _objects){
       obj->enableDrag(_panel_bot);
    }
    if (this->getQuestion()->getData().index == 0) {
        enableControlQuiz();
        this->addChildNode(lines[current_index],2);
        lines[current_index]->onShow(0);
        _slots.push_back(lines[current_index]);
    }else{
        this->addChildNode(lines[current_index],2);
        lines[current_index]->onShow(0);
        _slots.push_back(lines[current_index]);
    }
}
void Nam_IspyWithSortInLineQuiz::update(float dt){
    if (lines[current_index]->isUsed()){
        //if (yeu_cau_2 == "color"){
            if (lines[current_index]->getLinker() == shapes_question[current_index]){
                if (current_index < number_line-1){
                    this->nextQuestion();
                    current_index++;
                }
                else{
                    this->scheduleOnce([=](float dt) {
                        this->nextQuestion();
                    }, 0.5f, cocos2d::StringUtils::format("a"));
                }
            }else{
                this->scheduleOnce([=](float dt) {
                    lines[current_index]->getBackground()->setVisible(true);
                    auto obj = lines[current_index]->getLinker();
                    lines[current_index]->removeLinker();
                    obj->backToStart();
                }, 0.5f, cocos2d::StringUtils::format("a"));
            }
        //}
    }
}
void Nam_IspyWithSortInLineQuiz::onExplanation(bool correct) {}
void Nam_IspyWithSortInLineQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    float delay = showConratulation(0.3);
    float m_delay = delay - 0.3;
    m_delay += 0.5;
    for (Math_Object* object : _objects) {
        object->onHide(m_delay,math::animation::SlideUp);
    }
    for (Math_Slot* slot : _slots) {
        slot->onHide(m_delay,math::animation::SlideUp);
    }
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
    
}

#pragma mark - object touch events callback

void Nam_IspyWithSortInLineQuiz::onAnswerDone(Node* node, std::string str_ans) {
    Math_Answer* btn = (Math_Answer*)node;
    
}
void Nam_IspyWithSortInLineQuiz::onTouchedObject(Node* node) {
    Math_Object* object = (Math_Object*)node;
    if (selected_object == NULL) {
        selected_object = object;
    }
    else {
        object->setAllowMove(false);
    }
}

void Nam_IspyWithSortInLineQuiz::onMoveObjectEnded(Node* node, Vec2 position) {
    Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
    // check slot dich
    Math_Slot* t_slot = NULL;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    if (t_slot != NULL && !t_slot->isUsed()) {
        t_slot->getBackground()->setVisible(false);
        t_slot->setLinker(object);
        object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
    }
    else {
        //move start
        object->backToStart();
    }
    selected_object = NULL;
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void Nam_IspyWithSortInLineQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}

void Nam_IspyWithSortInLineQuiz::autoTestingRun(float delaytime)
{
    
}

