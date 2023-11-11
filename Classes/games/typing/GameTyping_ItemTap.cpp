
#include "GameTyping_ItemTap.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameTyping_ItemTap);

USING_NS_CC;
#define CSB_NAME "csb/game/typing/GameTyping_ItemTap.csb"

MJGameTyping_ItemTap * MJGameTyping_ItemTap::createGame(ItemTapInfo item_info) {
    auto p = static_cast<MJGameTyping_ItemTap *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameTyping_ItemTap::didLoadFromCSB(ItemTapInfo item_info) {
    
}

void MJGameTyping_ItemTap::setDelegate(MJGameTyping_ItemTapDelegate *_delegate){
    this->delegate = _delegate;
}




