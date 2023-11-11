
#include "GameLandThePlane_Cloud.h"
#include "GAFWrapper.h"

INITIALIZE_READER(MJGameLandThePlane_Cloud);

USING_NS_CC;

MJGameLandThePlane_Cloud * MJGameLandThePlane_Cloud::createFrame() {
    auto p = static_cast<MJGameLandThePlane_Cloud *>(cocos2d::CSLoader::createNode("csb/game/landtheplane/GameLandThePlane_Cloud.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameLandThePlane_Cloud::didLoadFromCSB() {
}


