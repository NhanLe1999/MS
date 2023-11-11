//
//  APFacebookOAuth-iOS.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APFacebookOAuth-iOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <FBSDKLoginKit/FBSDKLoginKit.h>
#include <Firebase.h>

@interface FacebookAuthDelegate : NSObject
- (void)signIn;


@property(nonatomic) ap::FacebookOAuth * auth;
@end

@implementation FacebookAuthDelegate
-(void) signIn {
    FBSDKLoginManager *loginManager = [[FBSDKLoginManager alloc] init];
    [loginManager logOut];
    [loginManager logInWithPermissions:@[@"public_profile", @"email"] fromViewController: [[[UIApplication sharedApplication] keyWindow] rootViewController] handler:^(FBSDKLoginManagerLoginResult * _Nullable result, NSError * _Nullable error) {
        if (error) {
            NSLog(@"Process error");
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
        } else if (result.isCancelled) {
            NSLog(@"Cancelled");
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
        } else {
            NSLog(@"Logged in");
            if ([FBSDKAccessToken currentAccessToken]){
                [[[FBSDKGraphRequest alloc] initWithGraphPath:@"me" parameters:@{@"fields": @"email"}]
                  startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                    if (!error) {
                       NSLog(@"user:%@", result);
                        auto email = [[result valueForKey:@"email"] UTF8String];
                        if(email){
                            _auth->onDidAuthenticated([[FBSDKAccessToken currentAccessToken].tokenString UTF8String], email);
                        }else{
//                            email = [[result valueForKey:@"id"] UTF8String];
                            email = "";
                            _auth->onDidAuthenticated([[FBSDKAccessToken currentAccessToken].tokenString UTF8String], email);
//                            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_email_null");
                        }
                    }
                }];
            }
        }
    }];
}

@end

namespace ap {
    void FacebookOAuthiOS::signIn() {
        FacebookAuthDelegate * delegate = [FacebookAuthDelegate new];
        delegate.auth = this;
        [delegate signIn];
    }
}
#endif
