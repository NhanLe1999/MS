#include "ToMau.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
#include "Shape2DHitUtility.h"
INITIALIZE_READER(ToMau);

std::once_flag ToMau_reader;
#define CSB_NAME "csb/game/ToMau/ToMau_CSB.csb"

ToMau * ToMau::createGame() {
	std::call_once(ToMau_reader, [] {
		REGISTER_CSB_READER(ToMau);
	});
	auto p = static_cast<ToMau *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void ToMau::didLoadFromCSB() {
	
    this->generateData();
	screen_size = Director::getInstance()->getVisibleSize();
	//    autoPlay = isAutoPlay;
	this->loadFrames();
}

void ToMau::generateData() {
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());
    
    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    
    //  get data
    string json_file = "config.json";
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    
    if (JSON_SAFE_CHECK(document, "question", String)){
        auto q_id = document.FindMember("question")->value.GetString();
        auto qImage = math::resource::ResHelper::getImage(images, q_id);

        question.text = qImage->getQuestionVMonkey("name_1", lang);
        question.audio_path = qImage->getAudios("name_1",  lang).front();
        question.image_path = qImage->path;
    }
    
    if(document.HasMember("is_free")){
        is_free = document.FindMember("is_free")->value.GetBool();
    }
    
    if(document.HasMember("kieu_doc")){
        is_portrait = document.FindMember("kieu_doc")->value.GetBool();
    }
    
    //TODO: GAME CONFIG

    if (document.HasMember("color_story")) {
        rapidjson::Value& colors = document.FindMember("color_story")->value.FindMember("list")->value;
        for (rapidjson::SizeType i = 0; i < colors.Size(); i++) {
            selected_color.push_back(colors[i].GetString());
        }
        
    }
    
    //Lấy hết màu có sẵn
    list_color = math::resource::ResHelper::getImages("game/colors.json");
    for (int i=0; i<list_color.size(); i++) {
        list_color[i]->path = cocos2d::StringUtils::format("%s%s", m_theme->getThemePath().c_str(), list_color[i]->path.c_str());
    }
    
    //Bỏ màu trắng
    for (int i = 0; i<list_color.size(); i++) {
        if (list_color[i]->getText("color") == "white") {
            list_color.erase(list_color.begin()+i);
            break;
        }
    }
    //Chon những màu được config
    if (!selected_color.empty()) {
        std::vector<math::resource::Image*> choose_color_img;
        for (std::string color_select : selected_color) {
            for (math::resource::Image* img : list_color) {
                if (color_select == img->getText("color")) {
                    choose_color_img.push_back(img);
                    break;
                }
            }
        }
        list_color = choose_color_img;
    }
    mj::helper::shuffle(list_color);
    
    loadColor();
    
}

void ToMau::loadColor(){
    colors_map[""] = Color3B::WHITE;
    colors_map["black"] = Color3B(17, 15, 16);
    colors_map["blue"] = Color3B(58, 156, 223);
    colors_map["brown"] = Color3B(124, 55, 21);
    colors_map["green"] = Color3B(9, 168, 0);
    colors_map["grey"] = Color3B(127, 126, 127);
    colors_map["orange"] = Color3B(239, 108, 0);
    colors_map["pink"] = Color3B(252,45,120);
    colors_map["purple"] = Color3B(162, 73, 174);
    colors_map["red"] = Color3B(255, 16, 34);
    colors_map["yellow"] = Color3B(255, 209, 16);
    colors_map["indigo"] = Color3B(73,1,209);
}

void ToMau::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
	this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] {

		//         this->inActiveMode();

	}), NULL));

	
}

void ToMau::loadFrames() {
    root_layout = utils::findChild<Layout*>(this, "root_layout");
    screen_size = root_layout->getContentSize() ;
    
    auto text_question = utils::findChild<Text*>(this, "Text_1");
    text_question->setFontName(font_name);
    text_question->setFontSize(35);
    text_question->setString(question.text);
    audio_id_question = AUDIO_ENGINE->playSound(question.audio_path);
    
    img_holder = utils::findChild<Layout*>(this, "Panel_2");
    img_holder->setBackGroundColorType(BackGroundColorType::NONE);
    
    if(is_free){
        img_holder->setContentSize(utils::findChild<ImageView*>(this, "Image_vien")->getContentSize());
        
        auto layout_touch = Layout::create();
        root_layout->addChild(layout_touch, 100);
        layout_touch->setPosition(img_holder->getPosition());
        layout_touch->setContentSize(img_holder->getContentSize());
        layout_touch->setTouchEnabled(true);
        layout_touch->addTouchEventListener(CC_CALLBACK_2(ToMau::onTouchPanel, this));
    }
    
    for(auto img: img_holder->getChildren()){
        auto imageView = (ImageView*) img;
        if( imageView->isTouchEnabled() ){
            if(is_free){
                imageView->setTouchEnabled(false);
            }
            objects_order.push_back(imageView);
            auto render_size = imageView->getVirtualRendererSize();
            cocos2d::Rect cap = cocos2d::Rect(1, 1, render_size.width - 2, render_size.height - 2);
            imageView->setCapInsets(cap);
            
            auto image_touch = new cocos2d::Image();
            image_touch->initWithImageFile(imageView->getTextureFilePath());
            imageView->setVisible(true);
            
            objects_list[imageView] = image_touch;
        }
    }
    //sắp xếp thứ tự duyệt
    std::sort(objects_order.begin(), objects_order.end(), [](ImageView* i1, ImageView* i2)-> bool {
        auto s1 = i1->getContentSize().width* i1->getContentSize().height;
        auto s2 = i2->getContentSize().width* i2->getContentSize().height;
        return s1 < s2 ;
    });
    
    auto next_btn = Button::create("games/ToMau/icon.png");
    this->addChild(next_btn);
    next_btn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width*0.94, Director::getInstance()->getVisibleSize().height*0.08));
    next_btn->addClickEventListener(CC_CALLBACK_1(ToMau::onNext, this));
    auto next_icon = ImageView::create("games/ToMau/next_icon.png");
    next_btn->addChild(next_icon);
    next_icon->ignoreContentAdaptWithSize(false);
    next_icon->setContentSize(next_btn->getContentSize()*0.5);
    next_icon->setPosition(next_btn->getContentSize()/2);
    
    color_panel = utils::findChild<Layout*>(this, "right_panel");
    color_panel->setLocalZOrder(100);
   
    auto win_size = Director::getInstance()->getVisibleSize();
    auto win_width = win_size.width;
    auto win_height = win_size.height;
    
    if (is_portrait) {
        color_panel->setContentSize(Size(250, 500));
        showRightColorPad(selected_color, CC_CALLBACK_1(ToMau::selectPen, this), nullptr);
        color_panel->setPosition(root_layout->convertToNodeSpace(Vec2(win_width*0.95, win_height*0.5)));
    }else{
        color_panel->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        auto panel_width = screen_size.width - ((win_width - next_btn->getPositionX())*2);
        color_panel->setContentSize(Size(panel_width, screen_size.height*0.25));
        auto start_x = root_layout->getPositionX() - 512;
        showBottomColorPad(selected_color, CC_CALLBACK_1(ToMau::selectPen, this), nullptr);
        
        if(win_width > screen_size.width){
            start_x = (screen_size.width - panel_width)/2;
        }
        color_panel->setPosition(Vec2(start_x, screen_size.height*0.1));
    }
}

void ToMau::onTouchPanel(Ref* sender, TouchEventType type){
    auto layout_touch = (Layout*)sender;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            touch_pos = layout_touch->convertToNodeSpace(layout_touch->getTouchBeganPosition());
           
            CCLOG("pos began: %f, %f", layout_touch->getTouchBeganPosition().x, layout_touch->getTouchBeganPosition().y);
            
            for (auto obj : objects_order) {
                if(obj->getBoundingBox().containsPoint(touch_pos)){
                    if(is_free){
                        auto image_touch = objects_list[obj];
                        CCLOG("image sz: %d, %d", image_touch->getWidth(), image_touch->getHeight());
                        CCLOG("obj sz: %f, %f", obj->getContentSize().width, obj->getContentSize().height);
                        
                        auto convert_touch = cocos2d::Vec2(image_touch->getWidth() / obj->getContentSize().width, image_touch->getHeight() / obj->getContentSize().height);
                        auto obj_touch_pos = obj->convertToNodeSpace(layout_touch->getTouchBeganPosition());
                        auto img_touch_pos = cocos2d::Vec2(obj_touch_pos.x*convert_touch.x, obj_touch_pos.y*convert_touch.y);
                        CCLOG("pos convert: %f, %f", img_touch_pos.x, img_touch_pos.y);
                        if( Shape2DHitUtility::tapsOnNonTransparent(img_touch_pos, image_touch)){
                            AUDIO_ENGINE->play("sounds/ToMau/toMau.mp3");
                            obj->setColor(colors_map[current_color]);
                            break;
                        }else{
                            AUDIO_ENGINE->play("sounds/ToMau/koToMau.mp3");
                        }
                    }
                }
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
            
            break;
        default:
            break;
    }
}

void ToMau::onTouchObject(Ref* sender, TouchEventType type){
    
    auto touch_obj = (ImageView*) sender;
    auto image_path = touch_obj->getTextureFilePath();
        
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            touch_pos = img_holder->convertToNodeSpace(touch_obj->getTouchBeganPosition());
            CCLOG("touch began");
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
             CCLOG("touch moved");
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
             CCLOG("touch ended");
            for (auto obj : objects_order) {
                if(obj->getBoundingBox().containsPoint(touch_pos)){
                    if(utils::findChild<Text*>(obj, current_color) ){
                        auto image_touch = objects_list[obj];
                        CCLOG("image sz: %d, %d", image_touch->getWidth(), image_touch->getHeight());
                        CCLOG("obj sz: %f, %f", obj->getContentSize().width, obj->getContentSize().height);
                        
                        auto convert_touch = cocos2d::Vec2(image_touch->getWidth() / obj->getContentSize().width, image_touch->getHeight() / obj->getContentSize().height);
                        auto obj_touch_pos = obj->convertToNodeSpace(touch_obj->getTouchBeganPosition());
                        auto img_touch_pos = cocos2d::Vec2(obj_touch_pos.x*convert_touch.x, obj_touch_pos.y*convert_touch.y);
                        CCLOG("pos convert: %f, %f", img_touch_pos.x, img_touch_pos.y);
                        if( Shape2DHitUtility::tapsOnNonTransparent(img_touch_pos, image_touch)){
                            AUDIO_ENGINE->play("sounds/ToMau/toMau.mp3");
                            //if(obj->getOpacity() < 10){
                                obj->setOpacity(255);
                            //}else {
                                obj->setColor(colors_map[current_color]);
                            //}
                           
                            break;
                        }else{
                            AUDIO_ENGINE->play("sounds/ToMau/koToMau.mp3");
                        }
                    }else{
                        AUDIO_ENGINE->play("sounds/ToMau/koToMau.mp3");
                    }
                }
                
            }
            
            break;
            
        default:
            break;
    }
}

#include "PoolManager.h"
void ToMau::showRightColorPad(std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done ){
    float bar_size = 200;
    
    _color_pens.clear();
    
    float pen_width = 300;
    float pen_height = 89;
    while(500 - colors.size()*(pen_height) < 0){
        pen_height -= 5;
    }
    for (std::string color : colors) {
        for(auto pen_img : list_color){
            if (pen_img->getText() == color) {
                auto pen = mpool->getObject();
                pen->setEnable(0, math::common::touch, pen_img, nullptr);
                pen->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
                pen->setObjectSize(cocos2d::Size(300, pen_height));
                auto render_size = pen->getImage()->getVirtualRendererSize();
                pen->getImage()->setScale9Enabled(true);
                pen->getImage()->setCapInsets(cocos2d::Rect(render_size.width*0.5, 1, render_size.width*0.1, render_size.height-2));
                _color_pens.push_back(pen);
                color_panel->addChild(pen);
                                  
                    pen->setTouchCallback([=](Node* node) {
                        Math_Object* selected = static_cast<Math_Object*>(node);
                        float start_position_x = bar_size;
                        float target_position_x = bar_size * 0.5 + 40;// (bar_size + 50) *  - 70;// 180;
                        for (Math_Object* pen : _color_pens) {
                            if (abs(pen->getPosition().x - target_position_x) < 10) {
                                if (pen->isSame(selected)) {
                                    return;
                                }
                                pen->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(start_position_x, pen->getPosition().y))));
                            }
                        }
                        auto current_pos= selected->getPosition();
                        selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(target_position_x, current_pos.y))));
                        _color_selected->setVisible(true);
                        _color_selected->setPosition(cocos2d::Vec2(current_pos.x, current_pos.y+5));
                        _color_selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(target_position_x, current_pos.y+5))));
                        if (callback != nullptr) {
                            callback(node);
                        }
                    });
                }
                else {
                    CCLOG("Math Error: Missing color '%s'", color.c_str());
                }
            }
    }
    if (_color_pens.empty()) {
        return ;
    }
    math::func::shuffle(_color_pens);
    
    //ui pad color
    _color_selected = cocos2d::ui::ImageView::create("themeui/penselect.png");
    _color_selected->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _color_selected->setScale9Enabled(true);
    _color_selected->setCapInsets(cocos2d::Rect(_color_selected->getVirtualRendererSize().width*0.4, 1, _color_selected->getVirtualRendererSize().width*0.2, _color_selected->getVirtualRendererSize().height-2));
    _color_selected->setContentSize(cocos2d::Size(pen_width+50, 130));
    _color_selected->setPosition(cocos2d::Vec2::ZERO);
    _color_selected->setVisible(false);
   
    color_panel->addChild(_color_selected);
    
    auto pen_size = _color_pens.front()->getContentSize();
    
    float delta = 20;
    while ((color_panel->getContentSize().height - (int)_color_pens.size() * (pen_size.height + delta) - delta * 2) < 0) {
        delta -= 5;
    }
    float start_position_y = ((callback_done == nullptr) ? (color_panel->getContentSize().height * 0.5) : (color_panel->getContentSize().height - 25 - 250/2)) + ((int)_color_pens.size() * (pen_size.height + delta) - delta)/2 - pen_size.height/2;

    int i = 0;
    for (Math_Object* pen : _color_pens) {
//        pen->setContentSize(Size(pen_width, pen_height));
        pen->getImage()->setPosition(pen->getContentSize()/2);
        pen->setPosition(cocos2d::Vec2(bar_size, start_position_y));
//         pen->onShow(0.4 + 0 + 0.1 * ((int)_color_pens.size()- (i++)), math::animation::SlideLeft, color_panel->getContentSize().width*.7);
        start_position_y -= (pen_size.height + delta);
        pen->runAction(FadeIn::create(0));
    }
    
}

void ToMau::showBottomColorPad(std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done){
    
    float pen_width = 300;
    for (std::string color : colors) {
        for(auto pen_img : list_color){
            if (pen_img->getText() == color) {

                auto pen = mpool->getObject();
                pen->setEnable(0, math::common::touch, pen_img, nullptr);
                pen->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
                pen->setObjectSize(cocos2d::Size(300, 89));
                auto render_size = pen->getImage()->getVirtualRendererSize();
                pen->getImage()->setScale9Enabled(true);
                pen->getImage()->setCapInsets(cocos2d::Rect(render_size.width*0.5, 1, render_size.width*0.1, render_size.height-2));
                _color_pens.push_back(pen);
                
                color_panel->addChild(pen);
                pen->setTouchCallback([=](Node* node) {
//                    this->disableControlQuiz(0.3);
                    Math_Object* selected = static_cast<Math_Object*>(node);
                    AUDIO_ENGINE->play("sounds/ToMau/chonMau.mp3");
                    float start_position_y = screen_size.height*0.05;
                    float target_position_y = screen_size.height*0.15-40;
                   
                    for (Math_Object* pen : _color_pens) {
                        if (abs(pen->getPosition().y - target_position_y) < 20) {
                            if (pen->isSame(selected)) {
//                                return;
                            }else{
                                pen->runAction(cocos2d::EaseBackOut::create( cocos2d::MoveTo::create(0.3, cocos2d::Vec2(pen->getPosition().x, start_position_y))));
                            }
                        }
                    }
                    auto current_pos= selected->getPosition();
                    selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(current_pos.x, target_position_y))));
                    _color_selected->setVisible(true);
                    _color_selected->setPosition(cocos2d::Vec2(current_pos.x + 5, current_pos.y));
                    _color_selected->runAction(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.3, cocos2d::Vec2(current_pos.x + 5, target_position_y))));
                    if (callback != nullptr) {
                        callback(node);
                    }
                });
                
            } else {
                CCLOG("Math Error: Missing color '%s'", color.c_str());
            }
        }
    }
    
    if (_color_pens.empty()) {
        return ;
    }
//    math::func::shuffle(_color_pens);
    
    //ui pad color
    _color_selected = cocos2d::ui::ImageView::create("themeui/penselect.png");
    _color_selected->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _color_selected->setScale9Enabled(true);
    _color_selected->setCapInsets(cocos2d::Rect(_color_selected->getVirtualRendererSize().width*0.4, 1, _color_selected->getVirtualRendererSize().width*0.2, _color_selected->getVirtualRendererSize().height-2));
    _color_selected->setContentSize(cocos2d::Size(pen_width+50, 130));
    _color_selected->setPosition(cocos2d::Vec2::ZERO);
    _color_selected->setVisible(false);
    
    color_panel->addChild(_color_selected);
//    addChildNodeToBottomPanel(_color_selected, 2);
    
    _color_selected->setRotation(90);
    
    auto pen_size = _color_pens.front()->getContentSize();
    float delta = 40;
    while ((color_panel->getContentSize().width - (int)_color_pens.size() * (pen_size.height + delta) - delta * 2) < 0) {
        delta -= 5;
    }
   
    float start_position_x = ((callback_done == nullptr) ? (color_panel->getContentSize().width*0.5) : (25+color_panel->getContentSize().width/2)) - ((int)_color_pens.size() * (pen_size.height + delta) - delta)/2 + pen_size.height/2;
    
    for (Math_Object* pen : _color_pens) {
        pen->setContentSize(cocos2d::Size(pen_size.height, pen_size.width));
        pen->getImage()->setPosition(pen->getContentSize()/2);
        pen->getImage()->setRotation(90);
        pen->setPosition(cocos2d::Vec2(start_position_x, color_panel->getPositionY()));
        start_position_x += (pen_size.height + delta);
    }
}

void ToMau::selectPen(Ref* sender){
    current_color = static_cast<Math_Object*>(sender)->getComparisionValue().color;
    CCLOG("%s: ", current_color.c_str());
}


void ToMau::onNext(Ref* sender){
    CCLOG("on next");
    //show alert hỏi ghi lại ảnh, ok = ghi lại r finish, no = finish game
    StoryAlert::showAlert2Button("Bé có muốn lưu lại bức tranh này?", "Có", "Không", false, 28, this);
}

#include "MJPlatformConfig.h"
void ToMau::onAlertOK(cocos2d::Ref *sender){
    CCLOG("Ok");
     auto board = utils::findChild(this, "board");
    
    auto img_pos = img_holder->getParent()->convertToWorldSpace(img_holder->getPosition()) ;
    img_holder->retain();
    img_holder->removeFromParent();
    board->addChild(img_holder);
    img_holder->release();
    img_holder->setPosition(board->convertToNodeSpace(img_pos));
    
    auto board_pos = board->getParent()->convertToWorldSpace(board->getPosition());
    board->retain();
    board->removeFromParent();
    this->addChild(board);
    board->setPosition(this->convertToNodeSpace(board_pos));
    board->release();
    
    auto board_size = board->getContentSize();
    auto ap = board->getAnchorPoint();
    board_pos = board->getPosition();
    auto start_x = board_pos.x - board_size.width/2;
    auto start_y = board_pos.y - board_size.height/2;
    
    RenderTexture* renderTexture = RenderTexture::create(board_size.width, board_size.height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->setAutoDraw(false);
    renderTexture->setKeepMatrix(true);
    renderTexture->setVirtualViewport(board_pos - Vec2(board_size.width*ap.x, board_size.height * ap.y), Rect(0, 0, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height),Rect(start_x, start_y, Director::getInstance()->getWinSizeInPixels().width,Director::getInstance()->getWinSizeInPixels().height));
    renderTexture->begin();
    board->visit();
    renderTexture->end();
    
    auto name = StringUtils::format("Screenshot_%s.png", LTDate::getCurrentDate().toPrettyFileName().c_str());
    renderTexture->saveToFile(name, Image::Format::PNG);
    auto full_name = FileUtils::getInstance()->getWritablePath() + name;
  
    auto listener = EventListenerCustom::create("save picture", [=](EventCustom* event){
        MJMainGBase::onFinishGameVMonkey();
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this);
    auto deny_listener = EventListenerCustom::create("deny save picture", [=](EventCustom* event){
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
         StoryAlert::showAlert("Ảnh chưa được lưu. Hãy cấp quyền trong cài đặt để lưu ảnh", "OK", "");
        });
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( deny_listener, this);
    this->scheduleOnce([=](float){
        if(FileUtils::getInstance()->isFileExist(full_name)){
            MJ_PLATFORM_CONFIG.saveScreenShot(full_name, name);
        }
    }, 0.01f, "save_Screenshot");
}

void ToMau::onAlertCancel(cocos2d::Ref *sender){
    CCLOG("cancel");
     MJMainGBase::onFinishGameVMonkey();
}

void ToMau::onExitTransitionDidStart() {
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void ToMau::showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path) {
	auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		auto hand = GAFWrapper::createGAF(gaf_guide_path);
		guide_layout->addChild(hand, 1900000000);
		hand->setPosition(pos1);
		hand->play(false, [=](gaf::GAFObject *obj) {
			hand->runAction(Sequence::create(MoveTo::create(0.6f, pos2),
				CallFunc::create([=]() {
				hand->play(false, [=](gaf::GAFObject *obj) {
					hand->removeFromParent();
				});
			}), NULL));
		});
	}, delay_time, 100, delay_time, "animated_guide");
}

void ToMau::hideTapGuideGame() {
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
}

void ToMau::inActiveMode(float delay_time) {
	this->schedule([=](float) {
		//this->showTapGuideGame();
	}, 7, 100, delay_time, "animated_guide");

}

void ToMau::onFinishGame() {
	MJMainGBase::onFinishGame();
}

void ToMau::onClickSpeaker(Ref* sender){
    AUDIO_ENGINE->stopSound(question.audio_path);
    audio_id_question = AUDIO_ENGINE->playSound(question.audio_path);
}

ui::Widget::ccWidgetClickCallback ToMau::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(ToMau, onClickSpeaker)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
	return nullptr;
}

ui::Widget::ccWidgetTouchCallback ToMau::onLocateTouchCallback(const std::string &callback_name){
    if (callback_name == "onTouchObject") {
        return CC_CALLBACK_2(ToMau::onTouchObject, this);
    }
    return nullptr;
}
