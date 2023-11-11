//
//  WordTv.h
//

#ifndef WordTv_h
#define WordTv_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "WordTv_Item.h"
#include "PhonicInfoModel.h"
#include "WordTv_Led.h"
#include "WordTv_Tv.h"

using namespace cocos2d;
class WordTv : public MJMainGBase
{
public:
    CREATE_FUNC(WordTv);
    
    enum CAR_STATE{
        CS_IDLE,
        CS_DROP,
        CS_RUN_OUT,
        CS_RUN_IN
    };
    
    struct WORD_INFO{
        std::vector<mj::PHONIC_WORD> characters;
        std::vector<mj::GEAR_ITEM> items;
    };
    
    struct GAME_INFO {
        int number_level;
        std::vector<WORD_INFO> word_infos;
    };
    
    static WordTv *createGame(std::string json_file);
    
private:
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data);
    GAME_INFO game_info;
    
    void onEnter() override;
    void onExitTransitionDidStar();
    
    void showIntroGame();
    void loadFrames();
    void startGame();
    
    void startLevel();
    void showItems();
    void onExit() override;
    cocos2d::ui::Layout* touchLayout = nullptr, * tvLayout = nullptr, * itemLayout = nullptr, * textLayout = nullptr, * posTextLayout = nullptr;
    std::vector<WordTv_Tv*> Tvs = {};
    std::vector<WordTv_Item*> items = {};
    std::vector<Text*> texts = {};
    std::vector<float> widths = {};
    bool _isAudioIntroGameStart = false;

    WordTv_Led* led = nullptr;
    
    Sprite* bg1 = nullptr, *bg2 = nullptr, *bg3 = nullptr;
    
    int index_item = 0, cur_level = 0, curZ = 0;

    bool _isStartGame = true;

	void inActiveMode(float delay_time = 7.0f);
    std::string current_intro = "";

    //speaker and learning reminder
    bool _touchable = false;
    int _introSpeakerSoundId = -1;
    void stopIntroAudioWhenPlayGame();
    void loadTVIcon();

    float _delaytimeAudioGuidePlayback = 10;
    std::string _audioGuidePlaybackScheduleName = "audio guide playback";
    //end speaker
};
CREATE_READER(WordTv);
#endif /* MJ_WordTv_h */
