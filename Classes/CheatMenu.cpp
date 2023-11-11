#include "CheatMenu.h"
#include "APProfileAgeF2p.h"
#include "ProfileMiniScene.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"
#include "EndTrialLessonPopup.h"
#include "EndLessonScene.h"
#include "GettingStartedScene.h"
#include "MsSubscriptionLandscape.h"
#include "ShowNewAlert.h"
#include "StoryLanguageManager.h"
#include "MSFreeContentPage.h"

#include "MSFree2PaidPermissionPopup.h"
#include "SplashScene.h"
#include "UtilitySqlite.h"
#include "Merge1Scene.h"

#include "NameSetupScene.h"
#include "ChooseAvatarScene.h"

#include"F2PCommon.h"
#include "BirthdayPicker.h"
#include "MSMergeSuccess.h"
#include "F2PManager.h"
#include "APRegisterScreen.h"
#include "APLoginScreen.h"
#include "APLoginView.h"
#include "MJSubscriptionGetPaymentInfo.h"
using namespace ui;

INITIALIZE_READER(CheatMenu);

std::once_flag support_debug_ui_flag;

#pragma mark - Initialize

void CheatMenu::show(Node* i_parent)
{
    Button* debugButton = Button::create("profile_parent/back_icon.png");
    debugButton->setName("monkey_back");
    debugButton->setAnchorPoint(Vec2(1, 0));
    debugButton->setTouchEnabled(true);
    debugButton->addClickEventListener([=](cocos2d::Ref* sender) {

        auto cheatMenu = CheatMenu::createView();
        
        if(i_parent!=nullptr){
            i_parent->addChild(cheatMenu, INT_MAX - 1);
        } else if (auto currentScene = Director::getInstance()->getRunningScene()) {
            currentScene->addChild(cheatMenu, INT_MAX - 1);
        }
    });
    debugButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, 120));
    
    if(i_parent!=nullptr){
        i_parent->addChild(debugButton, INT_MAX - 1);
    } else if (auto currentScene = Director::getInstance()->getRunningScene()) {
        currentScene->addChild(debugButton, INT_MAX - 1);
    }
}

CheatMenu * CheatMenu::createView()
{
	std::call_once(support_debug_ui_flag, []
	{
		REGISTER_CSB_READER(CheatMenu);
	});

	auto p = static_cast<CheatMenu*>(CSLoader::createNodeWithVisibleSize("csb/CheatMenu.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool CheatMenu::didLoadFromCSB()
{
    
    
	auto item = utils::findChild<ui::Button*>(this, "item_02");

	std::vector<std::vector<std::pair<std::string, std::function<void(cocos2d::Ref*)>>>> buttonNameAndFunctionList = {
        //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"Close Menu",[=](cocos2d::Ref*) {
                    
                    this->removeFromParent();
                    
                }},
                {"Luong 1.2",[=](cocos2d::Ref*) {
                    auto scene = GettingStartedScene::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY);

                }},
                {"Luong1.1",[=](cocos2d::Ref*) {
                    auto scene = GettingStartedScene::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                }},
                {"Luong 2.1",[=](cocos2d::Ref*) {
                    auto screen = APRegisterScreen::createView(nullptr);
                    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX - 1);
                }}
            },
        //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"Luong2.2",[=](cocos2d::Ref*) {
                    auto screen = APRegisterScreen::createView(nullptr);
                    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX - 1);
                }},
                {"Luong1.1",[=](cocos2d::Ref*) {
                    auto scene = GettingStartedScene::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                }},
                {"Luong 2.3",[=](cocos2d::Ref*) {

                        F2P_MANAGER->setCanMergeProfileKey(true);
                        auto scene = GettingStartedScene::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                        MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                }},
                {"Luong 2.4",[=](cocos2d::Ref*) {
                    auto loginScreen = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::HomeLogin,ms::Authen::StartRegisterFrom::Default);
                    APPopupBase::sAnimationMove(loginScreen, APPopupBase::AnimationDirection::LEFT);
                    Director::getInstance()->getRunningScene()->addChild(loginScreen, INT_MAX - 1);
                }}
            },
	//////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"Luong 3",[=](cocos2d::Ref*) {
                    auto screen = APRegisterScreen::createView(nullptr);
                    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX - 1);
                }},
                {"Luong 4",[=](cocos2d::Ref*) {


                }},
                {"Luong 5",[=](cocos2d::Ref*) {

                }}
            },
     //////////////////////////////////////////////////////////////////////////////////////////////
            {{"CuongManHinhTuoi",[=](cocos2d::Ref*) {
                
                auto loginScreen = APProfileAgeF2p::createView();
                Director::getInstance()->getRunningScene()->addChild(loginScreen, INT_MAX - 1);
                
            }},
            {"ThangProfileMini",[=](cocos2d::Ref*) {
                
                auto scene = ProfileMiniScene::createView();
                Director::getInstance()->getRunningScene()->addChild(scene, INT_MAX - 1);
                
            }}},
        //////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////
            {{"ThangEndTrialLessonPopup",[=](cocos2d::Ref*) {
                //EndTrialLessonPopup* alert = EndTrialLessonPopup::createView();
                EndTrialLessonPopup* alert = EndTrialLessonPopup::createView();
                alert->show(Director::getInstance()->getRunningScene());
            }},
            {"ThangEndLessonScene",[=](cocos2d::Ref*) {
                auto view = EndLessonScene::createView(ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
            }},
            {"[ThangEndLessonSceneV2]",[=](cocos2d::Ref*) {
                auto view = EndLessonScene::createView(ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY);
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
            }}},
        
        //////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////
        

            {{"[New PayMent]",[=](cocos2d::Ref*) {
                auto newPayment = MSNewTabSubscribe::createTab(monkey::iap::FROM::FREETOWPAID);
                newPayment->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
                newPayment->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
                
                newPayment->setCallbackOnClose([=] () {
                    auto scene = ProfileMiniScene::createView();
                    Director::getInstance()->getRunningScene()->addChild(scene, INT_MAX - 1);
                });
                
                Director::getInstance()->getRunningScene()->addChild(newPayment, INT_MAX - 1);
            }},
            {"[show New Alert]",[=](cocos2d::Ref*) {
                
                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_7_lesson");
                auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_study_stories");
                auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
                auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("key_try_learn_now");

                ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel, true);
            }},
            {"[GettingStartedScene]",[=](cocos2d::Ref*) {
                 auto scene = GettingStartedScene::createScene();
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            }}},
        
        //////////////////////////////////////////////////////////////////////////////////////////////
    
            {{"[ShowFreeContentPage]",[=](cocos2d::Ref*) {
                MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                auto scene = MSFreeContentPage::createScene();
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            }},
            {"[CuongKuPermission]",[=](cocos2d::Ref*) {

                MSFree2PaidPermissionPopup* mSFree2PaidPermissionPopup = new MSFree2PaidPermissionPopup();
                                mSFree2PaidPermissionPopup->show();
                                mSFree2PaidPermissionPopup->onChangeView();

            }},
            {"[DatSlideShow]",[=](cocos2d::Ref*) {

                auto scene = SplashScene::createScene();
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
            }},
            {"[Cuong Creat Account Free]",[=](cocos2d::Ref*) {

                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_10_lesson_free_unlock");
                auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_study_stories");
                auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
                auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");

                ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, true);
            }}},
        //////////////////////////////////////////////////////////////////////////////////////////////
            {{"[Cuong Creat Account Free2]",[=](cocos2d::Ref*) {
                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key.content.end_trial_lesson");
                auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
                auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
                auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");
               auto alert =  ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, nullptr, true, false);
                alert->setSizeTextMessage(Size(550, 115));
            }},
            {"[Complete Lesson]",[=](cocos2d::Ref*) {
                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.v1.of.popup.notice.completed.lesson");
                auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("title.of.popup.notice.completed.lesson");
                auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.top.button.of.end.lesson.scene");
                auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.bottom.button.of.end.lesson.scene");

                ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel);
            }},
            {MJDEFAULT->getBoolForKey("debugkey_dev_environment", false) ? "[DEV][Switch-DEV]":"[LIVE][Switch-DEV]",[=](cocos2d::Ref*) {
                onReset("debugkey_dev_environment");
                
            }}
            },
        //////////////////////////////////////////////////////////////////////////////////////////////
            {{"[Switch->LIVE]",[=](cocos2d::Ref*) {
                onReset("debugkey_live_environment");
            }},
            {MJDEFAULT->getBoolForKey("debugkey_product_environment", false) ? "[PRODUCT][Switch->PRODUCT]":"[LIVE OR DEV][Switch->PRODUCT]",[=](cocos2d::Ref*) {
                
                onReset("debugkey_product_environment");
                
            }},
            {"[Alert 14 Lesson]",[=](cocos2d::Ref*) {

                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_14_lesson");
                auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_study_stories");
                auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
                auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("key_try_learn_now");

                ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel, true);
            }}
            },
            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            
            { 
                {"[FreeContentPage OnlyStory]",[=](cocos2d::Ref*) {
                     MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
                    auto scene = MSFreeContentPage::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }},
                {"[FreeContentPage RandomOneItems]",[=](cocos2d::Ref*) {
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                    auto scene = MSFreeContentPage::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }},

                {"[FreeContentPage MultiItemOneDay]",[=](cocos2d::Ref*) {
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY);
                    auto scene = MSFreeContentPage::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }}
            },
        //////////////////////////////////////////////////////////////////////////////////////////////

            {
                {"[NameSetup]",[=](cocos2d::Ref*) {
                auto view = NameSetupScene::createView();
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
			    }},
                {"[BirthdayPicker]",[=](cocos2d::Ref*) {
                        auto timePicker = BirthdayPicker::createView([=](int day, int month, int year) {
                            });
                        Director::getInstance()->getRunningScene()->addChild(timePicker, INT_MAX - 1);

                        timePicker->scrollToDefaultData(1, 1, 2019);
                }},
                {"[ChooseAvatar]",[=](cocos2d::Ref*) {
                    auto view = ChooseAvatarScene::createView();
                    Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                }}
            },
            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"[NameSetup]",[=](cocos2d::Ref*) {
                auto view = NameSetupScene::createView();
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
			    }},
                {"[BirthdayPicker]",[=](cocos2d::Ref*) {
                        auto timePicker = BirthdayPicker::createView([=](int day, int month, int year) {
                            });
                        Director::getInstance()->getRunningScene()->addChild(timePicker, INT_MAX - 1);

                        timePicker->scrollToDefaultData(1, 1, 2019);
                }},
                {"[ChooseAvatar]",[=](cocos2d::Ref*) {
                    auto view = ChooseAvatarScene::createView();
                    Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                }}
            },
        //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"[MSMergeSuccess]",[=](cocos2d::Ref*) {
                    MSMergeSuccess* mSMergeSuccess = MSMergeSuccess::createTab();
                    Director::getInstance()->getRunningScene()->addChild(mSMergeSuccess, INT_MAX - 1);
                }},
                {"[CuongKu_10itemFree]",[=](cocos2d::Ref*) {
                
                        Database::getInstance().saveIdentity(std::to_string(2302172), "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6MjMwMjE3MiwibmFtZSI6IiIsImVtYWlsIjoiIiwicGhvbmUiOiIwNDY1NDY3ODY3IiwiY291bnRyeSI6IlZOIiwicGFzc3dvcmQiOiIkMnkkMTAkRFZDSjFUVHJsd3NjVUxVdUY3REpjZTJYQU5ka2RjbElrU3dNMXlvcjBHOXJjZkw4WERvQzYiLCJtYXhfcHJvZmlsZSI6MywiaXNfdmVyaWZ5IjoxLCJtYXhfZGV2aWNlX29uX2FjdGl2ZSI6MiwibWF4X2RldmljZV90b3RhbCI6OTksInRpbWVfZXhwaXJlZCI6IjE1ODUyMTY2ODMiLCJhcHBfaWQiOjJ9.ljVFIwXkAVVyhY9PaTZ4sl4aSHrxNzn9TiOjIuWraNo");

                        ap::Profile profileFake;

                        profileFake.profile_id = 4234542;
                        mj::api::MJAPI_F2P_TrialContent::call(profileFake, nullptr);
                        MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::TEN_ITEM_FREE);
                        auto scene = MSFreeContentPage::createScene();
                        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }},
                {"[Merge1+2+Success]",[=](cocos2d::Ref*) {
                    auto scene = Merge1Scene::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }}
            },
            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            { 
                {"[popup đã mua]",[=](cocos2d::Ref*) {
                    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.f2p.lifetime.purchased.account");
                    auto textTitle = LANGUAGE_MANAGER->getDisplayTextByKey("monkey.talking.ask.permission.title.text");
                    auto textOk = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");
                    auto textCancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.key_try_learn_now");
                    auto alert = ShowNewAlert::createAlert(textTitle, textMessage, textOk, textCancel, false, nullptr, true, false);
                    alert->setSizeTextMessage(Size(550, 115));
                    alert->MoveTextMessage(textMessage, false, 10);
                }},
                {"[Cuong TEST]",[=](cocos2d::Ref*) {
                    auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("text_of_pop_study_trial_7_lesson");
                    auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("key_title_study_stories");
                    auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("key_open_lock_all_stories");
                    auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("key_try_learn_now");

                    ShowNewAlert* alert = ShowNewAlert::createAlert(texttitle, "", textok, cancel, true);
                    alert->MoveTextMessage(textMessage, true, 20);
                }},
                {"[Complete Lesson V2]",[=](cocos2d::Ref*) {
                auto textMessage = LANGUAGE_MANAGER->getDisplayTextByKey("content.v2.of.popup.notice.completed.lesson");
                auto texttitle = LANGUAGE_MANAGER->getDisplayTextByKey("title.of.popup.notice.completed.lesson");
                auto textok = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.top.button.of.end.lesson.scene");
                auto cancel = LANGUAGE_MANAGER->getDisplayTextByKey("f2p.text.of.bottom.button.of.end.lesson.scene");

                ShowNewAlert::createAlert(texttitle, textMessage, textok, cancel);
            }},
            },
            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"Turn on A B Testing",[=](cocos2d::Ref*) {
                    
                    MJDEFAULT->setBoolForKey("debugkey_turn_on_A_B_testing", true);
                    this->removeFromParent();
                    
                }},
                {"Turn off A B Testing",[=](cocos2d::Ref*) {
                    
                    MJDEFAULT->setBoolForKey("debugkey_turn_on_A_B_testing", false);
                    this->removeFromParent();
                    
                }},
                {"Firebase[dev]",[=](cocos2d::Ref*) {

                    auto user_id = ap::Database::getInstance().getUserInfo()._id;
                    if (MJDEFAULT->getBoolForKey("debugkey_dev_environment", false))
                    {
                        mm::RealtimeDatabase::getInstance().dropDataUserFreeFromFireBase(user_id);
                    }
                    onReset("debugkey_dev_environment");
                    
                }}
            }, 
            //////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////
            {
                {"payment sucess",[=](cocos2d::Ref*) {
                ShowNewAlert* alert = nullptr;
                auto message = LANGUAGE_MANAGER->getDisplayTextByKey("message.payment.sucess");
                auto title = LANGUAGE_MANAGER->getDisplayTextByKey("title.payment.sucess");

                auto ok = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes");

                alert = ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);

                alert->setSizeTextMessage(Size(450, 115));
                alert->setFontSizeTextMessage(24.0);
                alert->setFontSizeTextTitle(25);
                }},
                {"DangKyTuVan",[=](cocos2d::Ref*) {
                    std::string title = LANGUAGE_MANAGER->getDisplayTextByKey("subscription.response.title");
                    std::string message, ok;
                    ok		= LANGUAGE_MANAGER->getDisplayTextByKey("subscription.response.button.title");
                    message	= LANGUAGE_MANAGER->getDisplayTextByKey("subscription.response.popup.message");

                    auto alert = ShowNewAlert::createAlert(title, message, ok, ok, false, nullptr, true, false);
                    alert->setOnOkCallback([=] {
                        auto url = MJDEFAULT->getStringForKey("pay_use_url", "https://monkey.edu.vn/san-pham/monkey-stories");
                        Application::getInstance()->openURL(url);
                    });
                }},
                {"PayMentFalail",[=](cocos2d::Ref*) {
                    MJSubscriptionGetPaymentInfo::Type typePop = MJDEFAULT->getStringForKey(key_popup_fail_payment, "phone") == "phone" ? MJSubscriptionGetPaymentInfo::Type::PHONE : MJSubscriptionGetPaymentInfo::Type::EMAIL;

                    auto view = MJSubscriptionGetPaymentInfo::createView(typePop, "");

                    this->addChild(view, INT_MAX - 1);
                }}
            }
            };

	if (auto itemList = utils::findChild<ui::ListView*>(this, "item_list"))
	{
		for(auto items : buttonNameAndFunctionList)
		{
			auto newItem = (ui::Button*)item->clone();
			itemList->addChild(newItem);

			for ( int i=0; i < items.size();i++)
			{
				auto value = items.at(i);

				ui::Button* button = nullptr;

				if (i == 0) {
					button = cocos2d::utils::findChild<ui::Button*>(newItem, "item_01");
				}
				else if (i == 1) {
					button = newItem;
				}
				else {
					button = cocos2d::utils::findChild<ui::Button*>(newItem, "item_03");
				}

				if (button) {
					button->setTitleText(value.first);
                    while (button->getTitleLabel()->getContentSize().width > (button->getContentSize().width - 10)) {
                        button->setTitleFontSize(button->getTitleFontSize() - 1);
                    }
					button->addClickEventListener(value.second);
				}
			}
		}
	}

	if (item)
	{
		item->removeFromParent();
	}

	return true;
}

void CheatMenu::onCancel(cocos2d::Ref * sender)
{
	this->removeFromParent();
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback CheatMenu::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(CheatMenu, onCancel),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void CheatMenu::onReset(std::string environmentKey)
{
    CCLOG("reset...");
    
    mj::helper::showWaitView();
    
    auto user_defaults_xml = cocos2d::UserDefault::getInstance()->getXMLFilePath();

    if (FileUtils::getInstance()->removeFile(user_defaults_xml)) {
        CCLOG("Removed User Defaults XML %s", user_defaults_xml.c_str());
    } else {
        CCLOG("Cannot remove %s", user_defaults_xml.c_str());
    }

    mj::PlatformConfig::getInstance().clearUserDefaults();
    ap::Database::getInstance().dropDatabase();
    StoryDataModel::getInstance()->dropDatabase(Director::getInstance()->getRunningScene());
    mj::helper::ResetUserData();
    auto writable_path = FileUtils::getInstance()->getWritablePath();
//    writable_path.resize(writable_path.length() - 1);
    FileUtils::getInstance()->removeDirectory(writable_path + image_cache_path);
    FileUtils::getInstance()->removeDirectory(writable_path + "mjstory/");
    FileUtils::getInstance()->removeDirectory(writable_path + "audiobooks/");
    FileUtils::getInstance()->removeDirectory(writable_path + "cached/");
    FileUtils::getInstance()->removeDirectory(writable_path + "extracted/");
    FileUtils::getInstance()->removeDirectory(writable_path + "games/");

    MJDEFAULT->setStringForKey("use_url", "");
    UtilitySqlite::shareInstance()->deleteData("mjdefault");
    StoryDataModel::getInstance()->removeAllStories();

    UtilitySqlite::shareInstance()->onCloseDatabase();
    StoryDataModel::getInstance()->closeConnect();

    FileUtils::getInstance()->removeFile(writable_path + "database.sqlite");
    FileUtils::getInstance()->removeFile(writable_path + "mjdefault.sqlite");
    FileUtils::getInstance()->removeFile(writable_path + "ap.sqlite");

    UtilitySqlite::shareInstance()->encryptData();
    
    if(environmentKey=="debugkey_dev_environment") {

        MJDEFAULT->setBoolForKey("debugkey_dev_environment", true);
        MJDEFAULT->setBoolForKey("debugkey_product_environment", false);
        //MJDEFAULT->setBoolForKey("mjdk_full_features", true);
        //MJDEFAULT->setBoolForKey("debugkey_all_lessons", true);
    }
    
    if(environmentKey=="debugkey_product_environment") {
        
        MJDEFAULT->setBoolForKey("debugkey_product_environment", true);
        MJDEFAULT->setBoolForKey("debugkey_dev_environment", false);
        //MJDEFAULT->setBoolForKey("mjdk_full_features", false);
        //MJDEFAULT->setBoolForKey("debugkey_all_lessons", false);
    }
    
    if(environmentKey=="debugkey_live_environment") {
        MJDEFAULT->setBoolForKey("debugkey_product_environment", false);
        MJDEFAULT->setBoolForKey("debugkey_dev_environment", false);

        //MJDEFAULT->setBoolForKey("mjdk_full_features", false);
        //MJDEFAULT->setBoolForKey("debugkey_all_lessons", false);
    }
    
    cocos2d::UserDefault::getInstance()->flush();

    this->scheduleOnce([](float) {
        exit(0);
    }, 3.9, "delayexit");
    
}

