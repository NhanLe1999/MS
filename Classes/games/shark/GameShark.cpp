
#include "GameShark.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "StoryConfigManager.h"

INITIALIZE_READER(MJGameShark);

USING_NS_CC;
std::once_flag shark_reader;
#define CSB_NAME "csb/game/shark/GameShark.csb"

MJGameShark * MJGameShark::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(shark_reader, [] {
        REGISTER_CSB_READER(MJGameShark);
        REGISTER_CSB_READER(MJGameShark_Fish);
    });
    auto p = static_cast<MJGameShark *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameShark::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_shark.plist");
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
    
}

void MJGameShark::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
    auto list_object = value_data["content_info"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameShark_Fish::FishInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();
        
        game_dataset.list_objects.push_back(object_info);
    }
}

void MJGameShark::onEnter(){
    ms::BaseActivity::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/shark/shark_bgsound.mp3");
    }
    setInputConfigGame();
    loadFrames();
   /* this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
        this->onStartGame();
    }), NULL));*/
}
void MJGameShark::loadFrames() {
    auto screensize = Director::getInstance()->getWinSize();
    for (int i = 0; i < (int) game_dataset.list_objects.size(); i++) {
        for (int j  = 0; j < max_item; j++) {
            auto fish = MJGameShark_Fish::createFish(game_dataset.list_objects[i]);
            fish->setPosition(Vec2(random(100, (int)screensize.width-100), random(100, 700)));
            this->addChild(fish, 2);
            fish->setDelegate(this);
            fish->run();
			list_fishs.pushBack(fish);
			if (i*max_item + j + 1>max_fish_on_screen)
				fish->setVisible(false);
        }
    }
    
    shark = MJGameShark_Fish::createShark();
    shark->setPosition(Vec2(random(0, 1)?-500:(screensize.width +500), screensize.height/2));
    this->addChild(shark, 99999999);
    shark->setDelegate(this);
    
    auto sanho = GAFWrapper::createGAF("gaf/shark/sanho/sanho.gaf");
    sanho->setPosition(Vec2(screensize.width/2, 50));
    this->addChild(sanho);
    sanho->play(true);
    
    auto canho = GAFWrapper::createGAF("gaf/shark/ca_nho/ca_nho.gaf");
    canho->setPosition(Vec2(screensize.width/2, screensize.height/2));
    this->addChild(canho);
    canho->play(true);
    if (CONFIG_MANAGER->isGameVMonkey()){
        onStartGame();
    }else{
        auto imgGameName = cocos2d::ui::ImageView::create("games/shark/gameName.png");
        this->addChild(imgGameName, 10000);
        imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
        auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/shark/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            imgGameName->runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(0.5f)
                , CallFunc::create([=](){
                onStartGame();
            }), RemoveSelf::create(true), nullptr));
        });
    }
	
}

void MJGameShark::setInputConfigGame(){
   // auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	max_turn = min(max_turn, (int)game_dataset.list_objects.size());
	if (max_turn <= 3)
		max_item = 5;
	//game_dataset.list_objects.erase(game_dataset.list_objects.begin() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
}

void MJGameShark::onStartGame(){
    auto audio_path = std::string("sounds/shark/shark_mc1.mp3");
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path);
        isPlay = true;
        auto text = cocos2d::ui::Text::create(game_dataset.list_objects[object_index].text, "fonts/monkeystories_ultra.ttf", 80);
        text->setScale(0);
        this->addChild(text, 99999999);
        text->setPosition(this->getContentSize()/2);
        text->enableOutline(Color4B::BLACK, 3);
        
        text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(0.3f), EaseBackIn::create(ScaleTo::create(0.25f, 0)), CallFunc::create([=]{
            text->removeFromParent();
        }), NULL));
    });
    
    
}

//void MJGameShark::pauseGame(){
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

void MJGameShark::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameShark_Fish::FishInfo object_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		object_info.img_path = img_nature->path;

		game_dataset.list_objects.push_back(object_info);
	}
}

void MJGameShark::onFinishGame(){
   
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/shark/shark_endgame_gaf/shark_endgame_gaf.gaf", false);
	}), NULL));
}

void MJGameShark::onFish(cocos2d::Ref *sender){
    if (!isPlay) return;
    auto fish = dynamic_cast<MJGameShark_Fish *>(sender);
    if (fish && fish->_item_info.text == game_dataset.list_objects[object_index].text) {
        AUDIO_ENGINE->playEffect("sounds/shark/shark_correct.mp3");
        CCLOG("fish text = %s", fish->_item_info.text.c_str());
        shark->sharkRunToGoal(fish->getPosition(), fish->flipx);
//        if (shark->flipx != fish->flipx) {
//            fish->fishRunWhenTap(shark->pos_tap_to, fish->time_shark);
//        }
        fish->setName("this_is_fish");
        isPlay = false;
        
    }
}

void MJGameShark::onSharkRun(cocos2d::Ref *sender){
	auto fish = static_cast<MJGameShark_Fish*>(this->getChildByName("this_is_fish"));
    if (fish) {
		list_fishs.eraseObject(fish);
		for (int i = 0; i < list_fishs.size(); i++)	{
			if (i < max_fish_on_screen){
				if (!list_fishs.at(i)->isVisible()){
					list_fishs.at(i)->setOpacity(0);
					list_fishs.at(i)->setVisible(true);
					list_fishs.at(i)->runAction(FadeIn::create(0.3f));
				}
			}
			else
				break;
		}
        fish->removeFromParent();
        count_fish_tap++;
        if (count_fish_tap >= max_item) {
            isPlay = false;
            this->scheduleOnce([=](float){
                object_index++;
                count_fish_tap = 0;
                if (object_index >= max_turn) {
                    onFinishGame();
                }else {
                    onStartGame();
                }
                
            }, 1.0f, "onStartGame");
        }else{
			if (!game_play_enable){
				auto value_pause = min(2, max_item);
				if (count_fish_tap >= value_pause) {
					this->pauseGame();
					return;
				}
			}
            
            this->scheduleOnce([=](float){
                isPlay = true;
            }, 1.75f, "onStartGame");
        }
    }
}

#pragma callback
ui::Widget::ccWidgetClickCallback MJGameShark::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameShark::onRefresh, this);
    }
    return nullptr;
}

void MJGameShark::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = MJGameShark::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
    
}

void MJGameShark::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}
