
#include "GamePhonics_Monkey.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGamePhonics_Monkey);

USING_NS_CC;
#define CSB_NAME "csb/game/phonics/GamePhonics_Monkey.csb"

MJGamePhonics_Monkey * MJGamePhonics_Monkey::createGame() {
    auto p = static_cast<MJGamePhonics_Monkey *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGamePhonics_Monkey::didLoadFromCSB() {
    gaf_monkey = GAFWrapper::createGAF("gaf/phonics/phonic_danh_van_monkey/phonic_danh_van_monkey.gaf");
    this->addChild(gaf_monkey);
}

void MJGamePhonics_Monkey::setDelegate(MJGamePhonics_MonkeyDelegate *_delegate){
    this->delegate = _delegate;
}

void MJGamePhonics_Monkey::playMonkeyWithAction(int monkey_action_type, bool isloop){
    CCLOG("NAME = %s", getGafSequenceNameByType(monkey_action_type).c_str());
    _monkey_finish_type = monkey_action_type;
    gaf_monkey->playSequence(getGafSequenceNameByType(monkey_action_type), isloop, [=] (gaf::GAFObject *obj, std::string name_sequence){
        if (delegate) {
            delegate->onFinishMonkey(_monkey_finish_type);
        }
    });
}

std::string MJGamePhonics_Monkey::getGafSequenceNameByType(int type){
    switch (type) {
        case JUMP: return "jump";
            break;
        case IDLE: return "idle";
            break;
        case POINT_UP: return "point_up";
            break;
        case POINT_DOWN: return "point_down";
            break;
        case HAND: return "khoang_tay";
            break;
        case LISTEN: return "nghe2";
            break;
        case LISTEN_HAND: return "nghe1";
            break;
        case READ: return "noi2";
            break;
        case MIC_SHOW: return "noi1";
            break;
        case MIC_HIDE: return "cat_mic";
            break;
        case TALK: return "talk";
            break;
        default:
            return "";
            break;
    }
    return "";
}
