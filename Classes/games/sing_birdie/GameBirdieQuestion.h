//
//  GameBirdieQuestion.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#ifndef GameBirdieQuestion_hpp
#define GameBirdieQuestion_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "GameBirdieAnswerOverlay.h"

class MJGameBirdieTree;

class MJGameBirdieQuestion_ItemDelegate {
public:
	virtual void onPauseQuestion() {};
    virtual void onAnswerBegan() {};
    virtual void onAnswerEnded() {};
};

class MJGameBirdieQuestion : public cocos2d::ui::Layout, public MJGameBirdieAnswerDelegate {
public:
    CREATE_FUNC(MJGameBirdieQuestion);
	static MJGameBirdieQuestion * createQuestion(mj::BirdieDataSet data, bool is_enable = true, MJGameBirdieQuestion_ItemDelegate* dele_=nullptr);
    
    cocos2d::Point getFirstTreePosition();
    cocos2d::Point getSecondTreePosition();
    std::string getFirstTreeWord();
    cocos2d::Point getTreePosition(MJGameBirdieTree* tree);
    std::string getTreeWord(MJGameBirdieTree* tree);
    MJGameBirdieTree* getUnanswerTree(bool getSecondTree = false);

private:
	void didLoadFromCSB(mj::BirdieDataSet data, bool is_enable = true,MJGameBirdieQuestion_ItemDelegate* dele_ = nullptr);
    mj::BirdieDataSet _data;

    std::vector<MJGameBirdieTree *> _trees;
	bool game_play_enable = false;
    void onAnswerDropped(MJGameBirdieAnswer * answer, cocos2d::Point position) override;
    void onAnswerTouchBegan() override;
    void onAnswerTouchEnded() override;
	CC_SYNTHESIZE(MJGameBirdieQuestion_ItemDelegate *, _delegate, Delegate);
};

CREATE_READER(MJGameBirdieQuestion);

#endif /* GameBirdieQuestion_hpp */
