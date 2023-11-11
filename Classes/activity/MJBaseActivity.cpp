//
//  MJBaseActivity.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/21/17.
//

#include "MJBaseActivity.h"
#include "ActivityLayout.h"
#include "GameBase.h"
#include "GameList.h"

#include "GameQuizLayout.h"
#include "TestingModeUtils.h"

#include "StoryConfigManager.h"
#include "StoryFreeManager.h"
#include "HSAudioEngine.h"
#include "ProScore.h"
#include "MSLessonPreview.h"
#include "StoryPageVC.h"
#include "MSLessonManager.h"
#include "MSGradeManager.h"
#include "audio/include/AudioEngine.h"
#include "StoryPageVC.h"
//#include "AISpeakingScene.h"
#include "CocoStudioMacro.h"
#include "F2PManager.h"

USING_NS_CC;

ms::BaseActivity * ms::BaseActivity::createActivity(mj::model::ActInfo info, std::vector<math::resource::Image*> images, mj::model::AISpeakingLessonInfo lessonInfo, ms::AISpeakingMode mode) {

    ms::BaseActivity * activity = nullptr;
	MJGBase * game = nullptr;
    switch (info.type) {
        case mj::model::ActInfo::Type::GAME:
            if (info.sub_id == 100006 || info.sub_id == 10012) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
             //   activity = AISpeakingScene::createView(images, lessonInfo, mode);
#endif
            }
            else if (info.sub_id == 12) {
                activity = GameRabbitClassify::createGame(images);
			}	else if (info.sub_id == 13) {
				activity = MJGameWaterSpray::createGame(images);
			}	else if (info.sub_id == 15) {
				activity = MJGameBuildWall::createGame(images);
			}	else if (info.sub_id == 18) {
				activity = GameDiamondDig::createGame(images);
			}	else if (info.sub_id == 19) {
				activity = GameKhiXeUi::createGame(images);
			}	else if (info.sub_id == 20) {
				activity = GameRestInNest::createGame(images);
			}	else if (info.sub_id == 21) {
				activity = GameShookBook::createGame(images);
			}	else if (info.sub_id == 22) {
				activity = GameUFO::createGame(images);
			}	else if (info.sub_id == 23) {
				activity = GameXepChu::createGame(images);
			}	else if (info.sub_id == 24) {
				activity = MJGameTyping::createGame(images);
			}	else if (info.sub_id == 25) {
				//activity = MJGameShark::createGame(images);
				activity = MJGameShark2::createGame(images);
			}	else if (info.sub_id == 45) {
				activity = GameLoadTheTrain::createGame(images);
			}	else if (info.sub_id == 46) {
				activity = GamePlugInBulb2::createGame(images);
			}
			else if ((info.sub_id >= 47 && info.sub_id <= 55) || info.sub_id == 44 || info.sub_id == 73){ //old game here <---------------
				auto buffer_string = FileUtils::getInstance()->getStringFromFile("config.json");
				rapidjson::Document value_data;
				value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
				if (value_data.HasParseError()) {
					CCLOG("JSON ERROR!");
					return nullptr;
				}
				auto game_info = value_data.GetObject();
				
				if (info.sub_id == 44) {
					activity = mj::game::createGameFromJSon<MJGameBuildSentence, mj::BuildSentenceDataSet>(game_info, mj::GameType::BUILD_SENTENCE, images);
				}
				else if (info.sub_id == 47) {
					activity = mj::game::createGameFromJSon<MJGameChickensToCoop, mj::ChickensToCoopDataSet>(game_info, mj::GameType::CHICKENS_TO_COOP, images);
				}
				else if (info.sub_id == 48) {
					activity = mj::game::createGameFromJSon<MJGameJumpingFrog, mj::JumpingFrogDataSet>(game_info, mj::GameType::JUMPING_FROG, images);
				}
				else if (info.sub_id == 49) {
					activity = mj::game::createGameFromJSon<MJGameLandThePlane, mj::LandThePlaneDataSet>(game_info, mj::GameType::LAND_THE_PLANE, images);
				}
				else if (info.sub_id == 50) {
					activity = mj::game::createGameFromJSon<MJGameMakeAMatch, mj::MakeAMatchDataSet>(game_info, mj::GameType::MAKE_A_MATCH_TI, images);
				}
				else if (info.sub_id == 51) {
					activity = mj::game::createGameFromJSon<MJGameMatching, mj::MatchingGameDataSet>(game_info, mj::GameType::PICTURE_MATCH, images);
				}
				else if (info.sub_id == 52) {
					activity = mj::game::createGameFromJSon<MJGameLazer, mj::LazerGameDataSet>(game_info, mj::GameType::PLUG_IN_BULBS_TI, images);
				}
				else if (info.sub_id == 53) {
					activity = mj::game::createGameFromJSon<MJGameQA, mj::QADataSet>(game_info, mj::GameType::Q_AND_A, images);
				}
				else if (info.sub_id == 54) {
					activity = mj::game::createGameFromJSon<MJGameBirdie, mj::BirdieDataSet>(game_info, mj::GameType::SING_BIRDIE, images);
				}
				else if (info.sub_id == 55) {
					activity = mj::game::createGameFromJSon<MJGameWordMachine, mj::WordMachineDataSet>(game_info, mj::GameType::WORD_MACHINE, images);
				}
				else if (info.sub_id == 73) {
					activity = mj::game::createGameFromJSon<MJGameMatching, mj::MatchingGameDataSet>(game_info, mj::GameType::LIGHT_THEM_UP, images);
				}
			}	else if (info.sub_id == 56) {
				activity = buildwords::GamePlay::createGame(images);
			}	else if (info.sub_id == 57) {
				activity = MJGameJumpingMonkey::createGame(images);
			}	else if(info.sub_id == 331) {
				activity = MJGameJumpingMonkeyVMonkey::createGame(images);
			}	else if (info.sub_id == 58) {
				activity = MJGameWordTrain::createGame(images);
			}	else if (info.sub_id == 59) {
				activity = houseofwords::HouseOfWord::createGame(images);
			}	else if (info.sub_id == 60) {
				activity = sequencing::GamePlay::createGame(images);
			}	else if (info.sub_id == 61) {
				activity = makefireworks::GamePlay::createGame(images);
			}	else if (info.sub_id == 62) {
                activity = story_elements::GamePlay::createGame(images);
			}	else if (info.sub_id == 63) {
				activity = WS_Playarea::createGame(images);
			}
#if CC_TARGET_PLATFORM != CC_PLATFORM_MAC &&  CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
            else if (info.sub_id == 66) {
				activity = MJGamePhonics::createGame(images);

            }
			else if (info.sub_id == 1488) {
				activity = ProScore::createGame(images);
			}
#endif
			else if ((info.sub_id >= 188 && info.sub_id <= 291)|| info.sub_id == 180 || info.sub_id == 181 || info.sub_id == 184 || info.sub_id == 329 || (info.sub_id >= 372 && info.sub_id <= 379)) {
                
                auto search_path = FileUtils::getInstance()->getSearchPaths();
                activity = ActivityLayout::createGame(info.sub_id,search_path[1]);
            }else {
                activity = GameQuizLayout::createGame(info.quizz_id, info.list_num_turnplay);
            }
			if (activity)
				activity->_info = info;
			else if (game)
				game->_info = info;
				
            break;
            
        case mj::model::ActInfo::Type::STORY:
//            activity = MJStoryActivity::createActvity(info.sub_id, 1);
//            activity->setInfo(info);
            break;
            
        case mj::model::ActInfo::Type::QUIZZES:
            break;
            
        default:
            return nullptr;
            break;
    }
    
	if (activity)
	{
		MS_LOGGER.logEvent("BaseActivity_createActivity", {
			{"game_subid",cocos2d::Value( info.sub_id)}
		});
        activity->setCloseButtonEnale(true);
		return activity;
	}
    
	return game;
}

void ms::BaseActivity::onEnter() {
	mj::helper::SetFpsByNumberFrames(30);
    cocos2d::ui::Layout::onEnter();

    if ((int) CONFIG_MANAGER->isAppActive() || F2P_MANAGER->isFreeContentPage()){
        game_play_enable = true;
    }
	else {
		auto lesson_ids = ms::LessonManager::getInstance().getListLessonFree();

		//if (lesson_ids.empty())
		//{
		//	int currentCategory = ms::LessonManager::getInstance().getCurrentLessonForCategory();
		//	lesson_ids = ms::LessonManager::getInstance().getFreeLessonId(currentCategory);
		//}
        
		auto it = std::find_if(lesson_ids.begin(), lesson_ids.end(), [=](int a) {
			return _info.lesson_id == a;
			});

		game_play_enable = (it == lesson_ids.end()) ? false : true;
	}

	if (!game_play_enable && !F2P_MANAGER->isFreeContentPage())
	{
			this->runAction(Sequence::create(DelayTime::create(5.0f),
				CallFunc::create([=] {
					this->pauseGame();
					}),
				NULL));;
	}

	_info.zip_urls.erase(unique(_info.zip_urls.begin(), _info.zip_urls.end()), _info.zip_urls.end());

}

void ms::BaseActivity::setCloseButtonEnale(bool isEnable) {
    
    _isEnableCloseButton = isEnable;
    
    if(isEnable) {
        
        addCloseButton();
  
    } else  {
        
        if(auto node =cocos2d::utils::findChild(this,"close_layout")) {
            node->removeFromParent();
        }
    }
}

void ms::BaseActivity::addCloseButton() {

    auto closeLayout = ImageView::create("mjstory/close_game.png");
    closeLayout->setName("close_layout");
    this->addChild(closeLayout, 1000000000);
    closeLayout->setPosition(cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height +50));
    closeLayout->setAnchorPoint(cocos2d::Point::ANCHOR_TOP_RIGHT);

    auto lblClose = Label::createWithTTF("Close?", "fonts/Montserrat-SemiBold.ttf", 16);
    lblClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lblClose->setPosition(Vec2(closeLayout->getContentSize().width / 2, -10));
    lblClose->setTextColor(Color4B(Color3B(38, 176, 230)));
    lblClose->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    closeLayout->addChild(lblClose);
    lblClose->setVisible(false);

    auto button = cocos2d::ui::Button::create("mjstory/papgemenucontrol_btclose.png");
    button->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    button->setPositionX(closeLayout->getContentSize().width / 2);
    button->setPositionY(10);
    button->setLocalZOrder(-1);
    button->setOpacity(0);
    button->setScale(1.5f);
    button->addClickEventListener([=](Ref * sender) {
        if (!close_button_data.onMove){
            if (close_button_data.onBot){
                close_button_data.onMove = true;

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
                if (MJDEFAULT->getBoolForKey("ms_skip_game_activity_enable", false)) {
                    commonEndGame(ms::BaseActivity::EGT_COMMON);
                }
#endif
                if(_callbackCloseActivity != nullptr)
                {
                    _callbackCloseActivity();
                }
                pauseActivity();
                onCloseGame();//to override;
                if (CONFIG_MANAGER->isGameVMonkey()){
                    //onCloseGameVmonkey(true);
                }else{
                    _activitydelegate->onActivityEnd(this, ms::delegate::Activity::EndReason::INTERRUPTED);
                }
                AUDIO_ENGINE->stopAllEffects();
            }
            else{
                close_button_data.onMove = true;
                closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height))
                    , CallFunc::create([=](){
                    lblClose->setVisible(true);
                    close_button_data.onBot = true;
                    close_button_data.onMove = false;
                    this->scheduleOnce([=](float dt){
                        close_button_data.onMove = true;
                        close_button_data.onBot = false;
                        closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height + 50))
                            , CallFunc::create([=](){
                            close_button_data.onMove = false;
                            lblClose->setVisible(false);
                        }), nullptr));
                    }, 3.0f, "back_to_top");
                }), nullptr));
            }
        }
    });
    closeLayout->addChild(button, 1000000000);
}

void ms::BaseActivity::startParticle(int type, cocos2d::Node *parent){

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    const int k_particleImgID = random(1,6);
    ParticleSystemQuad *exploisive = ParticleSystemQuad::create("plist/particle/particle_acti_0.plist");
    this->addChild(exploisive, 100001);
    exploisive->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height+50));
    exploisive->setDisplayFrame(cocos2d::Sprite::create(StringUtils::format("plist/particle/particle_acti_%d.png",k_particleImgID))->getSpriteFrame());
    //    exploisive->setDisplayFrame(cocos2d::SpriteFrame::create("particle/9.png", cocos2d::Rect(0,0,100,100)));
    //exploisive->start();
    //exploisive->setVisible(false);
    exploisive->setName("particle");
#else
	const int k_particleImgID = random(1,6);
    ParticleSystemQuad *exploisive = ParticleSystemQuad::create("plist/particle/particle_acti_1.plist");
	exploisive->setDisplayFrame(cocos2d::Sprite::create(StringUtils::format("plist/particle/particle_acti_%d.png", k_particleImgID))->getSpriteFrame());
	this->addChild(exploisive, 100001);
    exploisive->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height+50));
    //    exploisive->setDisplayFrame(cocos2d::SpriteFrame::create("particle/9.png", cocos2d::Rect(0,0,100,100)));
    //exploisive->start();
    //exploisive->setVisible(false);
    exploisive->setName("particle");
#endif
}

void ms::BaseActivity::pauseActivity() {
    this->pause();
    this->enumerateChildren("//.*", [](Node * child) {
        child->pause();
        return false;
    });
}

void ms::BaseActivity::resumeActivity() {
    this->resume();
    this->enumerateChildren("//.*", [](Node * child) {
        child->resume();
        return false;
    });
}

void ms::BaseActivity::commonEndGame(END_GAME_TYPE eType, std::string charGafPath /*= ""*/, bool isNeedtextBonus/* = false*/,bool is_INTERRUPTED)
{
//	CleverTapManager::GetInstance()->pushEvent("end_game", {
//		{"activity_name ",cocos2d::Value(_info.name)},
//		{"activity_id", cocos2d::Value(_info.activity_id)}
//	});
	GAFWrapper* bgGaf = nullptr;
	GAFWrapper* mainGaf = nullptr;
	GAFWrapper* textBonusGaf = nullptr;
	int typeBonus = rand() % 3;
	bool isNeedFireWork = false;
   
	enum ANCHO_GAF  {
		AG_MID = 0,
		AG_MID_TOP
	};
	enum SOUND_TEXT  {
		ST_NONE = 0,
		ST_YOU_DID_IT,
		ST_GREAT_JOB,
		ST_WELL_DONE,
		ST_GOOD_JOB,
        ST_VM
	};
    
	ANCHO_GAF ancho = AG_MID_TOP;
	SOUND_TEXT sound_text = SOUND_TEXT::ST_NONE;
    if (CONFIG_MANAGER->isVietnameseStories()){
        eType = ms::BaseActivity::EGT_COMMON;
    }
	switch (eType)
	{
	case ms::BaseActivity::EGT_AWESOME:{
										   bgGaf = GAFWrapper::createGAF("gaf/end_game/awesome_gaf/awesome_gaf.gaf");
										   AUDIO_ENGINE->playEffect(StringUtils::format("sounds/end_game/awesome%d.mp3", rand() % 3 + 1));
										   this->addChild(bgGaf, 100001);
										   //ko can main
										   break; }
	case ms::BaseActivity::EGT_GOOD_JOB:{
											bgGaf = GAFWrapper::createGAF("gaf/end_game/goodjob_gaf/goodjob_gaf.gaf");
											AUDIO_ENGINE->playEffect("sounds/end_game/goodjob_1.mp3");
											this->addChild(bgGaf, 100001);
											//ko can main
											break; }
	case ms::BaseActivity::EGT_HOORAY:{
//                                          bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
//                                          this->addChild(bgGaf, 100001);
										  mainGaf = GAFWrapper::createGAF("gaf/end_game/horray_standalone/horray_standalone.gaf");
										  this->addChild(mainGaf, 100002);
										  AUDIO_ENGINE->playEffect(StringUtils::format("sounds/end_game/hooray%d.mp3", rand() % 4 + 1));
										  isNeedFireWork = true;
										  break; }
	case ms::BaseActivity::EGT_WELL_DONE:{
											 bgGaf = GAFWrapper::createGAF("gaf/end_game/welldone_gaf/welldone_gaf.gaf");
											 AUDIO_ENGINE->playEffect("sounds/end_game/welldone.mp3");
											 this->addChild(bgGaf, 100001);
											 isNeedFireWork = true;
											 //ko can main
											 break; }
	case ms::BaseActivity::EGT_CHAR:{
										/*bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
										this->addChild(bgGaf, 100001);*/
										mainGaf = GAFWrapper::createGAF(charGafPath);
										this->addChild(mainGaf, 100002);
										isNeedFireWork = true;
										break; }
	case ms::BaseActivity::EGT_COMMON:{
    
										  enum TYPE_COMMON_ENDING  {
											  TCE_PIG = 0,
                                              TCE_BearRainbow_standalone,
                                              TCE_RainbowGreenmonster,
                                              TCE_RainbowVioletmonster,
                                              TCE_RainbowPinkmonster,
                                              TCE_RainbowYellowmonster,
                                              TCE_PigFoxBearDance_standalone,
											  TCE_DINOSAUR,
											  TCE_greatjobParty4monster,
											  TCE_balloonPig,
											  TCE_GoodjobFox
										  };
                                        
										  TYPE_COMMON_ENDING type_c = (TYPE_COMMON_ENDING)(CONFIG_MANAGER->isVietnameseStories()?random(1, 5): random(0, 10));

										  if (type_c==TYPE_COMMON_ENDING::TCE_PIG){
											 /* bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/piggy_standalone/piggy_standalone.gaf");
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_DINOSAUR){
											  mainGaf = GAFWrapper::createGAF("gaf/end_game/dinosaur_gaf/dinosaur_gaf.gaf");
											  sound_text = ST_YOU_DID_IT;
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_greatjobParty4monster){
											/*  bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/greatjobParty4monster_gaf/greatjobParty4monster_gaf.gaf");
											  ancho = ANCHO_GAF::AG_MID;
											  sound_text = ST_GREAT_JOB;
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_RainbowGreenmonster){
											 /* bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/RainbowGreenmonster_gaf/RainbowGreenmonster_gaf.gaf");
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_RainbowVioletmonster){
											  mainGaf = GAFWrapper::createGAF("gaf/end_game/RainbowVioletmonster_gaf/RainbowVioletmonster_gaf.gaf");
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_RainbowPinkmonster){
											  mainGaf = GAFWrapper::createGAF("gaf/end_game/RainbowPinkmonster_gaf/RainbowPinkmonster_gaf.gaf");
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_RainbowYellowmonster){
											  mainGaf = GAFWrapper::createGAF("gaf/end_game/RainbowYellowmonster_gaf/RainbowYellowmonster_gaf.gaf");
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_balloonPig){
											 /* bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/balloonPig_gaf/balloonPig_gaf.gaf");
											  ancho = ANCHO_GAF::AG_MID;
											  sound_text = ST_WELL_DONE;
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_GoodjobFox){
											 /* bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/GoodjobFox_gaf/GoodjobFox_gaf.gaf");
											  ancho = ANCHO_GAF::AG_MID;
											  sound_text = ST_GOOD_JOB;
											  this->addChild(mainGaf, 100002);
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_PigFoxBearDance_standalone){
											  /*bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/PigFoxBearDance_standalone/PigFoxBearDance_standalone.gaf");
											  this->addChild(mainGaf, 100002);
											  isNeedtextBonus = true;
										  }
										  else if (type_c == TYPE_COMMON_ENDING::TCE_BearRainbow_standalone){
											 /* bgGaf = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
											  this->addChild(bgGaf, 100001);*/

											  mainGaf = GAFWrapper::createGAF("gaf/end_game/BearRainbow_standalone/BearRainbow_standalone.gaf");
											  this->addChild(mainGaf, 100002);
											  isNeedtextBonus = true;
										  }
										 
										  isNeedFireWork = true;
										  break; }

	}
	if (bgGaf){
//        bgGaf->setPosition(Director::getInstance()->getVisibleSize() / 2);
//        
//        bgGaf->play(mainGaf!=nullptr, [=](gaf::GAFObject* obj){
//            if (!mainGaf){
//                this->scheduleOnce([=](float dt){
//                    _activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
//                }, 0.5f, "finish_game");
//            }
//            if (mainGaf == nullptr){
//                bgGaf->runAction(Sequence::createWithTwoActions(FadeOut::create(0.2f),RemoveSelf::create()));
//            }
//        });
	}
    if (CONFIG_MANAGER->isVietnameseStories()){
        isNeedtextBonus = false;
        sound_text = SOUND_TEXT::ST_VM;
        if (is_INTERRUPTED){
            sound_text = SOUND_TEXT::ST_NONE;
        }
    }
	if (mainGaf){
		if (ancho == ANCHO_GAF::AG_MID)
			mainGaf->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
		else
			mainGaf->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height - mainGaf->getContentSize().height / 2));

		mainGaf->play(false, [=](gaf::GAFObject* obj){
            this->resume();
			this->scheduleOnce([=](float dt){
				_activitydelegate ? (is_INTERRUPTED?_activitydelegate->onActivityEnd(this,ms::delegate::Activity::EndReason::INTERRUPTED):_activitydelegate->onActivityEnd(this)) : (void) nullptr;
				//this->removeAllChildren();
				//commonEndGame(END_GAME_TYPE::EGT_COMMON);
			}, 0.5f, "finish_game");
			if (bgGaf)
				bgGaf->runAction(Sequence::createWithTwoActions(FadeOut::create(0.2f), RemoveSelf::create()));
			//mainGaf->removeFromParent();
		});
    }else {
        this->resume();
        this->scheduleOnce([=](float dt){
            _activitydelegate ? (is_INTERRUPTED?_activitydelegate->onActivityEnd(this,ms::delegate::Activity::EndReason::INTERRUPTED):_activitydelegate->onActivityEnd(this)) : (void) nullptr;
			//this->removeAllChildren();
			//commonEndGame(END_GAME_TYPE::EGT_COMMON);
        }, 0.5f, "finish_game");
    }
	if (isNeedtextBonus){
		if (typeBonus == 0){
			textBonusGaf = GAFWrapper::createGAF("gaf/end_game/goodjob_Standalone/goodjob_Standalone.gaf");
			AUDIO_ENGINE->playEffect("sounds/end_game/goodjob_1.mp3");
		}
		else if (typeBonus == 1){
			textBonusGaf = GAFWrapper::createGAF("gaf/end_game/horray_standalone/horray_standalone.gaf");
			AUDIO_ENGINE->playEffect(StringUtils::format("sounds/end_game/hooray%d.mp3", rand() % 5 + 1));
		}
		else if (typeBonus == 2){
			textBonusGaf = GAFWrapper::createGAF("gaf/end_game/welldone_Standalone_gaf/welldone_Standalone_gaf.gaf");
			AUDIO_ENGINE->playEffect("sounds/end_game/welldone.mp3");
		}
		else if (typeBonus == 3){// ko co gaf awesome , ko co sound yumy
			textBonusGaf = GAFWrapper::createGAF("gaf/end_game/Yummy_standalone/Yummy_standalone.gaf");
		}
		if (textBonusGaf){
			this->addChild(textBonusGaf, 100002);
			textBonusGaf->setPositionX(Director::getInstance()->getVisibleSize().width*0.5);
			textBonusGaf->setPositionY(Director::getInstance()->getVisibleSize().height*0.75);
			textBonusGaf->play(false, [=](gaf::GAFObject* obj){
				textBonusGaf->removeFromParent();
			});
		}
	}

	if (sound_text != SOUND_TEXT::ST_NONE){
		switch (sound_text)
		{
		case ST_NONE:{
						 if (!isNeedtextBonus){
							 if (typeBonus == 0){
								 AUDIO_ENGINE->playEffect("sounds/end_game/goodjob_1.mp3");
							 }
							 else if (typeBonus == 1){
								 AUDIO_ENGINE->playEffect(StringUtils::format("sounds/end_game/hooray%d.mp3", rand() % 5 + 1));
							 }
							 else if (typeBonus == 2){
								 AUDIO_ENGINE->playEffect("sounds/end_game/welldone.mp3");
							 }
							 else if (typeBonus == 3){// ko co gaf awesome , ko co sound yumy
								 AUDIO_ENGINE->playEffect("sounds/end_game/yeah.mp3");
							 }
							 
						 }
						 break;
		}
		case ST_YOU_DID_IT:
			break;
		case ST_GREAT_JOB:
			break;
		case ST_WELL_DONE:{
							  AUDIO_ENGINE->playEffect("sounds/end_game/welldone.mp3");
							  break; }
		case ST_GOOD_JOB:{
							 AUDIO_ENGINE->playEffect("sounds/end_game/goodjob_1.mp3");
							 break; }
        case ST_VM:{
                            AUDIO_ENGINE->playEffect(cocos2d::random(0,1)?"sounds/end_game/Gioi qua.mp3":"sounds/end_game/Tuyet voi.mp3");
                            break; }
		default:
			break;
		}
	}
	
	if (isNeedFireWork){
		std::vector<Vec2> posFirework;
		posFirework.push_back(Vec2(Director::getInstance()->getVisibleSize().width*0.65f, Director::getInstance()->getVisibleSize().height*0.4f));
		posFirework.push_back(Vec2(Director::getInstance()->getVisibleSize().width*0.3f, Director::getInstance()->getVisibleSize().height*0.65f));
		posFirework.push_back(Vec2(Director::getInstance()->getVisibleSize().width*0.4f, Director::getInstance()->getVisibleSize().height*0.3f));
		for (int i = 0; i < 3; i++)	{
			auto phaoHoa = GAFWrapper::createGAF("gaf/end_game/phaoHoaNo_standalone/phaoHoaNo_standalone.gaf");
			phaoHoa->setVisible(false);
			this->addChild(phaoHoa,100001);
			phaoHoa->setPosition(posFirework.at(i));
			phaoHoa->runAction(Sequence::create(DelayTime::create(0.5f + 0.7f*i), CallFunc::create([=](){
				phaoHoa->setVisible(true);
				phaoHoa->play(false, [=](gaf::GAFObject* obj){
					phaoHoa->removeFromParent();
				});
			})
				,nullptr));
		}
	}

	Layout* blackLayout = Layout::create();
	blackLayout->setContentSize(Director::getInstance()->getVisibleSize());
	blackLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	blackLayout->setBackGroundColor(Color3B::BLACK);
	blackLayout->setOpacity(0);
	blackLayout->setTouchEnabled(true);
	blackLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	blackLayout->setPosition(Director::getInstance()->getVisibleSize()/2);
	this->addChild(blackLayout, 100000);

	blackLayout->runAction(cocos2d::FadeTo::create(0.2f, 180));

	AUDIO_ENGINE->playEffect("sounds/end_game/Cheer_2.mp3");

	this->startParticle(1, this);

}
#pragma mark - PAUSE ACTIVITY
StorySubscribePopup* ms::BaseActivity::pauseGame(bool isPause, StorySubscribePopup::SUB_POPUP_TYPE type) {
	if (auto subscirbe_popup_old = cocos2d::utils::findChild<StorySubscribePopup*>(this, "subscirbe_popup"))
	{
		return subscirbe_popup_old;
	}
    auto subscirbe_popup = StorySubscribePopup::createSubscribePopup(0, type);
	subscirbe_popup->setName("subscirbe_popup");
    this->addChild(subscirbe_popup, 1999999999);
    subscirbe_popup->setDelegate(this);
    subscirbe_popup->onShowHidePopUp(true);
    MJDEFAULT->setBoolForKey("new_subscirbe_popup_comfim", true);
	return subscirbe_popup;
    
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(this->getContentSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(this->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(this->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
}

void ms::BaseActivity::onSubscribeOk() {
//    commonEndGame(ms::BaseActivity::EGT_COMMON);
    pauseActivity();
    onCloseGame();//to override;
    if(_activitydelegate)
        _activitydelegate->onActivityEnd(this, ms::delegate::Activity::EndReason::INTERRUPTED);
    AUDIO_ENGINE->stopAllEffects();
    StorySubscribePopupDelegate::onSubscribeOk();
}

void ms::BaseActivity::onSubscribeFree() {
    //    commonEndGame(ms::BaseActivity::EGT_COMMON);
    pauseActivity();
    onCloseGame();//to override;
    if(_activitydelegate)
        _activitydelegate->onActivityEnd(this, ms::delegate::Activity::EndReason::INTERRUPTED);
    AUDIO_ENGINE->stopAllEffects();
    StorySubscribePopupDelegate::onSubscribeFree();
}

void ms::BaseActivity::onSubscribeCancel() {
    if (CONFIG_MANAGER->isVietnameseStories()){
        commonEndGame(ms::BaseActivity::EGT_COMMON,"",false,true);
    }else{
        commonEndGame(ms::BaseActivity::EGT_COMMON);
    }
    
}

void ms::BaseActivity::onCloseGame(){
        AUDIO_ENGINE->stopAllSound();
     if(CONFIG_MANAGER->isVietnameseStories()){
         onCloseGameVmonkey(true);
     }
	 mj::helper::SetFpsByNumberFrames(10);
}

void ms::BaseActivity::onCloseGameVmonkey(bool interrupted){
    auto parent = dynamic_cast<StoryPageVC *>(this->getParent()->getParent());
    if (parent) {
        parent->showGameOver(interrupted);
    }else{
        commonEndGame(EGT_COMMON,"",false,interrupted);
    }
}
