//
//  NhinBongDoanVat.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef NhinBongDoanVat_hpp
#define NhinBongDoanVat_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
#include "GameData.h"
//#include "Math_Libs.h"

USING_NS_CC;
using namespace ui;

class NhinBongDoanVat : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(NhinBongDoanVat);
    static NhinBongDoanVat * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
 
    void showGuide(Point start, Point stop);
    void showTutorial(float dt);
    void stopGuiding();
    void onTouchObject(Ref* sender, TouchEventType type );
    void radio();
    void inActiveMode();
    void readAnswer(int count);
    Layout* root_layout = nullptr;
    mj::GameObject q_obj;
    std::vector<mj::GameObject> imgs = {};
    std::vector<ImageView*> objs_copy;
    std::vector<ImageView*> objs = {};
    int number_obj = 0;
    int count = 0;
    std::map<std::string, Color3B> _colors;
    Vec2 pos;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Medium.ttf";
    int font_sz = 37;
    bool autoPlay = false;
    int audio_id_question = -1;
    Button* _speaker;

    
};
CREATE_READER(NhinBongDoanVat);

#endif /* NhinBongDoanVat_hpp */
