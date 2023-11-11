//
//  APGoogleOAuth-Android.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APGoogleOAuth-Android.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include "APWaitView.h"

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onGoogleSignInFailed(JNIEnv * env, jobject thiz ) {
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
            cocos2d::Director::getInstance()->getScheduler()->schedule([](float) {
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
            }, cocos2d::Application::getInstance(), 0, 0, 1, false, "delay_event_login_cancelled");

        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onGoogleSignInFinished(JNIEnv * env, jobject thiz, jstring jtoken, jstring jemail) {

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

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([token, email] {
            cocos2d::Director::getInstance()->getScheduler()->schedule([token, email](float) {
                auto finish_event = cocos2d::EventCustom("event.googleSignIn.finished");
                std::string * tokeN = new std::string;
                * tokeN = token;
                std::string * mail = new std::string;
                * mail = email;
                std::vector<std::string> info;
                info.push_back(token);
                info.push_back(email);
                finish_event.setUserData(&info);
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&finish_event);
            }, cocos2d::Application::getInstance(), 0, 0, 1, false, "delay_event.googleSignIn.finished");
        });
    }
}

namespace ap {

    void GoogleOAuthAndroid::signIn() {
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "loginWithGoogle");

    }
}
#endif
