//
//  MJInApp-Win32.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#include "MJInApp-Win32.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "MJPlatformConfig.h"
#include "MJRequest.h"
#include "StoryConfigManager.h"

/*
 Name: %s -- ID: %s -- \
 Price: %f -- \
 Receipt: %s -- \
 Receipt Cipher: %s -- \
 Transaction ID: %s",
 p.name.c_str(),
 p.id.c_str(),
 p.priceValue,`
 p.receipt.c_str(),
 p.receiptCipheredPayload.c_str(),
 p.transactionID.c_str());
 */

namespace mj {
    void InAppWin32::init(const std::vector<std::string> & ids) {
        
    }

    void InAppWin32::purchase(const std::string product_id) {
        
    }

    void InAppWin32::restore() {
        
    }

    void InAppWin32::refresh(const std::vector<std::string> &ids) {
        
    }

    std::string InAppWin32::getReceipt(const std::string & product_id) {
        return "";
    }

//    void InAppWin32::payWithOnePay(std::string product_id, std::string coupon) {
//        
//    }
}

#endif

