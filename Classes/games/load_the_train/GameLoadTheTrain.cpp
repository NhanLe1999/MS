
#include "GameLoadTheTrain.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
#include "StoryConfigManager.h"
#include "MsHelperGame.h"

INITIALIZE_READER(GameLoadTheTrain);
#define speed_opacity 15

std::once_flag GameLoadTheTrain_reader;
#define CSB_NAME "csb/game/load_the_train/GameLoadTheTrain.csb"

GameLoadTheTrain * GameLoadTheTrain::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameLoadTheTrain_reader, [] {
        REGISTER_CSB_READER(GameLoadTheTrain);
        REGISTER_CSB_READER(GameLoadTheTrain_Item);
		REGISTER_CSB_READER(GameLoadTheTrain_ToaTau);
    });
    auto p = static_cast<GameLoadTheTrain *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameLoadTheTrain::didLoadFromCSB(std::vector<math::resource::Image*> images)
{
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
	readConfig(value_data.GetObject(),images);

	for (auto audio : game_dataset.list_objects) {

		cocos2d::experimental::AudioEngine::preload(audio.audio_path);
	}
}

void GameLoadTheTrain::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    question_vi = "";
    
    auto getAudioPath = [=](std::string i_text)->std::string
    {
        for(auto image : images)
        {
            if(i_text == image->getText())
            {
                auto path = image->getAudios();
                
                return  path.front();
            }
        }
        return "";
    };
    
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi"))
    {
       auto audio_intro = value_data["audio_intro_vi"].GetArray();
       for (int i = 0; i < (int)audio_intro.Size(); i++)
       {
           auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
           question_vi = img_audio->getAudios("name_1").front();
       }
    }

	game_dataset.number_level = value_data["number_level"].GetInt();
	game_dataset.max_leng = 0;
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		TrainData object_info;
		auto object_value = list_object[i].GetObject();

		auto dataFull = math::resource::ResHelper::getImage(images, object_value["item_data"].GetString());

		auto synTexts = dataFull->getSyncText();
		
		std::copy(synTexts.begin(), synTexts.end(), back_inserter(object_info.listTextHight));

		object_info.audio_path = dataFull->getAudio();
		object_info.img_path = dataFull->path;

		auto toas = object_value["highlight"].GetArray();
		for (int j = 0; j < (int)toas.Size(); j++)
        {
			ToaData toa;
			toa.key = toas[j]["key"].GetString();
         
            if(game_dataset.textAndAudioPathList.count(toa.key) <= 0 || //not found
               (game_dataset.textAndAudioPathList.count(toa.key) > 0 && game_dataset.textAndAudioPathList.at(toa.key).empty()) //found but empty value
               )
            {
                game_dataset.textAndAudioPathList.insert({toa.key,getAudioPath(toa.key)});
            }
            
			auto lisDistractingValue = toas[j]["other_text"].GetArray();
			for (auto& dis : lisDistractingValue)
            {
                auto text =dis.GetString();

                toa.listDistracting.push_back(text);
                
                if(game_dataset.textAndAudioPathList.count(text) <= 0 || //not found
                   (game_dataset.textAndAudioPathList.count(text) > 0 && game_dataset.textAndAudioPathList.at(text).empty()) //found but empty value
                   )
                {
                    game_dataset.textAndAudioPathList.insert({text,getAudioPath(text)});
                }
			}
			object_info.toas.push_back(toa);
		}
		game_dataset.list_objects.push_back(object_info);

		if (game_dataset.max_leng < (int)object_value["highlight"].Size()){
			game_dataset.max_leng = (int)object_value["highlight"].Size();
		}
	}
}

void GameLoadTheTrain::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GameLoadTheTrain::setInputConfigGame(){
  
	mj::helper::shuffle(game_dataset.list_objects);
}

void GameLoadTheTrain::onItemTouch(Ref* sender, Widget::TouchEventType evt, GameLoadTheTrain_Item* aItem)
{
	timerCount = 0.0f;

	if (curText>=1 && timeToPause >= 2 && game_state == GAME_BASE_STATE::GBS_PLAYING && !game_play_enable && !is_show_pause_game){//&& bien gi do o day
        is_show_pause_game = true;
		pauseGame();
		game_state == GAME_BASE_STATE::GBS_PAUSE;
	}
	if (game_state != GAME_BASE_STATE::GBS_PLAYING)
		return;
	ImageView* img = static_cast<ImageView*>(sender);

	if (aItem->getOpacity() < 255 || img->getColor() != Color3B::WHITE)
		return;
	
	bool isMove = false;
	auto panelrun = cocos2d::utils::findChild(this, "runPanel");

	switch (evt)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		//phat tieng
		timeNoTouch = 0;
		hand->setVisible(false);
		AUDIO_ENGINE->playEffect("sounds/load_the_train/drag.mp3");
		aItem->setTop(img, ++curZorder);
		timerCount = 0.0f;
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::MOVED: {
		if (img->getTouchBeganPosition().distance(img->getTouchMovePosition()) > 15) {
			isMove = true;
		}
		if (isMove) {
			img->setPosition(aItem->convertToNodeSpace(img->getTouchMovePosition()));
		}
		break; }
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED: {
		if (img->getTouchBeganPosition().distance(img->getTouchEndPosition()) > 15) {
			isMove = true;
		}
		if (isMove) {
			if (toas.at(curWord)->getBoundingBox().containsPoint(panelrun->convertToNodeSpace(img->getTouchEndPosition()))) {
				if (aItem->_item_info.texts.at(img->getTag()).compare(toas.at(curWord)->_item_info.text) == 0) {
					// chon_dung
					timeToPause++;

					auto text = aItem->_item_info.texts.at(img->getTag());

					if (game_dataset.textAndAudioPathList.count(text) > 0 &&
						!game_dataset.textAndAudioPathList.at(text).empty())
					{
						AUDIO_ENGINE->playEffect(game_dataset.textAndAudioPathList.at(text));
					}

					AUDIO_ENGINE->playEffect("sounds/load_the_train/win.mp3");
					Vec2 posToa = aItem->convertToNodeSpace(toas.at(curWord)->convertToWorldSpace(toas.at(curWord)->getToaPos()));
					img->runAction(Sequence::create(
						MoveTo::create(0.2f, posToa)
						, CallFunc::create([=]() {
						aItem->setDone(img);
						toas.at(curWord)->loadColor(aItem->getColor(img));
						toas.at(curWord)->showToa(true);
						AUDIO_ENGINE->playEffect("sounds/load_the_train/horn.mp3");
						train->playSequence("win", false, [=](gaf::GAFObject* obj, const std::string name) {
							train->playSequence("idle", true);
						});

						if (curWord + 1 == game_dataset.list_objects.at(curText).toas.size()) {
							//show image
							game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
							this->scheduleOnce([=](float dt) {
								AUDIO_ENGINE->playEffect("sounds/load_the_train/show_image.mp3");
								for (int i = 0; i < items.size(); i++) {
									items.at(i)->runAction(FadeOut::create(0.5f));
								}

								auto aLayout = Layout::create();
								aLayout->setContentSize(Director::getInstance()->getVisibleSize());
								aLayout->setBackGroundColor(Color3B::BLACK);
								aLayout->setBackGroundColorOpacity(100);
								aLayout->setOpacity(0);
								this->addChild(aLayout);


								std::string textStr = "";

								for (auto s : game_dataset.list_objects.at(curText).toas) {

									textStr += (s.key + " ");
								}

								if (!textStr.empty() && textStr.back() == ' ') {
									textStr.pop_back();
								}

								auto text = ui::Text::create(textStr, "fonts/monkeystories_ultra.ttf", 50.0f);

								auto aImage = ImageView::create(game_dataset.list_objects.at(curText).img_path);
								aImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
								aLayout->addChild(aImage);
								aImage->setPosition({aLayout->getContentSize().width*0.5f,aLayout->getContentSize().height*0.5f - text->getContentSize().height*0.5f });
								aImage->setScale(0.01f);
								aLayout->runAction(FadeIn::create(0.5f));
								aImage->runAction(ScaleTo::create(0.5f, 1.0f));

								text->setPosition({ aImage->getPosition().x, aImage->getPosition().y + aImage->getContentSize().height * 0.5f + text->getContentSize().height * 0.5f + 10.0f });

								aImage->getParent()->addChild(text, aImage->getZOrder());

								ms::game::startHighlightText(game_dataset.list_objects.at(curText).listTextHight, text, game_dataset.list_objects.at(curText).audio_path, [=](int au_id, std::string path) {

									aLayout->runAction(Sequence::create(DelayTime::create(1.5f), RemoveSelf::create(), nullptr));
									// aLayout->removeFromParent();
									curText++;
									if (curText == game_dataset.number_level) {
										//max roi thi finish
										auto distance_move = toas.at(game_dataset.list_objects.at(curText - 1).toas.size() - 1)->getPositionX() + TOA_TAU_CONTENSIZE.width;
										moveTheTrain(true);
										panelrun->runAction(Sequence::create(
											EaseInOut::create(MoveBy::create(distance_move / velocity, Vec2(-distance_move, 0)), 1)
											, CallFunc::create([=]() {
											panelrun->setPositionX(Director::getInstance()->getVisibleSize().width);
											moveTheTrain(false);
											//onStartGame();
											onFinishGame();
										}), nullptr));
									}
									else {
										//chuaw max thi next lvl
										auto distance_move = toas.at(game_dataset.list_objects.at(curText - 1).toas.size() - 1)->getPositionX() + TOA_TAU_CONTENSIZE.width;
										moveTheTrain(true);
										panelrun->runAction(Sequence::create(
											EaseInOut::create(MoveBy::create(distance_move / velocity, Vec2(-distance_move, 0)), 1)
											, CallFunc::create([=]() {
											panelrun->setPositionX(Director::getInstance()->getVisibleSize().width);
											moveTheTrain(false);
											onStartGame();
										}), nullptr));
									}

								});

								text->removeFromParent();

							}, 0.5f, "finish_a_level");
						}
						else if (curWord % 4 == 3) {
							// qua toa tiep
							this->scheduleOnce([=](float dt) {
								moveTheTrain(true);
								game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
								int so_toa = game_dataset.list_objects.at(curText).toas.size() - 1 - curWord;
								if (so_toa > 4)
									so_toa = 4;
								for (int i = 0; i < items.size(); i++) {
									items.at(i)->runAction(FadeOut::create(0.5f));
								}
								float dis = so_toa * TOA_TAU_CONTENSIZE.width;
								float time = dis / velocity;
								panelrun->runAction(Sequence::create(
									MoveBy::create(time, Vec2(-dis, 0))
									, CallFunc::create([=]() {
									moveTheTrain(false);
									curWord++;
									showItem();
									this->scheduleOnce([=](float dt) {
										playItem(curWord);
									}, 0.5f, "start_enable_touch");
								}), nullptr));
							}, 0.5f, "toa_tiep");
						}
						else
							playItem(++curWord);
					}), nullptr));
					return;
				}
				else {
					AUDIO_ENGINE->playEffect("sounds/load_the_train/wrong.mp3");
					timeWrong++;
				}
			}

			//                                                               (aItem->_item_info.texts.at(img->getTag()).compare(->_item_info.text)==0)
																		   //aItem->_item_info.texts.at(img->getTag()).compare(toas.at(curWord)->_item_info.text)==0

			auto text = aItem->_item_info.texts.at(img->getTag());

			if (game_dataset.textAndAudioPathList.count(text) > 0 &&
				!game_dataset.textAndAudioPathList.at(text).empty())
			{
				AUDIO_ENGINE->playEffect(game_dataset.textAndAudioPathList.at(text));
			}

			//neu_ko_dung_la_sai
			float dis = img->getPosition().distance(aItem->getPosItem(img));
			float time = dis / 3000;
			img->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(time, aItem->getPosItem(img))), nullptr));
			if (timeWrong >= 2) {
				//timeWrong = 0;
				for (int i = 0; i < 4; i++) {
					if (i == curWord % 4) {
						items.at(i)->highLightItem(toas.at(curWord)->_item_info.text);
					}
				}
			}
		}
		break; }
	default:
		break;
	}

}

void GameLoadTheTrain::showItem()
{
	AUDIO_ENGINE->playEffect("sounds/load_the_train/show_up.mp3");
	float with = Director::getInstance()->getVisibleSize().width;
	auto tang = with - 1024;
	tang = tang / 1024;
	tang = tang / 2;
	tang = tang + 1;

	int word = game_dataset.list_objects.at(curText).toas.size() - curWord;
	if (word>4){
		word = 4;
	}
	
	for (int i = 0; i < 4; i++){
		items.at(i)->setVisible(false);
		items.at(i)->setTouchEnabled(false);
	}

	for (int i = 0; i < word; i++){
		items.at(i)->setOpacity(0);
		items.at(i)->setVisible(true);
		items.at(i)->loadData(datas.at(curWord + i));
		items.at(i)->runAction(FadeIn::create(0.5f));
		if (word % 2 == 0) {
			if (i < word / 2){
				items.at(i)->setPositionX(with / 2 - (word / 2 - i - 0.5f) * 256*tang);
			}
			else{
				items.at(i)->setPositionX(with / 2 + (i - word / 2 + 0.5f) * 256*tang);
			}
		}
		else{
			if (i < word/ 2){
				items.at(i)->setPositionX(with / 2 - (word / 2 - i)*256*tang);
			}
			else if (i == word / 2){
				items.at(i)->setPositionX(with / 2);
			}
			else{
				items.at(i)->setPositionX(with / 2 + (i - word / 2)*256*tang);
			}
		}
	}
	
}

void GameLoadTheTrain::playItem(int curItem)
{
	timeWrong = 0;
	for (int i = 0; i < 4; i++){
		if (i==curItem%4){
			items.at(i)->runAction(Sequence::createWithTwoActions(FadeIn::create(0.2f), CallFunc::create([=](){items.at(i)->setTouchEnabled(true);})));
		}
		else if (i>curItem%4){
			items.at(i)->setOpacity(100);
		}
	}
	this->scheduleOnce([=](float dt){
		game_state = GAME_BASE_STATE::GBS_PLAYING;
	}, 0.2f, "start_touch");
}

void GameLoadTheTrain::moveTheTrain(bool isMove)
{
	if (isMove){
		train->playSequence("run",true);
		for (int i = 0; i < toas.size(); i++){
			toas.at(i)->toa->playSequence("run",true);
		}
		audio_id = AUDIO_ENGINE->playEffect("sounds/load_the_train/train.mp3", true);
	}
	else{
		train->playSequence("idle");
		for (int i = 0; i < toas.size(); i++){
			toas.at(i)->toa->playSequence("idle");
		}
		AUDIO_ENGINE->stopEffect(audio_id);
	}
	
}

void GameLoadTheTrain::updateLoadTheTrain(float delta)
{
	if (game_state == GAME_BASE_STATE::GBS_PLAYING && curText >= 0 && curWord >= 0 && !onHandMove){
		timeNoTouch += delta;
		if (timeNoTouch > 3.0f) {
			timeNoTouch = 0;
			
		}
	}
}

void GameLoadTheTrain::moveHand()
{
	onHandMove = true;

	int index = curWord % 4;
	Vec2 posStart = Vec2(0, 0);
	Vec2 posTo = Vec2(0, 0);
	for (int i = 0; i < 3; i++){
		if (items.at(index)->_item_info.texts.at(i).compare(toas.at(curWord)->_item_info.text)==0){
			posStart = items.at(index)->convertToWorldSpace(items.at(index)->getPosItem(items.at(index)->imgs.at(i)));
		}
	}
	posTo = toas.at(curWord)->convertToWorldSpace(toas.at(curWord)->getToaPos());

	hand->setPosition(posStart);
	hand->setVisible(true);
	float dis = posStart.distance(posTo);
	float time_move = dis / 300;
	onHandMove = true;
	hand->runAction(Sequence::create(
		DelayTime::create(0.2f)
		, MoveTo::create(time_move, posTo)
		, DelayTime::create(0.2f)
		, CallFunc::create([=](){
		hand->setVisible(false); 
		onHandMove = false;
	}), nullptr));
}

void GameLoadTheTrain::loadFrames() {	
	//load anim con khi o day

	train = GAFWrapper::createGAF("gaf/load_the_train/dau_tau/dau_tau.gaf");
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "runPanel")->addChild(train);
	train->playSequence("run", true);
	train->setPosition(Director::getInstance()->getVisibleSize() / 2);
	train->setPositionX(Director::getInstance()->getVisibleSize().width/2-400);
	train->setPositionY(cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "duongray")->getPositionY() + 170);

	for (int i = 0; i < game_dataset.max_leng; i++){
		GameLoadTheTrain_ToaTau::ItemInfo aifno;
		auto toa = GameLoadTheTrain_ToaTau::createItem(aifno);
		cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "runPanel")->addChild(toa);
		//toa->setPosition(Director::getInstance()->getVisibleSize()/2);
		toa->setPositionY(cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "duongray")->getPositionY());
		if (i==0)
			toa->setPositionX(train->getPositionX() + train->getContentSize().width / 4 + 2);
		else
			toa->setPositionX(toas.at(i - 1)->getPositionX() + TOA_TAU_CONTENSIZE.width);

		toas.push_back(toa);
	}

	for (int i = 0; i < 4; i++){
		GameLoadTheTrain_Item::ItemInfo ifo;
		auto item = GameLoadTheTrain_Item::createItem(ifo);
		item->setVisible(false);
		item->setPositionY(192);
		cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectPanel")->addChild(item);
		items.push_back(item);
		auto sizedsa = Director::getInstance()->getVisibleSize().width;
		for (int j = 0; j < 3; j++)	{
			auto img = item->imgs.at(j);
			img->setTouchEnabled(true);
			img->addTouchEventListener(CC_CALLBACK_2(GameLoadTheTrain::onItemTouch, this, item));
		}
	}
	
	hand = cocos2d::utils::findChild<Sprite*>(this, "hand");
	hand->setLocalZOrder(10000);

	auto btnAudio = cocos2d::utils::findChild<Button*>(this, "btnAudio");
	btnAudio->setScale(0.5f);
	btnAudio->addClickEventListener([=](Ref* sender) {

		if (game_state == GAME_BASE_STATE::GBS_PLAYING) {

			auto currentSate = cocos2d::experimental::AudioEngine::getState(currentGameSentenceAudioID);

			if (currentSate != cocos2d::experimental::AudioEngine::AudioState::PLAYING) {

				currentGameSentenceAudioID = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects.at(curText).audio_path);

				cocos2d::experimental::AudioEngine::getDuration(currentGameSentenceAudioID);
			}
		}

	});
	//onLoadLevel();
	onPreGame();
}

void GameLoadTheTrain::onPreGame()
{
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/load_the_train/music.mp3", 1.0f);
    }
    
    if(!CONFIG_MANAGER->isGameVMonkey()) AUDIO_ENGINE->playEffect("sounds/load_the_train/gameName.mp3");
    
    auto game_name = cocos2d::utils::findChild<ImageView*>(this, "imgGameName");
    game_name->setVisible(!CONFIG_MANAGER->isGameVMonkey());
    game_name->runAction(Sequence::create(
                                          DelayTime::create(2.1f)
                                          , CallFunc::create([=](){
        onStartGame();
    }),FadeOut::create(0.5f)
                                          , RemoveSelf::create(true), nullptr));
    this->schedule(schedule_selector(GameLoadTheTrain::updateLoadTheTrain), 0);
    
}

void GameLoadTheTrain::onStartGame(){
	curWord = 0;
	onLoadLevel();
	moveTheTrain(true);

	auto playHornSoundAction = CallFunc::create([=]() {
		AUDIO_ENGINE->playEffect("sounds/load_the_train/horn.mp3");
	});

	auto runPanel = cocos2d::utils::findChild<cocos2d::ui::Widget*>(this, "runPanel");
	float dis = runPanel->getPositionX();
	velocity = dis / 2.5f;
	runPanel->runAction(Sequence::create(EaseIn::create(MoveTo::create(2.5f, Vec2(0, 0)), 1)
		, CallFunc::create([=]() {
		moveTheTrain(false);
	}), nullptr));

	this->runAction(Sequence::create(playHornSoundAction->clone(),DelayTime::create(2.5f),CallFunc::create([=]() {

		if (curText == 0) {

			auto callback = [=]() {

				this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]() {
					
					showItem();
					
					currentGameSentenceAudioID = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects.at(curText).audio_path);

					currentGameSentenceDuration = cocos2d::experimental::AudioEngine::getDuration(currentGameSentenceAudioID);

					cocos2d::experimental::AudioEngine::setFinishCallback(currentGameSentenceAudioID, [=](int au_id, std::string path) {
						playItem(0);
					});

					this->schedule([=](float dt) {
					
						timerCount += dt;

						if (currentGameSentenceDuration <= -1.0f) {

							currentGameSentenceDuration = cocos2d::experimental::AudioEngine::getDuration(currentGameSentenceAudioID);
						}

						if (timerCount >= (currentGameSentenceDuration <= -1.0f ? 8.0f : currentGameSentenceDuration + 8.0f)) {

							timerCount = 0.0f;

							if (!game_dataset.list_objects.empty()) {

								auto currentSate = cocos2d::experimental::AudioEngine::getState(currentGameSentenceAudioID);

								if (currentSate != cocos2d::experimental::AudioEngine::AudioState::PLAYING) {

									moveHand();

									currentGameSentenceAudioID  = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects.at(curText).audio_path);

									cocos2d::experimental::AudioEngine::getDuration(currentGameSentenceAudioID);
								}
							}
						}

					}, "repeat_game_audio_schedule");

				})));
			
			};

			if (CONFIG_MANAGER->isGameVMonkey() && question_vi.empty())
			{
				callback();
			}
			else
			{
				auto voice_id = cocos2d::experimental::AudioEngine::play2d(CONFIG_MANAGER->isGameVMonkey() ?
					question_vi :
					"sounds/load_the_train/put_the_correct_word.mp3");
				cocos2d::experimental::AudioEngine::setFinishCallback(voice_id, [=](int au_id, std::string path)
				{
					callback();
				});
			}
		}
		else
		{

			timerCount = 0.0f;

			this->scheduleOnce([=](float dt) {

				showItem();

				currentGameSentenceAudioID = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects.at(curText).audio_path);

				currentGameSentenceDuration = cocos2d::experimental::AudioEngine::getDuration(currentGameSentenceAudioID);

				timerCount = 0.0f;

				cocos2d::experimental::AudioEngine::setFinishCallback(currentGameSentenceAudioID, [=](int au_id, std::string path) {

					timerCount = 0.0f;

					playItem(0);
				});
			}, 0.5f, "start_a_level");
		}

	}),nullptr));

}

void GameLoadTheTrain::onFinishGame(){
   
	this->unschedule("repeat_game_audio_schedule");

	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_COMMON);
	}), NULL));
}

void GameLoadTheTrain::onFinishLevel()
{
	
}

void GameLoadTheTrain::onLoadLevel()
{
	datas.clear();
	for (int i = 0; i < game_dataset.list_objects.at(curText).toas.size(); i++){
		std::vector<std::string> a_item_data;
		a_item_data.push_back(game_dataset.list_objects.at(curText).toas.at(i).key);
		mj::helper::shuffle(game_dataset.list_objects.at(curText).toas.at(i).listDistracting);
		for (int j = 0; j < 2; j++){
			a_item_data.push_back(game_dataset.list_objects.at(curText).toas.at(i).listDistracting.at(j));
		}
		mj::helper::shuffle(a_item_data);
		datas.push_back(a_item_data);
	}

	for (int i = 0; i < toas.size(); i++){
		if (i<game_dataset.list_objects.at(curText).toas.size()){
			toas.at(i)->setVisible(true);
			toas.at(i)->loadString(game_dataset.list_objects.at(curText).toas.at(i).key);
		}
		else{
			toas.at(i)->setVisible(false);
		}
	}
}

#pragma callback

void GameLoadTheTrain::onRefresh(cocos2d::Ref *sender) {

}

void GameLoadTheTrain::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

