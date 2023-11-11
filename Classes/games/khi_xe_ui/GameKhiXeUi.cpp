
#include "GameKhiXeUi.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GameKhiXeUi);
#define speed_opacity 15
USING_NS_CC;
std::once_flag gamekhixeui_reader;
#define CSB_NAME "csb/game/khi_xe_ui/GameKhiXeUi.csb"

GameKhiXeUi * GameKhiXeUi::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(gamekhixeui_reader, [] {
        REGISTER_CSB_READER(GameKhiXeUi);
        REGISTER_CSB_READER(GameKhiXeUi_Item);
    });
    auto p = static_cast<GameKhiXeUi *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameKhiXeUi::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/game_sample.plist");
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

void GameKhiXeUi::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
	auto list_object = value_data["content_info"]["data"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        GameKhiXeUi_Item::ItemInfo data_info;
        auto object_value = list_object[i].GetObject();
		data_info.text = object_value["text"].GetString();
		data_info.audio_path = object_value["audio_path"].GetString();
        game_dataset.list_objects.push_back(data_info);
    }
	game_dataset.maxNumItem = value_data["content_info"]["type_play"].GetInt();
	if (game_dataset.maxNumItem == 9 &&game_dataset.list_objects.size()<9 ){
		game_dataset.maxNumItem = 6;
		if (game_dataset.maxNumItem == 6 && game_dataset.list_objects.size() < 6){
			game_dataset.maxNumItem = 4;
		}
	}
	
}

void GameKhiXeUi::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	int typeGame = value_data["type_game"].GetInt();
	if (typeGame == 0) {
		auto img_intro = math::resource::ResHelper::getImage(images, value_data["audio_intro"].GetString());
		auto img_type = math::resource::ResHelper::getImage(images, value_data["audio_type"].GetString());
		game_dataset.audio_intro = img_intro->getAudio();
		game_dataset.intro = img_type->getAudio();
	}
	else{
        if (CONFIG_MANAGER->isGameVMonkey()){
            if (game_dataset.hoc_am_hoc_van == "hoc_am"){
                game_dataset.audio_intro = "";//StringUtils::format("sounds/khi_xe_ui/intro_vi_am.mp3",typeGame);
                game_dataset.intro = StringUtils::format("sounds/khi_xe_ui/find_vi_am.mp3", typeGame);
            }else{
                game_dataset.audio_intro = "";//StringUtils::format("sounds/khi_xe_ui/intro_vi_van.mp3",typeGame);
                game_dataset.intro = StringUtils::format("sounds/khi_xe_ui/find_vi_van.mp3", typeGame);
            }
            
        }else{
            game_dataset.audio_intro = StringUtils::format("sounds/khi_xe_ui/intro%d.mp3",typeGame);
            game_dataset.intro = StringUtils::format("sounds/khi_xe_ui/find%d.mp3", typeGame);
        }
	}
	

	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		GameKhiXeUi_Item::ItemInfo data_info;
		auto dataFull = math::resource::ResHelper::getImage(images, list_object[i]["text_info"].GetString());
		data_info.text = dataFull->getText();
		data_info.audio_path = dataFull->getAudio();
        auto pair = dataFull->getComparisionValue("pair");
        data_info.pair =  (pair == -1) ? 112197 + i : pair;
		game_dataset.list_objects.push_back(data_info);
	}

	game_dataset.maxNumItem = value_data["type_play"].GetInt();
    if (value_data.HasMember("option_vm_phonic")){
        game_dataset.hoc_am_hoc_van = value_data["option_vm_phonic"].GetString();
    }
}

void GameKhiXeUi::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();

	
}

void GameKhiXeUi::setInputConfigGame(){
//    auto limit = 3;
   // mj::helper::shuffle(game_dataset.list_objects);
	for (int i = 0; i< std::min((int)game_dataset.list_objects.size(),game_dataset.maxNumItem); i++){
		game_dataset.list_objects.push_back(game_dataset.list_objects.at(i));
	}
	while ((int)game_dataset.list_objects.size()>game_dataset.maxNumItem){
		game_dataset.list_objects.pop_back();
	}
	mj::helper::shuffle(game_dataset.list_objects);
	text_call = game_dataset.list_objects;
	mj::helper::shuffle(text_call);
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
    
}

void GameKhiXeUi::updateKhiXeUi(float dt)
{
	if (game_state==GAME_BASE_STATE::GBS_PAUSE)
		return;

	if (imgTextBuilding){
		auto posx1 = imgTextBuilding->getPositionX() - imgTextBuilding->getContentSize().width / 2;
		auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout");
		auto nenNha = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "imgNenNha");
		auto posx2 = objectLayout->convertToWorldSpace(nenNha->getPosition() + nenNha->getContentSize() / 2).x + 100;
		if (posx1<=posx2){
			imgTextBuilding->runAction(FadeOut::create(0.5f));
			imgTextBuilding = nullptr;
		}
	}
	if (game_state == GAME_BASE_STATE::GBS_PLAYING){
		game_playData.timeFromCall += dt;
	}
	if (game_playData.timeFromCall>=5){
		game_playData.timeFromCall = 0;
		game_playData.timeWrong++;
		callAText();
	}
}

void GameKhiXeUi::playMonkey(MONKEY_STATE ms, int posDestroy /*=0*/, const std::function<void()> aFunc/* = nullptr*/)
{
	std::string sequence_name = "idle";
	switch (ms)
	{
	case MONKEY_STATE::MS_IDLE:{
								  monkey->playSequence(sequence_name, true);
								  AUDIO_ENGINE->stopEffect(curAudio);
								  AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/idle.mp3", true);
								  return;
	}
	case MONKEY_STATE::MS_RUN:{
					 monkey->playSequence("run", true);
					 curAudio = AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/run.mp3", true);
					 return;
	}
	case MONKEY_STATE::MS_WRONG:{
					sequence_name = "wrong";
//                    AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/wrong.mp3");
					break;
	}
	case MONKEY_STATE::MS_DESTROY:{
									  AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/Smash_wall_correct.mp3");
									  if (house_item->house_type == HOUSE_TYPE::HT_SMALL){
										  sequence_name = StringUtils::format("%s%d_%d", "destroy_", posDestroy, posDestroy);
									  }
									  else{
										  sequence_name = StringUtils::format("%s%d", "destroy_", posDestroy);
									  }
					 break;
	}
	default:
		break;
	}
	monkey->playSequence(sequence_name, false, [=](gaf::GAFObject *obj, std::string name_sequence){
		playMonkey(MONKEY_STATE::MS_IDLE);
		if (ms == MONKEY_STATE::MS_DESTROY)	{
			if (aFunc){
				aFunc();
			}
		}
	});
}

void GameKhiXeUi::callAText()
{

	game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
	if (!_speakable)
	{
		return;
	}
	_speakable = false;
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.intro);

    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		if (game_playData.timeWrong >= 2){
			for (int i = 0; i < (int)house_item->_item_infos.size(); i++) {
				if (_click_wrongItem)
				{
					_click_wrongItem = false;
					break;
				}
				if (house_item->_item_infos.at(i).text.compare(text_call.at(curText).text) == 0 && house_item->_item_infos.at(i).imgCrash == nullptr){
					house_item->_item_infos.at(i).aLayout->runAction(Sequence::create(
						FadeIn::create(0.2f)
						, FadeOut::create(0.2f)
						, FadeIn::create(0.2f)
						, FadeOut::create(0.2f)
						, nullptr));

					auto correctPos = house_item->_item_infos.at(i).textItem->getPosition();
						
					showDragGuideGame(correctPos, correctPos, 0, 0.1f, 1.0f, "gaf/guidegame/hand_tap/hand_tap.gaf", house_item->_item_infos.at(i).aLayout);
					break;
				}
			}
		}

        auto audio_text_path = text_call.at(curText).audio_path;
        auto audio_text_id = cocos2d::experimental::AudioEngine::play2d(audio_text_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_text_id, [=](int au_id, std::string path){
			game_state = GAME_BASE_STATE::GBS_PLAYING;
			_speakable = true;

        });
    });
}

void GameKhiXeUi::crashATrueItem()
{
	curText++;
	if (curText==2 && !game_play_enable){ //&& cai bien gi do o day
		pauseGame();
		return;
	}
	
	if (curText == (int)house_item->_item_infos.size()){
		//onFinishGame();
		onFinishLevel();
	}
	else{
		callAText();
	}
}

void GameKhiXeUi::loadFrames() {
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/khi_xe_ui/music.mp3", 0.5f);
    }
	monkey = GAFWrapper::createGAF("gaf/khi_xe_ui/xe_ui/Xe_ui.gaf");
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "layout_touch")->addChild(monkey);
	auto nenNha = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "imgNenNha");
	auto pos_1 = Vec2(nenNha->getPositionX() + nenNha->getContentSize().width / 2 + 40, Director::getInstance()->getVisibleSize().height / 2);
	monkey->setPosition(pos_1);
	playMonkey(MONKEY_STATE::MS_RUN);
	//monkey->playSequence("run", true);

	if (game_dataset.maxNumItem==6)
		house_gaf = GAFWrapper::createGAF("gaf/khi_xe_ui/nha_no6/nha_no6.gaf");
	else
		house_gaf = GAFWrapper::createGAF("gaf/khi_xe_ui/nha_no/nha_no.gaf");
	nenNha->addChild(house_gaf);
	house_gaf->setPositionX(nenNha->getContentSize().width / 2 +8);
	house_gaf->setPositionY(385);
	house_gaf->setVisible(false);
	//house_gaf->setPosition(Vec2(nenNha->getPositionX(),));
	//monkey->playSequence("run", true);

	imgTextBuilding = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "imgTextBuilding");
	imgTextBuilding->setLocalZOrder(10000);

	auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout");
	auto bg = objectLayout->getChildByName<cocos2d::ui::ImageView*>("bg");

	objectLayout->setPosition(Vec2(-(bg->getContentSize().width - 30 - Director::getInstance()->getVisibleSize().width),0));
	_speakable = false;
	_speakerBtn = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speakerBtn->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	_speakerBtn->setVisible(_speakable);
	_speakerBtn->addClickEventListener([=](cocos2d::Ref* sender) {

		auto audio_text_path = text_call.at(curText).audio_path;
		//auto audio_text_id = cocos2d::experimental::AudioEngine::play2d(audio_text_path);
		PlayChooseItem(NULL, audio_text_path);
		});
	this->addChild(_speakerBtn);


	onLoadLevel();
	onPreGame();
}

void GameKhiXeUi::onPreGame()
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        auto game_name_img = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "imgTextBuilding");
        game_name_img->setVisible(false);
        auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout");
        objectLayout->runAction(Sequence::create(MoveTo::create(3.f, Vec2(0, 0))
            , CallFunc::create([=](){
            playMonkey(MONKEY_STATE::MS_IDLE);
            
            auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d(game_dataset.hoc_am_hoc_van == "hoc_am"?"sounds/khi_xe_ui/intro_vi_am.mp3":"sounds/khi_xe_ui/intro_vi_van.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
                AUDIO_ENGINE->playEffect(game_dataset.audio_intro);
            });
            
            this->runAction(Sequence::create(DelayTime::create(3.2f), CallFunc::create([=](){
                onStartGame();
            }), nullptr));
        }), nullptr));

        

        this->schedule(schedule_selector(GameKhiXeUi::updateKhiXeUi), 0);
    }else{
        auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/khi_xe_ui/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout");
            objectLayout->runAction(Sequence::create(MoveTo::create(5.5f, Vec2(0, 0))
                , CallFunc::create([=](){
                playMonkey(MONKEY_STATE::MS_IDLE);
                //auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d("sounds/khi_xe_ui/intro.mp3");
                //cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
                //});
                //
				//AUDIO_ENGINE->playEffect(game_dataset.audio_intro); //se them lai sau khi config intro tren v2
                this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
                    onStartGame();
                }), nullptr));
            }), nullptr));

            this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){
                AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/khi_xe_ui_running_sound.mp3");
            }), nullptr));

            this->schedule(schedule_selector(GameKhiXeUi::updateKhiXeUi), 0);
        });
    }

}

void GameKhiXeUi::onStartGame(){
	_speakable = true;
	_speakerBtn->setVisible(_speakable);
	for (int i = 0; i < (int)house_item->_item_infos.size(); i++){
		house_item->_item_infos.at(i).textItem->runAction(FadeIn::create(0.5f));
	}
	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
        callAText();
		doAutoPlay();
//        auto audio_text_path = text_call.at(curText).audio_path;
//        cocos2d::experimental::AudioEngine::play2d(audio_text_path);
	}), nullptr));
}

void GameKhiXeUi::onFinishGame(){
  

	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
        if (CONFIG_MANAGER->isGameVMonkey()){
            this->onCloseGameVmonkey();
        }else{
            _activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        }
		
	}), NULL));
}

void GameKhiXeUi::onFinishLevel()
{
	auto sequence_name = "house_1";
	if (house_item->house_type == HOUSE_TYPE::HT_SMALL){
		sequence_name = "house_2";
	}
	house_gaf->setVisible(true);
	house_item->mainLayout->setVisible(false);
	house_gaf->playSequence(sequence_name, false, [=](gaf::GAFObject *obj, std::string name_sequence){
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
			onFinishGame();
		}), nullptr));
	});
	Sequence* aSeq = nullptr;
	AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/khi_xe_ui_true.mp3");
	AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/win_end_game.mp3");
	AUDIO_ENGINE->playEffect("sounds/khi_xe_ui/Building_destroy_win_stage.mp3");

	if (house_item->house_type == HOUSE_TYPE::HT_BIG){
		aSeq = Sequence::create(DelayTime::create(0.15f), CallFunc::create([=](){
			house_item->_item_infos.at(0).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(1).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(2).imgCrash->runAction(FadeOut::create(0.15f));
		})
			, DelayTime::create(0.35f), CallFunc::create([=](){
			house_item->_item_infos.at(3).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(4).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(5).imgCrash->runAction(FadeOut::create(0.15f));
		})
			, DelayTime::create(0.35f), CallFunc::create([=](){
			house_item->_item_infos.at(6).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(7).imgCrash->runAction(FadeOut::create(0.15f));
			house_item->_item_infos.at(8).imgCrash->runAction(FadeOut::create(0.15f));
		}), nullptr);
	}
	else if (house_item->house_type == HOUSE_TYPE::HT_MEDIUM){
		aSeq = Sequence::create(DelayTime::create(0.2f), CallFunc::create([=](){
			house_item->_item_infos.at(0).imgCrash->runAction(FadeOut::create(0.2f));
			house_item->_item_infos.at(1).imgCrash->runAction(FadeOut::create(0.2f));
			house_item->_item_infos.at(2).imgCrash->runAction(FadeOut::create(0.2f));
		})
			, DelayTime::create(0.45f), CallFunc::create([=](){
			house_item->_item_infos.at(3).imgCrash->runAction(FadeOut::create(0.4f));
			house_item->_item_infos.at(4).imgCrash->runAction(FadeOut::create(0.4f));
			house_item->_item_infos.at(5).imgCrash->runAction(FadeOut::create(0.4f));
		}), nullptr);
	}
	else {
		aSeq = Sequence::create(DelayTime::create(0.2f), CallFunc::create([=](){
			house_item->_item_infos.at(0).imgCrash->runAction(FadeOut::create(0.2f));
			house_item->_item_infos.at(1).imgCrash->runAction(FadeOut::create(0.2f));
			//house_item->_item_infos.at(2).imgCrash->runAction(FadeOut::create(0.2f));
		})
			, DelayTime::create(0.45f), CallFunc::create([=](){
			house_item->_item_infos.at(2).imgCrash->runAction(FadeOut::create(0.4f));
			house_item->_item_infos.at(3).imgCrash->runAction(FadeOut::create(0.4f));
			//house_item->_item_infos.at(5).imgCrash->runAction(FadeOut::create(0.4f));
		}), nullptr);
	}
	this->runAction(aSeq);
}

void GameKhiXeUi::onLoadLevel()
{
	auto housePanel = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "housePanel");
	auto imgNenNha = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "imgNenNha");

	house_item = GameKhiXeUi_Item::createGame(game_dataset.list_objects);
	house_item->setPosition(imgNenNha->getPosition() + Vec2(0,imgNenNha->getContentSize().height/2));
	housePanel->addChild(house_item);
	for (int i = 0; i < (int)house_item->_item_infos.size(); i++){
		house_item->_item_infos.at(i).aLayout->setTag(i);
		house_item->_item_infos.at(i).aLayout->setTouchEnabled(true);
		house_item->_item_infos.at(i).aLayout->addClickEventListener(CC_CALLBACK_1(GameKhiXeUi::onItemClicked, this));

	}
}

#pragma callback

void GameKhiXeUi::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = GameKhiXeUi::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameKhiXeUi::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void GameKhiXeUi::PlayChooseItem(Item* chooseItem, std::string audio_path) {
	if (chooseItem != NULL)
	{
		chooseItem->setGlobalZOrder(0);
		chooseItem->runAction(ScaleTo::create(0.2, 1.2));
	}
	//AUDIO_ENGINE->playSound(item_choose->getAudio());
	if (_speakable == false || audio_path == "")
		return;
	_speakable = false;
	AUDIO_ENGINE->playSound(audio_path, false, [=]() {
		_speakable = true;
		if (chooseItem != NULL)
		{
			chooseItem->runAction(ScaleTo::create(0.2, 1));
		}
		});
}
void GameKhiXeUi::showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop, float delay_time, float dt, std::string gaf_guide_path, cocos2d::ui::Layout* parentLayout) {
	auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check)
	{
		guide_layout_check->removeFromParent();
	}
	cocos2d::Size screen_size = cocos2d::Director::getInstance()->getWinSize();
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	if (parentLayout == NULL)
		Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX);
	else
	{
		parentLayout->addChild(guide_layout, parentLayout->getZOrder());
	}
	//Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		auto hand = GAFWrapper::createGAF(gaf_guide_path);
		guide_layout->addChild(hand, 1900000000);
		hand->setPosition(posStart);
		hand->play(false, [=](gaf::GAFObject* obj) {
			hand->runAction(Sequence::create(MoveTo::create(0.6f, posEnd),
				CallFunc::create([=]() {
					hand->play(false, [=](gaf::GAFObject* obj) {
						hand->removeFromParent();
						});
					}), NULL));
			});
		}, dt, loop, delay_time, "animated_guide");
}
void GameKhiXeUi::onItemClicked(Ref* sender) {
	auto housePanel = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "housePanel");
	auto imgNenNha = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "imgNenNha");
	auto thisItem = static_cast<cocos2d::ui::Layout*>(sender);
	int i = thisItem->getTag();
	if (game_state == GAME_BASE_STATE::GBS_PLAYING) {
		if (house_item->_item_infos.at(i).isCrash)//dap roi thi ko lam gi nua
			return;
		if (house_item->_item_infos.at(i).text.compare(text_call.at(curText).text) == 0 ||
			house_item->_item_infos.at(i).pair == text_call.at(curText).pair) {
			_click_wrongItem = false;
			//chon dung thi dien action
			game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
			game_playData.timeFromCall = 0;
			game_playData.timeWrong = 0;
			house_item->_item_infos.at(i).isCrash = true;
			house_item->_item_infos.at(i).aLayout->runAction(Sequence::create(
				CallFunc::create([=]() {
					housePanel->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(-3, 0))
						, MoveTo::create(0.2f, Vec2(2, 0))
						, MoveTo::create(0.2f, Vec2(-2, 0))
						, MoveTo::create(0.2f, Vec2(1, 0))
						, MoveTo::create(0.2f, Vec2(0, 0))
						, nullptr));
					//house_item->playBoom(i);
					string breakAudioPath = "sounds/khi_xe_ui/break_item_audio.mp3";
					if (cocos2d::FileUtils::getInstance()->isFileExist(breakAudioPath))
						AUDIO_ENGINE->playEffect(breakAudioPath);

					house_item->_item_infos.at(i).textItem->setVisible(false);
					string img_break = "games/khi_xe_ui/vet_nut.png";
					if (cocos2d::FileUtils::getInstance()->isFileExist(img_break)) {
						cocos2d::ui::ImageView* crashItem = cocos2d::ui::ImageView::create(img_break);
						crashItem->setPosition(house_item->_item_infos.at(i).aLayout->getPosition() + house_item->_item_infos.at(i).aLayout->getContentSize() / 2 + cocos2d::Size(5, 0));
						house_item->_item_infos.at(i).imgCrash = crashItem;
						house_item->boomPanel->addChild(crashItem);
					}
					}),
				DelayTime::create(1.6f)
						, CallFunc::create([=]() {
						housePanel->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(-3, 0))
							, MoveTo::create(0.2f, Vec2(2, 0))
							, MoveTo::create(0.2f, Vec2(-2, 0))
							, MoveTo::create(0.2f, Vec2(1, 0))
							, MoveTo::create(0.2f, Vec2(0, 0))
							, nullptr));
						house_item->playBoom(i);
						house_item->_item_infos.at(i).textItem->setVisible(false);
						cocos2d::ui::ImageView* crashItem = house_item->_item_infos.at(i).imgCrash;
						if (crashItem)
						{
							crashItem->removeFromParent();
							crashItem = nullptr;
						}
						crashItem = cocos2d::ui::ImageView::create(house_item->img_crash_path);
						crashItem->setPosition(house_item->_item_infos.at(i).aLayout->getPosition() + house_item->_item_infos.at(i).aLayout->getContentSize() / 2 + cocos2d::Size(5, 0));
						house_item->_item_infos.at(i).imgCrash = crashItem;
						house_item->boomPanel->addChild(crashItem);
							}), nullptr));
			if (game_dataset.maxNumItem == 6) {
				playMonkey(MONKEY_STATE::MS_DESTROY, i + 4, [=]() {
					crashATrueItem();
					});
			}
			else {
				playMonkey(MONKEY_STATE::MS_DESTROY, i + 1, [=]() {
					crashATrueItem();
					});
			}

		}
		else {
			//chon sai
			game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
			game_playData.timeWrong++;
			playMonkey(MONKEY_STATE::MS_WRONG);
			if (!CONFIG_MANAGER->isGameVMonkey()) {
				house_item->_item_infos.at(i).aLayout->runAction(
					Sequence::create(
						RotateTo::create(0.1f, 5.0f)
						, RotateTo::create(0.1f, -5.0f)
						, RotateTo::create(0.1f, 5.0f)
						, RotateTo::create(0.1f, -5.0f)
						, RotateTo::create(0.1f, 5.0f)
						, RotateTo::create(0.1f, -5.0f)
						, RotateTo::create(0.1f, 5.0f)
						, RotateTo::create(0.1f, 0.0f)
						, nullptr));
			}
			auto audio_path = std::string("sounds/khi_xe_ui/wrong.mp3");
			auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);

			cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
				if (CONFIG_MANAGER->isGameVMonkey()) {
					auto audio_text_path = house_item->_item_infos.at(i).audio_path;
					auto audio_text_id = cocos2d::experimental::AudioEngine::play2d(audio_text_path);
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_text_id, [=](int au_id, std::string path) {
						callAText();
						});
				}
				else
				{
					_click_wrongItem = true;
					auto audio_path = std::string("sounds/khi_xe_ui/khi_xe_ui_wrong.mp3");
					auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
						auto audio_text_path = house_item->_item_infos.at(i).audio_path;
						auto audio_text_id = cocos2d::experimental::AudioEngine::play2d(audio_text_path);
						cocos2d::experimental::AudioEngine::setFinishCallback(audio_text_id, [=](int au_id, std::string path) {
							callAText();
							});
						});
				}

				});
		}
	}
}

void GameKhiXeUi::doAutoPlay()
{
	if (CONFIG_MANAGER->isAutoPlay()) {
		int time = 0;
		for (auto i = 0; i < text_call.size(); i++) {
			this->scheduleOnce([=](float) {
				for (auto item : house_item->_item_infos) {
					if (item.text.compare(text_call.at(i).text) == 0 || item.pair == text_call.at(i).pair) {
						onItemClicked(item.aLayout);
						break;
						}
				}
			},i* 8 + 2, "do_action" + std::to_string(i));
		}
	}
}
