//
//  StoreHelper.m
//  MJStory
//
//  Created by To Ngoc Linh on 4/5/18.
//

#import "StoreHelper.h"
#include "MJPay.h"

@implementation StoreKitHelper
+ (id)sharedStoreHelper {
    static StoreKitHelper * shared_storehelper = nil;
    @synchronized (self) {
        if (shared_storehelper == nil) {
            shared_storehelper = [[self alloc] init];
        }
        
        return shared_storehelper;
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray<SKPaymentTransaction *> *)transactions {
    //need to refresh receipt and verify with server
    CCLOG("updated transactions");
    mj::Pay::getInstance().verify();
}
@end
