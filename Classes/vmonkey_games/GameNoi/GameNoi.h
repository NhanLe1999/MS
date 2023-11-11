//
//  GameNoi.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//	Cloned by MinhTzy on 29/05/2019

#ifndef GameNoi_hpp
#define GameNoi_hpp

#include "cocostudiomacro2.h"
#include "MJMainGBase.h"
#include "GameData.h"
#include "GameNoiFrameTop.h"
#include "GameNoiFrameBottom.h"

class GameNoi : public MJMainGBase,public cocostudio::WidgetCallBackHandlerProtocol, public GameNoiPlugDelegate {
public:
    static void initReader();
    CREATE_FUNC(GameNoi);
    static GameNoi * createGame(mj::GameDataSet<mj::LazerGameDataSet> data);
    static GameNoi * createGame(std::string configPath = "config.json");
    static mj::GameDataSet<mj::LazerGameDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::LazerGameDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::LazerGameDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::string configPath);
    void didLoadFromCSB(mj::GameDataSet<mj::LazerGameDataSet> data);
    void onEnter() override;

    void doAppear();
    bool loadQuestion();
    void doDisappear();

	void loadFrames();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onRefresh(cocos2d::Ref * sender);

    void onPlugBeginning(GameNoiFrameTop * frame) override;
    void onPlugMoving(GameNoiFrameTop * frame, cocos2d::Point position) override;
    void onPlugMoved(GameNoiFrameTop * frame, cocos2d::Point position) override;

    std::vector<GameNoiFrameBottom *> _bottom_frames;
    std::vector<GameNoiFrameTop *> _top_frames;

    
    mj::LazerGameDataSet _current_set;
	mj::GameDataSet<mj::LazerGameDataSet> _data;
    int _total_connected;
    
    void showGuide(cocos2d::Point start, cocos2d::Point stop);
    bool _is_guide_showed = false;

	cocos2d::ui::Text* lbl_question;
	cocos2d::ui::Button* btn_speaker;
	std::string sound_question;
	void onClickedSpeaker(cocos2d::Ref * sender);
    int font_sz = 38;
    int q_id = -1;
};

CREATE_READER(GameNoi);

#endif /* GameNoi_hpp */
