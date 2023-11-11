#include "ShootingStar.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"

std::once_flag ShootingStar_Reader;
INITIALIZE_READER(ShootingStar);

using namespace cocos2d;

#define CSB_NAME "csb/game/ShootingStar/ShootingStar.csb"

#define gaf_smallstars "gaf/ShootingStar/stars_smallstars/stars_smallstars.gaf"
#define gaf_star1 "gaf/ShootingStar/stars_star1/stars_star1.gaf"
#define gaf_star2 "gaf/ShootingStar/stars_star2/stars_star2.gaf"
#define gaf_star3 "gaf/ShootingStar/stars_star3/stars_star3.gaf"
#define gaf_star4 "gaf/ShootingStar/stars_star4/stars_star4.gaf"
#define gaf_star5 "gaf/ShootingStar/stars_star5/stars_star5.gaf"
#define gaf_star6 "gaf/ShootingStar/stars_star6/stars_star6.gaf"
#define gaf_cloud "gaf/ShootingStar/stars_cloud/stars_cloud.gaf"
#define gaf_light "gaf/ShootingStar/stars_light/stars_light.gaf"
#define gaf_light1 "gaf/ShootingStar/stars_light1/stars_light.gaf"
#define gaf_starparticle "gaf/ShootingStar/stars_starparticle/stars_starparticle.gaf"

#define img_land1 "games/ShootingStar/stars_land1.png"
#define img_land2 "games/ShootingStar/stars_land2.png"
#define img_land3 "games/ShootingStar/stars_land3.png"
#define img_land4 "games/ShootingStar/stars_land4.png"
#define img_gameName "games/ShootingStar/game_name.png"


#define sfx_gameName "sounds/ShootingStar/sfx/game_name.mp3"
#define sfx_starsIntro "sounds/ShootingStar/sfx/stars_intro.mp3"
#define sfx_title "sounds/ShootingStar/tap on the star to uncover the new word.mp3"
#define sfx_bg "sounds/ShootingStar/sfx/stars_bgm.mp3"
#define sfx_effect1 "sounds/ShootingStar/sfx/stars_effect1.mp3"
#define sfx_effect2 "sounds/ShootingStar/sfx/stars_effect2.mp3"
#define sfx_effect3 "sounds/ShootingStar/sfx/stars_effect3.mp3"

#define sfx_hit1 "sounds/ShootingStar/sfx/stars_hit1.mp3"
#define sfx_hit2 "sounds/ShootingStar/sfx/stars_hit2.mp3"
#define sfx_hit3 "sounds/ShootingStar/sfx/stars_hit3.mp3"
#define sfx_interact "sounds/ShootingStar/sfx/stars_interact.mp3"

#define sfx_landing1 "sounds/ShootingStar/sfx/stars_landing1.mp3"
#define sfx_landface "sounds/ShootingStar/sfx/stars_landface.mp3"
#define sfx_idleAlt "sounds/ShootingStar/sfx/stars_idle_alt1.mp3"

ShootingStar* ShootingStar::createGame(std::string json_file, bool is_auto_play) {
	std::call_once(ShootingStar_Reader, [] {
		REGISTER_CSB_READER(ShootingStar);
	});
	auto p = static_cast<ShootingStar*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p)
	{
		p->didLoadFromCSB(json_file);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

ShootingStar * ShootingStar::createGame(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play, std::string game_type) {
	std::call_once(ShootingStar_Reader, [] {
		REGISTER_CSB_READER(ShootingStar);
	});
	auto p = static_cast<ShootingStar *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p)
	{
		p->didLoadFromCSB(list_phonic_info, is_auto_play);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void ShootingStar::didLoadFromCSB(std::vector<mj::PHONIC_INFO> list_phonic_info, bool is_auto_play) {
	//_list_phonic_info = list_phonic_info;
	this->is_auto_play = is_auto_play;
	screen_size = Director::getInstance()->getVisibleSize();

	//this->loadFrame();
}

void ShootingStar::didLoadFromCSB(std::string json_file) {
	json = json_file;
	//generateData();
	//loadFrame();

}

void ShootingStar::onExit()
{
	AUDIO_ENGINE->stopAllAudio();
	this->unschedule(_audioGuidePlaybackScheduleName);
	this->unscheduleAllCallbacks();
	this->unscheduleAllSelectors();
	this->stopAllActions();
	MJMainGBase::onExit();
}

void ShootingStar::onEnter() {
	MJMainGBase::onEnter();
	generateData();
    AUDIO_ENGINE->playBackgroundMusic(sfx_bg, 1.0f);
//    AUDIO_ENGINE->play(sfx_bg, true);
	loadFrame();
}

void ShootingStar::generateData() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json);
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	rapidjson::Value& json_data = document.FindMember("data")->value;
	/*for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
		ShootingStar_Data s_data;
		s_data.text_full = json_data[i].FindMember("text_full")->value.FindMember("text")->value.GetString();
		s_data.text_less = json_data[i].FindMember("text")->value.FindMember("text")->value.GetString();
		s_data.n_sound_full = json_data[i].FindMember("text_full")->value.FindMember("n_sound")->value.GetString();
		s_data.s_sound_full = json_data[i].FindMember("text_full")->value.FindMember("s_sound")->value.GetString();
		s_data.n_sound_less = json_data[i].FindMember("text")->value.FindMember("n_sound")->value.GetString();
		s_data.s_sound_less = json_data[i].FindMember("text")->value.FindMember("s_sound")->value.GetString();

		std::vector<std::string> p_sounds;
		rapidjson::Value& json_psound = json_data[i].FindMember("text")->value.FindMember("phonic")->value;
		for (rapidjson::SizeType j = 0; j < json_psound.Size(); j++) {
			p_sounds.push_back(json_psound[j].GetString());
		}
		s_data.p_sound = p_sounds;
		s_data.pos = json_data[i].FindMember("position")->value.GetInt();

		list_data.push_back(s_data);
	}*/

	for (rapidjson::SizeType i = 0; i < json_data.Size(); i++) {
		ShootingStar_Data s_data;

		auto image_id_1 = json_data[i].FindMember("text_full")->value.GetString();
		auto image_full = math::resource::ResHelper::getImage(images, image_id_1);
		s_data.text_full = image_full->getText("name_1");
		s_data.n_sound_full = image_full->getAudios("name_1").front();
		s_data.psound_full = image_full->getAudioPhonic("name_p");

		auto image_id_2 = json_data[i].FindMember("text_less")->value.GetString();
		auto image_less = math::resource::ResHelper::getImage(images, image_id_2);
		s_data.text_less = image_less->getText("name_1");
		s_data.n_sound_less = image_less->getAudios("name_1").front();
		s_data.psound_less = image_less->getAudioPhonic("name_p");

		list_data.push_back(s_data);
	}

	number_stages = list_data.size();
}

void ShootingStar::loadFrame() {

	_touchable = false;
	auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	speaker->setPosition(Vec2(40, Director::getInstance()->getWinSize().height - 40));
	speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		if (_touchable) {
			_touchable = false;
			stopIntroAudioWhenPlayGame();
			this->runAction(Sequence::create(
				CallFunc::create([=] {
					AUDIO_ENGINE->playSound(sfx_title, false, [=] {
						learningReminder();
						});
					}),
				CallFunc::create([=] {
						_touchable = true;
					}), NULL));
		}
		});
	this->addChild(speaker);

	list_star_gaf.push_back(gaf_star1);
	list_star_gaf.push_back(gaf_star2);
	list_star_gaf.push_back(gaf_star3);
	list_star_gaf.push_back(gaf_star4);
	list_star_gaf.push_back(gaf_star5);
	list_star_gaf.push_back(gaf_star6);
	mj::helper::shuffle(list_star_gaf);

	list_land_img.push_back(img_land1);
	list_land_img.push_back(img_land2);
	list_land_img.push_back(img_land3);
	list_land_img.push_back(img_land4);
	mj::helper::shuffle(list_land_img);

	list_sfx_effect.push_back(sfx_effect1);
	list_sfx_effect.push_back(sfx_effect2);
	list_sfx_effect.push_back(sfx_effect3);

	screen_size = Director::getInstance()->getVisibleSize();
	root_layout = utils::findChild<ui::Layout*>(this, "root_layout");

	star_land = utils::findChild<ui::Layout*>(this, "star_land");
	star_land->setVisible(false);
	int land_id = cocos2d::random(0, 3);
	star_land->setBackGroundImage(list_land_img[land_id]);
	star_land->runAction(MoveBy::create(0, Vec2(0, -screen_size.height)));

	stars_smallstars = GAFWrapper::createGAF(gaf_smallstars);
	stars_smallstars->setVisible(false);
	root_layout->addChild(stars_smallstars);

	starparticle = GAFWrapper::createGAF(gaf_starparticle);
	root_layout->addChild(starparticle, 10000);
	starparticle->setVisible(false);
	starparticle->setScale(0.3);

	star_light1 = GAFWrapper::createGAF(gaf_light1);
	root_layout->addChild(star_light1, 10000);
	star_light1->setVisible(false);
	star_light1->setScale(0.2);

	starparticle2 = GAFWrapper::createGAF(gaf_starparticle);
	root_layout->addChild(starparticle2, 10000);
	starparticle2->setVisible(false);
	starparticle2->setPosition(Vec2(screen_size / 2) + Vec2(0, -100));

	star_light = GAFWrapper::createGAF(gaf_light);
	root_layout->addChild(star_light);
	star_light->setVisible(false);
	star_light->setPosition(Vec2(screen_size / 2) + Vec2(0, -100));

	AUDIO_ENGINE->playSound(sfx_starsIntro, true);

	for (int i = 1; i <= 5; i++) {
		std::string str = list_star_gaf[cocos2d::random(0, 5)];
		ShootingStar_Item* item = new ShootingStar_Item("", str, true);
		switch (i)
		{
		case 1:
			item->setPosition(Vec2(screen_size.width / 2 - 340, screen_size.height * 0.25));
			break;
		case 2:
			item->setPosition(Vec2(screen_size.width / 2 - 190, screen_size.height * 0.6));
			break;
		case 3:
			item->setPosition(Vec2(screen_size.width / 2 - 40, screen_size.height * 0.25));
			break;
		case 4:
			item->setPosition(Vec2(screen_size.width / 2 + 110, screen_size.height * 0.6));
			break;
		case 5:
			item->setPosition(Vec2(screen_size.width / 2 + 260, screen_size.height * 0.25));
		default:
			break;
		}

		item->setScale(0.8);

		list_star_move.push_back(item);
	}

	for (ShootingStar_Item* item : list_star_move) {
		root_layout->addChild(item);
		//item->runAction(MoveBy::create(0, Vec2(-600, -600)));
	}

	list_star_move[0]->setScale(0.7);
	list_star_move[1]->setScale(0.9);
	list_star_move[4]->setScale(0.6);

	list_star_move[0]->runAction(MoveBy::create(0, Vec2(-1000, -1000)));
	list_star_move[1]->runAction(MoveBy::create(0, Vec2(-900, -900)));
	list_star_move[2]->runAction(MoveBy::create(0, Vec2(-600, -600)));
	list_star_move[3]->runAction(MoveBy::create(0, Vec2(-1200, -1200)));
	list_star_move[4]->runAction(MoveBy::create(0, Vec2(-500, -500)));

	star_cloud = GAFWrapper::createGAF(gaf_cloud);
	root_layout->addChild(star_cloud);
	star_cloud->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	star_cloud->setPosition(Vec2(screen_size.width, screen_size.height / 2));
	star_cloud->play(false);

	this->runAction(Sequence::create(DelayTime::create(0.1), 
		CallFunc::create([=] {
			this->runAction(Sequence::create(
				CallFunc::create([=] {
					if (FileUtils::getInstance()->isFileExist(img_gameName)) {
						if (auto magicEText = Sprite::create(img_gameName)) {
							magicEText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
							magicEText->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
							magicEText->setOpacity(0);
                            magicEText->setScale(1.3);
							root_layout->addChild(magicEText);
							magicEText->runAction(Sequence::create(
								FadeIn::create(0.5),
								CallFunc::create([=] {
									AUDIO_ENGINE->playSound(sfx_gameName);
									}),
								DelayTime::create(2),
										FadeOut::create(0.5),
										NULL));
						}
					}
					}),
				DelayTime::create(4.0f),
				CallFunc::create([=] {
					for (ShootingStar_Item* item : list_star_move) {
						//item->runAction(MoveBy::create(2, Vec2(600, 600)));
						item->getTail()->play(true);
						item->getStar()->playSequence("idle", true);
					}
					list_star_move[0]->runAction(MoveBy::create(2, Vec2(1000, 1000)));
					list_star_move[1]->runAction(MoveBy::create(2, Vec2(900, 900)));
					list_star_move[2]->runAction(MoveBy::create(2, Vec2(600, 600)));
					list_star_move[3]->runAction(MoveBy::create(2, Vec2(1200, 1200)));
					list_star_move[4]->runAction(MoveBy::create(2, Vec2(500, 500)));
					})
				, NULL));
			}), 
		DelayTime::create(4), 
		CallFunc::create([=] {
			for (ShootingStar_Item* item : list_star_move) {
				//item->runAction(MoveBy::create(2, Vec2(800, 800)));
				/*item->getTail()->play(true);
				item->getStar()->playSequence("idle", true);*/
			}
			list_star_move[0]->runAction(MoveBy::create(2, Vec2(1000, 1000)));
			list_star_move[1]->runAction(MoveBy::create(2, Vec2(900, 900)));
			list_star_move[2]->runAction(MoveBy::create(2, Vec2(800, 800)));
			list_star_move[3]->runAction(MoveBy::create(2, Vec2(1200, 1200)));
			list_star_move[4]->runAction(MoveBy::create(2, Vec2(700, 700)));
			onStageBegin();
		}), NULL));
}

void ShootingStar::setStarsPosition(std::vector<ShootingStar_Item*> list_star) {
	int size = list_star.size();
	for (int i = 0; i < size; i++) {
		list_star[i]->setPosition(Vec2(screen_size.width / 2 + (i - (float)size / 2 + 0.5) * 160, screen_size.height * 0.25));
	}
}

void ShootingStar::setStarMovePosition(std::vector<ShootingStar_Item*> list_star) {

}

void ShootingStar::onStageBegin() {
	list_stars.clear();

	stopIntroAudioWhenPlayGame();
	AUDIO_ENGINE->stopSound(sfx_starsIntro);

	_touchable = false;
	
	mj::helper::shuffle(list_star_gaf);
	for (int i = 0; i < list_data[stage_count].text_full.size(); i++) {
		std::string str;
		if (i > 5) {
			str = list_star_gaf[5];
		}
		else {
			str = list_star_gaf[i];
		}

		ShootingStar_Item* item = new ShootingStar_Item(StringUtils::format("%c", list_data[stage_count].text_full.at(i)), str, false);
		item->getLayout()->addClickEventListener(CC_CALLBACK_1(ShootingStar::onClickStar, this));

		list_stars.push_back(item);
		item->setVisible(false);
		root_layout->addChild(item, 10000);
	}
	setStarsPosition(list_stars);

	for (ShootingStar_Item* item : list_stars) {
		item->runAction(MoveBy::create(0, Vec2(0, screen_size.height)));
	}

	this->runAction(Sequence::create(
		DelayTime::create(2),
		CallFunc::create([=] {
			int land_id = stage_count % 4;
			star_land->setBackGroundImage(list_land_img[land_id]);
			star_land->setVisible(true);
			star_land->runAction(MoveBy::create(0.5, Vec2(0, screen_size.height)));
			}),
		DelayTime::create(1),
		CallFunc::create([=] {
			for (int i = 0; i < list_stars.size() - 1; i++) {
				list_stars[i]->setVisible(true);
				this->runAction(Sequence::create(
					DelayTime::create(i * 1.0),
					CallFunc::create([=] {
						list_stars[i]->runAction(MoveBy::create(0.2, Vec2(0, -screen_size.height)));
						int id = cocos2d::random(1, 4);
						if (id == 3) {
							id = 4;
						}
						list_stars[i]->getStar()->playSequence("fall" + cocos2d::StringUtils::toString(id), false);
						}),
					DelayTime::create(0.2),
					CallFunc::create([=] {
						stars_smallstars->setVisible(true);
						stars_smallstars->setPosition(Vec2(list_stars[i]->getPosition().x, list_stars[i]->getPosition().y - 300));
						stars_smallstars->play(false);
						list_stars[i]->getTail()->setVisible(false);
						int ran = cocos2d::random(0, 2);
						AUDIO_ENGINE->playSound(list_sfx_effect[ran]);
							}),
					DelayTime::create(0.5),
						CallFunc::create([=] {
						AUDIO_ENGINE->playSound(list_data[stage_count].psound_less[i]);
						}), NULL));
			}
		}),
		DelayTime::create(1 * list_stars.size()),
		CallFunc::create([=] {
			for (int i = 0; i < list_stars.size() - 1; i++) {
				list_stars[i]->runAction(EaseBackIn::create(ScaleTo::create(0.2, 1.1)));
			}
			}), 
		DelayTime::create(0.25), 
		CallFunc::create([=] {
			//AUDIO_ENGINE->playSound(list_data[stage_count].s_sound_less);
			AUDIO_ENGINE->playSound(list_data[stage_count].n_sound_less, false, [=] {
				AUDIO_ENGINE->playSound(list_data[stage_count].s_sound_less);
				});
			}),
			DelayTime::create(0.5),
			CallFunc::create([=] {
				for (int i = 0; i < list_stars.size() - 1; i++) {
					list_stars[i]->runAction(ScaleTo::create(0.2, 1));
				}
				}),
			DelayTime::create(0.5),
			CallFunc::create([=] {
				list_stars.back()->getText()->setVisible(false);
				list_stars.back()->setVisible(true);
				list_stars.back()->runAction(MoveBy::create(0.2, Vec2(0, -screen_size.height)));
				}),
			DelayTime::create(0.2),
			CallFunc::create([=] {
				list_stars.back()->getTail()->setVisible(false);
				list_stars.back()->getStar()->playSequence("fall_e", false);
				}),
			DelayTime::create(0.2),
			CallFunc::create([=] {
				AUDIO_ENGINE->playSound(sfx_landface);
				list_stars.back()->getStar()->playSequence("idle_e", true);
				starparticle->setPosition(list_stars.back()->getPosition() + Vec2(0, -20));
				starparticle->setVisible(true);
				starparticle->play(true);

				star_light1->setPosition(list_stars.back()->getPosition() + Vec2(0, -20));
				star_light1->setVisible(true);
				star_light1->play(true);
				}),
			DelayTime::create(0.2),
			CallFunc::create([=] {
				AUDIO_ENGINE->playSound(sfx_title, false, [=] {
					_touchable = true;
					_touchableForStar = true;
					learningReminder();
					this->runAction(Sequence::create(
						DelayTime::create(1.0f),
						CallFunc::create([=] {
							AUDIO_ENGINE->playSound(sfx_idleAlt, false);
							})
						, NULL));
					});
				}),
			DelayTime::create(4),
			CallFunc::create([=] {
				for (int i = 0; i < list_stars.size(); i++) {
					list_stars[i]->getLayout()->setTouchEnabled(true);
				}
				}), NULL));
}

void ShootingStar::onClickStar(Ref* sender) {
	if (_touchableForStar) {
		stopIntroAudioWhenPlayGame();
		_touchable = false;
		int choose_pos;
		
		for (int i = 0; i < list_stars.size(); i++) {
			if (auto choose = static_cast<ui::Layout*> (sender)) {
				if (list_stars[i]->getLayout() == choose) {
					choose_pos = i;
					break;
				}
			}
		}

		if (choose_pos != list_stars.size() - 1) {
			AUDIO_ENGINE->playSound(list_data[stage_count].psound_less[choose_pos], false, [=] {
				_touchable = true;
				learningReminder();
				});
			return;
		}

		list_stars.back()->setTouchEnabled(false);
		CCLOG("On Clicked Star!");
		if (!touched) {
			_touchableForStar = false;
			_touchable = false;
			float audio_time;
			touched = true;
			starparticle->setVisible(false);
			star_light1->setVisible(false);
			AUDIO_ENGINE->playSound(sfx_hit1);
			list_stars.back()->getStar()->playSequence("fall3", false);
			auto text_move_pos = getTextLocations(screen_size / 2, list_stars, Vec2(0, 0), 2.5f);//getFrameLocations(screen_size / 2, Size(100, 200), list_stars.size(), list_stars.size());
			this->runAction(Sequence::create(
				DelayTime::create(0.5),
				CallFunc::create([=] {
					list_stars.back()->getText()->setVisible(true);
					AUDIO_ENGINE->playSound(sfx_interact);
					stars_smallstars->setPosition(Vec2(list_stars.back()->getPosition().x, list_stars.back()->getPosition().y - 300));
					stars_smallstars->play(false);
					}),
				DelayTime::create(1),
						CallFunc::create([=] {
						for (int i = 0; i < list_stars.size(); i++) {
							auto text = list_stars[i]->getText();
							text->runAction(MoveTo::create(0.5, text->getParent()->convertToNodeSpace(text_move_pos[i])));
							text->runAction(ScaleBy::create(0.5, 1.5));
							text->runAction(Sequence::createWithTwoActions(DelayTime::create(0.4), FadeOut::create(0.2)));

							list_stars[i]->runAction(ScaleTo::create(0.5, 1.2));
						}
						auto full_txt = Text::create(list_data[stage_count].text_full, "fonts/KGWHATTHETEACHERWANTS.TTF", 150);
						full_txt->enableOutline(Color4B::BLACK, 3);
						//            full_txt->setScale(0.01);
						full_txt->setOpacity(0);
						full_txt->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.55));
						full_txt->setName("full_text");
						this->addChild(full_txt);
						full_txt->runAction(Sequence::create(DelayTime::create(0.5), EaseBounceOut::create(/*ScaleTo::create(0.2, 1)*/FadeIn::create(0.2)), NULL));

							}),
						DelayTime::create(0.5),
								CallFunc::create([=] {
								/*int audio1 = AUDIO_ENGINE->playSound(list_data[stage_count].n_sound_full, false, [=] {
									int audio2 = AUDIO_ENGINE->playSound(list_data[stage_count].s_sound_full);
								});*/
								std::vector<std::string> paths;
								paths.push_back(list_data[stage_count].n_sound_full);
								for (int i = 0; i < list_data[stage_count].s_sound_full.size(); i++) {
									paths.push_back(list_data[stage_count].psound_full[i]);
								}
								AUDIO_ENGINE->playChain(paths);

									}),
								DelayTime::create(2),
										CallFunc::create([=] {
										for (int i = 0; i < list_stars.size(); i++) {
											auto text = list_stars[i]->getText();
											auto text_pos = text->getParent()->convertToWorldSpace(text->getPosition());
											text->retain();
											text->removeFromParent();
											root_layout->addChild(text, 10001);
											text->release();
											text->setPosition(root_layout->convertToNodeSpace(text_pos));
											text->runAction(Sequence::create(FadeOut::create(0.25), RemoveSelf::create(), NULL));
											list_stars[i]->runAction(Sequence::create(
												Spawn::createWithTwoActions(JumpBy::create(0.5, Vec2(100, -screen_size.height / 2), 250, 1), ScaleTo::create(0.5, 1.75)), RemoveSelf::create(), NULL));
										}
										utils::findChild<Text*>(this, "full_text")->runAction(Sequence::createWithTwoActions(FadeOut::create(0.25), RemoveSelf::create()));
											}),
										DelayTime::create(0.5),
												CallFunc::create([=] {
												stage_count++;
												onNextStage();
												touched = false;
													}), NULL));
		}
	}
}


void ShootingStar::onNextStage() {
	_touchableForStar = true;
	star_land->runAction(MoveBy::create(0.5, Vec2(0, -screen_size.height)));
	if (stage_count == number_stages) {
		starparticle2->setVisible(true);
		starparticle2->play(false);
		star_light->setVisible(true);
		star_light->play(false);
		this->scheduleOnce([=](float dt) {
			onFinishGame();
		}, 3, "On finished!");

		return;
	}
	this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
		onStageBegin();
	}), NULL));
}

void ShootingStar::onFinishGame() {
	MJMainGBase::onFinishGame();
}


std::vector<cocos2d::Vec2> ShootingStar::getFrameLocations(cocos2d::Vec2 mid_point, cocos2d::Size element_size, int total_element, int max_col, cocos2d::Vec2 padding, cocos2d::Vec2 skew)
{
	std::vector<cocos2d::Vec2> locations;
	int col = max_col;
	int row = total_element / max_col;
	if (row * col < total_element) ++row;
	cocos2d::Vec2 start_pos = cocos2d::Vec2(mid_point.x - (col - 1) * (element_size.width + padding.x) * 0.5f - (row - 1) * skew.x / 2, mid_point.y - (row - 1) * (element_size.height + padding.y) / 2 - (col - 1) * skew.y / 2);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			cocos2d::Vec2 pos(start_pos.x + j * (element_size.width + padding.x) + i * skew.x, start_pos.y + i * (element_size.height + padding.y) + j * skew.y);
			locations.push_back(pos);
		}
	}
	return locations;
}

std::vector<cocos2d::Vec2> ShootingStar::getTextLocations(cocos2d::Vec2 mid_point, std::vector<ShootingStar_Item*> list_stars, cocos2d::Vec2 padding,float scale)
{
	std::vector<cocos2d::Vec2> locations;
	float total_width = -padding.x;
	float start_x = 0;
	for (auto star : list_stars) {
		auto text = star->getText();
		float text_width = scale * text->getContentSize().width;
		start_x += text_width / 2;
		locations.push_back(Vec2(start_x, 0));
		start_x += (text_width / 2 + padding.x);
		total_width +=  text_width + padding.x;
	}
	Vec2 fix_pos = Vec2(mid_point.x - total_width / 2, mid_point.y);
	for (int i = 0; i < locations.size(); ++i) {
		locations[i] += fix_pos;
	}
	return locations;
}

void ShootingStar::learningReminder()
{
	this->schedule([=](float) {
		AUDIO_ENGINE->playSound(sfx_title, false, [=] {
			this->unschedule(_audioGuidePlaybackScheduleName);
			learningReminder();
			});
		}, _delaytimeAudioGuidePlayback, _audioGuidePlaybackScheduleName);
}

void ShootingStar::stopIntroAudioWhenPlayGame()
{
	AUDIO_ENGINE->stopSound(sfx_title);
	this->unschedule(_audioGuidePlaybackScheduleName);
}
