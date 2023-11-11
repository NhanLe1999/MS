//
//  GameBase.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/24/17.
//
//

#ifndef GameBase_hpp
#define GameBase_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "MJHelper.h"
#include "AudioEngine.h"
#include "GameInstructionOverlay.h"
#include "StoryConfigManager.h"
#include "MJBaseActivity.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "PhonicConfig.h"
//

// template class for any mini game

class MJGBase : public ms::BaseActivity {
public:
    CC_SYNTHESIZE(float, _time_delay, StartDelay);
    CC_SYNTHESIZE(mj::GameInfo, _game_info, GameInfo);
    void onEnter() override;
};

template <class T, typename D>
class MJGameBase : public MJGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    /**
     * create game with a data set
     */
	static MJGameBase * createGame(mj::GameDataSet<D> data) {
		return T::createGame(data);
	};

    /**
     * create game from a plist configuration
     */
	static mj::GameDataSet<D> generateData(cocos2d::ValueMap value_map, std::string resource) {
		return T::generateData(value_map, resource);
	}

    /**
     * create game from a json configuration
     */
	static mj::GameDataSet<D> generateData(rapidjson::Value value_data, std::string resource) {
		return T::generateData(value_data.GetObject(), resource);
	}

	static mj::GameDataSet<D> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images) {
		return T::generateData(value_data.GetObject(), images);
	}

	void onEnter() override;
	virtual void onGameOver(bool is_interrupt = false);

protected:
    mj::GameDataSet<D> _data;
};

// helper functions to create game
namespace mj {
    namespace game {
        MJGBase * createGameFromFile(std::string file, std::string resource = "");
        int getLevelIDFromLevelString(std::string level_string);
        template <class T, typename D>
        MJGBase * createGameFromPlistWithGameInfo(cocos2d::ValueMap value_map, mj::GameInfo info, std::string resource = "") {
            auto data = MJGameBase<T, D>::generateData(value_map, resource);
            data.info = info;
            return MJGameBase<T, D>::createGame(data);
        }
        
        template <class T, typename D>
        MJGBase * createGameFromJSonWithGameInfo(rapidjson::Value value_data, mj::GameInfo info, std::string resource = "") {
            auto data = MJGameBase<T, D>::generateData(value_data.GetObject(), resource);
            data.info = info;
            auto game = MJGameBase<T, D>::createGame(data);
            return game;
        }


		template <class T, typename D>
		MJGBase * createGameFromJSon(rapidjson::Value value_data, GameType game_id, std::vector<math::resource::Image*> images) {
			std::vector<GameObject> intros;
			if (value_data.GetObject().HasMember("audio_intro")){

                if (JSON_SAFE_CHECK(value_data,"audio_intro",Array))
                {
                    auto v_instructions = value_data["audio_intro"].GetArray();
                    std::vector<math::resource::Image*> img_audio;
                    img_audio.clear();
                    for (int i = 0; i < (int)v_instructions.Size(); i++) {
                        auto img = math::resource::ResHelper::getImage(images, v_instructions[i].GetString());
                        img_audio.push_back(img);
                    }
                    for (int i = 0; i < img_audio.size(); i++){
                        for (int j = i; j < img_audio.size(); j++){
                            if (img_audio[i]->getComparisionValue("number") > img_audio[j]->getComparisionValue("number")){
                                std::swap(img_audio[i],img_audio[j]);
                            }
                        }
                    }
                    for (int i = 0; i < img_audio.size(); i++){
                        intros.push_back({
                            "",
                            "",
                            img_audio[i]->getAudio()
                        });
                    }
                }
                else
                {
                    auto audio = math::resource::ResHelper::getImage(images, value_data["audio_intro"].GetString())->getAudio();
                    intros.push_back({
                        "",
                        "",
                        audio
                    });
                }
				
			}
			auto data = MJGameBase<T, D>::generateData(value_data.GetObject(), images);
			data.info.game_id = game_id;
            
            if(!intros.empty() && CONFIG_MANAGER->isVietnameseStories())
            {
                data.info.instructions = intros;
            }
			else if (intros.size() >= data.info.instructions.size())
            {
                data.info.instructions = intros;
            }
			
            if (game_id == mj::GameType::LIGHT_THEM_UP){
				data.info.extras.insert({ "matching_6_type", "light" });
			}
			else if (game_id == mj::GameType::PICTURE_MATCH){
				data.info.extras.insert({ "matching_6_type", "sea" });
			}
			
			auto game = MJGameBase<T, D>::createGame(data);
			if(game)
                game->setStartDelay(1.0f);
			return game;
		}
    }
}

#endif /* GameBase_hpp */
