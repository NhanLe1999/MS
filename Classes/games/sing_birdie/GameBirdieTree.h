//
//  MJGameBirdieTree.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/4/17.
//

#ifndef GameBirdieTree_hpp
#define GameBirdieTree_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameBirdieAnswer;

class MJGameBirdieTree : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJGameBirdieTree);
    static MJGameBirdieTree * createTree(mj::GameObject data);
    inline std::string getWord() { return _data.text; }
    inline std::string getSound() { return _data.audio_path; }
    inline bool getAnswered() { return _is_answered; }
    void addAnswer(MJGameBirdieAnswer * answer);
    void onWrongAnswer();
private:
    void didLoadFromCSB(mj::GameObject data);

    mj::GameObject _data;
    bool _is_answered = false;
};

CREATE_READER(MJGameBirdieTree);

#endif /* GameBirdieTree_hpp */
