//
//  APPhoneAuth-Android.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APPhoneAuth-Android.h"
#include "cocos2d.h"
using namespace cocos2d;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

namespace ap {
    void PhoneAuthAndroid::signIn(const std::string & phone_number) {
		if (phone_number == ""){
			cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "phoneLogin");
		}else{
			cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "phoneVerification", phone_number);
		}
    }

}
extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onAccountKitFailed(JNIEnv* env, jobject thiz) {
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
			cocos2d::Director::getInstance()->getScheduler()->schedule([](float) {
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
				}, cocos2d::Application::getInstance(), 0, 0, 1, false, "delay_event_login_cancelled");
			});
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onAccountKitFinished(JNIEnv* env, jobject thiz, jobject jtoken, jobject jphone) {
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

		str = (jstring)jphone;
		std::string phone;
		if (!str) {
			phone = std::string();
		}

		chars = env->GetStringUTFChars(str, &isCopy);
		if (chars) {
			phone = chars;
			env->ReleaseStringUTFChars(str, chars);
		}
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([token, phone] {
			cocos2d::Director::getInstance()->getScheduler()->schedule([token, phone](float) {
				auto finish_event = cocos2d::EventCustom("event.accountkit.android.finished");

				std::string* access_token = new std::string;
				*access_token = token;
				std::string* phone_number = new std::string;
				*phone_number = phone;
				std::pair<std::string*, std::string*>* data = new std::pair<std::string*, std::string*>;
				*data = { access_token,phone_number };
				CCLOG("nam token: %s %s", data->first->c_str(), data->second->c_str());
				finish_event.setUserData(data);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&finish_event);
				}, cocos2d::Application::getInstance(), 0, 0, 2, false, "delay_event.accountkit.android.finished");
			});
	}

}

#endif
