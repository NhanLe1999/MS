//
//  MMQRScan.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/26/18.
//

#include "MMQRScan.h"
#include "MJHelper.h"
#include "StoryDataModel.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"

#include "AudioEngine.h"

#include "StoryAlert.h"
#include "MSHome.h"
#include "MSTabLessons.h"
#include "StoryLanguageManager.h"
#include "NativeBridge.h"

#include "GAFWrapper.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSFirebasePlugin.h"
#include "StoryAudioPlayer.h"
#include "MSLessonManager.h"

USING_NS_CC;
INITIALIZE_READER(MMQRScan);

#pragma mark - Clever Tap
void MMQRScan::pushCleverTapEvent_scanQR(const std::string& str_is_success) {
    try {
        CleverTapManager::GetInstance()->pushEvent("worksheet_scan_qr", {
            {"is_success",cocos2d::Value(str_is_success)},
        });
    } catch (const std::exception&) {
    }
}

// giá trị 0 (false): cách làm mới, xử lý trực tiếp link lấy từ ảnh QR
// giá trị 1 (true): cách làm cũ, lấy link từ firebase xuống
#define QR_GET_LINK_FROM_FIREBASE   0

std::once_flag qr_cap_reader_flags;

bool MMQRScan::Is_parsing_deeplink = false;

//trả về true nếu hiện alert, false nếu không hiện và sẽ dispatch event nhận qr code
bool MMQRScan::showAlert()
{
    auto current_scene = Director::getInstance()->getRunningScene();

    if(!current_scene) {
        
        return true;
    }
    
    if (current_scene->getChildByName("home_view"))
    {
        //là màn hình home
        auto player = utils::findChild(current_scene, "audiobook_player");
        
        if (player)
        {
            //là màn hình play audio
            StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("qr.alert"), "OK", "");
            return true;
        }

        auto content = utils::findChild(current_scene, "tab_content");
        
        if (content->getChildByName("content") && dynamic_cast<MSTabLessons *>(content->getChildByName("content")))
        {
            StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("qr.alert"), "OK", "");
            return true;
        }
        
        return false;
    }
    else if (current_scene->getChildByName("loadview")) {
        //là màn hình loading
        //trả về true
        return false;
    }
    else
    {
        //là màn hình truyện
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("qr.alert"), "OK", "");
        
        return true;
    }
}

MMQRScan * MMQRScan::createView()
{
    std::call_once(qr_cap_reader_flags, [] {
        REGISTER_CSB_READER(MMQRScan);
    });
    
    auto view = reinterpret_cast<MMQRScan *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/qr_scan.csb"));
    
    if (view)
    {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return view;
}

void MMQRScan::didLoadFromCSB()
{
    ui::Layout::doLayout();
    
    auto pattern = Director::getInstance()->getTextureCache()->addImage("mjstory/new_ui/qr_texture.png");
    
    const Texture2D::TexParams tex_params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};

    auto visible_size = Director::getInstance()->getVisibleSize();

    this->setTouchEnabled(true);

    auto background = Sprite::createWithTexture(pattern, Rect(0, 0, visible_size.width, visible_size.height));
    background->getTexture()->setTexParameters(tex_params);
    background->setAnchorPoint(Point::ANCHOR_MIDDLE);
    background->setPosition(this->getContentSize() / 2);
    this->addChild(background, -1);

    this->getChildByName("overlay")->setVisible(false);

    auto video_view = utils::findChild(this, "video_view");
    auto gaf = GAFWrapper::createGAF("gaf/qr_scan_truyen/qr_scan_truyen.gaf");
    gaf->setPosition(video_view->getContentSize() / 2);
    gaf->play(true);
    gaf->setScale(0.8);
    video_view->addChild(gaf);
    video_view->setPosition(Vec2(750,-350));
    auto instruction = utils::findChild<ui::Text *>(this, "lb.qr.scan.guide.1");
    instruction->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.instruction"));

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_scan_closed, [=](EventCustom *) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
            this->onClose(nullptr);
        });
    }), this);

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("key.app.enter.background", [=](EventCustom *) {
//        this->onClose(nullptr);
//    }), this);
//#endif

    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "lb.qr.scan.guide.1"),
            utils::findChild<ui::Text *>(this, "lb.qr.scan.guide.4"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }
    
    utils::findChild<ui::Text *>(this, "lb.qr.scan.guide.4")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("deactive.wait"));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(auto closeButton = utils::findChild<ui::Button*>(this, "button_close"))
    {
        closeButton->setVisible(false);
    }

    if(auto panelScan = utils::findChild(this, "panel_scan"))
    {
        auto waittingView = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        waittingView->setPosition(panelScan->getContentSize()/2);
        waittingView->runAction(RepeatForever::create(RotateBy::create(0.5f, 360)));
        panelScan->addChild(waittingView);
    }
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_enable_close_button, [=](EventCustom * i_ev)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
        {
            if(auto closeButton = utils::findChild<ui::Button*>(this, "button_close"))
            {
                CCLOG("FullAc|setEnabled true");
                closeButton->setVisible(true);
            }
        });
    }), this);
    
    startCamera(cocos2d::Rect::ZERO);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && QR_GET_LINK_FROM_FIREBASE)
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_deeplink_handled, [=](EventCustom * i_ev) {

        std::string* dataStrPointer = reinterpret_cast<std::string*>(i_ev->getUserData());

        std::string dataStr = (*dataStrPointer);
        
        if (CONFIG_MANAGER->isVietnameseStories()) {
            auto url_parse = mj::helper::url_parse(dataStr);
            std::string domain = url_parse[URL_DOMAIN];
            if(dataStr.empty() || !NativeBridge::networkAvaiable() || (dataStr == "error"))
            {
                std::string reason = dataStr.empty() ? "Data NULL" : dataStr == "error" ? "Data Error" : "No Internet";
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
                    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
                    alert->setNetworkDownContent(false, true);
                    alert->show();

                    this->removeFromParent();

                });

                return;
            }
            
            // TODO: for Android MS
            if (domain.find(DEF_DOMAIN_CORRECT_MS) != std::string::npos)
            {
                std::string part = url_parse[URL_ROOT_PART];
                
                if (part.compare("qrstory") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        this->handleQRDeeplinkStory(query);
                    });
                }
                else if (part.compare("qrphonics") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        MMQRScan::handleQRDeeplinkPhonics(query);
                    });
                }
                else
                {
                    // code cũ, dễ bị crash lắm, cần sửa lại
                    auto story_id = mj::helper::parseDynamicLinkQR(dataStr);
                    auto view = MSStoryAudioPlayer::createView(story_id);
                    Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
                    Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                }
            }
            // TODO: for Android VM
            else if (domain.find(DEF_DOMAIN_CORRECT_VM) != std::string::npos && CONFIG_MANAGER->isVietnameseStories())
            {
                std::string part = url_parse[URL_ROOT_PART];

                if (part.compare("qrphonics") == 0) {
                    auto query = url_parse[URL_QUERY];

                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        MMQRScan::handleQRDeeplinkPhonics(query);
                    });
                }
            }
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                this->removeFromParent();
            });
        }
        else
        {
            onDeeplinkHandled(dataStr);
        }
    }), this);
#endif
}

void MMQRScan::onEnter()
{
    ui::Layout::onEnter();
    ms::analytics::Firebase::QR_DEEP_LINK = "";
}

void MMQRScan::onEnterTransitionDidFinish()
{
    ui::Layout::onEnterTransitionDidFinish();

    auto screen_size = Director::getInstance()->getVisibleSize();
    auto preview = utils::findChild(this, "preview"); //preview là khung scan trong màn hình
    auto bb_screen = preview->getBoundingBox();
    auto bb_scr = ui::Helper::convertBoundingBoxToScreen(preview);
    int padding = (int) (screen_size.height * 0.03f);
    bb_screen = cocos2d::Rect(padding, padding, bb_scr.size.width - padding, bb_scr.size.height - padding * 2);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    startCamera(bb_screen);
#endif
}

void MMQRScan::onBack(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void MMQRScan::onRead(cocos2d::Ref *sender)
{
    
}

void MMQRScan::onExit()
{
    ui::Layout::onExit();
}

void MMQRScan::onClose(cocos2d::Ref * sender)
{
    this->pushCleverTapEvent_scanQR(_qr_string.empty() ? "none" : "no");
    
    stopCamera();
    this->removeFromParent();
}

void MMQRScan::onLinkClicked(cocos2d::Ref * sender)
{

}

ui::Widget::ccWidgetClickCallback MMQRScan::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MMQRScan, onBack),
        CLICK_MAP(MMQRScan, onRead),
        CLICK_MAP(MMQRScan, onClose),
        CLICK_MAP(MMQRScan, onLinkClicked),
    };

    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }

    return nullptr;
}

void MMQRScan::handleQRLink(const std::string& str)
{
    if (_qr_string.compare(str) == 0) {
        return;
    }
    
    _qr_string = str;
    
    bool is_scan_fail = true;
    std::string str_domain = "";
    
    if (CONFIG_MANAGER->isVietnameseStories() && str.find("vmonkey.page.link/") != std::string::npos) {
        str_domain = "vmonkey.page.link/";
    } else if (!CONFIG_MANAGER->isVietnameseStories() && str.find("monkeystories.page.link/") != std::string::npos) {
        str_domain = "monkeystories.page.link/";
    } else {
        return;
    }
    
    std::string str_query = str.substr(str.find(str_domain) + str_domain.length());
    
    // TODO: -- QR audibooks
    if (str_query.find("bts") != std::string::npos) {
        std::string str_id = str_query.substr(str_query.find("bts") + 3);
        
        if (mj::helper::isDigitString(str_id) && std::stoi(str_id) > 0) {
            int story_id_chung = std::stoi(str_id);
            auto story_info = StoryDataModel::getInstance()->getStoryByStoryIdAndLang(story_id_chung, LANGUAGE_EN_CODE);
            
            auto view = MSStoryAudioPlayer::createView(story_info.unique_id);
            Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
            Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
            
            is_scan_fail = false;
            this->pushCleverTapEvent_scanQR("yes");
        }
    // TODO: -- QR story
    } else if (str_query.find("story") != std::string::npos) {
        std::string str_id = str_query.substr(str_query.find("story") + 5);
        if (mj::helper::isDigitString(str_id) && std::stoi(str_id) > 0) {
            int story_id_chung = std::stoi(str_id);
            auto story_info = StoryDataModel::getInstance()->getStoryByStoryIdAndLang(story_id_chung, LANGUAGE_EN_CODE);
            
            auto runningScene = Director::getInstance()->getRunningScene();
            if (runningScene && runningScene->getName().compare(ms_scene_name_home) == 0) {
                if (auto homeView = (MSHome *)runningScene->getChildByName(MSHOME_NAME)) {
                    homeView->openStory(story_info);
                    
                    is_scan_fail = false;
                    this->pushCleverTapEvent_scanQR("yes");
                }
            }
        }
    // TODO: -- QR lesson
    } else if (str_query.find("les") != std::string::npos) {
        
        std::string str_id = str_query.substr(str_query.find("les") + 3);
        
        
        bool isReadingComprehension =str_query.find("rc") != std::string::npos;
        
        if(isReadingComprehension) {
            
            str_id = str_query.substr(str_query.find("rc") + 2);
        }
        
        if(!mj::helper::isDigitString(str_id)) {
            
            return;
        }
        
        int lessonId = -1;
        
        try {
           
            lessonId =  std::stoi(str_id) ;
            
        } catch (const std::runtime_error& e) {
           
        }
        
        if(lessonId <= 0) {
            return;
        }
           
        auto lesson_info = ms::LessonManager::getInstance().getLessonById(lessonId);
        
        auto runningScene = Director::getInstance()->getRunningScene();
    
        if (runningScene && runningScene->getName().compare(ms_scene_name_home) == 0) {
        
            if (auto homeView = (MSHome *)runningScene->getChildByName(MSHOME_NAME)) {
            
                homeView->openLesson(lesson_info.category_id, lessonId,isReadingComprehension?ms::LessonTab::STORIES_LESSON:ms::LessonTab::PHONICS_LESSON);
                
                is_scan_fail = false;
                this->pushCleverTapEvent_scanQR("yes");
            }
        }
        
    }
    
    if (is_scan_fail) {
        this->pushCleverTapEvent_scanQR("no");
    }
    
    Director::getInstance()->getRunningScene()->scheduleOnce([=](float){
        this->removeFromParent();
    }, 0.3f, "remove_scan_qr");
}

void MMQRScan::update(float dt) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->unscheduleUpdate();
    return;
#endif
    
    auto str = getQRLink();

    if (str.empty()) {
        return;
    }

    if (str == _qr_string) {
        //đã recognize được rồi
        return;
    }
    
    if (str.find(CONFIG_MANAGER->isVietnameseStories() ? "vmonkey.page.link" : "monkeystories.page.link") == std::string::npos) {
        return;
    }

    this->getChildByName("overlay")->setVisible(true);
    hideCamera();
    parseDeepLink(str);
    Is_parsing_deeplink = true;
    this->unscheduleUpdate();
    
    // MARK: - Cách làm mới, xử lý trực tiếp URL lấy từ ảnh QR
#if !QR_GET_LINK_FROM_FIREBASE
    
    this->handleQRLink(str);
    return;
    
#endif
    
    // code cũ: cách làm này yêu cầu người dùng phải có internet, app sẽ request lên firebase để lấy về một deeplink, app sẽ làm việc với deeplink đó
    // MARK: - Cách làm cũ, Get and Handle deep link
    _qr_string = str;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_deeplink_handled, [=](EventCustom *) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
            Is_parsing_deeplink = false;
            this->getChildByName("overlay")->setVisible(false);
            auto link = getDeepLink();
            CCLOG("handled link: %s", link.c_str());
            if (link == "error") {
                showCamera();
                this->scheduleUpdate();
                return;
            }
            
            auto url_parse = mj::helper::url_parse(link);
            std::string domain = url_parse[URL_DOMAIN];
            
            if (domain.find(DEF_DOMAIN_CORRECT_MS) != std::string::npos)
            {
                std::string part = url_parse[URL_ROOT_PART];
                
                if (part.compare("qrstory") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        this->handleQRDeeplinkStory(query);
                    });
                }
                else if (part.compare("qrphonics") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        MMQRScan::handleQRDeeplinkLesson(query,true);
                    });
                }
                else if (part.compare("qrrc") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        MMQRScan::handleQRDeeplinkLesson(query,false);
                    });
                }
                else
                {
                    // code cũ, dễ bị crash lắm, cần sửa lại
                    auto story_id = mj::helper::parseDynamicLinkQR(link);
                    auto view = MSStoryAudioPlayer::createView(story_id);
                    Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
                    Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                }
            }
            else if (domain.find(DEF_DOMAIN_CORRECT_VM) != std::string::npos && CONFIG_MANAGER->isVietnameseStories())
            {
                std::string part = url_parse[URL_ROOT_PART];
                
                if (part.compare("qrphonics") == 0)
                {
                    auto query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        MMQRScan::handleQRDeeplinkLesson(query,true);
                    });
                }
            }
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                this->removeFromParent();
            });
        });
    }), this);
}

void MMQRScan::handleQRDeeplinkStory(const std::string& query)
{
    auto runningScene = Director::getInstance()->getRunningScene();
    
    if (runningScene && runningScene->getName().compare(ms_scene_name_home) == 0)
    {
        if (auto homeView = (MSHome *)runningScene->getChildByName(MSHOME_NAME)) {
            std::map<std::string, std::string> query_parse = mj::helper::url_query_parse(query);
            std::string id_truyen_rieng = query_parse.find("id")->second;
            auto story_info = StoryDataModel::getInstance()->getStoryById(std::stoi(id_truyen_rieng));
            
            homeView->openStory(story_info);
        }
    }
}

void MMQRScan::handleQRDeeplinkLesson(const std::string& query,const bool isPhonicLesson)
{
    auto runningScene = Director::getInstance()->getRunningScene();
    
    if (runningScene && runningScene->getName().compare(ms_scene_name_home) == 0)
    {
        if (auto homeView = (MSHome *)runningScene->getChildByName(MSHOME_NAME)) {
            
            std::map<std::string, std::string> query_parse = mj::helper::url_query_parse(query);
           
            std::string id_str = query_parse.find("id")->second;
            
            if (mj::helper::isDigitString(id_str))
            {
                int idValue = std::stoi(id_str);
                
                if (idValue < 1) {
                    return;
                }
                
                const int PHONICS_DEFAULT_GRADE_ID = CONFIG_MANAGER->isVietnameseStories() ? 764 : -1;
                
                int gradeValue = PHONICS_DEFAULT_GRADE_ID;
                std::string grade_str = query_parse.find("grade")->second;
                
                if (mj::helper::isDigitString(grade_str) && std::stoi(grade_str) > 0)
                {
                    gradeValue = std::stoi(grade_str);
                }
                
                if (gradeValue < 1)
                {
                    return;
                }
                
                // kiểm tra xem Lesson có tồn tại hay không
                auto allLessons = ms::LessonManager::getInstance().getAllLessonsByGrade(gradeValue);
                
                for (auto elem : allLessons)
                {
                    if (elem.lesson_id == idValue)
                    {
                        homeView->openLesson(gradeValue, idValue,isPhonicLesson ? ms::LessonTab::PHONICS_LESSON :ms::LessonTab::STORIES_LESSON);
                        
                        return;
                    }
                }
            }
        }
    }
}

void MMQRScan::onDeeplinkHandled(std::string i_deepLink)
{
    if(i_deepLink.empty() || !NativeBridge::networkAvaiable() || (i_deepLink == "error"))
    {
        std::string reason = i_deepLink.empty() ? "Deep Link NULL" : i_deepLink == "error" ? "Deep Link Error" : "No Internet";
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
            auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
            alert->setNetworkDownContent(false, true);
            alert->show();

            this->removeFromParent();

        });

        return;
    }

    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        Is_parsing_deeplink = false;
        this->getChildByName("overlay")->setVisible(false);

        auto story_id = mj::helper::parseDynamicLinkQR(i_deepLink);
        auto view = MSStoryAudioPlayer::createView(story_id);
        Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
        Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);

        this->removeFromParent();
    });
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/JniHelper.h"
#include <jni.h>
#define QRSCAN_CLASS "org.cocos2dx.cpp.QRReaderHelper"

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onCameraQRCodeCallback(JNIEnv* env, jobject thiz, jobject jUrl) {
        jstring str = (jstring)jUrl;
        std::string token;
        if (!str) {
            token = std::string();
        }
        jboolean isCopy;
        const char* chars = env->GetStringUTFChars(str, &isCopy);
        if (chars) {
            token = chars;
            env->ReleaseStringUTFChars(str, chars);
        }

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([token] {
            ((MMQRScan *)cocos2d::utils::findChild(Director::getInstance()->getRunningScene(), mmqrscan_name))->handleQRLink(token);
        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_QRReaderHelper_onDeeplinkHandled(JNIEnv * env, jobject thiz, jstring i_input) {

        const char* str = env->GetStringUTFChars(i_input, NULL);

        std::string url = str;

        env->ReleaseStringUTFChars(i_input, str);

        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_deeplink_handled,&url);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_QRReaderHelper_onScanClosed(JNIEnv * env, jobject thiz) {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_scan_closed);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_QRReaderHelper_setCloseButtonEnable(JNIEnv * env, jobject thiz) {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_enable_close_button);
    }
}

void MMQRScan::startCamera(cocos2d::Rect rect) {
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "startScan", 10);
}

void MMQRScan::stopCamera() {
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "stopCamera");
}

void MMQRScan::hideCamera() {
    stopCamera();
}

void MMQRScan::showCamera() {
    startCamera(Rect::ZERO);
}

std::string MMQRScan::getReadedDeepLink() {
    return cocos2d::JniHelper::callStaticStringMethod(QRSCAN_CLASS, "getReadedDeepLink");
}

void MMQRScan::cleanReadedDeepLink(){
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "cleanReadedDeepLink");
}

std::string MMQRScan::getQRLink(){
    return cocos2d::JniHelper::callStaticStringMethod(QRSCAN_CLASS, "getQRLink");
}

void MMQRScan::cleanQRLink(){
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "cleanQRLink");
}

void MMQRScan::parseDeepLink(const std::string & qrLink){
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "parseDeepLink", qrLink);
}

std::string MMQRScan::getDeepLink() {
    return cocos2d::JniHelper::callStaticStringMethod(QRSCAN_CLASS, "getDeepLink");
}

void MMQRScan::cleanDeepLink() {
    cocos2d::JniHelper::callStaticVoidMethod(QRSCAN_CLASS, "cleanDeepLink");
}

//for android button overlay
bool MMQRScan::isScanCloseClicked() {
    return cocos2d::JniHelper::callStaticBooleanMethod(QRSCAN_CLASS, "isScanCloseClicked");
}

bool MMQRScan::isScanLinkClicked() {
    return cocos2d::JniHelper::callStaticBooleanMethod(QRSCAN_CLASS, "isScanLinkClicked");
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//for android button overlay
bool MMQRScan::isScanCloseClicked() {
    return false;
}
bool MMQRScan::isScanLinkClicked() {
    return false;
}

#else

void MMQRScan::hideCamera() {}
void MMQRScan::showCamera() {}
void MMQRScan::startCamera(cocos2d::Rect rect) {}
void MMQRScan::stopCamera() {}

std::string MMQRScan::getReadedDeepLink() {
    return "";
}
void MMQRScan::cleanReadedDeepLink(){}

std::string MMQRScan::getQRLink(){
    return "";
}
void MMQRScan::cleanQRLink(){}

void MMQRScan::parseDeepLink(const std::string & qrLink){}
std::string MMQRScan::getDeepLink() {
    return "";
}
void MMQRScan::cleanDeepLink() {}

//for android button overlay
bool MMQRScan::isScanCloseClicked() {
    return false;
}
bool MMQRScan::isScanLinkClicked() {
    return false;
}
#endif
