#ifndef __GAME_PLAY_H__
#define __GAME_PLAY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ItemFront.h"
#include "ItemBack.h"
#include "LevelData.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "MJBaseActivity.h"
#include "GAFWrapper.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

namespace buildwords {
    class GamePlay : public ms:: BaseActivity {
    private:
        EventListenerTouchOneByOne *_listener = nullptr;
        int _currentIndex = 0;
        vector<buildword::LevelData> _allLevels;
        vector<ItemFront *> _tiles;
        vector<ItemBack *> _tilesBack;
        vector<Layout*> _captionHolder;
        Layout *_tray = nullptr;
        ItemFront *_dragTile = nullptr;

        void startGame();
        void endGame();
        vector<int> getRandomTypes(int count);
        vector<int> getRandomTypesByWords(std::vector<std::string> words);

        void shuffledAndMovePosition();

        void checkIfSolved();

        void playEachLetterSound(float &delay);

        void playEachLetterSound(float &delay, Label *label);

        void moveCaptionToCenter();

        void roundFinish();

        void initOptions(vector<buildword::LevelData> allLevels);
		void generateData(std::vector<math::resource::Image*> images);
        bool canTouch = false;
        //bool game_play_enable = true;
        //void pauseGame();
    public:

        GamePlay() {};

        ~GamePlay() {};

        static GamePlay* create(vector<buildword::LevelData> allLevels);
		static GamePlay * createGame(std::vector<math::resource::Image*> images);
        virtual void onEnter();
        virtual void onExit();
        int bg_audio_id = -1;
    };
}
#endif // __GAME_PLAY_H__
