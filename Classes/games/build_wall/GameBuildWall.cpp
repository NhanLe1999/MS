
#include "GameBuildWall.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJBaseActivity.h"
#include "MJDefault.h"
#include "MsHelperGame.h"

INITIALIZE_READER(MJGameBuildWall);

USING_NS_CC;
std::once_flag buildwall_reader;
#define CSB_NAME "csb/game/build_wall/GameBuildWall.csb"

MJGameBuildWall * MJGameBuildWall::createGame(std::vector<math::resource::Image*> images) {
    std::call_once(buildwall_reader, [] {
        REGISTER_CSB_READER(MJGameBuildWall);
        REGISTER_CSB_READER(MJGameBuildWall_Item);
        REGISTER_CSB_READER(MJGameBuildWall_Crocodile);
    });
    auto p = static_cast<MJGameBuildWall *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(images);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}


#define key_count_guide_build_wall "key_count_guide_build_wall"

void MJGameBuildWall::didLoadFromCSB(std::vector<math::resource::Image*> images) {
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_build_wall, 0);
    _is_guide_showed = !(count < 3);
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guide_build_wall, count + 1);
    }, 20, "delay_update_count_guide_build_wall");
    
    auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/monkey_buildwall.plist");
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

void MJGameBuildWall::generateData(rapidjson::Value value_data){
    auto game_info = value_data["game_info"].GetObject();
    game_dataset.game_id = game_info["id"].GetInt();
    game_dataset.title = game_info["title"].GetString();
    game_dataset.intro = game_info["intro"].GetString();
    game_dataset.audio_intro = game_info["audio_intro"].GetString();
    game_dataset.audio_title = game_info["audio_title"].GetString();
    
    auto list_object = value_data["content_info"].GetArray();
    for (int i = 0; i < (int) list_object.Size(); i++) {
        MJGameBuildWall_Item::ItemInfo object_info;
        auto object_value = list_object[i].GetObject();
        object_info.text_full = object_value["text_full"].GetString();
        object_info.text_true = object_value["text_true"].GetString();
        object_info.text_false = object_value["text_false"].GetString();
        object_info.audio_path_full = object_value["audio_path_full"].GetString();
        object_info.audio_path_quest = object_value["audio_path_quest"].GetString();
        object_info.audio_path_true = object_value["audio_path_true"].GetString();
        object_info.audio_path_false = object_value["audio_path_false"].GetString();
        game_dataset.list_objects.push_back(object_info);
    }
    
    mj::helper::shuffle(game_dataset.list_objects);
}


void MJGameBuildWall::readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{

    play_image = JSON_SAFE_CHECK(value_data, "play_image", Bool) ? value_data["play_image"].GetBool() : false;

	auto datas = value_data["data"].GetArray();
	item_wall = max_item_wall;
	if (datas.Size() < max_item_wall) {
		item_wall = datas.Size();
	}
	for (int i = 0; i < (int)datas.Size(); i++) {
		MJGameBuildWall_Item::ItemInfo object_info;
		auto object_value = datas[i].GetObject();
		auto dataFull = math::resource::ResHelper::getImage(images, object_value["text_full"].GetString());
		auto text_true = math::resource::ResHelper::getImage(images, object_value["text_true"].GetString());
		auto text_false = math::resource::ResHelper::getImage(images, object_value["text_false"].GetString());
		//auto audio_path_quest = math::resource::ResHelper::getImage(images, object_value["audio_path_quest"].GetString());

		object_info.text_full = dataFull->getText();
		object_info.text_true = text_true->getText();
		object_info.text_false = text_false->getText();
		object_info.audio_path_full = dataFull->getAudio();
		object_info.audio_path_quest = "";
		object_info.audio_path_true = text_true->getAudio();
        object_info.audio_path_false = text_false->getAudio();
        object_info.sync_text = dataFull->getSyncText();
        if (play_image)
        {
            if (!dataFull->path.empty()) object_info.image_path = dataFull->path;//kien
        }
		game_dataset.list_objects.push_back(object_info);

	}

	mj::helper::shuffle(game_dataset.list_objects);
    question_vi = "";
    if (CONFIG_MANAGER->isGameVMonkey() && value_data.HasMember("audio_intro_vi")){
        auto audio_intro = value_data["audio_intro_vi"].GetArray();
        for (int i = 0; i < (int)audio_intro.Size(); i++) {
            auto img_audio = math::resource::ResHelper::getImage(images, audio_intro[i].GetString());
            question_vi = img_audio->getAudios("name_1").front();
        }
    }
    
}

void MJGameBuildWall::onEnter(){
    ms::BaseActivity::onEnter();
    loadFrames();
    if (CONFIG_MANAGER->isGameVMonkey()){
        if(MJDEFAULT->getBoolForKey(key_background_music)){
            audio_bg_id = AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_bg.mp3", true);
        }
        auto audio_path = question_vi;
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
        if (audio_id == -1){
            onStartGame();
            return;
        }
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
            onStartGame();
        });
    }else{
        auto imgGameName = cocos2d::ui::ImageView::create("games/build_wall/logo.png");
        this->addChild(imgGameName, 10000);
        imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);
        auto audio_id_gameName = cocos2d::experimental::AudioEngine::play2d("sounds/build_wall/gameName.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_gameName, [=](int au_id, std::string path){
            imgGameName->runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(0.5f), RemoveSelf::create(true), nullptr));
            if(MJDEFAULT->getBoolForKey(key_background_music)){
                audio_bg_id = AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_bg.mp3", true);
            }
            auto audio_path = std::string("sounds/build_wall/buildwall_mc1.mp3");
            auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
                onStartGame();
            });
        });
    }
}
void MJGameBuildWall::loadFrames() {
	layout_content = this->getChildByName<cocos2d::ui::Layout *>("layout_content");
	crocodile = MJGameBuildWall_Crocodile::createGame();
	layout_content->addChild(crocodile);
	crocodile->setPosition(Vec2(this->getContentSize().width + 200, 0));
	crocodile->setDelegate(this);

	auto bg = this->getChildByName("bg");
	bg->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(1.0f, Vec2(this->getContentSize().width*0.5, this->getContentSize().height))), CallFunc::create([=]{
		auto duration = 2.5f / 1024;
		auto pos_goal = Vec2(820, 0);
		auto width = crocodile->getPositionX() - pos_goal.x;
		duration = duration*width;
		crocodile->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=]{
			AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_show_crocodile.mp3");
			crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
		}), NULL));
	}), NULL));

	if (bg->getContentSize().width < Director::getInstance()->getVisibleSize().width)
		bg->setScale(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);

	auto bg2 = cocos2d::utils::findChild(this, "bg2");
	if (bg2->getContentSize().width < Director::getInstance()->getVisibleSize().width)
		bg2->setScale(Director::getInstance()->getVisibleSize().width / bg2->getContentSize().width);

	explosive = GAFWrapper::createGAF("gaf/build_wall/Explosive/Explosive.gaf");
	explosive->setVisible(false);
	this->addChild(explosive, 100000);
    
    // guide btn
    guideBtn = ui::Button::create("games/build_wall/2.png");
    guideBtn->setPosition(cocos2d::Vec2(guideBtn->getContentSize().width / 2 + 20, Director::getInstance()->getVisibleSize().height - guideBtn->getContentSize().height / 2 - 20));
    this->addChild(guideBtn, 100001);
    guideBtn->setTouchEnabled(false);
    guideBtn->addClickEventListener([=](Ref*) {
        if (object_index < list_item.size())
        {
            if (list_item[object_index])
            {
                if (auto hand = utils::findChild(list_item[object_index], "hand_suggetion"))
                {
                    hand->removeFromParent();
                }
                list_item[object_index]->unschedule("show_guide_brick_layers");
                list_item[object_index]->setCheckIsPlayAudio(true);
            }
            auto isPlayAudio = list_item[object_index]->checkIsPlayAudio();
            if (_isPlayAudio && isPlayAudio)
            {
                AUDIO_ENGINE->playSound(game_dataset.list_objects[object_index].audio_path_full, false, [=] {
                    list_item[object_index]->runActionHand(5.0f);
                    });
            }
        }
    });


}

void MJGameBuildWall::setInputConfigGame(){

}

void MJGameBuildWall::onStartGame(){
    auto item = MJGameBuildWall_Item::createGame(game_dataset.list_objects[object_index], crocodile->is_flip);
    this->addChild(item,oderZ_item - object_index);
    item->setAnchorPoint(Vec2(0.5, 1));
    item->setPosition(Vec2(this->getContentSize().width/2, object_index*108));
    item->setDelegate(this);
    item->setTouchableStatus(false);
    list_item.push_back(item);
    
    auto width_compare = 200;
    if (object_index == 0) {
        auto rect = item->rect_convert;
        auto pos_compare = layout_content->convertToNodeSpace(item->convertToWorldSpace(rect.origin));
        pos_compare.x += rect.size.width/2;
        CCLOG("pos_compare = %f | width = %f", pos_compare.x, layout_content->getContentSize().width/2);
        if (pos_compare.x > layout_content->getContentSize().width/2) {
            auto duration = 2.5f/1024;
            crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::RUN, true);
            auto audio_walk = AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_walk.mp3");
            crocodile->runAction(Sequence::create(MoveTo::create(duration*(crocodile->getPositionX()-wall_left), Vec2(wall_left, crocodile->getPositionY())), CallFunc::create([=]{
                AUDIO_ENGINE->stopEffect(audio_walk);
                crocodile->setFlipCrocodile(true);
                
            }), DelayTime::create(0.25f), CallFunc::create([=]{
                showItemQuestion();
            }), NULL));
        }else {
            showItemQuestion();
        }
        
    }else {
        auto duration = 2.5f/1024;
        Vec2 pos_goc;
        
        auto rect = item->rect_convert;
        auto pos_compare = layout_content->convertToNodeSpace(item->convertToWorldSpace(rect.origin));
        bool isFlip = crocodile->is_flip;
        
        pos_compare.x += rect.size.width/2;
        if (pos_compare.x < layout_content->getContentSize().width/2) {
            pos_goc = Vec2(wall_right, crocodile->getPositionY());
            if (!isFlip) {
                isFlip = true;
            }
        }else {
            pos_goc = Vec2(wall_left, crocodile->getPositionY());
            if (isFlip) {
                isFlip = false;
            }
        }
        
        crocodile->setFlipCrocodile(isFlip);
        
        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::RUN, true);
        auto width = pos_goc.distance(crocodile->getPosition());
        auto audio_walk = AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_walk.mp3");
        crocodile->runAction(Sequence::create(MoveTo::create(duration*width, pos_goc), CallFunc::create([=]{
            crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
            crocodile->setFlipCrocodile(!crocodile->is_flip);
            AUDIO_ENGINE->stopEffect(audio_walk);
            
        }), DelayTime::create(0.25f), CallFunc::create([=]{
            showItemQuestion();
        }), NULL));
    }
}

void MJGameBuildWall::showItemQuestion(){
    if (play_image)
    {
        if (!game_dataset.list_objects[object_index].image_path.empty()) {
            Sprite* testimg = Sprite::create(game_dataset.list_objects[object_index].image_path);
            this->addChild(testimg, 9999);
            testimg->setPosition(Vec2(this->getBoundingBox().size.width / 2, this->getBoundingBox().size.height - testimg->getBoundingBox().size.height / 4 - 150));
            testimg->setScale(2);
            this->scheduleOnce([=](float) {
                testimg->removeFromParent();
            }, 5.5, "delay_delete_image");//kien
        }
    }
    AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_show_text.mp3");
    auto duration = 0.25f;
    crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::JUMP, true);
    layout_content->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, 108)), CallFunc::create([=]{
        list_item[object_index]->setLocalZOrder(oderZ_item + object_index);
    }), DelayTime::create(1.6f), CallFunc::create([=]{
        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
    }), NULL));
    
    list_item[object_index]->runAction(Sequence::create(MoveBy::create(duration, Vec2(0, 108)), CallFunc::create([=]{
        if (!game_play_enable && object_index > 0) {
            this->pauseGame();
            return;
        }
        list_item[object_index]->onShow(crocodile->is_flip);
        _isPlayAudio = true;
        guideBtn->setTouchEnabled(true);
        this->unschedule("show_guide_brick_layers");
        this->unschedule("delay_show_guide_brick_layers");
        if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson)){
            _is_guide_showed = true;
        }
    }), NULL));
}

void MJGameBuildWall::onCrocodileActionFinish(int action_type){
    switch (action_type) {
        case MJGameBuildWall_Crocodile::SMASH:{    
            is_answer_done = 2;
            auto audio_id = cocos2d::experimental::AudioEngine::play2d(game_dataset.list_objects[object_index].audio_path_full);
            if (auto textFull = cocos2d::utils::findChild<ui::Text*>(this, game_dataset.list_objects[object_index].text_full)) {
                if (auto textTrue = cocos2d::utils::findChild<ui::Text*>(this, game_dataset.list_objects[object_index].text_true)) {
                    auto answer = textTrue->getString();
                    if (game_dataset.list_objects[object_index].sync_text.size() !=0 ) {
                        textTrue->setVisible(false);
                        textFull->setVisible(false);
                        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
                        ms::game::startHighlightText(game_dataset.list_objects[object_index].sync_text, textFull, "", nullptr, textTrue->getString(), 15, cocos2d::Color4B::WHITE, cocos2d::Color4B::YELLOW);
                    }
                    
                }

            }
            float delaytime = 0;
            for (int i = 0; i < game_dataset.list_objects[object_index].sync_text.size(); i++)
            {
                auto valueSyncText = game_dataset.list_objects[object_index].sync_text[i];
                delaytime = delaytime + (valueSyncText.e - valueSyncText.s) / 1000.0f;
            }

            if (delaytime != 0)
            {
                this->runAction(Sequence::create(
                    DelayTime::create(delaytime + 0.5f),
                    CallFunc::create([this] {
                        this->nextTurnPlay();
                        _isPlayAudio = true;
                        }),
                    NULL));
            }
            else
            {
                cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
                    this->nextTurnPlay();
                    _isPlayAudio = true;
                    });
            }
        }
            break;
        case MJGameBuildWall_Crocodile::WRONG:
            crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
            break;
        default:
            break;
    }
}

void MJGameBuildWall::nextTurnPlay(){
    object_index++;
    guideBtn->setTouchEnabled(false);
     this->unschedule("show_guide_brick_layers");
    if (object_index >= item_wall) {
        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
        onDoneGame();
    }else {
        this->scheduleOnce([=](float){
            
        }, 0.5f, "delay_next_turn");
        
        this->onStartGame();
        
        
    }
}

void MJGameBuildWall::onChooseAnswerFinish(){
    _isPlayAudio = false;
    for (auto item : list_item)
    {
        item->setTouchableStatus(false);
    }
    this->unschedule("show_guide_brick_layers");
    this->unschedule("delay_show_guide_brick_layers");
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    
    auto duration = 2.5f/1024;
    auto post_goal = list_item[object_index]->convertToWorldSpace(list_item[object_index]->rect_convert.origin);
    auto pos_explosive = list_item[object_index]->convertToWorldSpace(list_item[object_index]->pos_goal);;
    post_goal = layout_content->convertToNodeSpace(post_goal);
    post_goal.y = crocodile->getPositionY();
    auto width = post_goal.distance(crocodile->getPosition());
    Vec2 pos_them_bot;
    
    if (crocodile->is_flip) {
        pos_them_bot =Vec2(-50, 0);
    }else {
        pos_them_bot =Vec2(+50+list_item[object_index]->rect_convert.size.width, 0);
    }
    
    auto audio_walk = AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_walk.mp3");
    crocodile->runAction(Sequence::create(MoveTo::create(duration*width, post_goal+pos_them_bot), CallFunc::create([=]{
        AUDIO_ENGINE->stopEffect(audio_walk);
        AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_smash.mp3");
        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::SMASH, false);
    }), DelayTime::create(0.7f), CallFunc::create([=]{
        explosive->setPosition(Vec2(pos_explosive.x, object_index*108+100));
        explosive->setVisible(true);
        explosive->play(false, [=] (gaf::GAFObject *obj){
            explosive->setVisible(false);
        });
        list_item[object_index]->setDoneAnswer();
    }), NULL));
    crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::RUN, true);
    
}

void MJGameBuildWall::onChooseWrongAnswer() {
    crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::WRONG, false);
}

void MJGameBuildWall::onFinishGame(){
    _activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
}

void MJGameBuildWall::onDoneGame(){
    crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::JUMP, true);
    crocodile->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
        AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_yipee.mp3");
    }) ,DelayTime::create(1.5), CallFunc::create([=]{
        crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
    }), NULL));
    
    auto audio_path = CONFIG_MANAGER->isGameVMonkey()?std::string("sounds/build_wall/Tot lam.mp3"):std::string("sounds/build_wall/buildwall_mc2.mp3");
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_winner.mp3");
        crocodile->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]{
            AUDIO_ENGINE->stopEffect(audio_bg_id);
            AUDIO_ENGINE->playEffect("sounds/build_wall/buildwall_yipee.mp3");
            crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::JUMP, true);
        }), DelayTime::create(1.6f), CallFunc::create([=]{
            crocodile->playMonkeyWithAction(MJGameBuildWall_Crocodile::IDLE, true);
        }), NULL));
        
        auto bg2 = this->getChildByName("bg2");
        bg2->setVisible(true);
        bg2->runAction(FadeIn::create(0.5f));
        auto sun = this->getChildByName("sun");
        sun->setVisible(true);
        sun->runAction(MoveBy::create(2.75f, Vec2(0, -1500)));
        
        auto h_move = 1000;
        auto duration_speed = 2.25f/768;
        layout_content->runAction(Sequence::create(MoveBy::create(duration_speed*h_move, Vec2(0, -h_move)), CallFunc::create([=]{
            this->onFinishGame();
        }), NULL));
        
        for (int i = 0; i < item_wall; i++) {
            auto h_goal = h_move - (item_wall-i)*108;
            list_item[i]->runAction(MoveBy::create(duration_speed*h_goal, Vec2(0, -h_goal)));
        }
    });
}


#pragma callback
ui::Widget::ccWidgetClickCallback MJGameBuildWall::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MJGameBuildWall::onRefresh, this);
    }
    return nullptr;
}

void MJGameBuildWall::onRefresh(cocos2d::Ref *sender) {
    /*this->removeFromParent();
    auto phonics = MJGameBuildWall::createGame("plist/game_phonics.json");
    Director::getInstance()->getRunningScene()->addChild(phonics, 1000000);*/
    
}

void MJGameBuildWall::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

void MJGameBuildWall::showGuide(Point start, Point stop) {

}
