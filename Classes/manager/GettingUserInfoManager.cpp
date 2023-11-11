#include "GettingUserInfoManager.h"
#include "StoryLanguageManager.h"
#include "popup/RemindingUpdateInfoPopup.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MJCalculator.h"
#include "StoryConfigManager.h"

USING_NS_CC;

#define GETTING_USER_INFO_SHOWING_COUNT_KEY			"getting_user_info_showing_count" 
#define DEFAULT_TIME								-1
#define GETTING_USER_INFO_SHOWING_MAX_NUM			3
#define LAST_GETTING_USER_INFO_POPUP_TIME_KEY		"last_getting_user_info_popup_time"
#define SHOWING_ACTION_TAG							37752
#define TEST										0

void GettingUserInfoManager::parsePopupTitlesFromSever(char * i_jsonStr)
{
	rapidjson::Document doc;

	doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>(i_jsonStr);

	if (doc.HasParseError())
    {
        CCLOG("something wrong!");
        
		return;
	}

	if (!doc.IsObject())
    {
        CCLOG("something wrong!");
        
		return;
	}

	auto data = doc.GetObject();

	if (!JSON_SAFE_CHECK(data, "list_label_for_app", Object))
	{
		CCLOG("something wrong!");

		return;
	}
    
    m_languageList.clear();

    if(JSON_SAFE_CHECK(data["list_label_for_app"], "vi", Object))
    {
        m_languageList.insert({"vi_remainding_update_info", data["list_label_for_app"]["vi"]["remain_update_info"].GetString() });
        m_languageList.insert({"vi_getting_user_info", data["list_label_for_app"]["vi"]["getting_user_info"].GetString() });
        m_languageList.insert({"vi_link_download_ebook_lable", data["list_label_for_app"]["vi"]["link_download_ebook"]["label"].GetString() });
        m_languageList.insert({"vi_link_download_ebook_link", data["list_label_for_app"]["vi"]["link_download_ebook"]["link"].GetString() });
        m_languageList.insert({"vi_link_download_ebook_detail", data["list_label_for_app"]["vi"]["link_download_ebook"]["detail"].GetString() });
    }
    
    if(JSON_SAFE_CHECK(data["list_label_for_app"], "en", Object))
    {
        m_languageList.insert({"en_remainding_update_info", data["list_label_for_app"]["en"]["remain_update_info"].GetString() });
        m_languageList.insert({"en_getting_user_info", data["list_label_for_app"]["en"]["getting_user_info"].GetString() });
        m_languageList.insert({"en_link_download_ebook_lable", data["list_label_for_app"]["en"]["link_download_ebook"]["label"].GetString() });
        m_languageList.insert({"en_link_download_ebook_link", data["list_label_for_app"]["en"]["link_download_ebook"]["link"].GetString() });
        m_languageList.insert({"en_link_download_ebook_detail", data["list_label_for_app"]["en"]["link_download_ebook"]["detail"].GetString() });
    }
    
    if(JSON_SAFE_CHECK(data["list_label_for_app"], "th", Object))
    {
        m_languageList.insert({"th_remainding_update_info", data["list_label_for_app"]["th"]["remain_update_info"].GetString() });
        m_languageList.insert({"th_getting_user_info", data["list_label_for_app"]["th"]["getting_user_info"].GetString() });
        m_languageList.insert({"th_link_download_ebook_lable", data["list_label_for_app"]["th"]["link_download_ebook"]["label"].GetString() });
        m_languageList.insert({"th_link_download_ebook_link", data["list_label_for_app"]["th"]["link_download_ebook"]["link"].GetString() });
        m_languageList.insert({"th_link_download_ebook_detail", data["list_label_for_app"]["th"]["link_download_ebook"]["detail"].GetString() });
    }
    
    m_phonCodeList.clear();
    
    if (JSON_SAFE_CHECK(data, "phone_country_list", Array))
    {
        auto viProblems = data["phone_country_list"].GetArray();

        for (size_t i = 0; i < viProblems.Size(); i++)
        {
            auto viObj = viProblems[i].GetObject();

            //    PhoneCodeInfo(std::string i_id,std::string i_name, std::string i_code,bool i_selected)

            PhoneCodeInfo info = PhoneCodeInfo{
            viObj["key"].GetString(),
            viObj["name"].GetString(),
            viObj["code"].GetString(),
            viObj["selected"].GetBool(),
            };
            
            if(info.selected)
            {
                MJDEFAULT->setStringForKey("phonecode_id",info.id);
                MJDEFAULT->setStringForKey("phonecode_code",info.code);
            }
            
            m_phonCodeList.push_back(info);
        }
    }

	m_remindingUpdateInfoPopupCount = 0;

	m_isFirebaseDataLoaded = false;
//	mm::RealtimeDatabase::getInstance().addRemindingUpdateInfoPopupSkipVariableListener(); will add in MMFirebaseDatabase

}

void GettingUserInfoManager::showGettingUserInfoPopup(bool isSkipButtonEnable, bool isRewardPopupShow)
{
    if(auto scene = Director::getInstance()->getRunningScene())
    {
        auto check_node = scene->getChildByName("update_popup");
        
        if (check_node)
        {
            return;
        }
        
        //
        // only one popup ( remaindingPopup,updatePopup, gettingUserInfo popup) should be show at a time.
        // please search "update_popup" to get more information.
        //
        //
        //auto popup = GettingUserInfoPopup::createView(isSkipButtonEnable, isRewardPopupShow);
        //
        //popup->setName("update_popup");
        //
        //scene->addChild(popup, INT_MAX - 1); // skip b
    }
}

void GettingUserInfoManager::increaseRemindingUpdateInfoPopupCount()
{
	m_remindingUpdateInfoPopupCount += 1;

	//MJDEFAULT->setIntegerForKey(GETTING_USER_INFO_SHOWING_COUNT_KEY, MJDEFAULT->getIntegerForKey(GETTING_USER_INFO_SHOWING_COUNT_KEY) + 1);

	if (m_remindingUpdateInfoPopupCount > GETTING_USER_INFO_SHOWING_MAX_NUM) // don't update if the value on the server > GETTING_USER_INFO_SHOWING_MAX_NUM
	{
		return;
	}

	mm::RealtimeDatabase::getInstance().updateRemindingUpdateInfoPopupSkipNumber(m_remindingUpdateInfoPopupCount);
}

bool GettingUserInfoManager::hasRemindingUpdateInfoPopupShow()
{
    return false;
    
//	//LAST_RATING_TIME_KEY
//	auto lastRatingTime = MJDEFAULT->getIntegerForKey(LAST_GETTING_USER_INFO_POPUP_TIME_KEY, DEFAULT_TIME);
//
//	auto currentTime = mj::helper::getTime() / 3600;//convert to hour
//
//	auto userInfo = ap::Database::getInstance().getUserInfo();
//	
//#if TEST && defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//	currentTime = mj::helper::getTime() / 60;//minute.
//#else
//	currentTime = mj::helper::getTime() / 3600;//hour
//#endif 
//	
//	CCLOG("m_isRatingPopupShow %s, m_remindingUpdateInfoPopupCount %d,lastRatingTime %d ,currentTime %d,userInfo._mail = %s ", userInfo._isNewUser == true ? "true" : "false", m_remindingUpdateInfoPopupCount, lastRatingTime, currentTime, userInfo._mail.c_str());
//
//
//#if TEST 
//	if (userInfo._isNewUser || (userInfo._mail.empty() && m_remindingUpdateInfoPopupCount < GETTING_USER_INFO_SHOWING_MAX_NUM && (lastRatingTime == DEFAULT_TIME || (currentTime - lastRatingTime) >= 3))) //test mode: 3 minutes
//#else
//	// (new user && user dont have an email)  && showing count <3 && ( neu chua show || lan truoc da show roi den thoi diem nay phai >= 3 ngay )
//	if (userInfo._isNewUser || (userInfo._mail.empty() && m_remindingUpdateInfoPopupCount < GETTING_USER_INFO_SHOWING_MAX_NUM && (lastRatingTime == DEFAULT_TIME || (currentTime - lastRatingTime) >= 3 * 24)))
//#endif 	
//	{
//		return true;
//	}
//
//	return false;

}

void GettingUserInfoManager::showRemindingUpdateInfoPopup(std::function<void()> i_callback)
{
    return;
    
	//auto userInfo = ap::Database::getInstance().getUserInfo();

	//if (userInfo._isNewUser)
	//{
	//	GettingUserInfoManager::GetInstance()->showGettingUserInfoPopup(false,true);

	//	return;
	//}

	//if (hasRemindingUpdateInfoPopupShow())
	//{
 //       if(auto scene = Director::getInstance()->getRunningScene())
 //       {
 //           //
 //           // only one popup ( remaindingPopup,updatePopup, gettingUserInfo popup) should be show at a time.
 //           // please search "update_popup" to get more information.
 //           //
 //           
 //           auto check_node = scene->getChildByName("update_popup");
 //           
 //           if (check_node)
 //           {
 //               return;
 //           }
 //           
 //           auto remaidingPopup = RemindingUpdateInfoPopup::createView();

 //           remaidingPopup->setName("update_popup");
 //           
 //           remaidingPopup->setOKButtonCallback(i_callback);

 //           Director::getInstance()->getRunningScene()->addChild(remaidingPopup, INT_MAX - 1);
 //         
 //       }
//	}
}

std::string GettingUserInfoManager::getRemaidingUpdateInfoPopupTitle()
{
    const std::string k_key = StringUtils::format("%s_remainding_update_info", getLanguageKey().c_str());
    
	return (m_languageList.count(k_key) ? m_languageList.at(k_key): "" );
}

std::string GettingUserInfoManager::getGettingUserInfoPopupTitle()
{
    const std::string k_key = StringUtils::format("%s_getting_user_info", getLanguageKey().c_str());
   
	return (m_languageList.count(k_key) ? m_languageList.at(k_key) : "");
}

std::string GettingUserInfoManager::getRewardPopupStringFromServer()
{
    const std::string k_key = StringUtils::format("%s_link_download_ebook", getLanguageKey().c_str());

	return (m_languageList.count(k_key) ? m_languageList.at(k_key) : "");
}

void GettingUserInfoManager::setRemindingUpdateInfoPopupCount(int i_value)
{
	if (m_remindingUpdateInfoPopupCount == i_value)
	{
		return;
	}

	m_remindingUpdateInfoPopupCount = i_value;
}

void GettingUserInfoManager::startCalculationTime()
{
#if TEST 
	MJDEFAULT->setIntegerForKey(LAST_GETTING_USER_INFO_POPUP_TIME_KEY, mj::helper::getTime() / 60);//minutes
#else
	MJDEFAULT->setIntegerForKey(LAST_GETTING_USER_INFO_POPUP_TIME_KEY, mj::helper::getTime() / 3600);//hours
#endif
}

void GettingUserInfoManager::deleteData()
{
	MJDEFAULT->deleteValueForKey(LAST_GETTING_USER_INFO_POPUP_TIME_KEY);

	m_remindingUpdateInfoPopupCount = 0; //for other user 
}

void GettingUserInfoManager::show()
{
    static float timeout = 0.0f;
    
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
	{
		auto delayAction = RepeatForever::create(Sequence::createWithTwoActions(CallFunc::create([=]() {
		
			CCLOG("GettingUserInfoManager: excute repeat action!");

            timeout += 1.0f;
            
            if( timeout >= 10.0f)
            {
                Director::getInstance()->getRunningScene()->stopActionByTag(SHOWING_ACTION_TAG);
            }
            
			if (!m_isFirebaseDataLoaded)
			{
				return;
			}
           
			Director::getInstance()->getRunningScene()->stopActionByTag(SHOWING_ACTION_TAG);

			GettingUserInfoManager::GetInstance()->showRemindingUpdateInfoPopup([=]() {

				MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {

					GettingUserInfoManager::GetInstance()->showGettingUserInfoPopup(false, true);

				}, [=]()
				{
					GettingUserInfoManager::GetInstance()->startCalculationTime();
				});

			});
		
		}), DelayTime::create(1.0f)));

		delayAction->setTag(SHOWING_ACTION_TAG);

		Director::getInstance()->getRunningScene()->runAction(delayAction);
		
	});

	//mm::RealtimeDatabase::getInstance().retrieveCustomData([=](bool isSuccess) {

	//	if (isSuccess)
	//	{
	//		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
	//		{
	//			GettingUserInfoManager::GetInstance()->showRemindingUpdateInfoPopup([=]() {

	//				MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {

	//					GettingUserInfoManager::GetInstance()->showGettingUserInfoPopup(false, true);

	//				}, [=]()
	//				{

	//					GettingUserInfoManager::GetInstance()->startCalculationTime();

	//					//GettingUserInfoManager::GetInstance()->increaseRemindingUpdateInfoPopupCount();

	//				});

	//			});
	//		});
	//	}
	//}
	//);

}

void GettingUserInfoManager::setFirebaseDataLoaded(bool i_value)
{
	m_isFirebaseDataLoaded = i_value;
}

std::string GettingUserInfoManager::getLanguageKey()
{
    return  ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en"));
}


