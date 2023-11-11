
#include "GameWaterSpray.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"

INITIALIZE_READER(MJGameWaterSpray);
#define speed_opacity 25
USING_NS_CC;
std::once_flag waterspray_reader;
#define CSB_NAME "csb/game/waterspray/GameWaterSpray.csb"

MJGameWaterSpray * MJGameWaterSpray::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(waterspray_reader, [] {
        REGISTER_CSB_READER(MJGameWaterSpray);
        REGISTER_CSB_READER(MJGameWaterSpray_Item);
    });
    auto p = static_cast<MJGameWaterSpray *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameWaterSpray::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_waterspray.plist");
        buffer_string = JsonDump::getInstance()->MapToString(value_map);
    }
    
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    value_data.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	//generateData(value_data.GetObject());
	readConfig(value_data.GetObject(),images);
    
    layout_content = cocos2d::utils::findChild<Layout *>(this, "layout_content");
}

void MJGameWaterSpray::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
    auto content_info = value_data["content_info"].GetObject();
    auto list_object = content_info["list_items"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameWaterSpray_Item::ItemInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();
        
        game_dataset.list_objects.push_back(object_info);
    }
    
    auto list_distracting = content_info["list_distracting"].GetArray();
    for (int i = 0; i < (int) list_distracting.Size(); i++) {
        auto distracting = list_distracting[i].GetObject();
        MJGameWaterSpray_Item::ItemInfo distracting_info;
        distracting_info.text = distracting["text"].GetString();
        distracting_info.img_path = distracting["img_path"].GetString();
        game_dataset.list_distracting.push_back(distracting_info);
    }

}

void MJGameWaterSpray::onEnter(){
    ms::BaseActivity::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/waterspray/waterspray_bg.mp3",0.2f);
    }
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(MJGameWaterSpray::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(MJGameWaterSpray::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(MJGameWaterSpray::onTouchMoved, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(MJGameWaterSpray::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    setInputConfigGame();
    loadFrames();
	auto imgGameName = cocos2d::ui::ImageView::create("games/waterspray/gameName.png");
	this->addChild(imgGameName, 10001);
	imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);

	auto blackLayout = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(blackLayout, 10000);
	


	auto audio_id_gameName = cocos2d::experimental::AudioEngine::play2d("sounds/waterspray/gameName.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_gameName, [=](int au_id, std::string path){
		blackLayout->runAction(Sequence::createWithTwoActions(DelayTime::create(0.55f), RemoveSelf::create(true)));
		imgGameName->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f)
			, CallFunc::create([=](){
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
				head_monkey->playSequence("talk", true);
				auto audio_path = std::string("sounds/waterspray/waterspray_mc1.mp3");
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
					this->onStartGame();
					this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=]{
						head_monkey->playSequence("idle", true);
					}), NULL));
				});
			}), NULL));
		}), RemoveSelf::create(true), nullptr));
	});
	
}

void MJGameWaterSpray::setInputConfigGame(){
    auto size_objects = (int) game_dataset.list_objects.size();
    max_turn_water = min(size_objects, 4);
    max_item_water = 4;
    mj::helper::shuffle(game_dataset.list_objects);
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
    
}

void MJGameWaterSpray::loadFrames() {
	auto bg = cocos2d::utils::findChild(this, "bg");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width)
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);

    tay_layout = cocos2d::ui::Layout::create();
    tay_layout->setContentSize(cocos2d::Size(30, 30));
//    tay_layout->setBackGroundColor(Color3B::BLACK);
//    tay_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout_content->addChild(tay_layout, 100);
    tay_layout->setPosition(Vec2(880, 130));
    
    phun_nuoc_layout = cocos2d::ui::Layout::create();
    phun_nuoc_layout->setContentSize(cocos2d::Size(30, 30));
//    phun_nuoc_layout->setBackGroundColor(Color3B::BLACK);
//    phun_nuoc_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    tay_layout->addChild(phun_nuoc_layout, 1);
    phun_nuoc_layout->setPosition(Vec2(-225, -97.5));
    phun_nuoc_layout->setOpacity(180);
    
    auto tay = GAFWrapper::createGAF("gaf/waterspray/tay/tay.gaf");
    tay_layout->addChild(tay, 2);
    tay->setPosition(Vec2(-15, -175));
//    tay->play(true);
    
    phun_nuoc = GAFWrapper::createGAF("gaf/waterspray/phun_nuoc/phun_nuoc.gaf");
    phun_nuoc_layout->addChild(phun_nuoc);
    phun_nuoc->setPosition(Vec2(-300, -60));
    phun_nuoc->play(true);
    
    
    witdh_phunnuoc = Vec2(-560, -130).distance(Vec2(0, 0));
    
    auto nuoc_ban_ra = GAFWrapper::createGAF("gaf/waterspray/nuoc_ban_ra/nuoc_ban_ra.gaf");
    phun_nuoc_layout->addChild(nuoc_ban_ra);
    nuoc_ban_ra->setPosition(Vec2(-540, -100));
    nuoc_ban_ra->play(true);
    
    phun_nuoc_layout->setScale(0.25);
    phun_nuoc_layout->setVisible(false);
    
    head_monkey = GAFWrapper::createGAF("gaf/waterspray/head/head.gaf");
    layout_content->addChild(head_monkey, 101);
    head_monkey->setPosition(tay_layout->getPosition() + Vec2(-50, 105));
    head_monkey->playSequence("idle", true);
    
    auto than_monkey = cocos2d::ui::ImageView::create("games/waterspray/waterspray_monkey.png");
    layout_content->addChild(than_monkey, 99);
    than_monkey->setPosition(tay_layout->getPosition() + Vec2(42.5, -125));

    _imgFrames = cocos2d::ui::ImageView::create("games/waterspray/khung1.png");
    _imgFrames->ignoreContentAdaptWithSize(false);
    layout_content->addChild(_imgFrames, 1);
    _imgFrames->setPosition(tay_layout->getPosition() + Vec2(-550,-100));
    _imgFrames->setVisible(false);
    

    
    finish_gaf = GAFWrapper::createGAF("gaf/waterspray/clean/clean.gaf");
    layout_content->addChild(finish_gaf);
    finish_gaf->setPosition(layout_content->getContentSize()/2);
    finish_gaf->setVisible(false);

    _speaker = mj::helper::addSpeaker(this);
    _speaker->setPosition(Vec2(40, Director::getInstance()->getVisibleSize().height- 40));
    _speaker->setScale(0.9f);
    _speaker->setVisible(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d(_isStain ? "sounds/waterspray/waterspray_mc5.mp3" : game_dataset.list_objects[object_index].audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
        });
    });
}

void MJGameWaterSpray::onStartGame(){
    _isStain = true;
    count_xoavetban_done = 0;
    list_count_touch = {255, 255, 255, 255};
    list_vetban.clear();
    std::vector<int> list_id_vetban = {1, 2, 3, 4};
    mj::helper::shuffle(list_id_vetban);
    for (int i = 0; i < 4; i++) {
        auto vetban = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("games/waterspray/waterspray_vetban%d.png", list_id_vetban[i]));
        vetban->setPosition(Vec2(layout_content->getContentSize().width/2 + 120*(i%2*2-1), layout_content->getContentSize().height/2 + 120*(i/2*2-1)));
        layout_content->addChild(vetban);
        list_vetban.push_back(vetban);
    }
    
    mj::helper::shuffle(list_vetban);
    for (int i = 0; i < (int) list_vetban.size(); i++) {
        list_vetban[i]->setLocalZOrder(10-i);
    }
    
    if (!img_object) {
        img_object = cocos2d::ui::ImageView::create(game_dataset.list_objects[object_index].img_path);
        layout_content->addChild(img_object, 0);
		float fw = 518/img_object->getContentSize().width;
		float fh = 373 / img_object->getContentSize().height;

		img_object->setScale(fw<fh ? fw : fh);
        img_object->setPosition(layout_content->getContentSize()/2);
        img_object->setOpacity(0);
        img_object->runAction(Sequence::create(FadeIn::create(0.25f), CallFunc::create([=]{
            if (!game_play_enable && object_index > 0) {
                this->pauseGame();
                return;
            }
            index_type_voinuoc = 0;
        }), NULL));
    }
    text_object = cocos2d::ui::Text::create(game_dataset.list_objects[object_index].text, "fonts/monkeystories_ultra.ttf", 50);
    text_object->setTextColor(Color4B(165, 94, 23, 255));//BROWN COLOR
    text_object->setAnchorPoint(Point::ANCHOR_MIDDLE);
    if (text_object->getBoundingBox().size.width > _imgFrames->getContentSize().width)
    {
        _imgFrames->setContentSize(Size(text_object->getContentSize().width * 1.5, _imgFrames->getContentSize().height));
    }
    text_object->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text_object->setTextVerticalAlignment(TextVAlignment::CENTER);
    text_object->setPosition(_imgFrames->getContentSize()/2);
    _imgFrames->addChild(text_object, 1);
    _imgFrames->setVisible(true);
    text_object->setVisible(true);
    _speaker->setVisible(true);
    this->inActiveMode();
}

//void MJGameWaterSpray::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(this->getContentSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(this->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(this->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void MJGameWaterSpray::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	
	auto list_object = value_data["list_item"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameWaterSpray_Item::ItemInfo object_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		object_info.img_path = img_nature->path;

		game_dataset.list_objects.push_back(object_info);
	}

	auto list_distracting = value_data["list_distracting"].GetArray();
	for (int i = 0; i < (int)list_distracting.Size(); i++) {
		MJGameWaterSpray_Item::ItemInfo distracting_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_distracting[i].GetString());

		distracting_info.text = img_nature->getText();
		distracting_info.img_path = img_nature->path;
		game_dataset.list_distracting.push_back(distracting_info);
	}
}

void MJGameWaterSpray::onFinishGame(){
    unscheduleInActiveMode();
	layout_content->setVisible(false);
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/waterspray/KhiLaoCong_standalone/KhiLaoCong_standalone.gaf", true);
	}), NULL));
}

void MJGameWaterSpray::showItemQuestion(){
    _isStain = false;
    text_object->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
//        head_monkey->playSequence("talk", true);
        auto audio_path = std::string(game_dataset.list_objects[object_index].audio_path);
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path){
//            head_monkey->playSequence("idle", true);
            text_object->setTextColor(Color4B(165, 94, 23, 255));//BROWN COLOR
            text_object->runAction(Sequence::create(ScaleTo::create(0.1f, 1.0f), DelayTime::create(0.5f), CallFunc::create([=]{
                text_object->runAction(Sequence::create(FadeOut::create(0.25f), DelayTime::create(0.25f), CallFunc::create([=]{
                   // text_object->setString("");
                    text_object->setVisible(false);
                    text_object->getParent()->setVisible(false);
                    img_object->removeFromParent();
                    img_object = nullptr;
                    AUDIO_ENGINE->playEffect("sounds/waterspray/waterspray_imgshow.mp3");
                    this->showItemAnswer(MJGameWaterSpray_Item::IMG_TYPE);
                }), NULL));
            }), NULL));
        });

    }), ScaleTo::create(0.15f, 1.2f), CallFunc::create([=]{
        text_object->setTextColor(Color4B::RED);
    }), NULL));
    
}


void MJGameWaterSpray::showItemAnswer(int type){
    std::vector<MJGameWaterSpray_Item::ItemInfo> list_answer;
    auto list_distracting = game_dataset.list_distracting;
    for (int i = 0; i < (int) game_dataset.list_objects.size(); i++) {
        if (i == object_index) continue;
        list_distracting.push_back(game_dataset.list_objects[i]);
    }
    mj::helper::shuffle(list_distracting);
    
    for (int i = 0; i < max_item_water-1; i++) {
        list_answer.push_back(list_distracting[i]);
    }
    list_answer.push_back(game_dataset.list_objects[object_index]);
    mj::helper::shuffle(list_answer);
    
    for (int i = 0; i < list_answer.size(); i++) {
        CCLOG("i = %d | %s", i, list_answer[i].text.c_str());
    }
    listAnswerObject.clear();

    for (int i = 0; i < max_item_water; i++) {
        auto item = MJGameWaterSpray_Item::createGame(list_answer[i], type);
        auto pos_item = Vec2(layout_content->getContentSize().width/2 + 150*(i%2*2-1), layout_content->getContentSize().height/2 + 120*(i/2*2-1));
        item->setPosition(Vec2(pos_item.x, 850));
        item->setOpacity(0);
        layout_content->addChild(item);
        item->setDelegate(this);
        item->runAction(Sequence::create(DelayTime::create((i/2*2-1)*0.2f), Spawn::create(EaseBackOut::create(MoveTo::create(0.5f, pos_item)), FadeIn::create(0.5f), NULL), CallFunc::create([=]{
            item->setClickEnable(true);
            _speaker->setTouchEnabled(true);
        }), NULL));
        item->setName(StringUtils::format("item_%d", i));
        listAnswerObject.push_back(item);
    }
    
    if (type == MJGameWaterSpray_Item::TEXT_TYPE) {
        head_monkey->playSequence("talk", true);
        auto audio_path = std::string("sounds/waterspray/waterspray_mc2.mp3");
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            head_monkey->playSequence("idle", true);
            _currentAudio = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path){
                is_answer_enable = true;
            });
        });
    }else {
        head_monkey->playSequence("talk", true);
        auto audio_path = std::string("sounds/waterspray/waterspray_mc3.mp3");
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            head_monkey->playSequence("idle", true);
            _currentAudio = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path){
                is_answer_enable = true;
            });
        });
    }
    this->inActiveMode();
}


void MJGameWaterSpray::onClickItem(cocos2d::Ref *sender, int type){
    unscheduleInActiveMode();
    if (!is_answer_enable) return;
    auto item = static_cast<MJGameWaterSpray_Item *>(sender);
    if (item->_item_info.text == game_dataset.list_objects[object_index].text) {
        head_monkey->playSequence("talk", true);
        int rand_num = random(1,4);
        AUDIO_ENGINE->stopAllAudio();
        auto audio_path = StringUtils::format("sounds/waterspray/waterspray_awesome%d.mp3",rand_num);
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            head_monkey->playSequence("idle", true);
            this->inActiveMode();
        });
        for (auto item : listAnswerObject) {
            item->setTouchEnabled(false);
        }
        item->onRightAnswer(game_dataset.list_objects[object_index].audio_path);
        is_answer_enable = false;

    }else {
        for (auto item : listAnswerObject) {
            item->setClickEnable(false);
        }
        item->onWrongAnswer(game_dataset.list_objects[object_index].audio_path);
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
            for (auto item : listAnswerObject) {
                item->setClickEnable(true);
            }
            })
            , NULL));
    }
    
    if (!is_answer_enable) {
        for (auto item : listAnswerObject) {
            item->setClickEnable(false);
        }
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]{
            for (int i = 0; i < max_item_water; i++) {
                auto item = layout_content->getChildByName(StringUtils::format("item_%d", i));
                if (item) {
                    item->runAction(Sequence::create(Spawn::create(EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, 400))), FadeOut::create(0.4f), NULL), CallFunc::create([=]{
                        item->removeFromParent();
                    }), NULL));
                }
            }
            
            if (type == MJGameWaterSpray_Item::TEXT_TYPE) {
                object_index++;
                if (object_index >= max_turn_water) {//game finish
                    this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
                        finish_gaf->setVisible(true);
                        finish_gaf->play(true);
                        AUDIO_ENGINE->playEffect("sounds/waterspray/waterspray_finish.mp3");
                        
                        head_monkey->playSequence("talk", true);
                        auto audio_path = std::string("sounds/waterspray/waterspray_ending.mp3");
                        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                            head_monkey->playSequence("idle", true);
                            this->onFinishGame();
                        });
                        
                    }), NULL));
                    
                }else {//new turn
                    this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]{
                        if (object_index >= 3) {
                            head_monkey->playSequence("idle", true);
                        }else {
                            head_monkey->playSequence("talk", true);
                            auto audio_path = std::string(StringUtils::format("sounds/waterspray/waterspray_mc%d.mp3", random(4, 5)));
                            auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                                head_monkey->playSequence("idle", true);
                            });
                        }
                        
                        this->onStartGame();
                        
                    }), NULL));
                }
            }else {
                AUDIO_ENGINE->playEffect("sounds/waterspray/waterspray_closeshowup.mp3");
                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
                    this->showItemAnswer(MJGameWaterSpray_Item::TEXT_TYPE);
                }), NULL));
                is_answer_enable = true;
            }
            
        }), NULL));
    }
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameWaterSpray::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameWaterSpray::onRefresh, this);
    }
    return nullptr;
}

void MJGameWaterSpray::onRefresh(cocos2d::Ref *sender) {
	/*   this->removeFromParent();
	   auto phonics = MJGameWaterSpray::createGame("plist/game_phonics.json");
	   Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void MJGameWaterSpray::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    unscheduleInActiveMode();
}


//#pragma mark - Xoá Vết Bẩn.
bool MJGameWaterSpray::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    is_begin = true;
    unscheduleInActiveMode();
    cocos2d::Point location = touch->getLocation();
    pointTouch = location;
    if (index_type_voinuoc == 0) {
        index_type_voinuoc = 1;
        phun_nuoc_layout->setVisible(true);
        this->schedule(schedule_selector(MJGameWaterSpray::updateVetBan));
        sound_water_id = AUDIO_ENGINE->playEffect("sounds/waterspray/waterspray_water.mp3", true);
    }
    
    if (index_type_voinuoc != 3 && pointTouch.y >= 140) {
        if(pointTouch.x <= 700){
            tay_layout->setRotation(getAnglePhunNuoc());
            phun_nuoc_layout->setScale(getScalePhunNuoc());
        }
    }
    return true;
}

void MJGameWaterSpray::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    if (index_type_voinuoc != 3) {
        pointTouch = this->convertTouchToNodeSpace(touch);
            CCLOG("location = (%f, %f)", pointTouch.x, pointTouch.y);
        if (pointTouch.y < 150) return;
        if (pointTouch.x > 700) return;
        tay_layout->setRotation(getAnglePhunNuoc());
        phun_nuoc_layout->setScale(getScalePhunNuoc());
    }
}

void MJGameWaterSpray::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    is_begin = false;
    if (index_type_voinuoc == 2) {
        cleanVetBanDone();
    }
}

void MJGameWaterSpray::cleanVetBanDone(){
    _speaker->setTouchEnabled(false);
    unscheduleInActiveMode();
    inActiveMode();
    index_type_voinuoc = 3;
    auto duration = 0.25f;
    phun_nuoc_layout->runAction(Sequence::create(ScaleTo::create(duration, 0.25), CallFunc::create([=]{
        this->showItemQuestion();
    }), ScaleTo::create(0.25f, 0), CallFunc::create([=]{
        phun_nuoc_layout->stopAllActions();
        AUDIO_ENGINE->stopEffect(sound_water_id);
    }), NULL));
    tay_layout->runAction(RotateTo::create(duration, 0));
    
}

void MJGameWaterSpray::updateVetBan(float dt){
//    check_check -= 5;
    list_vetban[0]->setOpacity(check_check);
    auto location_touch = layout_content->convertToNodeSpace(pointTouch);
    int index = -1;
    for (int i = 0; i < list_vetban.size(); i++) {
        if (list_vetban[i]->isVisible() && list_vetban[i]->getBoundingBox().containsPoint(location_touch)) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        list_count_touch[index] -= speed_opacity;
        if (list_count_touch[index] <= 0) {
            list_vetban[index]->setVisible(false);
            count_xoavetban_done++;
            if (count_xoavetban_done >= 4) {
                 this->unschedule(schedule_selector(MJGameWaterSpray::updateVetBan));
                index_type_voinuoc = 2;//done
                AUDIO_ENGINE->playEffect("sounds/waterspray/waterspray_textshow.mp3");
                
                cleanVetBanDone();
                if (!is_begin) {
                    
                }
            }
        }else {
            list_vetban[index]->setOpacity(list_count_touch[index]);
            CCLOG("index = %d | opan = %d", index, list_count_touch[index]);
        }
    }
}

float MJGameWaterSpray::getAnglePhunNuoc(){
    auto location_touch = layout_content->convertToNodeSpace(pointTouch);
    auto huyen = location_touch.distance(tay_layout->getPosition());
    auto pos_ke = Vec2(location_touch.x, tay_layout->getPositionY());
    auto ke = pos_ke.distance(tay_layout->getPosition());
    float cos = ke/huyen;
    float  radian = acosf(cos);
    float angle = radian * 180/M_PI;
    if (location_touch.y < pos_ke.y) {
        angle = -angle;
    }
    angle += 15;
    CCLOG("angle = %f", pointTouch.y);
    return angle;
}

float MJGameWaterSpray::getScalePhunNuoc(){
    auto location_touch = tay_layout->convertToNodeSpace(pointTouch);
    auto huyen = location_touch.distance(phun_nuoc_layout->getPosition());
    CCLOG("(float)(huyen/witdh_phunnuoc) = %f", (float)(huyen/witdh_phunnuoc));
    return (float)(huyen/witdh_phunnuoc);
}

void MJGameWaterSpray::inActiveMode(){
    this->schedule([=](float) {
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d(_isStain ? "sounds/waterspray/waterspray_mc5.mp3" : game_dataset.list_objects[object_index].audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
            unscheduleInActiveMode();
            inActiveMode();
        });
    }, 10, 100, 10, "animated_guide");
}

void MJGameWaterSpray::unscheduleInActiveMode(){
        this->unschedule("animated_guide");
}
