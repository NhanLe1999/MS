#ifndef __makefireworks_H__
#define __makefireworks_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MakeFireWorksData.h"
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

namespace makefireworks {
    class GamePlay : public ms::BaseActivity {
    private:
        int _currentIndex = 0;
        vector<LevelData> _allLevels;
        GAFObject* _actor = nullptr;
        Layout* _leftAnswer = nullptr;
        Layout* _rightAnswer = nullptr;
        ImageView* _question = nullptr;
        Button* _speaker = nullptr;
        int audio_question_id = 0;


        void initOptions(std::vector<math::resource::Image*> images);

        vector<LevelData> loadLevelData(std::vector<math::resource::Image*> images);
        void showQuestion();
        void onFinishSequence(GAFObject* object, const std::string& sequenceName);
        void showNextLevel();
        void sheduleSoundGuiding();
        std::string json_file;
        bool canTouch = false;
        bool userGameContacted = false;
        //bool game_play_enable = true;
        //void pauseGame();
        void onEnter() override;
    public:

        GamePlay() {};

        ~GamePlay() {};

        static GamePlay* createGame(std::vector<math::resource::Image*> images);

        //        void onCloseGame() override;
        void onAnswerClick(Ref* sender, ui::Widget::TouchEventType event);
    };
}
#endif // __GAME_PLAY_H__
