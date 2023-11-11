//#include "AISpeakingScene.h"
//#include "CameraHelper.h"
//#include "MJDefinitions.h"
//#include "MJHelper.h"
//#include "MsHelperGame.h"
//#include "AISpeakingReplayPopup.h"
//#include "AISpeakingReviewScreen.h"
//#include "StoryDataModel.h"
//#include "AISpeakingClosingPopup.h"
//#include "AISpeakingFakeCallScreen.h"
//#include "MJPlatformConfig.h"
//#include "AISpeakingRelearnSplashScreen.h"
//
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//INITIALIZE_READER(AISpeakingScene);
//
//using namespace cocos2d;
//
//using namespace experimental::ui;
//
//#define RECORD_BUTTON_DELTA_POS             -80.0f
//#define RECORD_BUTTON_NORMAL_ANIMATION_TAG  111
//#define RECORD_BUTTON_BLOW_ANIMATION_TAG    112
//#define MAX_DURATION_FOR_USER_RECORD        10.0f
//#define RECORD_BUTTON_ACTION_TAG            1199
//
//#define USER_INTRO_VIDEO_PATH                       StringUtils::format("%s/user_video_intro.mp4",AI_SPEAKING_FOLDER_PATH)
//#define USER_TALKING_VIDEO_PATH(text)               StringUtils::format("%s/user_tl_%s.mp4",AI_SPEAKING_FOLDER_PATH,removeSpecialCharacter(text).c_str())
//#define USER_LISTEING_VIDEO_PATH(text)              StringUtils::format("%s/user_ls_%s.mp4",AI_SPEAKING_FOLDER_PATH,removeSpecialCharacter(text).c_str())
//#define TEXT_COLOR                                  Color3B(85, 85, 85)
//
//
//std::string removeSpecialCharacter(string s)
//{
//    for (int i = 0; i < s.size(); i++) {
//
//        if ((s[i] < 'A' || s[i] > 'Z') &&
//            (s[i] < 'a' || s[i] > 'z'))
//        {
//            s.erase(i, 1);
//            i--;
//        }
//    }
//
//    return s;
//}
//
//AISpeakingScene::~AISpeakingScene() {
//
//    for(auto &v: _videoInfoCacheList) {
//
//        if(v.second) {
//            delete v.second;
//            v.second = nullptr;
//        }
//    }
//}
//
//Scene * AISpeakingScene::createScene(std::vector<math::resource::Image*> images)
//{
//    auto scene = Scene::create();
//
//    if (scene) {
//        auto view = AISpeakingScene::createView(images);
//        scene->addChild(view);
//    } else {
//        CC_SAFE_DELETE(scene);
//    }
//
//    return scene;
//}
//
//std::once_flag ai_speaking_scene;
//
//AISpeakingScene* AISpeakingScene::createView(std::vector<math::resource::Image*> images, mj::model::AISpeakingLessonInfo lessonInfo, ms::AISpeakingMode mode) {
//
//    std::call_once(ai_speaking_scene, [] {
//
//        REGISTER_CSB_READER(AISpeakingScene);
//
//    });
//
//    auto view = reinterpret_cast<AISpeakingScene*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingScene.csb"));
//    if (view) {
//        view->setName("AISpeakingView");
//        view->didLoadFromCSB(images, lessonInfo, mode);
//        return view;
//    }
//    return nullptr;
//}
//
//void AISpeakingScene::didLoadFromCSB(std::vector<math::resource::Image*> images, mj::model::AISpeakingLessonInfo lessonInfo, ms::AISpeakingMode mode){
//
//    setAISpeakingMode(mode);
//    setLessonInfo(lessonInfo);
//    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING,true);
//    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING,true);
//
//    _screenSize = Director::getInstance()->getVisibleSize();
//
//    //auto notch = MJ_PLATFORM_CONFIG.getNotchHeight();
//
//    if(auto backButton = utils::findChild(this, "back_button")) {
//
//        backButton->setPositionX(Director::getInstance()->getVisibleOrigin().x + _screenSize.width - backButton->getContentSize().width*0.6f );
//
//    }
//
//    //create local saving path.
//    auto path = cocos2d::FileUtils::getInstance()->getWritablePath()+ AI_SPEAKING_FOLDER_PATH + "/";
//
//    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
//
//        cocos2d::FileUtils::getInstance()->createDirectory(path);
//    }
//
//    cocos2d::FileUtils::getInstance()->removeDirectory(path,[=](bool isSuccess){
//
//        if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(path)) {
//
//            cocos2d::FileUtils::getInstance()->createDirectory(path);
//        }
//    });
//
//    _mergedVideoInfo = MergedVideoInfo();
//
//    _rootLayout = cocos2d::utils::findChild(this, "root_layout");
//
//    if (_rootLayout) {
//
//        const float k_scaleFactor = ((_screenSize.width / _screenSize.height) / (4.0f / 3.0f));
//
//        _rootLayout->setScale(std::min(k_scaleFactor, 1.2f));
//    }
//
//    _gameLayer  = utils::findChild(this, "game_layer");
//
//    _introLayer = utils::findChild(this, "intro_layer");
//
//    _videoAreaAtGameLayer = utils::findChild(_gameLayer, "video_area");
//
//    _videoAreaAtIntroLayer = utils::findChild(_introLayer, "video_area");
//
//    _textButton = utils::findChild<ui::Button*>(_gameLayer, "text_button");
//    _speakerIcon = utils::findChild<ui::ImageView*>(_gameLayer, "image_speaker");
//    if(_speakerIcon)
//        _speakerIcon->setVisible(false);
//
//    if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//        _textButton->setTouchEnabled(false);
//    else
//        _textButton->setTouchEnabled(true);
//    _recordButton = utils::findChild(_gameLayer, "record_button");
//
//    if(_recordButton) {
//
//        _recordButton->setLocalZOrder(100);
//        _recordButton->setVisible(false);
//    }
//
//    if(_textButton) {
//
//        _textButton->setLocalZOrder(100);
//    }
//
//    if(getAISpeakingMode() != ms::AISpeakingMode::MIMIC)
//    {
//        _scaleTextSize = 0.9;
//    }
//    else
//    {
//        if (auto text = utils::findChild<ui::Text*>(_textButton, "text")) {
//
//            text->setContentSize(_textButton->getContentSize());
//            text->setPositionX(_textButton->getContentSize().width/2);
//        }
//    }
//    if(!initData(images)) {
//        return;
//    }
//
//    if (!_data._listInfo.empty()) {
//
//        _data._currentSessionID = 0;
//
//        //_currentSessionInfo = _data._listInfo.at(_data._currentSessionID);
//
//        playGame();
//    }
//}
//
//bool AISpeakingScene::initData(std::vector<math::resource::Image*> images)
//{
//    CCLOG("%s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//    auto configJsonData = FileUtils::getInstance()->getStringFromFile("config.json");
//
//    if (configJsonData.empty()) {
//        return false;
//    }
//
//    rapidjson::Document valueData;
//
//    valueData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(configJsonData.c_str());
//
//    if (valueData.IsNull() || valueData.HasParseError() || !valueData.IsObject()) {
//        CCLOG("AI Speaking JSON ERROR! \n\n %s", configJsonData.c_str());
//        return false;
//    }
//
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//    rapidjson::StringBuffer rj_buffer;
//    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
//    valueData.Accept(writer);
//
//
//    CCLOG("AI Speaking JSON: \n%s", rj_buffer.GetString());
//#endif
//
//    if (!valueData.HasMember("session") || valueData["session"].IsNull() || !valueData["session"].IsArray()) {
//        return false;
//    }
//
//    std::string actorName = JSON_SAFE_EXTRACT(valueData,"actor_name", String, "");
//
//    if(actorName.empty()) {
//        return false;
//    }
//
////    auto talkingCommonInfos = StoryDataModel::getInstance()->getAiSpeakingCommonInfosByActor(actorName);
////
////    if(talkingCommonInfos.empty()){
////
////        return false;
////    }
////
////    int actorID = MJDEFAULT->getIntegerForKey("common_actor_id",-1);
//std:;vector<mj::model::TalkingCommonInfo> talkingCommonInfos;
//    talkingCommonInfos = getRamdomCommonInfo(actorName);
//
////    for(auto info:talkingCommonInfos) {
////
////        if(info.id == actorID) {
////            talkingCommonInfo = info;
////        }
////
////    }
//
//    if(talkingCommonInfos.size() <= 0) {
//        return false;
//    }
//
////    auto rootVideoDataStr = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING+"root.json");
////
////    if (rootVideoDataStr.empty()) {
////        return false;
////    }
////
////    rapidjson::Document rootVideoData;
////
////    rootVideoData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(rootVideoDataStr.c_str());
////
////    mj::api::MJAPIDecodeObjectJsonInfo::readAISpeakingCommonInfoActorVideoURL(rootVideoData.GetArray(),"video",talkingCommonInfo);
//
//    for(auto talkingCommonInfo : talkingCommonInfos)
//    {
//        if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//        {
//            _data._introVideoPath.push_back(talkingCommonInfo.intro);
//            _data._introMIMICModeVideoPath.push_back(talkingCommonInfo.intro_mimic);
//
//        }
//        _data._introTALKINGModeVideoPath.push_back(talkingCommonInfo.intro_talking);
//        _data._yourTurnVideoPath.push_back(talkingCommonInfo.your_turn);
//        _data._listeningActorVideoPath.push_back(talkingCommonInfo.listening);
//        _data._outroMIMICVideoPath.push_back(talkingCommonInfo.outro_mimic);
//        _data._outroVideoPath.push_back(talkingCommonInfo.outro);
//        _data._cheerUpConfig._cheerUpVideoPath.push_back(talkingCommonInfo.cheer_up);
//    }
//    if(JSON_SAFE_CHECK(valueData, "cheer_up_video_config", Object)) {
//
//        auto cheerUpConfigObj = valueData["cheer_up_video_config"].GetObject();
//
//        if(JSON_SAFE_CHECK(cheerUpConfigObj,"cheer_up_mimic_display_turn_id", Array)) {
//
//            auto sessionList = cheerUpConfigObj["cheer_up_mimic_display_turn_id"].GetArray();
//
//            for(int i = 0; i < (int)sessionList.Size(); i++){
//
//                auto session = sessionList[i].GetString();
//
//                int castValue = -1;
//
//                try {
//                    castValue = stoi(session);
//                } catch (const std::string &msg) {
//                    CCLOG("Error: %s",msg.c_str());
//                }
//
//                if(castValue >=1 ) {
//
//                    _data._cheerUpConfig._cheerUpMIMICDisplayTurnIDs.push_back(castValue);
//                }
//            }
//        }
//
//        if(JSON_SAFE_CHECK(cheerUpConfigObj,"cheer_up_talking_display_turn_id", Array)) {
//
//            auto sessionList = cheerUpConfigObj["cheer_up_talking_display_turn_id"].GetArray();
//
//            for(int i = 0; i < (int)sessionList.Size(); i++){
//
//                auto session = sessionList[i].GetString();
//
//                int castValue = -1;
//
//                try {
//                    castValue = stoi(session);
//                } catch (const std::string &msg) {
//                    CCLOG("Error: %s",msg.c_str());
//                }
//
//                if(castValue >=1 ) {
//
//                    _data._cheerUpConfig._cheerUpTALKINGDisplayTurnIDs.push_back(castValue);
//                }
//            }
//        }
//
//       // getDataStringList("cheer_up_video_path", images, cheerUpConfigObj, _data._cheerUpConfig._cheerUpVideoPath);
//    }
//
//  //  getDataStringList("outro_video_path", images, valueData, _data._outroVideoPath);
//
//    auto sessionList = valueData["session"].GetArray();
//
//    for (int i = 0; i < (int)sessionList.Size(); i++) {
//
//        CCLOG("i = %d | size = %d", i, (int)sessionList.Size());
//
//        auto session = sessionList[i].GetObject();
//
//        if (JSON_SAFE_CHECK(session, "turns", Array) && JSON_SAFE_CHECK(session, "session_mode", String)) {
//
//            AISpeakingSessionInfo sessionInfo;
//
//            std::string mode = JSON_SAFE_EXTRACT(session, "session_mode", String, "mimic");
//            //if(getLessonInfo().is_learned)
//            //{
//            if (getAISpeakingMode() == ms::AISpeakingMode::TALKING && mode == "mimic") {
//                continue;
//            }
//            else if (getAISpeakingMode() == ms::AISpeakingMode::MIMIC && mode != "mimic") {
//                continue;
//            }
//            //}
//
//            sessionInfo._mode = (mode == "mimic") ? ms::AISpeakingMode::MIMIC : ms::AISpeakingMode::TALKING;
//
//            auto turns = sessionList[i]["turns"].GetArray();
//
//            for (int j = 0; j < (int)turns.Size(); j++) {
//
//                auto normalVideoInfo = math::resource::ResHelper::getImage(images, JSON_SAFE_CHECK(turns[j], "normal_video_path", String) ? turns[j]["normal_video_path"].GetString() : "");
//
//                auto slowVideoInfo = math::resource::ResHelper::getImage(images, JSON_SAFE_CHECK(turns[j], "slow_video_path", String) ? turns[j]["slow_video_path"].GetString() : "");
//
//                auto imageAndTextInfo = math::resource::ResHelper::getImage(images, JSON_SAFE_CHECK(turns[j], "text_and_image_path", String) ? turns[j]["text_and_image_path"].GetString() : "");
//
//                if (!normalVideoInfo || !imageAndTextInfo) {
//
//                    continue;
//                }
//
//                AISpeakingTurnInfo card;
//
//                card._audio.path = imageAndTextInfo->getAudio();
//                card._audio.sync_data = imageAndTextInfo->getSyncText();
//                card._text = imageAndTextInfo->getText();
//                card._questionText = imageAndTextInfo->getText("name_n");
//                card._imagePath = imageAndTextInfo->path;
//                card._normalVideoPath = normalVideoInfo->path;
//                card._slowVideoPath = (slowVideoInfo != nullptr ? slowVideoInfo->path : "");
//
//                sessionInfo._turns.push_back(card);
//            }
//
//            _data._listInfo.push_back(sessionInfo);
//        }
//    }
//
//    return initVideoInfoList();
//}
//
//bool AISpeakingScene::initVideoInfoList() {
//
//    int videoID = 0;
//
//    if(_data._listInfo.empty()) {
//        return false;
//    }
//
//    /////////////////////////////////////////////////////////////////////////////////
//    if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//    {
//        std::string introVideoPath = _data._introVideoPath.empty() ? "" : _data._introVideoPath.at(cocos2d::random(0, (int)_data._introVideoPath.size() - 1));
//
//        if(introVideoPath.empty()) {
//            return false;
//        }
//
//        _videoUrls.push_back({videoID,introVideoPath});
//        //sessionInfo._mode = (mode == "mimic") ? ms::AISpeakingMode::MIMIC
//        AISpeakingCustomVideoInfo* introVideoInfo = new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::INTRO_COMMON,introVideoPath, [=](AISpeakingCustomVideoInfo *info){
//
//            //CameraHelper::GetInstance()->startCapture(false,false);
//
//        },[=](AISpeakingCustomVideoInfo *info){
//
//            //CameraHelper::GetInstance()->endCapture(USER_INTRO_VIDEO_PATH,[](bool){});
//
//        }});
//        introVideoInfo->_mode = ms::AISpeakingMode::MIMIC;
//        _data._introVideo = *introVideoInfo;
//
//        _videoInfoCacheList.insert({_data._introVideo._videoID,introVideoInfo});
//        _reviewTurnList.push_back({{videoID,introVideoPath},{videoID,USER_INTRO_VIDEO_PATH},"",false});
//    }
//
//    /////////////////////////////////////////////////////////////////////////////////
//
//    int sessionID = 0;
//
//    int turnID = 0 ;
//    CameraHelper::GetInstance()->enableAudioWhenRecordCamera(false);
//    for(auto &mode :_data._listInfo) {
//
//        mode._sessionID = sessionID;
//
//        /////////////////////////////////////////////////////////////////////////////////
//
//        std::vector<std::string>  &tempList =(mode._mode ==  ms::AISpeakingMode::MIMIC)  ? _data._introMIMICModeVideoPath:_data._introTALKINGModeVideoPath;
//
//        std::string introVideoPath = tempList.empty() ? "" :tempList.at(cocos2d::random(0, (int)tempList.size() - 1));
//
//        if(!introVideoPath.empty()) {
//
//            _videoUrls.push_back({videoID,introVideoPath});
//
//            auto introVideoType =(mode._mode ==  ms::AISpeakingMode::MIMIC)  ?AISpeakingCustomVideoInfo::VideoTurnType::INTRO_MIMIC:AISpeakingCustomVideoInfo::VideoTurnType::INTRO_TALKING;
//
//            AISpeakingCustomVideoInfo* introMMTLInfo = new AISpeakingCustomVideoInfo({videoID++,introVideoType,introVideoPath,[=](AISpeakingCustomVideoInfo *info){
//                if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//                {
//                    CameraHelper::GetInstance()->startCapture(false,false);
//                    CameraHelper::GetInstance()->enableAudioWhenRecordCamera(false);
//                }
//                if(sessionID==0)
//                {
//                    playIntroActorCallback(info);
//                }
//
//            },
//                [=](AISpeakingCustomVideoInfo *info){
//
//                    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//                    {
//                        CameraHelper::GetInstance()->endCapture(USER_INTRO_VIDEO_PATH,[](bool){});
//                        CameraHelper::GetInstance()->enableAudioWhenRecordCamera(true);
//                    }
//
//
//            }});
//
//            if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//            {
//                introMMTLInfo->_mode = ms::AISpeakingMode::MIMIC;
//            }
//            else{
//                introMMTLInfo->_mode = ms::AISpeakingMode::TALKING;
//            }
//            mode._introMIMICOrTalkingVideo = *introMMTLInfo;
//
//            _videoInfoCacheList.insert({mode._introMIMICOrTalkingVideo._videoID,introMMTLInfo});
//
//            if(mode._mode ==  ms::AISpeakingMode::TALKING)
//            {
//                _reviewTurnList.push_back({{videoID,introVideoPath},{videoID,USER_INTRO_VIDEO_PATH},"",false});
////
////                _reviewTurnList.push_back({{videoID,introVideoPath},{videoID,StringUtils::format("%s/user_ls_%02d.mp4",AI_SPEAKING_FOLDER_PATH,_recordUserListeningVideoID++)},"",false});
//            }
//        }
//
//        /////////////////////////////////////////////////////////////////////////////////
//
//
//        std::vector<int> &tempCheerUpDislayTurnIDs = (mode._mode == ms::AISpeakingMode::MIMIC) ?
//        _data._cheerUpConfig._cheerUpMIMICDisplayTurnIDs :
//        _data._cheerUpConfig._cheerUpTALKINGDisplayTurnIDs;
//
//        int localID = 0;
//        for(auto &turn : mode._turns){
//
//            turn._id = turnID;
//
//            turn._localID = localID++;
//
//            std::string qnormalVideo    = turn._normalVideoPath;
//            std::string qslowVideo      = turn._slowVideoPath;
//            std::string listeningActorVideoPath = _data._listeningActorVideoPath.empty() ? "" : _data._listeningActorVideoPath.at(cocos2d::random(0, (int)_data._listeningActorVideoPath.size() - 1));
//
//            std::string yourTurnVideoPath       = _data._yourTurnVideoPath.empty() ? "" : _data._yourTurnVideoPath.at(cocos2d::random(0, (int)_data._yourTurnVideoPath.size() - 1));
//
//            _mergedVideoInfo._userNormalVideoPath = "ai_speaking/user_listening.mp4";
//
//            if(!qnormalVideo.empty()){
//
//                _videoUrls.push_back({videoID,qnormalVideo});
//
//                AISpeakingCustomVideoInfo *normalVideoInfo = new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::NORMAL_QUESTION_SPEED, qnormalVideo,[=](AISpeakingCustomVideoInfo *quesVideoInfo){
//
//                    playNormalSpeedQuestionStartCallback(quesVideoInfo);
//
//                },[=](AISpeakingCustomVideoInfo *quesVideoInfo){
//
//                    playNormalSpeedQuestionEndCallback(quesVideoInfo);
//
//                },turnID,mode._mode});
//
//                _videoInfoCacheList.insert({normalVideoInfo->_videoID,normalVideoInfo});
//
//                turn._videoInfoList.insert({(int)AISpeakingCustomVideoInfo::VideoTurnType::NORMAL_QUESTION_SPEED,normalVideoInfo});
//
//                if(mode._mode ==  ms::AISpeakingMode::TALKING) {
//
//                    _reviewTurnList.push_back({{videoID,qnormalVideo},{videoID,StringUtils::format("%s/user_ls_%02d.mp4",AI_SPEAKING_FOLDER_PATH,_recordUserListeningVideoID++)},turn._imagePath,false});
//                }
//            }
//
//            if(mode._mode ==  ms::AISpeakingMode::MIMIC) {
//
//                if(!qslowVideo.empty()) {
//
//                    _videoUrls.push_back({videoID,qslowVideo});
//
//                    AISpeakingCustomVideoInfo *slowVideoInfo = new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::SLOW_QUESTION_SPEED,qslowVideo,nullptr,nullptr,turnID,mode._mode});
//
//                    _videoInfoCacheList.insert({slowVideoInfo->_videoID,slowVideoInfo});
//
//                    turn._videoInfoList.insert({(int)AISpeakingCustomVideoInfo::VideoTurnType::SLOW_QUESTION_SPEED,slowVideoInfo});
//                }
//
//                if(!yourTurnVideoPath.empty()) {
//
//                    _videoUrls.push_back({videoID,yourTurnVideoPath});
//
//                    AISpeakingCustomVideoInfo *yourTurnVideoInfo =new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::YOUR_TURN,yourTurnVideoPath,nullptr,nullptr,turnID,mode._mode});
//
//                    _videoInfoCacheList.insert({yourTurnVideoInfo->_videoID,yourTurnVideoInfo});
//
//                    turn._videoInfoList.insert({(int)AISpeakingCustomVideoInfo::VideoTurnType::YOUR_TURN,yourTurnVideoInfo});
//                }
//            }
//
//            if(!listeningActorVideoPath.empty()) {
//
//                _videoUrls.push_back({videoID,listeningActorVideoPath});
//
//                AISpeakingCustomVideoInfo *listeningVideoInfo =   new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::LISTENING,listeningActorVideoPath,[=](AISpeakingCustomVideoInfo *videoInfo){
//
//                    playListeningActorStateStartCallback(videoInfo);
//
//                },[=](AISpeakingCustomVideoInfo *videoInfo){
//
//                    playListeningActorStateEndCallback(videoInfo);
//
//                },turnID,mode._mode});
//
//                _videoInfoCacheList.insert({listeningVideoInfo->_videoID,listeningVideoInfo});
//
//                turn._videoInfoList.insert({(int)AISpeakingCustomVideoInfo::VideoTurnType::LISTENING,listeningVideoInfo});
//
//                if(mode._mode ==  ms::AISpeakingMode::TALKING)
//                {
//                    _reviewTurnList.push_back({{videoID,listeningActorVideoPath},{videoID,USER_TALKING_VIDEO_PATH(turn._text)},turn._imagePath,true});
//                }
//            }
//
//            for(auto id:tempCheerUpDislayTurnIDs) {
//
//                if(id==turnID+1) {
//
//                    std::string cheerUpVideoPath = _data._cheerUpConfig._cheerUpVideoPath.empty() ? "" : _data._cheerUpConfig._cheerUpVideoPath.at(cocos2d::random(0, (int)_data._cheerUpConfig._cheerUpVideoPath.size() - 1));
//
//                    if(!cheerUpVideoPath.empty()) {
//
//                        _videoUrls.push_back({videoID,cheerUpVideoPath});
//
//                        AISpeakingCustomVideoInfo *cheerupVideoInfo =  new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::CHEER_UP,cheerUpVideoPath,nullptr,nullptr,turnID,mode._mode});
//
//                        _videoInfoCacheList.insert({cheerupVideoInfo->_videoID,cheerupVideoInfo});
//
//                        turn._videoInfoList.insert({(int)AISpeakingCustomVideoInfo::VideoTurnType::CHEER_UP,cheerupVideoInfo});
//
//                    }
//
//                    break;
//                }
//            }
//
//            _turnInfoCacheList.insert({turnID++,&turn});
//
//        }
//
//        sessionID++;
//    }
//
//    std::string outroVideoPath = _data._outroVideoPath.empty() ? "" : _data._outroVideoPath.at(cocos2d::random(0, (int)_data._outroVideoPath.size() - 1));
//
//    if(!outroVideoPath.empty()) {
//
//        _videoUrls.push_back({videoID,outroVideoPath});
//
//        AISpeakingCustomVideoInfo *outroV = new AISpeakingCustomVideoInfo({videoID++,AISpeakingCustomVideoInfo::VideoTurnType::OUTRO,outroVideoPath,[=](AISpeakingCustomVideoInfo *info){
//
//            if(getAISpeakingMode()== ms::AISpeakingMode::MIMIC)
//            {
//                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//                        auto screen = AISpeakingRelearnSplashScreen::createView(AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::LETTALK,getLessonInfo(), getDelegate());
//                        Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
//                    this->removeFromParent();
//                });
//
//            }
//            else
//            {
//                playOutroStartCallback();
//            }
//
//        },[=](AISpeakingCustomVideoInfo *info){
//            ms::AISpeakingMode mode = getAISpeakingMode();
//            if(getAISpeakingMode()!= ms::AISpeakingMode::MIMIC)
//            {
//                playOutroEndCallback();
//            }
//
//        }});
//
//
//        _data._outroVideo = *outroV;
//
//        _videoInfoCacheList.insert({_data._outroVideo._videoID,outroV});
//
//        _reviewTurnList.push_back({{videoID,outroVideoPath},{videoID,USER_OUTRO_VIDEO_PATH},"",true});
//    }
//
//    _recordUserListeningVideoID = 0;
//
//    return true;
//}
//
//void AISpeakingScene::playGame() {
//
//    _gameLayer->setVisible(false);
//
//    auto cameraArea = utils::findChild(_introLayer, "camera_area");
//
//    auto cameraWordPos = _introLayer->convertToWorldSpace(cameraArea->getPosition());
//
//    CameraHelper::GetInstance()->addViewToCamera(cameraWordPos - Vec2(cameraArea->getContentSize()/2.f) ,{cameraArea->getContentSize().width,cameraArea->getContentSize().height});
//
//    _record = MSRecord::createRecordWithDuration(MAX_DURATION_FOR_USER_RECORD,"ai_speaking/haha.wav",false,true,MSRecord::Type::AVAudioSessionCategoryOptionMixWithOthers);
//    _record->setDelegate(this);
//    _record->setName("_record");
//    this->addChild(_record);
//
//    _videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
//    _videoPlayer->setURLs(_videoUrls);
//    _videoAreaAtIntroLayer->addChild(_videoPlayer, INT_MAX - 1);
//    _videoPlayer->setCornerRadius(10.0f);
//    _videoPlayer->setContentSize(_videoAreaAtIntroLayer->getContentSize());
//    _videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _videoPlayer->setPosition(_videoAreaAtIntroLayer->getContentSize()/2.f);
//    _videoPlayer->play();
//    _videoPlayer->addEventListener([=](Ref* sender, VideoPlayer::EventType eventType) {
//        switch (eventType) {
//            case VideoPlayer::EventType::PLAYING:
//                CCLOG("PLAYING");
//                break;
//            case VideoPlayer::EventType::PAUSED:
//                CCLOG("PAUSED");
//                break;
//            case VideoPlayer::EventType::STOPPED:
//                CCLOG("STOPPED");
//                break;
//            case VideoPlayer::EventType::COMPLETED: {
//
//                CCLOG("COMPLETED");
//                _learedVideocount++;
//
//                break;
//            }
//            default:
//                break;
//        }
//    });
//
//    _videoInfoTracks = _videoPlayer->getInfoTracks();
//
//    for(auto video : _videoInfoCacheList){
//
//        if(video.second) {
//
//            auto id = video.second->_videoID;
//
//            if(_videoInfoTracks.count(id)>0){
//
//                auto track = _videoInfoTracks.at(id);
//
//                video.second->_timeStart    = track.first;
//
//                video.second->_timeEnd      = track.second;
//
//                video.second->_duration     = track.second - track.first;
//
//                CCLOG("xxx vID:%d,timeStar:%f,timeEnd:%f,vPath:%s",video.second->_videoID,video.second->_timeStart,video.second->_timeEnd,video.second->_videoUrl.c_str());
//
//            }
//
//        }
//    }
//
//    this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f),CallFunc::create([=](){
//
//        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//    })));
//
//}
//
//void AISpeakingScene::playGameScheduleUpdate(float dt)
//{
//    float duration = _videoPlayer->getCurrentTime();
//
//    for(auto track: _videoInfoTracks) {
//
//        //track.second.first timestart
//        //track.second.second timeEnd
//        //[timestart,timeEnd)[
//        if(duration > track.second.first && duration <= track.second.second)
//        {
//            const int k_currentPlayingVideoID = track.first;
//
//            int k_priousPlayingVideoID  = track.first - 1;
//
//            if(k_currentPlayingVideoID == _videoInfoCacheList.size() -1 && duration >= track.second.second) {
//
//                k_priousPlayingVideoID = k_currentPlayingVideoID;
//            }
//
//           // CCLOG("AISpeaking: k_currentPlayingVideoID = %d",k_currentPlayingVideoID);
//
//            if(k_priousPlayingVideoID < _videoInfoCacheList.size() && k_priousPlayingVideoID >= 0){
//
//                if(_videoInfoCacheList.count(k_priousPlayingVideoID)>0) {
//
//                    auto info = _videoInfoCacheList.at(k_priousPlayingVideoID);
//
//                    if(info->_endCallback  != nullptr  && !info->_isEndCallbackCalled) {
//
//                        CCLOG("NextLevel AISpeaking: k_priousPlayingVideoID .end callback call. ID = %d ,url = %s\n",k_priousPlayingVideoID,info->_videoUrl.c_str());
//
//                        info->_isEndCallbackCalled = true;
//                        info->_endCallback(info);
//                    }
//
//                }
//            }
//
//            if(k_currentPlayingVideoID < _videoInfoCacheList.size() && k_currentPlayingVideoID >= 0 ){
//
//                if(_videoInfoCacheList.count(k_currentPlayingVideoID)>0) {
//
//                    auto info = _videoInfoCacheList.at(k_currentPlayingVideoID);
//
//                    if(info->_startCallback != nullptr && !info->_isStartCallbackCalled) {
//
//                        CCLOG("NextLevel AISpeaking: k_currentPlayingVideoID .start callback call. ID = %d ,url = %s \n",k_currentPlayingVideoID,info->_videoUrl.c_str());
//
//                        info->_isStartCallbackCalled = true;
//                        info->_startCallback(info);
//                    }
//
//                    //_currentVideoInfo = &info;
//
//                }
//            }
//
//            _currentPlayingVideoID = k_currentPlayingVideoID;
//
//        }
//    }
//
//    if (_record) {
//
//        _record->checkBlow();
//    }
//
//    CCLOG("video_duration %f",duration);
//}
//
//void AISpeakingScene::playIntroActorCallback(AISpeakingCustomVideoInfo *quesVideoInfo) {
//
//    CCLOG("AISpeaking: playIntroActorCallback %s",__FUNCTION__);
//
//    auto cameraGameLayerArea = utils::findChild(_gameLayer, "camera_area");
//
//    auto cameraWordPos = _gameLayer->convertToWorldSpace(cameraGameLayerArea->getPosition()*1.05);
//
//    CameraHelper::GetInstance()->moveFrontCameraToPos(cameraWordPos - Vec2(cameraGameLayerArea->getContentSize() / 2.f), { cameraGameLayerArea->getContentSize().width,cameraGameLayerArea->getContentSize().height });
//
//    auto videoGameArea = utils::findChild(_gameLayer, "video_area");
//
//    auto pos = _videoAreaAtIntroLayer->convertToNodeSpace(_gameLayer->convertToWorldSpace(videoGameArea->getPosition()));
//
//    _videoPlayer->runAction(Sequence::create(MoveTo::create(1.0f, pos), ResizeTo::create(1.0f, videoGameArea->getContentSize()), CallFunc::create([=]() {
//
//        _introLayer->setVisible(false);
//
//    }), NULL));
//}
//
//void AISpeakingScene::playNormalSpeedQuestionStartCallback(AISpeakingCustomVideoInfo *quesVideoInfo) {
//
//    CCLOG("AISpeaking: playNormalSpeedQuestionCallback vID:%d tID:%d mode:%s",quesVideoInfo->_videoID,quesVideoInfo->_turnID,quesVideoInfo->_mode ==  ms::AISpeakingMode::MIMIC ?"mimic":"talking");
//
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING) {
//
//        CameraHelper::GetInstance()->startCapture(false,false);
//    }
//
//    _mergedVideoInfo._actorQuesVideoPath = quesVideoInfo->_videoUrl;
//
//    CCLOG("AISpeaking: _gameLayer->setVisible true %s",__FUNCTION__);
//
//    _gameLayer->setVisible(true);
//
//    //_textButton->setEnabled(true);
//
//    if(_videoInfoCacheList.count(quesVideoInfo->_videoID) > 0) {
//
//        CCLOG("AISpeaking: playNormalSpeedQuestionCallack found video info %s",__FUNCTION__);
//
//        if(auto turnInfo = _turnInfoCacheList.at(quesVideoInfo->_turnID)) {
//
//            if(!getLessonInfo().is_learned)
//            //if(MJDEFAULT->getBoolForKey(AI_SPEAKING_FIRST_TIME,true))
//            {
//                _textButton->setEnabled(false);
//
//                if (auto text = utils::findChild<ui::Text*>(_textButton, "text")) {
//
//                    text->setTextColor(Color4B(1,196,248,255));
//                }
//            }
//            else
//            {
//                _textButton->setEnabled(true);
//
//                if (auto text = utils::findChild<ui::Text*>(_textButton, "text")) {
//
//                    text->setTextColor(Color4B::WHITE);
//                }
//            }
//            if(!_isResetCurrentState)
//            {
//                if (auto text = utils::findChild<ui::Text*>(_gameLayer, "text")) {
//                    std::string str = quesVideoInfo->_mode == ms::AISpeakingMode::TALKING ? turnInfo->_questionText:turnInfo->_text;
//                    float fontSize = getFontSizeForText(_textButton->getContentSize(), text, text->getFontSize(), str);
//                    text->setFontSize(fontSize);
//                    text->setString(str);
//                }
//            }
//            _isResetCurrentState = false;
//
//            _mergedVideoInfo._imagePath = turnInfo->_imagePath;
//
//            if (auto displayImage = utils::findChild(_gameLayer, "display_image")) {
//
//                displayImage->removeFromParent();
//            }
//
//            if (auto image = utils::findChild<ui::ImageView*>(_gameLayer, "image")) {
//
//                auto img = ImageView::create(turnInfo->_imagePath);
//                img->setScaleX(image->getContentSize().width / img->getContentSize().width);
//                img->setScaleY(image->getContentSize().height / img->getContentSize().height);
//                img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//                img->setPosition(image->getPosition());
//                img->setName("display_image");
//                image->getParent()->addChild(img);
//                image->setVisible(false);
//            }
//
//            _mergedVideoInfo._actorVideoQuestDurationTime = quesVideoInfo->_duration;
//        }
//    }
//}
//
//void AISpeakingScene::playNormalSpeedQuestionEndCallback(AISpeakingCustomVideoInfo *quesVideoInfo) {
//
//    CCLOG("AISpeaking: playNormalSpeedQuestionEndCallback vID:%d tID:%d mode:%s",quesVideoInfo->_videoID,quesVideoInfo->_turnID,quesVideoInfo->_mode ==  ms::AISpeakingMode::MIMIC ?"mimic":"talking");
//
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING) {
//
//        CameraHelper::GetInstance()->endCapture(StringUtils::format("%s/user_ls_%02d.mp4",AI_SPEAKING_FOLDER_PATH,_recordUserListeningVideoID++),[](bool){});
//    }
//}
//
//void AISpeakingScene::playListeningActorStateStartCallback(AISpeakingCustomVideoInfo *videoInfo) {
//
//    CCLOG("AISpeaking: playListeningActorStateStartCallback %s",__FUNCTION__);
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//        CameraHelper::GetInstance()->enableAudioWhenRecordCamera(true);
//    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("ai_speaking_ev_animate_record_button");
//
//    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("ai_speaking_ev_idle_record_button");
//
//    _textButton->setEnabled(false);
//
//    if (auto text = utils::findChild<ui::Text*>(_textButton, "text")) {
//
//        text->setTextColor(Color4B(1,196,248,255));
//    }
//
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING) {
//
//        if(_videoInfoCacheList.count(videoInfo->_videoID) > 0) {
//
//            if(auto turnInfo = _turnInfoCacheList.at(videoInfo->_turnID)) {
//
//                if (auto text = utils::findChild<ui::Text*>(_gameLayer, "text")) {
//
//                    std::string str = turnInfo->_text;
//                    float fontSize = getFontSizeForText(_textButton->getContentSize(), text, text->getFontSize(), str);
//                    text->setFontSize(fontSize);
//                    text->setString(str);
//                }
//
//                _textButton->setEnabled(true);
//
//                if (auto text = utils::findChild<ui::Text*>(_textButton, "text")) {
//
//                    text->setTextColor(Color4B::WHITE);
//                }
//            }
//        }
//        CameraHelper::GetInstance()->startCapture(true,true);
//        if(videoInfo->_videoType == AISpeakingCustomVideoInfo::VideoTurnType::INTRO_TALKING)
//            CameraHelper::GetInstance()->enableAudioWhenRecordCamera(false);
//        else
//            CameraHelper::GetInstance()->enableAudioWhenRecordCamera(true);
//    }
//    else
//    {
//        CameraHelper::GetInstance()->enableBorder(2, Color4B(1,196,248,255));
//    }
//
//    cocos2d::experimental::AudioEngine::play2d("sounds/recording_start.mp3");
//
//    playUserStateRecordAudio();
//
//    _mergedVideoInfo._actorListeningVideoPath = videoInfo->_videoUrl;
//}
//
//void AISpeakingScene::playListeningActorStateEndCallback(AISpeakingCustomVideoInfo *videoInfo) {
//
//    CCLOG("AISpeaking: playListeningActorStateEndCallback %s",__FUNCTION__);
//
//    resetRecord();
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//        CameraHelper::GetInstance()->enableAudioWhenRecordCamera(false);
//    _textButton->setEnabled(false);
//
//    if(_videoInfoCacheList.count(videoInfo->_turnID)>0) {
//
//        CCLOG("AISpeaking: _gameLayer->setVisible false %s",__FUNCTION__);
//
//        _gameLayer->setVisible(false);
//
//        AISpeakingTurnInfo *info = _turnInfoCacheList.at(videoInfo->_turnID);
//
//        _mergedVideoInfo._userSpeakVideoPath =  USER_TALKING_VIDEO_PATH(info->_text);
//
//        auto capinfo = _mergedVideoInfo;
//
//        if(getAISpeakingMode() == ms::AISpeakingMode::TALKING) {
//
//            CCLOG("AISpeaking:XXXXXXXXXXXXXXXXX endCapture %s",__FUNCTION__);
//
//            CameraHelper::GetInstance()->endCapture(USER_TALKING_VIDEO_PATH(info->_text),[=](bool isSuccess){
//
//            });
//        }
//        else
//        {
//            CameraHelper::GetInstance()->enableBorder(1.5, Color4B::GRAY);
//        }
//    }
//}
//
//
//void AISpeakingScene::playOutroStartCallback() {
//
//    _lessonInfo.state = mj::model::LessonInfo::State::LEARNED;
//
//    CCLOG("AISpeaking: playOuttroStartCallback %s",__FUNCTION__);
//
//    CCLOG("AISpeaking: _gameLayer->setVisible false %s",__FUNCTION__);
//
//   //
//
//    _gameLayer->setVisible(false);
//
//    _introLayer->setVisible(true);
//
//    auto videoIntroArea = utils::findChild(_introLayer, "video_area");
//    auto cameraGameLayerArea = utils::findChild(_introLayer, "camera_area");
//    auto cameraWordPos = _gameLayer->convertToWorldSpace(cameraGameLayerArea->getPosition());
//    CameraHelper::GetInstance()->moveFrontCameraToPos(cameraWordPos * 0.95, { cameraGameLayerArea->getContentSize().width,cameraGameLayerArea->getContentSize().height });
//    auto pos = _videoAreaAtIntroLayer->convertToNodeSpace(_introLayer->convertToWorldSpace(videoIntroArea->getPosition()));
//    _videoPlayer->runAction(Sequence::create(MoveTo::create(1.0f, pos),CallFunc::create([=](){
//
//    }), ResizeTo::create(1.0f, videoIntroArea->getContentSize()), CallFunc::create([=]() {
//        if(getAISpeakingMode() == ms::AISpeakingMode::TALKING){
//            CameraHelper::GetInstance()->startCapture(false,false);
//
//        }
//
//    }), NULL));
//}
//
//void AISpeakingScene::playOutroEndCallback() {
//
//    CCLOG("AISpeaking: playOuttroEndCallback %s",__FUNCTION__);
//
//    this->unscheduleAllCallbacks();
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING) {
//    CameraHelper::GetInstance()->endCapture(USER_OUTRO_VIDEO_PATH,[=](bool isSuccess){
//
//    });
//    }
//
//    CameraHelper::GetInstance()->removeCamera([=](){
//
//        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//            if(!getLessonInfo().is_learned)
//            {
//
//                this->removeAllChildren();
//                auto pop = AISpeakingFakeCallScreen::createView(AISpeakingFakeCallScreen::AISpeakingFakeCallScreenType::END_CALLING_SCREEN,getLessonInfo(), {}, this->getDelegate());
//
//                //
//                this->addChild(pop,INT_MAX-1);
//                this->runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([=](){
//
//                    // push info to CleverTap
//                    pushEventAIspeakingToCleaverTap();
//
//                    Director::getInstance()->pushScene(AISpeakingReviewScreen::createScene(_reviewTurnList, getLessonInfo()));
//
//                    this->removeFromParent();
//
//                }),NULL));
//
//
//
//            }
//            else
//            {
//                auto screen = AISpeakingRelearnSplashScreen::createView(AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::REVIEW,_lessonInfo, this->getDelegate());
//                screen->setReviewTurnList(_reviewTurnList);
//                Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
//                this->removeFromParent();
//            }
//
//        });
//    });
//
//}
//
//void AISpeakingScene::playUserStateRecordAudio() {
//
//    CCLOG("AISpeaking: playUserStateRecordAudio %s",__FUNCTION__);
//
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    _recordButton->setVisible(false);
//    if(getAISpeakingMode()== ms::AISpeakingMode::TALKING)
//        _speakerIcon->setVisible(true);
//    _recordButton->stopActionByTag(RECORD_BUTTON_ACTION_TAG);
//
//    auto action = Sequence::createWithTwoActions(DelayTime::create(2.0f),Show::create());
//
//    action->setTag(RECORD_BUTTON_ACTION_TAG);
//
//    _recordButton->runAction(action);
//
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    CCLOG("AISpeaking: playUserStateRecordAudio currentTime %lld",mj::helper::getTime());
//
//    this->stopActionByTag(1998);
//
//    auto recordCountAction = Sequence::createWithTwoActions(DelayTime::create(MAX_DURATION_FOR_USER_RECORD),CallFunc::create([=](){
//
//        CCLOG("AISpeaking: playUserStateRecordAudio endTime %lld",mj::helper::getTime());
//
//        resetRecord();
//
//    }));
//
//    recordCountAction->setTag(1998);
//
//    this->runAction(recordCountAction);
//
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    auto addRecordButtonFunction = [=](int tag, int animationID) {
//
//        if (_recordButton) {
//
//            _recordButton->getParent()->removeChildByName("recordSpineButton");
//
//            std::string animantionName = (animationID <= 1?"animation": StringUtils::format("animation%d",animationID));
//
//            CCLOG("animationName:%s",animantionName.c_str());
//
//            auto recordSpineButton = SpineWrapper::createWithPath("spine/monkey_talking/Record");
//            recordSpineButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            recordSpineButton->setPosition(_recordButton->getPosition() + Vec2(0, RECORD_BUTTON_DELTA_POS));
//            recordSpineButton->playSequence(tag == RECORD_BUTTON_NORMAL_ANIMATION_TAG ? "idle" :animantionName, true);
//            recordSpineButton->setSkin("MS");
//            recordSpineButton->setName("recordSpineButton");
//            recordSpineButton->setTag(tag);
//            //recordSpineButton->addClickEventListener(CC_CALLBACK_1(AISpeakingScene::onRecordButtonClicked, this));
//
//            _recordButton->getParent()->addChild(recordSpineButton);
//
//        }
//    };
//
//    addRecordButtonFunction(RECORD_BUTTON_NORMAL_ANIMATION_TAG,0);
//
//    if (_recordButton)
//    {
//        auto timerAnimationSpine = SpineWrapper::createWithPath("spine/monkey_talking/Time");
//        timerAnimationSpine->setAnchorPoint(Vec2(0.5, 0.5));
//        timerAnimationSpine->setPosition(_recordButton->getPosition() + Vec2(0, RECORD_BUTTON_DELTA_POS - 10.0f));
//        timerAnimationSpine->playSequence("animation",10.0f,false);
//        timerAnimationSpine->setSkin("MS");
//        timerAnimationSpine->setName("timerAnimationSpine");
//        //timerAnimationSpine->addClickEventListener(CC_CALLBACK_1(AISpeakingScene::onRecordButtonClicked, this));
//        _recordButton->getParent()->addChild(timerAnimationSpine,100);
//    }
//
//    Director::getInstance()->getEventDispatcher()->addCustomEventListener("ai_speaking_ev_animate_record_button", [=](EventCustom* eventCustom) {
//
//        int* edata = (int*)eventCustom->getUserData();
//
//        int data = 1;
//
//        if (edata != nullptr) {
//
//            data = *edata;
//
//            delete edata;
//        }
//
//        CCLOG("ai_speaking_ev_animate_record_button data= %d",data);
//
//        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//
//            if (auto analyzeSpine = utils::findChild<SpineWrapper*>(this, "recordSpineButton")) {
//
//                if(analyzeSpine->getTag()== RECORD_BUTTON_BLOW_ANIMATION_TAG) {
//                    return;
//                }
//
//                addRecordButtonFunction(RECORD_BUTTON_BLOW_ANIMATION_TAG,data);
//            }
//        });
//
//    });
//
//    Director::getInstance()->getEventDispatcher()->addCustomEventListener("ai_speaking_ev_idle_record_button", [=](EventCustom* eventCustom) {
//
//        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//
//                if (auto analyzeSpine = utils::findChild<SpineWrapper*>(this, "recordSpineButton")) {
//
//                    if(analyzeSpine->getTag()== RECORD_BUTTON_NORMAL_ANIMATION_TAG){
//                        return;
//                    }
//
//                    addRecordButtonFunction(RECORD_BUTTON_NORMAL_ANIMATION_TAG,0);
//                }
//
//            });
//
//        });
//
//    });
//
//    if (_record) {
//
//        if(!_record->isRecording()) {
//
//            _record->startRecord();
//        }
//
//        return;
//        //_record->removeFromParent();
//    }
//
//    _record = MSRecord::createRecordWithDuration(MAX_DURATION_FOR_USER_RECORD,"ai_speaking/haha.wav",false,true,MSRecord::Type::AVAudioSessionCategoryOptionMixWithOthers);
//
//
//    if (!_record) {
//        return;
//    }
//    _record->setDelegate(this);
//    _record->setName("_record");
//    this->addChild(_record);
//}
//
//void AISpeakingScene::resetRecord() {
//
//    cocos2d::experimental::AudioEngine::play2d("sounds/recording_end.mp3");
//
//    CCLOG("%s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//    if (Director::getInstance()->getEventDispatcher()->hasEventListener("ai_speaking_ev_animate_record_button")) {
//
//        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("ai_speaking_ev_animate_record_button");
//    }
//
//    if (Director::getInstance()->getEventDispatcher()->hasEventListener("ai_speaking_ev_idle_record_button")) {
//
//        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("ai_speaking_ev_idle_record_button");
//    }
//
//    if (_record) {
//
//        _record->stopRecord();
//
//        //_record->removeFromParent();
//
//       // _record = nullptr;
//    }
//
//    if (auto node = utils::findChild(this, "recordSpineButton")) {
//
//        node->removeFromParent();
//    }
//
//    if (auto node = utils::findChild(this, "timerAnimationSpine")) {
//
//        node->removeFromParent();
//    }
//
//    if(_recordButton) {
//        _recordButton->setVisible(false);
//        _speakerIcon->setVisible(false);
//    }
//}
//
//void AISpeakingScene::resetAll()
//{
//    CCLOG("%s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//    resetRecord();
//
//    //reset text.
//    if (auto text = utils::findChild<ui::Text*>(_gameLayer, "text")) {
//
//        text->setString("");
//    }
//
//    //reset image.
//    if (auto displayImage = utils::findChild(_gameLayer, "display_image")) {
//
//        displayImage->removeFromParent();
//    }
//
//    //reset camera.
//
//    _mergedVideoInfo = MergedVideoInfo();
//}
//
//void AISpeakingScene::onFinishRecord(int finish_type, std::string url) {
//
//    CCLOG("%s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//    std::string absolutePath = FileUtils::getInstance()->getWritablePath() + url;
//    std::string path = "";
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    path = absolutePath;
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    path = url;
//#endif
//    CCLOG("+++++++++++++++++++++++++++%s", url.c_str());
//    CCLOG("+++++++++++++++++++++++++++%s", absolutePath.c_str());
//
//    switch (finish_type)
//    {
//        case MSRecord::RECORD_TYPE::RECORD_DONE:
//        {
//            break;
//        }
//        case MSRecord::RECORD_TYPE::PLAY_DONE:
//        {
//            break;
//        }
//        default:
//            break;
//    }
//}
//
//void AISpeakingScene::onRecordButtonClicked(Ref* sender) {
//
//    CCLOG("onRecordButtonClicked");
//
//    resetRecord();
//    if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//        CameraHelper::GetInstance()->enableAudioWhenRecordCamera(false);
//    _timeToClickRecoding++;
//    this->unschedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//    if(_videoInfoCacheList.count(_currentPlayingVideoID)>0) {
//
//        CCLOG("onRecordButtonClicked _currentPlayingVideoID found");
//
//        auto videoInfo =_videoInfoCacheList.at(_currentPlayingVideoID);
//
//        if(_turnInfoCacheList.count(videoInfo->_turnID) > 0) {
//
//            auto turn = _turnInfoCacheList.at(videoInfo->_turnID);
//
//            CCLOG("onRecordButtonClicked _turnInfoCacheList found turnID:%d seekWithoutDelay", turn->_id);
//
//            if(turn) {
//
//                auto listening = turn->_videoInfoList.at((int)AISpeakingCustomVideoInfo::VideoTurnType::LISTENING);
//
//                _videoPlayer->seekWithoutDelay(listening->_timeEnd);
//
//            }
//        }
//    }
//
//    ((Node*)sender)->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f),CallFuncN::create([=](Node *node){
//
//        if(auto button = dynamic_cast<Button*>(node)) {
//
//            button->setEnabled(true);
//        }
//
//        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//    })));
//}
//
//void AISpeakingScene::resetCurrentState(Ref* sender) {
//
//    if(_videoInfoCacheList.count(_currentPlayingVideoID)>0) {
//
//        _isClickAnswer++;
//        CCLOG("onTextButtonClicked vao count 1 %s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//        auto videoInfo = _videoInfoCacheList.at(_currentPlayingVideoID);
//
//        if(_turnInfoCacheList.count(videoInfo->_turnID) > 0) {
//
//            CCLOG("onTextButtonClicked vao count 2 %s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//            auto turn = _turnInfoCacheList.at(videoInfo->_turnID);
//
//            if(turn) {
//
//                auto normalVideoInfo = turn->_videoInfoList.at((int)AISpeakingCustomVideoInfo::VideoTurnType::NORMAL_QUESTION_SPEED);
//
//                CCLOG("xxx found r ne, seek thui! %s url:%s turnID:%d currentVID:%d timeStart:%f timeEnd:%f vID:%d", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str(),normalVideoInfo->_videoUrl.c_str(),videoInfo->_turnID,_currentPlayingVideoID,normalVideoInfo->_timeStart,normalVideoInfo->_timeEnd,normalVideoInfo->_videoID);
//
//                normalVideoInfo->_isStartCallbackCalled = false;
//
//                normalVideoInfo->_isEndCallbackCalled = false;
//
//                auto listeningVideoInfo = turn->_videoInfoList.at((int)AISpeakingCustomVideoInfo::VideoTurnType::LISTENING);
//
//                listeningVideoInfo->_isEndCallbackCalled = false;
//
//                listeningVideoInfo->_isStartCallbackCalled = false;
//
//                if(videoInfo->_mode == ms::AISpeakingMode::MIMIC) {
//
//                    _videoPlayer->seekWithoutDelay(normalVideoInfo->_timeStart);
//
//                    ((Node*)sender)->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f),CallFuncN::create([=](Node *node){
//
//                        ((Button*)node)->setEnabled(true);
//
//                        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//                    })));
//                }
//                else
//                {
//                    _videoPlayer->pause();
//
//                    auto text =  utils::findChild<ui::Text*>(_gameLayer, "text");
//
//                    //text->setVisible(false);
//
//                    if(_currentAudioID > 0) {
//
//                        cocos2d::experimental::AudioEngine::stop(_currentAudioID);
//                    }
//
//                    _currentAudioID = cocos2d::experimental::AudioEngine::play2d(turn->_audio.path);
//
//                    ms::game::highLightText(text, this, text->getTextColor(),Color3B::RED, 0, turn->_audio.sync_data,  ms::game::getTextIndexSentence(text->getString(),turn->_audio.sync_data), [=](int,std::string){
//                        _isResetCurrentState = true;
//                        _videoPlayer->seekWithoutDelay(listeningVideoInfo->_timeStart);
//
//                        ((Button*)sender)->setEnabled(true);
//
//                        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//                        if(!_isClickHangUpPopup)
//                            _videoPlayer->resume();
//
//                    });
//                }
//            }
//        }
//    }
//}
//
//void AISpeakingScene::onTextButtonClicked(Ref* sender) {
//
//    if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//        return;
//    CCLOG("onTextButtonClicked %s", StringUtils::format("AISpeaking: %s", __FUNCTION__).c_str());
//
//    ((Button*)sender)->setEnabled(false);
//
//    resetRecord();
//
//    CameraHelper::GetInstance()->stop();
//
//    this->unschedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//    resetCurrentState(sender);
//}
//
//void AISpeakingScene::onBackButtonClicked(Ref* sender) {
//    this->unschedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//    _videoPlayer->pause();
//
//    setLayoutVisible(false);
//
//    _isListeningVideo = false;
//
//    if(_videoInfoCacheList.count(_currentPlayingVideoID)>0) {
//
//        auto videoInfo =_videoInfoCacheList.at(_currentPlayingVideoID);
//
//        if(videoInfo->_videoType == AISpeakingCustomVideoInfo::VideoTurnType::LISTENING) {
//
//            _isListeningVideo = true;
//        }
//    }
//
//    if(_isListeningVideo) {
//
//        resetRecord();
//
//        CameraHelper::GetInstance()->stop();
//    }
//
//    if(!getLessonInfo().is_learned)
//    //if(MJDEFAULT->getBoolForKey(AI_SPEAKING_FIRST_TIME,true))
//    {
//        _isClickHangUpPopup = true;
//        if( auto popup = AISpeakingClosingPopup::createView(AISpeakingClosingPopup::AISpeakingClosingPopupType::FIRST_CALL)) {
//
//            popup->setHangUpCallback([=](){
//                _isClickHangUpPopup = false;
//                auto pop = AISpeakingFakeCallScreen::createView(AISpeakingFakeCallScreen::AISpeakingFakeCallScreenType::END_CALLING_SCREEN,getLessonInfo(), {}, this->getDelegate());
//                //pushEventAIspeakingToCleaverTap();
//                pop->runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([=](){
//
//                    _videoPlayer->resume();
//
//                    this->removeFromParent();
//
//                }), RemoveSelf::create(),NULL));
//
//                Director::getInstance()->getRunningScene()->addChild(pop,INT_MAX-1);
//
//            });
//
//            popup->setContinueCallback([=](){
//                _isClickHangUpPopup = false;
//                setLayoutVisible(true);
//
//                _videoPlayer->resume();
//
//                if(_isListeningVideo) {
//
//                    resetCurrentState(utils::findChild(this, "text_button"));
//                }
//                else
//                {
//                    if(!this->isScheduled(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate))) {
//
//                        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//                    }
//
//                }
//
//            });
//
//            Director::getInstance()->getRunningScene()->addChild(popup,INT_MAX-1);
//        }
//    }
//    else
//    {
//        bool isLastTurn =  false;
//
//        if(_videoInfoCacheList.count(_currentPlayingVideoID)>0) {
//
//            auto videoInfo =_videoInfoCacheList.at(_currentPlayingVideoID);
//
//            isLastTurn = (videoInfo->_mode == ms::AISpeakingMode::TALKING);
//        }
//        if(getAISpeakingMode() == ms::AISpeakingMode::TALKING)
//            isLastTurn = true;
//        else
//            isLastTurn = false;
//
//        if( auto popup = AISpeakingClosingPopup::createView(isLastTurn?AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN_LAST_TURN:AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN)) {
//            popup->setClosePopupCallback([=](){
//                setLayoutVisible(true);
//                _videoPlayer->resume();
//
//                if(_isListeningVideo) {
//
//                    resetCurrentState(utils::findChild(this, "text_button"));
//                }
//                else
//                {
//                    if(!this->isScheduled(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate))) {
//
//                        this->schedule(CC_SCHEDULE_SELECTOR(AISpeakingScene::playGameScheduleUpdate));
//
//                    }
//
//                }
//
//            });
//
//            popup->setSkipCallback([=](){
//                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//                    _skipMIMIC = true;
//                        auto screen = AISpeakingRelearnSplashScreen::createView(AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::LETTALK,getLessonInfo(), getDelegate());
//                        Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
//                    this->removeFromParent();
//                    mj::helper::deleteWaitView();
//                });
//
//            });
//            popup->setExitCallback([=](){
//
//                //setLayoutVisible(false);
//                pushEventAIspeakingToCleaverTap();
//                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//                    CameraHelper::GetInstance()->clean();
//                    this->removeFromParent();
//                });
//
//            });
//
//            Director::getInstance()->getRunningScene()->addChild(popup,INT_MAX-1);
//        }
//    }
//}
//
//void AISpeakingScene::onExit() {
//
//    ms::BaseActivity::onExit();
//
//    commonExit();
//}
//
//void AISpeakingScene::commonExit() {
//
//    if(_currentAudioID > 0) {
//
//        cocos2d::experimental::AudioEngine::stop(_currentAudioID);
//    }
//    HSAudioEngine::resetAudioEngine();
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//        CameraHelper::GetInstance()->clean();
//    });
//
//    //exit end turn => finish lesson.
//    if(_lessonInfo.state == mj::model::LessonInfo::State::LEARNED) {
//        _lessonInfo.is_learned = true;
//        _delegate ? _delegate->onSavelessonLearnAfterLearned(_lessonInfo): (void) nullptr;;
//    }
//    pushEventAIspeakingToCleaverTap();
//}
//
//void AISpeakingScene::setLayoutVisible(bool isVisible) {
//
//
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//        _rootLayout->setVisible(isVisible);
//        CameraHelper::GetInstance()->setVisible(isVisible);
//        _videoPlayer->setVisible(isVisible);
//
//
//    });
//}
//
//void AISpeakingScene::getDataStringList(std::string key,std::vector<math::resource::Image*> images, const rapidjson::Value& valueData , std::vector<std::string>& outputRef)
//{
//    if(JSON_SAFE_CHECK(valueData, key.c_str(), Array)) {
//
//        auto listObject = valueData[key.c_str()].GetArray();
//
//        for (int i = 0; i < (int)listObject.Size(); i++) {
//
//            std::string str = listObject[i].IsString()? listObject[i].GetString():"";
//
//            auto normalVideoInfo = math::resource::ResHelper::getImage(images, str);
//
//            if(normalVideoInfo) {
//
//                outputRef.push_back(normalVideoInfo->path);
//            }
//        }
//    }
//}
//
//cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingScene::onLocateClickCallback(const std::string & callBackName) {
//
//    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(AISpeakingScene, onTextButtonClicked),
//        CLICK_MAP(AISpeakingScene, onBackButtonClicked),
//        CLICK_MAP(AISpeakingScene, onRecordButtonClicked)
//    };
//
//    if (function_map.find(callBackName) != function_map.end()) {
//        return function_map.at(callBackName);
//    }
//
//    return nullptr;
//}
//
//float AISpeakingScene::getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput) {
//    text->setFontSize(35); //default fontsize of text is 30
//    auto textTemp = Text::create(strInput, text->getFontName(), text->getFontSize());
//    auto text_bbox = textTemp->getBoundingBox();
//    float fontSize = textTemp->getFontSize();
//    float newFontSize = fontSize;
//
//    if((buttonSize.width* _scaleTextSize)-30< text_bbox.size.width)
//    {
//        newFontSize = fontSize * ((buttonSize.width * _scaleTextSize - 30)/text_bbox.size.width);
//        textTemp->setFontSize(newFontSize);
//    }
//    //CC_SAFE_DELETE(textTemp);
//    return newFontSize;
//
//}
//
//void AISpeakingScene::onScenePause() {
//
//    if(_videoPlayer) {
//        _videoPlayer->pause();
//    }
//}
//
//void AISpeakingScene::onSceneResume() {
//
//    if(_videoPlayer) {
//        _videoPlayer->resume();
//    }
//}
//
//void AISpeakingScene::pushEventAIspeakingToCleaverTap() {
//    // push info to CleverTap
//    int currentPlayID = _currentPlayingVideoID;
//    if(currentPlayID<0)
//        currentPlayID = 0;
//    if(currentPlayID> _videoInfoCacheList.size() - 1)
//        currentPlayID = _videoInfoCacheList.size() - 1;
//
//    int learnPercent = currentPlayID * 100/_videoInfoCacheList.size();
//    auto videoInfo = _videoInfoCacheList.at(currentPlayID);
//    std::string text = "";
//    std::string mode_text = "MIMIC";
//    if(_turnInfoCacheList.count(videoInfo->_turnID) > 0) {
//        auto turn = _turnInfoCacheList.at(videoInfo->_turnID);
//        text = turn->_text;
//    }
//    if(getLessonInfo().is_learned )
//    {
//        if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//            mode_text = "Listen_and_repeat";
//        else
//            mode_text = "Let_talk";
//    }
//    else
//    {
//        if(getAISpeakingMode() == ms::AISpeakingMode::MIMIC)
//            mode_text = "MIMIC";
//        else
//            mode_text = "TALKING";
//    }
//    mj::model::AISpeakingCategoryInfo aISpeakingCategoryInfo = StoryDataModel::getInstance()->getAiSpeakingTopicById(getLessonInfo().category_id);
//    if(!getLessonInfo().is_learned)
//    {
//        CleverTapManager::GetInstance()->pushEvent("talk_play_new_lesson", {
//                            {"lesson_topic",cocos2d::Value(aISpeakingCategoryInfo.topic_name)},
//                            {"lesson_id",cocos2d::Value(getLessonInfo().lesson_id)},
//                            {"lesson_number",cocos2d::Value(getLessonInfo().lesson_order)},
//                            {"start_recording",cocos2d::Value(_timeToClickRecoding)},
//                            {"completed",cocos2d::Value(learnPercent)},
//                            {"time_spent",cocos2d::Value(MJDEFAULT->getIntegerForKey(key_time_talking_call_end, 0))},
//                            {"stop_at_question",cocos2d::Value(text)},
//                            {"stop_at_mode",cocos2d::Value(mode_text)},
//                    });
//    }
//    else
//    {
//
//        CleverTapManager::GetInstance()->pushEvent("talk_play_old_lesson", {
//                            {"lesson_topic",cocos2d::Value(aISpeakingCategoryInfo.topic_name)},
//                            {"lesson_id",cocos2d::Value(getLessonInfo().lesson_id)},
//                            {"lesson_number",cocos2d::Value(getLessonInfo().lesson_order)},
//                            {"start_recording",cocos2d::Value(_timeToClickRecoding)},
//                            {"completed",cocos2d::Value(learnPercent)},
//                            {"time_spent",cocos2d::Value(MJDEFAULT->getIntegerForKey(key_time_talking_call_end, 0))},
//                            {"stop_at_question",cocos2d::Value(text)},
//                            {"stop_at_mode",cocos2d::Value(mode_text)},
//                            {"skip",cocos2d::Value(_skipMIMIC)},
//                            {"click_answer",cocos2d::Value(_isClickAnswer)},
//                    });
//    }
//
//}
//std::vector<mj::model::TalkingCommonInfo> AISpeakingScene::getRamdomCommonInfo(std::string actorName) {
//    mj::model::TalkingCommonInfo talkingCommonInfosReturn;
//    talkingCommonInfosReturn.id = -1;
//
//    auto talkingCommonInfos = StoryDataModel::getInstance()->getAiSpeakingCommonInfosByActor(actorName);
//    vector<mj::model::TalkingCommonInfo> talkingCommonInfosExits;
//
//    if(talkingCommonInfos.empty()){
//
//        return talkingCommonInfosExits;
//    }
//    std::string common_actor_ids_str = MJDEFAULT->getStringForKey("common_actor_id","");
//    std::vector<int> commonActorIds= mj::model::ConvertStringToArrayInterger(common_actor_ids_str);
//
//
//    for(int id: commonActorIds){
//        for(auto info:talkingCommonInfos) {
//            if(info.id == id) {
//
//                std::string dir = mj::helper::getFileNameWithoutExtentionFromURL(info.zipPath);
//                auto rootVideoDataStr = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING + dir +"/root.json");
//                if (rootVideoDataStr.empty()) {
//                    break;;
//                }
//                rapidjson::Document rootVideoData;
//                rootVideoData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(rootVideoDataStr.c_str());
//                mj::api::MJAPIDecodeObjectJsonInfo::readAISpeakingCommonInfoActorVideoURL(rootVideoData.GetArray(),"video",info,dir+"/");
//                talkingCommonInfosExits.push_back(info);
//                break;
//            }
//        }
//    }
//    if(talkingCommonInfosExits.size()<=0)
//        return talkingCommonInfosExits;
//    /*
//     std::string actor_name = "";
//     std::string version_name = "";
//     std::string zipPath = "";
//     std::string intro = "";
//     std::string intro_mimic = "";
//     std::string intro_talking = "";
//     std::string your_turn = "";
//     std::string cheer_up = "";
//     std::string listening = "";
//     std::string outro_mimic = "";
//     std::string outro = "";
//     */
////    int id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.id =talkingCommonInfos.at(id).id;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.actor_name =talkingCommonInfos.at(id).actor_name;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.version_name =talkingCommonInfos.at(id).version_name;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.zipPath =talkingCommonInfos.at(id).zipPath;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.intro =talkingCommonInfos.at(id).intro;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.intro_mimic =talkingCommonInfos.at(id).intro_mimic;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.intro_talking =talkingCommonInfos.at(id).intro_talking;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.your_turn =talkingCommonInfos.at(id).your_turn;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.cheer_up =talkingCommonInfos.at(id).cheer_up;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.listening =talkingCommonInfos.at(id).listening;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.outro_mimic =talkingCommonInfos.at(id).outro_mimic;
////
////    id = random(0,(int)talkingCommonInfos.size()-1) ;
////    talkingCommonInfosReturn.outro =talkingCommonInfos.at(id).outro;
//
//    return  talkingCommonInfosExits;
//
//}
//
//
//
//#endif
//
//
