
#include "APFacebookOAuth-Android.h"
#include "cocos2d.h"
using namespace cocos2d;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

namespace ap {
	void FacebookOAuthAndroid::signIn() {
		cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "loginFacebook");

	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onFacebookFailed(JNIEnv* env, jobject thiz) {
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
			cocos2d::Director::getInstance()->getScheduler()->schedule([](float) {
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
				}, cocos2d::Application::getInstance(), 0, 0, 1, false, "delay_event_login_cancelled");
			});
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onFacebookLoginFinished(JNIEnv* env, jobject thiz, jobject jtoken, jobject jemail) {
        jstring str = (jstring)jtoken;
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

        str = (jstring)jemail;
        std::string email;
        if (!str) {
            email = std::string();
        }

        chars = env->GetStringUTFChars(str, &isCopy);
        if (chars) {
            email = chars;
            env->ReleaseStringUTFChars(str, chars);
        }

		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([token,email] {
			cocos2d::Director::getInstance()->getScheduler()->schedule([token,email](float) {
				auto finish_event = cocos2d::EventCustom("event.facebooklogin.android.finished");

				std::string* access_token = new std::string;
				*access_token = token;
				std::string* mail = new std::string;
				*mail = email;
				std::pair<std::string*, std::string*>* data = new std::pair<std::string*, std::string*>;
				*data = { access_token,mail };
                CCLOG("nam token: %s %s", data->first->c_str(),data->second->c_str());
				finish_event.setUserData(data);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&finish_event);
			}, cocos2d::Application::getInstance(), 0, 0, 2, false, "delay_event.facebooklogin.android.finished");
		});
    }

}

#endif

#endif

