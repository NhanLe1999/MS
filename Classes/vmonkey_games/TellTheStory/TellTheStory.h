//
//  TellTheStory.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef TellTheStory_hpp
#define TellTheStory_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
#include "MSRecord.h"

//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class TellTheStory : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public MSRecordDelegate  {
public:
    CREATE_FUNC(TellTheStory);
    static TellTheStory * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
 
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onRecord(Ref* sender);
    void onStop(cocos2d::Ref *sender);
    void onRepeat(Ref* sender);
    void onStopPlay(cocos2d::Ref *sender);
    void onShare(Ref* sender);
    void onNext(Ref* sender);
    void onClickSpeaker(Ref* sender);
    
    enum GAME_STATE {
        ON_NONE,
        ON_RECORD,
        ON_REPEAT,
        ON_STOP,
        ON_STOP_PLAY,
        ON_SHARE
    };
    void setGameState(GAME_STATE state);
  
    enum RECORD_TYPE {
        RECORD_DONE = 4,
        AUDIO_DONE = 5,
    };
    void startRecord();
    void onFinishRecord(int finish_type,std::string url) override;
    MSRecord *_record = nullptr;
    bool _is_recording = false;
    
    Layout* root_layout;
    Button *btstop, *btrecord, *btplay, *btshare,  *btnext, *btstopPlay;
    float timer = 5.0f;
    int count = 0;
    std::vector<std::string> imgs;
    std::string character;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Regular.ttf";
    int font_sz = 37;
    
    bool autoPlay = false;
};
CREATE_READER(TellTheStory);

#endif /* TellTheStory_hpp */
