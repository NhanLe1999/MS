//
//  APGoogleOAuth-iOS.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/19/19.
//

#include "APGoogleOAuth-iOS.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <GoogleSignIn/GoogleSignIn.h>

@interface GoogleAuthDelegate : NSObject<GIDSignInDelegate/*, GIDSignInUIDelegate*/>
- (void)signIn:(GIDSignIn *)signIn didSignInForUser:(GIDGoogleUser *)user withError:(NSError *)error;
- (void)signIn:(GIDSignIn *)signIn didDisconnectWithUser:(GIDGoogleUser *)user withError:(NSError *)error;

// The sign-in flow has finished selecting how to proceed, and the UI should no longer display
// a spinner or other "please wait" element.
- (void)signInWillDispatch:(GIDSignIn *)signIn error:(NSError *)error;

// If implemented, this method will be invoked when sign in needs to display a view controller.
// The view controller should be displayed modally (via UIViewController's |presentViewController|
// method, and not pushed unto a navigation controller's stack.
- (void)signIn:(GIDSignIn *)signIn presentViewController:(UIViewController *)viewController;

// If implemented, this method will be invoked when sign in needs to dismiss a view controller.
// Typically, this should be implemented by calling |dismissViewController| on the passed
// view controller.
- (void)signIn:(GIDSignIn *)signIn dismissViewController:(UIViewController *)viewController;

@property(nonatomic) ap::GoogleOAuth * auth;
@end

@implementation GoogleAuthDelegate
- (void)signIn:(GIDSignIn *)signIn didSignInForUser:(GIDGoogleUser *)user withError:(NSError *)error {
    // ...
    if (error == nil) {
        GIDAuthentication *authentication = user.authentication;
        _auth->onDidAuthenticated([authentication.accessToken UTF8String], [user.profile.email UTF8String]);
        //        FIRAuthCredential *credential =
        //        [FIRGoogleAuthProvider credentialWithIDToken:authentication.idToken
        //                                         accessToken:authentication.accessToken];
        // ...
    } else {
        // ...
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
    }
}

- (void)signIn:(GIDSignIn *)signIn didDisconnectWithUser:(GIDGoogleUser *)user withError:(NSError *)error {
    // Perform any operations when the user disconnects from app here.
    // ...
}

- (void)signIn:(GIDSignIn *)signIn dismissViewController:(UIViewController *)viewController {
    auto root = [UIApplication sharedApplication].keyWindow.rootViewController;
       [root dismissViewControllerAnimated:true completion:nil];
}

- (void)signIn:(GIDSignIn *)signIn presentViewController:(UIViewController *)viewController {
    auto root = [UIApplication sharedApplication].keyWindow.rootViewController;
    [root presentViewController:viewController animated:true completion:nil];
}

- (void)signInWillDispatch:(GIDSignIn *)signIn error:(NSError *)error {
//    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
}
@end

namespace ap {
    void GoogleOAuthiOS::signIn() {
        GoogleAuthDelegate * delegate = [GoogleAuthDelegate new];
        delegate.auth = this;
        //[GIDSignIn sharedInstance].presentingViewController = delegate;
        [GIDSignIn sharedInstance].delegate = delegate;

        
        //[GIDSignIn sharedInstance].uiDelegate = delegate;
        [[GIDSignIn sharedInstance] signOut];
        [[GIDSignIn sharedInstance] signIn];
    }
}
#endif
