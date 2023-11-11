//
//  GameBase.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/24/17.
//
//

#include "GameBase.h"
#include "AudioEngine.h"

#include "GameQA.h"
#include "GameMakeAMatch.h"
#include "GameJumpingFrog.h"
#include "GameMatching.h"
#include "GameBuildSentence.h"
#include "GameLandThePlane.h"
#include "GameLazer.h"
#include "GameChickensToCoop.h"
#include "GameBirdie.h"
#include "GameWordMachine.h"
#include "StoryPageVC.h"
#include "HSAudioEngine.h"
#include "MJFirebaseListener.h"
#include "HSLib.h"

#include "GameBuildWall.h"
#include "MSEventLogger.h"

#define default_time_delay 1.f

USING_NS_CC;

namespace mj {
    namespace game {

        MJGBase * createGameFromJSON(std::string file, std::string resource);
        MJGBase * createGameFromPlist(std::string plist, std::string resource);
        int getLevelIDFromLevelString(std::string level_string) {
            if (level_string == "A") {
                return 0;
            } else if (level_string == "B") {
                return 1;
            } else if (level_string == "C") {
                return 2;
            } else if (level_string == "D") {
                return 3;
            } else if (level_string == "E") {
                return 4;
            } else if (level_string == "F") {
                return 5;
            }else if (level_string == "G") {
                return 6;
            }else if (level_string == "H") {
                return 7;
            }else if (level_string == "I") {
                return 8;
            }else if (level_string == "J") {
                return 9;
            }else if (level_string == "K") {
                return 10;
            }else if (level_string == "L") {
                return 11;
            }else if (level_string == "M") {
                return 12;
            }
            return 0;
        }
        
        MJGBase * createGameFromFile(std::string file, std::string resource) {
            assert(FileUtils::getInstance()->isFileExist(file) && "Properties List must exist.");
            
            if (!resource.empty() && resource[resource.size() - 1] != '/') {
                resource += "/";
            }
            
            auto extension = FileUtils::getInstance()->getFileExtension(file);
            if (extension == ".plist") {
                return createGameFromPlist(file, resource);
            }
            if (extension == ".json") {
                return createGameFromJSON(file, resource);
            }
            
            return nullptr;
        }
        
        MJGBase * createGameFromJSON(std::string file, std::string resource) {
            file = "plist/game_config.json";
            auto buffer_string = FileUtils::getInstance()->getStringFromFile(file);
            rapidjson::Document value_data;
            value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
            if (value_data.HasParseError()) {
                CCLOG("JSON ERROR!");
                return nullptr;
            }
            
            // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            rapidjson::StringBuffer rj_buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
            value_data.Accept(writer);
            CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif  
            auto gamedict = value_data["game"].GetObject();
            mj::GameInfo game_info;
            game_info.game_id = gamedict["id"].GetInt();
            
            game_info.title = {
                gamedict["title"].GetString(),
                "",
                resource + gamedict["audio_title"].GetString()
            };
            
            auto level_string = gamedict["level_story"].GetString();
            game_info.level = getLevelIDFromLevelString(level_string);
            
            auto v_instructions = gamedict["audio_intro"].GetArray();
            for (int i = 0; i < (int)v_instructions.Size(); i++) {
                auto audio = v_instructions[i].GetString();
                game_info.instructions.push_back({
                    "",
                    "",
                    resource + audio
                });
            }
            
            MJGBase * game = nullptr;
            switch ((mj::GameType)game_info.game_id) {
                case mj::GameType::BUILD_SENTENCE:
                    game = createGameFromJSonWithGameInfo<MJGameBuildSentence, mj::BuildSentenceDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::PLUG_IN_BULBS_TT:
                    game_info.extras.insert({"plug_bulbs_type", "text_text"});
                    game = createGameFromJSonWithGameInfo<MJGameLazer, mj::LazerGameDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::PLUG_IN_BULBS_TI:
                    game_info.extras.insert({"plug_bulbs_type", "imgage_text"});
                    game = createGameFromJSonWithGameInfo<MJGameLazer, mj::LazerGameDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::LAND_THE_PLANE:
                    game = createGameFromJSonWithGameInfo<MJGameLandThePlane, mj::LandThePlaneDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::LIGHT_THEM_UP:
                    game_info.extras.insert({"matching_6_type", "light"});
                    game = createGameFromJSonWithGameInfo<MJGameMatching, mj::MatchingGameDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::PICTURE_MATCH:
                    game_info.extras.insert({"matching_6_type", "sea"});
                    game = createGameFromJSonWithGameInfo<MJGameMatching, mj::MatchingGameDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::SING_BIRDIE:
                    game = createGameFromJSonWithGameInfo<MJGameBirdie, mj::BirdieDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::WORD_MACHINE:
                    game = createGameFromJSonWithGameInfo<MJGameWordMachine, mj::WordMachineDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::CHICKENS_TO_COOP:
                    game = createGameFromJSonWithGameInfo<MJGameChickensToCoop, mj::ChickensToCoopDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::MAKE_A_MATCH_TT:
                case mj::GameType::MAKE_A_MATCH_TI:
                    game = createGameFromJSonWithGameInfo<MJGameMakeAMatch, mj::MakeAMatchDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::JUMPING_FROG:
                    game = createGameFromJSonWithGameInfo<MJGameJumpingFrog, mj::JumpingFrogDataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::Q_AND_A:
                    game = createGameFromJSonWithGameInfo<MJGameQA, mj::QADataSet>(value_data.GetObject(), game_info, resource);
                    break;
                    
                case mj::GameType::BUILD_WALL:
                    break;
                    
                default:
                    assert(false && "Game id does not match");
                    break;
            }
            
            if (game) {
				game->setActivityDelegate(nullptr);
                game->setStartDelay(default_time_delay);
            }

            
            return game;
        }
        
        MJGBase * createGameFromPlist(std::string plist, std::string resource) {
            assert(FileUtils::getInstance()->isFileExist(plist) && "Properties List must exist.");

            auto valuemap = FileUtils::getInstance()->getValueMapFromFile(plist);
            auto gamedict = valuemap["game"].asValueMap();

            mj::GameInfo game_info;
            game_info.game_id = gamedict["id"].asInt();
            game_info.title = {
                gamedict["title"].asString(),
                "",
                resource + gamedict["audio_title"].asString()
            };

            game_info.level = 0;
            auto level_string = gamedict["level_story"].asString();
            game_info.level = getLevelIDFromLevelString(level_string);
            

            auto v_instructions = gamedict["audio_intro"].asValueVector();
            for (auto it : v_instructions) {
                auto audio = it.asString();
                game_info.instructions.push_back({
                    "",
                    "",
                    resource + audio
                });
            }

            auto story_id = gamedict["id_story"].asInt();
            auto lang_id = gamedict["lang_id"].asInt();

            auto plist_name = StringUtils::format("%d_%d.plist", story_id, lang_id);
            if (FileUtils::getInstance()->isFileExist(plist_name)) {
                auto temp = FileUtils::getInstance()->getValueVectorFromFile(plist_name);
                auto story_info_vm = temp[0].asValueMap();
                game_info.story_title = story_info_vm["name"].asString();
            }

            MJGBase * game = nullptr;
            switch ((mj::GameType)game_info.game_id) {
                case mj::GameType::BUILD_SENTENCE:
                    game = createGameFromPlistWithGameInfo<MJGameBuildSentence, mj::BuildSentenceDataSet>(valuemap, game_info, resource);
                    break;
                    
                case mj::GameType::PLUG_IN_BULBS_TT:
                    game_info.extras.insert({"plug_bulbs_type", "text_text"});
                    game = createGameFromPlistWithGameInfo<MJGameLazer, mj::LazerGameDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::PLUG_IN_BULBS_TI:
                    game_info.extras.insert({"plug_bulbs_type", "imgage_text"});
                    game = createGameFromPlistWithGameInfo<MJGameLazer, mj::LazerGameDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::LAND_THE_PLANE:
                    game = createGameFromPlistWithGameInfo<MJGameLandThePlane, mj::LandThePlaneDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::LIGHT_THEM_UP:
                    game_info.extras.insert({"matching_6_type", "light"});
                    game = createGameFromPlistWithGameInfo<MJGameMatching, mj::MatchingGameDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::PICTURE_MATCH:
                    game_info.extras.insert({"matching_6_type", "sea"});
                    game = createGameFromPlistWithGameInfo<MJGameMatching, mj::MatchingGameDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::SING_BIRDIE:
                    game = createGameFromPlistWithGameInfo<MJGameBirdie, mj::BirdieDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::WORD_MACHINE:
                    game = createGameFromPlistWithGameInfo<MJGameWordMachine, mj::WordMachineDataSet>(valuemap, game_info, resource);
                    break;
                    
                case mj::GameType::CHICKENS_TO_COOP:
                    game = createGameFromPlistWithGameInfo<MJGameChickensToCoop, mj::ChickensToCoopDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::MAKE_A_MATCH_TT:
                case mj::GameType::MAKE_A_MATCH_TI:
                    game = createGameFromPlistWithGameInfo<MJGameMakeAMatch, mj::MakeAMatchDataSet>(valuemap, game_info, resource);
                    break;

                case mj::GameType::JUMPING_FROG:
                    game = createGameFromPlistWithGameInfo<MJGameJumpingFrog, mj::JumpingFrogDataSet>(valuemap, game_info, resource);
                    break;
                case mj::GameType::Q_AND_A:
                    game = createGameFromPlistWithGameInfo<MJGameQA, mj::QADataSet>(valuemap, game_info, resource);
                    break;

                default:
                    assert(false && "Game id does not match");
                    break;
            }
            
            if (game) {
				game->setActivityDelegate(nullptr);
                game->setStartDelay(default_time_delay);
                game->setGameInfo(game_info);
            }

            return game;
        }
    }
}

void MJGBase::onEnter(){
    ms::BaseActivity::onEnter();
}

template <class T, typename D>
void MJGameBase<T, D>::onGameOver(bool is_interrupt){
    if (is_interrupt) {
        ValueMap _dict_event;
        _dict_event["id_story"] = StringUtils::toString(XHSLib->_id);
        _dict_event["id_game"] = StringUtils::toString(_data.info.game_id);
        _dict_event["event_detail"] = analytics_event_close_game;
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsGameEvent(analytics_event_game, _dict_event);
        _dict_event.clear();
    }else {
        ValueMap _dict_event;
        _dict_event["id_story"] = StringUtils::toString(XHSLib->_id);
        _dict_event["id_game"] = StringUtils::toString(_data.info.game_id);
        _dict_event["event_detail"] = analytics_event_done_game;
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsGameEvent(analytics_event_game, _dict_event);
        auto game_id = this->_data.info.game_id; //hưng sửa (đã hỏi anh hội)
        auto game_title = mj::TITLE_GAME_DEFAULT.at(game_id);
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishGameInStory, {
            {ms::analytics::kPropertyGameType, cocos2d::Value(game_title)}
        });
        _dict_event.clear();
    }
    auto parent = dynamic_cast<StoryPageVC *>(this->getParent());
    if (parent) {
        if (is_interrupt) parent->showGameOver(is_interrupt);
        else parent->showGameOver();
    }
    this->pause();
    this->enumerateChildren("//.*", [](Node * child) {
        if (child->getName() != "particle") {
            child->pause();
        }
        return false;
    });
    AUDIO_ENGINE->stopBackgroundMusic();
    AUDIO_ENGINE->stopAllEffects();
    if (_activitydelegate) {
		if (is_interrupt)
            if (CONFIG_MANAGER->isVietnameseStories()){
                onCloseGameVmonkey();
            }else{
                _activitydelegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FINISHED);
                
            }
		else{
			switch ((mj::GameType)_data.info.game_id)
			{
			case mj::WORD_MACHINE:
			case mj::Q_AND_A:
			case mj::MAKE_A_MATCH_TT:
			case mj::MAKE_A_MATCH_TI:
			case mj::CHICKENS_TO_COOP:
			case mj::PICTURE_MATCH:
			case mj::PLUG_IN_BULBS_TT:
			case mj::PLUG_IN_BULBS_TI:
			case mj::LIGHT_THEM_UP:
			case mj::BUILD_SENTENCE:
			case mj::SING_BIRDIE:{
                                    if (CONFIG_MANAGER->isVietnameseStories()){
                                       onCloseGameVmonkey();
                                   }else{
                                       commonEndGame(ms::BaseActivity::EGT_COMMON);
                                       
                                   }
									 break; }
			/*case mj::BUILD_SENTENCE:{
										commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/Owl_standalone/Owl_standalone.gaf", true);
										break;}*/
			default:{
                    if (CONFIG_MANAGER->isVietnameseStories()){
                        onCloseGameVmonkey();
                    }else{
                        _activitydelegate->onActivityEnd(this, ms::delegate::Activity::EndReason::FINISHED);
                    }
                break;
                }
			}
		}
    }
}

template <class T, typename D>
void MJGameBase<T, D>::onEnter() {
    MJGBase::onEnter();
    ValueMap _dict_event;
    _dict_event["id_story"] = StringUtils::toString(XHSLib->_id);
    _dict_event["id_game"] = StringUtils::toString(_data.info.game_id);
    _dict_event["event_detail"] = analytics_event_play_game;
    mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsGameEvent(analytics_event_game, _dict_event);
    _dict_event.clear();
    

    this->setClippingEnabled(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        this->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
        this->setAnchorPoint(Vec2(0, 0));
        this->setPosition(Vec2(0, 0));
    }
#endif

    auto title_path = _data.info.title.audio_path;
    if (_data.info.title.audio_path.empty()) {
        title_path = mj::TITLE_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
    }

    if (!cocos2d::FileUtils::getInstance()->isFileExist(_data.info.title.audio_path)) {
        title_path = mj::TITLE_AUDIO_PATH_DEFAULT.at(_data.info.game_id);
    }
    
    if(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false)){
        _time_delay = 0;
    }else{
        if (!(CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey())){
            this->getParent()->scheduleOnce([=](float dt) {
                cocos2d::experimental::AudioEngine::play2d(title_path);
            }, _time_delay, "play_title_game");
        }
    }
}

template class MJGameBase<MJGameQA, mj::QADataSet>;
template class MJGameBase<MJGameMakeAMatch, mj::MakeAMatchDataSet>;
template class MJGameBase<MJGameMatching, mj::MatchingGameDataSet>;
template class MJGameBase<MJGameLandThePlane, mj::LandThePlaneDataSet>;
template class MJGameBase<MJGameLazer, mj::LazerGameDataSet>;
template class MJGameBase<MJGameBirdie, mj::BirdieDataSet>;
template class MJGameBase<MJGameChickensToCoop, mj::ChickensToCoopDataSet>;
template class MJGameBase<MJGameJumpingFrog, mj::JumpingFrogDataSet>;
template class MJGameBase<MJGameBuildSentence, mj::BuildSentenceDataSet>;
template class MJGameBase<MJGameWordMachine, mj::WordMachineDataSet>;
