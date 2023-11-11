//
//	CloudCatcher.h
//	Created by mintzy on 10/05/18
//
// 
#include "CloudCatcher.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"
INITIALIZE_READER(CloudCatcher);

#define speed_opacity 15

std::once_flag CloudCatcher_reader;

#define CSB_NAME "csb/game/CloudCatcher/CloudCatcher.csb"
#define GAF_BG "gaf/cloudcatcher/cloudcatcher_bg/cloudcatcher_bg.gaf"
#define GAF_BGNIGHT "gaf/cloudcatcher/cloudcatcher_bgnight/cloudcatcher_bgnight.gaf"
#define GAF_PIG "gaf/cloudcatcher/cloudcatcher_pig/cloudcatcher_pig.gaf"
#define GAF_EFFECTOVER "gaf/cloudcatcher/cloudcatcher_effectover/cloudcatcher_effectover.gaf"
#define GAF_EFFECTUNDER "gaf/cloudcatcher/cloudcatcher_effectunder/cloudcatcher_effectunder.gaf"
#define GAF_GAMENAME "gaf/cloudcatcher/cloudcatcher_gamename/cloudcatcher_gamename.gaf"
#define GAF_ENDGAME "gaf/cloudcatcher/cloudcatcher_endgame/cloudcatcher_endgame.gaf"
#define GAF_STARS "gaf/cloudcatcher/cloudcatcher_stars/cloudcatcher_stars.gaf"
#define GAF_ENDGAMEEFFECT "gaf/cloudcatcher/cloudcatcher_endgameeffect/cloudcatcher_endgameeffect.gaf"

#define SOUND_BG "sounds/cloudcatcher/cloudcatcher_bgm.mp3"
#define SOUND_ACTION "sounds/cloudcatcher/cloudcatcher_action.mp3" 
#define SOUND_COPTER "sounds/cloudcatcher/cloudcatcher_copter.mp3"	
#define SOUND_TAKEFLIGHT "sounds/cloudcatcher/cloudcatcher_takeflight.mp3"

#define SOUND_STARTGAME "sounds/cloudcatcher/cloudcatcher_startgame.mp3"
#define SOUND_STARTGAME_VN "sounds/cloudcatcher/cloudcatcher_startgame_VN.mp3"
#define SOUND_ENDGAME "sounds/cloudcatcher/cloudcatcher_endgame.mp3"



#define TIME_BG_FADEOUT 12.5f
#define PIG_MOVE_TIME 0.58f
#define CLOUD_SPEED 120.0f
#define MAX_DISTANCE 400.0f
#define THRESH 100
CloudCatcher * CloudCatcher::createGame(std::string json_file)
{
	std::call_once(CloudCatcher_reader, [] {
		REGISTER_CSB_READER(CloudCatcher);
		REGISTER_CSB_READER(CloudCatcher_CloudItem);
	});
	auto p = static_cast<CloudCatcher *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

CloudCatcher * CloudCatcher::createGame(mj::GAME_PHONIC_INFO game_phonic_info, int max_cloud_catch)
{
	std::call_once(CloudCatcher_reader, [] {
		REGISTER_CSB_READER(CloudCatcher);
		REGISTER_CSB_READER(CloudCatcher_CloudItem);
	});
	auto p = static_cast<CloudCatcher *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(game_phonic_info, max_cloud_catch);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;;
}

CloudCatcher * CloudCatcher::createGame(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::call_once(CloudCatcher_reader, [] {
		REGISTER_CSB_READER(CloudCatcher);
		REGISTER_CSB_READER(CloudCatcher_CloudItem);
	});
	auto p = static_cast<CloudCatcher *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(json_file,images);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void CloudCatcher::generateData(rapidjson::Value value_data)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;
	this->max_cloud_catch = game_config_data.FindMember("max_cloud_catch")->value.GetInt();

	// Get game type here
	if (JSON_SAFE_CHECK(game_config_data, "game_type", String))
	{
		_gameType = game_config_data.FindMember("game_type")->value.GetString();
	}

	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		mj::PHONIC_INFO info;
		if (item.HasMember("text_phonic")) {
			info.text_phonic = item.FindMember("text_phonic")->value.GetString();
		}
		if (item.HasMember("audio_phonic")) {
			info.audio_phonic = item.FindMember("audio_phonic")->value.GetString();
		}
		if (item.HasMember("text_full")) {
			info.text_full = item.FindMember("text_full")->value.GetString();
		}
		if (item.HasMember("audio_full")) {
			info.audio_full = item.FindMember("audio_full")->value.GetString();
		}
		if (item.HasMember("image_path")) {
			info.image_path = item.FindMember("image_path")->value.GetString();
		}
		_game_phonic_info.list_phonic.push_back(info);
	}
}

void CloudCatcher::generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images)
{
	rapidjson::Value& game_config_data = value_data.FindMember("game_config")->value;
	this->max_cloud_catch = game_config_data.FindMember("max_cloud_catch")->value.GetInt();

	// Get game type here
	if (JSON_SAFE_CHECK(game_config_data, "game_type", String))
	{
		_gameType = game_config_data.FindMember("game_type")->value.GetString();
	}
	rapidjson::Value& json_data = value_data.FindMember("data")->value;
	int index = cocos2d::random(0, (int)json_data.Size() - 1);
	rapidjson::Value& json_game = json_data[index].FindMember("game")->value;
	for (rapidjson::SizeType j = 0; j < json_game.Size(); j++) {
		rapidjson::Value& item = json_game[j].FindMember("item")->value;
		mj::PHONIC_INFO info;
		auto item_id = item.FindMember("image_id")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images,item_id);
		info.text_full = image->getText("name_1");
		info.text_phonic = image->getText("name_n");
		info.audio_full = image->getAudios("name_1").front();
		info.audio_phonic = image->getAudios("name_n").front();
		info.image_path = image->path;
		_game_phonic_info.list_phonic.push_back(info);
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback CloudCatcher::onLocateClickCallback(const std::string & callback_name)
{
	if (callback_name == "onRefresh") {
		return CC_CALLBACK_1(CloudCatcher::onRefresh, this);
	}
	return nullptr;
}

void CloudCatcher::didLoadFromCSB(std::string json_file)
{
	if (json_file.size() < 5) {
		json_file = "json/cloudcatcher/config.json";
	};
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	this->generateData(document.GetObject());
	mj::helper::shuffle(_game_phonic_info.list_phonic);
}

void CloudCatcher::didLoadFromCSB(std::string json_file, std::vector<math::resource::Image*> images)
{
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	this->generateData(document.GetObject(),images);
	mj::helper::shuffle(_game_phonic_info.list_phonic);
}

void CloudCatcher::didLoadFromCSB(mj::GAME_PHONIC_INFO game_phonic_info, int max_cloud_catch)
{
	_game_phonic_info = game_phonic_info;
	this->max_cloud_catch = max_cloud_catch;
	mj::helper::shuffle(_game_phonic_info.list_phonic);
	mj::helper::shuffle(_game_phonic_info.list_distracting_phonic);
}

void CloudCatcher::loadFrames()
{
	screen_size = Director::getInstance()->getWinSize();
	cloud_speed = CLOUD_SPEED;
	// Component
	layout_root = cocos2d::utils::findChild<ui::Layout* >(this, "layout_root");
	gaf_bg = GAFWrapper::createGAF(GAF_BG);
	gaf_pig = GAFWrapper::createGAF(GAF_PIG);
	gaf_bgnight = GAFWrapper::createGAF(GAF_BGNIGHT);

	gaf_pig->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.25));
	gaf_bg->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.75));
	gaf_bgnight->setPosition(screen_size / 2);
	gaf_bgnight->play(true);

	layout_root->addChild(gaf_bg);
	layout_root->addChild(gaf_bgnight, -1);
	layout_root->addChild(gaf_pig, 10000);

	gaf_pig->playSequence("idle", true);

	_locations_base.push_back(Vec2(0.5 * screen_size.width, screen_size.height + 300));
	_locations_base.push_back(Vec2(0.25 * screen_size.width, screen_size.height + 300));
	_locations_base.push_back(Vec2(0.75 * screen_size.width, screen_size.height + 300));

	clickable = true;
	max_distance = MAX_DISTANCE;
	_clouds.clear();
    
    fixBackgroundWidth();
}

void CloudCatcher::fixBackgroundWidth()
{
    ImageView * bg1 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg1");
    ImageView * bg2 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg2");
    ImageView * bg3 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg3");
    ImageView * bg4 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg4");
    
    if(bg1->getContentSize().width < screen_size.width)
    {
        bg1->setScaleX(screen_size.width / bg1->getContentSize().width);
    }
    if(bg2->getContentSize().width < screen_size.width)
    {
        bg2->setScaleX(screen_size.width / bg2->getContentSize().width);
    }
    if(bg3->getContentSize().width < screen_size.width)
    {
        bg3->setScaleX(screen_size.width / bg3->getContentSize().width);
    }
    if(bg4->getContentSize().width < screen_size.width)
    {
        bg4->setScaleX(screen_size.width / bg4->getContentSize().width);
    }
}

void CloudCatcher::showIntroGame()
{
	auto touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	touch_layout->setTouchEnabled(true);
	this->addChild(touch_layout, 69696969);
	AUDIO_ENGINE->play(SOUND_COPTER, true);

	//GAFWrapper *gaf_gamename = GAFWrapper::createGAF(GAF_GAMENAME);
	//gaf_gamename->setPosition(Vec2(screen_size.width * 0.7, screen_size.height * 0.75));
	//layout_root->addChild(gaf_gamename, 1);
	//gaf_gamename->play(false, [=](gaf::GAFObject * obj) {
	//	gaf_gamename->removeFromParentAndCleanup(true);
	//});

	gaf_pig->playSequence("fade_in", false, [=](gaf::GAFObject * object, const std::string &sequenceName) {
		//autoGenCloud();
		AUDIO_ENGINE->play(SOUND_TAKEFLIGHT);
        if(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
        {
            this->scheduleOnce([=](float dt) {
                AUDIO_ENGINE->play(SOUND_STARTGAME_VN);
            }, 1.0f, "audio_start");
            
        }else{
            AUDIO_ENGINE->play(SOUND_STARTGAME);
        }
			
		gaf_pig->playSequence("takeflight", false, [=](gaf::GAFObject * object, const std::string &sequenceName) {
			gaf_pig->playSequence("idle", true);
			//gen_cloud = true;
			game_state = GameState::StartGame;
			auto cloud = CloudCatcher_CloudItem::createItem(_game_phonic_info.list_phonic[0], CloudCatcher_CloudItem::TEXT_PHONIC, true);
            if(_gameType == "image") {
                cloud = CloudCatcher_CloudItem::createItem(_game_phonic_info.list_phonic[0], CloudCatcher_CloudItem::PICTURE, true);
            }
			cloud->setPosition(Vec2(screen_size.width / 2, screen_size.height + 500));
			cloud->setDelegate(this);
			layout_root->addChild(cloud, 500);
			_clouds.push_back(cloud);
			cloud->setScale(1.75f);
			cloud->runAction(Sequence::create(EaseQuadraticActionOut::create(MoveTo::create(1.25f, Vec2(screen_size.width / 2, screen_size.height * 0.75))),
				CallFunc::create([=] {
				if (is_auto_play) {
					autoPlay();
				}
				else {
					touch_layout->removeFromParent();
					showTapGuideGame(10.0f);
				}
			}),NULL));

		});
	});

	gaf_bg->playSequence("fade_in", false, [=](gaf::GAFObject * object, const std::string &sequenceName) {
		//gaf_bg->playSequence("idle", true);
	});

}

void CloudCatcher::onRefresh(cocos2d::Ref * sender)
{
}

void CloudCatcher::updateCloud(float dt)
{

	if (game_state == GameState::Playing) {
		current_time += dt;

		if (current_time - last_time > 3.0f) {
			//combo_count = 0;
			last_time = current_time;
		}
		int number = 0;
		if (8 > combo_count && combo_count >= 5)
		{
			_speedCloud = combo_count * combo_count * 4;
			_maxDistance = combo_count * 15;
		}
		else if (combo_count >= 8)
		{
			_speedCloud = _speedCloud;
			_maxDistance = _maxDistance;
		}
		else {
			_speedCloud = combo_count* combo_count * 3;
			_maxDistance = combo_count * 15;
		}
		cloud_speed = (CLOUD_SPEED + _speedCloud) ;
		max_distance = MAX_DISTANCE - _maxDistance;
		if (gen_cloud) {
			distance += cloud_speed * dt;
			if (distance > max_distance) autoGenCloud();
		}
		for (auto cloud : _clouds) {
			if (cloud) {
				auto pos = cloud->getPosition();
				if (pos.y <= 0) {
					cloud->setVisible(false);
				}
				else if(pos.y > 0){
					pos.y -= cloud_speed * dt;
					cloud->setPosition(pos);
				}
			}
		}
	}
	
}

void CloudCatcher::onEnter()
{
	MJMainGBase::onEnter();
	this->loadFrames();
	schedule(schedule_selector(CloudCatcher::updateCloud), 0);
	game_state = GameState::StartGame;
	this->playBackgroundMusic(SOUND_BG);
	showIntroGame();

}

void CloudCatcher::onExitTransitionDidStart()
{
	PHONIC_MANAGER->clearCacheGaf();
	MJMainGBase::onExitTransitionDidStart();
}

void CloudCatcher::onFinishGame()
{
	game_state = GameState::Finnish;
	this->unscheduleAllCallbacks();
	gen_cloud = false;

	for (auto cloud : _clouds) {
		if (cloud) {
			cloud->setEnableClick(false);
			cloud->setUsed(true);
			cloud->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.5f), CallFunc::create([=] {
				cloud->setVisible(true);
			}),NULL));
		}

	}

	auto cloud = CloudCatcher_CloudItem::createItem(_game_phonic_info.list_phonic[0], CloudCatcher_CloudItem::TEXT_PHONIC, true);
    if(_gameType == "image") {
        cloud = CloudCatcher_CloudItem::createItem(_game_phonic_info.list_phonic[0], CloudCatcher_CloudItem::PICTURE, true);
    }
	cloud->setPosition(Vec2(screen_size.width / 2, screen_size.height + 500));
	cloud->setDelegate(this);
	cloud->setEnableClick(false);
	layout_root->addChild(cloud, 500);
	_clouds.push_back(cloud);
	cloud->setScale(2.25f);
	cloud->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
		auto pig_pos = gaf_pig->getPosition();
		auto move_pos = Vec2(100, screen_size.height / 2);
		if (pig_pos.x > screen_size.width / 2) {
			move_pos = Vec2(screen_size.width - 100, screen_size.height / 2);
		}
		float last_scale = gaf_pig->getScaleX();
		float scale = (pig_pos.x > move_pos.x) ? 1 : -1;
		gaf_pig->setScaleX(scale);
		Vec2 new_pos = Vec2(pig_pos.x + (last_scale - scale) * 105, pig_pos.y);
		gaf_pig->setPosition(new_pos);
		gaf_pig->runAction(Sequence::create(MoveTo::create(0.82f, move_pos), CallFunc::create([=] {
			float last_scale = gaf_pig->getScaleX();
			float scale = (pig_pos.x > screen_size.width / 2) ? 1 : -1;
			gaf_pig->setScaleX(scale);
			Vec2 new_pos = Vec2(move_pos.x + (last_scale - scale) * 105, move_pos.y);
			gaf_pig->setPosition(new_pos);
		}), NULL));
	}),
		EaseQuadraticActionOut::create(MoveTo::create(2.0f, Vec2(screen_size.width / 2, screen_size.height * 0.5))), CallFunc::create([=] {	
		cloud->setEnableClick(true);
	}), NULL));
	GAFWrapper * gaf_endgame = GAFWrapper::createGAF(GAF_ENDGAMEEFFECT);
	gaf_endgame->setPosition(Vec2(cloud->getContentSize().width * 0.75,cloud->getContentSize().height * 0.25));
	gaf_endgame->play(true);
	gaf_endgame->setScale(0.5f);
	cloud->addChild(gaf_endgame, -1, "gaf_endgameeffect");
	layout_root->runAction(Repeat::create(Sequence::create(DelayTime::create(0.05f),
		CallFunc::create([=] {
		GAFWrapper * star_gaf = GAFWrapper::createGAF(GAF_STARS);
		layout_root->addChild(star_gaf);
		float rotate = cocos2d::random(0, 360);
		star_gaf->setRotation(rotate);
		star_gaf->setPosition(Vec2(cloud->getPosition()));
		star_gaf->playSequence(cocos2d::StringUtils::format("star%d", cocos2d::random(1, 6)), false, [=](gaf::GAFObject * obj, const std::string &sqN) {
			star_gaf->removeFromParent();
		});
	}), NULL), 1000000));

}

void CloudCatcher::onClickedCloudDelegate(cocos2d::Ref * sender) 
{
	CloudCatcher_CloudItem * cloud = static_cast<CloudCatcher_CloudItem *>(sender);
	hideTapGuideGame();
	showTapGuideGame(10.0f);
	last_time = current_time;
	if (game_state == GameState::StartGame || game_state == GameState::Finnish) {
		cloud->setEnableClick(false);
		cloud->setUsed(true);
		auto cloud_pos = cloud->getPosition();
		auto pig_pos = gaf_pig->getPosition();
		auto gaf_effectunder = GAFWrapper::createGAF(GAF_EFFECTUNDER);
		gaf_effectunder->setPosition(cloud_pos);
		layout_root->addChild(gaf_effectunder, 1000);
		gaf_effectunder->play(false, [=](gaf::GAFObject * obj) {
			gaf_effectunder->removeFromParent();
		});
		auto gaf_effectover = GAFWrapper::createGAF(GAF_EFFECTOVER);
		gaf_effectover->setPosition(cloud_pos);
		layout_root->addChild(gaf_effectover, 1001);
		gaf_effectover->play(false, [=](gaf::GAFObject * obj) {
			gaf_effectover->removeFromParent();
		});
		gaf_pig->playSequence("cast", false, [=](gaf::GAFObject * obj, const std::string &sqName) {
				gaf_pig->playSequence("idle",true);
				if (game_state == GameState::StartGame) {
					gaf_pig->runAction(EaseQuadraticActionOut::create(MoveTo::create(1.5f, Vec2(screen_size.width / 2, screen_size.height * 0.35))));
					autoGenCloud();
					gen_cloud = true;
					cloud->onCatcherCloud([=] {
						onPlayingGame();
					});	
				}
				else if(game_state == GameState::Finnish) {
                    this->scheduleOnce([=](float dt) {
                        AUDIO_ENGINE->stopAllAudio();
                        cloud->setVisible(false);
                        gaf_pig->setVisible(false);
                        GAFWrapper * gaf_endgame = GAFWrapper::createGAF(GAF_ENDGAME);
                        if(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())
                        {
                            AUDIO_ENGINE->play(SOUND_TAKEFLIGHT);
                        }else{
                            AUDIO_ENGINE->play(SOUND_ENDGAME);
                        }
                        gaf_endgame->setPosition(Vec2(screen_size.width * 0.45f,screen_size.height * 0.05f));
                        layout_root->addChild(gaf_endgame, 600);
                        layout_root->stopAllActions();
                        gaf_endgame->playSequence("hit", false, [=](gaf::GAFObject * obj, const std::string & sqN) {
                            MJMainGBase::onCloseGame();
                        });
                    }, 1.5f, "delay_finish");
				}

		});
		float last_scale = gaf_pig->getScaleX();
		float scale = (pig_pos.x >= cloud_pos.x) ? 1 : -1;
		gaf_pig->setScaleX(scale);
		Vec2 new_pos = Vec2(pig_pos.x + (last_scale - scale) * 105, pig_pos.y);
		gaf_pig->runAction(Sequence::create(EaseQuadraticActionOut::create(MoveTo::create(PIG_MOVE_TIME, Vec2(cloud_pos.x, cloud_pos.y))), CallFunc::create([=] {
			AUDIO_ENGINE->play(cloud->_phonic_info.audio_full);
			AUDIO_ENGINE->play(SOUND_ACTION, false);	
		}), NULL));
	}
	else if (game_state == GameState::Playing) {
		if(clickable && cloud && !cloud->isCatcher()) {
			cloud->setEnableClick(false);
			cloud->setUsed(true);
			clickable = false;
			if(combo_count < 10)
			++combo_count;
			auto cloud_pos = cloud->getPosition();
			auto pig_pos = gaf_pig->getPosition();
			float last_scale = gaf_pig->getScaleX();

			Vec2 move_pos = Vec2(cloud_pos.x, cloud_pos.y - PIG_MOVE_TIME * cloud_speed);

			auto gaf_effectunder = GAFWrapper::createGAF(GAF_EFFECTUNDER);
			gaf_effectunder->setPosition(move_pos + Vec2(50, 0));
			layout_root->addChild(gaf_effectunder, 1000);
			gaf_effectunder->play(false, [=](gaf::GAFObject * obj) {
				gaf_effectunder->removeFromParent();
			});
			auto gaf_effectover = GAFWrapper::createGAF(GAF_EFFECTOVER);
			gaf_effectover->setPosition(move_pos + Vec2(50, 0));
			layout_root->addChild(gaf_effectover, 1001);
			gaf_effectover->play(false, [=](gaf::GAFObject * obj) {
				gaf_effectover->removeFromParent();
			});
			Vec2 new_pig_pos = move_pos;
			if (new_pig_pos.y < 180) {
				new_pig_pos.y = 180;
			}
			else if (new_pig_pos.y > screen_size.height - 180) {
				new_pig_pos.y = screen_size.height - 180;
			}
			float scale = (pig_pos.x > cloud_pos.x) ? 1 : -1;
			gaf_pig->setScaleX(scale);
			Vec2 new_pos = Vec2(pig_pos.x + (last_scale - scale) * 105, pig_pos.y);
			gaf_pig->setPosition(new_pos);
			gaf_pig->playSequence("cast", false, [=](gaf::GAFObject * obj, const std::string &sqName) {
				gaf_pig->playSequence("hit", false, [=](gaf::GAFObject * obj, const std::string &sqName) {
					gaf_pig->playSequence("idle", true);
				});
			});
			gaf_pig->runAction(Sequence::create(EaseQuadraticActionOut::create(MoveTo::create(PIG_MOVE_TIME, new_pig_pos)),
				CallFunc::create([=] {
					if (cloud->getCloudType() == CloudCatcher_CloudItem::CloudType::TEXT_PHONIC) {
						AUDIO_ENGINE->play(cloud->_phonic_info.audio_phonic);
					}
					else {
						AUDIO_ENGINE->play(cloud->_phonic_info.audio_full);
					}
				AUDIO_ENGINE->play(SOUND_ACTION, false);
				cloud->onCatcherCloud();	
			}), NULL));
			this->scheduleOnce([=](float dt) {
				clickable = true;
			}, PIG_MOVE_TIME + 0.1f, "enable_clickable");
			
		}
	}

}

void CloudCatcher::onChangeCloudStateDelegate(cocos2d::Ref * sender, int state)
{
	if (game_state == Playing && state == CloudCatcher_CloudItem::CloudState::CATCHERED) {
		++cloud_catch_count;
		if (cloud_catch_count == max_cloud_catch) {
            onFinishGame();
            
		}
	}

}

void CloudCatcher::showTapGuideGame(float delay_time, std::string gaf_guide_path)
{
	auto guide_layout_check = this->getChildByName("guide_layout");
	if (guide_layout_check) return;
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);
	Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX - 2);
	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		for (auto cloud : _clouds) {
			if (cloud && !cloud->isCatcher() && cloud->getPositionY() > screen_size.height * 0.4 && !cloud->isUsed()) {
				cloud->showTapGuideGame(gaf_guide_path);
				break;
			}
		}
	}, 5, 100, delay_time, "animated_guide");
}

void CloudCatcher::hideTapGuideGame()
{
	Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
	for (auto cloud : _clouds) {
		if (cloud && !cloud->isCatcher() && !cloud->isUsed()) cloud->hideTapGuideGame();
	}
}

void CloudCatcher::onPlayingGame()
{
	game_state = Playing;
	cloud_catch_count = 0;
	ImageView * bg1 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg1");
	ImageView * bg2 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg2");
	ImageView * bg3 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg3");
	ImageView * bg4 = cocos2d::utils::findChild<ImageView *>(layout_root, "bg4");
	bg1->runAction(Sequence::create(EaseQuarticActionIn::create(FadeOut::create(1.5 * TIME_BG_FADEOUT)), CallFunc::create([=] {
		bg2->runAction(Sequence::create(EaseQuarticActionIn::create(FadeOut::create(TIME_BG_FADEOUT)), CallFunc::create([=] {
			gaf_bg->runAction(EaseQuarticActionIn::create(FadeOut::create(TIME_BG_FADEOUT)));
			bg3->runAction(Sequence::create(EaseQuarticActionIn::create(FadeOut::create(TIME_BG_FADEOUT)), CallFunc::create([=] {
				bg4->runAction(Sequence::create(EaseQuarticActionIn::create(FadeOut::create(TIME_BG_FADEOUT)), CallFunc::create([=] {

				}), NULL));
			}), NULL));
		}), NULL));
	}), NULL));
}

void CloudCatcher::genCloud(int num_cloud, bool only_text)
{
	mj::helper::shuffle(_locations_base);

	for (int i = 0; i < num_cloud; ++i) {
		CloudCatcher_CloudItem::CloudType type;
		if (only_text) {
			type = CloudCatcher_CloudItem::TEXT_PHONIC;
		}
		else {
			type = CloudCatcher_CloudItem::CloudType::PICTURE;
		}
		bool is_uppercase = cocos2d::random(0, 1) == 1;
		int index_phonic = cocos2d::random(0, (int)_game_phonic_info.list_phonic.size() - 1);
		if (type == CloudCatcher_CloudItem::CloudType::PICTURE) {
			index_phonic = ++last_index % (int)_game_phonic_info.list_phonic.size();
			if (index_phonic == 0)
				mj::helper::shuffle(_game_phonic_info.list_phonic);
		}
		auto cloud = CloudCatcher_CloudItem::createItem(_game_phonic_info.list_phonic[index_phonic], type, is_uppercase);
		cloud->setScale(1.35f);
		cloud->setPosition(screen_size / 2);
		layout_root->addChild(cloud, 100);
		cloud->setDelegate(this);
		Vec2 loc = _locations_base[i] + Vec2(cocos2d::random(-7, 7) * 10, 0);// cocos2d::random(-7, 7) * 10);
		cloud->setPosition(loc);
		_clouds.push_back(cloud);
	}
}

void CloudCatcher::autoGenCloud()
{
	distance = 0;
	if (_gameType == "image") {
		// Tao dam may chua hinh anh
		CCLOG("Choi voi hinh anh");
		genCloud(1, false);
	}
	else if(_gameType == "text") {
		// Tao dam may chua text
		CCLOG("Choi voi text");
		genCloud(1, true);
	}
	else {
		CCLOG("Choi voi ca hinh anh va text");
		genCloud(1, cloud_catch_count % 2);
	}
}

void CloudCatcher::autoPlay()
{
	this->schedule([=](float dt) {
		for (auto cloud : _clouds) {
			if (cloud && !cloud->isCatcher() && cloud->getPositionY() > PIG_MOVE_TIME * 2 * cloud_speed && !cloud->isUsed()) {
				cloud->onAutoClick();
				break;
			}
		}
	}, 1.0f, kRepeatForever, 2.0f, "autoPlay");
}
