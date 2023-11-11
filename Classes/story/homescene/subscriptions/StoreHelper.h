//
//  StoreHelper.h
//  MJStory
//
//  Created by To Ngoc Linh on 4/5/18.
//

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface StoreKitHelper : NSObject<SKPaymentTransactionObserver> {
}
+ (id) sharedStoreHelper;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray<SKPaymentTransaction *> *)transactions;
@end
#endif