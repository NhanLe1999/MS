//#include "AISpeakingClosingPopup.h"
//#include "CameraHelper.h"
//#include "MJDefinitions.h"
//#include "MJHelper.h"
//#include "MsHelperGame.h"
//#include "MSActivityWrapper.h"
//#include "GAFWrapper.h"
//
////#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//INITIALIZE_READER(AISpeakingClosingPopup);
//
//using namespace cocos2d;
//
//std::once_flag ai_speaking_closing_popup_screen;
//
//AISpeakingClosingPopup* AISpeakingClosingPopup::createView(AISpeakingClosingPopupType type) {
//
//    std::call_once(ai_speaking_closing_popup_screen, [] {
//
//        REGISTER_CSB_READER(AISpeakingClosingPopup);
//
//    });
//
//    auto view = reinterpret_cast<AISpeakingClosingPopup*>(CSLoader::createNodeWithVisibleSize("csb/game/ai_speaking/AISpeakingClosingPopup.csb"));
//    if (view) {
//        view->didLoadFromCSB(type);
//        return view;
//    }
//    return nullptr;
//}
//
//void AISpeakingClosingPopup::didLoadFromCSB(AISpeakingClosingPopupType type) {
//
//    _screenSize = Director::getInstance()->getVisibleSize();
//    initView();
//    _type   =  type;
//
//    switch (_type)
//    {
//        case AISpeakingClosingPopup::AISpeakingClosingPopupType::FIRST_CALL: {
//            _rootLayout = cocos2d::utils::findChild(this, "first_call_layout");
//            MK_SAFELY_REMOVE(this, "relearn_layout");
//            MK_SAFELY_REMOVE(this, "relearn_last_turn_layout");
//            break;
//        }
//        case AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN:
//            _rootLayout = cocos2d::utils::findChild(this, "relearn_layout");
//            MK_SAFELY_REMOVE(this, "first_call_layout");
//            MK_SAFELY_REMOVE(this, "relearn_last_turn_layout");
//            break;
//        case AISpeakingClosingPopup::AISpeakingClosingPopupType::RELEARN_LAST_TURN:
//            _rootLayout = cocos2d::utils::findChild(this, "relearn_last_turn_layout");
//            MK_SAFELY_REMOVE(this, "first_call_layout");
//            MK_SAFELY_REMOVE(this, "relearn_layout");
//            break;
//        default:
//            break;
//    }
//
//    if(_rootLayout) {
//
//        const float k_scaleFactor = ((_screenSize.width / _screenSize.height) / (4.0f / 3.0f));
//
//        _rootLayout->setScale(std::min(k_scaleFactor, 1.3f));
//    }
//}
//
//void AISpeakingClosingPopup::onContinueButtonClicked(Ref* sender) {
//
//    if(_continueCallback) {
//        _continueCallback();
//        HSAudioEngine::resetAudioEngine();
//    }
//
//    this->removeFromParent();
//}
//
//void AISpeakingClosingPopup::initView()
//{
//    /*
//        Text* _firstcall_lb_sure_to_hang_up = nullptr, * _firstcall_lb_continue = nullptr, * _firstcall_lb_hang_up = nullptr,
//        *_relearn_lb_conversation_paused_title = nullptr, * _relearn_lb_exit = nullptr, * _relearn_lb_skip = nullptr, * _relearn_lb_conversation_paused_content = nullptr,
//        * _relearn_last_turn_lb_conversation_paused_title = nullptr, * _relearn_last_turn_lb_exit = nullptr;
//     Layout* relearn_last_turn_layout = nullptr, *first_call_layout = nullptr, *relearn_layout = nullptr;
//    */
//
//    _first_call_layout = utils::findChild<Layout*>(this, "first_call_layout");
//    _relearn_layout = utils::findChild<Layout*>(this, "relearn_layout");
//    _relearn_last_turn_layout = utils::findChild<Layout*>(this, "relearn_last_turn_layout");
//
//    _firstcall_lb_sure_to_hang_up = utils::findChild<Text*>(_first_call_layout, "lb.sure.to.hang.up");
//    if(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)
//        _firstcall_lb_sure_to_hang_up->setFontSize(23);
//
//    _firstcall_lb_continue = utils::findChild<Text*>(_first_call_layout, "lb.continue");
//    _firstcall_lb_hang_up = utils::findChild<Text*>(_first_call_layout, "lb.hang.up");
//
//    _relearn_lb_conversation_paused_title = utils::findChild<Text*>(_relearn_layout, "lb.conversation.paused.title");
//    _relearn_lb_exit = utils::findChild<Text*>(_relearn_layout, "lb.exit");
//    _relearn_lb_skip = utils::findChild<Text*>(_relearn_layout, "lb.skip");
//    _relearn_lb_conversation_paused_content = utils::findChild<Text*>(_relearn_layout, "lb.conversation.paused.content");
//
//    _relearn_last_turn_lb_conversation_paused_title = utils::findChild<Text*>(_relearn_last_turn_layout, "lb.conversation.paused.title");
//    _relearn_last_turn_lb_exit = utils::findChild<Text*>(_relearn_last_turn_layout, "lb.exit");
//
//    //set font for labels
//
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _firstcall_lb_sure_to_hang_up->setFontName("fonts/leelawdb.ttf") : _firstcall_lb_sure_to_hang_up->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _firstcall_lb_continue->setFontName("fonts/leelawdb.ttf") : _firstcall_lb_continue->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _firstcall_lb_hang_up->setFontName("fonts/leelawdb.ttf") : _firstcall_lb_hang_up->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_lb_conversation_paused_title->setFontName("fonts/leelawdb.ttf") : _relearn_lb_conversation_paused_title->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_lb_exit->setFontName("fonts/leelawdb.ttf") : _relearn_lb_exit->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_lb_skip->setFontName("fonts/leelawdb.ttf") : _relearn_lb_skip->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_lb_conversation_paused_content->setFontName("fonts/leelawdb.ttf") : _relearn_lb_conversation_paused_content->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_last_turn_lb_conversation_paused_title->setFontName("fonts/leelawdb.ttf") : _relearn_last_turn_lb_conversation_paused_title->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _relearn_last_turn_lb_exit->setFontName("fonts/leelawdb.ttf") : _relearn_last_turn_lb_exit->setFontName("fonts/Montserrat-ExtraBold.ttf");
//    if(auto background = utils::findChild<ImageView*>(_first_call_layout, "background"))
//    {
//        float fontSize = getFontSizeForText(background->getContentSize(), _firstcall_lb_sure_to_hang_up, _firstcall_lb_sure_to_hang_up->getFontSize(), LANGUAGE_MANAGER->getDisplayTextByKey("lb.sure.to.hang.up").c_str());
//        _firstcall_lb_sure_to_hang_up->setFontSize(fontSize);
//    }
//    _firstcall_lb_sure_to_hang_up->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.sure.to.hang.up").c_str());
//    _firstcall_lb_continue->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.continue").c_str());
//    _firstcall_lb_hang_up->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.hang.up").c_str());
//    _relearn_lb_conversation_paused_title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.conversation.paused.title").c_str());
//    _relearn_lb_exit->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.exit").c_str());
//    _relearn_lb_skip->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.skip").c_str());
//    _relearn_lb_conversation_paused_content->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.conversation.paused.content").c_str());
//    std::string text = LANGUAGE_MANAGER->getDisplayTextByKey("lb.conversation.paused.title").c_str();
//    _relearn_last_turn_lb_conversation_paused_title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.conversation.paused.title").c_str());
//    _relearn_last_turn_lb_exit->setString(LANGUAGE_MANAGER->getDisplayTextByKey("lb.exit").c_str());
//}
//
//void AISpeakingClosingPopup::onHangUpClicked(Ref* sender) {
//
//    if(_hangUpCallback) {
//        _hangUpCallback();
//        HSAudioEngine::resetAudioEngine();
//    }
//
//    this->removeFromParent();
//}
//
//void AISpeakingClosingPopup::onClosePopupClicked(Ref* sender) {
//
//    if(_closePopupCallback) {
//
//        _closePopupCallback();
//        HSAudioEngine::resetAudioEngine();
//    }
//
//	this->removeFromParent();
//}
//
//void AISpeakingClosingPopup::onExitButtonClicked(Ref* sender) {
//
//    if(_exitCallback) {
//        _exitCallback();
//        HSAudioEngine::resetAudioEngine();
//    }
//
//    this->removeFromParent();
//}
//
//void AISpeakingClosingPopup::onSkipButtonClicked(Ref* sender) {
//
//    if(_skipCallback) {
//        _skipCallback();
//        HSAudioEngine::resetAudioEngine();
//    }
//
//    this->removeFromParent();
//}
//
//cocos2d::ui::Widget::ccWidgetClickCallback AISpeakingClosingPopup::onLocateClickCallback(const std::string& callBackName) {
//
//	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(AISpeakingClosingPopup, onContinueButtonClicked),
//        CLICK_MAP(AISpeakingClosingPopup, onHangUpClicked),
//        CLICK_MAP(AISpeakingClosingPopup, onClosePopupClicked),
//        CLICK_MAP(AISpeakingClosingPopup, onExitButtonClicked),
//        CLICK_MAP(AISpeakingClosingPopup, onSkipButtonClicked),
//	};
//
//	if (function_map.find(callBackName) != function_map.end()) {
//		return function_map.at(callBackName);
//	}
//
//	return nullptr;
//}
//float AISpeakingClosingPopup::getFontSizeForText(Size buttonSize, Text* text, float defaultFontSize, std::string strInput) {
//    //text->setFontSize(26); //default fontsize of text is 30
//    auto textTemp = Text::create(strInput, text->getFontName(), text->getFontSize());
//    auto text_bbox = textTemp->getBoundingBox();
//    float fontSize = textTemp->getFontSize();
//    float newFontSize = fontSize;
//    float _scaleTextSize = 1;
//    if((buttonSize.width* _scaleTextSize)-30< text_bbox.size.width)
//    {
//        newFontSize = fontSize * ((buttonSize.width * _scaleTextSize - 30)/text_bbox.size.width);
//        textTemp->setFontSize(newFontSize);
//    }
//    //CC_SAFE_DELETE(textTemp);
//    return newFontSize;
//
//}
////#endif
