//
//  MSScreenRecorder.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/10/18.
//

#include <stdio.h>
#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"
#include "StoryLanguageManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include "StoryAlert.h"

namespace ms { namespace recorder {
    std::function<void()> g_recording_callback = nullptr;
    void startRecord(std::function<void()> callback ) {
        g_recording_callback = callback;
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "startRecord");
    }

    void stopRecord() {
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "stopRecord");
    }

    void cancelRecord() {
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "stopRecord");
    }

    bool isRecording() {
        return cocos2d::JniHelper::callStaticBooleanMethod("org.cocos2dx.cpp.MJHelper", "isRecording");
    }

    bool isSupported() {
        return cocos2d::JniHelper::callStaticBooleanMethod("org.cocos2dx.cpp.MJHelper", "isScreenRecordingSupported");
    }

    void enableMicro() {

    }

    void disableMicro() {

    }

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onStartRecording(JNIEnv * env, jobject thiz)
        {
            if (g_recording_callback)
            {
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]
                {
                    g_recording_callback();
                });
            }
        }

        JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_AppActivity_isVietnameseDisplay(JNIEnv * env, jobject thiz) {
            return LANGUAGE_MANAGER->getDisplayLangId() == 4;
        }

        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_showVideoPathPopup(JNIEnv * env, jobject thiz, jobject jMsg) {

                jstring str = (jstring)jMsg;

                std::string message = "";

                jboolean isCopy;
                const char* chars = env->GetStringUTFChars(str, &isCopy);
                if (chars)
                {
                    message = chars;
                    env->ReleaseStringUTFChars(str, chars);
                }

                if (!message.empty())
                {
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {

                        StoryAlert::showAlert(message, "OK", "Close");

                    });
                }

        }


    }
}}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
namespace ms {
	namespace recorder {
		void startRecord(std::function<void()> callback) {
		}

		void stopRecord() {
		}

		void cancelRecord() {
		}

		bool isRecording() {
			return false;
		}

		bool isSupported() {
			return false;
		}

		void enableMicro() {

		}

		void disableMicro() {

		}
	}
}
#endif
