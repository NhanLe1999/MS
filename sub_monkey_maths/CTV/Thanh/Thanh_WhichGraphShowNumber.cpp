//
//  Thanh_WhichGraphShowNumber.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_WhichGraphShowNumber.h"
#include "Math_Libs.h"

Thanh_WhichGraphShowNumber* Thanh_WhichGraphShowNumber::createQuiz()
{
    Thanh_WhichGraphShowNumber* quiz=new Thanh_WhichGraphShowNumber();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_WhichGraphShowNumber::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_WhichGraphShowNumber::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);
    
	Math_Quiz::initQuiz(game);
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    // TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;

    number_type_object = json_game.FindMember("number_type_object")->value.GetInt();

    //TODO: GROUP CONFIG
    rapidjson::Value& json_group=json_data.FindMember("group")->value;
    element_size = cocos2d::Size(json_group.FindMember("size")->value.FindMember("width")->value.GetInt(), json_group.FindMember("size")->value.FindMember("height")->value.GetInt());

	background = groupBG();


    //TODO:
    //lay tat ca icon trong media_set

    std::random_device rd;
    std::mt19937 g(rd());

    // lay ra cac icon group
    auto list_icon = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);

    std::vector<math::resource::Image*> icons;

    std::vector<math::resource::Image*>::iterator it;
    for(int i = 0; i < number_type_object; i++){
        it = list_icon.begin() + (int)cocos2d::random(0, (int)list_icon.size() - 1);
        math::resource::Image *img = *it;
        icons.push_back(img);
        list_icon.erase(it);
    }

    // tao ra cac gia tri
    std::vector<int> temp;
	int number_group = 3;
	if (number_type_object == 3) number_group = 2;
    for(int i = 0; i < number_group; i++){
        bool ok = false;
        while(!ok){
            temp.clear();
            for(int j = 0; j < number_type_object; j++){
                temp.push_back(cocos2d::random(1, 10));
            }
            ok = true;
            for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){
                std::vector<int>::iterator index = temp.begin();
                int cnt = 0;
                for(std::pair<math::resource::Image*, int> p: v){
                    if(*index == p.second) cnt++;
                    index++;
                }
                if(cnt == number_type_object) ok = false;
            }
            if(ok){
                std::vector<std::pair<math::resource::Image*, int> > tmp;
                for(int index = 0; index < number_type_object; index++){
                    tmp.push_back(std::make_pair(icons[index], temp[index]));
                }
                icons_group.push_back(tmp);
            }
        }
    }
    // dung cho du kien cau hoi
    auto icons_number = getNumbers(0, 10);

    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    std::vector<math::resource::Image*> img_list_number, img_list_icon;
    for(std::pair<math::resource::Image*, int> p: icons_group[0]){ 
        for(math::resource::Image *img: icons_number){
            if(img->getComparisionValue("number") == p.second){
				img_list_number.push_back(img);
				img_list_icon.push_back(p.first);
                break;
            }
        }
    }
    parseQuestion();
    // optional
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list_number) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : img_list_number) {
                auto img_txt = img->getTextValue("name_n", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, img_list_icon);
        }
    }
    /*
     if (random_sorted) {
     random_shuffle(list_questiondata.begin(), list_questiondata.end());
     }
     */

    int obj_id = 0;
    int gr_id = 0;
    auto sort_key = getComparisionKey("name_1");

    for(std::vector<std::pair<math::resource::Image*, int> > v: icons_group){

        std::vector<Math_Object*> _group_list;
        int maximum = 0;
        for(std::pair<math::resource::Image*, int> p: v){
            if(p.second > maximum) maximum = p.second;

            for(int i = 0; i < 10; i++)
            {
                Math_Object* object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, p.first, p.first);
                object->setImageVisible(false);
                if(i < p.second) object->setBackgroundVisible(true);
				else object->setBackgroundVisible(false);
                object->setObjectSize(element_size);
				object->getLabelValue()->disableEffect();
                _objects.push_back(object);
                _group_list.push_back(object);
            }
        } 
		Math_Group *group = Math_Pool::getInstance()->getGroup();
		group->setEnable(gr_id++, _group_list, math::group::GroupType::queue, _group_list.size());
		group->enableToch(CC_CALLBACK_1(Thanh_WhichGraphShowNumber::onTouchedGroup, this));
		group->setBackground(background, 30);
		group->setObjectSize(element_size);
		auto size1 = cocos2d::Size(element_size.width*1.25*10, element_size.height*number_type_object*1.25);
		group->setGroupQueue(size1);
		group->setArrangeObjects(math::group::GroupArange::kind);
		group->setBackgroundVisiable(true);
		group->updatePosition(false, false); 
        groups.push_back(group);
    }
    result = groups[0];
    std::shuffle(groups.begin(), groups.end(), g);
    // Math_Pool::getInstance()->resetImages();
}

void Thanh_WhichGraphShowNumber::onStart() {
    CCLOG("%s override", __func__); 
    touch_off = false;
    math::func::setPositionList(groups, 1, cocos2d::Rect(0, 0.05, 01, 0.78));
    for(Math_Group *group: groups) {
        addChildNode(group, 2);
        group->onShow();
    }

    //Gen question
    showQuestionBox();
}

void Thanh_WhichGraphShowNumber::startQuestion() {
    CCLOG("%s override", __func__);

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();
    }
}

void Thanh_WhichGraphShowNumber::onExplanation(bool correct) {
    CCLOG("%s override", __func__);

    if(touch_off == false) {
        touch_off = true;

        this->nextQuestion();
    }
}

void Thanh_WhichGraphShowNumber::onComplete() {
    CCLOG("%s override", __func__);

    for (Math_Answer* node: _answers) {
        if (node->isEnable()) {
            node->setDisable();
        }
    }

    float delay = showConratulation(0.3);

    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }

    m_delay= delay - 0.3;

//    for(Math_Group *group: groups){
//        group->setDisable();
//    }

    this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=](){
        for(Math_Group *group: groups){
            group->stopAllActions();
            if(group == result){
                for(int i = 0; i < group->getNumberObjects(); i++){
                    Math_Object* object = group->getObjectAtIndex(i);
                    object->runAction(cocos2d::Repeat::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.25, 1.1), cocos2d::ScaleTo::create(0.25, 1)), 4));
                }
            }
        }
    }), cocos2d::DelayTime::create(m_delay) ,cocos2d::CallFunc::create([=](){
        for(Math_Group *group: groups){
            group->setDisable();
        }
    }), NULL));

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_WhichGraphShowNumber::autoTesting() {
    
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_WhichGraphShowNumber::autoTestingRun(float dt)
{
    
}

#pragma mark - on this Thanh_WhichGraphShowNumber
void Thanh_WhichGraphShowNumber::onTouchedGroup(cocos2d::Node *node)
{
    if(touch_off == false){
        Math_Group *group = (Math_Group*)node;
        if(group != result){
            group->onTouchedAnimation(true);
        }else{
            onExplanation(true);
        }
    }
}
