//
//  MSFacebookPlugin.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/28/18.
//

#ifndef MSFacebookPlugin_hpp
#define MSFacebookPlugin_hpp

#include "cocos2d.h"
#include "MSAnalyticsPlugin.h"

namespace ms { namespace analytics {
    class Facebook : public ms::AnalyticsPlugin {
    public:
        static Facebook & getInstance() {
            static Facebook sharedFacebook;
            return sharedFacebook;
        };
        
        void setIdentifier(std::string identifier);
        void setUserProperties(std::map<std::string, cocos2d::Value> props);
        void logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> parameters, bool doPushClevertap = false);
        
    protected:
        explicit Facebook();
        
    private:
        Facebook(Facebook const &);
        void operator = (Facebook const &);
    };
}}

#endif
