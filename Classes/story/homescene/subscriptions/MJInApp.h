//
//  MJInApp.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#ifndef MJInApp_hpp
#define MJInApp_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "cocos2d.h"

namespace mj {
    struct InAppProduct {
        std::string product_id;
        std::string description;
        std::string title;
        std::string receipt;
        double price;
        std::string price_str;
        std::string currency_code;
        std::string currency_symbol;
    };

    class InAppListener {
    public:
        virtual void onInitialized() {};
        virtual void onSuccess(const InAppProduct & product) {};
        virtual void onFailed(const InAppProduct & product) {};
        virtual void onCanceled(const InAppProduct & product) {};
        virtual void onRestored(const InAppProduct & product) {};
        virtual void onProductRequestSuccess(const std::vector<InAppProduct> & products) {};
        virtual void onProductRequestFailure(const std::string & message) {};
        virtual void onRestoreComplete(bool ok, const std::string &message) {};
    };

    class InApp {
    public:
        static class InApp & getInstance();

        virtual void init(const std::vector<std::string> & ids) {}
        virtual void purchase(const std::string product_id) {}
        virtual void restore() {}
        virtual void refresh(const std::vector<std::string> & ids) {}

        virtual void payWithOnePay(std::string product_id, std::string coupon);
        virtual std::string getReceipt(const std::string & product_id) {return "";}

    protected:
        explicit InApp() {};

    private:
        InApp(InApp const &);
        void operator = (InApp const &);
        CC_SYNTHESIZE(mj::InAppListener *, _listener, Listener);

        std::vector<InAppProduct> _products;
    };

}

#endif /* MJInApp_hpp */
