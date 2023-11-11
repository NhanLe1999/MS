//#include "AISpeakingReviewScreen.h"
//#include "CameraHelper.h"
//#include "MJDefinitions.h"
//#include "MJHelper.h"
//#include "MsHelperGame.h"
//#include "cocostudiomacro2.h"
//#include "cocos2d.h"
//#include "AISpeakingScene.h"
//#include "AISpeakingClosingPopup.h"
//#define AI_SEAPKING_PROCESS_BAR_PATH "games/ai_speaking/bar.png"
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//INITIALIZE_READER(AISpeakingReviewScreen);
//using namespace experimental::ui;
//using namespace cocos2d;
//
//std::once_flag ai_speaking_review_screen;
//
//std::pair<std::string,std::string> turnDelay(int vLeftIndex,int vRightIndex,int imageIndex, std::string parentName,float delayTime) {
//
//    std::string imgFn = "";
//
//    int delayTimeMiliseconds = delayTime*1000;
//
//    std::string leftCmd = StringUtils::format("[%d]setpts=PTS+%f/TB[v%d];[v%d]scale=722:-1[v%ds];[%s][v%ds]overlay=159:112[v%dfn];[%d]adelay=%d|%d[a%d];",vLeftIndex,delayTime,vLeftIndex,vLeftIndex,vLeftIndex,parentName.c_str(),vLeftIndex,vLeftIndex,vLeftIndex,delayTimeMiliseconds,delayTimeMiliseconds,vLeftIndex);
//
//    std::string rightCmd = StringUtils::format("[%d]setpts=PTS+%f/TB[v%d];[v%d]scale=722:-1[v%ds];[v%dfn][v%ds]overlay=1035:112[v%dfn];[%d]adelay=%d|%d[a%d];",vRightIndex,delayTime,vRightIndex,vRightIndex,vRightIndex,vLeftIndex,vRightIndex,vRightIndex,vRightIndex,delayTimeMiliseconds,delayTimeMiliseconds,vRightIndex);
//
//
//    if(delayTime <= 0.0f) {
//
//        leftCmd = StringUtils::format("[%d]scale=722:-1[v%d];[%s][v%d]overlay=159:112[v%dfn];[%d]adelay=%d|%d[a%d];",vLeftIndex,vLeftIndex,parentName.c_str(),vLeftIndex,vLeftIndex,vLeftIndex,delayTimeMiliseconds,delayTimeMiliseconds,vLeftIndex);
//
//        rightCmd = StringUtils::format("[%d]scale=722:-1[v%d];[v%dfn][v%d]overlay=1035:112[v%dfn];[%d]adelay=%d|%d[a%d];",vRightIndex,vRightIndex,vLeftIndex,vRightIndex,vRightIndex,vRightIndex,delayTimeMiliseconds,delayTimeMiliseconds,vRightIndex);
//    }
//
//    imgFn = StringUtils::format("v%dfn",vRightIndex);
//
//    std::string imgCmd = "";
//
//    if(imageIndex>0) {
//
//        imgCmd = StringUtils::format("[%d]scale=-1:435[image%d];[v%dfn][image%d]overlay=666:584[imgFn%d];",imageIndex,imageIndex,vRightIndex,imageIndex,imageIndex);
//
//        imgFn = StringUtils::format("imgFn%d",imageIndex);
//    }
//
//    return {leftCmd+rightCmd+imgCmd,imgFn};
//}
//
//Scene * AISpeakingReviewScreen::createScene(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo lessonInfo)
//{
//    auto scene = Scene::create();
//
//    if (scene) {
//        auto view = AISpeakingReviewScreen::createView(infoList, lessonInfo);
//        scene->addChild(view);
//    } else {
//        CC_SAFE_DELETE(scene);
//    }
//
//    return scene;
//}
//
//AISpeakingReviewScreen* AISpeakingReviewScreen::createView(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo lessonInfo) {
//
//    std::call_once(ai_speaking_review_screen, [] {
//
//        REGISTER_CSB_READER(AISpeakingReviewScreen);
//
//    });
//
//    auto view = reinterpret_cast<AISpeakingReviewScreen*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingReviewScreen.csb"));
//    if (view) {
//        view->didLoadFromCSB(infoList, lessonInfo);
//        return view;
//    }
//    return nullptr;
//}
//
//static  std::string cmd = "";
//
//void AISpeakingReviewScreen::didLoadFromCSB(std::vector<AISpeakingReviewScreenTurnInfo> infoList, mj::model::AISpeakingLessonInfo lessonInfo) {
//
//    setLessonInfo(lessonInfo);
//    _turnInfoList = infoList;
//
//    _screenSize = Director::getInstance()->getVisibleSize();
//
//    if(auto backButton = utils::findChild(this, "back_button")) {
//
//        backButton->setPositionX(Director::getInstance()->getVisibleOrigin().x + _screenSize.width - backButton->getContentSize().width*0.6f );
//    }
//
//    playPreviewStart([=]() {
//        playPreviewContent(_turnInfoList);
//    });
//}
//
//void AISpeakingReviewScreen::playPreviewStart(std::function<void()> callback){
//    //CameraHelper::GetInstance()->startVideoRecorder();
//
//    _callback = callback;
//    if(getLessonInfo().is_learned)
//    {
//        _callback();
//        return;
//    }
//    if(auto reviewLayout = utils::findChild(this,"root_review")) {
//
//        reviewLayout->setVisible(false);
//    }
//
//    if(auto rootLayout = utils::findChild(this,"root_layout")) {
//
//        rootLayout->setVisible(true);
//        // setup aactor background
//        MK_SAFELY_REMOVE(rootLayout, "actor");
//
//        if(auto actor = utils::findChild<ImageView*>(rootLayout,"template"))
//        {
//
//            Text *reviewText = Text::create("Review", "fonts/Montserrat-ExtraBold.ttf", 48);
//            reviewText->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
//            reviewText->setName("reviewText");
//            reviewText->setVisible(false);
//            rootLayout->addChild(reviewText);
//            reviewText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            reviewText->setPosition(Vec2(actor->getPosition().x, actor->getPosition().y + actor->getContentSize().height/2 + reviewText->getContentSize().height * 2));
//            reviewText->setVisible(true);
//            if(!_lessonInfo._actorName.empty()) {
//
//                std::string imagePath = AI_SEAPKING_RESOURCE_PATH;
//                imagePath += StringUtils::format("relearn/review_%s.png",_lessonInfo._actorName.c_str());
//                //std::string avPath = StringUtils::format("%savt_%s.png",AI_SEAPKING_RESOURCE_PATH,_lessonInfo._actorName.c_str());
//
//                if(FileUtils::getInstance()->isFileExist(imagePath)) {
//                    actor->loadTexture(imagePath);
//                }
//            }
//        }
//
//
//        // setup process bar
//        if (_progcess == nullptr)
//        {
//            if(FileUtils::getInstance()->isFileExist(AI_SEAPKING_PROCESS_BAR_PATH))
//            {
//                if(auto bar_bg = utils::findChild(rootLayout,"bar_bg"))
//                {
//                    if(auto loading_bar = utils::findChild(bar_bg,"loading_bar"))
//                    {
//                        loading_bar->setVisible(false);
//                    }
//                    auto bar = Sprite::create(AI_SEAPKING_PROCESS_BAR_PATH);
//                    if(!bar){
//                        _progcess = nullptr;
//                        return;
//                    }
//                    bar->setContentSize(Size(bar_bg->getContentSize()));
//                    _progcess = ProgressTimer::create(bar);
//                    _progcess->setType(ProgressTimer::Type::BAR);
//                    _progcess->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//                    _progcess->setPosition(bar_bg->getContentSize() / 2);
//                    _progcess->setVisible(true);
//                    _progcess->setName("ProgressTimerPreview");
//                    _progcess->setPercentage(0);
//                    bar_bg->addChild(_progcess);
//
//                    this->schedule([this](float)
//                        {
//                        float per = _progcess->getPercentage();
//                        _progcess->setPercentage(per + 5);
//                            if(100 <= _progcess->getPercentage())
//                            {
//                                this->unschedule("runProccessBar");
//                                _callback();
//                            }
//
//                        }, 0.05, CC_REPEAT_FOREVER, 0, "runProccessBar");
//                }
//                else
//                {
//                    _callback();
//                }
//            }
//            else
//            {
//                _callback();
//            }
//        }
//    }
//}
//void AISpeakingReviewScreen::playPreviewContent(std::vector<AISpeakingReviewScreenTurnInfo> infoList){
//    this->unschedule("runProccessBar");
//    if(auto rootLayout = utils::findChild(this,"root_layout")) {
//
//        rootLayout->setVisible(false);
//    }
//
//    if(auto reviewLayout = utils::findChild(this,"root_review")) {
//
//        reviewLayout->setVisible(true);
//    }
//
//    if(auto backButton = utils::findChild(this, "back_button")) {
//
//        backButton->setPositionX(Director::getInstance()->getVisibleOrigin().x + _screenSize.width - backButton->getContentSize().width*0.6f );
//    }
//
//    _leftVideoArea = utils::findChild(this, "video_left");
//
//   _rightVideoArea = utils::findChild(this, "video_right");
//
//    std::vector<std::pair<int,std::string>> leftURLs;
//
//    std::vector<std::pair<int,std::string>> rightURLs;
//
//    for(auto info : _turnInfoList) {
//
//        leftURLs.push_back({info._leftVideo._vID, info._leftVideo._url} );
//
//        rightURLs.push_back({info._rightVideo._vID, info._rightVideo._url} );
//    }
//
//    _leftVideoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
//    _leftVideoPlayer->setURLs(leftURLs);
//    _leftVideoArea->getParent()->addChild(_leftVideoPlayer);
//    _leftVideoPlayer->setCornerRadius(10.0f);
//    _leftVideoPlayer->setName("videoPlayer");
//    _leftVideoPlayer->setContentSize(_leftVideoArea->getContentSize());
//    _leftVideoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _leftVideoPlayer->setPosition(_leftVideoArea->getPosition());
//
//    auto leftTracks = _leftVideoPlayer->getInfoTracks();
//
//    for(auto &video : _turnInfoList){
//
//        auto id = video._leftVideo._vID;
//
//        if(leftTracks.count(id)>0) {
//
//            auto track = leftTracks.at(id);
//
//            auto &leftvideo = video._leftVideo;
//
//            leftvideo._timeStart    = track.first;
//
//            leftvideo._timeEnd      = track.second;
//
//            leftvideo._duration     = track.second - track.first;
//
//        }
//    }
//
//    _rightVideoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
//    _rightVideoPlayer->setURLs(rightURLs);
//    _rightVideoArea->getParent()->addChild(_rightVideoPlayer);
//    _rightVideoPlayer->setCornerRadius(10.0f);
//    _rightVideoPlayer->setName("videoPlayer");
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    if(!CameraHelper::GetInstance()->getSupportIOS15())
//    {
//        if(!CameraHelper::GetInstance()->getVideoOrientationLockStatus())
//        {
//            CameraHelper::GetInstance()->setVideoOrientationLockStatus(true);
//            int orientation = CameraHelper::GetInstance()->getOrientation();
//            if(orientation == 4)
//            {
//                _rightVideoPlayer->setRotate(-90.0f);
//            }
//            else if(orientation == 3)
//
//                _rightVideoPlayer->setRotate(90.0f);
//        }
//
//    }
//
//    /*
//     case unknown = 0
//
//     case portrait = 1
//
//     case portraitUpsideDown = 2
//
//     case landscapeLeft = 4
//
//     case landscapeRight = 3
//     */
//    #endif
//    _rightVideoPlayer->setContentSize(_rightVideoArea->getContentSize());
//    _rightVideoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _rightVideoPlayer->setPosition(_rightVideoArea->getPosition());
//
//    auto rightTracks = _rightVideoPlayer->getInfoTracks();
//
//    for(auto &video : _turnInfoList){
//
//        auto id = video._rightVideo._vID;
//
//        if(rightTracks.count(id)>0) {
//
//            auto track = rightTracks.at(id);
//
//            auto &vdeo = video._rightVideo;
//
//            vdeo._timeStart    = track.first;
//
//            vdeo._timeEnd      = track.second;
//
//            vdeo._duration     = track.second - track.first;
//        }
//    }
//
//    _leftVideoPlayer->play();
//
//    _rightVideoPlayer->play();
//
//    std::vector<std::string> videoPaths ;
//    std::vector<AISpeakingReviewScreenTurnInfo> turnInfoListForMerge;
//
//    int pID  = 0;
//
//    std::string a1;
//    int audioID = 0;
//    for(int i = 0 ; i < _turnInfoList.size();i++) {
//
//        auto turnInfo  = _turnInfoList.at(i);
//
//        std::string leftPath = FileUtils::getInstance()->fullPathForFilename(turnInfo._leftVideo._url);
//        std::string rightPath = FileUtils::getInstance()->fullPathForFilename(turnInfo._rightVideo._url);
//        if(leftPath.empty() || rightPath.empty())
//        {
//            continue;
//        }
//        else
//        {
//            turnInfoListForMerge.push_back(turnInfo); // only using invalid video path
//        }
//
//    }
//
//    for(int i = 0 ; i < turnInfoListForMerge.size();i++) {
//
//        auto turnInfo  = turnInfoListForMerge.at(i);
//
//        pID += (turnInfo._imagePath.empty()?2:3);
//
//        videoPaths.push_back(turnInfo._leftVideo._url);
//        videoPaths.push_back(turnInfo._rightVideo._url);
//
//        a1 += StringUtils::format("[a%d]",audioID++);
//        a1 += StringUtils::format("[a%d]",audioID++);
//
//        if(!turnInfo._imagePath.empty()) {
//            audioID++;
//            videoPaths.push_back(turnInfo._imagePath);
//        }
//    }
//
//    cmd = "-filter_complex \"";
//
//    int vID = 0;
//
//    std::string parentID = StringUtils::format("%d",pID);
//
//    for(int i = 0 ; i < turnInfoListForMerge.size();i++) {
//
//        auto turnInfo  = turnInfoListForMerge.at(i);
//
//        auto cmdTmp =  turnDelay(vID++,vID++,turnInfo._imagePath.empty()?-1:vID++,parentID, turnInfo._isNextTurnBasedOnRightVideo?turnInfo._rightVideo._timeStart:turnInfo._leftVideo._timeStart);
//
//        cmd += cmdTmp.first;
//
//        CCLOG("xxxx cmd:%s",cmdTmp.first.c_str());
//
//        parentID = cmdTmp.second;
//    }
//
//    const int k_numberCount = std::count_if(
//           a1.begin(),
//           a1.end(),
//        [](char c)
//    {
//        return c == '[';
//    });
//
//    cmd += StringUtils::format("%samix=inputs=%d:dropout_transition=0",a1.c_str(),k_numberCount);
//
//    cmd += "\" -y -b:v 128k";
//
//    CCLOG("xxxx final cmd:%s",cmd.c_str());
//
//    cmd = mj::helper::replace_string(cmd,StringUtils::format("[%s]",parentID.c_str()), "");
//
//    this->schedule([=](float dt){
//
//        if(FileUtils::getInstance()->isFileExist(USER_OUTRO_VIDEO_PATH)) {
//
//            this->unschedule("hihixx");
//
//            auto time_current = int(utils::getTimeInMilliseconds() / 1000);
//            std::string timeRenewStr =  mj::helper::getStringTime(time_current,  "%H_%M_%S_%d_%m_%G");
//            std::string outputFileName = "output_" + timeRenewStr + ".mp4";
//
//            setOutputVideoPath(FileUtils::getInstance()->getWritablePath() + outputFileName);
//        }
//
//    },1.0f,"hihixx");
//    playSequenceVideos();
//}
//void AISpeakingReviewScreen::onTryAgain(Ref* sender) {
//
//}
//
//void AISpeakingReviewScreen::onLetTalk(Ref* sender) {
//
//}
//
//void AISpeakingReviewScreen::onReplay(Ref* sender) {
//
//}
//
//void AISpeakingReviewScreen::onBackButtonClicked(Ref* sender) {
//
//    _leftVideoArea->stopAllActions();
//
//    _rightVideoArea->stopAllActions();
//
//    if(_rightVideoPlayer) {
//
//        _rightVideoPlayer->pause();
//
//        _rightVideoPlayer->setVisible(false);
//    }
//
//    _leftVideoPlayer->pause();
//
//    _leftVideoPlayer->setVisible(false);
//
//    if (auto displayImage = utils::findChild(this, "display_image")) {
//
//        displayImage->removeFromParent();
//    }
//
//    bool isLastTurn = (_currentID >= _turnInfoList.size());
//
//    if(!isLastTurn) {
//
//        if( auto popup = AISpeakingClosingPopup::createView(AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN_LAST_TURN)) {
//
//            popup->setClosePopupCallback([=](){
//
//                _currentID -= 1;
//
//                CCLOG("_currentID:%d",_currentID);
//
//                if(_currentID<0) {
//                    _currentID = 0;
//                }
//
//                if(_rightVideoPlayer) {
//
//                    _rightVideoPlayer->setVisible(true);
//                }
//
//                _leftVideoPlayer->setVisible(true);
//
//
//                playSequenceVideos();
//
//            });
//
//            popup->setExitCallback([=](){
//
//                Director::getInstance()->popScene();
//
//            });
//
//            Director::getInstance()->getRunningScene()->addChild(popup,INT_MAX-1);
//        }
//    }
//    else//show share popup here
//    {
//        Director::getInstance()->popScene();
//    }
//}
//void AISpeakingReviewScreen::showPopupShare() {
//
//    //if(FileUtils::getInstance()->isFileExist(getOutputVideoPath()))
//    _MSAISpeakingSharePopup = MSAISpeakingSharePopup::showPopup(this, this, INT_MAX - 1);
//    _MSAISpeakingSharePopup->setVisible(false);
//    setOpacitySharePopup(0);
//    //else
//        //Director::getInstance()->popScene();
//
//}
//
//void AISpeakingReviewScreen::hidePopupShare() {
//    if(_MSAISpeakingSharePopup)
//    {
//        _MSAISpeakingSharePopup->removeFromParent();
//        _MSAISpeakingSharePopup = nullptr;
//    }
//
//}
//void AISpeakingReviewScreen::setOpacitySharePopup(int opacity) {
//    if(_MSAISpeakingSharePopup)
//    {
//        _MSAISpeakingSharePopup->setOpacity(opacity);
//        if(opacity == 0)
//        {
//            this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
//
//                if(!_isMerged)
//                {
//                    _sharePopupScale =  _MSAISpeakingSharePopup->getScale();
//                    _MSAISpeakingSharePopup->setScale(0.1);
//                }
//
//            }), NULL));
//        }
//        else if (opacity == 255)
//        {
//            _MSAISpeakingSharePopup->runAction(ScaleTo::create(0.5f, _sharePopupScale));
//        }
//    }
//
//}
//void AISpeakingReviewScreen::playSequenceVideos() {
//
//    CCLOG("playSequenceVideos ----------------------------------------------------------------------");
//
//
//    CCLOG("\n\n\nAISpeakingReviewScreen playSequenceVideos");
//
//    if(_currentID >= _turnInfoList.size()) {
//
//        return;
//    }
//
//    _rightVideoArea->stopAllActions();
//
//    _leftVideoArea->stopAllActions();
//
//    auto info = _turnInfoList.at(_currentID++);
//
//    CCLOG("AISpeakingReviewScreen playSequenceVideos _leftVideo URL:%s timeStart:%f du:%f",info._leftVideo._url.c_str(),info._leftVideo._timeStart,info._leftVideo._duration);
//
//    CCLOG("AISpeakingReviewScreen playSequenceVideos _rightVideo URL:%s timeStart:%f du:%f",info._rightVideo._url.c_str(),info._rightVideo._timeStart,info._rightVideo._duration);
//
//    _currentTurnInfo = info;
//
//    if (auto displayImage = utils::findChild(this, "display_image")) {
//
//        displayImage->removeFromParent();
//    }
//
//    if(auto imageInstance = utils::findChild(this, "image")) {
//
//        if(!info._imagePath.empty()) {
//
//            auto img = ImageView::create(info._imagePath);
//            img->setScaleX(imageInstance->getContentSize().width / img->getContentSize().width);
//            img->setScaleY(imageInstance->getContentSize().height / img->getContentSize().height);
//            img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            img->setPosition(imageInstance->getPosition());
//            img->setName("display_image");
//            imageInstance->getParent()->addChild(img);
//            imageInstance->setVisible(false);
//        }
//    }
//
//    if(info._leftVideo._timeStart > 0)
//        _leftVideoPlayer->seekWithoutDelay(info._leftVideo._timeStart);
//
//    if(info._rightVideo._timeStart > 0)
//        _rightVideoPlayer->seekWithoutDelay(info._rightVideo._timeStart);
//
//    if(_currentID == _turnInfoList.size()) {
//
//        _leftVideoPlayer->resume();
//
//        auto videoIntroArea = utils::findChild(this, "video_area");
//
//        auto pos = videoIntroArea->getPosition();
//
//        if(info._leftVideo._duration > 0) {
//
//            _leftVideoArea->runAction(Sequence::create(DelayTime::create(info._leftVideo._duration+1.0f),CallFunc::create([=](){
//                _leftVideoArea->setVisible(false);
//                _leftVideoPlayer->setVisible(false);
//                _isFinishReviewed = true;
//                Director::getInstance()->popScene();
//                //onFinishButtonClicked(nullptr);
//                //CameraHelper::GetInstance()->stopVideoRecorder(nullptr, this);
//                //showPopupShare();
//
//            }), NULL));
//        }
//
//        _leftVideoPlayer->runAction(Sequence::create(MoveTo::create(1.0f, pos),ResizeTo::create(1.0f, videoIntroArea->getContentSize()), CallFunc::create([=]() {
//        }), NULL));
//
//    if(_rightVideoPlayer)
//    {
//        _rightVideoPlayer->removeFromParent();
//
//        _rightVideoPlayer = nullptr;
//
//    }
//
//        return;
//    }
//
//    bool isCallbackBasedOnRightVideo  = info._isNextTurnBasedOnRightVideo;
//
//    CCLOG(isCallbackBasedOnRightVideo?"playSequenceVideos isCallbackBasedOnRightVideo=true":"playSequenceVideos isCallbackBasedOnRightVideo=flase");
//
//    //bool isLeftVideoMaxDuration = info._leftVideo._duration >= info._rightVideo._duration;
//
//    if(info._leftVideo._duration > 0) {
//
//        CCLOG("playSequenceVideos _leftVideoArea->runAction(");
//
//        _leftVideoArea->runAction(Sequence::create(DelayTime::create(info._leftVideo._duration),CallFunc::create([=](){
//
//            CCLOG("playSequenceVideos _leftVideoPlayer");
//
//            _leftVideoPlayer->pause();
//
//            if(!isCallbackBasedOnRightVideo) {
//
//                CCLOG("playSequenceVideos _leftVideoPlayer callback call");
//
//                playSequenceVideos();
//            }
//
//            CCLOG("playSequenceVideos _leftVideoPlayer end");
//
//
//        }), NULL));
//
//    }
//
//    if(info._rightVideo._duration > 0) {
//
//        CCLOG("playSequenceVideos  _rightVideoArea->runAction(");
//
//
//        _rightVideoArea->runAction(Sequence::create(DelayTime::create(info._rightVideo._duration),CallFunc::create([=](){
//
//            CCLOG("playSequenceVideos _rightVideoPlayer");
//            _rightVideoPlayer->pause();
//
//            if(isCallbackBasedOnRightVideo) {
//
//                CCLOG("playSequenceVideos _leftVideoPlayer callback call");
//
//                playSequenceVideos();
//            }
//
//            CCLOG("playSequenceVideos _rightVideoPlayer end");
//
//
//        }), NULL));
//
//    }
//
//    _rightVideoPlayer->resume();
//
//    _leftVideoPlayer->resume();
//}
//
//cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingReviewScreen::onLocateClickCallback(const std::string& callBackName) {
//
//    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(AISpeakingReviewScreen, onTryAgain),
//        CLICK_MAP(AISpeakingReviewScreen, onLetTalk),
//        CLICK_MAP(AISpeakingReviewScreen, onReplay),
//        CLICK_MAP(AISpeakingReviewScreen, onBackButtonClicked)
//    };
//
//    if (function_map.find(callBackName) != function_map.end()) {
//        return function_map.at(callBackName);
//    }
//
//    return nullptr;
//}
//
//void AISpeakingReviewScreen::onCloseButtonClick(cocos2d::Ref *sender) {
//    if( auto popup = AISpeakingClosingPopup::createView(AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN_LAST_TURN)) {
//
//        popup->setClosePopupCallback([=](){
//
//            Director::getInstance()->popScene();
//
//        });
//
//        popup->setExitCallback([=](){
//
//            Director::getInstance()->popScene();
//
//        });
//
//        Director::getInstance()->getRunningScene()->addChild(popup,INT_MAX-1);
//    }
//}
//
//void AISpeakingReviewScreen::onShareButtonClicked(cocos2d::Ref *sender) {
//    pushEventAIspeakingToCleaverTap(true);
//    if(FileUtils::getInstance()->isFileExist(getOutputVideoPath()))
//        CameraHelper::GetInstance()->shareVideoWithUrl(getOutputVideoPath(),[](void){
//        });
//    else
//        hidePopupShare();
//}
//
//void AISpeakingReviewScreen::onFinishButtonClicked(cocos2d::Ref *sender) {
//    pushEventAIspeakingToCleaverTap(false);
//    if( auto popup = AISpeakingClosingPopup::createView(AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN_LAST_TURN)) {
//
//        popup->setClosePopupCallback([=](){
//
//            Director::getInstance()->popScene();
//
//        });
//
//        popup->setExitCallback([=](){
//
//            Director::getInstance()->popScene();
//
//        });
//
//        Director::getInstance()->getRunningScene()->addChild(popup,INT_MAX-1);
//    }
//}
//void AISpeakingReviewScreen::pushEventAIspeakingToCleaverTap(bool isShare){
//    bool learnAgain = MJDEFAULT->getBoolForKey(key_is_relearn_talking_lesson, false);
//    std::string type = "new_lesson";
//    if(learnAgain)
//        type = "old_lesson";
//
//        CleverTapManager::GetInstance()->pushEvent("alk_share_video_popup", {
//                            {"share",cocos2d::Value(isShare)},
//                            {"type",cocos2d::Value(type)}
//                    });
//}
//
//
//
//#endif
