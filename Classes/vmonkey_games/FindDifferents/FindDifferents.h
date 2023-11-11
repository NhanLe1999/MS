//
//  FindDifferents.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef FindDifferents_hpp
#define FindDifferents_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
//#include "Math_Libs.h"
#include "GameData.h"

USING_NS_CC;
using namespace ui;

class FindDifferents : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(FindDifferents);
    static FindDifferents * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
 
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onTouchPicture();
    void onClickHolder(Ref* sender);
    void onClickSpeaker(Ref* sender);
    void readAnswer(int count);
    
    void playEffect(ImageView* ring);
    
    Layout* root_layout;
    mj::GameObject question;
    
    int count = 0;
    int number_diff = 0;
    int q_id = -1;
    
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Medium.ttf";
    int font_sz = 37;
    bool autoPlay = false;
    
};
CREATE_READER(FindDifferents);

#endif /* FindDifferents_hpp */
