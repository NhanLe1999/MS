//#include "AISpeakingFakeCallScreen.h"
//#include "CameraHelper.h"
//#include "MJDefinitions.h"
//#include "MJHelper.h"
//#include "MsHelperGame.h"
//#include "MSActivityWrapper.h"
//#include "GAFWrapper.h"
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//INITIALIZE_READER(AISpeakingFakeCallScreen);
//
//using namespace cocos2d;
//
//std::once_flag ai_speaking_fake_call_screen;
//
//AISpeakingFakeCallScreen* AISpeakingFakeCallScreen::createView(AISpeakingFakeCallScreenType type,mj::model::AISpeakingLessonInfo lessonInfo,std::vector<AISpeakingReviewScreenTurnInfo> reviewTurnInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate) {
//
//	std::call_once(ai_speaking_fake_call_screen, [] {
//
//		REGISTER_CSB_READER(AISpeakingFakeCallScreen);
//
//	});
//
//	auto view = reinterpret_cast<AISpeakingFakeCallScreen*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingFakeCallScreen.csb"));
//	if (view) {
//		view->didLoadFromCSB(type,lessonInfo,reviewTurnInfo, delegate);
//		return view;
//	}
//	return nullptr;
//}
//
//void AISpeakingFakeCallScreen::didLoadFromCSB(AISpeakingFakeCallScreenType type,mj::model::AISpeakingLessonInfo lessonInfo,std::vector<AISpeakingReviewScreenTurnInfo> reviewTurnInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate) {
//    this->setDelegate(delegate);
//    _type   =  type;
//
//    _lessonInfo = lessonInfo;
//
//    _reviewTurnInfoList = reviewTurnInfo;
//
//	_screenSize = Director::getInstance()->getVisibleSize();
//
//    cocos2d::Node* rootLayout = nullptr;
//
//    MK_SAFELY_VISIBLE(this,"back_button" , type == AISpeakingFakeCallScreenType::START_CALLING_SCREEN);
//    rootLayout = cocos2d::utils::findChild(this, "start_layout");
//
//    _callButton = cocos2d::utils::findChild<Button*>(rootLayout, "call_button");
//
//    _callButton->setVisible(type == AISpeakingFakeCallScreenType::START_CALLING_SCREEN);
//    if(type == AISpeakingFakeCallScreenType::START_CALLING_SCREEN || type == AISpeakingFakeCallScreenType::CALLING_SCREEN) {
//
//        if(type == AISpeakingFakeCallScreenType::START_CALLING_SCREEN) {
//
//            auto currentTime = mj::helper::getTime();
//
//            MJDEFAULT->setIntegerForKey("calling_time_count",currentTime);
//
//            CCLOG("AISpeaking: playUserStateRecordAudio currentTime %lld",mj::helper::getTime());
//
//            auto configJsonData = FileUtils::getInstance()->getStringFromFile("config.json");
//
//            if (!configJsonData.empty()) {
//
//                rapidjson::Document valueData;
//
//                valueData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(configJsonData.c_str());
//
//                if (!valueData.IsNull() && !valueData.HasParseError() && valueData.IsObject()) {
//
//                    rapidjson::StringBuffer rj_buffer;
//                    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
//                    valueData.Accept(writer);
//                    CCLOG("AI Speaking JSON: \n%s", rj_buffer.GetString());
//
//                    _lessonInfo._actorName = JSON_SAFE_EXTRACT(valueData,"actor_name", String, "");
//                }
//            }
//
//            std::string actorName =  mj::helper::toCapitalizedCase(_lessonInfo._actorName);
//
//            std::string callingText = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.call.text").c_str(),actorName.c_str());
//            if(auto text = cocos2d::utils::findChild<Text*>(rootLayout, "call_text"))
//            {
//                text->setPositionX(_callButton->getContentSize().width/2*1.08);
//            float fontSize = getFontSizeForText(_callButton->getContentSize(), text, text->getFontSize(), callingText);
//                text->setFontSize(fontSize);
//            }
//            MK_SAFELY_SET_TEXT(this, "call_text",callingText);
//        }
//
//
//        if(auto layout = cocos2d::utils::findChild(this, "end_layout")) {
//
//            layout->removeFromParent();
//        }
//
//        if(auto node = cocos2d::utils::findChild(rootLayout, "calling_text")) {
//
//            node->setVisible(type != AISpeakingFakeCallScreenType::START_CALLING_SCREEN);
//
//            std::string actorName =  mj::helper::toCapitalizedCase(_lessonInfo._actorName);
//
//            std::string callingText = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.calling.text").c_str(),actorName.c_str());
//
//            MK_SAFELY_SET_TEXT_FOR_NODE(node, callingText);
//        }
//
//        std::vector<std::string> auioPaths = {
//            CALLING_SOUND_EFFECT_PATH,
//        };
//
//        if(type == AISpeakingFakeCallScreenType::START_CALLING_SCREEN) {
//
//            auioPaths = {
//                PRESS_CALL_BUTTON_SOUND_PATH_01,
//                PRESS_CALL_BUTTON_SOUND_PATH_02,
//                PRESS_CALL_BUTTON_SOUND_PATH_03,
//            };
//
//
//            this->runAction(Sequence::create(DelayTime::create(3.0f), CallFuncN::create([this](Node * n) {
//                auto gaf = GAFWrapper::createGAF("gaf/hand_tap/hand_tap.gaf");
//                gaf->setName("gaf");
//                gaf->setPosition(Point(_callButton->getContentSize() / 2) + Point(20, -20));
//                gaf->setOpacity(0);
//                gaf->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.5), CallFuncN::create([](Node * n) {
//                    reinterpret_cast<GAFWrapper *>(n)->play(true);
//                }), DelayTime::create(1.25), FadeOut::create(0.25),DelayTime::create(3.0f), NULL)));
//                _callButton->addChild(gaf, INT_MAX - 1);
//            }),nullptr));
//        }
//        else
//        {
//            if(auto node = cocos2d::utils::findChild<ImageView*>(rootLayout, "actor")) {
//
//                auto recordSpineButton = SpineWrapper::createWithPath("spine/monkey_talking/Soundwave");
//                recordSpineButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//                recordSpineButton->setPosition(node->getPosition());
//                recordSpineButton->playSequence("animation", true);
//                recordSpineButton->setSkin("MS");
//                node->getParent()->addChild(recordSpineButton);
//
//                cocos2d::utils::findChild(rootLayout, "calling_text")->setLocalZOrder(10000);
//
//                if(!_lessonInfo._actorName.empty()) {
//
//                    std::string avPath = StringUtils::format("%savt_%s.png",AI_SEAPKING_RESOURCE_PATH,_lessonInfo._actorName.c_str());
//
//                    if(FileUtils::getInstance()->isFileExist(avPath)) {
//
//                        node->loadTexture(avPath);
//                    }
//                }
//
//                node->setLocalZOrder(10000);
//            }
//        }
//
//        _audio_guiding_id = cocos2d::experimental::AudioEngine::play2d(auioPaths.at(random(0,(int)auioPaths.size()-1)));
//
//        cocos2d::experimental::AudioEngine::setFinishCallback(_audio_guiding_id, [=](int auId, std::string auPath) {
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//
//                if(type == AISpeakingFakeCallScreenType::CALLING_SCREEN) {
//
//                    this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f),CallFunc::create([=](){
//
//                        playMonkeyTalkingGame(_lessonInfo,[=](){
//
//                            this->removeFromParent();
//
//                        }, this->getDelegate(), ms::AISpeakingMode::MIMIC);
//
//                    })));
//                }
//            });
//
//        });
//    }
//    else
//    {
//
//        long long timeCountSeconds = MJDEFAULT->getIntegerForKey("calling_time_count",-1);
//        if(timeCountSeconds != -1 ) {
//
//            timeCountSeconds =  mj::helper::getTime() - timeCountSeconds;
//            MJDEFAULT->setIntegerForKey(key_time_talking_call_end, timeCountSeconds);
//            int hours, minutes;
//            minutes = timeCountSeconds / 60;
//            hours = minutes / 60;
//            timeCountSeconds = timeCountSeconds%60;
//
//            std::string log = "";
//
//            if(hours > 0) {
//                log+= StringUtils::format("%02d:",hours);
//            }
//
//            log += StringUtils::format("%02d:%02d",minutes,(int)timeCountSeconds);
//
//            CCLOG("%s",log.c_str());
//
//            MK_SAFELY_SET_TEXT(this, "call_time", log);
//
//        }
//
//
//        rootLayout = cocos2d::utils::findChild(this, "end_layout");
//
//        if(auto layout = cocos2d::utils::findChild(this, "start_layout")) {
//
//            layout->removeFromParent();
//        }
//
//        auto id = cocos2d::experimental::AudioEngine::play2d("sounds/monkey_talking/end_call_sound_effect.mp3");
//
//        cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int auId, std::string auPath) {
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//
//                //end call next screen
//
//            });
//
//        });
//    }
//
//	if (rootLayout) {
//
//		const float k_scaleFactor = ((_screenSize.width / _screenSize.height) / (4.0f / 3.0f));
//
//		rootLayout->setScale(std::min(k_scaleFactor, 1.2f));
//
//        if(auto node = cocos2d::utils::findChild<ImageView*>(rootLayout, "actor")) {
//
//            if(!_lessonInfo._actorName.empty()) {
//
//                std::string avPath = StringUtils::format("%savt_%s.png",AI_SEAPKING_RESOURCE_PATH,_lessonInfo._actorName.c_str());
//
//                if(FileUtils::getInstance()->isFileExist(avPath)) {
//
//                    node->loadTexture(avPath);
//                }
//            }
//
//            node->setLocalZOrder(10000);
//        }
//	}
//}
//
//void AISpeakingFakeCallScreen::playMonkeyTalkingGame(mj::model::AISpeakingLessonInfo lessonInfo,std::function<void()> callback,  ms::delegate::CollectionAISpeakingLessonItem* delegate, ms::AISpeakingMode mode){
//
//    auto talkingCommonInfos = StoryDataModel::getInstance()->getAiSpeakingCommonInfosByActor(lessonInfo._actorName);
//
//    if(talkingCommonInfos.empty( )){
//
//        CCLOG("AISpeakingFakeCallScreen error return ! Empty common actors data.");
//
//        if(callback) {
//            callback();
//        }
//
//        return;
//    }
//
//    for(auto &file : talkingCommonInfos) {
//
//     std::string zipPath = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING + mj::helper::getFilenameFromURL(file.zipPath);
//
//        if(FileUtils::getInstance()->isFileExist(zipPath)) {
//            file.isDownloaded = true;
//        }
//    }
//
//    talkingCommonInfos.erase(std::remove_if(
//                                  talkingCommonInfos.begin(),
//                                  talkingCommonInfos.end(),
//        [](const mj::model::TalkingCommonInfo p) { return !p.isDownloaded; }),
//                             talkingCommonInfos.end());
//
//    if(talkingCommonInfos.empty()) {
//
//        if(callback) {
//            callback();
//        }
//
//        return;
//    }
//
//    std::vector<int> talkingCommonInfoIds;
//    for(auto talkingCommonInfo: talkingCommonInfos)
//    {
//        talkingCommonInfoIds.push_back(talkingCommonInfo.id);
//    }
//
//    std::string talkingCommonInfoIdsStr = mj::model::ConvertArrayIntergerToString(talkingCommonInfoIds);
//    MJDEFAULT->setStringForKey("common_actor_id",talkingCommonInfoIdsStr);
//
//    std::vector<std::pair<std::string, std::string>> unzip_data;
//    for(int i = 0; i < talkingCommonInfos.size(); i++){
//        auto talkingCommonInfo = talkingCommonInfos.at(i);
//        std::string zipPath = FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_ZIP_PATH_FOR_AISPEAKING + mj::helper::getFilenameFromURL(talkingCommonInfo.zipPath);
//        std::string dir = mj::helper::getFileNameWithoutExtentionFromURL(zipPath);
//        unzip_data.push_back(std::make_pair(zipPath, FileUtils::getInstance()->getWritablePath() + VIDEO_CACHE_PATH_FOR_AISPEAKING + dir + "/"));
//    }
//
//    mj::helper::unzipAsync(unzip_data,[=](bool isSuccess) {
//
//        if(isSuccess)
//        {
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
//
//                auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lessonInfo.lesson_id);
//
//                if(!activities.empty()) {
//
//                    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
//
//                    if (ms::BaseActivity* game = ms::BaseActivity::createActivity(activities.front(), images, lessonInfo, mode))
//                    {
//                        if(auto aiGame = dynamic_cast<AISpeakingScene*>(game)) {
//                            //aiGame->setLessonInfo(lessonInfo);
//                            aiGame->setDelegate(delegate);
//                        }
//
//                        game->setCloseButtonEnale(false);
//                        game->setTag(STARTING_FROM_LESSON_TAG);
//                        game->setName("game_activity");
//                        Director::getInstance()->getRunningScene()->addChild(game, INT_MAX-1);
//
//                        if(callback){
//                            callback();
//                        }
//                       // this->removeFromParent();
//                    }
//                }
//
//            });
//        }
//    });
//}
//
//
//void AISpeakingFakeCallScreen::onExit() {
//
//    cocos2d::ui::Layout::onExit();
//
//}
//
//void AISpeakingFakeCallScreen::onCallButtonClicked(Ref* sender) {
//    cocos2d::experimental::AudioEngine::stop(_audio_guiding_id);
//    this->getParent()->addChild(AISpeakingFakeCallScreen::createView(AISpeakingFakeCallScreenType::CALLING_SCREEN,_lessonInfo,_reviewTurnInfoList, this->getDelegate()));
//
//    this->removeFromParent();
//}
//
//void AISpeakingFakeCallScreen::onBackButtonClicked(Ref* sender) {
//
//    this->removeFromParent();
//
//    AUDIO_ENGINE->stopAllAudio();
//}
//
//cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingFakeCallScreen::onLocateClickCallback(const std::string& callBackName) {
//
//	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//	   CLICK_MAP(AISpeakingFakeCallScreen, onCallButtonClicked),
//        CLICK_MAP(AISpeakingFakeCallScreen, onBackButtonClicked),
//	   //CLICK_MAP(AISpeakingFakeCallScreen, onClose)
//	};
//
//	if (function_map.find(callBackName) != function_map.end()) {
//		return function_map.at(callBackName);
//	}
//
//	return nullptr;
//}
//float AISpeakingFakeCallScreen::getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput) {
//    //text->setFontSize(33); //default fontsize of text is 30
//    auto textTemp = Text::create(strInput, text->getFontName(), text->getFontSize());
//    auto text_bbox = textTemp->getBoundingBox();
//    float fontSize = textTemp->getFontSize();
//    float newFontSize = fontSize;
//    float _scaleTextSize = 0.9;
//    if((buttonSize.width* _scaleTextSize)-30 < text_bbox.size.width)
//    {
//        newFontSize = fontSize * ((buttonSize.width * _scaleTextSize - 30)/text_bbox.size.width);
//        textTemp->setFontSize(newFontSize);
//    }
//    //CC_SAFE_DELETE(textTemp);
//    return newFontSize;
//
//}
//#endif
