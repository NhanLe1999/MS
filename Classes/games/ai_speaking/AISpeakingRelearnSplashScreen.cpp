//#include "AISpeakingRelearnSplashScreen.h"
//#include "CameraHelper.h"
//#include "MJDefinitions.h"
//#include "MJHelper.h"
//#include "MsHelperGame.h"
//#include "MSActivityWrapper.h"
//#include "AISpeakingFakeCallScreen.h"
//#include "GAFWrapper.h"
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//INITIALIZE_READER(AISpeakingRelearnSplashScreen);
//
//using namespace cocos2d;
//
//std::once_flag ai_speaking_AISpeakingRelearnSplashScreen;
//
//AISpeakingRelearnSplashScreen* AISpeakingRelearnSplashScreen::createView(AISpeakingRelearnSplashScreenType type,mj::model::AISpeakingLessonInfo lessonInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate) {
//
//	std::call_once(ai_speaking_AISpeakingRelearnSplashScreen, [] {
//
//		REGISTER_CSB_READER(AISpeakingRelearnSplashScreen);
//	});
//
//	auto view = reinterpret_cast<AISpeakingRelearnSplashScreen*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingRelearnSplashScreen.csb"));
//	if (view) {
//		view->didLoadFromCSB(type,lessonInfo, delegate);
//		return view;
//	}
//	return nullptr;
//}
//
//void AISpeakingRelearnSplashScreen::didLoadFromCSB(AISpeakingRelearnSplashScreenType type,mj::model::AISpeakingLessonInfo lessonInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate) {
//
//    this->setDelegate(delegate);
//    _type   =  type;
//
//    _lessonInfo = lessonInfo;
//    if(_type == AISpeakingRelearnSplashScreenType::LISTEN_AND_REPEAT)
//        _lessonInfo.is_learned = true;
//
//    _screenSize = Director::getInstance()->getVisibleSize();
//
//    auto configJsonData = FileUtils::getInstance()->getStringFromFile("config.json");
//
//    if (!configJsonData.empty()) {
//
//        rapidjson::Document valueData;
//
//        valueData.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(configJsonData.c_str());
//
//        if (!valueData.IsNull() && !valueData.HasParseError() && valueData.IsObject()) {
//
//            rapidjson::StringBuffer rj_buffer;
//            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
//            valueData.Accept(writer);
//            CCLOG("AI Speaking JSON: \n%s", rj_buffer.GetString());
//
//            _lessonInfo._actorName = JSON_SAFE_EXTRACT(valueData,"actor_name", String, "");
//        }
//    }
//
//    std::function<void()> callback = nullptr;
//
//    std::string imagePath = AI_SEAPKING_RESOURCE_PATH;
//
//	switch (_type)
//	{
//	case AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::LISTEN_AND_REPEAT:
//        {
//            cocos2d::experimental::AudioEngine::play2d(StringUtils::format("sounds/monkey_talking/listen_and_repeat_%d.mp3",random(0,2)));
//            _rootLayout = cocos2d::utils::findChild(this, "listen_and_repeat");
//            imagePath += StringUtils::format("relearn/%s.png",_lessonInfo._actorName.c_str());
//            MK_SAFELY_REMOVE(this, "lettalk");
//            MK_SAFELY_REMOVE(this, "review");
//            callback = [=](){
//                this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f),CallFunc::create([=](){
//
//                    AISpeakingFakeCallScreen::playMonkeyTalkingGame(_lessonInfo,[=](){
//
//                        this->removeFromParent();
//
//                    }, this->getDelegate(), ms::AISpeakingMode::MIMIC);
//
//                })));
//            };
//            break;
//        }
//	case AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::LETTALK:
//        {
//            float deplayTime = 3.0f;
//            if(_lessonInfo.is_learned)
//            {
//                cocos2d::experimental::AudioEngine::play2d("sounds/monkey_talking/let_s_talk_1.mp3");
//                _rootLayout = cocos2d::utils::findChild(this, "lettalk");
//
//            }
//            else
//            {
//                MK_SAFELY_REMOVE(this, "lettalk");
//                deplayTime = 0.0f;
//            }
//            imagePath += StringUtils::format("relearn/lettalk_%s.png",_lessonInfo._actorName.c_str());
//            MK_SAFELY_REMOVE(this, "listen_and_repeat");
//            MK_SAFELY_REMOVE(this, "review");
//            callback = [=](){
//                this->runAction(Sequence::createWithTwoActions(DelayTime::create(deplayTime),CallFunc::create([=](){
//
//                    AISpeakingFakeCallScreen::playMonkeyTalkingGame(_lessonInfo,[=](){
//
//                        this->removeFromParent();
//
//                    }, this->getDelegate(), ms::AISpeakingMode::TALKING);
//
//                })));
//            };
//            break;
//        }
//
//    case AISpeakingRelearnSplashScreen::AISpeakingRelearnSplashScreenType::REVIEW:
//        {
//             cocos2d::experimental::AudioEngine::play2d("sounds/monkey_talking/end_call_sound_effect.mp3");
//            _rootLayout = cocos2d::utils::findChild(this, "review");
//            imagePath += StringUtils::format("relearn/review_%s.png",_lessonInfo._actorName.c_str());
//            MK_SAFELY_REMOVE(this, "listen_and_repeat");
//            MK_SAFELY_REMOVE(this, "lettalk");
//            callback = [=](){
//
//                this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f),CallFunc::create([=](){
//
//                    Director::getInstance()->pushScene(AISpeakingReviewScreen::createScene(_reviewTurnList, _lessonInfo));
//
//                    this->removeFromParent();
//
//                })));
//            };
//        break;
//        }
//	default:
//		break;
//	}
//
//    if(FileUtils::getInstance()->isFileExist(imagePath) && _lessonInfo.is_learned) {
//
//        if(auto image = utils::findChild<ui::ImageView*>(_rootLayout,"template")) {
//
//            image->loadTexture(imagePath);
//        }
//    }
//
//	if (_rootLayout) {
//
//		const float k_scaleFactor = ((_screenSize.width / _screenSize.height) / (4.0f / 3.0f));
//
//		_rootLayout->setScale(std::min(k_scaleFactor, 1.2f));
//	}
//
//    if(callback) {
//        callback();
//    }
//
//}
//
//void AISpeakingRelearnSplashScreen::onCallButtonClicked(Ref* sender) {
//
//    this->removeFromParent();
//}
//
//void AISpeakingRelearnSplashScreen::onClose(Ref* sender) {
//
//}
//
//cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingRelearnSplashScreen::onLocateClickCallback(const std::string& callBackName) {
//
//	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//	   CLICK_MAP(AISpeakingRelearnSplashScreen, onCallButtonClicked),
//	   //CLICK_MAP(AISpeakingRelearnSplashScreen, onClose)
//	};
//
//	if (function_map.find(callBackName) != function_map.end()) {
//		return function_map.at(callBackName);
//	}
//
//	return nullptr;
//}
//#endif
