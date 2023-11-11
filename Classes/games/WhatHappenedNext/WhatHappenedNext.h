#ifndef WhatHappenedNext_h
#define WhatHappenedNext_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
//#include "PhonicInfoModel.h"
#include "Item.h"
#include "HSAudioEngine.h"
USING_NS_CC;

struct whn_data {
	std::string right_answer_text;
	std::string right_answer_audio;
	std::string wrong_answer_text;
	std::string wrong_answer_audio;
	std::string question;
	std::string question_image;
	std::string question_audio;
};

class WhatHappenedNext : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(WhatHappenedNext);
	static WhatHappenedNext *createGame();
	void didLoadFromCSB();
    
private:
	cocos2d::ui::Layout* root_layout;
	cocos2d::ui::Layout* train;
	Node* billboard;
	Node* carriage;
	ui::ImageView* _imgCarriage;
	Node* carriage_white;
    Node* carriage_dash;
	Node* station;
	std::vector< cocos2d::ui::Layout*> list_carriage;
	cocos2d::ui::ImageView* _imageView, *_shineSprite;
	ui::Layout* _billboardWhite;
	GAFWrapper* title;
	GAFWrapper* star;
	std::vector<Item*> list_item;
	Item* item_choose, *item_correct;
	cocos2d::ui::Text* question;

	std::vector<whn_data> list_game_data;
	int stage_count = 0;
	int number_stage;

	void onEnter() override;
	void generateData();
	void loadFrame();
	void onStageBegin();
	void onNextStage();
	void onFinishGame() override;

	void onClickItem(cocos2d::Ref* sender);
	void onMoveItem(cocos2d::Ref* sender);
	void onMoveItemEnd(cocos2d::Ref* sender);

	void onClickToImage(Ref* pSender, Widget::TouchEventType type);

	void playChooseItem(Item* chooseItem = NULL, std::string audio_path = "");
	void readGafBlinklink();

	void readCommandAndShowGuide();
	void suggetionGame(Vec2 posStart, Vec2 posEnd);
	void removeAndStopActionHand();

    Vec2 train_pos;
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 30;
	bool _speakable = false, _next_state = false;
	cocos2d::ui::Button *_speakerBtn;
	std::string _audioCommand = "";
	int _deplayForShowGuideline = 0;
	int _firstTimeGuiding = 0;
	int _idAudioChoose = -1;
	std::string _pathAudio = "";
	//GAFWrapper* gafStar;

};

CREATE_READER(WhatHappenedNext);

#endif // !WhatHappenedNext_h
