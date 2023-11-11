//#pragma once
//#include "cocostudiomacro2.h"
//#include "cocos2d.h"
//#include "SpineWrapper.h"
//#include "AISpeakingReviewScreen.h"
//#include "AISpeakingScene.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//class AISpeakingFakeCallScreen : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
//
//public:
//
//    enum class AISpeakingFakeCallScreenType {
//        START_CALLING_SCREEN,
//        CALLING_SCREEN,
//        END_CALLING_SCREEN
//    };
//
//    CREATE_FUNC(AISpeakingFakeCallScreen);
//
//    static AISpeakingFakeCallScreen* createView(AISpeakingFakeCallScreenType type,mj::model::AISpeakingLessonInfo lessonInfo, std::vector<AISpeakingReviewScreenTurnInfo> reviewTurnInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate);
//
//    void didLoadFromCSB(AISpeakingFakeCallScreenType type,mj::model::AISpeakingLessonInfo lessonInfo ,std::vector<AISpeakingReviewScreenTurnInfo> reviewTurnInfo, ms::delegate::CollectionAISpeakingLessonItem* delegate);
//
//    static void playMonkeyTalkingGame(mj::model::AISpeakingLessonInfo lessonInfo,std::function<void()> callback, ms::delegate::CollectionAISpeakingLessonItem* delegate,ms::AISpeakingMode mode = ms::AISpeakingMode::DEFAULT);
//
//private:
//
//    void onExit() override;
//
//    void onCallButtonClicked(Ref* sender);
//
//    void onBackButtonClicked(Ref* sender);
//    float getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput);
//    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
//private:
//	cocos2d::Size _screenSize = cocos2d::Size::ZERO;
//    CC_SYNTHESIZE(ms::delegate::CollectionAISpeakingLessonItem *, _delegate, Delegate);
//
//    std::vector<AISpeakingReviewScreenTurnInfo> _reviewTurnInfoList;
//
//    AISpeakingFakeCallScreenType _type;
//
//    mj::model::AISpeakingLessonInfo _lessonInfo;
//    Button* _callButton = nullptr;
//    int _audio_guiding_id = -1;
//
//};
//
//CREATE_READER(AISpeakingFakeCallScreen);
//
//#endif
