//
//  MJInApp-Android.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/27/17.
//

#include "MJInApp-Android.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MJPay.h"
#include "MJAPIRestore.h"
#include "platform/android/jni/JniHelper.h"
#include "../../external/json/document.h"
#include "MJDefinitions.h"

#define INAPP_CLASS "org.cocos2dx.cpp.IABHelper"

void mj::InAppAndroid::init(const std::vector<std::string> & ids) {

#if CC_PLATFORM_HUAWEI
    return;
#endif
    JniHelper::callStaticVoidMethod(INAPP_CLASS, "initBillingProcessor");
}

void mj::InAppAndroid::purchase(const std::string product_id) {
#if CC_PLATFORM_HUAWEI
    return;
#endif
    JniHelper::callStaticVoidMethod(INAPP_CLASS, "purchase", product_id);
}

void mj::InAppAndroid::refresh(const std::vector<std::string> & ids) {

#if CC_PLATFORM_HUAWEI
    return;
#endif

    auto arr_string = cocos2d::JniHelper::callStaticStringMethod(INAPP_CLASS, "getPackagesInfo");
    CCLOG("package info: %s", arr_string.c_str());

    rapidjson::Document doc;
    doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(arr_string.c_str());

    std::vector<mj::InAppProduct> iaps;
    for (auto i = 0; i < static_cast<int>(doc.Size()); i++) {
        auto o = doc[i].GetObject();

        mj::InAppProduct iap;
        iap.product_id = o["product_id"].GetString();
        iap.price = o["priceValue"].GetFloat();
        iap.currency_code = o["currency"].GetString();

        iaps.push_back(iap);
    }
    mj::Pay::getInstance().onProductRequestSuccess(iaps);
}


void mj::InAppAndroid::restore() {

#if CC_PLATFORM_HUAWEI
    return;
#endif

    cocos2d::JniHelper::callStaticVoidMethod(INAPP_CLASS, "restore");

    if(Director::getInstance()->getEventDispatcher()->hasEventListener("mj_inapp_restore_error_event")) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_error_event");
    }

    if(Director::getInstance()->getEventDispatcher()->hasEventListener("mj_inapp_restore_complete_event")) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_complete_event");
    }

    Director::getInstance()->getEventDispatcher()->addCustomEventListener("mj_inapp_restore_complete_event", [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_complete_event");
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_error_event");

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

            _listener->onRestoreComplete(true, "");

        });


    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener("mj_inapp_restore_error_event", [this](EventCustom* eventCustom) {

        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_complete_event");
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("mj_inapp_restore_error_event");

        std::string* data = static_cast<std::string*>(eventCustom->getUserData());

        std::string message = data != nullptr ? std::string(*data) : "";

        CC_SAFE_DELETE(data);

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

            _listener->onRestoreComplete(false, message);

        });

    });

}

std::string mj::InAppAndroid::getReceipt(const std::string & product_id) {
    return "";
}

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_IABHelper_onCppBillingInitialized(JNIEnv * env, jobject thiz) {
//        mj::Pay::getInstance().onInitialized();
//        auto arr = safejni::callStatic<std::vector<std::string>>(INAPP_CLASS, "getPackagesInfo");
//        std::vector<mj::InAppProduct> iaps;
//        for (int i = 0; i < arr.size(); ++i) {
//            CCLOG("IAP %s", arr[i].c_str());
//            if (arr[i] == "fake") {
//                continue;
//            }
//            mj::InAppProduct iap;
//            iap.product_id = arr[i];
//            iap.price = cocos2d::utils::atof(arr[++i].c_str());
//            iap.currency_code = arr[++i];
//
//            iaps.push_back(iap);
//        }
//        mj::Pay::getInstance().onProductRequestSuccess(iaps);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_IABHelper_onCppBillingError(JNIEnv * env, jobject thiz, jint error) {

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

            mj::Pay::getInstance().onFailed({
                                                    "",
                                                    "",
                                                    "",
                                                    "",
                                            });
        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_IABHelper_onCppProductPurchased(JNIEnv * env, jobject thiz, jobjectArray array) {
        std::vector<std::string> result;
        if (array) {
            jint length = env->GetArrayLength(array);
            for (int i = 0; i < length; i++) {
                jobject valueJObject = env->GetObjectArrayElement(array, i);
                jstring str = (jstring)valueJObject;
                std::string r;
                if (!str) {
                    r = std::string();
                }
                jboolean isCopy;
                const char* chars = env->GetStringUTFChars(str, &isCopy);
                if (chars) {
                    r = chars;
                    env->ReleaseStringUTFChars(str, chars);
                }
                result.push_back(r);
                env->DeleteLocalRef(valueJObject);
            }
        }

        mj::Pay::getInstance().onSuccess({
            result[1],
            "",
            "",
            result[4],
        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_IABHelper_onCppPurchaseHistoryRestored(JNIEnv * env, jobject thiz,jobject jdetails) {

        CCLOG("Purchase Restored");

        std::string details = "";

        if (jdetails) {
            jstring jstr = (jstring)jdetails;
            jboolean is_copy;
            const char * chars = env->GetStringUTFChars(jstr, &is_copy);
            if (chars) {
                details = chars;
                env->ReleaseStringUTFChars(jstr, chars);
            }
        }

        if(!details.empty())
        {
            rapidjson::Document doc;

            doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(details.c_str());

            if (doc.HasParseError())
            {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("excute_verify_callback");
                return;
            }

            std::string packID      = JSON_SAFE_EXTRACT(doc,"product_id",String,"");
            std::string packToken   = JSON_SAFE_EXTRACT(doc,"purchase_token",String,"");

            mj::RestoreAPI::callOnce(packID,packToken, [=](bool isSuccess,std::string message ) {

                if(isSuccess) {
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mj_inapp_restore_complete_event");
                }
                else {
                    std::string* errMessage = new std::string();

                    (*errMessage) = message;

                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mj_inapp_restore_error_event",errMessage);
                }

            });
        }
    }



    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_IABHelper_onRestoreError(JNIEnv * env, jobject thiz,jobject jdetails) {

        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mj_inapp_restore_error_event");

        CCLOG("Purchase Restored");
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onDebugEpoch(JNIEnv * env, jobject thiz, jint value) {
        cocos2d::UserDefault::getInstance()->setIntegerForKey("key.debug.epoch", value);
    }

}
#endif
