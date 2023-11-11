//
//  SEGameplay.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#ifndef SEGameplay_hpp
#define SEGameplay_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "gaf/Library/Sources/GAF.h"
#include "MJBaseActivity.h"

NS_GAF_BEGIN
    class GAFObject;
    class GAFAsset;
NS_GAF_END

USING_NS_GAF;

using namespace cocos2d;
using namespace ui;
using namespace std;

namespace story_elements {

    struct LevelData {
        string titleText;
        string infoText;
        string infoAudio;
        string ansImage;
        string ansAudio;
    };

    class ImageTile : public ImageView
    {
    private:
        GAFObject *_tickObject = nullptr;
        void initOptions();
    public:
        ImageView* holder = nullptr;
        bool placed = false;
        Vec2 originalPos;
        int audioIndex;

        ImageTile() {};
        ~ImageTile() {};
        static ImageTile* create(string src);
        void showTick();
    };

	class GamePlay : public ms::BaseActivity {
    private:
        EventListenerTouchOneByOne *_listener = nullptr;
        vector<LevelData> _levelData;
        ImageView *_bookImage = nullptr;
        vector<ImageView*> _holder;
        vector<ImageTile*> _pictures;
        vector<ImageTile*> _picturesGuideGame;
        ImageTile *_dragTile = nullptr;
        Layout* _overlay = nullptr;
        int _infoAudio = -1;
        int _ansAudio = -1;
        bool _gameOver = false;

		void initOptions(std::vector<math::resource::Image*> images);
		vector<LevelData> loadLevelData(std::vector<math::resource::Image*> images);
		void zoomIn(ImageTile*, bool inFinish = false);
		void zoomOut(ImageTile* img, bool inFinish, float scaleImg);
        bool canTouch = false;
		bool canZoomOut = false;
        //bool game_play_enable = true;
        //void pauseGame();
        std::string _json_file;
        void onFinishGame();
		void onFinish();
        struct SEGameInfo {
            std::string title;
            std::string text;
            std::string audio;
        };
        std::vector<SEGameInfo> list_game_info;
        std::string story_name;
		int num_main_char = 1;
		int num_support = 1;
		Vector<ImageTile*> sorted_imgs;
		std::vector<std::string> audios;
        void showGuide();
        void stopShowGuide();
        void remind();
        void stopRemind();
        bool _is_guide_showed;
        
    public:
        GamePlay() {};

        ~GamePlay();

		static GamePlay * createGame(std::vector<math::resource::Image*> images);

        virtual void onEnter();

        virtual void onExit();
        float _audio = -1;
        bool flag = true;
        Layout *overLay;
        bool _flag = true;
        bool flag2 = false;
        float _defaultElementScaled = 1.f;
    };
}
#endif /* SEGameplay_hpp */
