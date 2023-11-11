
#include "GameRabbitClassify.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "Common.h"
#include "MJDefault.h"
#include "StoryConfigManager.h"

#define speed_opacity 15
#define CSB_NAME "csb/game/rabbit_classify/GameRabbitClassify_2.csb"
#define key_show_guide_classify "key_show_guide_classify"
#define reminder_time 8

INITIALIZE_READER(GameRabbitClassify);
USING_NS_CC;
std::once_flag gamesample_reader;

GameRabbitClassify * GameRabbitClassify::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(gamesample_reader, [] {
        REGISTER_CSB_READER(GameRabbitClassify);
        REGISTER_CSB_READER(GameRabbitClassify_Item);
    });
    auto p = static_cast<GameRabbitClassify *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}


void GameRabbitClassify::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto count_guide = MJDEFAULT->getIntegerForKey(key_show_guide_classify, 0);
	auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");

//	auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
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

void GameRabbitClassify::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();

	auto list_object = value_data["content_info"]["item"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        GameRabbitClassify_Item::ItemInfo object_info;
        auto object_value = list_object[i].GetObject();
		for (int j = 0; j < (int)object_value["type"].Size(); j++){
			object_info.iType.push_back(object_value["type"][j].GetInt());
		}
        object_info.text = object_value["text"].GetString();
        object_info.audio_path = object_value["audio_path"].GetString();
        object_info.img_path = object_value["img_path"].GetString();

        game_dataset.list_objects.push_back(object_info);
    }

	auto list_type = value_data["content_info"]["box"].GetArray();
	for (int i = 0; i < (int)list_type.Size(); i++) {
		auto object_value = list_type[i].GetObject();
		std::vector<Type_Info> data_type_info;
		game_dataset.audio_mc_path.push_back(object_value["audio_mc_path"].GetString());
        //game_dataset.read_box_name = object_value.HasMember("read_box_name") ? object_value["read_box_name"].GetBool() : false;
		auto listData = object_value["data"].GetArray();
		for (int j = 0; j < (int)listData.Size(); j++){
			Type_Info object_info;
			object_info.iType = listData[j]["id"].GetInt();
			object_info.text = listData[j]["text"].GetString();
			object_info.duration = listData[j]["duration"].GetFloat();
			object_info.audio = listData[j]["audio"].GetString();
			data_type_info.push_back(object_info);
		}

		game_dataset.list_types.push_back(data_type_info);
	}
}

void GameRabbitClassify::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
    auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
    rapidjson::Document document_icons;
    document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
    auto lang = m_lang->getLanguage(document_icons[0]["language"].GetString());

	auto list_object = value_data["item"].GetArray();
	for (int i = 0; i < (int)list_object.Size(); i++) {
		GameRabbitClassify_Item::ItemInfo object_info;
		auto object_value = list_object[i].GetObject();
		for (int j = 0; j < (int)object_value["type"].Size(); j++){
			object_info.iType.push_back(object_value["type"][j].GetInt());
		}
		std::string icon_id = object_value["item_data"].GetString();
		auto dataImg = math::resource::ResHelper::getImage(images, icon_id);

		object_info.text = dataImg->getText();
		object_info.audio_path = dataImg->getAudios("name_1", lang).front();
		object_info.img_path = dataImg->path;


		game_dataset.list_objects.push_back(object_info);
	}

	// get phonic sound for type
	std::vector<Type_Info> sound_types;
	for (auto image : images) {

		if (image->isExist("name_p", lang)) {
			Type_Info info;
			info.iType = image->getComparisionValue("pair");
			//info.text = image->getText("name_p");
			info.audio = image->getAudios("name_p", lang).front();
			sound_types.push_back(info);
		}

	}
	auto list_type = value_data["box"].GetArray();
	for (int i = 0; i < (int)list_type.Size(); i++) {
		auto object_value = list_type[i].GetObject();
		std::vector<Type_Info> data_type_info;
		if (object_value.HasMember("audio_mc_path"))
		{
			std::string icon_id = object_value["audio_mc_path"].GetString();
			auto dataImg = math::resource::ResHelper::getImage(images, icon_id);
			game_dataset.audio_mc_path.push_back(dataImg->getAudio("name_1", lang));
		}
		else {
			game_dataset.audio_mc_path.push_back("");
		}
        //game_dataset.read_box_name = object_value.HasMember("read_box_name") ? object_value["read_box_name"].GetBool() : false;
		if (object_value.HasMember("data"))
		{
			auto listData = object_value["data"].GetArray();
			for (int j = 0; j < (int)listData.Size(); j++) {
				Type_Info object_info;
				object_info.iType = listData[j]["id"].GetInt();
				object_info.text = listData[j]["text"].GetString();
				object_info.duration = listData[j]["duration"].GetFloat();
				
				if (JSON_SAFE_CHECK(listData[j], "audio", String))
				{
					std::string icon_id = listData[j]["audio"].GetString();
					auto dataImg = math::resource::ResHelper::getImage(images, icon_id);
					object_info.audio = dataImg->getAudio("name_1", lang);
				}

				
				//object_info.audio = listData[j]["audio"].GetString();
				for (int k = 0; k < (int)sound_types.size(); ++k) {
					if (object_info.iType == sound_types.at(k).iType) {
						object_info.audio = sound_types.at(k).audio;
						break;
					}
				}
				data_type_info.push_back(object_info);
			}
		}
		

		game_dataset.list_types.push_back(data_type_info);
	}

    if(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
    {
        game_dataset.audio_intro = "";
    }
    else if (value_data.HasMember("instruction")) {
  		int intro_number = value_data["instruction"].GetInt();
  		game_dataset.audio_intro = StringUtils::format("sounds/rabbit_classify/intro%d.mp3", intro_number);
  	}

}

void GameRabbitClassify::onEnter(){
    ms::BaseActivity::onEnter();
    setInputConfigGame();
    loadFrames();
}

void GameRabbitClassify::setInputConfigGame(){
//    auto limit = 3;
    mj::helper::shuffle(game_dataset.list_objects);
	mj::helper::shuffle(game_dataset.list_types);//chi co game con tho co cai nay
	for (int i = 0; i < (int)game_dataset.list_types.size(); i++){
		mj::helper::shuffle(game_dataset.list_types.at(i));
	}
//    game_dataset.list_objects.erase(idle() + limit-1, game_dataset.list_objects.begin() + (int) game_dataset.list_objects.size() - 1);

	// nam fix
	std::vector<GameRabbitClassify_Item::ItemInfo> list_trung;
	for (int i = 1; i < game_dataset.list_objects.size(); i++) {
		if (game_dataset.list_objects[i].text == game_dataset.list_objects[i - 1].text) {
			list_trung.push_back(game_dataset.list_objects[i]);
			game_dataset.list_objects.erase(game_dataset.list_objects.begin() + i);
		}
	}
	if (!list_trung.empty()) {
		for (int i = 0; i < list_trung.size(); i++) {
			int kt = 1;
			int data_size = game_dataset.list_objects.size();
			for (int j = 0; j < data_size; j++) {
				if (game_dataset.list_objects[j].text != list_trung[i].text && game_dataset.list_objects[(j + 1) % data_size].text != list_trung[i].text) {
					game_dataset.list_objects.insert(game_dataset.list_objects.begin() + j, list_trung[i]);
					kt = 0;
					break;
				}
			}
			if (kt == 1) {
				game_dataset.list_objects.push_back(list_trung[i]);
			}
		}

	}
}

void GameRabbitClassify::updateRabbitClassify(float dt)
{
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
	auto bgRun = panelRun->getChildByName<cocos2d::ui::ImageView *>("bgRun");
	auto bgRun1 = panelRun->getChildByName<cocos2d::ui::ImageView *>("bgRun1");
	//if (list_items.size()>0)
	//	CCLOG("pos run = %f--%f", panelRun->convertToWorldSpace(list_items.at(0)->getPosition()).x, panelRun->convertToWorldSpace(list_items.at(0)->getPosition()).y);
	if (panelRun->convertToWorldSpace(bgRun->getPosition()).x > Director::getInstance()->getVisibleSize().width + bgRun->getContentSize().width / 2) {
		bgRun->setPositionX(bgRun1->getPositionX() - bgRun->getContentSize().width);
		//CCLOG("doi choi run");
	}
	if (panelRun->convertToWorldSpace(bgRun1->getPosition()).x > Director::getInstance()->getVisibleSize().width + bgRun->getContentSize().width / 2){
		bgRun1->setPositionX(bgRun->getPositionX() - bgRun->getContentSize().width);
		//CCLOG("doi choi run1");
	}

	if (game_state == GAME_RABBIT_CLASSIFY_STATE::GRCS_PLAYING_TIME){
		for (int i = 0; i < list_items.size(); i++){
			if (list_items.at(i)->itemState == ITEM_STATE::IS_RUNNING){
				if (panelRun->convertToWorldSpace(list_items.at(i)->getPosition()).x > Director::getInstance()->getVisibleSize().width + list_items.at(i)->getContentSize().width / 2)	{
					updatePosAItem(list_items.at(i));
				}
			}
			if (list_items.at(i)->itemState == ITEM_STATE::IS_DRAGING){
				list_items.at(i)->setPosition(panelRun->convertToNodeSpace(touchLocation));
			}
		}
	}

}

void GameRabbitClassify::updatePosItemForStart()
{
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
	for (int i = 0; i < list_items.size(); i++){
		if (i==0){
			list_items.at(i)->setPosition(Vec2(panelRun->convertToNodeSpace(Vec2(-300,0)).x,panelRun->getContentSize().height/2));
		}
		else{
			list_items.at(i)->setPosition(list_items.at(i-1)->getPosition() + Vec2(-300,0));
		}
		list_items.at(i)->setVisible(true);
		list_items.at(i)->itemState = ITEM_STATE::IS_RUNNING;
		list_items.at(i)->curPos = list_items.at(i)->getPosition();
	}
}

cocos2d::Vec2 GameRabbitClassify::getMinPos(bool isDragFail /*= false*/)
{
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
	Vec2 minPos = Director::getInstance()->getVisibleSize();
	minPos.y = panelRun->getContentSize().height / 2;

	int count_item = 0;
	for (int i = 0; i < list_items.size(); i++){
		if (list_items.at(i)->itemState == ITEM_STATE::IS_RUNNING || list_items.at(i)->itemState == ITEM_STATE::IS_COMMINGBACK){
			count_item++;
		}
	}
	if (count_item<=3 && isDragFail){
		int minX = panelRun->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 300, 0)).x;
		int maxX = panelRun->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleSize().width / 2 + 300, 0)).x;
		bool gotObject = false;
		for (int i = 0; i < list_items.size(); i++){
			if (list_items.at(i)->curPos.x >minX && list_items.at(i)->curPos.x < maxX && list_items.at(i)->itemState != ITEM_STATE::IS_COMMINGBACK)
				gotObject = true;
		}
		if (gotObject){
			return getMinPos();
		}
		else{
			minPos = panelRun->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleSize().width/2, panelRun->getContentSize().height / 2));
		}
	}
	else{
		for (int i = 0; i < list_items.size(); i++){
			if (list_items.at(i)->curPos.x < minPos.x && (list_items.at(i)->itemState == ITEM_STATE::IS_RUNNING || list_items.at(i)->itemState == ITEM_STATE::IS_COMMINGBACK))
				minPos = list_items.at(i)->curPos;
		}
		if (panelRun->convertToWorldSpace(minPos).x < 0){
			minPos.x -= 300;
		}
		else{
			minPos = Vec2(panelRun->convertToNodeSpace(Vec2(-300, 0)).x, panelRun->getContentSize().height / 2);
		}
	}
	return minPos;
}

void GameRabbitClassify::updatePosAItem(GameRabbitClassify_Item* aItem)
{
	auto minPos = getMinPos();
	aItem->setPosition(minPos);
	aItem->curPos = minPos;
}

void GameRabbitClassify::onItemTouch(Ref* aRef, Widget::TouchEventType evt)
{
	if (timeToPause==2 && !game_play_enable){ //&& cai bien gi do o day
		if (game_state == GAME_RABBIT_CLASSIFY_STATE::GRCS_PLAYING_TIME){
			pauseGame();
			game_state = GAME_RABBIT_CLASSIFY_STATE::GRCS_PAUSE_TIME;
		}
		return;
	}
	//auto aitem = static_cast<cocos2d::ui::ImageView*>(aRef);
	auto aitem = static_cast<GameRabbitClassify_Item*>(aRef);
	if (evt == Widget::TouchEventType::BEGAN){
		unschedule("show_guide_rabbit_classify");
		_isGuideShowing = true;
		touchLocation = aitem->getTouchBeganPosition();
		aitem->itemState = ITEM_STATE::IS_DRAGING;
		//phat tieng
		aitem->setLocalZOrder(++curZ);
	}
	else if (evt == Widget::TouchEventType::MOVED){
		_isGuideShowing = true;
		touchLocation = aitem->getTouchMovePosition();
        aitem->itemState = ITEM_STATE::IS_DRAGING;
	}
	else if (evt == Widget::TouchEventType::ENDED || evt == Widget::TouchEventType::CANCELED){
		showGuideGame();

		for (int i = 0; i < list_boxs.size(); i++){
			if (list_boxs.at(i)->getBoundingBox().containsPoint(list_boxs.at(i)->getParent()->convertToNodeSpace(aitem->getTouchEndPosition()))){
				bool isTrueMove = false;
				for (int j = 0; j < (int)aitem->_item_info.iType.size(); j++){
					if (aitem->_item_info.iType.at(j) == list_boxs.at(i)->getTag()){
						isTrueMove = true;
						break;
					}
				}
				if (isTrueMove){//move dung cho
					_item = static_cast<GameRabbitClassify_Item*>(aRef);
					aitem->itemState = ITEM_STATE::IS_REMOVING;
					auto winItem = static_cast<GameRabbitClassify_Item*>(aitem->clone());
					aitem->setVisible(false);
					list_boxs.at(i)->addChild(winItem, 100);
					//this->addChild(winItem,10);
					winItem->setPosition(list_boxs.at(i)->convertToNodeSpace(aitem->getTouchEndPosition()));
					winItem->setTouchEnabled(false);

					placeTrueItem(winItem, list_boxs.at(i));
				}
				else{
					//place false
					playBunny(BUNNY_STATE::BS_LOSE);
					AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_wrong.mp3");

				}
			}
		}
		if (aitem->itemState != ITEM_STATE::IS_REMOVING){
			AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_wrong_back.mp3");
			aitem->itemState = ITEM_STATE::IS_COMMINGBACK;
			auto minPos = aitem->curPos;//getMinPos(true);
			auto curDragPos = aitem->getPosition();
			auto move_distance = curDragPos.distance(minPos);
			float time_move = move_distance / 3000;
			aitem->curPos = minPos;
			aitem->runAction(Sequence::create(MoveTo::create(time_move, minPos), CallFunc::create([=](){aitem->itemState = ITEM_STATE::IS_RUNNING; }), nullptr));
		}

        //inActiveMode();

		touchLocation = cocos2d::Vec2(0, 0);
	}
}

void GameRabbitClassify::placeTrueItem(GameRabbitClassify_Item* aItem, Layout* boxContain)
{
	timeToPause++;
	playBunny(BUNNY_STATE::BS_WIN);

	auto posWin = boxContain->convertToNodeSpace(boxContain->getParent()->getChildByName("posWin")->getPosition());
	auto posEff = boxContain->convertToWorldSpace(boxContain->getChildByName("posEff")->getPosition());
	auto winEff = boxContain->getParent()->getChildByName<GAFWrapper*>("winEff");
	winEff->setPosition(posEff);


	aItem->runAction(Sequence::create(
		MoveTo::create(0.1f, posWin)
		, CallFunc::create([=](){
		AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_correct.mp3");
		aItem->setLocalZOrder(50);
		})
		, DelayTime::create(0.1f)
		, CallFunc::create([=](){
		this->runAction(Sequence::create(
			DelayTime::create(0.3f)
			, CallFunc::create([=](){
			auto scaleX = boxContain->getScaleX();
			boxContain->runAction(Sequence::create(
				ScaleTo::create(0.2f, scaleX, 1.2f)
				, ScaleTo::create(0.1f, scaleX, 0.9f)
				, ScaleTo::create(0.1f, scaleX, 1.1f)
				, ScaleTo::create(0.1f, scaleX, 0.95f)
				,CallFunc::create([=]() {
					if (auto boxName = cocos2d::utils::findChild<cocos2d::ui::Text*>(boxContain->getParent(), "boxName")) {
						if (boxName->getString() != "") {
							boxName->runAction(Sequence::create(
								CallFunc::create([=]() {
									boxName->setTextColor(Color4B::RED);
								 }),
								DelayTime::create(1.0f),
								CallFunc::create([=]() {
									boxName->setTextColor(Color4B::BLACK);
								})
								, NULL));
						};
					}
					cocos2d::experimental::AudioEngine::play2d((_item->_item_info.audio_path));

				})
				, ScaleTo::create(0.1f, scaleX, 1), nullptr));

			winEff->setVisible(true);
			winEff->playSequence("run", false, [=](gaf::GAFObject* obj, const std::string& sequenceName){
				winEff->setVisible(false);
			});
		}), nullptr));
		})
		, MoveBy::create(0.5f, Vec2(0, -Director::getInstance()->getVisibleSize().height / 2))
		, DelayTime::create(0.4f)
		, CallFunc::create([=](){
		int countLightOn = 0;
		for (int i = 0; i < list_lights.size(); i++){
			if (!list_lights.at(i)->isOn){
				list_lights.at(i)->onLightOn();
				auto nodePosOn = cocos2d::utils::findChild(this, "nodePosOn");
				posLightOn = nodePosOn->getPositionY();
				list_lights.at(i)->runAction(MoveTo::create(0.2f, Vec2(list_lights.at(i)->getPositionX(), posLightOn)));
				AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_light_on.mp3");
				countLightOn++;
				break;
			}
			else
				countLightOn++;
		}
		if (countLightOn == list_lights.size()){
			this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
				//onFinishGame();
				onFinishLevel();
			}), nullptr));
		}
		})
		, nullptr));


}


void GameRabbitClassify::playBunny(BUNNY_STATE bs)
{
	std::string sequence_name = "idle";
	switch (bs)
	{
	case BS_IDLE:{
					 bunny->playSequence(sequence_name, true);
					 return;
	}
	case BS_WIN:{
					sequence_name = "win";
					break;
	}
	case BS_LOSE:{
					 sequence_name = "lose";
					 break;
	}
	default:
		break;
	}
	bunny->playSequence(sequence_name, false, [=](gaf::GAFObject *obj, std::string name_sequence){
		playBunny(BUNNY_STATE::BS_IDLE);
	});
}

void GameRabbitClassify::loadFrames() {
	bunny = GAFWrapper::createGAF("gaf/rabbit_classify/rabbit/rabbit.gaf");
	cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelBot")->addChild(bunny);
	auto pos_1 = cocos2d::utils::findChild(this, "posBunny")->getPosition();
	bunny->setScale(0.8f);
	bunny->setPosition(pos_1);
	playBunny(BUNNY_STATE::BS_IDLE);
	//explosive->play(false, [=](gaf::GAFObject *obj){
		//callback finish
	//});
	auto bg = cocos2d::utils::findChild<cocos2d::ui::ImageView*>(this, "bg");
	if (Director::getInstance()->getVisibleSize().width > bg->getContentSize().width){
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	}
	auto bgbot = cocos2d::utils::findChild(this, "bgbot");//set zorder de cho no len tren con tho?
	bgbot->setLocalZOrder(100);
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
	panelRun->setLocalZOrder(101);

	auto topPanel = cocos2d::utils::findChild(this, "topPanel");
	topPanelPosY = topPanel->getPositionY();

	// add speaker
	auto speaker = ui::Button::create("games/rabbit_classify/loa icon.png");
	speaker->setPosition(Vec2(40, Director::getInstance()->getVisibleSize().height - 40));
	speaker->addClickEventListener([=](cocos2d::Ref* sender) {

		for (int i = 0; i < (int)game_dataset.list_types.at(curLevel).size(); i++) {
			AUDIO_ENGINE->stopSound(game_dataset.list_types.at(curLevel).at(i).audio);
		}
		if (!_isSpeaking) {
			_isSpeaking = true;
			AUDIO_ENGINE->stopSound(_reminderAudio);
			AUDIO_ENGINE->playSound(_reminderAudio, false,
				[=]() {
					_isSpeaking = false;
					unschedule("show_guide_rabbit_classify");
					this->scheduleOnce([=](float) {
						_isGuideShowing = false;
						showGuideGame();
						}, reminder_time, "show_guide_rabbit_classify");
				});

		}
		
		});
	speaker->setTouchEnabled(false);
	speaker->setName("loa");
	this->addChild(speaker, INT_MAX-1);
	onLoadLevel();
	onPreGame();
}

void GameRabbitClassify::onPreGame()
{
	//AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_pregame_2box.mp3");
	auto panelBot = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelBot");
	//panelRun->runAction(runBangChuyen);
	panelBot->runAction(Sequence::create(
		EaseOut::create(MoveTo::create(1.0f, Vec2(0, 0)),3)
		, CallFunc::create([=](){
		auto panelRun = panelBot->getChildByName<cocos2d::ui::Widget*>("panelRun");
		auto imgTitle = panelRun->getChildByName<cocos2d::ui::ImageView *>("imgTitle");
       Vector<FiniteTimeAction*> actions;
        if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
            imgTitle->setVisible(false);
            actions.pushBack(CallFunc::create([=](){
                auto audio_path = std::string("sounds/rabbit_classify/rabbit_classify_bg_maychay_landau.mp3");
                auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                        AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_bg_maychay.mp3", true);
                    }), nullptr));
                });
            }));
            actions.pushBack(CallFunc::create([=]() {
                auto moveBy = MoveBy::create(3.0f, Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));
                auto runBangChuyen = RepeatForever::create(moveBy);
                panelRun->runAction(runBangChuyen);
                AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=]{
                    onStartGame();
                });
            }));
        }else{
            imgTitle->setContentSize(imgTitle->getVirtualRendererSize());
            auto actionMoveIn = EaseOut::create(MoveBy::create(0.8f, Vec2(Director::getInstance()->getVisibleSize().width / 2 + imgTitle->getContentSize().width / 2, 0)), 2);
            auto actionMoveOut = EaseIn::create(MoveBy::create(0.8f, Vec2(Director::getInstance()->getVisibleSize().width / 2 + imgTitle->getContentSize().width / 2, 0)), 2);
            AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_bg_maychay_landau.mp3");
            actions.pushBack(actionMoveIn);
            actions.pushBack(CallFunc::create([=](){
                if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false)){
                }else{
                    AUDIO_ENGINE->playEffect("sounds/rabbit_classify/gameName.mp3");
                }
            }));
            actions.pushBack(DelayTime::create(2.0f));
            actions.pushBack(CallFunc::create([=](){
                auto audio_path = std::string("sounds/rabbit_classify/rabbit_classify_bg_maychay_landau.mp3");
                auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                        AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_bg_maychay.mp3", true);
                    }), nullptr));
                });
            }));
            actions.pushBack(actionMoveOut);
            actions.pushBack(CallFunc::create([=]() {
                auto imgTitle = panelRun->getChildByName<cocos2d::ui::Widget *>("imgTitle");
                imgTitle->removeFromParent();
                auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
                panelRun->stopAllActions();
                auto moveBy = MoveBy::create(5.0f, Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));
                auto runBangChuyen = RepeatForever::create(moveBy);
                panelRun->runAction(runBangChuyen);
                AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=]{
                    onStartGame();
                });
            }));
        }

        panelRun->runAction(Sequence::create(actions));

//        panelRun->runAction(Sequence::create(actionMoveIn
//            , CallFunc::create([=](){
//            if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false) || CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()){
//            }else{
//                AUDIO_ENGINE->playEffect("sounds/rabbit_classify/gameName.mp3");
//            }
//        })
//            , DelayTime::create(2.0f)
//            , CallFunc::create([=](){
//            auto audio_path = std::string("sounds/rabbit_classify/rabbit_classify_bg_maychay_landau.mp3");
//            auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
//            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
//                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
//                    AUDIO_ENGINE->playEffect("sounds/rabbit_classify/rabbit_classify_bg_maychay.mp3", true);
//                }), nullptr));
//            });
//        })
//            , actionMoveOut
//            , CallFunc::create([=]() {
//            auto imgTitle = panelRun->getChildByName<cocos2d::ui::Widget *>("imgTitle");
//            imgTitle->removeFromParent();
//            auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
//            panelRun->stopAllActions();
//            auto moveBy = MoveBy::create(3.0f, Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));
//            auto runBangChuyen = RepeatForever::create(moveBy);
//            panelRun->runAction(runBangChuyen);
//            AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=]{
//                onStartGame();
//            });
//
//            /*auto audio_pre = AUDIO_ENGINE->playEffect(game_dataset.audio_intro);
//            cocos2d::experimental::AudioEngine::setFinishCallback(audio_pre, [=](int au_id, std::string path){
//                onStartGame();
//            });*/
//        }), nullptr));
	}), nullptr));
	this->schedule(schedule_selector(GameRabbitClassify::updateRabbitClassify), 0);
}

void GameRabbitClassify::onStartGame(){
	game_state = GAME_RABBIT_CLASSIFY_STATE::GRCS_PLAYING_TIME;
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget*>(this, "panelRun");

    if (curLevel==0){
        panelRun->stopAllActions();
        auto moveBy = MoveBy::create(5.0f, Vec2(Director::getInstance()->getVisibleSize().width / 2, 0));
        auto runBangChuyen = RepeatForever::create(moveBy);
        panelRun->runAction(runBangChuyen);
    }

	auto topPanel = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "topPanel");
	topPanel->runAction(MoveTo::create(0.5f, Vec2(topPanel->getPositionX(), Director::getInstance()->getVisibleSize().height)));

	for (int i = 0; i < (int)game_dataset.list_types.at(curLevel).size(); i++){
		std::string box_name = cocos2d::StringUtils::format("%s%d", "box", (i + 1));
		auto itemBox = cocos2d::utils::findChild(this, box_name);
		auto boxContain = itemBox->getChildByName<cocos2d::ui::Layout*>("boxContain");
		auto boxName = itemBox->getChildByName<cocos2d::ui::Text*>("boxName");

		
        boxContain->setUserData(new string(game_dataset.list_types.at(curLevel).at(i).audio));
        boxContain->addClickEventListener([=](Ref * sender){
			
			_isGuideShowing = true;
			if (!_isSpeaking) {
				_isSpeaking = true;
				if (boxName->getString() != "") {
					boxName->runAction(Sequence::create(
						CallFunc::create([=]() {
							boxName->setTextColor(Color4B::RED);
							}),
						DelayTime::create(1.0f),
						CallFunc::create([=]() {
							boxName->setTextColor(Color4B::BLACK);
						})
						, NULL));
					auto scaleX = boxContain->getScaleX();
					boxContain->runAction(Sequence::create(
						ScaleTo::create(0.2f, scaleX, 1.2f)
						, ScaleTo::create(0.1f, scaleX, 0.9f)
						, ScaleTo::create(0.1f, scaleX, 1.1f)
						, ScaleTo::create(0.1f, scaleX, 0.95f)
						, ScaleTo::create(0.1f, scaleX, 1)
						,nullptr));
								
				};
				auto path = (std::string*)boxContain->getUserData();
				if (*path != "") {
					auto audio_id = cocos2d::experimental::AudioEngine::play2d(*path);
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
						unschedule("show_guide_rabbit_classify");
						this->scheduleOnce([=](float) {
							_isGuideShowing = false;
							showGuideGame();
							}, reminder_time, "show_guide_rabbit_classify");

						_isSpeaking = false;
						});
				}
				else {
					_isSpeaking = false;
				}
			}
        });
        boxContain->setTouchEnabled(false);
        
		boxName->runAction(Sequence::create(
			DelayTime::create(game_dataset.list_types.at(curLevel).at(i).duration)
			, CallFunc::create([=](){
            //AUDIO_ENGINE->playSound(game_dataset.list_types.at(curLevel).at(i).audio);
            
			boxName->setOpacity(0);
			boxName->setLocalZOrder(100);
			auto scaleX = boxContain->getScaleX();
			boxContain->runAction(Sequence::create(
				ScaleTo::create(0.2f, scaleX, 1.2f)
				, ScaleTo::create(0.1f, scaleX, 0.9f)
				, ScaleTo::create(0.1f, scaleX, 1.1f)
				, ScaleTo::create(0.1f, scaleX, 0.95f)
				, ScaleTo::create(0.1f, scaleX, 1), nullptr));
				})
			, FadeIn::create(0.5f)
			, nullptr));

        
	}
	auto audio_path = std::string(game_dataset.audio_mc_path.at(curLevel));
	AUDIO_ENGINE->playSound(audio_path, false,[=]{
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
            
            if(MJDEFAULT->getBoolForKey(key_background_music)){
                AUDIO_ENGINE->playBackgroundMusic("sounds/rabbit_classify/rabbit_classify_bg_music.mp3");
            }
            if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
                utils::findChild<Button*>(this, "loa")->setTouchEnabled(true);
            }

			utils::findChild<Button*>(this, "loa")->setTouchEnabled(true);
        
            for (auto box : list_boxs){
                box->setTouchEnabled(true);
            }
     
               //inActiveMode();

		}), nullptr));
		updatePosItemForStart();
		this->scheduleOnce ([=](float) {
			showGuideGame();
		}, reminder_time, "show_guide_rabbit_classify");
	});



}

void GameRabbitClassify::onFinishGame(){
	hideTapGuideGame();
    AUDIO_ENGINE->stopAllEffects();
	bunny->setVisible(false);
	for (int i = 0; i < list_boxs.size(); i++){
		list_boxs.at(i)->setVisible(false);
	}
    this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=] {
       // _activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
        if (CONFIG_MANAGER->isVietnameseStories()) {
            this->onCloseGameVmonkey();
        }else{
            commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/rabbit_classify/rabit_classcify_standalone/rabit_classcify_standalone.gaf", true);
        }
    }), NULL));
}

void GameRabbitClassify::onFinishLevel()
{
    if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
        utils::findChild<Button*>(this, "loa")->setTouchEnabled(false);
    }

	curLevel++;

	for (int i = 0; i < (int)list_boxs.size(); i++){
		auto boxTo = static_cast<Layout*>(list_boxs.at(i)->getParent());
		boxTo->setCascadeOpacityEnabled(false);
		auto boxName = boxTo->getChildByName<cocos2d::ui::Text*>("boxName");
		boxName->setCascadeOpacityEnabled(false);
		CCLOG("fadeout");
		//boxName->setOpacity(0);
		//boxName->runAction(FadeOut::create(2.0f));
		boxName->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([=](){boxName->setLocalZOrder(-1); }), nullptr));
	}

	auto topPanel = cocos2d::utils::findChild(this, "topPanel");
	//if (topPanel->getPositionY<= Director::getInstance()->getVisibleSize().height)
	topPanel->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, topPanelPosY - Director::getInstance()->getVisibleSize().height))
		,CallFunc::create([=](){
		for (int i = 0; i < (int)numLight; i++){
			list_lights.at(i)->removeFromParent();
		}
		list_items.clear();
		list_lights.clear();
		if (curLevel < game_dataset.list_types.size()){
			onLoadLevel();
			AUDIO_ENGINE->stopBackgroundMusic();
			AUDIO_ENGINE->playSound(game_dataset.audio_intro, false, [=] {
				this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
					//AUDIO_ENGINE->playEffect(game_dataset.audio_mc_path.at(curLevel));
					onStartGame();
				}), nullptr));
			});
			//auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.audio_intro);
			//cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
			//	this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
			//		//AUDIO_ENGINE->playEffect(game_dataset.audio_mc_path.at(curLevel));
			//		onStartGame();
			//	}), nullptr));
			//});
			this->runAction(Sequence::create(DelayTime::create(2.0f)
				, CallFunc::create([=](){
				for (int i = 0; i < (int)list_boxs.size(); i++){
                    list_boxs.at(i)->getParent()->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -Director::getInstance()->getVisibleSize().height)), MoveBy::create(0.5f, Vec2(0, Director::getInstance()->getVisibleSize().height)), nullptr));
				}
			}), nullptr));
		}
		else
			onFinishGame();
	})	,nullptr));

}

void GameRabbitClassify::onLoadLevel()
{
	auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget *>(this, "panelRun");
	auto topPanel = cocos2d::utils::findChild(this, "topPanel");
	//get pos Y of light
	auto nodePosOff = cocos2d::utils::findChild(this, "nodePosOff");
	posLightOff = nodePosOff->getPositionY();

	numLight = 0;

	std::vector<int> curLevelIdBoxs;
	for (int i = 0; i < (int)game_dataset.list_types.at(curLevel).size(); i++){
		int type_this = game_dataset.list_types.at(curLevel).at(i).iType;
		curLevelIdBoxs.push_back(type_this);
		for (int j = 0; j < game_dataset.list_objects.size(); j++){
			for (int k = 0; k < game_dataset.list_objects.at(j).iType.size(); k++){
				if (game_dataset.list_objects.at(j).iType.at(k)==type_this){
					numLight++;
					break;
				}
			}
		}
	}

    /*if (!_is_guide_showed) {
        numLight--;
    }*/

	//create item and light
	int index_light = 0;
	for (int i = 0; i < game_dataset.list_objects.size(); i++){
		bool isThisLevelItem = false;
		for (int j = 0; j < (int)curLevelIdBoxs.size(); j++){
			for (int k = 0; k < (int)game_dataset.list_objects.at(i).iType.size(); k++)	{
				if (game_dataset.list_objects.at(i).iType.at(k) == curLevelIdBoxs.at(j)){
					isThisLevelItem = true;
					break;
				}
			}

		}

		GameRabbitClassify_Item* aItem = GameRabbitClassify_Item::createGame(game_dataset.list_objects.at(i));
		aItem->setVisible(false);
		panelRun->addChild(aItem, 10);
		list_items.pushBack(aItem);
		aItem->setScale(1.5f);
		aItem->setTouchEnabled(true);
		aItem->addTouchEventListener(CC_CALLBACK_2(GameRabbitClassify::onItemTouch, this));
		//tao den
		if (!isThisLevelItem)
			continue;
		GameRabbitClassify_Light* aLight = GameRabbitClassify_Light::createItem();
		//aLight->setVisible(false);
		topPanel->addChild(aLight);
		list_lights.pushBack(aLight);
		//set pos of light
		aLight->setPositionY(posLightOff);
		if (numLight % 2 == 0){
			if (index_light < numLight / 2){
				aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - (numLight / 2 - index_light - 0.5f)*aLight->getContentSize().width*1.5f);
			}
			else{
				aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 + (index_light - numLight / 2 + 0.5f)*aLight->getContentSize().width*1.5f);
			}
		}
		else{
			if (index_light < numLight / 2){
				aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - (numLight / 2 - index_light)*aLight->getContentSize().width*1.5f);
			}
			else if (index_light == numLight / 2){
				aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
			}
			else{
				aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 + (index_light - numLight / 2)*aLight->getContentSize().width*1.5f);
			}
		}
		index_light++;
	}

    //if (!_is_guide_showed) {
    //    //them 1 light
    //    GameRabbitClassify_Light* aLight = GameRabbitClassify_Light::createItem();
    //    //aLight->setVisible(false);
    //    topPanel->addChild(aLight);
    //    list_lights.pushBack(aLight);
    //    //set pos of light
    //    aLight->setPositionY(posLightOff);
    //    if (numLight % 2 == 0){
    //        if (index_light < numLight / 2){
    //            aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - (numLight / 2 - index_light - 0.5f)*aLight->getContentSize().width*1.5f);
    //        }
    //        else{
    //            aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 + (index_light - numLight / 2 + 0.5f)*aLight->getContentSize().width*1.5f);
    //        }
    //    }
    //    else{
    //        if (index_light < numLight / 2){
    //            aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - (numLight / 2 - index_light)*aLight->getContentSize().width*1.5f);
    //        }
    //        else if (index_light == numLight / 2){
    //            aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2);
    //        }
    //        else{
    //            aLight->setPositionX(Director::getInstance()->getVisibleSize().width / 2 + (index_light - numLight / 2)*aLight->getContentSize().width*1.5f);
    //        }
    //    }
    //    index_light++;
    //}

	//inlude box into list
	list_boxs.clear();
	int minFontSize = 100;
	for (int i = 0; i < game_dataset.list_types.at(curLevel).size(); i++){
		std::string box_name = cocos2d::StringUtils::format("%s%d", "box", (i + 1));
		auto itemBox = cocos2d::utils::findChild(this, box_name);
		auto boxContain = itemBox->getChildByName<Layout*>("boxContain");
		boxContain->getChildByName("than")->setLocalZOrder(90);
		auto boxName = itemBox->getChildByName<cocos2d::ui::Text*>("boxName");
		//boxName->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=](){
        boxName->setString(game_dataset.list_types.at(curLevel).at(i).text);
        while (boxName->getContentSize().width>245){
            boxName->setFontSize(boxName->getFontSize() - 1);
        }
        while (boxName->getContentSize().width < 245){
            boxName->setFontSize(boxName->getFontSize() + 1);
            if (boxName->getFontSize() > 100)
                break;
        }
//        boxName->setFontSize(100);
	//	}), nullptr));
		if (boxName->getFontSize() < minFontSize)
			minFontSize = boxName->getFontSize();
		boxContain->setTag(game_dataset.list_types.at(curLevel).at(i).iType);
		list_boxs.pushBack(boxContain);

		if (!itemBox->getChildByName("winEff")){
			auto winEff = GAFWrapper::createGAF("gaf/rabbit_classify/effect_win/effect_win.gaf");
			itemBox->addChild(winEff, 1000, "winEff");
			winEff->setVisible(false);
		}
		//CCLOG("pos eff = %f-%f", posEff.x, posEff.y);
	}
	for (int i = 0; i < game_dataset.list_types.at(curLevel).size(); i++){
		std::string box_name = cocos2d::StringUtils::format("%s%d", "box", (i + 1));
		auto itemBox = cocos2d::utils::findChild(this, box_name);
		auto boxContain = itemBox->getChildByName<Layout*>("boxContain");
		boxContain->getChildByName("than")->setLocalZOrder(90);
		auto boxName = itemBox->getChildByName<cocos2d::ui::Text*>("boxName");
		boxName->setFontSize(minFontSize);
	}
	if (list_boxs.size()==3){
		_reminderAudio = "sounds/rabbit_classify/Reminder2Box.mp3";
		bunny->setVisible(false);
		auto itemBox = cocos2d::utils::findChild(this, "box3");
		itemBox->setVisible(true);
		cocos2d::utils::findChild(this, "box2")->setVisible(true);
		cocos2d::utils::findChild(this, "box1")->setVisible(true);
	}
	else if (list_boxs.size() == 2){
		_reminderAudio = "sounds/rabbit_classify/Reminder2Box.mp3";
		auto itemBox = cocos2d::utils::findChild(this, "box3");
		itemBox->setVisible(false);
		cocos2d::utils::findChild(this, "box2")->setVisible(true);
		cocos2d::utils::findChild(this, "box1")->setVisible(true);
		bunny->setVisible(true);
	}
	else if (list_boxs.size() == 1){
		_reminderAudio = "sounds/rabbit_classify/Reminder1Box.mp3";
		cocos2d::utils::findChild(this, "box3")->setVisible(false);
		cocos2d::utils::findChild(this, "box2")->setVisible(false);
		auto box1 = cocos2d::utils::findChild(this, "box1");
		box1->setVisible(true);

		auto boxName = box1->getChildByName<Layout*>("boxName");
		auto posWin = box1->getChildByName<Layout*>("posWin");

		Layout* box1Lay = box1->getChildByName<Layout*>("boxContain");
		box1Lay->setPositionX(Director::getInstance()->getVisibleSize().width / 2 - box1Lay->getContentSize().width*0.6f);
		boxName->setPositionX(box1Lay->getPositionX() - 15);
		posWin->setPositionX(box1Lay->getPositionX() + 13);

		bunny->setVisible(true);
		bunny->setPositionX(Director::getInstance()->getVisibleSize().width / 2 + box1Lay->getContentSize().width*0.6f);
	}
}

#pragma callback
ui::Widget::ccWidgetClickCallback GameRabbitClassify::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(GameRabbitClassify::onRefresh, this);
    }
    return nullptr;
}

void GameRabbitClassify::onRefresh(cocos2d::Ref *sender) {
   /* this->removeFromParent();
    auto phonics = GameRabbitClassify::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
}

void GameRabbitClassify::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
    if(Director::getInstance()->getRunningScene()->getChildByName("hand_guide")){
           Director::getInstance()->getRunningScene()->removeChildByName("hand_guide");
       }
	hideTapGuideGame();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
}

void GameRabbitClassify::hideTapGuideGame(){
	_isSpeaking = true;
	_isGuideShowing = true;
	unschedule("show_guide_rabbit_classify");
	Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
}
void GameRabbitClassify::showGuideGame() {
	if (_isGuideShowing) {
		unschedule("show_guide_rabbit_classify");
		this->scheduleOnce([=](float) {
			_isGuideShowing = false;
			showGuideGame();
		}, reminder_time, "show_guide_rabbit_classify");
	}
	else {
		if (_isSpeaking) {
			unschedule("show_guide_rabbit_classify");
			this->scheduleOnce([=](float) {
				_isGuideShowing = false;
				showGuideGame();
				}, reminder_time, "show_guide_rabbit_classify");
		}
		else {
			_isSpeaking = true;
			auto audio_id = cocos2d::experimental::AudioEngine::play2d(_reminderAudio);
			cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {
				_isSpeaking = false;
				if (!(_countGuide < 3)) {
					this->scheduleOnce([=](float) {
						_isGuideShowing = false;
						showGuideGame();
						}, reminder_time + 2, "show_guide_rabbit_classify");
				}
				});
			if (_countGuide < 3) {
				_countGuide++;
				_isGuideShowing = true;
				Point _start;
				Point _stop;
				bool flag = false;
				for (int i = 0; i < list_items.size(); i++)
				{
					for (int j = 0; j < list_boxs.size(); j++) {
						if (list_items.at(i)->isVisible() && list_items.at(i)->_item_info.iType.at(0) == list_boxs.at(j)->getTag()) {
							if (list_items.at(i)->getParent()->convertToWorldSpace(list_items.at(i)->curPos).x > 0 && list_items.at(i)->getParent()->convertToWorldSpace(list_items.at(i)->curPos).x < Director::getInstance()->getVisibleSize().width) {
								_start = list_items.at(i)->getParent()->convertToWorldSpace(list_items.at(i)->curPos);
								_stop = list_boxs.at(j)->getParent()->convertToWorldSpace(list_boxs.at(j)->getPosition());
								flag = true;
								break;
							}
						}

					}
					if (flag == true) break;
				}

				Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");

				auto overlay = ui::Layout::create();
				overlay->setContentSize(Director::getInstance()->getVisibleSize());
				overlay->setName("guide_overlay");
				Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);

				auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
				overlay->addChild(hand, INT_MAX - 1);
				hand->setPosition(_start);
				auto panelRun = cocos2d::utils::findChild<cocos2d::ui::Widget*>(this, "panelRun");
				panelRun->pauseSchedulerAndActions();
				hand->runAction(Sequence::create(
					CallFunc::create([=]() {
						for (auto item : list_items) {
							item->setTouchEnabled(false);
						}
						}),
					DelayTime::create(0.5),
							MoveTo::create(1, _stop)
							, DelayTime::create(0.5),
							RemoveSelf::create(),
							CallFunc::create([=]() {
							panelRun->resumeSchedulerAndActions();
							this->scheduleOnce([=](float) {
								_isGuideShowing = false;
								showGuideGame();
								}, reminder_time, "show_guide_rabbit_classify");

							for (auto item : list_items) {
								item->setTouchEnabled(true);
							}

								}),
							nullptr
									));
			}
		}
		
	}	
}
//auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.audio_mc_path.at(curLevel));
//cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int, std::string) {});
