//
//  GameInstructionOverlay.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/12/17.
//

#ifndef GameInstructionOverlay_hpp
#define GameInstructionOverlay_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameInstructionDelegate {
public:
    virtual void onInstructionEnded(mj::GameType type) {}
};

class MJGameInstructionOverlay : public cocos2d::ui::Layout {
public:
    static void playInstruction(mj::GameType type, const std::string & audio_path = "", const std::string &audio_path_extra = "", MJGameInstructionDelegate * delegate = nullptr,std::function<void()> callback = nullptr);
    static void playInstructionAfterDelay(float delay, mj::GameType type, const std::string & audio_path = "",  const std::string &audio_path_extra = "", MJGameInstructionDelegate * delegate = nullptr, std::function<void()> callback = nullptr);
public:
    static MJGameInstructionOverlay * createOverlay(mj::GameType type, const std::string & audio_path = "", const std::string & audio_path_extra = "", std::function<void()> callback = nullptr);
public:
    void onDeleteCallback();
private:
    bool init(mj::GameType type, const std::string & audio_path = "", const std::string & audio_path_extra = "", std::function<void()> callback = nullptr);
    void onEnter() override;
    void onExit() override;

    mj::GameType _type;
    std::string _instruction_audio_path;
    std::string _instruction_audio_path_extra;
    int _instruction_audio_id;
    CC_SYNTHESIZE(float, _delay_time, Delay);
    CC_SYNTHESIZE(MJGameInstructionDelegate *, _delegate, Delegate);

    std::function<void()> _callback = nullptr;
    
    std::function<void()> _callbackPlayAudio = nullptr;
};

#endif /* GameInstructionOverlay_hpp */
