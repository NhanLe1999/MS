
#include "cocos2d.h"
#include "MSNewFirebase.h"
#include <firebase/firestore/field_value.h>
#include "firebase/internal/future_impl.h"
#include "firebase/future.h"
#include "APProfileManager.h"
#include "MJDefault.h"

using namespace firebase::firestore;

USING_NS_CC;

FirebaseFireStore* FirebaseFireStore::m_instance = NULL;

FirebaseFireStore* FirebaseFireStore::getInstance() {
    if (!m_instance) {
        m_instance = new FirebaseFireStore();
    }
    
    return m_instance;

}

void FirebaseFireStore::init()
{

}

FirebaseFireStore::FirebaseFireStore()
{
    auto app = ::firebase::App::GetInstance();

    firebase::AppOptions secondary_app_options;

   // secondary_app_options.set_database_url("https://monkeyx-e94aa.firebaseapp.com/");
    secondary_app_options.set_database_url("https://monkeyx-e94aa-default-rtdb.firebaseio.com");
    secondary_app_options.set_project_id("monkeyx-e94aa");
    secondary_app_options.set_storage_bucket("monkeyx-e94aa.appspot.com");
    secondary_app_options.set_messaging_sender_id("804661396414");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //android
    secondary_app_options.set_api_key("AIzaSyA6b5y_u8hfVRtoY1w2tLahcfavYPBAqlM");
    secondary_app_options.set_app_id("1:804661396414:android:8bedc2685b8b5762642fef");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


    //ios
    secondary_app_options.set_api_key("AIzaSyC45yN_fqKIxS4HgkOorytog3QylJAig0Y");
    secondary_app_options.set_app_id("1:804661396414:ios:ce6391c9c3d5eddd642fef");
#endif


    if (!app) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        app = ::firebase::App::Create(secondary_app_options, cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        app = ::firebase::App::Create(secondary_app_options);
        
#else
        auto opt_string = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services_mx.json");
        auto options = ::firebase::AppOptions::LoadFromJsonConfig(opt_string.c_str());
        if(options != nullptr)
        {
            app = ::firebase::App::Create(*options);
        }
        
#endif

        if(app != nullptr)
        {
            _fireStoreDatabase = Firestore::GetInstance(app);
        }
    }
}

void FirebaseFireStore::updateStories(int userId, int profileId, int storyId, std::function<void()> callback)
{
    bool isPushDataFireStore = MJDEFAULT->getBoolForKey("key_push_event_firestore", false);
    if(_fireStoreDatabase == nullptr || !isPushDataFireStore)
    {
//        if(callback)
//        {
//            callback();
//        }
        return;
    }
    
    auto time = utils::getTimeInMilliseconds();
    MapFieldValue datauser;
    datauser.insert({ "task_listen", FieldValue::Boolean(true)});
    datauser.insert({ "task_speak", FieldValue::Boolean(true)});
    datauser.insert({ "task_practise", FieldValue::Boolean(true)});
    datauser.insert({ "time_read", FieldValue::String(std::to_string(time))});

    _fireStoreDatabase->Collection("users").Document(std::to_string(userId))
        .Collection("profiles").Document(std::to_string(profileId))
        .Collection("courses").Document("LRC")
        .Collection("stories").Document(std::to_string(storyId))
        .Set(datauser)
        .OnCompletion([callback](const firebase::Future<void>& future) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (future.error() == Error::kErrorOk) {
                    CCLOG("error");
                }
                else {
                    CCLOG("sucess");
                }
            });
          
        });
}

void FirebaseFireStore::updateListStories(int userId, int profileId, std::vector<int> listStoryId, std::function<void()> callback)
{
    bool isPushDataFireStore = MJDEFAULT->getBoolForKey("key_push_event_firestore", false);
    if(_fireStoreDatabase == nullptr || !isPushDataFireStore)
    {
        if(callback)
        {
            callback();
        }
        return;
    }
    
    auto time = utils::getTimeInMilliseconds();
    MapFieldValue datauser;
    
    for(int i = 0; i < listStoryId.size(); i++)
    {
        MapFieldValue dataStory;
        dataStory["task_listen"] = FieldValue::Boolean(true);
        dataStory["task_speak"] = FieldValue::Boolean(true);
        dataStory["task_practise"] = FieldValue::Boolean(true);
        datauser[std::to_string(listStoryId[i])] = FieldValue::Map(dataStory);
        
    }
    MapFieldValue dataListStories;
    dataListStories["stories"] = FieldValue::Map(datauser);

    _fireStoreDatabase->Collection("users").Document(std::to_string(userId))
        .Collection("profiles").Document(std::to_string(profileId))
        .Collection("courses").Document("LRC")
        .Update(dataListStories)
        .OnCompletion([callback](const firebase::Future<void>& future) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                if (future.error() == Error::kErrorOk) {
                    CCLOG("error");
                }
                else {
                    CCLOG("sucess");
                }
            });
          
        });
}

void FirebaseFireStore::getDataStories(int userId, int profileId, std::function<void()> callback)
{
   
    if(_fireStoreDatabase == nullptr )
    {
        if(callback)
        {
            callback();
            return;
        }
    }
    
    _fireStoreDatabase->Collection("users").Document(std::to_string(userId))
        .Collection("profiles").Document(std::to_string(profileId))
        .Collection("courses").Document("LRC")
        .Collection("stories")
        .Get()
        .OnCompletion(
            [=](const firebase::Future<QuerySnapshot>& future) {

                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    if (future.error() == Error::kErrorOk) {

                        std::vector<std::pair<std::string, int>> listStoryList = {};

                        for (const DocumentSnapshot& document : future.result()->documents()) {
                            if (document.exists())
                            {
                                auto id = document.id();
                                long timeRead = utils::getTimeInMilliseconds();

                                for (auto data : document.GetData())
                                {
                                    if (data.first == "time_read")
                                    {
                                        //timeRead = std::stoi(data.second.string_value());

                                        std::istringstream iss(data.second.string_value());
                                        iss >> timeRead;

                                        break;
                                    }
                                }

                                listStoryList.push_back({ id, (int)timeRead });
                            }

                        }

                        PROFILE_MANAGER.updateReadStories(listStoryList, false);
                    }

                    if (callback)
                    {
                        callback();
                    }
                });

            });
}


