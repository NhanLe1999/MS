
#include "GameShookBook.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GameShookBook);
#define speed_opacity 15
#define audio_tap_on "sounds/shook_book/Tap on.mp3"
#define audio_correct "sounds/shook_book/correct.mp3"

#define audio_huzz "sounds/shook_book/huzz.mp3"
#define audio_pop "sounds/shook_book/pop.mp3"
#define audio_huzz_pop "sounds/shook_book/huzz_pop.mp3"
#define audio_wrong "sounds/shook_book/wrong.mp3"
#define audio_gameName "sounds/shook_book/gameName.mp3"
#define audio_music_01 "sounds/shook_book/music_01.mp3"
#define audio_jump "sounds/shook_book/jump.mp3"


std::once_flag GameShookBook_reader;
#define CSB_NAME "csb/game/shook_book/GameShookBook.csb"

GameShookBook * GameShookBook::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameShookBook_reader, [] {
        REGISTER_CSB_READER(GameShookBook);
    });
    auto p = static_cast<GameShookBook *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameShookBook::didLoadFromCSB(std::vector<math::resource::Image*> images) {
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

void GameShookBook::generateData(rapidjson::Value value_data){

}

void GameShookBook::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();

	game_dataset.repeat_duration = JSON_SAFE_CHECK(value_data, "repeat_duration", String) ? value_data["repeat_duration"].GetInt() : 8;

	for (int i = 0; i < (int)value_data["object"].Size(); i++) {
		LevelData levl;

		auto dataFull = math::resource::ResHelper::getImage(images, value_data["object"][i]["true_ans"].GetString());
		ItemInfo item_true;
		item_true.text = dataFull->getText();
		item_true.audio_path = dataFull->getAudio();
		item_true.img_path = dataFull->path;
		levl.true_ans = item_true;

		auto dataFalse = math::resource::ResHelper::getImage(images, value_data["object"][i]["flase_ans"].GetString());
		ItemInfo item_false;
		item_false.text = dataFalse->getText();
		item_false.audio_path = dataFalse->getAudio();
		item_false.img_path = dataFalse->path;
		levl.false_ans = item_false;
		if (value_data["object"][i].HasMember("intro")) {
			auto dataintro = math::resource::ResHelper::getImage(images, value_data["object"][i]["intro"].GetString());
			levl.intro = dataintro->getAudios("name_1").front();
		}

		auto dataquest = math::resource::ResHelper::getImage(images, value_data["object"][i]["text"].GetString());
		levl.audio_quest = dataquest->getAudios("name_1").front();

		game_dataset.list_objects.push_back(levl);
	}
}

void GameShookBook::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GameShookBook::setInputConfigGame(){

    mj::helper::shuffle(game_dataset.list_objects);

}

void GameShookBook::hideDistractingItem(int index_true)
{
	auto imgBook = cocos2d::utils::findChild<ImageView*>(this, "imgBook");
	imgBook->runAction(Sequence::create(
		RotateTo::create(0.1f, 5)
		,RotateTo::create(0.2f, -5)
		,RotateTo::create(0.2f, 5)
		,RotateTo::create(0.1f,0)
		, nullptr));
	for (int i = 0; i < 2; i++){
		std::string img_name = StringUtils::format("img%d", i + 1);
		auto img = cocos2d::utils::findChild<ImageView*>(this, img_name);
		std::string img_audio_name = StringUtils::format("imgAudio%d", i + 1);
		auto img_audio = cocos2d::utils::findChild<ImageView*>(this, img_audio_name);
		if (i!= index_true){
			img->runAction(FadeOut::create(0.5f));
			img_audio->runAction(FadeOut::create(0.5f));
		}
	}
}

void GameShookBook::loadFrames() {
	//load anim con khi o day

	monkey = GAFWrapper::createGAF("gaf/shook_book/monkey_shook.gaf");
	this->addChild(monkey,100);
	//monkey->playSequence("idle", true);
	monkey->setPositionY(320);
	monkey->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - 500);
	//monkey->setPosition(Director::getInstance()->getVisibleSize()/2);
	monkey->setVisible(false);

	for (int i = 0; i < 2; i++){
		std::string img_name = StringUtils::format("img%d", i + 1);
		auto img = cocos2d::utils::findChild<ImageView*>(this, img_name);
		std::string img_audio_name = StringUtils::format("imgAudio%d", i + 1);
		auto img_audio = cocos2d::utils::findChild<ImageView*>(this, img_audio_name);

		img->addClickEventListener([=](Ref* sender){
			if (game_state != GAME_BASE_STATE::GBS_PLAYING)
				return;

			if (curText>0 && !game_play_enable){
				pauseGame();
				return;
			}

            hideTapGuideGame();

			if (item_infos.at(i).text == game_dataset.list_objects.at(curText).true_ans.text){

                this->removeChildByName("guide_layout");

				game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
				auto speaker = cocos2d::utils::findChild<ImageView*>(this, "speaker");
				if (speaker) speaker->setVisible(false);
				img->runAction(Sequence::create(
					ScaleTo::create(0.05f, 1.05f)
					, ScaleTo::create(0.1f, 0.8f)
					, ScaleTo::create(0.05f, 1.05f)
					, ScaleTo::create(0.05f, 1.0f)
					, nullptr));
				AUDIO_ENGINE->playEffect(audio_correct);

				monkey->playSequence("win", false, [=](gaf::GAFObject * obj, const std::string seq_name){
					monkey->playSequence("idle", true);

					AUDIO_ENGINE->playEffect(audio_huzz);
					this->scheduleOnce([=](float dt){
						img->setTouchEnabled(true);
						//curText++;

					}, 0.7f, StringUtils::format("start_next_level%d", i).c_str());
				});
                showCorrectTextResult(curText);
			}
			else{
				AUDIO_ENGINE->playEffect(audio_pop);
				AUDIO_ENGINE->playEffect(audio_wrong);
				monkey->playSequence("wrong", false, [=](gaf::GAFObject * obj, const std::string seq_name){
					monkey->playSequence("idle", true);
				});
			}
		});

		img_audio->addClickEventListener([=](Ref* sender){
			img_audio->setTouchEnabled(false);

			if (!isReadingSound)
			{
				isReadingSound = true;
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(item_infos.at(i).audio_path);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
					isReadingSound = false;
				});
			}


			//AUDIO_ENGINE->playEffect(item_infos.at(i).audio_path);
			img_audio->runAction(Sequence::create(
				ScaleTo::create(0.1f,1.2f)
				, ScaleTo::create(0.2f, 0.8f)
				, ScaleTo::create(0.05f, 1.2f)
				, ScaleTo::create(0.05f, 0.8f)
				, ScaleTo::create(0.05f, 1.0f)
				, CallFunc::create([=](){
				img_audio->setTouchEnabled(true);
			}), nullptr));
		});
	}
	auto audio_id1 = cocos2d::experimental::AudioEngine::play2d(audio_gameName);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id1, [=](int au_id, std::string path){
        if(MJDEFAULT->getBoolForKey(key_background_music)){
            AUDIO_ENGINE->playEffect(audio_music_01, true);
        }
		onPreGame();
	});

}

void GameShookBook::onPreGame()
{
	auto runpanel = cocos2d::utils::findChild<Layout*>(this, "runPanel");
	auto gamename = cocos2d::utils::findChild<ImageView*>(this, "imgGameName");

	gamename->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.3f)
		//, MoveTo::create(2.0f, Vec2(0, runpanel->getPositionY()))
		, CallFunc::create([=](){
		runpanel->runAction(Sequence::create(
			EaseIn::create(MoveTo::create(2.0f, Vec2(0, runpanel->getPositionY())),1)
			, CallFunc::create([=](){
			monkey->setVisible(true);
			AUDIO_ENGINE->playEffect(audio_jump);
			monkey->playSequence("jump", false, [=](gaf::GAFObject * obj, const std::string seq_name){
				onStartGame();
				monkey->playSequence("idle", true);
			});

			cocos2d::utils::findChild(this, "bg_0")->removeFromParent();
			cocos2d::utils::findChild(this, "bg1_0")->removeFromParent();
		}),nullptr));
	}), nullptr));
}

void GameShookBook::onStartGame(){
	item_infos.clear();
	if (curText >= game_dataset.list_objects.size())
	{
		monkey->setVisible(false);
		onFinishGame();
	}
	item_infos.push_back(game_dataset.list_objects.at(curText).true_ans);
	item_infos.push_back(game_dataset.list_objects.at(curText).false_ans);
	mj::helper::shuffle(item_infos);

	for (int i = 0; i < 2; i++){
		std::string img_name = StringUtils::format("img%d", i + 1);
		auto img = cocos2d::utils::findChild<ImageView*>(this, img_name);
		img->setVisible(false);
		img->loadTexture(item_infos.at(i).img_path);

		auto sz = img->getVirtualRendererSize();
		auto ratio = sz.width / sz.height;
		auto size = Size(0, 0);
		if (ratio>1)
			size = Size(290, 290 / ratio);
		else
			size = Size(290 * ratio, 290);

		img->setContentSize(size);
		img_items.push_back(img);
		//img->setContentSize(img->getVirtualRendererSize());

		std::string img_audio_name = StringUtils::format("imgAudio%d", i + 1);
		auto img_audio = cocos2d::utils::findChild<ImageView*>(this, img_audio_name);
        img_audio->setVisible(false);

		this->scheduleOnce([=](float dt){
			img->setScale(0.01f);
			img->setVisible(true);
			img->setOpacity(255);
			AUDIO_ENGINE->playEffect(audio_huzz_pop);
			img_audio->setScale(0.01f);
			img_audio->setVisible(true);
			img_audio->setOpacity(255);

			img->runAction(Sequence::create(
                ScaleTo::create(0.05f, 0.2f)
                , ScaleTo::create(0.05f, 0.2f)
                , ScaleTo::create(0.05f, 0.4f)
				, ScaleTo::create(0.05f, 0.5f)
				, ScaleTo::create(0.05f, 0.6f)
				, ScaleTo::create(0.05f, 0.7f)
				, ScaleTo::create(0.05f, 0.8f)
				, ScaleTo::create(0.05f, 0.9f)
				, ScaleTo::create(0.05f, 1.0f)
				, nullptr));

			img_audio->runAction(Sequence::create(
                ScaleTo::create(0.05f, 0.2f)
                    , ScaleTo::create(0.05f, 0.2f)
                    , ScaleTo::create(0.05f, 0.4f)
                    , ScaleTo::create(0.05f, 0.5f)
                    , ScaleTo::create(0.05f, 0.6f)
                    , ScaleTo::create(0.05f, 0.7f)
                    , ScaleTo::create(0.05f, 0.8f)
                    , ScaleTo::create(0.05f, 0.9f)
                    , ScaleTo::create(0.05f, 1.0f)
				, nullptr));
		}, 0.5f*i, img_name);

	}

	// add speaker
	auto speaker = cocos2d::utils::findChild<ImageView*>(this, "speaker");
	if (!speaker) {
		speaker = (ImageView*)cocos2d::utils::findChild<ImageView*>(this, "imgAudio1")->clone();
		speaker->loadTexture("games/shook_book/loa icon.png");
		//speaker->setScale(0.6f);
		this->addChild(speaker, 1, "speaker");
		speaker->setPosition(Vec2(70, Director::getInstance()->getVisibleSize().height - 70));
		speaker->addClickEventListener([=](cocos2d::Ref* ref) {
			speaker->runAction(Sequence::create(ScaleTo::create(0.2f, 1.1f)
				, ScaleTo::create(0.05f, 0.95f)
				, ScaleTo::create(0.05f, 1.05f)
				, ScaleTo::create(0.05f, 0.97f)
				, ScaleTo::create(0.05f, 1.02f)
				, ScaleTo::create(0.05f, 1.0f)
				, nullptr));

            for(int i = 0; i < item_infos.size(); i++){
                if (item_infos.at(i).text == game_dataset.list_objects.at(curText).true_ans.text){
                    isReadingSound = true;
                    auto audio_path = std::string(item_infos.at(i).audio_path);
                    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                        isReadingSound = false;
                    });
                    break;
                }
            }
		});
	}

	speaker->setVisible(false);

	this->scheduleOnce([=](float dt){
		//monkey->playSequence("talk", true);
		monkey->playSequence("idle", true);
        //if(game_dataset.list_objects.at(curText).audio_quest == game_dataset.list_objects.at(curText).intro){
            auto audio_path = std::string(game_dataset.list_objects.at(curText).audio_quest);
            if(!cocos2d::FileUtils::getInstance()->isFileExist(audio_path))
            {
                game_state = GAME_BASE_STATE::GBS_PLAYING;
                CCLOG("start n�y");
                monkey->playSequence("idle", true);
                speaker->setVisible(true);
                inActiveMode();
                return;

            }
            AUDIO_ENGINE->playSound(audio_tap_on,false,[=]{
                for(int i = 0; i < item_infos.size(); i++){
                    if (item_infos.at(i).text == game_dataset.list_objects.at(curText).true_ans.text){
                        auto audio_path = std::string(item_infos.at(i).audio_path);
                        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                            game_state = GAME_BASE_STATE::GBS_PLAYING;
                            CCLOG("start n�y");
                            monkey->playSequence("idle", true);
                            speaker->setVisible(true);
                            inActiveMode();
                        });
                        break;
                    }
                }
            });
	}, 1.0f, "start_voice");


}

void GameShookBook::onFinishGame(){
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        cocos2d::experimental::AudioEngine::stopAll();
		commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
	}), NULL));
}

void GameShookBook::onFinishLevel()
{

}

#pragma callback

void GameShookBook::onRefresh(cocos2d::Ref *sender) {
    /*this->removeFromParent();
    auto phonics = GameShookBook::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameShookBook::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->removeChildByName("guide_layout");
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void GameShookBook::hideTapGuideGame()
{
    this->removeChildByName("guide_layout");
}

void GameShookBook::showCorrectTextResult(int &level)
{
	for (int i = 0; i < 2; i++) {
        creatLabelItem(i);
		std::string img_name = StringUtils::format("img%d", i + 1);
		std::string img_audio_name = StringUtils::format("imgAudio%d", i + 1);
		auto img_audio = cocos2d::utils::findChild<ImageView*>(this, img_audio_name);

		// creat label under sound image
		std::string image_text = StringUtils::format("imgText%d", i + 1);
		auto img_text = cocos2d::utils::findChild<Label*>(img_audio->getParent(), image_text);

		img_text->setOpacity(0);
		string correctText = game_dataset.list_objects.at(curText).true_ans.text;
		string imageTextString = img_text->getString();
		if(imageTextString == correctText)
			img_text->setTextColor(Color4B::RED);
		else
		{
			img_text->setTextColor(Color4B::BLACK);
		}
		img_text->setVisible(true);
		Vector<FiniteTimeAction*> actionsShowText;
		Vector<FiniteTimeAction*> actionshideAudio;



		int opacity = 255;
		for (int i = 0; i <= 10; i++) {
			opacity -= 30;
			if (opacity <= 0)
				opacity = 0;
			actionshideAudio.pushBack(CallFunc::create([=]() {
				img_audio->setOpacity(opacity);
				}));
			actionsShowText.pushBack(DelayTime::create(0.05f));
		}
		actionshideAudio.pushBack(CallFunc::create([=]() {
			img_audio->setVisible(false);
			// after hide audio, show text
			}));

		// show text
		actionsShowText.pushBack(DelayTime::create(0.5f));
		actionsShowText.pushBack(CallFunc::create([=]() {
			if (level >= game_dataset.list_objects.size())
				return;
			if (imageTextString == correctText)
				AUDIO_ENGINE->playEffect(game_dataset.list_objects.at(level).true_ans.audio_path);
			}));
		actionsShowText.pushBack(DelayTime::create(0.1f));

		for (int i = 0; i <= 10; i++) {

			opacity += 30;
			if (opacity >= 255)
				opacity = 255;
			actionsShowText.pushBack(CallFunc::create([=]() {
				img_text->setOpacity(opacity);
				}));
			actionsShowText.pushBack(DelayTime::create(0.05f));
		}
		actionsShowText.pushBack(DelayTime::create(2.0f));
		actionsShowText.pushBack(CallFunc::create([=]() {
			img_text->setVisible(false);
			img_text->removeFromParent();
			}));

		img_audio->runAction(Sequence::create(actionshideAudio));
		img_text->runAction(Sequence::create(actionsShowText));


		//img_audio->runAction(Sequence::create(FadeOut::create(0.1f), CallFunc::create([=]() {
		//	img_audio->setVisible(false);
		//	img_text->setVisible(true);
		//	img_text->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {

		//		img_text->setTextColor(Color4B::RED);
  //              
		//		this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
		//			//img->setVisible(false);
		//			img_text->setVisible(false);
		//			img_text->removeFromParent();
		//			}), NULL));
		//		}), NULL));


		//}), NULL));
	}

	this->runAction(Sequence::create(DelayTime::create(4.5f), CallFunc::create([=]() {
		if (level >= game_dataset.number_level || game_dataset.list_objects.size()<= curText) {
			monkey->setVisible(false);
			onFinishGame();
		}
		else
		{
			onStartGame();
		}
	}), NULL));
	level++;

}

void GameShookBook::creatLabelItem(int index)
{
	std::string img_audio_name = StringUtils::format("imgAudio%d", index + 1);
	auto img_audio = cocos2d::utils::findChild<ImageView*>(this, img_audio_name);

	std::string image_text = StringUtils::format("imgText%d", index + 1);
	auto label = Label::createWithTTF("", "fonts/ShookTheBookFonts.ttf", 60);
	img_audio->getParent()->addChild(label, 1000, image_text);
	string text = item_infos.at(index).text;
	label->setString(item_infos.at(index).text);
	label->setTextColor(Color4B::BLACK);
	label->setPosition(img_audio->getPosition());
	if (item_infos.at(index).text.size() > 10) {//chữ bé -> zoom to ra.
		label->setTTFConfig(TTFConfig("fonts/ShookTheBookFonts.ttf",45));
	}
	label->setWidth(350);
	label->setHeight(70);
	label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	label->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	label->setVisible(false);
}

void GameShookBook::inActiveMode() {
    
    auto guide_layout_check = this->getChildByName("guide_layout");
    if (guide_layout_check) return;
    auto guide_layout = cocos2d::ui::Layout::create();
    guide_layout->setContentSize(Director::getInstance()->getVisibleSize());
    this->addChild(guide_layout, INT_MAX - 2);
    guide_layout->setName("guide_layout");
    
    ImageView* right_img;
    
    for(int i = 0; i < item_infos.size(); i++){
        if (item_infos.at(i).text == game_dataset.list_objects.at(curText).true_ans.text){
            right_img = utils::findChild<ImageView*>(this, StringUtils::format("img%d", i + 1));
        }
    }
	std::string preAudio = "Tap on " + game_dataset.list_objects.at(curText).true_ans.text;
	int audioLength = std::count(preAudio.begin(), preAudio.end(), ' ');

    guide_layout->schedule([=](float) {

        if (curText >= 0 && curText < game_dataset.list_objects.size() && !isReadingSound) {
			isReadingSound = true;
			auto audio_text_id = cocos2d::experimental::AudioEngine::play2d(audio_tap_on);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_text_id, [=](int au_id, std::string path) {
				for(int i = 0; i < item_infos.size(); i++){
					if (item_infos.at(i).text == game_dataset.list_objects.at(curText).true_ans.text){
						auto audio_path = std::string(item_infos.at(i).audio_path);
						auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
						cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
							isReadingSound = false;
						});
						break;
					}
					else if(i >= item_infos.size())
					{
						isReadingSound = false;
					}
				}
                
            });
        }
    }, game_dataset.repeat_duration + 0.7* (audioLength +1), 100, game_dataset.repeat_duration+ 0.5 * (audioLength + 1), "animated_guide");
}
