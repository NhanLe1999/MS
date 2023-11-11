//
//  MJListLicense.hpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#ifndef MJListLicense_hpp
#define MJListLicense_hpp

#include "cocostudiomacro2.h"
#include "MJLicensePackage.h"

namespace license {
    struct Package;
}

class MJListLicense : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJListLicense);
    static MJListLicense * createView(std::vector<license::Package> packages);
    
private:
    void didLoadFromCSB(std::vector<license::Package> packages);
    void onEnter() override;
    
    void onClose(cocos2d::Ref * sender);
    void onManage(cocos2d::Ref * sender);
    
    std::vector<license::Package> _packages;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MJListLicense);

#endif /* MJListLicense_hpp */
