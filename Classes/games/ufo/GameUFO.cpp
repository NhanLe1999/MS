
#include "GameUFO.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GameUFO);
#define speed_opacity 15

std::once_flag GameUFO_reader;
#define CSB_NAME "csb/game/ufo/GameUFO.csb"

GameUFO * GameUFO::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameUFO_reader, [] {
        REGISTER_CSB_READER(GameUFO);
        REGISTER_CSB_READER(GameUFO_Item);
    });
    auto p = static_cast<GameUFO *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameUFO::didLoadFromCSB(std::vector<math::resource::Image*> images) {

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
	readConfig(value_data.GetObject(),images);
}

void GameUFO::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	
	game_dataset.number_level = value_data["content_info"]["number_level"].GetInt();
	auto list_object = value_data["content_info"]["data"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
		GameUFO_Item::ItemInfo word_info;
        auto object_value = list_object[i].GetObject();
		word_info.img_path = object_value["img_path"].GetString();
		word_info.audio_path = object_value["audio_path"].GetString();
		word_info.text = object_value["w"].GetString();
		word_info.description_path = object_value["audio_description"].GetString();
		game_dataset.list_objects.push_back(word_info);
		game_dataset.list_distractings.push_back(word_info.text);
    }

	if (value_data["content_info"].HasMember("list_distracting")){
		auto list_dictracting = value_data["content_info"]["list_distracting"].GetArray();
		for (int i = 0; i < (int)list_dictracting.Size(); i++){
			game_dataset.list_distractings.push_back(list_dictracting[i].GetString());
		}
	}
}

void GameUFO::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		GameUFO_Item::ItemInfo word_info;
		auto object_value = list_object[i].GetObject();
		auto dataFull = math::resource::ResHelper::getImage(images, object_value["text_data"].GetString());
		
		word_info.isHasImage = object_value["isHasImage"].GetBool();
		word_info.img_path = dataFull->path;
		word_info.audio_path = dataFull->getAudios("name_1").front();
		word_info.text = dataFull->getText("name_1");

		auto dataDes = math::resource::ResHelper::getImage(images, object_value["description_data"].GetString());
		word_info.description_path = dataDes->getAudios("name_1").front();
		game_dataset.list_objects.push_back(word_info);
		game_dataset.list_distractings.push_back(word_info.text);
	}

	if (value_data.HasMember("list_distracting")){
		auto list_dictracting = value_data["list_distracting"].GetArray();
		for (int i = 0; i < (int)list_dictracting.Size(); i++){
			game_dataset.list_distractings.push_back(list_dictracting[i].GetString());
		}
	}
}

void GameUFO::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GameUFO::setInputConfigGame(){
//    auto limit = 3;
	do 
	{
		mj::helper::shuffle(game_dataset.list_objects);
	} while (game_dataset.list_objects.at(0).img_path=="");
   
	
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
	for (int i = 0; i < game_dataset.number_level; i++){
		list_words.push_back(game_dataset.list_objects.at(i));
		//list_words.push_back(game_dataset.list_objects.at(i));
    }
	mj::helper::shuffle(list_words);

}

void GameUFO::playUfo(UFO_GAF_STATE ufo_state_)
{
	timeNoTap = 0;
	if (ufo_state_!=UFO_GAF_STATE_IDLE)
		this->ufo_state = ufo_state_;
	auto objectPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel");
	switch (ufo_state_)
	{
	case UFO_GAF_STATE_INTRO:{
								 ufo->playSequence("intro", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									 onStartGame();
								 });
								 break;
	}
	case UFO_GAF_STATE_PHASE1:{
								  ufo->playSequence("phase1", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									  //playUfo(UFO_GAF_STATE_PHASE1);
								  });
								  GameUFO_Item* aItem = GameUFO_Item::createItem(game_dataset.list_objects.at(curText).text);
								  objectPanel->addChild(aItem);
								  auto pos  = Vec2(Director::getInstance()->getVisibleSize().width / 2, 120);
								  aItem->setPosition(pos + Vec2(0, 150));
								  aItem->setVisible(false);
								  aItem->setScaleX(0.05f);
								  aItem->runAction(Sequence::create(DelayTime::create(0.3f)
									  , CallFunc::create([=](){aItem->setVisible(true); })
									  , MoveTo::create(0.15f, pos)
									  , CallFunc::create([=](){AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).audio_path); })
									  , ScaleTo::create(0.3f, 1.0f), nullptr));

								  auto img = ImageView::create(game_dataset.list_objects.at(curText).img_path);
								 // img->setVisible(false);
								  objectPanel->addChild(img);
								  img->setScale(0.01f);
								  auto pos2 = Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height*0.62f) + img->getContentSize() / 2;
								  img->setPosition(pos2);
								  img->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);

								  img->runAction(Sequence::create(
									  DelayTime::create(1.5f)
									  , CallFunc::create([=](){
									  img->setVisible(true);
								  }), ScaleTo::create(0.1f, 1.2f, 0.8f)
									  , ScaleTo::create(0.2f, 0.95f, 1.1f)
									  , ScaleTo::create(0.2f, 1.05f, 0.95f)
									  , ScaleTo::create(0.2f, 1.0f)
									  , CallFunc::create([=](){
									  auto audio_path = std::string(game_dataset.list_objects.at(curText).description_path);
									  auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
									  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
										  curText++;
										  if (curText==game_dataset.number_level){
											  curText = 0;
											  phase = 2;
										  }
										  onStartGame();
									  });
									  AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).description_path); 
								  }), nullptr));
								  break;
	}
	case UFO_GAF_STATE_PHASE4:{
								  AUDIO_ENGINE->playEffect("sounds/ufo/phase_4.mp3");
								  ufo->playSequence("phase_4", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									  auto audio_path = std::string(game_dataset.list_objects.at(curText).description_path);
									  auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
									  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
										  curText++;
										  if (curText == game_dataset.number_level){
											  curText = 0;
											  phase = 2;
										  }
										  onStartGame();
									  });
									  //AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).description_path);
								  });
								  GameUFO_Item* aItem = GameUFO_Item::createItem(game_dataset.list_objects.at(curText).text);
								  objectPanel->addChild(aItem);
								  Vec2 pos = Director::getInstance()->getVisibleSize() / 2 + cocos2d::Size(0,-50);
								  aItem->setPosition(pos + Vec2(0, 150));
								  aItem->setVisible(false);
								  aItem->setScaleX(0.05f);
								  aItem->runAction(Sequence::create(DelayTime::create(1.3f)
									  , CallFunc::create([=](){aItem->setVisible(true); })
									  , MoveTo::create(0.15f, pos)
									  , CallFunc::create([=](){AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).audio_path); })
									  , ScaleTo::create(0.3f, 1.0f), nullptr));
								  break;
	}
	case UFO_GAF_STATE_PHASE5:{
								  AUDIO_ENGINE->playEffect("sounds/ufo/phase_5.mp3");
								  ufo->playSequence("phase_5", false);// , [=](gaf::GAFObject *obj, const std::string seq_name){
								  this->runAction(Sequence::create(DelayTime::create(1.66f), CallFunc::create([=](){
									  auto audio_path = std::string(game_dataset.list_objects.at(curText).description_path);
									  auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
									  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
										  curText++;
										  if (curText == game_dataset.number_level){
											  curText = 0;
											  phase = 2;
										  }
										  onStartGame();
									  });
								  }), nullptr));
									 
									  //AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).description_path);
								//  });
								  GameUFO_Item* aItem = GameUFO_Item::createItem(game_dataset.list_objects.at(curText).text);
								  objectPanel->addChild(aItem);
								  Vec2 pos = Director::getInstance()->getVisibleSize() / 2 + cocos2d::Size(0, -50);
								  aItem->setPosition(pos + Vec2(0, 150));
								  aItem->setVisible(false);
								  aItem->setScaleX(0.05f);
								  aItem->runAction(Sequence::create(DelayTime::create(0.33f)
									  , CallFunc::create([=](){aItem->setVisible(true); })
									  , MoveTo::create(0.15f, pos)
									  , CallFunc::create([=](){AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(curText).audio_path); })
									  , ScaleTo::create(0.3f, 1.0f), nullptr));
								  break;
	}
	case UFO_GAF_STATE_PHASE2:{
								  this->scheduleOnce([=](float dt){
									  AUDIO_ENGINE->playEffect("sounds/ufo/UFO_showText_secondtime.mp3");
								  }, 0.6f, "sound_2nd_time");
								  ufo->playSequence("phase2", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									  playUfo(UFO_GAF_STATE_PHASE1);
								  });
								  break; }
	case UFO_GAF_STATE_PHASE3:{
								  audio_id = AUDIO_ENGINE->playEffect("sounds/ufo/UFO_ShootLaser1.mp3");
								  ufo->playSequence("phase3", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									  playUfo(UFO_GAF_STATE_SHOT_LAZER);
								  });
								  break; }
	case UFO_GAF_STATE_SHOT_LAZER:{
								  _speaker->setVisible(true);
								  _speaker->setTouchEnabled(false);
								  _speaker->addClickEventListener([=](Ref*) {
									  flag = false;
									  timeNoTap = 0;
									  this->unschedule("showTutorial");
									  _speaker->setTouchEnabled(false);
									  auto audio_path = StringUtils::format("sounds/ufo/Tap_on%d.mp3", random(1, 4));
									  AUDIO_ENGINE->playSound(audio_path, false, [=]() {
										  auto audio_path_text = std::string(list_words.at(curText).audio_path);
										  AUDIO_ENGINE->playSound(audio_path_text, false, [=]() {
											  game_state = GAME_BASE_STATE::GBS_PLAYING;
											  _speaker->setTouchEnabled(true);
											  flag = true;
											  timeNoTap = 0;
											  }, 1.0f);
										  }, 1.0f);
									  });
								  mj::helper::shuffle(game_dataset.list_distractings);
								  std::vector<std::string> this_texts;
								  this_texts.push_back(list_words.at(curText).text);
								  for (int i = 0; i < (int)game_dataset.list_distractings.size(); i++){
									  if (std::find(this_texts.begin(),this_texts.end(),game_dataset.list_distractings.at(i))==this_texts.end()){
										  this_texts.push_back(game_dataset.list_distractings.at(i));
									  }
									  if (this_texts.size() == 3)
										  break;
								  }
								  mj::helper::shuffle(this_texts);
									
								  AUDIO_ENGINE->stopEffect(audio_id);
								  AUDIO_ENGINE->playEffect("sounds/ufo/UFO_ShootLaser2.mp3");
								  ufo->playSequence("shot_laser", false, [=](gaf::GAFObject *obj, const std::string seq_name){
									 
								  });
								  
								  this->scheduleOnce([=](float dt){
									  CCLOG("vao day ne");
									  auto audio_path = StringUtils::format("sounds/ufo/Tap_on%d.mp3", random(1,4));
									  auto audio_id1 = cocos2d::experimental::AudioEngine::play2d(audio_path);
									  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id1, [=](int au_id, std::string path){
										  auto audio_path_text = std::string(list_words.at(curText).audio_path);
										  auto audio_id_text = cocos2d::experimental::AudioEngine::play2d(audio_path_text);
										  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_text, [=](int au_id, std::string path){
											  game_state = GAME_BASE_STATE::GBS_PLAYING;
											  timeNoTap = 0;
											  _speaker->setTouchEnabled(true);
										  });
									  });
									  playUfo(UFO_GAF_STATE_IDLE);
								  }, 1.2f, "ban_lazer");
								  timeNoTap = 0;
								  listItems.clear();
								  int maxFontSize = 100;
								  std::vector<GameUFO_Item*> list_as;
								  for (int i = 0; i < 3; i++) {
									  GameUFO_Item* aItem = GameUFO_Item::createItem(this_texts.at(i));
									  objectPanel->addChild(aItem);
									  if (aItem->getFontSize() < maxFontSize)
										  maxFontSize = aItem->getFontSize();
									  list_as.push_back(aItem);
									  Vec2 pos = Director::getInstance()->getVisibleSize() / 2 + cocos2d::Size(-190, +205 - i * 205);
									  aItem->setPosition(pos);
									  aItem->setOpacity(0);
									  aItem->runAction(Sequence::create(DelayTime::create(i*0.33f), FadeIn::create(0.33f),nullptr));
									  aItem->addClickEventListener([=](Ref* sender){
										  
										  if (game_state != GAME_BASE_STATE::GBS_PLAYING || !flag)
											  return;
										  if (curText>0 && !game_play_enable){
											  pauseGame();
											  return;
										  }
										  timeNoTap = 0;
										  if (aItem->text.compare(list_words.at(curText).text) == 0){// chon dung
											  this->unschedule("showTutorial");
											  
											  _speaker->setTouchEnabled(false);
											  AUDIO_ENGINE->playEffect("sounds/ufo/correct.mp3");
											  game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
											  playUfo(UFO_GAF_STATE_WIN);
											  auto audio_path_text = StringUtils::format("sounds/ufo/awesome%d.mp3", random(1,4));
											  auto audio_id_text = cocos2d::experimental::AudioEngine::play2d(audio_path_text);
											  cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_text, [=](int au_id, std::string path){
												  curText++;
												  if (curText < list_words.size()) {
													  onStartGame();
												  }
												  else {
													  _speaker->setVisible(false);
													  onFinishLevel();
												  }
											  });
											  aItem->runAction(Sequence::create(
												  TintTo::create(0.1f, Color3B::GREEN), TintTo::create(0.1f, Color3B::WHITE)
												  , TintTo::create(0.1f, Color3B::GREEN), TintTo::create(0.1f, Color3B::WHITE)
												  , TintTo::create(0.1f, Color3B::GREEN), TintTo::create(0.1f, Color3B::WHITE)
												  , CallFunc::create([=](){
												  //game_state = UFO_GAME_STATE::UGS_PLAYING;
											  }), nullptr));
										  }
										  else{
											  //choose wrong
											  
											  this->unschedule("showTutorial");
											  showTutorial(5.0f);
											  AUDIO_ENGINE->playEffect("sounds/ufo/wrong.mp3");
											  game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
											  aItem->runAction(Sequence::create(
												  TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE)
												  , TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE)
												  , TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE)
												  , CallFunc::create([=](){
												  game_state = GAME_BASE_STATE::GBS_PLAYING;
											  }), nullptr));
										  }
									  });
									  listItems.push_back(aItem);
								  }
								  for (int i = 0; i < 3; i++) {
									  list_as.at(i)->setFontSize(maxFontSize);
								  }
								  break; }
	case UFO_GAF_STATE_IDLE:{
								ufo->playSequence("idle", true);
								break; }
	case UFO_GAF_STATE_WIN:{
							   ufo->playSequence("win", false, [=](gaf::GAFObject *obj, const std::string seq_name){
								   playUfo(UFO_GAF_STATE_IDLE);
							   });
							   break; }
	case UFO_GAF_STATE_END:{
							   AUDIO_ENGINE->stopAllEffects();
							   AUDIO_ENGINE->playEffect("sounds/ufo/End_intro.mp3");
							   this->scheduleOnce([=](float dt){
								   AUDIO_ENGINE->playEffect("sounds/ufo/time_to_leave.mp3");
							   }, 0.57f, "sound_end");
							   this->scheduleOnce([=](float dt){
								   auto bg = cocos2d::utils::findChild<ImageView*>(this, "bg");
								   auto bg1 = cocos2d::utils::findChild<ImageView*>(this, "bg1");

								   bg->setVisible(false);
								   bg->runAction(Sequence::create(ScaleTo::create(1.33f, 1.5f*bg->getScale()), CallFunc::create([=](){bg->setVisible(true); }), nullptr));
								   bg1->runAction(ScaleTo::create(1.33f, 1.5f*bg1->getScale()));
							   }, 5.0f, "zoom_bg");
							   ufo->playSequence("end", false, [=](gaf::GAFObject *obj, const std::string seq_name){
								   onFinishGame();
							   });
							   break; }
	default:
		break;
	}
}

void GameUFO::updateTimeNoTap(float dt)
{
//    CCLOG("update");
	if (ufo_state == UFO_GAF_STATE_SHOT_LAZER){
		timeNoTap += dt;

		if (timeNoTap>10){
			timeNoTap = 0;
			_speaker->setTouchEnabled(false);
            auto audio_path = StringUtils::format("sounds/ufo/Tap_on%d.mp3", random(1,4));
			AUDIO_ENGINE->playSound(audio_path, false, [=]() {
				auto audio_path_text = std::string(list_words.at(curText).audio_path);
				AUDIO_ENGINE->playSound(audio_path_text, false, [=]() {
					game_state = GAME_BASE_STATE::GBS_PLAYING;
					timeNoTap = 0;
					_speaker->setTouchEnabled(true);
					}, 1.0f);
				}, 1.0f);
			//auto audio_id1 = cocos2d::experimental::AudioEngine::play2d(audio_path);
			/*cocos2d::experimental::AudioEngine::setFinishCallback(audio_id1, [=](int au_id, std::string path){
				auto audio_path_text = std::string(list_words.at(curText).audio_path);
				auto audio_id_text = cocos2d::experimental::AudioEngine::play2d(audio_path_text);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_text, [=](int au_id, std::string path){
					game_state = GAME_BASE_STATE::GBS_PLAYING;
					timeNoTap = 0;
					_speaker->setTouchEnabled(true);
				});
			});*/
		}
	}
}


void GameUFO::loadFrames() {	
	//load anim con khi o day
	flag = true;
	auto bg = cocos2d::utils::findChild<ImageView*>(this, "bg");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
		auto bg1 = cocos2d::utils::findChild<ImageView*>(this, "bg1");
		bg1->setScale(bg->getScale());
	}

	ufo = GAFWrapper::createGAF("gaf/ufo/ufo.gaf");
	this->addChild(ufo,100);
	//ufo->setPosition(Director::getInstance()->getVisibleSize()/2);
	ufo->setPositionX(Director::getInstance()->getVisibleSize().width/2 + +605);
	ufo->setPositionY(Director::getInstance()->getVisibleSize().height/6);
	
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	_speaker->setVisible(false);
	/*auto img1 = ImageView::create("games/ufo/ufo_item.png");
	img1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 120));
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel")->addChild(img1);

	auto img2 = ImageView::create("ufo/1.png");
	img2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height*0.62f));
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel")->addChild(img2);*/


	onPreGame();
}

void GameUFO::onPreGame()
{
	auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/ufo/gameName.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		this->scheduleOnce([=](float dt){
            if(MJDEFAULT->getBoolForKey(key_background_music)){
                AUDIO_ENGINE->playBackgroundMusic("sounds/ufo/music_04.mp3", 0.5f);
            }
			AUDIO_ENGINE->playEffect("sounds/ufo/UFO_intro.mp3");
			auto sprGameName = cocos2d::utils::findChild<Sprite*>(this, "sprGameName");
			sprGameName->runAction(Sequence::create(FadeOut::create(0.2f)
				, CallFunc::create([=](){
				AUDIO_ENGINE->playEffect("sounds/ufo/hello_kid.mp3", false);
				this->scheduleOnce([=](float dt){
					AUDIO_ENGINE->playEffect("sounds/ufo/UFO_showText_secondtime.mp3", false);
				}, 5.2f, "playIntroSound");
				playUfo(UFO_GAF_STATE_INTRO);
			}), nullptr));
		}, 0.75f, "hideGameName");
	});
	
	this->schedule(schedule_selector(GameUFO::updateTimeNoTap), 0);
	//this->scheduleUpdate();
}

void GameUFO::onStartGame(){
	
	game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
	if (phase==1){
		if (curText == 0){
			if (!game_dataset.list_objects.at(curText).isHasImage)
				playUfo(UFO_GAF_STATE_PHASE5);
			else
				playUfo(UFO_GAF_STATE_PHASE1);
		}
		else{
			if (!game_dataset.list_objects.at(curText).isHasImage){
				auto objectPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel");
				objectPanel->runAction(Sequence::create(
					DelayTime::create(0.2f), FadeOut::create(0.3f)
					, CallFunc::create([=](){
					objectPanel->removeAllChildren();
					objectPanel->setOpacity(255);
					playUfo(UFO_GAF_STATE_PHASE4);
				}), nullptr));
			}
			else{
				auto objectPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel");
				objectPanel->runAction(Sequence::create(
					DelayTime::create(0.2f), FadeOut::create(0.3f)
					, CallFunc::create([=](){
					objectPanel->removeAllChildren();
					objectPanel->setOpacity(255);
					playUfo(UFO_GAF_STATE_PHASE2);
				}), nullptr));
			}
		}
	}
	else if (phase == 2) {
		auto objectPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel");
		if (curText == 0){
			objectPanel->runAction(Sequence::create(
				DelayTime::create(0.2f), FadeOut::create(0.3f)
				, CallFunc::create([=](){
				objectPanel->removeAllChildren();
				objectPanel->setOpacity(255);
				playUfo(UFO_GAF_STATE_PHASE3);
			}), nullptr));
		}
		else{
			objectPanel->runAction(Sequence::create(
				DelayTime::create(0.2f), FadeOut::create(0.3f)
				, CallFunc::create([=](){
				objectPanel->removeAllChildren();
				objectPanel->setOpacity(255);
				
				playUfo(UFO_GAF_STATE_SHOT_LAZER);
			}), nullptr));
		}
	}
}

void GameUFO::onFinishGame(){
  
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
	}), NULL));
}

void GameUFO::onFinishLevel()
{
	game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
	auto objectPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel");
	objectPanel->runAction(Sequence::create(
		DelayTime::create(0.2f), FadeOut::create(0.3f)
		, CallFunc::create([=](){
		objectPanel->removeAllChildren();
		objectPanel->setOpacity(255);
		playUfo(UFO_GAF_STATE_END);
	}), nullptr));
}

#pragma callback

void GameUFO::onRefresh(cocos2d::Ref *sender) {
    /*this->removeFromParent();
    auto phonics = GameUFO::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameUFO::showTutorial(float dt) {
	this->scheduleOnce([=](float dt) {
		_speaker->setTouchEnabled(false);
		AUDIO_ENGINE->stopAllAudio();
		this->runAction(Sequence::create(CallFunc::create([=] {
			for (int j = 0; j < listItems.size(); j++) {
				if (listItems.at(j)->text.compare(list_words.at(curText).text) == 0) {
					listItems.at(j)->runAction(Sequence::create(DelayTime::create(1),
						RotateTo::create(0.1f, 5.0f)
						, RotateTo::create(0.2f, -5.0f)
						, RotateTo::create(0.2f, 5.0f)
						, RotateTo::create(0.1f, 0.0f)
						, nullptr));
				}
			}
			})
			,DelayTime::create(1.8f)
			, CallFunc::create([=] {
				AUDIO_ENGINE->playSound(list_words.at(curText).audio_path, false, [=]() {
					_speaker->setTouchEnabled(true);
					timeNoTap = 0;
					}, 1.0f);
				})
				,nullptr));
		},dt,"showTutorial");
}

void GameUFO::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

