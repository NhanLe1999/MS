
#include "BreakTheEgg_Board.h"
#include "HSAudioEngine.h"

BreakTheEgg_Board * BreakTheEgg_Board::createItem(mj::PHONIC_BOARD_INFO item_info) {
    BreakTheEgg_Board * obj = new BreakTheEgg_Board();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void BreakTheEgg_Board::didLoadFromCSB(mj::PHONIC_BOARD_INFO item_info) {
    _item_info = item_info;
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    board = cocos2d::ui::ImageView::create("games/breaktheegg/breaktheegg_board.png");
    board->setAnchorPoint(Vec2(0.5,0));
    board->setPosition(Vec2(screen_size.width/2, screen_size.height + 100));
    
    this->addChild(board);
    
    if (_item_info.video_path != ""){
        this->showVideo(_item_info.video_path, cocos2d::Size(628, 337), Vec2(board->getContentSize().width/2, board->getContentSize().height*0.5), board);
    }else {
        this->showImage(_item_info.image_path, Vec2(board->getContentSize().width/2, board->getContentSize().height*0.5));
    }
}

void BreakTheEgg_Board::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->showBoard();
}

void BreakTheEgg_Board::showBoard(){
    AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_boardin.mp3");
    board->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(1.0f, Vec2(screen_size.width/2, screen_size.height*0.3))), DelayTime::create(0.3f), CallFunc::create([=]{
        AUDIO_ENGINE->play(_item_info.audio_full);

        if (_item_info.video_path != ""){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            videoPlayer->play();
#endif
        }else {
            board->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=]{
                board->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(1.0f, Vec2(screen_size.width/2, screen_size.height+100))),DelayTime::create(0.3f), CallFunc::create([=]{
                    this->removeFromParent();
                }), NULL));
                if (_delegate){
                    _delegate->onBoardStartHide();
                }
            }), NULL));
        }
    }), NULL));
}

void BreakTheEgg_Board::showVideo(std::string video_path, cocos2d::Size video_size, cocos2d::Vec2 video_position, cocos2d::Node *parent){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    videoPlayer = VideoPlayer::create();
    videoPlayer->setPosition(video_position);
    videoPlayer->setContentSize(video_size);
    parent->addChild(videoPlayer);
    videoPlayer->setFileName(_item_info.video_path);
    videoPlayer->addEventListener(CC_CALLBACK_2(BreakTheEgg_Board::videoEventCallback, this));
    videoPlayer->seekTo(0.0f);
    videoPlayer->pause();
#endif
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

void BreakTheEgg_Board::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::PLAYING:
            CCLOG("PLAYING");
            break;
        case VideoPlayer::EventType::PAUSED:
            CCLOG("PAUSED");
            break;
        case VideoPlayer::EventType::STOPPED:
            CCLOG("STOPPED");
            break;
        case VideoPlayer::EventType::COMPLETED:
            CCLOG("COMPLETED");
            this->scheduleOnce([=](float dt){
                //videoCompleted();
                AUDIO_ENGINE->play("sounds/breaktheegg/breaktheegg_boardout.mp3");
                board->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(1.0f, Vec2(screen_size.width/2, screen_size.height+100))),DelayTime::create(0.3f), CallFunc::create([=]{
                    videoPlayer->removeFromParent();
                    videoPlayer = nullptr;
                    this->removeFromParent();
                }), NULL));
                if (_delegate){
                    _delegate->onBoardStartHide();
                }
            }, 0.05f, "video_done");
            break;
        default:
            break;
    }
}
#endif

void BreakTheEgg_Board::showImage(std::string img_path, Vec2 img_position){
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    auto img = cocos2d::ui::ImageView::create(img_path);
    img->setAnchorPoint(Vec2(0.5, 0.5));
    board->addChild(img);
    img->setPosition(img_position);
    auto size_board = board->getContentSize();
    img->setScale(size_board.height*0.7/img->getContentSize().height);
    
    if (img->getBoundingBox().size.width > size_board.width*0.9){
        img->setScale(img->getScale()*size_board.width*0.9/img->getBoundingBox().size.width);
    }
//#endif
}

void BreakTheEgg_Board::setDelegate(BreakTheEgg_BoardDelegate *delegate){
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    _delegate = delegate;
//#endif
}
