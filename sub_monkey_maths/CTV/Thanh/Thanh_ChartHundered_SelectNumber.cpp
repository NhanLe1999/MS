//
//  Thanh_ChartHundered_SelectNumber.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//. Move the numbers to the corresponding position in chart hundered
//. Fill color all cell of row with number 34

#include "Thanh_ChartHundered_SelectNumber.h"
#include "Math_Libs.h"

Thanh_ChartHundered_SelectNumber* Thanh_ChartHundered_SelectNumber::createQuiz()
{
    Thanh_ChartHundered_SelectNumber* quiz=new Thanh_ChartHundered_SelectNumber();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_ChartHundered_SelectNumber::init() {
    return cocos2d::ui::Layout::init();
}

#pragma mark - Quiz Life-cycle

void Thanh_ChartHundered_SelectNumber::initQuiz(mj::model::Game game)
{
    CCLOG("%s override", __func__);

	Math_Quiz::initQuiz(game);

    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    str = json_game.FindMember("option")->value.GetString();
    if(str == "select_1"){
        game_option = OptionGame::select_1;
    }else if(str == "select_n"){
        game_option = OptionGame::select_n;
    }

    int minimum_origin = json_game.FindMember("origin")->value.FindMember("minimum")->value.GetInt();
    int maximum_origin = json_game.FindMember("origin")->value.FindMember("maximum")->value.GetInt();

    str = json_game.FindMember("operator")->value.GetString();
    if(str == "add"){
        oper = Operator::add;
    }else if(str == "sub"){
        oper = Operator::sub;
    }else if(str == "random"){
        if(cocos2d::random(0, 1)){
            oper = Operator::add;
        }else {
            oper = Operator::sub;
        }
    }

    int minimum_range = json_game.FindMember("range")->value.FindMember("minimum")->value.GetInt();
    int maximum_range = json_game.FindMember("range")->value.FindMember("maximum")->value.GetInt();

    reset_before_touch = json_game.FindMember("reset_before_touch")->value.GetBool();

    str = json_game.FindMember("touch_option")->value.GetString();
    if(str == "block"){
        touch_option = TouchOption::block;
    }else if(str == "cell"){
        touch_option = TouchOption::cell;
    }else if(str == "a_to_b"){
        touch_option = TouchOption::a_to_b;
    }else if(str == "cell_and_a_to_b"){
        touch_option = TouchOption::cell_and_a_to_b;
    }else if(str == "cell_and_block"){
        touch_option = TouchOption::cell_and_block;
    }

    rapidjson::Value& json_select_n_option = json_game.FindMember("select_n_option")->value;
    str = json_select_n_option.FindMember("more")->value.GetString();
    if(str == "only_more"){
        only_more = true;
    }else if(str == "more_than_or_equal"){
        only_more = false;
    }

    str = json_select_n_option.FindMember("less")->value.GetString();
    if(str == "only_less"){
        only_less = true;
    }else if(str == "less_than_or_equal"){
        only_less = false;
    }

    //TODO: CHART CONFIG
    rapidjson::Value& json_chart=json_data.FindMember("chart")->value;
    first_number_is_1= json_chart.FindMember("first_number")->value.GetBool();
    first_row = json_chart.FindMember("first_row")->value.GetInt();
    number_row = json_chart.FindMember("number_row")->value.GetInt();
    if(first_row + number_row > 10) number_row = 10 - first_row;
    min_number = first_row*10 + (int)first_number_is_1;
    max_number = min_number + number_row*10 - 1;
    number_column = 10;
    if(number_row > 5){
        cell_size = cocos2d::Size(55, 55);
    }else if(number_row <= 5){
        cell_size = cocos2d::Size(70, 70);
    }

    //TODO:
    //lay tat ca icon trong media_set
    background = math::resource::ResHelper::createImage(getUIPath("common/frame/square2.png"));

    auto list_number = getNumbers(0, 100);

    bool *a;
    a = new bool[max_number + 1];
    for(int i = 0; i <= max_number; i++){
        a[i] = false;
    }
    for(math::resource::Image *img: list_number){
        int number = img->getComparisionValue("number");
        if(number >= min_number && number <= max_number && a[number] == false) {
            icons_number.push_back(img);
            a[number] = true;
        }
    }
    if(icons_number.size() < max_number - min_number + 1) {
        CCLOG("KHONG DU IMAGE");
        return;
    }
    std::sort(icons_number.begin(), icons_number.end(), Thanh_ChartHundered_SelectNumber::cmpImage);

    // TODO: GET ICONS RESULT
    // check origin true or false value
    if(minimum_origin > max_number || maximum_origin < min_number){
        str = "JSON file sai gia tri origin";
        minimum_origin = min_number;
        maximum_origin = max_number;
    }
    if(minimum_origin < min_number){
        minimum_origin = min_number;
        if(maximum_origin < minimum_origin) maximum_origin = minimum_origin;
    }
    if(maximum_origin > max_number){
        maximum_origin = max_number;
        if(maximum_origin < minimum_origin) minimum_origin = maximum_origin;
    }

    // gen value
    if(oper == Operator::add){
        bool ok = false;
        int count = 0;
        while(!ok){
            count++;
            ok = true;
            origin = cocos2d::random(minimum_origin, maximum_origin);
            if(minimum_origin != max_number) {
                while(origin == max_number){
                    origin = cocos2d::random(minimum_origin, maximum_origin);
                }
            }
            if(origin + maximum_range > max_number){
                maximum_range = max_number - origin;
                if(minimum_range > maximum_range) {
                    minimum_range = maximum_range;
                }
            }
            range = cocos2d::random(minimum_range, maximum_range);
            if((game_option == OptionGame::select_n && range <= 1 && only_less && only_more) || range == 0){
                ok = false;
            }
            if(ok == true || count > 1000){
                break;
            }
        }
        if(count > 1000){
            CCLOG("KHONG THE XU LY DU LIEU");
            return;
        }
    }
    else if(oper == Operator::sub){
        bool ok = false;
        int count = 0;
        while(!ok){
            count++;
             ok =true;
            origin = cocos2d::random(minimum_origin, maximum_origin);
            if(maximum_origin != min_number) {
                while(origin == min_number){
                    origin = cocos2d::random(minimum_origin, maximum_origin);
                }
            }
            if(origin - maximum_range < min_number){
                maximum_range = origin - min_number;
                if(minimum_range > maximum_range) {
                    minimum_range = maximum_range;
                }
            }
            range = cocos2d::random(minimum_range, maximum_range);
            if(origin < min_number || origin > max_number || origin - range < min_number){
                ok = false;
            }
            if((game_option == OptionGame::select_n && range <= 1 && only_less && only_more) || range == 0){
                ok = false;
            }
            if(ok == true || count > 1000){
                break;
            }
        }
        if(count > 1000){
            CCLOG("KHONG THE XU LY DU LIEU");
            return;
        }
    }
    if(game_option == OptionGame::select_n && range < 2){
        if(oper == Operator::add) {
            if(range + origin > max_number) {
                origin--;
            }else {
                range++;
            }
        }else if(oper == Operator::sub) {
            if(range - origin < min_number) {
                origin++;
            }else {
                range++;
            }
        }
    }

    // get image source
    int derivative = origin + range;
    if(oper == Operator::sub) {
        derivative = origin - range;
    }
    std::vector<math::resource::Image*> list_img;
    math::resource::Image *image_origin = nullptr, *image_derivative = nullptr, *image_range = nullptr;
    for(math::resource::Image *img: list_number){
        if(img->getComparisionValue("number") == derivative ) {
            image_derivative = img;
        }
        if(img->getComparisionValue("number") == origin){
            image_origin = img;
        }
        if(img->getComparisionValue("number") == range){
            image_range = img;
        }
    }

    if(game_option == OptionGame::select_1){
        list_img.push_back(image_range);
        list_img.push_back(image_origin);
    }else if(game_option == OptionGame::select_n){
        if(oper == Operator::sub){
            std::swap(image_origin, image_derivative);
        }
        list_img.push_back(image_origin);
        list_img.push_back(image_derivative);
    }

    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    // optional
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        if (key == "number") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : list_img) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                img_txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else if (key == "number_text") {
            std::vector<math::resource::Text> texts;
            for (math::resource::Image* img : list_img) {
                auto img_txt = img->getTextValue("name_1", m_lang->getLanguage());
                texts.push_back(img_txt);
            }
            getQuestion()->setOptionalTexts(key, texts);
        } else {
            getQuestion()->setOptionalTexts(key, list_img);
        }
    }

    // keyword
    if(OptionGame::select_1 == game_option){
        if(Operator::sub == oper){
            this->getQuestion()->setKeywordValue(0);
        }
        else if(Operator::add == oper){
            this->getQuestion()->setKeywordValue(1);
        }
    }
    else if(OptionGame::select_n == game_option){
        if(only_more){
            this->getQuestion()->setKeywordValue(0);
        }
        else {
            this->getQuestion()->setKeywordValue(1);
        }

        if(only_less){
            this->getQuestion()->setKeywordValue(2);
        }
        else {
            this->getQuestion()->setKeywordValue(3);
        }
    }
    /*
     if (random_sorted) {
     random_shuffle(list_questiondata.begin(), list_questiondata.end());
     }
     */

    int obj_id = 0;
    int gr_id = 0;
    auto sort_key = getComparisionKey("number");

    // gen chart
    chart = Math_Pool::getInstance()->getGroup();
    chart->setObjectSize(cell_size);
    chart->setComparisionKey(sort_key);

    std::vector<Math_Object*> _group_list;
    for(math::resource::Image *img: icons_number)
    {
        Math_Object* object = Math_Pool::getInstance()->getObject();
        object->setEnable(obj_id++, math::common::deny, img, background);
        object->setSelectedAnimation(math::common::SelectedAnimation::scale);
        object->setBackgroundVisible(true);
        object->setImageVisible(false);
        object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
        object->showValueTextWithAnimation(false);
        object->fitValueTextSize();
        object->setObjectSize(cell_size);
        object->getLabelValue()->setFontSize(cell_size.height*0.6);
        object->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
		object->getLabelValue()->disableEffect();
		object->getBackground()->setContentSize( cell_size*1.21 );
        _objects.push_back(object);
        _group_list.push_back(object);
        object->onShow(0); 
    }

	chart->setEnable( gr_id++, _group_list, math::group::GroupType::frame, 1 );
	chart->setGroupFrames( number_column, number_row,
						   math::resource::ResHelper::createImage( getUIPath( "" ) ),
						   math::resource::ResHelper::createImage( getUIPath( "common/frame/square.png" ) ) ); 
    _groups.push_back(chart);

    //Math_Pool::getInstance()->resetImages();
}

void Thanh_ChartHundered_SelectNumber::onStart() {
    CCLOG("%s override", __func__);

    // show chart
    if(number_row > 8){
        chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2 - 100, _panel->getContentSize().height/2));
    }else if(number_row > 5){
        chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, _panel->getContentSize().height/2));
    }else {
        chart->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, _panel->getContentSize().height/2 - 25));
    }
    addChildNode(chart,2);
    chart->onShow();
	for( int i = 0; i < chart->getNumberObjects(); i++ ) {
		Math_Object *object = chart->getObjectAtIndex( i );
		object->runAction(cocos2d::Sequence::createWithTwoActions( cocos2d::DelayTime::create( 0.5 ), cocos2d::FadeIn::create( 0.4 ) ) );
	}
	chart->getBackground()->setOpacity( 0 );
	chart->getBackground()->runAction( cocos2d::Sequence::createWithTwoActions( cocos2d::DelayTime::create( 0.5 ), cocos2d::FadeIn::create( 0.4 ) ) );

    // add event
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(Thanh_ChartHundered_SelectNumber::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(Thanh_ChartHundered_SelectNumber::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(Thanh_ChartHundered_SelectNumber::onTouchMoved, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

    // gen button done
    btn_done=Math_Pool::getInstance()->getAnswer(); 
	btn_done->setEnable( "Done", math::common::custom_bg, CC_CALLBACK_1( Thanh_ChartHundered_SelectNumber::onAnswerDone, this ), cocos2d::Size( 200, 70 ) );
	if( number_row > 8 ) { 
		btn_done->setCustomAnswer( math::resource::ResHelper::createImage( getUIPath( "common/done.png" ) ), cocos2d::Size( 200, 70 ), 70 * 0.6, cocos2d::Color4B::WHITE, 0.2 );
		btn_done->setPosition( cocos2d::Vec2( chart->getPositionX() + chart->getContentSize().width / 2 + 100 + btn_done->getContentSize().width / 2,
											  chart->getPositionY() - chart->getContentSize().height / 2 + 35 ) );
	}
	else { 
		btn_done->setCustomAnswer( math::resource::ResHelper::createImage( getUIPath( "common/done.png" ) ), cocos2d::Size( 200, 70 ), 70 * 0.6, cocos2d::Color4B::WHITE, 0.2 );
		btn_done->setPosition( cocos2d::Vec2( _panel->getContentSize().width/2, 60) );
	}
    btn_done->setVisible(true);
    _answers.push_back(btn_done);

    //Gen question
	if( number_row > 5 ) { 
		showQuestionBox();
	}
	else { 
		showQuestionBox();
	}
}

void Thanh_ChartHundered_SelectNumber::startQuestion() {
    CCLOG("%s override", __func__);

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();

        touch_off = false; 
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=](){
            //#2. show button done 
            btn_done->onShowInParent(_panel, 3);
        }), nullptr));
    };
}

void Thanh_ChartHundered_SelectNumber::onExplanation(bool correct) {
    CCLOG("%s override", __func__);

    touch_off = true;

    if(correct){
        btn_done->disableClickAnswer();
        this->nextQuestion();
    }
    else{
        btn_done->onAnimationClick(false);
        btn_done->disableClickAnswer();
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.7), cocos2d::CallFunc::create([this](){
            btn_done->enableClickAnswer();
            touch_off = false;
            for(Math_Object *object: choose_list)
            {
				object->getBackground()->loadTexture( background->path );
            }
            choose_list.clear();
        }), nullptr));
    }
}

void Thanh_ChartHundered_SelectNumber::onComplete() {
    CCLOG("%s override", __func__);

    //Math_Pool::getInstance()->resetAnswers();

    float delay = showConratulation(0.3);

    float m_delay=delay-0.5;
    if (m_delay < 0) {
        m_delay = 0;
    }
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=](){
        //#1. hide chart hunder
        for(int i = 0; i < chart->getNumberObjects(); i++){
            Math_Object *object = chart->getObjectAtIndex(i);
			object->getBackground()->loadTexture( background->path );
            object->getBackground()->setContentSize(cell_size); 
			object->runAction( cocos2d::FadeOut::create( 0.4 ) );
        }
		chart->getBackground()->runAction( cocos2d::FadeOut::create( 0.4 ) );

        chart->setDisable();

    }), nullptr));

    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void Thanh_ChartHundered_SelectNumber::onAnswerDone(bool correct)
{
    touch_off = true;
    bool done=true;
    switch (game_option) {
        case OptionGame::select_1:
            if(oper == Operator::sub){
                for(Math_Object *object: choose_list)
                {
                    if(object->getComparisionValue().number != origin - range)
                    {
                        done = false;
                    }
                }
            }else if(oper == Operator::add){
                for(Math_Object *object: choose_list)
                {
                    if(object->getComparisionValue().number != origin + range)
                    {
                        done = false;
                    }
                }
            }
            break;

        case OptionGame::select_n:
        {
            int a = 0, b = 0;
            // select a to b is correct
            if(oper == Operator::add){
                a = origin;
                b = origin + range;
            }else if(oper == Operator::sub){
                a = origin - range;
                b = origin;
            }
            if(only_more) a++;
            if(only_less) b--;


            // kiem tra co chon du so luong
            if(choose_list.size() != b - a + 1)
            {
                done = false;
            }
            // kiểm tra các số có thoả mãn yêu cầu
            for(Math_Object *object: choose_list)
            {
                if(object->getComparisionValue().number > b || object->getComparisionValue().number < a)
                {
                    done = false;
                }
            }
        }
            break;

        default:
            break;
    }

    this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_ChartHundered_SelectNumber::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_ChartHundered_SelectNumber::autoTestingRun(float dt)
{

}

#pragma mark - touch function
bool Thanh_ChartHundered_SelectNumber::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event){
    if(touch_off) return false;
    if(reset_before_touch) {
        for(std::set<Math_Object*>::iterator it = choose_list.begin(); it != choose_list.end(); it++){
			( *it )->getBackground()->loadTexture( background->path );
        }
        choose_list.clear();
    }
    first_position_touch = _panel->convertToNodeSpace(touch->getLocation());
    if(game_option == OptionGame::select_1){
        only_touch = true;
        for(int i = 0; i < chart->getNumberObjects(); i++){
            Math_Object *object = chart->getObjectAtIndex(i);
            cocos2d::Vec2 locationInNode = object->convertToNodeSpace(touch->getLocation());
            cocos2d::Size s = object->getBoundingBox().size;
            cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);

            if (rect.containsPoint(locationInNode))
            {
                object_be_touch = object;
                break;
            }
        }
    }else {
        only_touch = true;
        for(int i = 0; i < chart->getNumberObjects(); i++){
            Math_Object *object = chart->getObjectAtIndex(i);
            cocos2d::Vec2 locationInNode = object->convertToNodeSpace(touch->getLocation());
            cocos2d::Size s = object->getBoundingBox().size;
            cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);

            if (rect.containsPoint(locationInNode))
            {
                if(touch_option == TouchOption::cell){
                    return true;
                }
                object_be_touch = object;
                if(choose_list.find(object) != choose_list.end()){
					object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
                    choose_list.insert(object);
                }
                break;
            }
        }
    }
    return true;
}

void Thanh_ChartHundered_SelectNumber::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
    CCLOG("%s override", __func__);
    second_position_touch = _panel->convertToNodeSpace(touch->getLocation());

    switch (touch_option) {
        case TouchOption::cell:
        {
            if(object_be_touch == nullptr) return;
            cocos2d::Vec2 locationInNode = object_be_touch->convertToNodeSpace(second_position_touch);
            cocos2d::Size s = object_be_touch->getBoundingBox().size;
            cocos2d::Rect rect = cocos2d::Rect(0, 0, s.width, s.height);

            if (rect.containsPoint(locationInNode) == false)
            {
                only_touch = false;
            }
        }
            break;
        case TouchOption::block:
        case TouchOption::cell_and_block:
        {
            for(Math_Object *object: tmp_choose_list){
				object->getBackground()->loadTexture( background->path );
            }
            for(Math_Object *object: choose_list){
				object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
            }
            tmp_choose_list = getObjectInArea(first_position_touch, second_position_touch, TouchOption::block);
            if(tmp_choose_list.size() > 1 || touch_option == TouchOption::block) {
                only_touch = false;
            }
            for(Math_Object *object: tmp_choose_list){
				object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
            }
        }
            break;
        case TouchOption::a_to_b:
        case TouchOption::cell_and_a_to_b:
        {
            for(Math_Object *object: tmp_choose_list){
                object->getBackground()->loadTexture( background->path );
            }
            for(Math_Object *object: choose_list){
                object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
            }
            tmp_choose_list = getObjectInArea(first_position_touch, second_position_touch, TouchOption::a_to_b);
            if(tmp_choose_list.size() > 1 || touch_option == TouchOption::a_to_b) {
                only_touch = false;
            }
            for(Math_Object *object: tmp_choose_list){
                object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
            }
        }
            break;

        default:
            break;
    }
}

void Thanh_ChartHundered_SelectNumber::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    CCLOG("%s override", __func__);

    for(Math_Object *object: tmp_choose_list){
        object->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
        choose_list.insert(object);
    }

    if(only_touch){
        if(object_be_touch == nullptr) return;
        if(choose_list.find(object_be_touch) == choose_list.end()){
            choose_list.insert(object_be_touch);
            object_be_touch->getBackground()->loadTexture( getUIPath( "common/frame/brown1.png" ) );
        }else {
            choose_list.erase(object_be_touch);
            object_be_touch->getBackground()->loadTexture( background->path );
        }
    }
    tmp_choose_list.clear();
    object_be_touch = nullptr;
}

void Thanh_ChartHundered_SelectNumber::onTouchCancel(cocos2d::Touch *touch, cocos2d::Event *event)
{
    for(Math_Object *object: choose_list) object->getBackground()->loadTexture( background->path );
    choose_list.clear();
    object_be_touch = nullptr;
}


std::set<Math_Object*> Thanh_ChartHundered_SelectNumber::getObjectInArea(cocos2d::Point first_point, cocos2d::Point second_point, TouchOption touch_opt){
    std::set<Math_Object*> objs;
    switch (touch_opt) {
        case TouchOption::a_to_b:
        {
            cocos2d::Point top = second_point;
            cocos2d::Point bottom = first_point;
            if(top.y < bottom.y){
                std::swap(top, bottom);
            }
            bool ok;
            cocos2d::Size size = cell_size*1.2;
            for(int i = 0; i < chart->getNumberObjects(); i++){
                ok = true;
                Math_Object *object = chart->getObjectAtIndex(i);
                cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
                // nếu cao hơn top
                if(object_position.y - size.height/2 > top.y) {
                    ok = false;
                }
                // nếu bằng điểm top và nằm bên trái
                if(object_position.y + size.height/2 > top.y && object_position.y - size.height/2 < top.y && object_position.x + size.width/2 < top.x){
                    ok = false;
                }
                // nếu thấp hơn bottom
                if(object_position.y + size.height/2 < bottom.y) {
                    ok = false;
                }
                // nếu bằng bottom và bên phải
                if( object_position.y + size.height/2 > bottom.y && object_position.y - size.height/2 < bottom.y && object_position.x - size.width/2 > bottom.x ) {
                    ok = false;
                }

                // set color
                if(ok) {
                    objs.insert(object);
                }else {
                    objs.erase(object);
                }
            }
        }
            break;
        case TouchOption::block:
        {
            cocos2d::Size size = cell_size*1.2;
            cocos2d::Point bottom_right_touch = second_point;
            cocos2d::Point top_left_touch = first_point;
            if(top_left_touch.x > bottom_right_touch.x){
                float x = top_left_touch.x;
                top_left_touch.x = bottom_right_touch.x;
                bottom_right_touch.x = x;
            }
            if(top_left_touch.y < bottom_right_touch.y){
                float y = top_left_touch.y;
                top_left_touch.y = bottom_right_touch.y;
                bottom_right_touch.y = y;
            }

            for(int i = 0; i < chart->getNumberObjects(); i++){
                Math_Object *object = chart->getObjectAtIndex(i);
                cocos2d::Point object_position = _panel->convertToNodeSpace(chart->convertToWorldSpace(object->getPosition()));
                if(object_position.y + size.height/2 > bottom_right_touch.y
                   && object_position.y - size.height/2 < top_left_touch.y
                   && object_position.x - size.width/2 < bottom_right_touch.x
                   && object_position.x + size.width/2 > top_left_touch.x)
                {
                    objs.insert(object);
                }
                else {
                    objs.erase(object);
                }
            }
            break;
        }
        default:
            break;
    }
    return objs;
}

#pragma mark - bool of sort
bool Thanh_ChartHundered_SelectNumber::cmpImage(math::resource::Image *a, math::resource::Image *b)
{
    return a->getComparisionValue("number") < b->getComparisionValue("number");
}

bool Thanh_ChartHundered_SelectNumber::cmpObject(Math_Object *a, Math_Object *b)
{
    return a->getComparisionValue().number < b->getComparisionValue().number;
}

