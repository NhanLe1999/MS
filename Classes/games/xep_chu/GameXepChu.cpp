
#include "GameXepChu.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
#include "APProfileManager.h"

INITIALIZE_READER(GameXepChu);
#define speed_opacity 15

std::once_flag GameXepChu_reader;
#define CSB_NAME "csb/game/xep_chu/GameXepChu.csb"

GameXepChu * GameXepChu::createGame(std::vector<math::resource::Image*> images) {
	std::call_once(GameXepChu_reader, [] {
        REGISTER_CSB_READER(GameXepChu);
        REGISTER_CSB_READER(GameXepChu_Item);
    });
    auto p = static_cast<GameXepChu *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

#include "MJDefault.h"
#define key_count_guide_jumble_word "key_count_guide_jumble_word"
void GameXepChu::didLoadFromCSB(std::vector<math::resource::Image*> images) {
	_is_guide_showed = true;
	_age = ap::ProfileManager::getInstance().getInformationOfProfile().age;
    
    this->scheduleOnce([](float) {
    }, 20, "update_count_guide_jumble_words");
    
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

void GameXepChu::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	
	game_dataset.number_level = value_data["content_info"]["number_level"].GetInt();
	auto list_object = value_data["content_info"]["data"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        XepChuData data_info;
		data_info.numItemFly = 0;
        auto object_value = list_object[i].GetObject();
		data_info.audio_path = object_value["audio_path"].GetString();
		for (int j = 0; j < object_value["highlight"].Size(); j++){
			GameXepChu_Item::ItemInfo word_info;
			word_info.audio_path = object_value["highlight"][j]["audio_path"].GetString();
			word_info.text = object_value["highlight"][j]["w"].GetString();
			word_info.img_path = object_value["highlight"][j]["img_path"].GetString();
			word_info.fly = object_value["highlight"][j]["isFly"].GetBool();
			word_info.time_call = object_value["highlight"][j]["s"].GetFloat()/1000;
			word_info.time_end = object_value["highlight"][j]["e"].GetFloat() / 1000;
			data_info.word_infos.push_back(word_info);

			if (word_info.fly)
				data_info.numItemFly++;
		}
        game_dataset.list_objects.push_back(data_info);
    }

	color4bs.push_back(Color4B::RED);
	color4bs.push_back(Color4B::BLUE);
	color4bs.push_back(Color4B::GREEN);
	color4bs.push_back(Color4B::ORANGE);
	color4bs.push_back(Color4B::YELLOW);
}

void GameXepChu::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	game_dataset.number_level = value_data["number_level"].GetInt();

	if (JSON_SAFE_CHECK(value_data, "audio_intro", String))
	{
		auto keyAudio = value_data["audio_intro"].GetString();
		auto dataAudioIntro = math::resource::ResHelper::getImage(images, keyAudio);
		_pathAudioIntro = dataAudioIntro->getAudio();
	}

	auto list_object = value_data["data"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		XepChuData data_info;
		data_info.numItemFly = 0;
		auto object_value = list_object[i].GetObject();
		auto dataFull = math::resource::ResHelper::getImage(images, object_value["audio_path"].GetString());
		std::vector<math::resource::Sync_Text> sync_texts = dataFull->getSyncText();
		data_info.audio_path = dataFull->getAudio();
		data_info._listTextHight = dataFull->getSyncText();
		
		for (int j = 0; j < sync_texts.size(); j++){
			GameXepChu_Item::ItemInfo word_info;
			word_info.audio_path = "";
			word_info.img_path = "";
			
			word_info.text = sync_texts.at(j).w;
			word_info.fly = false;

			word_info.time_call = (float)sync_texts.at(j).s / 1000;
			word_info.time_end = (float)sync_texts.at(j).e / 1000;
			data_info.word_infos.push_back(word_info);
		}
		if (object_value.HasMember("normal_fly")){
			for (int j = 0; j < object_value["normal_fly"].Size(); j++){
				std::string key = object_value["normal_fly"][j].GetString();
				auto data_text = math::resource::ResHelper::getImage(images, key);
				std::string keyText = key, keyAudio = "";

				if (data_text)
				{
					keyText = data_text->getText();
					keyAudio = data_text->getAudio();
				}

				for (int k = 0; k < data_info.word_infos.size(); k++){
					if (data_info.word_infos.at(k).text.compare(keyText) == 0){
						data_info.word_infos.at(k).audio_path = keyAudio;
						data_info.word_infos.at(k).img_path = "";
						data_info.word_infos.at(k).fly = true;
						data_info.numItemFly++;
						break;
					}
				}
			}
		}
		if (object_value.HasMember("highlight")){
			for (int j = 0; j < object_value["highlight"].Size(); j++){
				std::string key = object_value["highlight"][j].GetString();
				auto data_text = math::resource::ResHelper::getImage(images, key);
				for (int k = 0; k < data_info.word_infos.size(); k++){
					if (data_info.word_infos.at(k).text.compare(data_text->getText()) == 0){
						data_info.word_infos.at(k).audio_path = data_text->getAudios("name_1").front();
						data_info.word_infos.at(k).img_path = data_text->path;
						data_info.word_infos.at(k).fly = true;
						data_info.numItemFly++;
						break;
					}
				}
			}
		}
		game_dataset.list_objects.push_back(data_info);
	}

	color4bs.push_back(Color4B::RED);
	color4bs.push_back(Color4B::BLUE);
	//color4bs.push_back(Color4B::GREEN);
	//color4bs.push_back(Color4B::ORANGE);
	//color4bs.push_back(Color4B::YELLOW);
}

void GameXepChu::onEnter(){
    ms::BaseActivity::onEnter();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("games/xep_chu/monkey.plist");
    setInputConfigGame();
    loadFrames();
}

void GameXepChu::setInputConfigGame(){
//    auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	//for (int i = 0; i < game_dataset.list_objects.size(); i++){
	//	mj::helper::shuffle(game_dataset.list_objects.at(i));
	//}
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);
    
}

void GameXepChu::monkeyJump()
{
	//GlobalFunc::loadSpriteSheets("lesson/monkey");
	SpriteFrameCache *cache = SpriteFrameCache::getInstance();
	auto sceneSize = Director::getInstance()->getVisibleSize();
	int zIndex = 201;
	//tao node chu 1 dong nhung chiec la
	auto node_lead = Node::create();
	node_lead->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	node_lead->setContentSize(sceneSize);
	node_lead->setPosition(Vec2(0, sceneSize.height + sceneSize.height * 0.5f));
	addChild(node_lead, zIndex + 1);

	auto lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.257f, 67.6f));
	lead->setRotation(85.1);
	node_lead->addChild(lead);
	auto lead_rotate = RotateBy::create(0.5, -71.5);
	lead->runAction(RepeatForever::create(lead_rotate));

	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.692f, 135.6f));
	lead->setRotation(-32.4);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, 36.4);
	lead->runAction(RepeatForever::create(lead_rotate));

	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.138f, 205.2f));
	lead->setScale(0.6f);
	lead->setRotation(0);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, -52.9);
	lead->runAction(RepeatForever::create(lead_rotate));

	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.836f, 238.8f));
	lead->setScale(0.7f);
	lead->setRotation(-90);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, 53.4);
	lead->runAction(RepeatForever::create(lead_rotate));


	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.358f, 278.2f));
	lead->setScale(0.7f);
	lead->setRotation(12.2);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, 52.4);
	lead->runAction(RepeatForever::create(lead_rotate));


	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.482f, 176.4f));
	lead->setScale(0.8f);
	lead->setRotation(12.2);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, 75.7);
	lead->runAction(RepeatForever::create(lead_rotate));

	lead = Sprite::createWithSpriteFrameName("khi_la.png");
	lead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lead->setPosition(Vec2(sceneSize.width*0.742f, 305.2f));
	lead->setScale(0.3f);
	lead->setRotation(12.2);
	node_lead->addChild(lead);
	lead_rotate = RotateBy::create(0.5, 77.0f);
	lead->runAction(RepeatForever::create(lead_rotate));


	//day thung cho khi tom
	auto day1 = Sprite::createWithSpriteFrameName("khi_day.png");
	day1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	day1->setPosition(Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.5f));
	addChild(day1, zIndex);

	auto khi = Sprite::createWithSpriteFrameName("khi_1.png");
	khi->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	khi->setPosition(Vec2(sceneSize.width / 2, sceneSize.height*-0.8f));
	addChild(khi, zIndex);



	auto day_delay = DelayTime::create(1.8f);
	auto day_mv1 = MoveTo::create(0.3f, (Vec2(sceneSize.width / 2, sceneSize.height - 31)));
	auto day_seq1 = Sequence::create(day_delay, day_mv1, NULL);
	day1->runAction(day_seq1);

	auto mv1 = MoveTo::create(1.0f, Vec2(sceneSize.width / 2, sceneSize.height*-0.15f));
	auto delay = DelayTime::create(1.0f);
	auto mv2 = MoveTo::create(0.3f, Vec2(sceneSize.width / 2, sceneSize.height*0.19f));
	auto seq1 = Sequence::create(mv1, delay, mv2, NULL);

	//scale
	auto delay2 = DelayTime::create(1.3f);
	auto s1 = ScaleTo::create(0.15f, 1.0f, 1.05f);
	auto s2 = ScaleTo::create(0.15f, 1.0f);
	auto delay3 = DelayTime::create(0.2f);
	auto s3 = ScaleTo::create(0.15f, 1.0f, 0.90);
	auto s4 = ScaleTo::create(0.15f, 1.0f);
	auto block1 = CallFunc::create([=](){
		khi->setSpriteFrame(cache->getSpriteFrameByName("khi_2.png"));
	});
	auto seq2 = Sequence::create(delay2, s1, s2, delay3, s3, s4, block1, NULL);

	//tum day
	auto block2 = CallFunc::create([=](){
		auto lead_move = MoveTo::create(1.2f, Vec2(0, -sceneSize.height));
		node_lead->runAction(lead_move);

		day1->removeFromParentAndCleanup(true);
		khi->setSpriteFrame(cache->getSpriteFrameByName("khi_3.png"));
		khi->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		khi->setPosition(Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.128f));

		auto p1 = MoveTo::create(0.12f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.103f));
		auto p2 = MoveTo::create(0.12f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.119f));
		auto p3 = MoveTo::create(0.8f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.109f));
		auto d1 = DelayTime::create(0.35f);
		auto p5 = MoveTo::create(0.12f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.109f));
		auto p6 = MoveTo::create(0.1f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.047f));
		auto p7 = MoveTo::create(0.1f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.038f));
		auto p8 = MoveTo::create(0.1f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*0.047f));
		auto p9 = MoveTo::create(0.1f, Vec2(sceneSize.width / 2, sceneSize.height + sceneSize.height*1.234f));
		auto seq3 = Sequence::create(p1, p2, p3, d1, p5, p6, p7, p8, p9, NULL);

		auto r1 = RotateTo::create(0.3f, -3.9);
		auto r2 = RotateTo::create(0.3f, 3.8);
		auto r3 = RotateTo::create(0.3f, 0.0);
		auto seq4 = Sequence::create(r1, r2, r3, NULL);
		auto spawn_3 = Spawn::createWithTwoActions(seq3, seq4);
		khi->runAction(spawn_3);
	});
	auto spawn = Spawn::create(seq1, seq2, NULL);
	auto seq_all = Sequence::create(spawn, block2, CallFunc::create([this] {
		
		}), NULL);

	khi->runAction(seq_all);

	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->playEffect("sounds/xep_chu/huzz_pop.mp3");
	}, 0.7f, "huzz_pop.mp3");
	this->scheduleOnce([=](float dt){
		//bungChu();
		AUDIO_ENGINE->playEffect("sounds/xep_chu/tungtung.mp3");
	}, 2.25f, "tungtung.mp3");
	this->scheduleOnce([=](float dt){
		AUDIO_ENGINE->playEffect("sounds/xep_chu/laroi_1.mp3");
		AUDIO_ENGINE->playEffect("sounds/xep_chu/laroi_2.mp3");
		AUDIO_ENGINE->playEffect("sounds/xep_chu/laroi_3.mp3");
		
	}, 2.3f, "laroi");

	if (curText == 0)
	{
		this->scheduleOnce([this](float dt) {
			AUDIO_ENGINE->playSound(_pathAudioIntro, false, [this]() {
				handSuggetionGame(8.0f);
				});
			}, 3.0, "play_sound");
	}
	else
	{
		handSuggetionGame(8.0f);
	}

}

void GameXepChu::softItem()
{
	std::vector<float> line_length;
	float leng = 0;
	int line = 0;
	for (int i = 0; i < (int)word_items.size(); i++){
		float pre_leng = leng;
		leng += word_items.at(i)->getContentSize().width;
		if (i + 1 != word_items.size()){
			if (word_items.at(i + 1)->_item_info.text != "." && word_items.at(i + 1)->_item_info.text != "?" && word_items.at(i + 1)->_item_info.text != "!"){
				leng += 26;
			}
		}
		if (leng>=Director::getInstance()->getVisibleSize().width*0.95){
			line++;
			line_length.push_back(pre_leng);
			leng = word_items.at(i)->getContentSize().width;
		}
		word_items.at(i)->line = line;
	}
	line_length.push_back(leng);

	posY.clear();
	if ( line == 0){
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2);
	}
	else if (line ==1){
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 + word_items.at(0)->text->getContentSize().height*0.75f);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 - word_items.at(0)->text->getContentSize().height*0.75f);
	}
	else if (line == 2){
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 + word_items.at(0)->text->getContentSize().height*1.5f);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 - word_items.at(0)->text->getContentSize().height * 1.5f);
	}
	else if (line == 3){
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 + word_items.at(0)->text->getContentSize().height *1.5f);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 + word_items.at(0)->text->getContentSize().height *0.5f);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 - word_items.at(0)->text->getContentSize().height*0.5f);
		posY.push_back(Director::getInstance()->getVisibleSize().height / 2 - word_items.at(0)->text->getContentSize().height *1.5f);
	}
	leng = 0;
	line = 0;
	for (int i = 0; i < (int)word_items.size(); i++){
		if (word_items.at(i)->line!=line){
			line++;
			leng = 0;
		}
		float posX = Director::getInstance()->getVisibleSize().width / 2 - line_length.at(word_items.at(i)->line) / 2 + leng + word_items.at(i)->getContentSize().width / 2;
		word_items.at(i)->setPosition(Vec2(posX, posY.at(word_items.at(i)->line)));
		leng += word_items.at(i)->getContentSize().width;
		if (i + 1 != word_items.size()){
			if (word_items.at(i + 1)->_item_info.text != "." && word_items.at(i + 1)->_item_info.text != "?" && word_items.at(i + 1)->_item_info.text != "!"){
				leng += 26;
			}
		}
	}
}

void GameXepChu::wordFlyToSky(GameXepChu_Item* aItem)
{
	aItem->stopAllActions();
	float distance_move = aItem->getPosition().distance(aItem->posFlying);
	float time = distance_move / 1500;
	aItem->runAction(Sequence::create(
		Spawn::create(MoveTo::create(time, aItem->posFlying), RotateTo::create(time, aItem->rotate_info), ScaleTo::create(time, 0.6f), nullptr)
		, CallFunc::create([=](){
		aItem->item_state = ITEM_WORD_STATE::IWS_IDLE;
	}),nullptr));
}

void GameXepChu::onItemTouch(Ref* sender, Widget::TouchEventType evt)
{
	if (game_state != GAME_BASE_STATE::GBS_PLAYING)
		return;
	if (curText > 0 && !game_play_enable &&game_state == GAME_BASE_STATE::GBS_PLAYING){
		pauseGame();
		game_state == GAME_BASE_STATE::GBS_PAUSE;
		return;
	}

	auto aItem = static_cast<GameXepChu_Item*>(sender);

	auto postStart = aItem->getPosition();
	switch (evt)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:{
														this->unschedule("delay_show_guide");
														this->unschedule("show_guide_jumble_word");
														this->removeChildByName("guide_overlay");
														if (_isPlayTextItem)
														{	
															AUDIO_ENGINE->stopSound(game_dataset.list_objects.at(curText).audio_path);
															audio_id = AUDIO_ENGINE->playEffect("sounds/xep_chu/giatgiat.mp3", true);
															aItem->onHold();
															aItem->setLocalZOrder(++curZorder);
														}

														break; 
	}
	case cocos2d::ui::Widget::TouchEventType::MOVED:{
														aItem->setPosition(aItem->getTouchMovePosition());
														break;
	}
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		handSuggetionGame();
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:{
														if (_isPlayTextItem)
														{
															AUDIO_ENGINE->stopEffect(audio_id);
															for (int i = 0; i < (int)word_items.size(); i++) {
																if (word_items.at(i)->_item_info.text.compare(aItem->_item_info.text) == 0) {
																	auto touchLayout = word_items.at(i)->text->getChildByName<Layout*>("touchPanel");
																	auto posEnd = word_items.at(i)->text->convertToNodeSpace(aItem->getTouchEndPosition());
																	auto postTouchEnd = aItem->getTouchEndPosition() - aItem->getTouchBeganPosition();

																	if (_age < 4)
																	{
																		if (abs(postTouchEnd.x) <= 15 || abs(postTouchEnd.y) <= 15)
																		{
																			aItem->setTouchEnabled(false);
																			_isPlayTextItem = false;
																			placeTrueItem(aItem, word_items.at(i), 2.0f);
																		}
																		else
																		{
																			if (touchLayout->getBoundingBox().containsPoint(posEnd)) {
																				//dung
																				_isPlayTextItem = false;
																				placeTrueItem(aItem, word_items.at(i), 0.2f);
																			}
																			else {
																				AUDIO_ENGINE->playEffect("sounds/xep_chu/drag_fail.mp3");
																				wordFlyToSky(aItem);																				
																			}
																		}
																		break;
																	}
																	else {
																		if (touchLayout->getBoundingBox().containsPoint(posEnd)) {
																			//dung
																			_isPlayTextItem = false;
																			placeTrueItem(aItem, word_items.at(i), 0.2f);
																		}
																		else {
																			AUDIO_ENGINE->playEffect("sounds/xep_chu/drag_fail.mp3");
																			wordFlyToSky(aItem);
																		}
																	}
																}
															}
														}
														   break;
	}
	default:
		break;
	}
}

void GameXepChu::placeTrueItem(GameXepChu_Item* aItem, GameXepChu_Item* trueItem, float timeDelay){
	aItem->stopAllActions();
		aItem->runAction(Sequence::create(
			Spawn::createWithTwoActions(MoveTo::create(timeDelay, trueItem->getPosition()), ScaleTo::create(0.2f, 1.0f)),
			CallFunc::create([=] {
				trueItem->onShowImage();
				aItem->removeFromParent();
				_isPlayTextItem = true;
				}),
			nullptr));
	
	for (int i = 0; i < _wordItemsCopy.size(); i++)
	{
		if (trueItem->getTextItem() == _wordItemsCopy.at(i)->getTextItem())
		{
			_wordItemsCopy.erase(_wordItemsCopy.begin() + i);
		}
	}
}

void GameXepChu::checkFinsh()
{
	int trueItem = 0;
	for (int i = 0; i < (int)word_items.size(); i++){
		if (word_items.at(i)->item_state==ITEM_WORD_STATE::IWS_TRUE_PLACE){
			trueItem++;
		}
	}
	if (trueItem==game_dataset.list_objects.at(curText).numItemFly){
		onFinishLevel();
	}
}


void GameXepChu::loadFrames() {	
	//load anim con khi o day

	auto posFlyingPanel = cocos2d::utils::findChild(this, "flyingTextPanel");
	for (int i = 1; i <= 6; i++){
		std::string item_name = StringUtils::format("%s%d", "pos", i);
		auto aPos = posFlyingPanel->getChildByName(item_name);
		posFlyingItems.push_back(aPos->getPosition());
	}

	auto imgGameName = ImageView::create("games/xep_chu/gameName.png");
	//imgGameName->setScale(4);
	imgGameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	imgGameName->setPosition(Director::getInstance()->getVisibleSize()/2);
	this->addChild(imgGameName,1000);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/xep_chu/gameName.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		imgGameName->runAction(Sequence::create(
			FadeOut::create(1.5f),
			CallFunc::create([=] {

				
				}),
			DelayTime::create(1.5),
			CallFunc::create([=]() {
					onLoadLevel();
				}),
			RemoveSelf::create(true), nullptr));
	});

    if(MJDEFAULT->getBoolForKey(key_background_music)){
        AUDIO_ENGINE->playBackgroundMusic("sounds/xep_chu/bg.mp3", 0.8f);
    }
	//onPreGame();
}

void GameXepChu::onPreGame()
{
	auto objectPanel = cocos2d::utils::findChild<Layout*>(this, "objectPanel");
	//auto khi = static_cast<Sprite*>(objectPanel->getChildByName("khi"));
	objectPanel->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(0, 0)), CallFunc::create([=](){
		AUDIO_ENGINE->playEffect("sounds/xep_chu/sentence_slice.mp3");
	}), MoveTo::create(0.2f, Vec2(45, 0))
		, MoveTo::create(0.2f, Vec2(0, 0))
		//, MoveTo::create(0.2f, Vec2(25, 0))
		//, MoveTo::create(0.2f, Vec2(0, 0))
		, DelayTime::create(0.2f)
		, CallFunc::create([=](){
		monkeyJump();
		//onStartGame();
	}),	DelayTime::create(2.25f)
		, CallFunc::create([=](){onStartGame(); })
		, nullptr));
}

void GameXepChu::onStartGame(){
	
	this->scheduleOnce([=](float dt){
		game_state = GAME_BASE_STATE::GBS_PLAYING;
	}, 2.0f, "start_game");

	for (int i = 0; i < (int)flying_items.size(); i++){
		wordFlyToSky(flying_items.at(i));
	}
	for (int i = 0; i < word_items.size(); i++){
		word_items.at(i)->setVisible(true);
	}
	
}

void GameXepChu::onFinishGame(){
	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
		//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
	}), NULL));
}

void GameXepChu::onFinishLevel()
{
	game_state = GAME_BASE_STATE::GBS_PLAY_VIDEO;
	//phat sound
	auto audio_path = std::string(game_dataset.list_objects.at(curText).audio_path);
	AUDIO_ENGINE->stopSound(audio_path);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	_isPlayAudio = false;
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
			//dien gi gi do
			_isPlayAudio = true;
			if (curText == game_dataset.number_level - 1){
				onFinishGame();
			}
			else{
				curText++;
				onLoadLevel();
			}

		}), nullptr));
	});

	for (int i = 0; i < (int)word_items.size(); i++){
		word_items.at(i)->runAction(Sequence::create(
			DelayTime::create(word_items.at(i)->_item_info.time_call)
			, CallFunc::create([=](){
			word_items.at(i)->onFinishAudio(true);
		}), DelayTime::create(word_items.at(i)->_item_info.time_end - word_items.at(i)->_item_info.time_call)
			, CallFunc::create([=](){
			word_items.at(i)->onFinishAudio(false);
		}), nullptr));
	}
}

void GameXepChu::onLoadLevel()
{
	mj::helper::shuffle(posFlyingItems);
	mj::helper::shuffle(color4bs);

	flying_items.clear();
	word_items.clear();
	_wordItemsCopy.clear();

	auto objectPanel = cocos2d::utils::findChild<Layout*>(this, "objectPanel");
	objectPanel->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));
	objectPanel->removeAllChildren();
	
	int rand_color = rand() % 2;

	for (int i = 0; i< (int)game_dataset.list_objects.at(curText).word_infos.size(); i++){
		auto aItem = GameXepChu_Item::createItem(game_dataset.list_objects.at(curText).word_infos.at(i), color4bs.at(rand_color));
		objectPanel->addChild(aItem);
		word_items.pushBack(aItem);
	}
	
	softItem();
	
	int index = 0;

	auto listSynceText = game_dataset.list_objects.at(curText)._listTextHight;
	float sumTimeDelay = 0.0;
	for (int i = 0; i < listSynceText.size(); i++)
	{
		float timeDelay = (listSynceText[i].e - listSynceText[i].s) / 1000.0f;
		_listTimeDelay.push_back(timeDelay);
		sumTimeDelay = sumTimeDelay + timeDelay;
	}
	
	_actions.clear();

	for (int i = 0; i < (int)game_dataset.list_objects.at(curText).word_infos.size(); i++){
		if (game_dataset.list_objects.at(curText).word_infos.at(i).fly){
			auto fItem = GameXepChu_Item::createItem(game_dataset.list_objects.at(curText).word_infos.at(i), color4bs.at(rand_color));
			fItem->posFlying = posFlyingItems.at(index);

			auto action1 = CallFunc::create([=] {
				fItem->setTextColor(Color4B::RED);
			});
			_actions.pushBack(action1);
			_actions.pushBack(DelayTime::create(_listTimeDelay[i]));
			auto action2 = CallFunc::create([=] {
				fItem->setTextColor(Color4B::BLACK);
			});
			_actions.pushBack(action2);

			objectPanel->addChild(fItem, 1);
			flying_items.pushBack(fItem);
			fItem->setFlyType();
			auto aItem = static_cast<GameXepChu_Item*>(word_items.at(i));
			fItem->setPosition(aItem->getPosition());
			aItem->setTextBlank();
			aItem->setVisible(false);//an di ko thi no bi vien hoi mo`

			fItem ->setContentSize(fItem->getContentSize() + Size(50, 50));
			fItem->addTouchEventListener(CC_CALLBACK_2(GameXepChu::onItemTouch,this));
			aItem->setCallback([=](){
				checkFinsh();
			});
			index++;
		}
	}
	onPreGame();
	if (curText == 0)
	{
		_actions.pushBack(DelayTime::create(sumTimeDelay));
		_actions.pushBack(CallFunc::create([=] {addSpeaker(); }));
	}

	std::string audioPath = game_dataset.list_objects.at(curText).audio_path;
	auto cc = Sequence::create(_actions);
	auto spanw = Spawn::create(CallFunc::create([audioPath] {
		AUDIO_ENGINE->playSound(audioPath);
		}), cc, NULL);
	this->runAction(Sequence::create(DelayTime::create(0.5f), spanw, NULL) );
	_wordItemsCopy = word_items;

	// run action gi do roi cho chu chay di
	//onStartGame();
	
}

#pragma callback

void GameXepChu::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = GameXepChu::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameXepChu::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void GameXepChu::showGuide(Point start, Point stop) {
	this->removeChildByName("guide_overlay");

    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setName("guide_overlay");
	this->addChild(overlay, INT_MAX - 1);
    
    auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
    overlay->addChild(hand, INT_MAX - 1);
    hand->setPosition(start - Point(0, 25));
    
    hand->runAction(Sequence::create(MoveTo::create(2, stop), RemoveSelf::create(), nullptr));
}


void GameXepChu::addSpeaker()
{
	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	auto screenSize = cocos2d::Director::getInstance()->getWinSize();
	this->addChild(speaker, 10000000);
	speaker->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	speaker->setPosition(Vec2(20, Director::getInstance()->getWinSize().height - 10));
	speaker->addClickEventListener([=](Ref* sender) {
		std::string pathAudio = game_dataset.list_objects.at(curText).audio_path;
		if (_isPlayAudio)
		{
			AUDIO_ENGINE->stopSound(pathAudio);
			AUDIO_ENGINE->playSound(pathAudio);
		}
	});
}

void GameXepChu::handSuggetionGame(float delayTime)
{
	if (_wordItemsCopy.empty())
	{
		this->removeChildByName("guide_overlay");
		this->unschedule("delay_show_guide");
		this->unschedule("show_guide_jumble_word");
		return;
	}
	this->scheduleOnce([=](float) {
		if (!_wordItemsCopy.empty())
		{
			Point start, stop;
			auto w = _wordItemsCopy.at(random(0, (int)_wordItemsCopy.size() - 1));
			for (auto f : flying_items) {
				if (w->_item_info.text == f->_item_info.text) {
					stop = w->getParent()->convertToWorldSpace(w->getPosition());
					start = f->getParent()->convertToWorldSpace(f->getPosition());
					break;
				}
			}
			runActionSuggetion(start, stop, delayTime);
		}
	}, 1, "delay_show_guide");

}


void GameXepChu::runActionSuggetion(Point start, Point stop, float timeDelay)
{
	this->schedule([this, start, stop](float) {
		auto post = start - stop;
		if (abs(post.x) <= 5 && abs(post.y) <= 5)
		{

			this->unschedule("delay_show_guide");
			this->unschedule("show_guide_jumble_word");
			this->removeChildByName("guide_overlay");
			handSuggetionGame();
			return;
		}
		this->showGuide(start, stop);
		std::string pathAudio = game_dataset.list_objects.at(curText).audio_path;
		if (_isPlayAudio)
		{
			_isRunActionHand = true;
			this->unschedule("show_guide_jumble_word");
			AUDIO_ENGINE->stopSound(pathAudio);
			AUDIO_ENGINE->playSound(pathAudio, false, [=]() {
				_isRunActionHand = false;
				runActionSuggetion(start, stop, 8.5);
				});
			if (_isRunActionHand)
			{
				runActionSuggetion(start, stop, 8.5);
			}
		}
	}, 8, 100, timeDelay, "show_guide_jumble_word");
	
}
