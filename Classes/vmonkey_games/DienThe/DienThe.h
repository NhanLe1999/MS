#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "Math_Func.h"
#include "Math_Slot.h"
#include "Math_Object.h"
#include "PoolManager.h"
#include "DienThe_Item.h"
#include "DienThe_Slot.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class DienThe : public MJMainGBase, public DienThe_SlotDelegate, public DienThe_ItemDelegate {
public:
	CREATE_FUNC(DienThe);
	static DienThe* createGame();
	//    static DienThe * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);


private:
	void didLoadFromCSB();
	//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

		//data
	void onClickedSlotDelegate(cocos2d::Ref* sender) override;
	virtual void onClickedItem(cocos2d::Ref* sender) override;
	virtual void onMoveBeginItem(cocos2d::Ref* sender) override;
	virtual void onMoveEndedItem(cocos2d::Ref* sender, bool is_item_moved) override;
	struct Question {
		std::string text;
		std::string audio;
		std::string theme;
	};
	struct Word {
		math::resource::Image* image_word;
	};
	struct Sentence {
		std::string text;
		std::string audio;
		std::vector<Word> words;
		std::vector <Word> word_wrong;
	};
	struct DataGame {
		Question ques;
		std::vector<Sentence>sentences;
	};
	std::vector <DataGame> data_game;
	std::vector <Word> wrong_answers;
	std::string theme;

	ImageView* box_slots;
	ImageView* box_items;
	Button* _speaker;
	std::vector<DienThe_Item*> m_items;
	std::vector<DienThe_Slot*> m_slots;

	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void showQuestion();
	void guideGame();
	void stopTutorial();
	void stopGuideGame();
	void showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop = 100, float delay_time = 0.5f, float dt = 4.5f, std::string gaf_guide_path = "gaf/guidegame/hand_tap/hand_tap.gaf", cocos2d::ui::Layout* parentLayout = NULL);

	math::lang::MathLanguage lang;
	void showTutorial(float delay_time);
	Layout* root_layout;
	Layout* btn_sound;
	//ImageView* image_question;
	void nextGame();
	int audio_id_question;
	void onFinishGameVMonkey();
	int current_question = 0;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	vector<std::string>audios;
	vector<std::string>audio;
	int font_sz = 70;
	void showItemsAndSlots();
	std::string trimText(std::string text);
	int number_paired;
	//// < 
	int stages_number;
	//// >
	float _audio = -1;
	bool flag = true;

	std::vector<std::vector<Text*>>texts_sentences;
	std::vector<Text*>textsSentences;
	void onPair(DienThe_Item* item, DienThe_Slot* slot);
	void playChain(std::vector<std::string> paths, int startIndex = 0, std::function<void()> _callback = nullptr);
	void radio();
	float _senLength = 0;
	float _text = 0;
	bool flagText = false;
	int textFontsize = 29;
	int textSize = 0;
	bool _flag = true;
	bool _flag2 = true;
	float _sizeQuestion = 0;
	Text* _text2 = nullptr;
	float _additionalLength = 0;
	int _slotItem = 0;
	bool _flag3 = true;
	int _quantityStr = 0;
	int _countBox = 0;
};

CREATE_READER(DienThe);