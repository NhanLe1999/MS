
#include "GameDiamondDig.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
INITIALIZE_READER(GameDiamondDig);
#define speed_opacity 15

std::once_flag GameDiamondDig_reader;
#define CSB_NAME "csb/game/diamond_dig/GameDiamondDig.csb"

GameDiamondDig * GameDiamondDig::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameDiamondDig_reader, [] {
        REGISTER_CSB_READER(GameDiamondDig);
        REGISTER_CSB_READER(GameDiamondDig_Item);
    });
    auto p = static_cast<GameDiamondDig *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

#include "MJDefault.h"
#include "MJ_AnimationCache.h"
#define key_count_guid_diamond_dig "key_count_guide_diamond_dig"

void GameDiamondDig::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guid_diamond_dig, 0);
    _is_guide_showed = !(count < 3);
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guid_diamond_dig, count + 1);
    }, 30, "delay_update_count_guide_diamond_dig");
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

	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		inActiveMode();
		if (!_speakable) {
			_speakable = true;
			_audioReminder = cocos2d::experimental::AudioEngine::play2d("sounds/diamond_dig/remider.mp3");
			cocos2d::experimental::AudioEngine::setFinishCallback(_audioReminder, [=](int au_id, std::string path) {
				_speakable = false;
				});
		}
		});
	this->addChild(speaker);
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

void GameDiamondDig::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	
	game_dataset.number_level = value_data["content_info"]["number_level"].GetInt();
	auto list_object = value_data["content_info"]["data"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        DiamondData data_info;
        auto object_value = list_object[i].GetObject();
		data_info.img_path = object_value["img_path"].GetString();
		for (int j = 0; j < object_value["highlight"].Size(); j++){
			GameDiamondDig_Item::ItemInfo word_info;
			word_info.audio_path = object_value["highlight"][j]["audio_path"].GetString();
			word_info.text = object_value["highlight"][j]["w"].GetString();
			word_info.isTrueAnswer = object_value["highlight"][j]["isTrueAnswer"].GetBool();
			data_info.word_infos.push_back(word_info);
		}
        game_dataset.list_objects.push_back(data_info);
    }
}

void GameDiamondDig::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();
	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		DiamondData data_info;
		auto object_value = list_object[i].GetObject();
		
		auto text_true = math::resource::ResHelper::getImage(images, object_value["true_answer"].GetString());
		auto text_false = math::resource::ResHelper::getImage(images, object_value["false_answer"].GetString());
		data_info.img_path = text_true->path;
		{
			GameDiamondDig_Item::ItemInfo word_info;
			word_info.audio_path = text_true->getAudio();
			word_info.text = text_true->getText();
			word_info.isTrueAnswer = true;
			data_info.word_infos.push_back(word_info);
            data_info.audio_path = word_info.audio_path;
		}
		{
			GameDiamondDig_Item::ItemInfo word_info;
			word_info.audio_path = text_false->getAudio();
			word_info.text = text_false->getText();
			word_info.isTrueAnswer = false;
			data_info.word_infos.push_back(word_info);
		}
		game_dataset.list_objects.push_back(data_info);
	}
}

void GameDiamondDig::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GameDiamondDig::setInputConfigGame(){
//    auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	for (int i = 0; i < game_dataset.list_objects.size(); i++){
		mj::helper::shuffle(game_dataset.list_objects.at(i).word_infos);
	}
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
    
}

void GameDiamondDig::callTextPreGame(int index/* = 0*/)
{
	_speakable = true;
	CCLOG("call index = %d", index);
	if (index < 0){
		_speakable = false;
		game_state = GAME_BASE_STATE::GBS_PLAYING;
		return;
	}
	CCLOG("call index = %d", index);
	word_items.at(index)->onRead(true);
	auto audio_path = std::string(word_items.at(index)->_item_info.audio_path);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		inActiveMode();
		word_items.at(index)->onRead(false);
		word_items.at(index)->runAction(Sequence::create( DelayTime::create(1.5f),CallFunc::create([=] {
			callTextPreGame(index - 1);
			})
		,NULL));
	});
}

void GameDiamondDig::digTrueItem(GameDiamondDig_Item* aItem)
{
	AUDIO_ENGINE->stopAllEffects();
	timeToPause++;
	game_state = BaseState::GBS_PLAY_VIDEO;
	for (int i = 0; i < (int)word_items.size(); i++){
		if (aItem == word_items.at(i)){
			aItem->setLocalZOrder(10);
			aItem->runAction(Sequence::create(
				MoveTo::create(0.3f, Vec2(aItem->getPositionX(), Director::getInstance()->getVisibleSize().height*0.2f))
				, CallFunc::create([=](){
				auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "objectLayout");
				auto img = static_cast<ImageView*>(objectLayout->getChildByName("img"));

					img->runAction(FadeOut::create(0.3f));

					AUDIO_ENGINE->playEffect("sounds/diamond_dig/smash_correct.mp3");

				aItem->onDig();

				riu->setPosition(aItem->getPosition() + Vec2(aItem->getContentSize().width / 2, 0));
				riu->setVisible(true);
				riu->playSequence("correct", false, [=](gaf::GAFObject *obj, std::string name_sequence){
					riu->setVisible(false);
					//game_state = DIAMON_DIG_GAME_STATE::DDGS_PLAYING;
					aItem->runAction(MoveBy::create(0.3f, Vec2(0, -Director::getInstance()->getVisibleSize().height)));
					this->scheduleOnce([=](float dt){
						onFinishLevel();
					}, 0.2f, "finish_level");
				});
			})
				, nullptr));
		}
		else{
			word_items.at(i)->runAction(MoveBy::create(0.5f, Vec2(0, -Director::getInstance()->getVisibleSize().height)));
		}
	}
}

void GameDiamondDig::loadFrames() {	
	//load anim con khi o day

	nodeStone = Node::create();
	auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "objectLayout");
	objectLayout->addChild(nodeStone, 50);

	stone = GAFWrapper::createGAF("gaf/diamond_dig/dig_stone/dig_stone.gaf");
	nodeStone->addChild(stone, 50);
	nodeStone->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height + 300));

	auto bg = cocos2d::utils::findChild<ImageView*>(this, "bg");
	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	}

	riu = GAFWrapper::createGAF("gaf/diamond_dig/dig_axe/dig_axe.gaf");
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout")->addChild(riu,100);
	riu->setPosition(Director::getInstance()->getVisibleSize()/2);
	riu->setVisible(false);

	for (int i = 0; i < 2; i++){
		auto aItem = GameDiamondDig_Item::createItem(game_dataset.list_objects.at(0).word_infos.at(0));
		aItem->setPosition(Vec2(-1000, -1000));
		cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "objectLayout")->addChild(aItem, curZorder);
		aItem->addClickEventListener([=](Ref* sender){
			inActiveMode();
            mj::helper::hideTap();
			if (game_state != GAME_BASE_STATE::GBS_PLAYING)
				return;
			if (game_state == GAME_BASE_STATE::GBS_PLAYING && timeToPause == 1 && !game_play_enable){// && 1 cai dieu kien gi do o day nua
				pauseGame();
				return;
			}
			if (aItem->_item_info.isTrueAnswer){
				if (_speakable) {
					AUDIO_ENGINE->stopEffect(_audioReminder);
				}
				digTrueItem(aItem);
			}
			else{
				game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
				if (_speakable) {
					AUDIO_ENGINE->stopEffect(_audioReminder);
				}
				AUDIO_ENGINE->playEffect("sounds/diamond_dig/smash_wrong.mp3");
				riu->setPosition(aItem->getPosition() + Vec2(aItem->getContentSize().width / 2, 0));
				riu->setVisible(true);
				riu->playSequence("wrong", false, [=](gaf::GAFObject *obj, std::string name_sequence){
					riu->setVisible(false);
					game_state = GAME_BASE_STATE::GBS_PLAYING;
                   //inActiveMode();
				});
			}
		});
		word_items.pushBack(aItem);
	}

    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playEffect("sounds/diamond_dig/music_bg.mp3", true);
    }
	//onLoadLevel();
	onPreGame();
}

void GameDiamondDig::onPreGame()
{
	game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;

	auto img_game_name = Sprite::create("games/diamond_dig/diamond_dig_game_name.png");
	img_game_name->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	img_game_name->setPosition(Vec2(0, -37));
	img_game_name->setScale(2.5f);
	nodeStone->addChild(img_game_name, 100);


	nodeStone->runAction(Sequence::create(MoveTo::create(0.8f, Director::getInstance()->getVisibleSize() / 2), CallFunc::create([=](){
		auto audio_gameName = cocos2d::experimental::AudioEngine::play2d("sounds/diamond_dig/gameName.mp3");
		cocos2d::experimental::AudioEngine::setFinishCallback(audio_gameName, [=](int au_id, std::string path){
			this->scheduleOnce([=](float dt){
				AUDIO_ENGINE->playEffect("sounds/diamond_dig/slide1.mp3");
			}, 0.3f, "slide1.mp3");

			this->scheduleOnce([=](float dt){
				AUDIO_ENGINE->playEffect("sounds/diamond_dig/smash_begin.mp3");
				riu->setPosition(nodeStone->getPosition());
				riu->setVisible(true);
				riu->playSequence("correct", false, [=](gaf::GAFObject *obj, std::string name_sequence){
					riu->setVisible(false);
				});
				img_game_name->runAction(FadeOut::create(1.0f));
				stone->playSequence("correct");
			}, 1.0f, "riu1.mp3");
			auto audio_path = std::string("sounds/diamond_dig/diamon_dig_pregame.mp3");
			auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
			cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
				nodeStone->setCascadeOpacityEnabled(true);
				nodeStone->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
				this->scheduleOnce([=](float dt){
					onStartGame();
				}, 0.2f, "start_game.mp3");
			});
		});
	}), nullptr));


}

void GameDiamondDig::onStartGame(){
	inActiveMode();
	auto objectLayout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "objectLayout");
	auto img = static_cast<ImageView*>(objectLayout->getChildByName("img"));
	if (img)
		img->removeFromParent();
	img = ImageView::create(game_dataset.list_objects.at(curText).img_path);
	img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	img->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height + img->getContentSize().height / 2));
    img->setContentSize(Size(500, 400));
    img->ignoreContentAdaptWithSize(false);
    img->setTouchEnabled(true);
	objectLayout->addChild(img, 1, "img");

	img->addClickEventListener([=](Ref* sender) {
		if (audio_question_id != 0) {
			AUDIO_ENGINE->stopEffect(audio_question_id);
		}
		if (!_speakable) {
			_speakable = true;
			auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects.at(curText).audio_path);
			cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
				_speakable = false;
				inActiveMode();
				});

		}
	});
	for (int i = 0; i < (int)game_dataset.list_objects.at(curText).word_infos.size(); i++){
		GameDiamondDig_Item* aItem = nullptr;
		if (word_items.size() <= i){
			aItem = GameDiamondDig_Item::createItem(game_dataset.list_objects.at(curText).word_infos.at(i));
			objectLayout->addChild(aItem);
			aItem->addClickEventListener([=](Ref* sender){
				inActiveMode();
                
				if (game_state != GAME_BASE_STATE::GBS_PLAYING)
					return;
				if (game_state == GAME_BASE_STATE::GBS_PLAYING && timeToPause == 1 && !game_play_enable){// && 1 cai dieu kien gi do o day nua
					pauseGame();
					return;
				}
				if (aItem->_item_info.isTrueAnswer){
					digTrueItem(aItem);
				}
				else{
					game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
					AUDIO_ENGINE->playEffect("sounds/diamond_dig/smash_wrong.mp3");
					riu->setPosition(aItem->getPosition() + Vec2(aItem->getContentSize().width / 2, 0));
					riu->setVisible(true);
					riu->playSequence("wrong", false, [=](gaf::GAFObject *obj, std::string name_sequence){
						riu->setVisible(false);
						game_state = GAME_BASE_STATE::GBS_PLAYING;
					});
				}
			});
			word_items.pushBack(aItem);
		}
		else{
			aItem = word_items.at(i);
			aItem->setNewData(game_dataset.list_objects.at(curText).word_infos.at(i));
		}
		float posY = -450 + i * 150;
		aItem->stopAllActions();
		aItem->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - aItem->getContentSize().width / 2, posY));
		aItem->runAction(MoveTo::create(0.3f, Vec2(Director::getInstance()->getVisibleSize().width / 2 - aItem->getContentSize().width / 2, 30 + i * 150)));
        
        if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson)){
            _is_guide_showed = true;
        }
	}

	img->runAction(Sequence::create(
		MoveTo::create(0.3f, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height*0.7f))
		, CallFunc::create([=](){
			//this->scheduleOnce([=](float dt){
				callTextPreGame(word_items.size()-1);
			//}, 0.2f, "call_text_pre_game");
		})
		, nullptr));

	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->playEffect("sounds/diamond_dig/");
	}, 0.3f, "slide2");
}
void GameDiamondDig::inActiveMode(){
	unschedule("inActiveMode");
    this->schedule([=](float) {
		_speakable = true;
		_audioReminder = cocos2d::experimental::AudioEngine::play2d("sounds/diamond_dig/remider.mp3");
		cocos2d::experimental::AudioEngine::setFinishCallback(_audioReminder, [=](int au_id, std::string path) {
			_speakable = false;
			unschedule("inActiveMode");
			inActiveMode();
			});
    }, 8, "inActiveMode");
}
void GameDiamondDig::onFinishGame(){
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
    AUDIO_ENGINE->stopAllAudio();
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_COMMON);
	}), NULL));
}

void GameDiamondDig::onFinishLevel()
{
	curText++;
	if (curText<game_dataset.number_level){
		onStartGame();
	}
	else{
		onFinishGame();
	}
}

#pragma callback
void GameDiamondDig::onRefresh(cocos2d::Ref *sender) {

}

void GameDiamondDig::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
}
void GameDiamondDig::showGuiding() {
	
	auto gaf = GAFWrapper::createGAF("gaf/hand_tap_2/hand_tap_2.gaf");
	gaf->setName("gaf_xxx_yyy");
	gaf->setAnchorPoint(Point(0.5, 0.5));
	gaf->setScale(1);
	
	for (auto item : word_items) {
		if (item->_item_info.isTrueAnswer) {
			auto bb = item->getBoundingBox();
			auto p = Point(bb.origin.x + bb.size.width * 0.8, bb.getMidY());
			gaf->setPosition(p);
			item->addChild(gaf, INT_MAX - 1);
			break;
		}
	}
	gaf->setOpacity(0);
	gaf->setName("hand_guiding");
	gaf->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), CallFuncN::create([gaf](Node* n) {
		gaf->play(true);
		})));
	gaf->setVisible(true);
}
