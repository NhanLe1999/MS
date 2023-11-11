
#ifndef GameLandThePlane_Cloud_hpp
#define GameLandThePlane_Cloud_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class MJGameLandThePlane_Cloud : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol{
public:
    CREATE_FUNC(MJGameLandThePlane_Cloud);
    static MJGameLandThePlane_Cloud * createFrame();

private:
    void didLoadFromCSB();
};

CREATE_READER(MJGameLandThePlane_Cloud);

#endif /* GameLandThePlane_Cloud_hpp */
