
#ifndef MJGameShark2_Fish_hpp
#define MJGameShark2_Fish_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
class MJGameShark2_FishDelegate {
public:
	virtual void resetText(cocos2d::Ref *sender) {};
	virtual void onFish(cocos2d::Ref *sender) {};
	virtual void onSharkRun(cocos2d::Ref *sender) {};
};
class MJGameShark2_Fish : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameShark2_FishDelegate {
public:
    CREATE_FUNC(MJGameShark2_Fish);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct FishInfo {
        std::string text;
        std::string audio_path;
        std::string img_path;
    };
    
	static MJGameShark2_Fish * createFish(std::string text);
    static MJGameShark2_Fish * createShark();
	std::string text_in_fish;
    void setDelegate(MJGameShark2_FishDelegate *_delegate);
    GAFWrapper *gaf_fish = nullptr;
    void run();
    void onEnter() override;
	void sharkRunToGoal(Ref* target, bool fish_flip);
    void fishRunWhenTap(Vec2 fish_tap_to, float duration = 0.f);
    bool flipx = false;
    float time_shark = 0;
    Vec2 pos_tap_to;
	std::vector<Vec2> listPos_;

	void runInStart();
	void runOutStart();
	void setPoss(std::vector<Vec2> listPoss);
	void resetText(std::string newText);
	void resetGaf();
	void returnBase();
	bool in_anim = false;
	float freeTime = 0;
    std::string audio_word;
    bool taped = false;
    cocos2d::ui::Text* text;
private:
    MJGameShark2_FishDelegate *delegate = nullptr;
	void didLoadFromCSB(std::string text);
    void didLoadFromCSB(bool is_shark = true);
    bool isShark = false;
    void onTapFish(cocos2d::Ref *sender);
    int fish_id = 0;
    
};

CREATE_READER(MJGameShark2_Fish);

#endif
