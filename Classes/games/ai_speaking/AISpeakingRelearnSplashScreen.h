//#pragma once
//#include "cocostudiomacro2.h"
//#include "cocos2d.h"
//#include "SpineWrapper.h"
//#include "AISpeakingReviewScreen.h"
//#include "AISpeakingScene.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//class AISpeakingRelearnSplashScreen : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
//
//public:
//
//    enum class AISpeakingRelearnSplashScreenType {
//        LISTEN_AND_REPEAT,
//        LETTALK,
//        REVIEW
//    };
//
//    CREATE_FUNC(AISpeakingRelearnSplashScreen);
//
//    static AISpeakingRelearnSplashScreen* createView(AISpeakingRelearnSplashScreenType type,mj::model::AISpeakingLessonInfo lessonInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate);
//
//    void didLoadFromCSB(AISpeakingRelearnSplashScreenType type,mj::model::AISpeakingLessonInfo lessonInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate);
//
//    CC_SYNTHESIZE(std::vector<AISpeakingReviewScreenTurnInfo>,_reviewTurnList,ReviewTurnList);
//
//private:
//
//    void onCallButtonClicked(Ref* sender);
//
//    void onClose(Ref* sender);
//
//    void playMonkeyTalkingGame();
//
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
//
//private:
//    CC_SYNTHESIZE(ms::delegate::CollectionAISpeakingLessonItem *, _delegate, Delegate);
//
//	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
//
//    AISpeakingRelearnSplashScreenType _type;
//
//    mj::model::AISpeakingLessonInfo _lessonInfo;
//
//    cocos2d::Node* _rootLayout = nullptr;
//};
//
//CREATE_READER(AISpeakingRelearnSplashScreen);
//
//#endif
