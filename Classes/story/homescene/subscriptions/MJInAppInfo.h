//
//  MJInAppInfo.h
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJInAppInfo_h
#define MJInAppInfo_h

#include <vector>
#include <string>
#include <cstdlib>
#include "StoryConfigManager.h"

namespace mj {

    struct AndroidTransaction {
        std::string orderId;
        std::string productId;
        long purchaseTime;
        std::string purchaseState;
        std::string purchaseToken;
        bool autoRenew;

        AndroidTransaction(): orderId(""), productId(""), purchaseTime(0), purchaseState(""), purchaseToken(""), autoRenew(false) {

        };

        AndroidTransaction(std::vector<std::string> details) {
            for (auto &prop : details) {
                if (prop == "null") {
                    prop = "";
                }
            }

            orderId = details[0];
            productId = details[1];
            purchaseTime = std::atol(details[2].c_str());
            purchaseState = details[3];
            purchaseToken = details[4];
            autoRenew = (details[5] == "yes");
        }
    };

    struct InAppPackage {
        int weight = 0;
        std::string product_id = "";
        std::string product_name = "";
        std::string product_id_store = "";
        std::string price_string = "";
		std::string currency = "";

        double price_original = 0;
        double price_30 = 0;
        double price_40 = 0;

        double price_store_30 = 0;
        double price_store_40 = 0;
        std::string store_currency_code = "";

        double getActualPrice(std::string coupon) {
            if (coupon == "30OFF") {
                return price_30;
            }
            if (coupon == "") {
                return price_40;
            }
            return price_40;
        };

        double getStorePrice(int discount) {
            if (discount == 30) {
                return price_store_30;
            }
            if (discount == 40) {
                return price_store_40;
            }
            return price_store_30;
        }

        std::string getStoreCurrency() {
            return store_currency_code;
        }

        double getActualPrice(int discount) {
//            assert((discount == 0 || discount == 30 || discount == 40) && "Chỉ chấp nhận mức giảm giá 30% hoặc 40%");
            if (discount == 30) {
                return price_30;
            }

            if (discount == 40) {
                return price_40;
            }

            return price_original;
        }

        std::string getProductIDStore(int discount) {
            if (CONFIG_MANAGER->isAppVNStories()) {
                auto pid = product_id_store;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                if (pid.find("lifetime") != std::string::npos) {
                    return "com.vmonkey.lifetime";
                }
                if (pid.find("1year") != std::string::npos) {
                    return "com.vmonkey.1year";
                }
                if (pid.find("6month") != std::string::npos) {
                    return "com.vmonkey.6month";
                }
                if (pid.find("1month") != std::string::npos) {
                    return "com.vmonkey.1month";
                }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                if (pid.find("lifetime") != std::string::npos) {
                    return "new.earlystart.stories.vn.lifetime.40_off";
                }
                if (pid.find("1year") != std::string::npos) {
                    return "new.earlystart.stories.vn.1year.40_off";
                }
                if (pid.find("6month") != std::string::npos) {
                    return "new.earlystart.stories.vn.6month.40_off";
                }
                if (pid.find("1month") != std::string::npos) {
                    return "new.earlystart.stories.vn.1month.40_off";
                }
#else
                if (pid.find("lifetime") != std::string::npos) {
                    return "mac.new.earlystart.stories.vn.lifetime.40_off";
                }
                if (pid.find("1year") != std::string::npos) {
                    return "mac.new.earlystart.stories.vn.1year.40_off";
                }
                if (pid.find("6month") != std::string::npos) {
                    return "mac.new.earlystart.stories.vn.6month.40_off";
                }
                if (pid.find("1month") != std::string::npos) {
                    return "mac.new.earlystart.stories.vn.1month.40_off";
                }
#endif
            }
            auto pid = product_id_store;
            if (discount == 30) {
                return pid + ".30_off";
            }
            if (discount == 40) {
                return pid + ".40_off";
            }

            return pid;
        }
    };

    typedef std::vector<mj::InAppPackage> ListPackages;

//    static const ListPackages BUY_PACKAGES = {
//        {1, "com.earlystart.stories.1month", "Một tháng", "test.monkeyjunior.stories.1month.auto"},
//        {2, "com.earlystart.stories.1year", "Một năm", "test.earlystart.stories.1year"},
//        {3, "com.earlystart.stories.lifetime", "Trọn đời", "test.earlystart.stories.lifetime"},
//    };
}

#endif /* MJInAppInfo_h */
