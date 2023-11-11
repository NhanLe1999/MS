#ifndef __sequencing_H__
#define __sequencing_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SequencingData.h"
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

namespace sequencing {
    class DroppableTile : public ImageView
    {
    private:
        void initOptions(int index);
    public:
        DroppableTile() {};
        ~DroppableTile() {};
        static DroppableTile* create(int index);
        bool isOverlap(Rect otherRect);
    };

    class ImageTile : public ImageView
    {
    private:
        GAFObject *_tickObject;
        void initOptions();
    public:
        DroppableTile* holder;
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
        int _currentIndex = 0;
        vector<LevelData> _allLevels;
        GAFObject *_khiObject = nullptr, *_paperFlyObject = nullptr;
        vector<ImageTile*> _images;
        vector<DroppableTile*> _droppable;
        ImageTile *_dragTile = NULL;
        Layout* _overlay = nullptr;
        int _detailsAudio = 0;

		void initOptions(std::vector<math::resource::Image*> images);
		vector<LevelData> loadLevelData(std::vector<math::resource::Image*> images);
        void applyImageEnterAnim(int index1, int index2);
        void applyImageInPosition(int index1, int index2);
        void zoomIn(ImageTile* , bool inFinish = false);
		void zoomOut(ImageTile* img, bool inFinish = false);
        bool canTouch;
		bool canTouchOut = false;
        std::string json_file;
        //bool game_play_enable = true;
        //void pauseGame();
		Vector<ImageTile*> sorted_imgs;
		void showSequence();
		void onFinish();
        
        void showHand();
        void inactiveMode();
        void scheduleInactiveMode();
        bool _is_guide_showed= false;
        bool _image_zoom = false;
        int _countDraggedWrongImage = 0;
        int _currentAudio;
        cocos2d::ui::Button* _speaker;
        int countImageLeft = 0;//dem so anh con la`i chua duoc keo vao o dap an 

    public:

        GamePlay() {};

        ~GamePlay() {};

		static GamePlay* createGame(std::vector<math::resource::Image*> images);
        virtual void onEnter() override;
        virtual void onExit();

    };
}
#endif // __GAME_PLAY_H__
