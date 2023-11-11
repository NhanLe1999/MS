
#include "GameBuildWall_Crocodile.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameBuildWall_Crocodile);

USING_NS_CC;
#define CSB_NAME "csb/game/build_wall/GameBuildWall_Crocodile.csb"

MJGameBuildWall_Crocodile * MJGameBuildWall_Crocodile::createGame() {
    auto p = static_cast<MJGameBuildWall_Crocodile *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameBuildWall_Crocodile::didLoadFromCSB() {
    crocodile = GAFWrapper::createGAF("gaf/build_wall/ca_sau_dap_da/ca_sau_dap_da.gaf");
    this->addChild(crocodile);
    pos_1 = Vec2(0, 165);
    crocodile->setPosition(pos_1);
    
    
}

void MJGameBuildWall_Crocodile::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->playMonkeyWithAction(RUN, true);
    
}

void MJGameBuildWall_Crocodile::playMonkeyWithAction(int monkey_action_type, bool isloop){
    _monkey_finish_type = monkey_action_type;
    crocodile->playSequence(getGafSequenceNameByType(monkey_action_type), isloop, [=] (gaf::GAFObject *obj, std::string name_sequence){
        CCLOG("_monkey_finish_type = %d", _monkey_finish_type);
        if (delegate) {
            delegate->onCrocodileActionFinish(_monkey_finish_type);
            
        }
    });
    
    
}

void MJGameBuildWall_Crocodile::setFlipCrocodile(bool is_flipX){
    crocodile->setFlippedX(is_flipX);
    is_flip = is_flipX;
}

std::string MJGameBuildWall_Crocodile::getGafSequenceNameByType(int type){
    switch (type) {
        case IDLE: return "idle";
            break;
        case WRONG: return "wrong";
            break;
        case JUMP: return "jump";
            break;
        case RUN: return "run";
            break;
        case STOP: return "stop";
            break;
        case SMASH: return "smash";
            break;
        case VIC: return "vic";
            break;
    }
    return "";
}

void MJGameBuildWall_Crocodile::setDelegate(MJGameBuildWall_CrocodileDelegate *_delegate){
    this->delegate = _delegate;
}

ui::Widget::ccWidgetClickCallback MJGameBuildWall_Crocodile::onLocateClickCallback(const std::string &callback_name) {
//    if (callback_name == "onTapFish") {
////        return CC_CALLBACK_1(MJGameBuildWall_Crocodile::onTapFish, this);
//    }
    return nullptr;
}
