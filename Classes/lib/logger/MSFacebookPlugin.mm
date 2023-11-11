//
//  MSFacebookPlugin.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/28/18.
//

#include "MSFacebookPlugin.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import <FBSDKCoreKit/FBSDKCoreKit.h>
//#endif
//namespace ms { namespace analytics {
//
//    Facebook::Facebook() {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
////        [Mixpanel sharedInstanceWithToken:@"e145b4844f53c063c4249c09d797308e"];
//#endif
//    }
//
//    void Facebook::setIdentifier(std::string identifier) {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        [FBSDKAppEvents setUserID:[NSString stringWithUTF8String:identifier.c_str()]];
//#endif
//    }
//
//    void Facebook::logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> parameters) {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        NSString * ns_name = [NSString stringWithUTF8String:name.c_str()];
//        NSMutableDictionary * ns_param = [NSMutableDictionary dictionary];
//
//        for (auto p : parameters) {
//            auto type = p.second.getType();
//            NSString * ns_key = [NSString stringWithUTF8String: p.first.c_str()];
//
//            switch (type) {
//                case cocos2d::Value::Type::STRING:
//                    [ns_param setValue:[NSString stringWithUTF8String:p.second.asString().c_str()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::INTEGER:
//                    [ns_param setValue:[NSNumber numberWithInt:p.second.asInt()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::DOUBLE:
//                    [ns_param setValue:[NSNumber numberWithDouble:p.second.asDouble()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::FLOAT:
//                    [ns_param setValue:[NSNumber numberWithFloat:p.second.asFloat()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::UNSIGNED:
//                    [ns_param setValue:[NSNumber numberWithUnsignedInteger:p.second.asUnsignedInt()] forKey:ns_key];
//                    break;
//
//                default:
//                    assert(false && "type is not supported by analytics");
//                    break;
//            }
//        }
//        [FBSDKAppEvents logEvent:ns_name parameters:ns_param];
////        [[Mixpanel sharedInstance] track:ns_name properties:ns_param];
//#endif
//
//    }
//
//    void Facebook::setUserProperties(std::map<std::string, cocos2d::Value> props) {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        NSMutableDictionary * ns_param = [NSMutableDictionary dictionary];
//        for (auto p : props) {
//            auto type = p.second.getType();
//            NSString * ns_key = [NSString stringWithUTF8String: p.first.c_str()];
//
//            switch (type) {
//                case cocos2d::Value::Type::STRING:
//                    [ns_param setValue:[NSString stringWithUTF8String:p.second.asString().c_str()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::INTEGER:
//                    [ns_param setValue:[NSNumber numberWithInt:p.second.asInt()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::DOUBLE:
//                    [ns_param setValue:[NSNumber numberWithDouble:p.second.asDouble()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::FLOAT:
//                    [ns_param setValue:[NSNumber numberWithFloat:p.second.asFloat()] forKey:ns_key];
//                    break;
//
//                case cocos2d::Value::Type::UNSIGNED:
//                    [ns_param setValue:[NSNumber numberWithUnsignedInteger:p.second.asUnsignedInt()] forKey:ns_key];
//                    break;
//
//                default:
//                    assert(false && "type is not supported by analytics");
//                    break;
//            }
//        }
//
//        [FBSDKAppEvents updateUserProperties:ns_param handler:nil];
//#endif
//    }
//}}
