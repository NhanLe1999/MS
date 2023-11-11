//
//  APAuth.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/31/19.
//

#include "APAuth.h"
#include "firebase/auth.h"
#include "APGoogleOAuth.h"
#include "APFacebookOAuth.h"
#include "APPhoneAuth.h"
#include "cocos2d.h"

#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "StoryDataModel.h"

#include "APDatabase.h"
#include "APProfileManager.h"
#include "api/APUpdateUserInfoAPI.h"
#include "api/APStoreErrorAPI.h"
#include "MJPlatformConfig.h"
#include "UtilitySqlite.h"
#include "MMFirebaseDatabase.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MACOS)
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>


using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;


//
// A simple listener class to capture OAuth 2.0 HTTP redirect to localhost.
// The listener captures redirected URI and obtains the token.
// This type of listener can be implemented in the back-end to capture and store tokens.
//
class oauth2_code_listener
{
public:
	oauth2_code_listener(uri listen_uri, oauth2_config& config)
		: m_listener(new http_listener(listen_uri)), m_config(config)
	{
		m_listener->support([this](http::http_request request) -> void {
			if (request.request_uri().path() == U("/") && request.request_uri().query() != U(""))
			{
				m_resplock.lock();

				m_config.token_from_redirected_uri(request.request_uri())
					.then([this, request](pplx::task<void> token_task) -> void {
					try
					{
						token_task.wait();
						m_tce.set(true);
					}
					catch (const oauth2_exception& e)
					{
						ucout << "Error: " << e.what() << std::endl;
						m_tce.set(false);
					}
						});

				request.reply(status_codes::OK, U("Ok."));

				m_resplock.unlock();
			}
			else
			{
				request.reply(status_codes::NotFound, U("Not found."));
			}
			});

		m_listener->open().wait();
	}

	~oauth2_code_listener() { m_listener->close().wait(); }

	pplx::task<bool> listen_for_code() { return pplx::create_task(m_tce); }

private:
	std::unique_ptr<http_listener> m_listener;
	pplx::task_completion_event<bool> m_tce;
	oauth2_config& m_config;
	std::mutex m_resplock;
};

//
// Base class for OAuth 2.0 sessions of this sample.
//
class oauth2_session_sample
{
public:
	oauth2_session_sample(utility::string_t name,
		utility::string_t client_key,
		utility::string_t client_secret,
		utility::string_t auth_endpoint,
		utility::string_t token_endpoint,
		utility::string_t redirect_uri)
		: m_oauth2_config(client_key, client_secret, auth_endpoint, token_endpoint, redirect_uri, U("openid profile"))
		, m_name(name)
		, m_listener(new oauth2_code_listener(redirect_uri, m_oauth2_config))
	{
	}

	void run()
	{
		if (is_enabled())
		{
			ucout << "Running " << m_name.c_str() << " session..." << std::endl;

			if (!m_oauth2_config.token().is_valid_access_token())
			{
				if (authorization_code_flow().get())
				{
					m_http_config.set_oauth2(m_oauth2_config);
				}
				else
				{
					ucout << "Authorization failed for " << m_name.c_str() << "." << std::endl;
				}
			}

			run_internal();
		}
		else
		{
			ucout << "Skipped " << m_name.c_str()
				<< " session sample because app key or secret is empty. Please see instructions." << std::endl;
		}
	}

protected:
	virtual void run_internal() = 0;

	pplx::task<bool> authorization_code_flow()
	{
		open_browser_auth();
		return m_listener->listen_for_code();
	}

	http_client_config m_http_config;
	oauth2_config m_oauth2_config;

private:
	bool is_enabled() const
	{
		return !m_oauth2_config.client_key().empty() && !m_oauth2_config.client_secret().empty();
	}

	void open_browser_auth()
	{
		auto auth_uri(m_oauth2_config.build_authorization_uri(true));
		ucout << "Opening browser in URI:" << std::endl;
		ucout << auth_uri << std::endl;
		cocos2d::Application::getInstance()->openURL(conversions::to_utf8string(auth_uri));
	}

	utility::string_t m_name;
	std::unique_ptr<oauth2_code_listener> m_listener;
};

class google_session_sample : public oauth2_session_sample {
public:
	google_session_sample() : oauth2_session_sample(
		U("Google"),
		U("794463747839-9k08f8vt127vs6jdggusq9ik40mh4vmj.apps.googleusercontent.com"),
		U("BnogRw_PoQmTLTIxAcZWZfQJ"),
		U("https://accounts.google.com/o/oauth2/v2/auth"),
		U("https://www.googleapis.com/oauth2/v4/token"),
		U("http://localhost:8889")
	) {
	}

protected:
	void run_internal() override
	{
		http_client api(U("https://www.googleapis.com/oauth2/v3/userinfo/"), m_http_config);
		ucout << "Requesting account information:" << std::endl;
		ucout << "Information: " << api.request(methods::GET, U("~?format=json")).get().extract_json().get()
			<< std::endl;
	}
};

#endif

#include "api/APBaseAPI.h"
#include "api/APLoginAPI.h"
#include "MJPlatformConfig.h"
#include "api/APLogoutAPI.h"

//#include "DatabaseHelper.h"
namespace ap {
    std::string Auth::_mail = "";
    std::string Auth::_token_facebook ="";
	void Auth::init() 
	{
		api::APIConfig config = api::APIConfig();
		ap::api::Base::setGlobalConfiguration(config);
	}

	void Auth::authenticateWithFacebook(std::string i_newAccessToken)
	{
		auto facebook_oauth = FacebookOAuth::createFacebookOAuth([i_newAccessToken](const std::string& access_token, const std::string &email) {
			Auth::authenticateWithFacebookToken(access_token, email,i_newAccessToken);
			});
		facebook_oauth->signIn();
	}

	void Auth::authenticateWithGoogle(std::string i_newAccessToken)
	{
		auto google_oauth = GoogleOAuth::createGoogleOAuth([i_newAccessToken](const std::string& id_token, const std::string& email) {
			Auth::authenticateWithGoogleToken(id_token, email, i_newAccessToken);
			});
		google_oauth->signIn();
	}

	void Auth::authenticateWithPhoneNumber(const std::string & phone_number) {
		auto phone_auth = PhoneAuth::createPhoneAuth([](const std::string& phone_number, const std::string& token) {
			authenticateWithAKToken(token, phone_number);
		});
		phone_auth->signIn(phone_number);
	}
	void Auth::authenticateWithAKToken(const std::string& token,const std::string& phone_number)
	{
		CCLOG("account kit finished - phone: %s, token: %s", phone_number.c_str(), token.c_str());
		
		ap::api::Login::call(api::Login::LoginType::PHONE, phone_number, phone_number, "", token, "","", [=](int error_code, bool is_success, std::string msg) {
			if (is_success) {
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			}
			else {
				showErrorAlert(msg);
				CCLOG("error: %s", msg.c_str());
			}
		});
	}

    void Auth::authenticateWithGoogleToken(const std::string& token, const std::string& email, const  std::string &i_newAccessToken)
	{
		CCLOG("login| called authenticateWithGoogleToken");

        ap::api::Login::callV3(api::Login::LoginType::GOOGLE, "", "", email,token,"","","", i_newAccessToken, [i_newAccessToken](int error_code, bool is_success, std::string message, std::string tokenToChangePassword, ap::api::Login::LoginDataInfo info)
		{
            if (is_success) 
			{
				if (!i_newAccessToken.empty() )
				{
					auto ev = new EventCustom("event_show_change_default_password_screen");

					ap::api::Login::LoginDataInfo* infoPr = new ap::api::Login::LoginDataInfo(info);

					ev->setUserData(infoPr);

					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(ev);
				}
				else
				{
					auto ev = EventCustom("event_login_successfully");

					std::string type("google");

					ev.setUserData(&type);

					CCLOG("login|login_with_google successful");

					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
				}
            }
            else 
			{
				showErrorAlert(message);

				if (!i_newAccessToken.empty())
				{
					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_remove_waitting_view");
                    
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_when_link_account_failed");
				}
            }
        });
	}

    void Auth::authenticateWithFacebookToken(const std::string& token, const std::string& email, const  std::string &i_newAccessToken)
	{
		_mail = email;
		_token_facebook = token;
		ap::api::Login::callV3(api::Login::LoginType::FACEBOOK, "", "", email, token, "", "", "", i_newAccessToken, [i_newAccessToken](int error_code, bool is_success, std::string message, std::string tokenToChangePassword, ap::api::Login::LoginDataInfo info)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([error_code, is_success, message, info, i_newAccessToken]
			{
				if (is_success )
				{
					std::string evKey = "event_login_successfully";

					if (!i_newAccessToken.empty() && info.oldUserInfo._phone.empty() && info.oldUserInfo._mail.empty())
					{
						evKey = "event_show_update_phone_number_screen";
					}
					else if (!i_newAccessToken.empty())
					{
						evKey = "event_show_change_default_password_screen";

					}

					auto ev = new EventCustom(evKey);

					if (evKey != "event_login_successfully")
					{
						ap::api::Login::LoginDataInfo* infoPr = new ap::api::Login::LoginDataInfo(info);
				
						ev->setUserData(infoPr);
					}
					else
					{
						std::string type("facebook");

						ev->setUserData(&type);
					}

					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(ev);
				}
				else
				{
					if (error_code == 305)
					{
						auto app = firebase::App::GetInstance();
						if (!app)
						{
							auto json = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services.json");
							auto option = firebase::AppOptions::LoadFromJsonConfig(json.c_str());
						}
						auto auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
						auto credential = firebase::auth::FacebookAuthProvider::GetCredential(_token_facebook.c_str());

						auto result = auth->SignInWithCredential(credential);

						auto email_ptr = new std::string(_mail);

						result.OnCompletion([](const firebase::Future<firebase::auth::User*>& result, void* user_data) {
							auto user = *result.result();
							if (user) {
								auto user_id = user->uid();
								auto user_email = user->email();
								auto user_name = user->display_name();
								if (user_email.empty()) {
									user_email = *reinterpret_cast<std::string*>(user_data);
								}

								CCLOG("created user with uid: %s email: %s and name: %s", user_id.c_str(), user_email.c_str(), user_name.c_str());

								auto get_token = user->GetToken(false);
								get_token.OnCompletion([user_name, user_email, user_id](const firebase::Future<std::string>& result) {
									auto token = *result.result();
									CCLOG("jwt token: %s", token.c_str());
									ap::api::Login::call(api::Login::LoginType::FACEBOOK, user_name, "", user_email, _token_facebook, user_id, "", [](int error_code, bool is_success, std::string message) {

										if (is_success) {
											Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
												cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
											});
											//
										}
										else {
											showErrorAlert(message);
										}
									});
								});
							}
							else {
								CCLOG("create user failed with error: %s, %i", result.error_message(), result.error());
								Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
									StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
									cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
								});
							}
						}, email_ptr);
					}
					else
					{
						showErrorAlert(message);

						if (!i_newAccessToken.empty())
						{
							cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_remove_waitting_view");
                            
                            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_when_link_account_failed");
						}
					}
				}
			});
		});
	}

    void Auth::showNetworkDownAlert(std::string msg, StoryAlertDelegate *delegate){
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, delegate]
        {
            auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
            auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
            alert->setNetworkDownContent(false, true);
            alert->show();
            
            if (delegate) {
                alert->setDelegate(delegate);
            }
        });
    }

    void Auth::showErrorAlert(std::string msg){
        bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, is_vn] {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
            if(msg == ""){
                StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
            }else{
                //TODO: show notification
                StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
            }
        });
    }
    
#include "MJDebugView.h"

    void Auth::signOut(StoryAlertDelegate *delegate) {
        CCLOG("signing out..");
        auto user_info = ap::Database::getInstance().getUserInfo();
        PROFILE_MANAGER.saveProcess();
        mm::RealtimeDatabase::getInstance().saveData();
        
        ap::api::Logout::call([=](bool is_success, std::string msg) {
            
            if (is_success)
            {
                clearUserData();
                //mở màn hình signIn
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_logout_successfully");
                MJDEFAULT->setBoolForKey("is_user_logout", true);
            }
            else
            {
                //
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_logout_failed");
                showNetworkDownAlert(msg, delegate);
            }
        });
    }

    void Auth::clearUserData()
    {
        auto user_info = ap::Database::getInstance().getUserInfo();
        
        // delete listener for user id
        mm::RealtimeDatabase::getInstance().removeListener(user_info._id);
        
        PROFILE_MANAGER.clearDataModel();
        
        //clear all information
        CCLOG("reset...");
        auto user_defaults_xml = cocos2d::UserDefault::getInstance()->getXMLFilePath();
        
        if (FileUtils::getInstance()->removeFile(user_defaults_xml)) {
            CCLOG("Removed User Defaults XML %s", user_defaults_xml.c_str());
        } else {
            CCLOG("Cannot remove %s", user_defaults_xml.c_str());
        }
        
        std::map<std::string, bool > debug_mode = {};
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        debug_mode[ debugkey_full_features] = MJDEFAULT->getBoolForKey(debugkey_full_features);
        debug_mode[ debugkey_country_gb] = MJDEFAULT->getBoolForKey(debugkey_country_gb);
        debug_mode[ debugkey_license_disable] = MJDEFAULT->getBoolForKey(debugkey_license_disable);
        debug_mode[ debugkey_test_config] = MJDEFAULT->getBoolForKey(debugkey_test_config);
        debug_mode[ debugkey_quick_play_game] = MJDEFAULT->getBoolForKey(debugkey_quick_play_game);
        debug_mode[ debugkey_test_7_days] = MJDEFAULT->getBoolForKey(debugkey_test_7_days);
        debug_mode[ debugkey_test_free] = MJDEFAULT->getBoolForKey(debugkey_test_free);
        debug_mode[ debugkey_dev_environment] = MJDEFAULT->getBoolForKey(debugkey_dev_environment);
        debug_mode[ debugkey_product_environment] = MJDEFAULT->getBoolForKey(debugkey_product_environment);
        debug_mode[ debugkey_fake_device_id] = MJDEFAULT->getBoolForKey(debugkey_fake_device_id);
        debug_mode[ debugkey_all_lessons] = MJDEFAULT->getBoolForKey(debugkey_all_lessons);
        debug_mode[ debugkey_fake_ipx] = MJDEFAULT->getBoolForKey(debugkey_fake_ipx);
        debug_mode[ debugkey_is_test_license] = MJDEFAULT->getBoolForKey(debugkey_is_test_license);
        debug_mode[ debugkey_test_act_vmonkey] = MJDEFAULT->getBoolForKey(debugkey_test_act_vmonkey);
    #endif
        auto v_cmonres_cur = MJDEFAULT->getIntegerForKey(key_common_res_version_current, -1);
        auto v_cmonres_req = MJDEFAULT->getIntegerForKey(key_common_res_version_request, 0);
        
        auto current_lang = MJDEFAULT->getIntegerForKey(key_current_lang);
        auto key_common_res = MJDEFAULT->getIntegerForKey(key_common_res_version_current);
        auto key_converted = MJDEFAULT->getBoolForKey(key_data_converted);
        auto lang_display = MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);
        auto is_account_unverified = MJDEFAULT->getBoolForKey("account_unverified", false);
        auto is_skip_login = MJDEFAULT->getBoolForKey("is_skip_login", false);
        auto time_popup_upgrade = MJDEFAULT->getIntegerForKey("time_skip_popup_upgrade_account", 0);

        auto isShowLanguageScene = MJDEFAULT->getBoolForKey("is_language_selection_show",false);
        
        auto last_time_enter_info = MJDEFAULT->getStringForKey(key_last_time_enter_info + std::to_string(user_info._id));

        auto keyLastUpdateId    = MJDEFAULT->getIntegerForKey("key_last_update_id",-1);
        auto keyUpdateShow      =   MJDEFAULT->getBoolForKey(key_update_show, false);
        auto keyTimeUpdateSkip  = MJDEFAULT->getIntegerForKey(key_time_update_skip, 0);
        auto loginType          = MJDEFAULT->getStringForKey("ap_login_type","none");
        auto keyListLang        = MJDEFAULT->getStringForKey(key_list_lang,"[1]");
        MJDEFAULT->deleteValueForKey("require_load_update");
        mj::PlatformConfig::getInstance().clearUserDefaults();
        
        auto writable_path = FileUtils::getInstance()->getWritablePath();
        
        FileUtils::getInstance()->removeDirectory(writable_path + "profile/");
      
        StoryDataModel::getInstance()->resetLessonLearned();
        
        MJDEFAULT->setIntegerForKey(key_common_res_version_current, v_cmonres_cur);
        MJDEFAULT->setIntegerForKey(key_common_res_version_request, v_cmonres_req);
        
        MJDEFAULT->setBoolForKey("is_language_selection_show",isShowLanguageScene);

        MJDEFAULT->setStringForKey("ap_login_type",loginType);
        MJDEFAULT->setBoolForKey("is_skip_login", is_skip_login);
        ap::Database::getInstance().dropDatabaseProfiles();
        
        MJDEFAULT->setIntegerForKey("key_last_update_id",keyLastUpdateId);
        MJDEFAULT->setBoolForKey(key_update_show,keyUpdateShow);
        MJDEFAULT->setIntegerForKey(key_time_update_skip, keyTimeUpdateSkip);

        MJDEFAULT->setBoolForKey(key_logged_in, false);
        
        //máy đã từng sign out -> đã có tài khoản đc convert
        MJDEFAULT->setBoolForKey(key_first_logged_out, true);
        MJDEFAULT->setIntegerForKey(key_common_res_version_current, key_common_res);
        MJDEFAULT->setIntegerForKey(key_current_lang, current_lang);
        MJDEFAULT->setBoolForKey(key_data_converted, key_converted);
        MJDEFAULT->setIntegerForKey(key_lang_display, lang_display);
        MJDEFAULT->setIntegerForKey("time_skip_popup_upgrade_account", time_popup_upgrade);
        MJDEFAULT->setStringForKey(key_list_lang, keyListLang);
        MJDEFAULT->setStringForKey(key_last_time_enter_info + std::to_string(user_info._id), last_time_enter_info);

		LANGUAGE_MANAGER->updateLanguageWithID(lang_display);
        
        for(auto m: debug_mode)
        {
            MJDEFAULT->setBoolForKey(m.first, m.second);
        }
    }

}


