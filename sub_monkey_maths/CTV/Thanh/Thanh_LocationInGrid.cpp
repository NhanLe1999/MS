//
//  Thanh_LocationInGrid.cpp
//  MJQuiz
//
//  Created by Daniel on 11/23/17.
//
//

#include "Thanh_LocationInGrid.hpp"
#include "Math_Libs.h"

Thanh_LocationInGrid* Thanh_LocationInGrid::createQuiz()
{
    Thanh_LocationInGrid* quiz=new Thanh_LocationInGrid();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_LocationInGrid::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void Thanh_LocationInGrid::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);

    Math_Quiz::initQuiz(game);

    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    _list_location = {
        {"top_left", 0, "top left"},
        {"middle_top", 1, "middle top"},
        {"top_right", 2, "top right"},
        {"middle_left", 3, "middle left"},
        {"center", 4, "center"},
        {"middle_right", 5, "middle right"},
        {"bottom_left", 6, "bottom left"},
        {"middle_bottom", 7, "middle bottom"},
        {"bottom_right", 8, "bottom right"}
    };

    //TODO: GAME CONFIG

    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    str = json_game.FindMember("mode")->value.GetString();
    if(str == "location_in_grid"){
        _mode = GameMode::location_in_grid;
    }else if(str == "which_location_include_object"){
        _mode = GameMode::which_location_include_object;
    }else if(str == "which_object_in_location"){
        _mode = GameMode::which_object_in_location;
    }

    str = json_game.FindMember("location")->value.GetString();
    if(str == "random"){
        _location = _list_location[cocos2d::random(0, (int)_list_location.size() - 1)];
    }
    else {
        for(int i = 0; i < _list_location.size(); i++){
            if(_list_location[i].key == str){
                _location = _list_location[i];
            }
        }
    }

    //TODO: GRID CONFIG
    _size = cocos2d::Size(120, 120);

    //TODO: get image, gen value
	_background = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));
 
    auto list_icon = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
    if(_mode == GameMode::location_in_grid){
        if(list_icon.size() < 1) {
            CCLOG("KHONG DU IMAGE");
            return;
        }
        std::vector<math::resource::Image*>::iterator it;
        for(int i = 0; i < 9; i++){
            it = list_icon.begin() + cocos2d::random(0, (int)list_icon.size() - 1);
            _icons.push_back(*it);
        }
    }
    else if(_mode == GameMode::which_object_in_location){
        if(list_icon.size() < 9) {
            CCLOG("KHONG DU IMAGE");
            return;
        }
        std::vector<math::resource::Image*>::iterator it;
        for(int i = 0; i < 9; i++){
            it = list_icon.begin() + cocos2d::random(0, (int)list_icon.size() - 1);
            _icons.push_back(*it);
            list_icon.erase(it);
        }
        _icons_question.push_back(_icons[_location.number]);
    }
    else if(_mode == GameMode::which_location_include_object){
        if(list_icon.size() < 9) {
            CCLOG("KHONG DU IMAGE");
            return;
        }
        std::vector<math::resource::Image*>::iterator it;
        for(int i = 0; i < 9; i++){
            it = list_icon.begin() + cocos2d::random(0, (int)list_icon.size() - 1);
            _icons.push_back(*it);
            list_icon.erase(it);
        }
        _icons_question.push_back(_icons[_location.number]);
    }


    //TODO: Xử lý dữ liệu cấu hình câu hỏi

    parseQuestion();
    CCLOG("%d location", _location.number);
    if(_mode == GameMode::location_in_grid){
        this->getQuestion()->setKeywordValue(_location.number);
    }
    else if(_mode == GameMode::which_location_include_object || _mode == GameMode::which_object_in_location){
        // optional
        auto opt_keys = getQuestion()->getOptionalKeys();
        for (std::string key : opt_keys) {
            if (key == "number") {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : _icons_question) {
                    auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                    img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else if (key == "number_text") {
                std::vector<math::resource::Text> texts;
                for (math::resource::Image* img : _icons_question) {
                    auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                    texts.push_back(img_txt);
                }
                getQuestion()->setOptionalTexts(key, texts);
            } else {
                getQuestion()->setOptionalTexts(key, _icons_question);
            }
        }

        // keyword
        this->getQuestion()->setKeywordValue(_location.number);
    }


    /*
     if (random_sorted) {
     random_shuffle(list_questiondata.begin(), list_questiondata.end());
     }
     */

    int obj_id = 0;
    int gr_id = 0;
    auto sort_key = getComparisionKey("number");

    // gen _grid
    _grid = Math_Pool::getInstance()->getGroup();
    _grid->setObjectSize(_size);
    _grid->setComparisionKey(sort_key);

    std::vector<Math_Object*> _group_list;
    for(math::resource::Image *img: _icons)
    {
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(obj_id++, math::common::touch, img, _background);
        object->setTouchCallback(CC_CALLBACK_1(Thanh_LocationInGrid::onTouchedObject, this));
        object->setSelectedAnimation(math::common::SelectedAnimation::scale);
        object->setBackgroundVisible(false);
        object->setImageVisible(true);
        object->setValueText("");
        object->setObjectSize(_size);
        object->setVisible(true);
        object->onShow(0);
        _objects.push_back(object);
        _group_list.push_back(object);
    }

    _grid->setEnable(gr_id++, _group_list, math::group::GroupType::frame, 1);
    _grid->setGroupFrames(3, 3, _background);
    _grid->updatePosition(false, false);
	_grid->setBackground(math::resource::ResHelper::createImage(getUIPath("common/frame/square.png")), 30);
    _grid->setBackgroundVisiable(true);
    _groups.push_back(_grid);

    if(_mode == GameMode::location_in_grid){
        _correct_answer = _grid->getObjectAtIndex(_location.number);
    }
    else if(_mode == GameMode::which_location_include_object){
        // gen answer
        std::vector<int> list_values;
        list_values.push_back(_location.number);
        while(list_values.size() < 4){
            bool ok = false;
            while(!ok){
                ok = true;
                int rd = cocos2d::random(0, 8);
                for(int k: list_values){
                    if(k == rd) ok = false;
                }
                if(ok) list_values.push_back(rd);
            }
        }
        for(int i = 0; i < 4; i++){
            _text_answers.push_back(_list_location[list_values[i]].text);
        }
    }
    else if(_mode == GameMode::which_object_in_location){
        std::vector<int> list_values;
        list_values.push_back(_location.number);
        while(list_values.size() < 4){
            bool ok = false;
            while(!ok){
                ok = true;
                int rd = cocos2d::random(0, 8);
                for(int k: list_values){
                    if(k == rd) ok = false;
                }
                if(ok) list_values.push_back(rd);
            }
        }
        for(int i = 0; i < 4; i++){
            _text_answers.push_back(_grid->getObjectAtIndex(list_values[i])->getImageSource()->getText("name_1"));
        }
    }


    //Math_Pool::getInstance()->resetImages();
}

void Thanh_LocationInGrid::onStart() {
    CCLOG("%s override", __func__);

    if(_mode == GameMode::location_in_grid){
        // show _grid
        _grid->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, _panel->getContentSize().height/2 + 10));
        addChildNode(_grid,2);
        _grid->onShow();

        // gen button done
        btn_done=Math_Pool::getInstance()->getAnswer();
        btn_done->setEnable("Done", math::common::done, CC_CALLBACK_1(Thanh_LocationInGrid::onAnswerDone, this), getAnswerSize());
		btn_done->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, 55));
        btn_done->setVisible(true);
        _answers.push_back(btn_done);
    }
    else if(_mode == GameMode::which_object_in_location){
        // show _grid
		onShowBottomBar();
        _grid->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, _panel->getContentSize().height/2 - 40));
        addChildNode(_grid,2);
        _grid->onShow();

        for(std::string text: _text_answers){
            Math_Answer* btn_answer=Math_Pool::getInstance()->getAnswer();
            btn_answer->setEnable(text, math::common::AnswerType::custom_bg, CC_CALLBACK_1(Thanh_LocationInGrid::onAnswerDone, this), cocos2d::Size(300, 60));
			btn_answer->setCustomAnswer(math::resource::ResHelper::createImage(getUIPath("common/done.png")), cocos2d::Size(300, 60), cocos2d::Size(300, 60).height*0.6, cocos2d::Color4B::WHITE, 0.2);
            btn_answer->setVisible(true);
            _answers.push_back(btn_done);
            _list_answer.push_back(btn_answer);
        }
        _correct_answer = _list_answer[0];
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(_list_answer.begin(), _list_answer.end(), g);
        math::func::setPositionList(_list_answer, 2, cocos2d::Rect(0, 0, 1, 0.2));
    }else {
        // show _grid
		onShowBottomBar();
		_grid->setPosition(cocos2d::Vec2(_panel->getContentSize().width / 2, _panel->getContentSize().height / 2 - 40));
        addChildNode(_grid,2);
        _grid->onShow();

        for(std::string text: _text_answers){
            Math_Answer* btn_answer=Math_Pool::getInstance()->getAnswer();
            btn_answer->setEnable(text, math::common::custom_bg, CC_CALLBACK_1(Thanh_LocationInGrid::onAnswerDone, this), cocos2d::Size(300, 60));
			btn_answer->setCustomAnswer(math::resource::ResHelper::createImage(getUIPath("common/done.png")), cocos2d::Size(300, 60), cocos2d::Size(300, 60).height*0.6, cocos2d::Color4B::WHITE, 0.2);
            btn_answer->setVisible(true);
            _answers.push_back(btn_done);
            _list_answer.push_back(btn_answer);
        }
        _correct_answer = _list_answer[0];
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(_list_answer.begin(), _list_answer.end(), g);
        math::func::setPositionList(_list_answer, 2, cocos2d::Rect(0, 0, 1, 0.2));
    }

    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.5), cocos2d::CallFunc::create([=](){
        for(int i = 0; i < _grid->getNumberObjects(); i++){
            _grid->getObjectAtIndex(i)->setBackgroundVisible(true);
            _grid->getObjectAtIndex(i)->getBackground()->setContentSize(_size*1.2);
        }
    }), nullptr));

    //Gen question
    showQuestionBox(); 
}

void Thanh_LocationInGrid::startQuestion() {
    CCLOG("%s override", __func__);

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();

        _disable = false;

        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=](){
            if(_mode == GameMode::location_in_grid){
                //#2. show button done 
                btn_done->onShowInParent(_panel, 3);
            }
            else if(_mode == GameMode::which_object_in_location || _mode == GameMode::which_location_include_object){
                // show answer
                for(Math_Answer* answer: _list_answer){ 
                    answer->onShowInParent(_panel_bot, 3);
                }
            }
        }), nullptr));
    };
}

void Thanh_LocationInGrid::onExplanation(bool correct) {
    CCLOG("%s override", __func__);

    _disable = true;

    if(correct){
        btn_done->disableClickAnswer();
        this->nextQuestion();
    }
    else{
        btn_done->onAnimationClick(false);
        btn_done->disableClickAnswer();
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.7), cocos2d::CallFunc::create([this](){
            btn_done->enableClickAnswer();
            _disable = false;
            if(_object_be_touch != nullptr){
				_object_be_touch->getBackground()->loadTexture( _background->path );
            }
        }), nullptr));
    }
}

void Thanh_LocationInGrid::onComplete() {
    CCLOG("%s override", __func__);

    //Math_Pool::getInstance()->resetAnswers();

    float delay = showConratulation(0.3);

    float m_delay=delay-0.5;
    if (m_delay < 0) {
        m_delay = 0;
    }
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=](){
        //#1. hide _grid hunder
        for(int i = 0; i < _grid->getNumberObjects(); i++){
            Math_Object *object = _grid->getObjectAtIndex(i);
            object->getBackground()->loadTexture( _background->path );
            object->getBackground()->setContentSize(_size);
            object->setBackgroundVisible(false);
        }
        _grid->setDisable();

    }), cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=](){
        //#1. hide answers
        for(Math_Answer* btn: _list_answer){
            btn->setDisable();
        }
    }), nullptr));

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void Thanh_LocationInGrid::onAnswerDone(cocos2d::Node* node)
{
    if(_disable) return;
    _disable = true;

    if(_mode == GameMode::location_in_grid){
        bool done=true;
        if(_mode == GameMode::location_in_grid){
            if(_object_be_touch != _correct_answer){
                done = false;
            }
        }
        this->onExplanation(done);
    }else {
        Math_Answer *btn = (Math_Answer*)node;
        if(btn == _correct_answer) {
            this->nextQuestion();
        }
        else {
            btn->onAnimationClick(false);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.8), cocos2d::CallFunc::create([=](){
                // show answer
                _disable = false;
            }), nullptr));
        }
    }
} 

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_LocationInGrid::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_LocationInGrid::autoTestingRun(float dt)
{

}

#pragma mark - touch function
void Thanh_LocationInGrid::onTouchedObject(cocos2d::Node* node){
    if(_mode == GameMode::location_in_grid)
    {
        Math_Object* object = (Math_Object*)node;
        if(_object_be_touch != nullptr){
            _object_be_touch->getBackground()->loadTexture( _background->path );
        }
        _object_be_touch = object;
        _object_be_touch->getBackground()->loadTexture(getUIPath("common/frame/brown1.png"));
    }
}

