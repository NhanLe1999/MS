//
//  Thanh_MeasureDragObject.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//  Measure the height of the object and drag it to the correct answer
//

#include "Thanh_MeasureDragObject.h"
#include "Math_Libs.h"

Thanh_MeasureDragObject* Thanh_MeasureDragObject::createQuiz()
{
    Thanh_MeasureDragObject* quiz=new Thanh_MeasureDragObject();
    if (quiz->_initQuiz()) {
        quiz->autorelease();
        return quiz;
    }

    CC_SAFE_DELETE(quiz);
    return NULL;
}

bool Thanh_MeasureDragObject::init() {
    return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_MeasureDragObject::initQuiz(mj::model::Game game)
{
	CCLOG( "%s override", __func__ );
    
	Math_Quiz::initQuiz( game );
    
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

    rapidjson::Value& json_data=document.FindMember("data")->value;

    //TODO: GAME CONFIG
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    str = json_game.FindMember("option_game")->value.GetString(); 
    if(str == "measure_leng") {
        option_game = OptionGame::measure_leng;
        _path = getUIPath( "gameui/measurement/measure_box_leng.png" );
    }
    else if(str == "measure_height") {
        option_game = OptionGame::measure_height;
        _path = getUIPath("gameui/measurement/measure_box_height.png");
    }



    //TODO: OBJECT CONFIG
    rapidjson::Value& json_number=json_data.FindMember("number")->value;
    number_size = cocos2d::Size(json_number.FindMember("size")->value.FindMember("width")->value.GetInt(), json_number.FindMember("size")->value.FindMember("height")->value.GetInt());

    str = json_number.FindMember("display")->value.GetString();
    if(str == "number") number_option = ObjectOption::number;
    else if(str == "text") number_option = ObjectOption::text;

	number_bg = math::resource::ResHelper::createImage( getUIPath( "common/frame/square2.png" ) );

    //TODO: GET ICONS
    // get icon object drag
    list_object = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
    for(math::resource::Image* img: list_object)
    {
        while(icons_object_drag.size() < 3){
			bool ok = true;
			while( ok ) {
				ok = true;
				math::resource::Image *img = math::func::getRandomNode( list_object );
				for( int j = 0; j < icons_object_drag.size(); j++ ) {
					if( icons_object_drag[j] == img ) ok = false;
				}
				if( ok ) {
					icons_object_drag.push_back( img );
					icons_result.push_back( getNumbers(img->getComparisionValue("number"), img->getComparisionValue("number")).at(0) ); 
				}
			}
        }
    }

    std::vector<math::resource::Image*> list_img; 

    if(option_game == OptionGame::measure_height){
		icon_ruler = math::resource::ResHelper::createImage( getUIPath( "gameui/measurement/ruler_height.png" ));
    }
    else {
		icon_ruler = math::resource::ResHelper::createImage( getUIPath( "gameui/measurement/ruler_leng.png" ) );
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
    /*
     if (random_sorted) {
     random_shuffle(list_questiondata.begin(), list_questiondata.end());
     }
     */

    obj_id = 0;
    switch (option_game) {
            // TODO: OPTION MEASURE LENG
        case OptionGame::measure_leng:
        {
            // TODO: gen ruler
            ruler = Math_Pool::getInstance()->getObject();
            ruler->setEnable(obj_id++, math::common::deny, icon_ruler, number_bg);
            ruler->setObjectSize((cocos2d::ui::ImageView::create(icon_ruler->path))->getContentSize());
            if(ruler->getContentSize().width > 800.0f) {
                scale = 800.0/ruler->getContentSize().width;
            }else scale = 1.0f;
            ruler->setObjectSize(cocos2d::Size(ruler->getContentSize().width*scale, ruler->getContentSize().height*scale));

            ruler->setPosition(cocos2d::Vec2(ruler->getImage()->getContentSize().width/2 + 140, 125));
            ruler->setBackgroundVisible(false);
            ruler->setImageVisible(true);
            _objects.push_back(ruler);

			scale *= 2;
            // TODO: gen drager
            for(math::resource::Image* img: icons_object_drag){
                Math_Object *object_drag = Math_Pool::getInstance()->getObject();
                object_drag->setEnable(obj_id++, math::common::deny, img, number_bg);
                object_drag->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragObject::onTouchedObject, this),
                                             CC_CALLBACK_2(Thanh_MeasureDragObject::onMoveObjectEnded, this));
                object_drag->setObjectSize((cocos2d::ui::ImageView::create(img->path))->getContentSize() * scale);
                object_drag->setBackgroundVisible(false);
                object_drag->setImageVisible(true);
                _objects.push_back(object_drag);
                objects_drag.push_back(object_drag);
            }
            math::func::setPositionList(objects_drag, 1, cocos2d::Rect(0.2, 0.21, 0.6, 0.25));
            for(Math_Object* object_drag: objects_drag){
                object_drag->setPositionX(object_drag->getImage()->getContentSize().width/2 + 190);
            }

            // TODO: gen slot & result
            std::vector<Math_Slot*> slots;
            std::vector<Math_Object*> results;
            // slots
            for(int i = 0; i < 3; i++)
            {
                Math_Slot *slot = Math_Pool::getInstance()->getSlot();
                slot->setEndble(obj_id++, slot_bg);
                slot->setBackgroundVisible(false);
                slot->setSlotSize(cocos2d::Size(ruler->getBackground()->getContentSize().width*0.7, number_size.height*0.9));
                _slots.push_back(slot);
                slots.push_back(slot);
            }
            math::func::setPositionList(slots, 1, cocos2d::Rect(0.315, 0.475, 0.6, 0.32));
            for(int i = 0; i < 3; i++){
                auto image_view = cocos2d::ui::ImageView::create(_path);
                image_view->setScale9Enabled(true);
                image_view->setCapInsets(cocos2d::Rect(1, 1, image_view->getVirtualRendererSize().width-2, image_view->getVirtualRendererSize().height-2));
                image_view->setContentSize(cocos2d::Size(ruler->getImage()->getContentSize().width*1.2, number_size.height)); 
				list_imageview.push_back( image_view );
                image_view->setVisible(true);
                image_view->setPosition(cocos2d::Vec2(slots[i]->getPosition().x - 100, slots[i]->getPosition().y));
            }
            // results
            for(math::resource::Image* img: icons_result)
            {
                Math_Object *object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, img, number_bg);
                object->setObjectSize(number_size);
                object->setBackgroundVisible(false);
                object->setImageVisible(false);
                if(number_option == ObjectOption::number)
                {
                    object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
                    object->getLabelValue()->setFontSize(number_size.height*0.6);
                }
                else object->setValueText(img->getText("name_1", m_lang->getLanguage()));
                object->getLabelValue()->setTextColor(cocos2d::Color4B(44, 51, 144, 232));
                object->showValueTextWithAnimation(false);
                object->fitValueTextSize();
                results.push_back(object);
                _objects.push_back(object);
            }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(results.begin(), results.end(), g);
            math::func::setPositionList(results, 1, cocos2d::Rect(0.12, 0.475, 0.1, 0.32));

            for(int i = 0 ; i < 3; i++) {
                pair_slots.push_back(std::make_pair(slots[i], results[i]));
            }
        }
            break;

            // TODO: OPTION MEASURE HEIGHT
        case OptionGame::measure_height:
        {
            // TODO: gen ruler
            ruler = Math_Pool::getInstance()->getObject();
            ruler->setEnable(obj_id++, math::common::deny, icon_ruler, number_bg);
            ruler->setObjectSize((cocos2d::ui::ImageView::create(icon_ruler->path))->getContentSize());
            if(ruler->getContentSize().height > 600.0f) {
                scale = 600.0f/ruler->getContentSize().height;
            }else scale = 1.0f;
            ruler->setObjectSize(ruler->getContentSize()*scale);
            ruler->setPosition(cocos2d::Vec2(70, ruler->getImage()->getContentSize().height/2+ 140));
            ruler->setBackgroundVisible(false);
            ruler->setImageVisible(true);
            _objects.push_back(ruler);

			scale *= 2;
            // TODO: gen drager
            for(math::resource::Image* img: icons_object_drag){
                Math_Object *object_drag = Math_Pool::getInstance()->getObject();
                object_drag->setEnable(obj_id++, math::common::deny, img, number_bg);
                object_drag->setMoveCallback(CC_CALLBACK_1(Thanh_MeasureDragObject::onTouchedObject, this),
                                             CC_CALLBACK_2(Thanh_MeasureDragObject::onMoveObjectEnded, this));
				cocos2d::ui::ImageView* image_view = cocos2d::ui::ImageView::create( img->path );
				CCLOG( "%f %f %f %f ", ( cocos2d::ui::ImageView::create( img->path ) )->getContentSize().width, ( cocos2d::ui::ImageView::create( img->path ) )->getContentSize().height
					   , object_drag->getImage()->getContentSize().width, object_drag->getImage()->getContentSize().height );
                object_drag->setObjectSize(image_view->getContentSize() * scale);
                object_drag->setBackgroundVisible(false);
                object_drag->setImageVisible(true);
                _objects.push_back(object_drag);
                objects_drag.push_back(object_drag);
            }
            math::func::setPositionList(objects_drag, 3, cocos2d::Rect(0.14, 0.152, 0.26, 0.6));
            for(Math_Object *object_drag: objects_drag){
                object_drag->setPositionY(object_drag->getImage()->getContentSize().height/2+ 171);
            }

            // TODO: gen slot & result
            std::vector<Math_Object*> results;
            std::vector<Math_Slot*> slots;
            // slot
            for(int i = 0; i < 3; i++)
            {
                Math_Slot *slot = Math_Pool::getInstance()->getSlot();
                slot->setEndble(obj_id++, math::resource::ResHelper::createImage(getUIPath("common/drag.png")));
                slot->setBackgroundVisible(false);
                slot->setSlotSize(cocos2d::Size(80, ruler->getImage()->getContentSize().height*0.9));
                _slots.push_back(slot);
                slots.push_back(slot);
            }
            math::func::setPositionList(slots, (int)slots.size(), cocos2d::Rect(0.45, 0.225, 0.55, 0.6));

			for( int i = 0; i < 3; i++ ) {
				auto image_view = cocos2d::ui::ImageView::create( _path );
				image_view->setScale9Enabled( true );
				image_view->setCapInsets( cocos2d::Rect( 1, 1, image_view->getVirtualRendererSize().width - 2, image_view->getVirtualRendererSize().height - 2 ) );
				float scale = ruler->getImage()->getContentSize().height*1.06 / image_view->getContentSize().height;
				image_view->setContentSize( cocos2d::Size( 150, ruler->getImage()->getContentSize().height*1.06 ) );
				list_imageview.push_back( image_view );
				addChildNode( image_view, 2 );
				image_view->setPosition( slots[i]->getPosition() );
				image_view->setPositionY( image_view->getContentSize().height / 2 + 115 );
				image_view->setVisible( true );
				image_view->setOpacity( 255 );
			}
           
            // result
            for(math::resource::Image* img: icons_result)
            {
                Math_Object *object = Math_Pool::getInstance()->getObject();
                object->setEnable(obj_id++, math::common::deny, img, number_bg);
                object->setObjectSize(number_size);
                object->setBackgroundVisible(false);
                object->setImageVisible(false);
                if(number_option == ObjectOption::number)
                {
                    object->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
                    object->getLabelValue()->setFontSize(number_size.height*0.6);
                }
                else object->setValueText(img->getText("name_1", m_lang->getLanguage()));
                object->getLabelValue()->setTextColor(cocos2d::Color4B(44, 51, 144, 232));
                object->showValueTextWithAnimation(false);
                object->fitValueTextSize();
                results.push_back(object);
                _objects.push_back(object);
            }

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(results.begin(), results.end(), g);
            math::func::setPositionList(results, (int)results.size(), cocos2d::Rect(0.45, 0.11, 0.55, 0.15));

            for(int i = 0 ; i < 3; i++) {
                pair_slots.push_back(std::make_pair(slots[i], results[i]));
            }
        }
            break;
            
        default:
            break;
    }

    //Math_Pool::getInstance()->resetImages();
}

void Thanh_MeasureDragObject::onStart() {
	CCLOG( "%s override", __func__ );
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=](){
        //#0. measure box
        for(Math_Object* object: measure_box_list){
            this->addChildNode(object, 2);
            object->onShow(0);
        }
    }), cocos2d::DelayTime::create(0.5), cocos2d::CallFunc::create([=](){
        //#1. show ruler
        this->addChildNode(ruler, 2);
        ruler->onShow(0);
    }), cocos2d::DelayTime::create(0.2), cocos2d::CallFunc::create([=](){
        //#2. show drager
        for(Math_Object* object: objects_drag){
            this->addChildNode(object, 2);
            object->onShow(0);
        }
    }), cocos2d::DelayTime::create(0.2), cocos2d::CallFunc::create([=](){
        //#3. show slot & result
        for(std::pair<Math_Slot*, Math_Object*> p: pair_slots){
            this->addChildNode(p.first, 2);
            p.first->onShow(0);
            this->addChildNode(p.second, 2);
            p.second->onShow(0);
        }
    }),  nullptr)); 

    //--------------------
    //Gen button done
    btn_done=Math_Pool::getInstance()->getAnswer();
    btn_done->setEnable("Done", math::common::answer, CC_CALLBACK_1(Thanh_MeasureDragObject::onAnswerDone, this), cocos2d::Size(250, 60));
    btn_done->setPosition(cocos2d::Vec2(_panel->getContentSize().width/2, 50));
    btn_done->setVisible(true);
    _answers.push_back(btn_done);

    //Gen question
    showQuestionBox();
}

void Thanh_MeasureDragObject::startQuestion() {
	CCLOG( "%s override", __func__ );

    if (this->getQuestion()->getData().index==0) {
        enableControlQuiz();

        for(Math_Object* object_drag: objects_drag){
            object_drag->enableDrag(_panel);
        }

        for (Math_Answer* done :  _answers) {
            done->onShowInParent(_panel, 2);
        }
    }
}

void Thanh_MeasureDragObject::onExplanation(bool correct) {
	CCLOG( "%s override", __func__ );

    if(correct) {
        for(Math_Object *object: objects_drag){
            object->setTouch(math::common::TouchPermission::deny);
        }
        btn_done->disableClickAnswer();
        this->nextQuestion();
    }else {
        btn_done->onAnimationClick(false);
        for(std::pair<Math_Slot*, Math_Object*> p: pair_slots){
            if(p.first->isUsed() == true && p.second->getComparisionValue().number != p.first->getLinker()->getComparisionValue().number){
                p.first->getLinker()->backToStart();
                p.first->removeLinker();
            }
        }
    }
}

void Thanh_MeasureDragObject::onComplete() {
	CCLOG( "%s override", __func__ );

    for (Math_Answer* node: _answers) {
        if (node->isEnable()) {
            node->setDisable();;
        }
    }

    float delay = showConratulation(0.3);

    float m_delay=delay-0.75;
    if (m_delay < 0) {
        m_delay = 0;
    }

    m_delay= delay - 0.3;
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay), cocos2d::CallFunc::create([=](){
        //#1. hide ruler
        ruler->onHide(0);
    }), cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=](){
        //#2. hide all
        for(Math_Object* object: measure_box_list){
            object->onHide(0);
        }
        for(std::pair<Math_Slot*, Math_Object*> p: pair_slots){
            p.first->onHide(0);
            p.second->onHide(0);
        }
        for(Math_Object*object:objects_drag){
            object->onHide(0);
        }
    }), nullptr));


    scheduleOnce([=](float dt) {
        hideQuestionBox();
    }, delay, "complete_quiz");
}

void Thanh_MeasureDragObject::onAnswerDone(bool correct)
{
	CCLOG( "%s override", __func__ );
    bool done=true;

    for(std::pair<Math_Slot*, Math_Object*> p: pair_slots){
        if(p.first->isUsed() == false || p.second->getComparisionValue().number != p.first->getLinker()->getComparisionValue().number) done = false;
    }
    this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_MeasureDragObject::autoTesting() {

    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d", testing_step++));
}

void Thanh_MeasureDragObject::autoTestingRun(float dt)
{

}

#pragma mark - on this game
void Thanh_MeasureDragObject::onTouchedObject(cocos2d::Node* node)
{
	CCLOG( "%s override", __func__ );
    Math_Object* object = (Math_Object*)node;
    object->setLocalZOrder(4);
}

void Thanh_MeasureDragObject::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point) {
	CCLOG( "%s override", __func__ );
    Math_Object* object=(Math_Object*)node;
    this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([=](){
        for(Math_Object *obj: objects_drag){
            obj->setTouch(math::common::TouchPermission::deny);
        }
    }), cocos2d::DelayTime::create(0.3), cocos2d::CallFunc::create([=](){
        for(Math_Object *obj: objects_drag){
            obj->setTouch(math::common::TouchPermission::move);
        }
    }), nullptr));

    object->removeFromParent();
    cocos2d::Vec2 position=_panel->convertToNodeSpace(point);
    object->setPosition(position);
    _panel->addChild(object);

    //Doi tuong keo tha nam trong 1 slot nao do.
    Math_Slot* m_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (object->isSame(slot->getLinker())) {
            m_slot = slot;
            break;
        }
    }

    Math_Slot* t_slot = nullptr;
    for (Math_Slot* slot : _slots) {
        if (slot->isAvailable(position)) {
            t_slot = slot;
            break;
        }
    }

    if (m_slot!=nullptr) {
        if (t_slot != nullptr) {
            //swap
            if (m_slot == t_slot) {
                object->backToLast(2);
            } else {
                if (t_slot->isUsed()) {
                    auto linker = t_slot->getLinker();
                    m_slot->removeLinker();
                    t_slot->removeLinker();

                    if(option_game == OptionGame::measure_leng){
                        t_slot->setLinker(object);
                        auto t_position = cocos2d::Vec2(t_slot->getPosition().x + object->getImage()->getContentSize().width/2 - t_slot->getContentSize().width/2 + 10,
                                                        t_slot->getPosition().y);
                        object->runAction(cocos2d::MoveTo::create(0.2, t_position));

                        m_slot->setLinker(linker);
                        auto m_position = cocos2d::Vec2(m_slot->getPosition().x + linker->getImage()->getContentSize().width/2 - m_slot->getContentSize().width/2 + 10,
                                                        m_slot->getPosition().y);
                        linker->runAction(cocos2d::MoveTo::create(0.3, m_position));
                    }
                    else if(option_game == OptionGame::measure_height){
                        t_slot->setLinker(object);
                        auto t_position = cocos2d::Vec2(t_slot->getPosition().x,
                                                        t_slot->getPosition().y + object->getImage()->getContentSize().height/2 - t_slot->getContentSize().height/2);
                        object->runAction(cocos2d::MoveTo::create(0.2, t_position));

                        m_slot->setLinker(linker);
                        auto m_position = cocos2d::Vec2(m_slot->getPosition().x,
                                                        m_slot->getPosition().y + linker->getImage()->getContentSize().height/2 - m_slot->getContentSize().height/2);
                        linker->runAction(cocos2d::MoveTo::create(0.3, m_position));
                    }
                } else {
                    m_slot->removeLinker();
                    t_slot->setLinker(object);

                    if(option_game == OptionGame::measure_leng){
                        auto t_position = cocos2d::Vec2(t_slot->getPosition().x + object->getImage()->getContentSize().width/2 - t_slot->getContentSize().width/2 + 10,
                                                        t_slot->getPosition().y);
                        object->runAction(cocos2d::MoveTo::create(0.2, t_position));
                    }
                    else if(option_game == OptionGame::measure_height){
                        auto t_position = cocos2d::Vec2(t_slot->getPosition().x,
                                                        t_slot->getPosition().y + object->getImage()->getContentSize().height/2 - t_slot->getContentSize().height/2);
                        object->runAction(cocos2d::MoveTo::create(0.2, t_position));
                    }
                }
            }
        } else {
            //move to start
            m_slot->removeLinker();
            object->backToStart();
        }
    } else {
        if (t_slot != nullptr) {
            if (t_slot->isUsed()) {
                auto linker = t_slot->getLinker();
                t_slot->removeLinker();
                linker->backToStart();
            }

            t_slot->setLinker(object);
            if(option_game == OptionGame::measure_leng){
                auto t_position = cocos2d::Vec2(t_slot->getPosition().x + object->getImage()->getContentSize().width/2 - t_slot->getContentSize().width/2 + 10,
                                                t_slot->getPosition().y);
                object->runAction(cocos2d::MoveTo::create(0.2, t_position));
            }
            else if(option_game == OptionGame::measure_height){
                auto t_position = cocos2d::Vec2(t_slot->getPosition().x,
                                                t_slot->getPosition().y + object->getImage()->getContentSize().height/2 - t_slot->getContentSize().height/2);
                object->runAction(cocos2d::MoveTo::create(0.2, t_position));
            }
        } else {
            //move start
            object->backToStart();
        }
    }
}

 
