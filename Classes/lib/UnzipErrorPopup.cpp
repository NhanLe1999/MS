//
// Created by quan on 3/2/2021.
//

#include "UnzipErrorPopup.h"

USING_NS_CC;

INITIALIZE_READER(UnzipErrorPopup);
std::once_flag unzipError_popup_flag;

UnzipErrorPopup *UnzipErrorPopup::createPopup(PopupType type, mj::model::ActInfo infoArg, cocos2d::ui::Layout* preAct) {
    /*
     * Tạo một đối tượng popup error cho kiểu type
     * * type = 0: Popup error được tạo để thông báo khi click vào một truyện đang bị lỗi
     * * type = 1: Popup error được tạo để thông báo khi học đến một activity đang bị lỗi.
     */
    /*
    * button_continue
    * caution_img
    * noti_text
    * cancel_button
    */
    std::call_once(unzipError_popup_flag, [] {
        REGISTER_CSB_READER(UnzipErrorPopup);
        });
    auto popup = static_cast<UnzipErrorPopup *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/MSUnzipError.csb"));
    if(popup) {
        popup->popupType = type;
        //if (popup->popupType == 1) {
            popup->info = infoArg;
       // }
        popup->preActivity = preAct;
        popup->didLoadFromCSB();

        return popup;
    }

    CC_SAFE_DELETE(popup);
    return nullptr;
}


void UnzipErrorPopup::didLoadFromCSB() {
    /*
     * Load scene của popup
     * Dịch popup qua các ngôn ngữ trong phần setting
     */
    std::string screen = "";
    if (popupType == PopupType::STORY)
    {
        screen = "Story";
    }
    if (popupType == PopupType::STORY_IN_LESSON)
    {
        screen = "Story In Lesson";
    }
    if (popupType == PopupType::ACTIVITY)
    {
        screen = "activity";
        MJDEFAULT->setStringForKey(key_link_url_download, info.data_zip);
        MS_LOGGER.logEvent("unzip_failed", {
            {"activity_url",cocos2d::Value(std::string(info.data_zip))},
            {"ip_address",cocos2d::Value(MJDEFAULT->getStringForKey("key_ip_adress", ""))}
         });
    }
    MJDEFAULT->setStringForKey(key_live_in_screen, screen);
    //mj::helper::pushEventCrashNetWork( info.resource_zip, "unzip_failed", "unzip");
    auto notiHolder = utils::findChild<ui::Text *> (this, "noti_text");
    if(notiHolder) {
        // Cài đặt string thông báo.
        auto notiText = createNotification(popupType);
        notiText->ignoreContentAdaptWithSize(false);
        notiText->setContentSize(notiHolder->getContentSize());
        notiText->setAnchorPoint(notiHolder->getAnchorPoint());
        notiText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
        notiText->setPosition(notiHolder->getPosition());
        notiText->setName(notiHolder->getName());
        notiHolder->getParent()->addChild(notiText);
        notiHolder->removeFromParent();
    }

    auto continueBtn = utils::findChild<ui::Button *>(this, "button_continue");
    if(continueBtn) {
        continueBtn->setTitleText("");
        // Cài đặt string cho nút "continue"
        auto btnText = cocos2d::ui::RichText::create();
        auto btnString = ui::RichElementText::create(3, Color3B::RED, 255, MSMAT_GET_TEXT("key.material.worksheet.exit"), MSMAT_FONT_FILE_PATH, 27);
        if (popupType == PopupType::STORY) {
            btnString = ui::RichElementText::create(3, Color3B::WHITE, 255, MSMAT_GET_TEXT("key.material.worksheet.exit"), MSMAT_FONT_FILE_PATH, 27);
        }
        else {
            btnString = ui::RichElementText::create(3, Color3B::WHITE, 255, MSMAT_GET_TEXT("ap_continue_button_title"), MSMAT_FONT_FILE_PATH, 27);
        }

        btnText->pushBackElement(btnString);
        ui::Layout* tmp = ui::Layout::create();
        btnText->setContentSize(continueBtn->getContentSize());
        btnText->setAnchorPoint(Vec2(0.5, 0.5));
        btnText->setPosition(continueBtn->getContentSize() / 2);
        btnText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
        tmp->addChild(btnText);
        continueBtn->addChild(tmp);
    }

    auto cancel = utils::findChild<ui::Button*>(this, "cancel_button");
    // Nếu là bài học, hiện nút "X" lên để người dùng thoát ra mà không học bài học tiếp
    if (popupType == PopupType::STORY) {
        if (cancel) {
            cancel->setVisible(false);
        }
    }
    else if(popupType == PopupType::STORY_IN_LESSON) {
        if (cancel) {
            cancel->setVisible(true);
        }
    }
}

void UnzipErrorPopup::onContinue(cocos2d::Ref *sender) {
    /*
     * Người dùng click vào nút "tiếp tục"
     * * Nếu đang đọc truyện thì bị lỗi, quay về màn hình các truyện.
     * * Nếu đang học một activity thì bị lỗi, bỏ qua activity này và chuyển qua activity kế tiếp.
     * ** Khi chuyển qua activity kế tiếp, set Activity hiện tại là học rồi.
     * ** Nếu đây là Activity cuối cùng của lesson, set cho lesson là học rồi.
     */
   // this->getParent()->removeFromParent();
    if(popupType == PopupType::STORY) { //
        this->getParent()->removeFromParent();
    } else {
        /*
        * Chuyển qua bài học tiếp theo
        * Set cho bài học hiện tại là học rồi
        * Tạo một popup cho "ActivityEnd" và phải setName cho nó là "popup_start" vì trong mấy scene sau có
        * removeChildByName("popup_start")
        */

        if (auto sceneDelegate = dynamic_cast<MSActivityWrapper*> (preActivity)) {

            sceneDelegate->setVisible(true);

            sceneDelegate->onActivityEndWithReason(ms::delegate::Activity::EndReason::FINISHED);
        }
    
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_activity_wrapper_ready);

        this->removeFromParent();
    } 
}

void UnzipErrorPopup::onClose(cocos2d::Ref* sender) {

    if (auto sceneDelegate = dynamic_cast<MSActivityWrapper*> (preActivity)) {

        sceneDelegate->setVisible(true);

        sceneDelegate->onActivityEndWithReason(ms::delegate::Activity::EndReason::INTERRUPTED);
    }

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_activity_wrapper_ready);


    this->removeFromParent();
}

cocos2d::ui::RichText* UnzipErrorPopup::createStoryNotification() {
    /*
    * Tạo thông báo cho trường hợp truyện bị lỗi
    */

   
    std::string k_textXML = StringUtils::format(MSMAT_GET_TEXT("key.unzip.error.reparing.story").c_str(), MSMAT_FONT_FILE_PATH_BOLD,info.name.c_str(),MSMAT_FONT_FILE_PATH);

    if (auto noti = cocos2d::ui::RichText::createWithXML(k_textXML)) {
        return noti;
    }
    return nullptr;
}

cocos2d::ui::RichText* UnzipErrorPopup::createLessonNotification() {

    auto lessonName = ms::LessonManager::getInstance().getLessonById(info.lesson_id).name;

    std::string k_textXML = StringUtils::format(MSMAT_GET_TEXT("key.unzip.error.reparing.lesson").c_str(), MSMAT_FONT_FILE_PATH_BOLD, lessonName.c_str(), MSMAT_FONT_FILE_PATH);

    if (auto noti = cocos2d::ui::RichText::createWithXML(k_textXML)) {
        return noti;
    }

    return nullptr;
}

cocos2d::ui::RichText* UnzipErrorPopup::createNotification(PopupType popupType) {
    if (popupType == PopupType::STORY) {
        return createStoryNotification();
    }
    else {
        return createLessonNotification();
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback UnzipErrorPopup::onLocateClickCallback(const std::string & callBackName) {
    // Map các callback
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(UnzipErrorPopup, onContinue),
        CLICK_MAP(UnzipErrorPopup, onClose)
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

    return nullptr;
}
