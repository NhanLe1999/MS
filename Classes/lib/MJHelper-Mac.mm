//
//  MJHelper-Apple.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJHelper-Mac.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

//#import <UIKit/UIKit.h>
//#import <Foundation/Foundation.h>

namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait) {
            if (isPortrait) {
            }
        }

        void shareStory() {
//            UIActivityViewController * share_vc = [[UIActivityViewController alloc] initWithActivityItems:@[[NSURL URLWithString:@"http://monkeystories.net"]] applicationActivities:nil];
//            [[share_vc popoverPresentationController] setSourceView:[[[[UIApplication sharedApplication] keyWindow] rootViewController] view]];
//            [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:share_vc animated:YES completion:nil];
        }
        
        void logLanguages() {
            NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
            NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
            
            for (NSString * lang in languages) {
                NSLog(@"Language: %@", lang);
            }
        }
    }
}

#endif
