
#include "GameWordTrain.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"

INITIALIZE_READER(MJGameWordTrain);
#define speed_opacity 15
USING_NS_CC;
std::once_flag GameWordTrain_reader;
#define CSB_NAME "csb/game/word_train/GameWordTrain.csb"

MJGameWordTrain * MJGameWordTrain::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(GameWordTrain_reader, [] {
        REGISTER_CSB_READER(MJGameWordTrain);
        REGISTER_CSB_READER(MJGameWordTrain_Item);
    });
    auto p = static_cast<MJGameWordTrain *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "APProfileManager.h"

void MJGameWordTrain::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    
    _speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    _speaker->setPosition(Vec2(70,Director::getInstance()->getWinSize().height - 60));
    _speaker->setTouchEnabled(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender){
        unscheduleInActiveMode();
        inActiveMode();
        auto path = "sounds/word_train/wordtrain_mc.mp3";
        if (CONFIG_MANAGER->isGameVMonkey()){
            path = question_vi.c_str();
           }
        stopListAudio();
        _speakerAudioPath= AUDIO_ENGINE->play(path);
    });
    this->addChild(_speaker);
    
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_wordtrain.plist");
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
	readConfig(value_data.GetObject(), images);
}

void MJGameWordTrain::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    auto content_info = value_data["content_info"].GetObject();
    auto character = content_info["character"].GetString();
    auto character_audio_path = content_info["character_audio_path"].GetString();
    
    auto list_object = content_info["list_items"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameWordTrain_Item::ItemInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();
//        object_info.start = object_value["start"].GetInt();
//        object_info.end = object_value["end"].GetInt();
        object_info.character = character;
        object_info.character_audio_path = character_audio_path;
        
        game_dataset.list_objects.push_back(object_info);
        
    }
    
    auto list_distracting = content_info["list_distracting"].GetArray();
    for (int i = 0; i < (int) list_distracting.Size(); i++) {
        MJGameWordTrain_Item::ItemDistracting object_distracting;
        auto object_value = list_distracting[i].GetObject();
        object_distracting.text = object_value["text"].GetString();
        object_distracting.audio_path = object_value["audio_path"].GetString();
        game_dataset.list_distracting.push_back(object_distracting);
    }
    
}

void MJGameWordTrain::onEnter(){
    ms::BaseActivity::onEnter();
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/word_train/wordtrain_bg.mp3");
    }
    setInputConfigGame();
    loadFrames();
}

void MJGameWordTrain::setInputConfigGame(){
//    auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
    mj::helper::shuffle(game_dataset.list_distracting);
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
    
}

void MJGameWordTrain::loadFrames() {
    _bling = GAFWrapper::createGAF("khung/sao.gaf");
    _bling->setPosition(Vec2(2*Director::getInstance()->getVisibleSize().width / 3, 2 * Director::getInstance()->getVisibleSize().height / 3));
    this->addChild(_bling, 100, "bling");
    _bling->setVisible(false);

    board = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "board");
    auto size_screen = Director::getInstance()->getWinSize();
    auto layout_bg = this->getChildByName("layout_bg");
    layout_bg->setPositionX(-(4660-size_screen.width-512));
    
    auto audio_index = AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_tauhoa.mp3");
    auto duration = 1.0f/1024;
    layout_bg->runAction(Sequence::create(MoveTo::create(duration*(size_screen.width/2 - layout_bg->getPositionX()), Vec2(size_screen.width/2, size_screen.height/2)), CallFunc::create([=]{
        AUDIO_ENGINE->stopEffect(audio_index);
        AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_coi.mp3");
        train->pauseAnimation();
    }), NULL));
    train = GAFWrapper::createGAF("gaf/word_train/train/train.gaf");
    pos_train = Vec2(size_screen.width/2+190,325);
    train->setPosition(pos_train);
    this->addChild(train);
    train->playSequence("run", true);
    
    if (CONFIG_MANAGER->isGameVMonkey()){
        this->scheduleOnce([=](float) {
            auto audio_path = std::string("sounds/word_train/wordtrain_mc.mp3");
            if (CONFIG_MANAGER->isGameVMonkey()){
                audio_path = question_vi;
            }
            auto audio_index_mc = cocos2d::experimental::AudioEngine::play2d(audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_index_mc, [=](int au_id, std::string path){
                this->onStartGame();
            });
        }, 1.f, "delay start");
    }else{
        auto imgGameName = cocos2d::ui::ImageView::create("games/word_train/gameName.png");
        this->addChild(imgGameName,10000);
        imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
        
        auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/word_train/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            imgGameName->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(0.5f)
                , CallFunc::create([=](){
                auto audio_path = std::string("sounds/word_train/wordtrain_mc.mp3");
                if (CONFIG_MANAGER->isGameVMonkey()){
                    audio_path = question_vi;
                }
                auto audio_index_mc = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_index_mc, [=](int au_id, std::string path){
                    this->onStartGame();
                    inActiveMode();
                });
            }), RemoveSelf::create(true), nullptr));
        });
    }
	
	
}

void MJGameWordTrain::onStartGame(){
    _speaker->setTouchEnabled(true);
    m_speakable = true;
    _panelBoard = cocos2d::utils::findChild<cocos2d::ui::Layout*>(board, "panel_board");
    //auto item_img = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(_panelBoard, "Image_2");
    auto item_img = cocos2d::ui::ImageView::create(game_dataset.list_objects[object_index].img_path);
    item_img->ignoreContentAdaptWithSize(false);
    item_img->setPosition(_panelBoard->getContentSize() / 2);
    item_img->setContentSize(_panelBoard->getContentSize());
    item_img->setOpacity(0);
    item_img->runAction(FadeIn::create(0.5f));
    item_img->setName("item_img");
   
    _panelBoard->addChild(item_img,INT_MAX -1);
    _panelBoard->addClickEventListener([=](Ref* sender) {
        _panelBoard->setTouchEnabled(false);
        unscheduleInActiveMode();
        inActiveMode();
        stopListAudio();
         if (utils::findChild(this, "bling") != NULL) {
             _bling->setVisible(false);
         }
         _imageAudioPath = AUDIO_ENGINE->playSound(game_dataset.list_objects[object_index].audio_path,false,[=]() {
             _panelBoard->setTouchEnabled(true);
			});
		});
        
    _panelBoard->setZOrder(INT_MAX - 1);
    _panelBoard->setTouchEnabled(true);
    
    std::vector<MJGameWordTrain_Item::ItemDistracting> list_distracting;
    if ((int)game_dataset.list_distracting.size() > 2) {
        game_dataset.list_distracting.push_back(game_dataset.list_distracting[0]);
        game_dataset.list_distracting.erase(game_dataset.list_distracting.begin());
        list_distracting = game_dataset.list_distracting;
    }else {
        list_distracting = game_dataset.list_distracting;
        mj::helper::shuffle(list_distracting);
    }

   

    auto item = MJGameWordTrain_Item::createGame(game_dataset.list_objects[object_index], list_distracting[0]);
    train->addChild(item);
    item->setPosition(Vec2(527, 120));
    item->setName("wordtrain_item");
    item->setDelegate(this);
    if (object_index != 0) {
        AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_coi.mp3");
    }
    train->playSequence("show", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        train->playSequence("bubble", true);
        item->onShow([=]() {
            item->scheduleOnce([=](float dt) {
                AUDIO_ENGINE->play(game_dataset.list_objects[object_index].audio_path, false, 1.0, [=]() {
                    });
                }, 0.5,"delay_show_up");
            if (_bling) {

                _bling->runAction(Sequence::create(Show::create(), CallFuncN::create([](Node* n) {
                    if (auto cc = reinterpret_cast<GAFWrapper*>(n))
                    {
                        cc->play(true);
                    }

                    }), DelayTime::create(1.25), NULL));

            }
            
            });
        unscheduleInActiveMode();
         inActiveMode();
    });
}

void MJGameWordTrain::onAnswerTrue(){
   
    stopListAudio();
    _handShowed = true;
    train->playSequence("hide");
    unscheduleInActiveMode();
    inActiveMode();
    m_speakable = false;
}

void MJGameWordTrain::onFinishTurn(){
    inActiveMode();
    auto item_img = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "item_img");
    if (item_img) {
        item_img->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=]{
            auto audio_index = AUDIO_ENGINE->playEffect("sounds/word_train/wordtrain_tauhoa.mp3");
            train->playSequence("run", true);
            auto duration = 2.0f/1024;
            auto w_road_train = Director::getInstance()->getWinSize().width;
            train->runAction(Sequence::create(MoveBy::create(duration*w_road_train, Vec2(-2*w_road_train, 0)), CallFunc::create([=]{
                object_index++;
                auto max_turn = min(4, (int)game_dataset.list_objects.size());
                if (object_index >= max_turn) {
                    AUDIO_ENGINE->stopEffect(audio_index);
                    onFinishGame();
                }else {
                    train->setPositionX(pos_train.x+w_road_train);
                    train->runAction(Sequence::create(MoveTo::create(duration*0.75*w_road_train, pos_train), CallFunc::create([=]{
                        AUDIO_ENGINE->stopEffect(audio_index);
                        if (game_play_enable) {
                            this->onStartGame();
                        }else {
                            train->pauseAnimation();
                            this->pauseGame();
                        }
                        
                     }), NULL));
                }
            }), NULL));
            item_img->removeFromParent();
            train->removeChildByName("wordtrain_item");
        }), NULL));
    }
}

void MJGameWordTrain::onTouchItemBegan(){
    unscheduleInActiveMode();
}

void MJGameWordTrain::onTouchItemEnded() {
    stopListAudio();
    static int countWrongTurn = 0;
    auto age = ap::ProfileManager::getInstance().getInformationOfProfile().age;
    if (++countWrongTurn > 2) {
        countWrongTurn = 0;
            if (!_handShowed && age >= CHILDREN_AGED) {
                showGuide();
            }
            _inactiveAudioPath = AUDIO_ENGINE->play(game_dataset.list_objects[object_index].audio_path, false, 1.0, [=]() {
                inActiveMode();
                });
    }
}

void MJGameWordTrain::stopAudioIndex(int index)
{
    if (index != -1) {
        AUDIO_ENGINE->stopEffect(index);
    }
}

void MJGameWordTrain::stopListAudio()
{
    std::vector<int> listAudio = { 
        _imageAudioPath ,
        _speakerAudioPath ,
        _imageAudioPath,
        _inactiveAudioPath
    };
    for (auto audioIndex : listAudio) {
        if (listAudio.size() > 0) {
            int audio = listAudio.back();
            listAudio.pop_back();
            stopAudioIndex(audio);
        }
    }
}

//void MJGameWordTrain::pauseGame(){
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

void MJGameWordTrain::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++) {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }
	auto img_n = math::resource::ResHelper::getImage(images, value_data["character"].GetString());
	auto character = img_n->getText();
	auto character_audio_path = img_n->getAudio();


	auto list_object = value_data["list_items"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		MJGameWordTrain_Item::ItemInfo object_info;
		auto img_nature = math::resource::ResHelper::getImage(images, list_object[i].GetString());

		object_info.text = img_nature->getText();
		object_info.audio_path = img_nature->getAudio();
		object_info.img_path = img_nature->path;
		object_info.character = character;
		object_info.character_audio_path = character_audio_path;

		game_dataset.list_objects.push_back(object_info);

	}
    
	auto list_distracting = value_data["list_distracting"].GetArray();
	for (int i = 0; i < (int)list_distracting.Size(); i++) {
		MJGameWordTrain_Item::ItemDistracting object_distracting;
		auto img_nature = math::resource::ResHelper::getImage(images, list_distracting[i].GetString());
        if(img_nature->getText().compare(character) == 0) continue;
		object_distracting.text = img_nature->getText();
		object_distracting.audio_path = img_nature->getAudio();
		game_dataset.list_distracting.push_back(object_distracting);
	}
}

void MJGameWordTrain::onFinishGame(){
    onExitTransitionDidStart();
	this->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([=] {
		commonEndGame(ms::BaseActivity::EGT_COMMON);
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
	}), NULL));
}
#pragma callback
ui::Widget::ccWidgetClickCallback MJGameWordTrain::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameWordTrain::onRefresh, this);
    }
    return nullptr;
}

void MJGameWordTrain::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = MJGameWordTrain::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void MJGameWordTrain::onExitTransitionDidStart(){
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
//    AUDIO_ENGINE->stopAllEffects();
}

void MJGameWordTrain::showGuide() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

    auto item = train->getChildByName<MJGameWordTrain_Item *>("wordtrain_item");
    if (!item) {
        return;
    }
    
    auto start = item->getGuideStartingPoint();
    auto stop = item->getGuideStoppingPoint();

    auto overlay = cocos2d::ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
    
    auto hand = cocos2d::ui::ImageView::create("mjstory/new_ui/hand.png");
    overlay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    
    hand->runAction(Sequence::create( MoveTo::create(1.5, stop), RemoveSelf::create(), nullptr));
}

void MJGameWordTrain::inActiveMode(){
    this->scheduleOnce([=](float) {
        auto age = ap::ProfileManager::getInstance().getInformationOfProfile().age;
        if (object_index < game_dataset.list_objects.size()) {
                if (!_handShowed && age >= CHILDREN_AGED) {
                    showGuide();
                }
                _inactiveAudioPath=AUDIO_ENGINE->play(game_dataset.list_objects[object_index].audio_path, false, 1.0, [=]() {
                    inActiveMode();
                    });
        }
        
    }, 10, "animated_guide");
}

void MJGameWordTrain::unscheduleInActiveMode() {
    unschedule("animated_guide");
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
}