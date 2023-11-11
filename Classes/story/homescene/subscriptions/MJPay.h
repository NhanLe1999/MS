//
//  MJPay.h
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#ifndef MJPay_hpp
#define MJPay_hpp

#include <stdio.h>
#include "MJRequest.h"
#include "MJInAppInfo.h"
#include "MJInApp.h"
#include "StoryAlert.h"
#include "IAPManager.h"

namespace mj {

    class RequestPriceDelegate {
    public:
        virtual void onGetPriceInfo(mj::ListPackages packages) {}
    };

    class ActiveLicenseDelegate {
    public:
        virtual void onActiveLicenseSuccess(const std::string & license, const int & time_expire, const std::string & message, rapidjson::Value data) {}
        virtual void onActiveLicenseFailed(const std::string & license, const int & error_code, const std::string & error, rapidjson::Value data) {}
    };

    class ValidateCouponDelegate {
    public:
        virtual void onValidateCouponSuccess(const std::string & coupon, const float & discount_percent) {}
        virtual void onValidateCouponFailed(const std::string & coupon, const std::string & error) {}
    };

    class Pay : public mj::RequestDelegate, public mj::InAppListener, public StoryAlertDelegate {
    public:
        static Pay & getInstance() {
            static Pay shared_pay;
            return shared_pay;
        }

        void updateActiveStatus(bool is_online, bool is_active = false);

        //lấy thông tin về các gói mua bán
        void refresh(std::string json = "");

        //thanh toán qua cổng OnePay
        void payWithOnePay(std::string product_id);

        //thanh toán theo hình thức COD
        void payWithCOD();

        //thanh toán theo hình thức chuyển khoản ngân hàng
        void payWithBankTransfer();

        //thanh toán qua AppStore/Play Store
        void payWithStore(const std::string & product_id, const int & _discount);

        //xác thực thanh toán
        void verify(std::string product_id = "", std::string token = "");

        //truy vấn thông tin coupon
        void setCoupon(const std::string & coupon);

        //kích hoạt license key
        void enterLicense(const std::string & license, bool is_confirm = false);

        //lấy thông tin các gói giá
        mj::ListPackages getPackages();
        void setPackages(std::unordered_map<std::string, InAppPackageModel> packages);

        //restore transaction
        void restore();

        void forceOverrideLicense(const std::string & license, const int & time_expire);

#pragma mark Getters
        bool isActive();
        inline std::string getLicense() { return _license; }
        inline std::string getCoupon() { return _coupon; }
        inline int getDiscountPercent() { return _discount_percent; }
        inline int getTimeExpire() { return _expire_time; }

    protected:
        explicit Pay();

    private:
        Pay(Pay const &);
        void operator = (Pay const &);

        mj::ListPackages _packages;

        std::string _coupon;
        int _discount_percent;

        std::string _license;
        int _expire_time;

        bool _is_active = false;

        mj::Request * _request_license;
        mj::Request * _request_coupon;
        mj::Request * _request_verify;
        mj::Request * _request_price_info;

        std::string _temp_license;
        std::string _temp_coupon;
        std::string _temp_receipt;

        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;

        void parsePackagesFromData(rapidjson::Value data);
//        void onRequestPriceInfoFailed();
//        void onRequestPriceInfoSuccess(rapidjson::Value data);

    public:
        void onProductRequestSuccess(const std::vector<InAppProduct> & products) override;
        void onSuccess(const InAppProduct & product) override;
        void onFailed(const InAppProduct & product) override;

    private:
        void onVerifySuccess(rapidjson::Value data);
        void onVerifyFailed(std::string = "");

        void onActiveLicenseSuccess(rapidjson::Value data);
        void onActiveLicenseFailed(const int & error_code, const std::string & error, rapidjson::Value data);

        void onValidateCouponSuccess(rapidjson::Value data);
        void onValidateCouponFailed(const std::string & error);

        void onRestoreComplete(bool ok, const std::string &message);

        void onAlertOK(cocos2d::Ref * sender);
        void onAlertCancel(cocos2d::Ref * sender);

        bool _should_verification_notify_ui;

    CC_SYNTHESIZE(RequestPriceDelegate *, _price_delegate, PriceDelegate);
        CC_SYNTHESIZE(ActiveLicenseDelegate *, _license_delegate, ActiveLicenseDelegate);
        CC_SYNTHESIZE(ValidateCouponDelegate *, _coupon_delegate, ValidateCouponDelegate);
    };
}

#endif /* MJPay_hpp */
