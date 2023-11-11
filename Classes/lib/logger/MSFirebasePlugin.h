//
//  MSFirebasePlugin.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/14/17.
//

#ifndef MSFirebasePlugin_hpp
#define MSFirebasePlugin_hpp
#include "firebase/analytics.h"
#include "firebase/dynamic_links.h"
#include "firebase/remote_config.h"
#include "firebase/installations.h"
#include "firebase/future.h"
#include "firebase/auth.h"
#include "cocos2d.h"
#include "MSAnalyticsPlugin.h"
#include "MSSidebar.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"
#include "GettingUserInfoManager.h"
#include "RatingManager.h"
#include "MJ_PageLoadResource.h"
#include "APChooseProfileView.h"
#include "MSHome.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSCollectionStoryItem.h"
#include "APChooseProfileView.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "platform/CCPlatformConfig.h"
#include "MMQRScan.h"

#define DEF_CHECK_2_STRINGS_EQUAL(__STR1__, __STR2__)   mj::helper::str_isEqual(__STR1__, __STR2__)
#define DEF_TRANSITION_DURATION                         0.5f // thời gian chuyển scene

USING_NS_CC;

namespace ms {

namespace remoteConfig {
    class Firebase {
        public:
            static Firebase & getInstance() {
                static Firebase sharedFirebase;
                return sharedFirebase;
            };

        void fetDataRemoteConfigABTesting(std::function<void(std::string)> callbackCallABTesting, std::string type = "");
    };
}

namespace analytics {
        class Firebase : public ms::AnalyticsPlugin {
        public:
            static std::string QR_DEEP_LINK;
            static std::string analyticsId;
            static Firebase & getInstance() {
                static Firebase sharedFirebase;
                return sharedFirebase;
            };
            
            void setIdentifier(std::string identifier);
            void setUserProperties(std::map<std::string, cocos2d::Value> props);
            void logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> parameters, bool doPushClevertap = false);
            void getAnalyticsId();
        protected:
            explicit Firebase();
            
        private:
            Firebase(Firebase const &);
            void operator = (Firebase const &);
        };
    
        class DynamicLinksListener : public firebase::dynamic_links::Listener {
        public:
#pragma mark - Static util methods
            static void reloadIntroScene(bool logged_in = false) {
                if (logged_in == false) {
                    MJDEFAULT->setBoolForKey(key_onboarding_license, false);
                    MJDEFAULT->setBoolForKey(key_logged_in, false);
                }

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
                {
                    if (auto runScene = Director::getInstance()->getRunningScene())
                    {
                        if (runScene->getName().compare(ms_scene_name_intro) == 0)
                        {
                            ((MSIntro *)runScene)->reloadIntro();
                        }
                        else
                        {
                            auto scene = MSIntro::createScene();
                            Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
                        }
                    }
                });
            }

            static bool isMSIntroScene(cocos2d::Scene* scene)
            {
                return scene && scene->getName().compare(ms_scene_name_intro) == 0;
            }

            static bool isMSHomeScene(cocos2d::Scene* scene)
            {
                return scene && scene->getName().compare(ms_scene_name_home) == 0;
            }

            static bool isUserLoggedIn()
            {
                // điều kiện check login này giống với điều kiện ở trong MSIntroLoad
                return (MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false)) && !ap::Database::getInstance().getAccessToken().empty();
            }

#pragma mark - Dynamic Links: Handling TASK functions
            static void handleTask_logout()
            {
                bool is_login = DynamicLinksListener::isUserLoggedIn();

                if (is_login)
                {
                    auto runningScene = Director::getInstance()->getRunningScene();
                    if (runningScene)
                    {
                        if (auto scan_screen = (MMQRScan *)runningScene->getChildByName(mmqrscan_name)) {scan_screen->onClose(nullptr);} // MMQRScan layout
                    }

                    // code dùng cho sign-out được copy từ hàm StoryParentAccountProfile_Landscape::onClickSignOut()
                    // các event event_logout_successfully và event_logout_failed được dispatch trong hàm ap::Auth::signOut()

                    Director::getInstance()->getEventDispatcher()->addCustomEventListener("event_logout_successfully", [](EventCustom * event)
                    {
                        ms::EventLogger::getInstance().logEvent("event_signout_done", {});
                        DynamicLinksListener::reloadIntroScene();
                    });

                    Director::getInstance()->getEventDispatcher()->addCustomEventListener("event_logout_failed", [](EventCustom * event)
                    {
                        ms::EventLogger::getInstance().logEvent("event_signout_fail", {});
                        cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
                    });

                    ap::Auth::signOut();

                    GettingUserInfoManager::GetInstance()->deleteData();
                    RatingManager::GetInstance()->deleteData();

                    mj::PageLoadResource::getInstance().deleteCacheAllStory();
                    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
                    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
                    FileUtils::getInstance()->purgeCachedEntries();
                }
            }

            static void handleTask_newprofile(const std::string& raw_url, bool createNewProfileShowed = false)
            {
                bool is_login = DynamicLinksListener::isUserLoggedIn();

                if (is_login)
                {
                    auto runningScene = Director::getInstance()->getRunningScene();

                    if (!runningScene || DynamicLinksListener::isMSIntroScene(runningScene))
                    {
                        ms::analytics::Firebase::QR_DEEP_LINK = raw_url;

                        if (runningScene)
                        {
                            if (auto choose_profile = (APChooseProfileView *)runningScene->getChildByName("choose_profile_view"))
                            {
                                if (createNewProfileShowed)
                                {
                                    choose_profile->onNewProfile(nullptr);
                                    ms::analytics::Firebase::QR_DEEP_LINK = "newprofile";
                                }
                                else
                                {
                                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                                }
                            }
                        }
                    }
                    else
                    {
                        if (auto scan_screen = (MMQRScan *)runningScene->getChildByName(mmqrscan_name)) {scan_screen->onClose(nullptr);} // MMQRScan layout

                        ms::analytics::Firebase::QR_DEEP_LINK = raw_url;
                        auto home = MSHome::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(DEF_TRANSITION_DURATION, home));
                    }
                }
            }

            // TODO: General handling
            static void handleTask_general(const std::string& raw_url)
            {
                bool is_login = DynamicLinksListener::isUserLoggedIn();

                if (is_login)
                {
                    auto runningScene = Director::getInstance()->getRunningScene();

                    if (!runningScene || DynamicLinksListener::isMSIntroScene(runningScene))
                    {
                        ms::analytics::Firebase::QR_DEEP_LINK = raw_url;
                    }
                    else
                    {
                        if (auto scan_screen = (MMQRScan *)runningScene->getChildByName(mmqrscan_name)) {scan_screen->onClose(nullptr);} // MMQRScan layout

                        ms::analytics::Firebase::QR_DEEP_LINK = raw_url;
                        auto home = MSHome::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(DEF_TRANSITION_DURATION, home));
                    }
                }else {
                    ms::analytics::Firebase::QR_DEEP_LINK = raw_url;
                }
            }

            static void handleTask_stories(const std::string& raw_url, std::map<std::string, std::string> query_parse)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_lessons(const std::string& raw_url)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_audiobooks(const std::string& raw_url, std::map<std::string, std::string> query_parse)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_qrcode(const std::string& raw_url)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_subscribe(const std::string& raw_url)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_account(const std::string& raw_url)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_settings(const std::string& raw_url, std::map<std::string, std::string> query_parse)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

            static void handleTask_moreapps(const std::string& raw_url)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

#pragma mark - Dynamic Links: Handling PART functions
            static void handlePart_qrstory(const std::string& raw_url, const std::string& query)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }
            
            static void handlePart_qraudiobooks(const std::string& raw_url, const std::string& query)
            {
                DynamicLinksListener::handleTask_general(raw_url);
            }

#pragma mark - Dynamic Links
            // Called on the client when a dynamic link arrives.
            void OnDynamicLinkReceived(const firebase::dynamic_links::DynamicLink* dynamic_link) override
            {
                std::map<std::string, std::string> url_parse = mj::helper::url_parse(dynamic_link->url);

                CCLOG("Received link: URL      = %s", url_parse[URL_RAW_URL].c_str());
                CCLOG("Received link: protocol = %s", url_parse[URL_PROTOCOL].c_str());
                CCLOG("Received link: domain   = %s", url_parse[URL_DOMAIN].c_str());
                CCLOG("Received link: port     = %s", url_parse[URL_PORT].c_str());
                CCLOG("Received link: path     = %s", url_parse[URL_ROOT_PART].c_str());
                CCLOG("Received link: query    = %s", url_parse[URL_QUERY].c_str());

                // check domain
                if ((url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_MS) != std::string::npos) || (url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_VM) != std::string::npos))
                {
                    handleDynamicLinks(url_parse);
                }
            }
            static void OnDynamicLinkReceivedAndroid(const std::string& url)
            {
                if(url.empty()) {
                    return;
                }
                
                std::map<std::string, std::string> url_parse = mj::helper::url_parse(url);

                CCLOG("Received link: URL      = %s", url_parse[URL_RAW_URL].c_str());
                CCLOG("Received link: protocol = %s", url_parse[URL_PROTOCOL].c_str());
                CCLOG("Received link: domain   = %s", url_parse[URL_DOMAIN].c_str());
                CCLOG("Received link: port     = %s", url_parse[URL_PORT].c_str());
                CCLOG("Received link: path     = %s", url_parse[URL_ROOT_PART].c_str());
                CCLOG("Received link: query    = %s", url_parse[URL_QUERY].c_str());
                CCLOG("Received link: query    = %s", url_parse[URL_QUERY].c_str());

                url_parse.insert({URL_ORIGIN_URL,url});
                
                // check domain
                if ((url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_MS) != std::string::npos) || (url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_VM) != std::string::npos))
                {
                    handleDynamicLinks(url_parse);
                }
                else
                {
                    bool isValidURL = (url.find("http") != std::string::npos) || (url.find("https") != std::string::npos);
                    
                    if(isValidURL)
                    {
                        cocos2d::Application::getInstance()->openURL(url);
                    }
                }
            }


            static void handleDynamicLinks(std::map<std::string, std::string>& url_parse)
            {
                const std::string& part = url_parse[URL_ROOT_PART];

                // MARK: URL PART is "app"
                if (DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_APP))
                {
                    const std::map<std::string, std::string>& query_parse = mj::helper::url_query_parse(url_parse[URL_QUERY]);
                    const std::string& taskValue = query_parse.find(DEF_DEEPLINK_KEYNAME_TASK)->second;
                    const std::string& raw_url = url_parse[URL_RAW_URL];

                    // TODO: -- task: logout
                    if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "logout"))
                    {
                        DynamicLinksListener::handleTask_logout();
                    }
                        // TODO: -- task: login
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "login"))
                    {
                        bool is_login = DynamicLinksListener::isUserLoggedIn();

                        if (!is_login)
                        {
                            DynamicLinksListener::reloadIntroScene();
                        }
                    }
                        // TODO: -- task: newprofile
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "newprofile"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_newprofile(raw_url, true);
                        });
                    }
                        // TODO: -- task: chooseprofile
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "chooseprofile"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_newprofile(raw_url, false);
                        });
                    }
                        // TODO: -- task: stories
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "stories"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_stories(raw_url, query_parse);

                        });
                    }
                        // TODO: -- task: lessons
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "lessons"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_lessons(raw_url);
                        });
                    }
                        // TODO: -- task: audiobooks
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "audiobooks"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_audiobooks(raw_url, query_parse);
                        });
                    }
                        // TODO: -- task: qrcode
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "qrcode"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_qrcode(raw_url);
                        });
                    }
                        // TODO: -- task: subscribe
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "subscribe"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_subscribe(raw_url);
                        });
                    }
                        // TODO: -- task: settings
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "settings"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_settings(raw_url, query_parse);
                        });
                    }
                        // TODO: -- task: moreapps
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "moreapps"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_moreapps(raw_url);
                        });
                    }
                        // TODO: -- task: account
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "account"))
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            DynamicLinksListener::handleTask_account(raw_url);
                        });
                    }
                        // TODO: -- task: deviceappsettings
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "deviceappsettings"))
                    {
                        mj::helper::openDeviceAppSettings();
                    }
                        // TODO: -- task: worksheet
                    else if (DEF_CHECK_2_STRINGS_EQUAL(taskValue, "worksheet"))
                    {
                        DynamicLinksListener::handleTask_general(raw_url);
                    }
                        // TODO: -- task: ...
                    else
                    {
                        // nothing here
                    }
                }
                    // MARK: URL PART is "qrstory"
                else if (DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_QRSTORY))
                {
                    const std::string& raw_url = url_parse[URL_RAW_URL];
                    const std::string& query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        DynamicLinksListener::handlePart_qrstory(raw_url, query);
                    });
                }
                    // MARK: URL PART is "qraubook"
                else if (DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_QRAUBOOK))
                {
                    const std::string& raw_url = url_parse[URL_RAW_URL];
                    const std::string& query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        DynamicLinksListener::handlePart_qraudiobooks(raw_url, query);
                    });
                }
                    // MARK: URL PART is "qrbook"
                else if (DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_QRBOOK))
                {
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = url_parse[URL_RAW_URL];

                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        MJDEFAULT->setBoolForKey(key_app_should_show_qr, true);
                        if (MSSidebar::defaultSidebar)
                            MSSidebar::defaultSidebar->enableQR();
                        if (!MMQRScan::showAlert()) {
                            CCLOG("dispatch event qr");
                            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.dynamic.link.received");
                        }

                    });

                }
                
                else if (DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_QRPHONICS) || DEF_CHECK_2_STRINGS_EQUAL(part, DEF_DEEPLINK_PART_QRREADING_COMPREHENSION))
                {
                    const std::string& raw_url = url_parse[URL_RAW_URL];
                    const std::string& query = url_parse[URL_QUERY];
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                    {
                        DynamicLinksListener::handlePart_qrstory(raw_url, query);
                    });
                }
                else
                {
                    if(url_parse.count(URL_ORIGIN_URL) > 0) {
                        
                        std::string url =url_parse.at(URL_ORIGIN_URL);
                        
                        if(!url.empty()) {
                            
                            cocos2d::Application::getInstance()->openURL(url);
                        }
                    }
                }
            }
      
        };
}}

#endif /* MSFirebasePlugin_hpp */
