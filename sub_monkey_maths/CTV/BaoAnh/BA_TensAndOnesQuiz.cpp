//
//  BA_TensAndOnesQuiz.cpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/2/17.
//


#include "BA_TensAndOnesQuiz.hpp"

BA_TensAndOnesQuiz* BA_TensAndOnesQuiz::createQuiz()
{
    BA_TensAndOnesQuiz* quiz=new BA_TensAndOnesQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool BA_TensAndOnesQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void BA_TensAndOnesQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    
    //GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    answer_checked = json_game.FindMember("answer")->value.GetBool();
    std::string object_kind = json_game.FindMember("kind")->value.GetString();
    number_min = json_game.FindMember("number")->value.FindMember("minimum")->value.GetInt();
    number_max = json_game.FindMember("number")->value.FindMember("maximum")->value.GetInt();
    std::string answer_position = json_game.FindMember("position")->value.GetString();
    
    bool is_answer_right = true;
    if (answer_position == "left") {
        is_answer_right = false;
    } else if (answer_position == "random") {
        is_answer_right = cocos2d::random(0, 1) == 0;
    }
    int display_level = json_game.FindMember("display_level")->value.GetInt();
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    std::string display_value = json_object.FindMember("display_value")->value.GetString();
    
    //============================
    //Icon config
    total_object = cocos2d::random(number_min, number_max);
    
    int number_ask = total_object;
    int thousands = number_ask / 1000;
    number_ask = number_ask % 1000;
    int hundreds = number_ask / 100;
    number_ask = number_ask % 100;
    int tens = number_ask / 10;
    int ones = number_ask % 10;
    
    int display = 4;
    if (thousands==0) {
        display--;
        if (hundreds==0) {
            display--;
        }
    }
    if (display > display_level) {
        display_level = display;
    }
    
    auto slot_bg = math::resource::ResHelper::createImage(getUIPath("common/drop.png"));
    img_bg = math::resource::ResHelper::createImage(getUIPath("common/frame.png"));
    
    int display_type = 0;
    if(!answer_checked) {
        auto all_images = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
        std::vector<math::resource::Image*> icons_list;
        std::vector<math::resource::Image*> block_list;
        for (math::resource::Image* img : all_images) {
            if (img->getComparisionValue("role") == 0) {
                icons_list.push_back(img);
            } else {
                block_list.push_back(img);
            }
        }
        
        if (total_object < 21) {
            display_type = 1;
            if (icons_list.empty()) {
                icons_list = all_images;
            }
            img_list = getImageList(icons_list, 1, "none");
        }else{
            display_type = 2;
            if (block_list.empty()) {
                block_list = all_images;
            }
            img_list = getImageList(block_list, display_level+1, "id", "color");
            std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image*b)->bool{
                return a->getComparisionValue("number") < b->getComparisionValue("number");
            });
        }
    }
    
    cocos2d::Size item_size = cocos2d::Size(70, 70);
    if (display_type == 1) {
        //nhom icon bang group
        std::vector<int> number_objects;
        if (total_object > 10) {
            number_objects.push_back(10);
            number_objects.push_back(total_object-10);
        } else {
            number_objects.push_back(total_object);
        }
        for (int i1=0; i1<number_objects.size(); i1++) {
            std::vector<Math_Object*> group_object;
            for (int i2=0; i2<number_objects[i1]; i2++) {
                auto object = mpool->getObject();
                object->setEnable(i2, math::common::deny, img_list.front(), nullptr);
                object->setObjectSize(item_size);
                group_object.push_back(object);
            }
            auto group = mpool->getGroup();
            group->setEnable(i1, group_object, math::group::frame, 10);
            group->setObjectSize(item_size);
            group->setGroupFrames(5, 2, img_bg);
            addChildNode(group);
            _groups.push_back(group);
        }
        math::func::setPositionList(_groups, (int)_groups.size(), cocos2d::Rect(0.05, 0.35, 0.9, 0.2));
    } else if(display_type == 2) {
        //-------------------------------------
        // calculate row and column
        int fives = ones / 5;
        int fix_ones = ones % 5;
        
        if (fix_ones > 0) {
            column = hundreds + tens + fives + 1;
        } else {
            column = hundreds + tens + fives;
        }
        item_size = cocos2d::Size(35, 35);
        
        float position_y_tens = 350;
        float position_x = (quiz_size.width - column * item_size.width * 1.5 + item_size.width*1.5) / 2;
        
        for (int i=0; i<hundreds; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(obj_id, math::common::deny, img_list[3], nullptr);
            obj->setObjectSize(item_size);
            obj->fixWidthObject();
            obj->setPosition(cocos2d::Vec2(position_x, position_y_tens));
            position_x+=(item_size.width*1.5);
            _objects.push_back(obj);
            blocks_object.push_back(obj);
        }
        
        for (int i=0; i<tens; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(obj_id, math::common::deny, img_list[2], nullptr);
            obj->setObjectSize(item_size);
            obj->fixWidthObject();
            obj->setPosition(cocos2d::Vec2(position_x, position_y_tens));
            position_x+=(item_size.width*1.5);
            _objects.push_back(obj);
            blocks_object.push_back(obj);
        }
        
        float position_y_fives = 140;
        if (blocks_object.size()>0) {
            position_y_fives = position_y_tens- (blocks_object.at(hundreds)->getContentSize().height/2);
        }
        for (int i=0; i<fives; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(obj_id, math::common::deny, img_list[1], nullptr);
            obj->setObjectSize(item_size);
            obj->fixWidthObject();
            obj->setPosition(cocos2d::Vec2(position_x, position_y_fives+obj->getContentSize().height/2));
            position_x+=(item_size.width*1.5);
            _objects.push_back(obj);
            blocks_object.push_back(obj);
        }
        
        int position_y_ones = position_y_fives;
        for (int i=0; i<fix_ones; i++) {
            Math_Object* obj = Math_Pool::getInstance()->getObject();
            obj->setEnable(obj_id, math::common::deny, img_list[0], nullptr);
            obj->setObjectSize(item_size);
            obj->fixWidthObject();
            obj->setPosition(cocos2d::Vec2(position_x, position_y_ones + obj->getContentSize().height/2));
            position_y_ones+=(item_size.height*1.5);
            _objects.push_back(obj);
            ones_object.push_back(obj);
        }
    }
    
    //gia tri moi don vi
    std::vector<int> number_data;
    number_data.push_back(ones);
    number_data.push_back(tens);
    if (display_level == 3) {
        number_data.push_back(hundreds);
    } else if (display_level == 4) {
        number_data.push_back(hundreds);
        number_data.push_back(thousands);
    }
    
    number_list = getNumbers(0, 9);
    std::vector<math::resource::Image*> number_img_list;
    for (int value : number_data) {
        for (math::resource::Image* img : number_list) {
            if (img->getComparisionValue("number") == value) {
                number_img_list.push_back(img);
                break;
            }
        }
    }
    
    auto operator_add = getOperator("+");
    auto operator_equal = getOperator("=");
    
    object_size = cocos2d::Size(80, 80);
    
    for (int i=0; i<number_data.size(); i++) {
        auto place = mpool->getObject();
        place->setEnable(i, math::common::deny, number_img_list.front(), nullptr);
        place->showValueTextWithAnimation(false);
        place->setImageVisible(false);
        if (i==0) {
            place->setValueText("ones");
        } else if (i==1) {
            place->setValueText("tens");
        } else if (i==2) {
            place->setValueText("hundreds");
        } else {
            place->setValueText("thousands");
        }
        place->setObjectSize(object_size);
        place->getLabelValue()->setFontSize(object_size.height*0.6);
        //place->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
        //place->setObjectSize(cocos2d::Size(place->getLabelValue()->getContentSize().width, object_size.height));
        addChildNode(place);
        expression.push_back(place);
        
        auto number_value = mpool->getObject();
        number_value->setEnable(i, math::common::deny, number_img_list[i], nullptr);
       // number_value->showValueTextWithAnimation(false);
        number_value->setImageVisible(false);
        number_value->setValueTextByKey("number");
       // number_value->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
        number_value->setObjectSize(object_size);
        number_value->setTag(0);
        addChildNode(number_value);
        expression.push_back(number_value);
        
        if (i<number_data.size()-1) {
            auto operator_value = mpool->getObject();
            operator_value->setEnable(i, math::common::deny, operator_add, nullptr);
            operator_value->showValueTextWithAnimation(false);
            operator_value->setImageVisible(false);
            operator_value->setValueTextByKey("name_1");
            operator_value->setObjectSize(object_size);
            operator_value->getLabelValue()->setFontSize(object_size.height*0.6);
//            operator_value->getLabelValue()->setTextColor(cocos2d::Color4B(221,66,60 ,255));
            //operator_value->setObjectSize(cocos2d::Size(operator_value->getLabelValue()->getContentSize().width, object_size.height));
            addChildNode(operator_value);
            expression.push_back(operator_value);
        }
    }
    std::reverse(expression.begin(), expression.end());
    
    //--------------------------
    //Gen equal
    auto equal = mpool->getObject();
    equal->setEnable(0, math::common::deny, operator_equal, nullptr);
    equal->showValueTextWithAnimation(false);
    equal->setImageVisible(false);
    equal->setValueTextByKey("name_1");
    equal->setObjectSize(object_size);
    equal->getLabelValue()->setFontSize(object_size.height*0.6);
//    equal->getLabelValue()->setTextColor(cocos2d::Color4B(221,66,60 ,255));
    //equal->setObjectSize(cocos2d::Size(equal->getLabelValue()->getContentSize().width, object_size.height));
    addChildNode(equal);
    expression.push_back(equal);
    
    //--------------------------
    //Gen result
    result = Math_Pool::getInstance()->getObject();
    result->setEnable(0, math::common::deny, img_bg, NULL);
    result->setImageVisible(false);
    if(display_type == 0){
        result->setValueText(cocos2d::StringUtils::toString(total_object));
    } else{
        result->setValueText("?");
    }
    result->setObjectSize(object_size);
    result->showValueTextWithAnimation(false);
    result->getLabelValue()->setFontSize(object_size.height*0.75);
//    result->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
    addChildNode(result);
    expression.push_back(result);
    
    if(!is_answer_right){
        std::rotate(expression.begin(), expression.begin()+expression.size()-1, expression.end());
        std::rotate(expression.begin()+1, expression.begin()+expression.size()-1, expression.end());
    }
    
    
    //set position list
    std::vector<cocos2d::Vec2> location_list;
    if(display_type != 0){
        if(_objects.size()!=0){
            for(int i = 0; i < _objects.size(); i++){
                Math_Object* obj = _objects[i];
                addChildNode(obj);
                obj->onShow(1);
            }
        }
        math::func::setPositionList(expression, (int) expression.size(),cocos2d::Rect(0.1, 0.05, 0.8, 0.2));
    } else {
        math::func::setPositionList(expression, (int) expression.size(),cocos2d::Rect(0.1, 0.25, 0.8, 0.2));
    }
    
    
    
    std::vector<cocos2d::Vec2> slot_positions;
    for(int i = 0 ; i < expression.size(); i++){
        if(expression.at(i)->getTag() == 0){
            slot_positions.push_back(expression.at(i)->getPosition());
        }
    }
    
    //--------------------------
    //Gen slot
    for(int i = 0; i< number_data.size(); i++){
        Math_Slot* slot = Math_Pool::getInstance()->getSlot();
        slot->setEndble(i, slot_bg) ;
        slot->setSlotSize(object_size);
        //slot->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
        slot->setPosition(slot_positions.at(i));
        addChildNode(slot);
        _slots.push_back(slot);
    }
    
    //--------------------------
    //Gen drager
    number_drag = number_data;
    sort( number_drag.begin(), number_drag.end() );
    number_drag.erase( unique( number_drag.begin(), number_drag.end() ), number_drag.end() );
    auto img_numbers = getNumbers(0, 9);
   // number_drag = {0,1,2,3,4,5,6,7,8,9};
    while(number_drag.size() < 4){
        int rand = cocos2d::random(0, 9);
        bool add = true;
        for(int i = 0; i < number_drag.size(); i++){
            if(rand == number_drag.at(i)){
                add = false;
            }
        }
        if(add){
            number_drag.push_back(rand);
        }
    }
    
    
    std::vector<math::resource::Image*> answer_imgs;
    for (int value : number_drag) {
        for (math::resource::Image* img : img_numbers) {
            if (img->getComparisionValue("number") == value) {
                answer_imgs.push_back(img);
                break;
            }
        }
    }
    
    drag_img = objectBG();
    for(int i = 0; i < number_drag.size(); i++){
        Math_Object* drager = Math_Pool::getInstance()->getObject();
        drager->setEnable(number_drag.at(i), math::common::deny, answer_imgs[i], drag_img);
        drager->setImageVisible(false);
        drager->setObjectSize(object_size);
        drager->setValueTextByKey("number");
        drager->getLabelValue()->setFontSize(object_size.height*0.75);
        //        drager->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
        drager->showValueTextWithAnimation(false);
        drager->setMoveCallback(nullptr, CC_CALLBACK_2(BA_TensAndOnesQuiz::onMoveObjectEnded, this));
        addChildNodeToBottomPanel(drager, 3);
        _dragers.push_back(drager);
    }
    
    math::func::setPositionList(_dragers, (int)_dragers.size(), cocos2d::Rect(quiz_size.width*0.02, quiz_size.height*0, quiz_size.width*0.7, 250));
    //PARSING QUESTION
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //Math_Pool::getInstance()->resetImages();
}

void BA_TensAndOnesQuiz::onStart() {
    
    if(!_groups.empty()){
        for(Math_Group* gr: _groups){
            gr->onShow();
        }
    }
    
    float total_width = 0;
    for (Math_Object* object : expression) {
        total_width += object->getContentSize().width;
    }
    
    //onShow expression
    for(int i = 0; i < expression.size(); i++){
        expression[i]->onShow(0.5);
    }
    //onShow dragers
    for(int i = 0; i < _dragers.size(); i++){
        _dragers[i]->onShow(0.5);
    }
    //onShow slots
    for(int i = 0; i < _slots.size(); i++){
        _slots[i]->onShow(0.5);
    }
    
    //---------------------
    onShowBottomBar(0, 250.0/quiz_size.height, true, CC_CALLBACK_1(BA_TensAndOnesQuiz::onAnswerDone, this));
    showQuestionBox(0);
    
}

void BA_TensAndOnesQuiz::startQuestion() {
    enableControlQuiz();

    if (this->getQuestion()->getData().index==0) {
        for(int i = 0; i < _dragers.size(); i++){
            _dragers[i]->enableDrag(_panel_bot);
        }
        
    }
   
}

void BA_TensAndOnesQuiz::onExplanation(bool correct) {
    if (!correct) {
        enableControlQuiz();
        //return wrong drager
        for (Math_Slot* slot : _slots) {
            if (slot->isUsed() ) {
                auto linker = slot->getLinker();
                slot->removeLinker();
                linker->onHide(0);
            }
            else{
                
            }
        }
    }
    else {
        if(!answer_checked){
            result->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.2),cocos2d::CallFunc::create([=](){
                result->setValueText(cocos2d::StringUtils::toString(total_object));
            }),cocos2d::DelayTime::create(0.3),cocos2d::FadeIn::create(0.2), NULL));
            
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1), cocos2d::CallFunc::create([=](){
                this->nextQuestion();
            }), NULL));
            
        }else{
            this->nextQuestion();
        }
        
    }
}

void BA_TensAndOnesQuiz::onComplete() {
    //TODO: Thuc hien them cac lenh o day
    for (Math_Answer* node : _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }
    
    float delay = showConratulation(0.3);
    
    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}


#pragma mark - object touch events callback
void BA_TensAndOnesQuiz::onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 position)
{
    disableControlQuiz(0.3);
    bool create = false;
//    int index;
    Math_Object* object=(Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
    
//    for(int i = 0; i < number_drag.size(); i++){
//        if(object->getID() == number_drag.at(i)){
//            index = i;
//        }
//    }
    
    //Doi tuong keo tha nam trong 1 slot nao do.
    Math_Slot* m_slot = NULL;
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            m_slot = slot;
            break;
        }
    }
    
    Math_Slot* t_slot = NULL;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }
    
    if (m_slot!=NULL) {
        if (t_slot != NULL) {
            //swap
            if (m_slot == t_slot) {
                object->backToLast(2);
            } else {
                if (t_slot->isUsed()) {
                    auto linker = t_slot->getLinker();
                    m_slot->removeLinker();
                    t_slot->removeLinker();
                    
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                    
                    m_slot->setLinker(linker);
                    linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);
                    object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
                }
            }
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
//            object->setPosition(cocos2d::Vec2(0, -1000));
//            object->onHide(0);
        }
    } else {
        if (t_slot != NULL) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->setPosition(cocos2d::Vec2(0, -1000));
                linker->onHide(0);
            }
            t_slot->setLinker(object);
            object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
            create = true;
        } else {
            //move start
            object->backToStart();
        }
    }
    
    if(create){
        Math_Object* obj = Math_Pool::getInstance()->getObject();
        obj->setEnable(object->getID(), math::common::move, object->getImageSource(), drag_img );
        obj->setImageVisible(false);
        obj->setValueTextByKey("number");
        obj->setObjectSize(object_size);
        obj->showValueTextWithAnimation(false);
        obj->getLabelValue()->setFontSize(object_size.height*0.75);
//        obj->getLabelValue()->setTextColor(cocos2d::Color4B(114,55,24 ,255));
        obj->setPosition(object->getStartPosition());
        obj->setMoveCallback(nullptr, CC_CALLBACK_2(BA_TensAndOnesQuiz::onMoveObjectEnded, this));
        addChildNodeToBottomPanel (obj,3);
        obj->onShow(0);
        obj->enableDrag(_panel_bot);
    }
}

void BA_TensAndOnesQuiz::onAnswerDone(bool correct){
    disableControlQuiz(0.5);
    
    bool done = true;
    
    for (Math_Slot* slot : _slots) {
        if (!slot->isUsed()){
            done = false;
            break;
        }
    }
    
    if(done){
        int check = 0;
        for(int i = 0 ; i < _slots.size(); i++){
            Math_Slot* slot = _slots.at(i);
            check = check * 10 + slot->getLinker()->getComparisionValue().number;
        }
        if(check != total_object){
            done = false;
        }
    }
   // _answers.front()->onAnimationClick(done);
    this->onExplanation(done);
    
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 4
void BA_TensAndOnesQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void BA_TensAndOnesQuiz::autoTestingRun(float delaytime)
{
    _answers[0]->onAutoClick();
}

