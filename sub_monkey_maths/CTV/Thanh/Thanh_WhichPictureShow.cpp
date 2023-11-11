//
//  Thanh_WhichPictureShow.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_WhichPictureShow.h"
#include "Math_Libs.h"

Thanh_WhichPictureShow* Thanh_WhichPictureShow::createQuiz()
{
    Thanh_WhichPictureShow* quiz=new Thanh_WhichPictureShow();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_WhichPictureShow::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_WhichPictureShow::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);
    
    Math_Quiz::initQuiz(game);
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    // TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;


    int maximum_group = json_game.FindMember("number_group")->value.FindMember("maximum")->value.GetInt();
    int minimum_group = json_game.FindMember("number_group")->value.FindMember("minimum")->value.GetInt();
    number_group = cocos2d::random(minimum_group, maximum_group);
 
    number_operand = json_game.FindMember("number_operand")->value.GetInt();

    int maximum_result = json_game.FindMember("result")->value.FindMember("maximum")->value.GetInt();
    int minimum_result = json_game.FindMember("result")->value.FindMember("minimum")->value.GetInt();

    object_size = cocos2d::Size(json_game.FindMember("object_size")->value.FindMember("width")->value.GetInt(),
                                json_game.FindMember("object_size")->value.FindMember("height")->value.GetInt());

    str = json_game.FindMember("game_mode")->value.GetString();
    if(str == "add"){
        game_mode = GameMode::add;
    }else if(str == "sub"){
        game_mode = GameMode::sub;
        number_operand = 2;
    }

    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;

    str = json_group.FindMember("group_type")->value.GetString();
    if(str == "queue"){
        group_type = math::group::GroupType::queue;
    }else if(str == "frame"){
        group_type = math::group::GroupType::frame;
    }


    //TODO:
    //lay tat ca icon trong media_set

    std::random_device rd;
    std::mt19937 g(rd());

	background = math::resource::ResHelper::createImage("themes/default/themeui/box.png");

    // lay ra cac icon group
    auto list_icon = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
    std::vector<math::resource::Image*> icons;
    std::vector<math::resource::Image*>::iterator it;
    for(int i = 0; i < number_operand; i++){
        it = list_icon.begin() + (int)cocos2d::random(0, (int)list_icon.size() - 1);
        math::resource::Image *img = *it;
        icons.push_back(img);
        list_icon.erase(it);
    }

    // gen value
    result = cocos2d::random(3 - 2*(int)(game_mode == GameMode::sub), 10 - (int)(game_mode == GameMode::sub));
    std::vector<int> temp; // vector gia tri toan hang
    std::vector<int> number_in_question;
    for(int i = 0; i < number_group; i++){
        bool ok = false;
        while(!ok){
            ok = true;
            temp.clear();
            // sinh gia tri toan hang
            if(game_mode == GameMode::add){
                int tmp = cocos2d::random(minimum_result, maximum_result);
                if(i == 0) tmp = result;
                if(result > 5) {
                    minimum_result = result - 1 < minimum_result? minimum_result : result - 1;
                    maximum_result = result + 1 > maximum_result? maximum_result : result + 1;
                } else if(result <= 5) {
                    minimum_result = result - 2 < minimum_result? minimum_result : result - 2;
                    maximum_result = result + 2 > maximum_result? maximum_result : result + 2;
                }
                for(int j = 0; j < number_operand - 1; j++){
                    temp.push_back(cocos2d::random(1, tmp - 1));
                    tmp -= temp[temp.size() - 1];
                    if(tmp <= 0 || tmp > 10) ok = false;
                }
                temp.push_back(tmp);
            }
            else if(game_mode == GameMode::sub){
                int tmp = cocos2d::random((result - 2 > 1? result - 2 : 1), (result + 2> 9 ? 9 : result + 2));
                if(i == 0) tmp = result;
                temp.push_back(tmp);
                // kiểm soát random không quá cách xa đáp án đúng
                int number = cocos2d::random(1, (10 > result + 3? result + 3: 10) - tmp);
                if(number > 8){
                    number = cocos2d::random(1, (10 > result + 3? result + 3: 10) - tmp);
                }
                temp.push_back(number);
                for(std::vector<int>::iterator number = temp.begin(); number != temp.end(); number++){
                    if(*number > 10 || *number < 1) ok = false;
                }
            }
            // kiem tra bộ toán hạng đã xuất hiện chưa
            for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){
                std::vector<int>::iterator index = temp.begin();
                int cnt = 0;
                for(std::pair<math::resource::Image*, int> p: v){
                    if(*index == p.second) cnt++;
                    index++;
                }
                if(cnt == number_operand) ok = false;
            }
            // đưa toán hạng được nhận vào vector cặp toán hạng và ảnh object
            if(ok){
                std::vector<std::pair<math::resource::Image*, int> > tmp;
                for(int index = 0; index < number_operand; index++){
                    tmp.push_back(std::make_pair(icons[index], temp[index]));
                    if(i==0) number_in_question.push_back(temp[index]);
                }
                icons_group.push_back(tmp);
            }
        }
    }

    // icon cho cau hoi
    if(game_mode == GameMode::add){
        int result_question = 0;
        for(int i = 0; i < number_in_question.size(); i++){
            img_list.push_back(getNumbers(number_in_question[i], number_in_question[i]).at(0));
            result_question += number_in_question[i];
        }
        img_list.push_back(getNumbers(result_question, result_question).at(0));
    }
    else if(game_mode == GameMode::sub){
        int result_question = number_in_question[0] + number_in_question[1];
        img_list.push_back(getNumbers(result_question, result_question).at(0));
        for(int i = 0; i < number_in_question.size(); i++){
            img_list.push_back(getNumbers(number_in_question[i], number_in_question[i]).at(0));
        }
    }

    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    // optional
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list);
        }
    }

    // TODO: gen object, group
    int obj_id = 0;
    int gr_id = 0;
    auto sort_key = getComparisionKey("kind");

    // find maximum_cap
    maximum_cap = 0;
    for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){
        int sum = 0;
        for(std::pair<math::resource::Image*, int> p: v){
            sum += p.second;
        }
        if(sum > maximum_cap) maximum_cap = sum;
    }

    CCLOG("gen group");
    // gen group
    if(game_mode == GameMode::add){
        for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){
            Math_Group *group = Math_Pool::getInstance()->getGroup();
            group->setComparisionKey(sort_key);
            std::vector<Math_Object*> _group_list;
            for(int i = 0; i < v.size(); i++){
                std::pair<math::resource::Image*, int> p = v[i];
                math::resource::Image *img = p.first;
                int value = p.second;
                for(int j = 0; j < value; j++)
                {
                    Math_Object* object = Math_Pool::getInstance()->getObject();
                    object->setEnable(obj_id++, math::common::deny, img, img);
                    object->setSelectedAnimation(math::common::SelectedAnimation::scale);
                    object->setSelected(false);
                    object->setBackgroundVisible(true);
                    object->setImageVisible(false);
                    object->setObjectSize(object_size);
                    object->setOpacity(255);
                    object->setValueText("");
                    object->onShow(0);
                    _objects.push_back(object);
                    _group_list.push_back(object);
                }
            }
            while(_group_list.size() < maximum_cap){
                Math_Object* object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, background, background);
                object->setBackgroundVisible(false);
                object->setImageVisible(false);
                object->setObjectSize(object_size);
                object->setOpacity(255);
                object->setValueText("");
                object->onShow(0);
                _objects.push_back(object);
                _group_list.push_back(object);
            }
            group->setObjectSize(object_size);
            group->setEnable(gr_id++, _group_list, group_type, (int)_group_list.size());
            group->enableToch(CC_CALLBACK_1(Thanh_WhichPictureShow::onTouchedGroup, this));
            if(group_type == math::group::GroupType::frame){
                group->setGroupFrames(maximum_cap, 1, background);
                group->setBackgroundVisiable(false);
            }
            else {
                group->setBackground(background);
                group->setGroupQueue(cocos2d::Size(object_size.width*maximum_cap*1.2 + 40, object_size.height*1.2 + 40));
                group->setBackgroundVisiable(true);
            }
            list_group.push_back(group);
        }
    }else if(game_mode == GameMode::sub){
        math::resource::Image *img = icons_group[0][0].first;
        for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){
            Math_Group *group = Math_Pool::getInstance()->getGroup();
            group->setComparisionKey(sort_key);
            std::vector<Math_Object*> _group_list;
            for(int i = 0; i < v.size(); i++){
                std::pair<math::resource::Image*, int> p = v[i];
                int value = p.second;
                for(int j = 0; j < value; j++)
                {
                    Math_Object* object = Math_Pool::getInstance()->getObject();
                    object->setEnable(obj_id++, math::common::deny, img, img);
                    object->setSelectedAnimation(math::common::SelectedAnimation::scale);
                    object->setSelected(false);
                    object->setBackgroundVisible(true);
                    object->setImageVisible(false);
                    object->setObjectSize(object_size);
                    if(i == 1){
                        object->setValueText("X");
                        object->getLabelValue()->setFontSize(object_size.height*1.2);
                        object->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
                        object->showValueTextWithAnimation(true);
                    }
                    else object->setValueText("");
                    object->setOpacity(255);
                    object->onShow(0);
                    _objects.push_back(object);
                    _group_list.push_back(object);
                }
            }
            while(_group_list.size() < maximum_cap){
                Math_Object* object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, background, background);
                object->setBackgroundVisible(false);
                object->setImageVisible(false);
                object->setObjectSize(object_size);
                object->setOpacity(255);
                object->setValueText("");
                object->onShow(0);
                _objects.push_back(object);
                _group_list.push_back(object);
            }
            group->setObjectSize(object_size);
            group->setEnable(gr_id++, _group_list, group_type, (int)_group_list.size());
            group->enableToch(CC_CALLBACK_1(Thanh_WhichPictureShow::onTouchedGroup, this));
            if(group_type == math::group::GroupType::frame){
                group->setGroupFrames(maximum_cap, 1, background);
                group->setBackgroundVisiable(false);
            }
            else {
                group->setBackground(background);
                group->setGroupQueue(cocos2d::Size(object_size.width*maximum_cap*1.2 + 20, object_size.height*1.2 + 10));
                group->setBackgroundVisiable(true);
            }
            list_group.push_back(group);
        }
    }

    correct_group = list_group[0];
    std::shuffle(list_group.begin(), list_group.end(), g);

    //Math_Pool::getInstance()->resetImages();
}

void Thanh_WhichPictureShow::onStart() {
    CCLOG("%s override", __func__);

    // show group
    auto rect = cocos2d::Rect(0, 0.12, 1, 0.65);
    math::func::setPositionList(list_group, 1, rect);
    for(Math_Group *group: list_group){
        addChildNode(group, 2);
		group->setBackgroundVisiable(false);
        group->onShow();
		cocos2d::ui::ImageView *background = cocos2d::ui::ImageView::create("themes/default/themeui/box.png");
		background->setScale9Enabled(true);
		background->setCapInsets(cocos2d::Rect(20, 20, background->getVirtualRendererSize().width - 40, background->getVirtualRendererSize().height - 40));
		background->setContentSize(group->getContentSize());
		addChildNode(background, 1);
		background->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, group->getPosition().y));
		background->setVisible(true);
    }
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.2), cocos2d::CallFunc::create([=](){
        for(Math_Group *group: list_group){
            for(int i = 0; i < group->getNumberObjects(); i++){
                Math_Object *object = group->getObjectAtIndex(i);
                object->getImage()->setContentSize(object_size*1.2);
            }
        }
    }), nullptr));
    
    //Gen question
    showQuestionBox(0);
}

void Thanh_WhichPictureShow::startQuestion() {
    CCLOG("%s override", __func__);

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();

        for (Math_Answer* done :  _answers) {
            done->onShowInParent(_panel, 2);
        }
    }
}

void Thanh_WhichPictureShow::onExplanation(bool correct) {
    CCLOG("%s override", __func__);

    if(correct) {
        disableControlQuiz(5);
        this->nextQuestion();
    }else {
        disableControlQuiz(0.7);
        if(group_be_touch == nullptr) return;
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.7), cocos2d::CallFunc::create([=](){
            for(int i = 0; i < group_be_touch->getNumberObjects(); i++){
                Math_Object *object = group_be_touch->getObjectAtIndex(i);
                object->setSelected(false);
            }
            group_be_touch = nullptr;
        }), nullptr));
    }
}

void Thanh_WhichPictureShow::onComplete() {
    CCLOG("%s override", __func__);

    //Math_Pool::getInstance()->resetAnswers();

    float delay = showConratulation(0.3);

    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }

    m_delay= delay - 0.3;

    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay) ,cocos2d::CallFunc::create([=](){
        for(Math_Group *group: list_group){
            group->setDisable();
        }
    }), nullptr));

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void Thanh_WhichPictureShow::onAnswerDone(bool correct)
{
    bool done=true;
    if(group_be_touch != correct_group) done = false;
    this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_WhichPictureShow::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_WhichPictureShow::autoTestingRun(float dt)
{
    
}

#pragma mark - on this Thanh_WhichPictureShow
void Thanh_WhichPictureShow::onTouchedGroup(cocos2d::Node* node)
{
    Math_Group *group = (Math_Group*)node;
    if(group != correct_group){
        group->onTouchedAnimation(true);
    }else{
        for(int i = 0; i < group->getNumberObjects(); i++) {
            Math_Object *object  = group->getObjectAtIndex(i);
            object->setSelected(true);
        }
        onExplanation(true);
    }
}

