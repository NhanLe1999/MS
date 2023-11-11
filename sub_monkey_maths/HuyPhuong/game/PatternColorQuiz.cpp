//
//  PatternColorQuiz.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#include "PatternColorQuiz.h"

PatternColorQuiz* PatternColorQuiz::createQuiz()
{
    PatternColorQuiz* quiz = new PatternColorQuiz();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }
    
    CC_SAFE_DELETE(quiz);
    return NULL;
}

PatternColorQuiz::~PatternColorQuiz()
{
    if(pattern)
    {
        CC_SAFE_DELETE(pattern);
    }
}

bool PatternColorQuiz::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle
void PatternColorQuiz::initQuiz(mj::model::Game game)
{
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data=document.FindMember("data")->value;
    //TODO: GAME CONFIG
    rapidjson::Value& json_game=json_data.FindMember("game")->value;
    type_game = getJsonString(json_game, "game_play", "continue");
    number_object = randomRangeJson(json_game, "number");
    number_slot = randomRangeJson(json_game, "missing");
    
    //TODO: OBJECT CONFIG
    rapidjson::Value& json_object=json_data.FindMember("object")->value;
    object_size = getSizeJson(json_object, "size");
    object_bg = getJsonString(json_object, "background", "none");
    
    //TODO: PATTERN CONFIG
    rapidjson::Value& json_pattern=json_data.FindMember("pattern")->value;
    int pattern_piece = randomRangeJson(json_pattern, "piece");
    int pattern_size = randomRangeJson(json_pattern, "size");
    std::string pattern_type = json_pattern.FindMember("type")->value.GetString();
    std::string pattern_kind = json_pattern.FindMember("class")->value.GetString();
    if (pattern_kind == "growing") {
        pattern_type = pattern_kind;
    }
    //TODO: Chon icon
    //lay tat ca icon trong media_set.
    //Tong so icon can lay = so kind trong moi group
    rapidjson::Value& json_icon = json_data.FindMember("icon")->value;

    std::string icon_filter = getJsonString(json_icon, "different", "id");
    std::string icon_same = getJsonString(json_icon, "same", "none");

    auto all_imgs = getImageList("icons");
    auto selected_img = math::func::getRandomNode(all_imgs);
    for (math::resource::Image*img : all_imgs) {
        if (img->getText() == selected_img->getText()) {
            if (img->getText("color") == "white") {
                img_list.push_back(img);
            } else {
                img_list_color.push_back(img);
            }
        }
    }
    math::func::shuffle(img_list_color);
    while (img_list_color.size() > pattern_piece) {
        img_list_color.pop_back();
    }
    
    pattern=new PatternCreator(pattern_piece, pattern_size, pattern_type);
    if (pattern_type.compare("size")==0) {
        key_compare = math::comparision::size;
    }
    
    for (int i=0; i<pattern_piece; i++) {
        pattern->addPatternImage(img_list_color[i]);
        if (pattern_type.compare("size")==0) {
            pattern->addPatternSize(i);
        } else {
            pattern->addPatternSize(0);
        }
    }
    if (pattern_kind == "growing") {
        pattern->setGrowingType(PatternCreator::PatternGrowingType::Pattern_3);
    }
    if (type_game.compare("continue")==0) {
        pattern->setPatternListSize(number_object);
    } else {
        pattern->setPatternListSize(number_object*2);
    }
    /*---> Lay xong data cua game*/
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
    
    //TODO: GENERATE QUIZ COMPONENTS
    //Gen doi tuong
    //Line 1
    for (int i=0; i<number_object; i++) {
        Math_Object* object=mpool->getObject();
        if (type_game.compare("line")==0) {
            object->setEnable(i, math::common::deny, pattern->getImageAt(i), img_bg);
        } else {
            if (i<number_object-number_slot) {
                object->setEnable(i, math::common::deny, pattern->getImageAt(i), img_bg);
            } else {
                //white icon
                object->setEnable(i, math::common::touch, img_list.front(), img_bg, true);
                object->setTouchCallback(CC_CALLBACK_1(PatternColorQuiz::onTouchedObject, this));
                white_objects.push_back(object);
            }
        }
        object->setObjectSize(object_size);
        _objects.push_back(object);
    }
    
    //Line 2
    if (type_game.compare("continue")!=0) {
        for (int i=0; i<number_object; i++) {
            Math_Object* object=mpool->getObject();
            object->setEnable(i, math::common::touch, img_list.front(), img_bg, true);
            object->setObjectSize(object_size);
            object->setTouchCallback(CC_CALLBACK_1(PatternColorQuiz::onTouchedObject, this));
            _objects.push_back(object);
            white_objects.push_back(object);
        }
    }
}

void PatternColorQuiz::onStart() {
    
    // gen color
    std::vector<std::string> colors_txt;
    for (math::resource::Image* img : img_list_color) {
        colors_txt.push_back(img->getText("color"));
    }
    
    showColorsPad(colors_txt, [=](Node* node){
        color_pick = static_cast<Math_Object*>(node)->getComparisionValue().color;
    }, CC_CALLBACK_1(PatternColorQuiz::onAnswerDone, this), 0, false);
    
    quizBegan(0, true);
    
    //Set position and setup quiz
    float delta = (quiz_size.width - number_object * object_size.width) / (number_object + 1);
    delta = delta > 60 ? 100: delta;
    float padding = (quiz_size.width - number_object * object_size.width - delta * (number_object-1)) / 2;
    
    math::func::smartSetPosition(_objects, object_size, cocos2d::Rect(padding - delta, 0, quiz_size.width - (padding-delta) * 2, quiz_size.height * 0.75 - 20 - getQuestion()->getContentSize().height));
    
    std::vector<int> random_show;
    for (int i=0; i<_objects.size(); i++) {
        random_show.push_back(i);
    }
    int number_div = ((int)_objects.size()+4)/5;
    if (number_div>1) {
        math::func::shuffle(random_show);
    }
    for (int i=0; i < _objects.size(); i++) {
        Math_Object* object = _objects[i];
        addChildNode(object, 2);
        object->onShow((random_show[i]/number_div)*0.1, math::animation::ScaleUp);
    }
}

void PatternColorQuiz::startQuestion() {
    enableControlQuiz();
    if (this->getQuestion()->getData().index==0) {
    }
}

void PatternColorQuiz::onExplanation(bool correct) {
     
    if (!correct) {
        disableControlQuiz(0.25);
        for (Math_Object* object : white_objects) {
            object->setImage(img_list.front());
            object->setComparisionValue("color", "white");
        }
    } else {
        disableControlQuiz();
        nextQuestion(0.75);
    }
}

void PatternColorQuiz::onComplete() {
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
        object->onHide((random_hide[i]/number_div)*0.1, math::animation::ScaleUp);
    }
    
    quizEnded(0.75, true);
}

#pragma mark - object touch events callback

void PatternColorQuiz::onTouchedObject(cocos2d::Node* node)
{
    auto object = static_cast<Math_Object*>(node);
    
    if (color_pick == "") {
        object->onTouchObject(false);
    } else {
        for (math::resource::Image* img : img_list_color) {
            if (img->getText("color") == color_pick) {
                object->setImage(img);
                object->setComparisionValue("color", color_pick);
                break;
            }
        }
    }
}

void PatternColorQuiz::onAnswerDone(Node* node)
{
    Math_Answer* answer = (Math_Answer*)node;
    bool done=true;
    
    for (int i = 0; i<_objects.size(); i++) {
        if (!_objects[i]->isEqual(pattern->getImageAt(i), math::comparision::color)) {
            done = false;
            break;
        }
    }
    
    answer->onAnimationClick(done);
    this->onExplanation(done);
}

#pragma mark - Auto Testing
#pragma mark -

#define auto_testing_delay 2
void PatternColorQuiz::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void PatternColorQuiz::autoTestingRun(float delaytime)
{
    
}

