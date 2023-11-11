//
//  APPhoneAuthiOS.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APPhoneAuthiOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import <AccountKit/AccountKit.h>
//
//@interface PhoneAuthImpl : NSObject<AKFViewControllerDelegate>
//- (void)viewController:(UIViewController<AKFViewController> *)viewController didCompleteLoginWithAuthorizationCode:(NSString *)code state:(NSString *)state;
//- (void)viewController:(UIViewController<AKFViewController> *)viewController didCompleteLoginWithAccessToken:(id<AKFAccessToken>)accessToken state:(NSString *)state;
//- (void)signIn:(NSString*)str_phone;
//
//@property(strong, nonatomic) AKFAccountKit * account_kit;
//@property(nonatomic) ap::PhoneAuth * phone_auth;
//@end
//
//@implementation PhoneAuthImpl
//- (id) init {
//    _account_kit = [[AKFAccountKit alloc] initWithResponseType:AKFResponseTypeAccessToken];
//    return self;
//}
//
//- (void) signIn: (NSString*) str_phone {
//    AKFPhoneNumber * phone = [[AKFPhoneNumber alloc] initWithCountryCode: @"+84" phoneNumber: str_phone];
//    UIViewController<AKFViewController> * view = [_account_kit viewControllerForPhoneLoginWithPhoneNumber:phone state:NULL];
//    [view setDelegate:self];
//    [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:view animated:YES completion:NULL];
//}
//
//- (void)viewController:(UIViewController<AKFViewController> *)viewController didCompleteLoginWithAuthorizationCode:(NSString *)code state:(NSString *)state {
//    NSLog(@"Account Kit finished with code: %@ and state %@", code, state);
//    [self release];
//    [_account_kit requestAccount:^(id<AKFAccount>  _Nullable account, NSError * _Nullable error) {
//        if ([account phoneNumber] != nil) {
//            NSLog(@"Phone Number: %@", [[account phoneNumber] stringRepresentation]);
//        }
//    }];
//    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.accountkit.finished");
//}
//
//- (void)viewController:(UIViewController<AKFViewController> *)viewController didCompleteLoginWithAccessToken:(id<AKFAccessToken>)accessToken state:(NSString *)state {
//    NSLog(@"Account Kit finished with code: %@ and state %@", accessToken.tokenString, state);
//    [_account_kit requestAccount:^(id<AKFAccount>  _Nullable account, NSError * _Nullable error) {
//        if ([account phoneNumber] != nil) {
//            auto phone_number = [[[account phoneNumber] stringRepresentation] UTF8String];
//            auto access_token = [[[_account_kit currentAccessToken] tokenString] UTF8String];
//            _phone_auth->onDidAuthenticated(phone_number, access_token);
//
////            auto finish_event = cocos2d::EventCustom("event.accountkit.finished");
////            std::string * phone_number = new std::string;
////            * phone_number = [[[account phoneNumber] stringRepresentation] UTF8String];
////            finish_event.setUserData(phone_number);
////            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&finish_event);
//        }
//    }];
//}
//
//- (void)viewControllerDidCancel:(UIViewController<AKFViewController> *)viewController {
//    [self release];
//    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
//}
//
//- (void)viewController:(UIViewController<AKFViewController> *)viewController didFailWithError:(NSError *)error {
//    NSLog(@"%@ did fail with error: %@", viewController, error);
//    [self release];
//    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
//}
//
//@end

namespace ap {
    void PhoneAuthiOS::signIn(const std::string & phone_number)
    {
    // remove account kit
    //        auto phone_authentication = [PhoneAuthImpl new];
    //        phone_authentication.phone_auth = this;
    //
    //        NSString *ns_phone = [NSString stringWithCString:phone_number.c_str()
    //                                                    encoding:[NSString defaultCStringEncoding]];
    //        [phone_authentication signIn:ns_phone];
    }
}

#endif
