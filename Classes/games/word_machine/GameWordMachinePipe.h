//
//  GameWordMachinePipe.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/27/17.
//
//

#ifndef GameWordMachinePipe_hpp
#define GameWordMachinePipe_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameWordMachineDelegate {
public:
    virtual void onReadSentence() = 0;
};

class MJGameWordMachinePipe : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJGameWordMachinePipe);
    void loadSentence(mj::GameObject sentence, std::string answer, bool isVisible = false, std::vector<math::resource::Sync_Text> syncText = {}, std::string audioPath = "");
    void adjust(bool isVisible = false);
    double runAnimation();

    double runAnimationEnter();
    double runAnimationReload();

private:
    std::vector<cocos2d::ui::ImageView *> _pipes;
    std::string _answer;
    mj::GameObject _sentence;
    float font_sz = 40;
    
    double _text_translation_factor;
    int _indexGame = 0;

    std::vector<math::resource::Sync_Text> _syncText;
    std::string _audoPath = "";
    std::vector<std::string> _textFullAnswer = {};
    float _sumSizeText = 0.0;
    float _sumSizeTextFull = 0.0;
    float _sizeTextAnswerBegin = 0.0;
    std::vector<float> sizeTextBegin = {};

    CC_SYNTHESIZE(MJGameWordMachineDelegate *, _delegate, Delegate);
};

CREATE_READER(MJGameWordMachinePipe);

#endif /* GameWordMachinePipe_hpp */
