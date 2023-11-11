#ifndef __HouseOfWord_H__
#define __HouseOfWord_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HouseOfWordData.h"
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

namespace houseofwords {

	enum GAME_HOUSE_OF_WORD_INTRO
	{
		GHW_CHARACTER = 0,
		GHW_CHARACTERS = 1,
		GHW_IDEA = 2,
		GHW_SETTING = 3,
		GHW_LESSON = 4
	};
	const std::unordered_map<int, std::string> INSTRUCTION_TEXT_DEFAULT = {
		{ GHW_CHARACTER, "The main character is the most important person or animal in the story." },
		{ GHW_CHARACTERS, "The main character is the most important person or animal in the story." },
		{ GHW_IDEA, "The main idea is the most important point of the story; it tells the reader what the story is about." },
		{ GHW_SETTING, "The setting of the story is where and when the story happens." },
		{ GHW_LESSON, "The story's moral lesson is a message that is conveyed or a lesson to be learned from the story." },
	};
	const std::unordered_map<int, std::string> INSTRUCTION_AUDIO_PATH_DEFAULT = {
		{ GHW_CHARACTER, "sounds/house_of_words/intro_1.mp3" },
		{ GHW_CHARACTERS, "sounds/house_of_words/intro_1.mp3" },
		{ GHW_IDEA, "sounds/house_of_words/intro_3.mp3" },
		{ GHW_SETTING, "sounds/house_of_words/intro_4.mp3" },
		{ GHW_LESSON, "sounds/house_of_words/intro_5.mp3" },
	};
	const std::unordered_map<int, std::string> QUESTION_TEXT_DEFAULT = {
		{ GHW_CHARACTER, "Choose the main character of the story." },
		{ GHW_CHARACTERS, "Choose the main characters of the story." },
		{ GHW_IDEA, "What is the main idea of the story?" },
		{ GHW_SETTING, "What is the main setting of the story?" },
		{ GHW_LESSON, "What is the story's moral lesson?" },
	};
	const std::unordered_map<int, std::string> QUESTION_AUDIO_PATH_DEFAULT = {
		{ GHW_CHARACTER, "sounds/house_of_words/quest_1.mp3" },
		{ GHW_CHARACTERS, "sounds/house_of_words/quest_2.mp3" },
		{ GHW_IDEA, "sounds/house_of_words/quest_3.mp3" },
		{ GHW_SETTING, "sounds/house_of_words/quest_4.mp3" },
		{ GHW_LESSON, "sounds/house_of_words/quest_5.mp3" },
	};
    class HouseOfWord : public ms::BaseActivity {
    private:
        int _currentIndex = 0;
		bool _enableTouchImage = false;			// when sound options done then enable sound when click character image
		bool _enableSugesstion = false;			// if true -> show light and text, else hide
        vector<houseofwords::LevelData> _allLevels;
        ImageView *_background = nullptr, *_cloud1 = nullptr, *_cloud2 = nullptr;
        Layout *_question = nullptr;
        Layout *_infoBar = nullptr;
		Layout* _bound = nullptr;
        ImageView *_lightButton = nullptr;
		int _infoAudioInt = 0;



		void initOptions(std::vector<math::resource::Image*> images);
		vector<houseofwords::LevelData> loadLevelData(std::vector<math::resource::Image*> images);
        Layout* makeQuestion();
        void scrollCloud(float dt);
        void showNextLevel();
        void PlayLate(int index, vector<Label*> answers);
		void schedulePlaySoundGuiding();
        bool canTouch = false;
        std::string json_path;
        //bool game_play_enable = true;
        //void pauseGame();
        void onEnter() override;
        
        bool _is_guide_showed = false;
        cocos2d::Point _guide_point;
    public:

        HouseOfWord() {};

        ~HouseOfWord() {};

		static HouseOfWord* createGame(std::vector<math::resource::Image*> images);

        void onAnswerClick(Ref *sender, ui::Widget::TouchEventType event);
		void onTouchImageAndWords(Ref* sender, ui::Widget::TouchEventType event);
    };
}
#endif // __GAME_PLAY_H__
