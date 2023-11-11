//
//  MJInApp-Apple.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#include "MJInApp-Apple.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "TargetConditionals.h"
#import <Foundation/Foundation.h>
#import <SafariServices/SafariServices.h>
#import "RMStore.h"
#include "MJPlatformConfig.h"
#include "MJRequest.h"
#include "StoryConfigManager.h"
//#include "MJPay.h"
#include "StoreHelper.h"

/*
 Name: %s -- ID: %s -- \
 Price: %f -- \
 Receipt: %s -- \
 Receipt Cipher: %s -- \
 Transaction ID: %s",
 p.name.c_str(),
 p.id.c_str(),
 p.priceValue,
 p.receipt.c_str(),
 p.receiptCipheredPayload.c_str(),
 p.transactionID.c_str());
 */

namespace mj {
    
    std::once_flag init_storekit_observer;
    
    void InAppApple::init(const std::vector<std::string> & ids) {
//        std::call_once(init_storekit_observer, [] {
//            [[SKPaymentQueue defaultQueue] addTransactionObserver:[StoreKitHelper sharedStoreHelper]];
//        });
        id arr_products = [NSMutableArray new];
        std::for_each(ids.begin(), ids.end(), [arr_products](std::string s) {
            id nss = [NSString stringWithUTF8String:s. c_str()];
            [arr_products addObject:nss];
        });
        NSSet *set_products = [NSSet setWithArray: arr_products];
        [[RMStore defaultStore] requestProducts:set_products success:^(NSArray *products, NSArray *invalidProductIdentifiers) {
            NSLog(@"InAppApple::init|Products loaded");
            for (SKProduct * product in products)
            {
                NSLog(@"InAppApple::init|Loaded product: %@", product.productIdentifier);
                
                InAppProduct iap;
                
                if ([product localizedDescription])
                {
                    iap.description = [[product localizedDescription] UTF8String];
                }
                
                if ([product localizedTitle])
                {
                    iap.title = [[product localizedTitle] UTF8String];
                }
                
                iap.price = [[product price] doubleValue];
                iap.product_id = [[product productIdentifier] UTF8String];

                NSNumberFormatter *number_formatter = [[NSNumberFormatter new] autorelease];
                [number_formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [number_formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                [number_formatter setLocale:product.priceLocale];

                iap.price_str       = [[number_formatter stringFromNumber:[product price]] UTF8String];
                iap.currency_code   = [[NSString stringWithFormat:@"%@", [product.priceLocale objectForKey:NSLocaleCurrencyCode]] UTF8String];
                iap.currency_symbol = [[NSString stringWithFormat:@"%@", [product.priceLocale objectForKey:NSLocaleCurrencySymbol]] UTF8String];
                
                if (iap.currency_code.empty())
                {
                    iap.currency_code = [[NSString stringWithFormat:@"%@", [product.priceLocale objectForKey:NSLocaleCurrencySymbol]] UTF8String];
                }
                
                NSLog(@"InAppApple::init|_products.push_back(iap)");

                _products.push_back(std::move(iap));
            }
            
            if (_listener)
            {
                NSLog(@"InAppApple::init|onProductRequestSuccess");

                _listener->onProductRequestSuccess(_products);
            }
            
            NSLog(@"InAppApple::init|end");

        } failure:^(NSError *error) {
            NSLog(@"InAppApple::init|Something went wrong %@", error.localizedDescription);
        }];
    }

    void InAppApple::purchase(const std::string product_id) {
        [[SKPaymentQueue defaultQueue] removeTransactionObserver:[StoreKitHelper sharedStoreHelper]];
        NSString * productId = [NSString stringWithUTF8String: product_id.c_str()];
        [[RMStore defaultStore] addPayment:productId success:^(SKPaymentTransaction *transaction) {
            NSLog(@"Purchased");

            InAppProduct iap;
            iap.product_id = [[[transaction payment] productIdentifier] UTF8String];

            if (_listener) {
                _listener->onSuccess(iap);
            }

//            auto receipt = MJ_PLATFORM_CONFIG.getReceipt();

        } failure:^(SKPaymentTransaction *transaction, NSError *error) {
            NSLog(@"Cannot purchase %@", error.localizedDescription);

            InAppProduct iap;
//            iap.product_id = [[[transaction payment] productIdentifier] UTF8String];

            _listener->onFailed(iap);
        }];
    }

    void InAppApple::restore() {
        [[RMStore defaultStore] restoreTransactionsOnSuccess:^(NSArray *transactions) {
            _listener->onRestoreComplete(true, "");
        } failure:^(NSError *error) {
            _listener->onRestoreComplete(false, "");
        }];
    }

    void InAppApple::refresh(const std::vector<std::string> &ids) {
        id arr_products = [NSMutableArray new];
        std::for_each(ids.begin(), ids.end(), [arr_products](std::string s) {
            id nss = [NSString stringWithUTF8String:s. c_str()];
            [arr_products addObject:nss];
        });
        NSSet *products = [NSSet setWithArray: arr_products];
        [[RMStore defaultStore] requestProducts:products success:^(NSArray *products, NSArray *invalidProductIdentifiers) {
            NSLog(@"Products loaded");
            for (SKProduct * product in products) {
                InAppProduct iap;
                if ([product localizedDescription]) {
                    iap.description = [[product localizedDescription] UTF8String];
                }
                if ([product localizedTitle]) {
                    iap.title = [[product localizedTitle] UTF8String];
                }
                iap.price = [[product price] doubleValue];
                iap.product_id = [[product productIdentifier] UTF8String];

                NSNumberFormatter * number_formatter = [NSNumberFormatter new];
                [number_formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [number_formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                [number_formatter setLocale:product.priceLocale];

                std::string formated_price = [[number_formatter stringFromNumber:[product price]] UTF8String];

                NSLog(@"In app price: %@", [NSString stringWithUTF8String:formated_price.c_str()]);
                _products.push_back(iap);
            }

            if (_listener) {
                _listener->onProductRequestSuccess(_products);
            }        } failure:^(NSError *error) {
            NSLog(@"Something went wrong %@", error.localizedDescription);
        }];
    }

    std::string InAppApple::getReceipt(const std::string & product_id) {
        NSURL * receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
        NSData * receipt = [NSData dataWithContentsOfURL:receiptURL];
        if (!receipt) {
            //không có receipt, người dùng không trong subscription
            return "";
        }

        auto str = std::string([[receipt base64EncodedStringWithOptions:0] UTF8String]);

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        cocos2d::FileUtils::getInstance()->writeStringToFile(str, cocos2d::FileUtils::getInstance()->getWritablePath() + "receipt.txt");
#endif
        return str;
    }

//    void InAppApple::payWithOnePay(std::string product_id, std::string coupon) {
//        auto url = mj::Request::getOnePayURL(product_id);
//
//        CCLOG("onepay url: %s", url.c_str());
//
//        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
//        if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
//            [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
//        }
//
//        if (NSClassFromString(@"SFSafariViewController")) {
//            SFSafariViewController * svc = [[SFSafariViewController alloc] initWithURL:[NSURL URLWithString:[NSString stringWithUTF8String: url.c_str()]]];
//            [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:svc animated:YES completion:nil];
//        } else {
//            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
//        }
//    }
}

#endif

