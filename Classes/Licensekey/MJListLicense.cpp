//
//  MJListLicense.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 10/9/18.
//

#include "MJListLicense.h"
#include "MJLicensePackage.h"
#include "LTDate.h"

USING_NS_CC;
INITIALIZE_READER(MJListLicense);

std::once_flag mj_list_license_reader_flag;

MJListLicense * MJListLicense::createView(std::vector<license::Package> packages) {
    std::call_once(mj_list_license_reader_flag, [] {
        REGISTER_CSB_READER(MJListLicense);
    });
    
    auto view = reinterpret_cast<MJListLicense *>(CSLoader::createNodeWithVisibleSize("csb/licensekey/MJListLicense.csb"));
    if (view) {
        view->didLoadFromCSB(packages);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}
#include "MJHelper.h"
#include "StoryDataModel.h"
#include "MJPlatformConfig.h"
void MJListLicense::didLoadFromCSB(std::vector<license::Package> packages) {
    _packages = StoryDataModel::getInstance()->getLicensePackages();
    
    auto current_device_name = std::string("N/A");
//    auto devices = DataSQL::getInstance()->getLicenseDeviceInfoOfLicense("32gha-d903d");
    auto devices = StoryDataModel::getInstance()->getLicenseDeviceInfo();
    auto it = std::find_if(devices.begin(), devices.end(), [](license::LicenseDeviceInfo d) {
        return d.device_id == std::atoi(MJ_PLATFORM_CONFIG.getDeviceId().c_str());
    });
    if (it != devices.end()) {
        current_device_name = it->device_name;
    }
    
    auto title = utils::findChild<ui::Text *>(this, "title_lb");
    auto str = title->getString();
    str = mj::helper::replace_string(str, "{number_license}", StringUtils::toString(_packages.size()));
    str = mj::helper::replace_string(str, "{device_name}", current_device_name);
    title->setString(str);
    
    auto listview_packages = utils::findChild<ui::ListView *>(this, "list_packages");
    auto model = utils::findChild<ui::Layout *>(this, "cell_license");
    
    for (auto i = 0; i < _packages.size(); ++i) {
        auto p = _packages[i];
        auto item = model->clone();
        
        auto order = utils::findChild<ui::Text *>(item, "stt");
        order->setString(StringUtils::toString(i + 1));
        
        auto name = utils::findChild<ui::Text *>(item, "package_name");
        name->setString(p.name);
        
        auto license = utils::findChild<ui::Text *>(item, "license");
        license->setString(p.license);
        
//        auto time_enter = utils::findChild<ui::Text *>(item, "date_enter");
//        time_enter->setString(LTDate(p.time_enter).toPrettyStringShort());
//
//        auto time_effect = utils::findChild<ui::Text *>(item, "date_effective");
//        if (p.time_effective != -1) {
//            time_effect->setString(LTDate(p.time_effective).toPrettyStringShort());
//        } else {
//            time_effect->setString("Chưa\nxác định");
//        }
        
        auto time_expiry = utils::findChild<ui::Text *>(item, "date_expire");
        if (p.time_expiry != -1) {
            time_expiry->setString(LTDate(p.time_expiry).toPrettyString());
            if (p.time_expiry > mj::helper::getTime()) {
                utils::findChild<ui::Text *>(item, "status")->setString("Đang sử dụng");
            } else {
                if (p.time_expiry == 0) {
                    utils::findChild<ui::Text *>(item, "status")->setString("Đang sử dụng");
                    time_expiry->setString("Trọn đời");
                } else {
                    utils::findChild<ui::Text *>(item, "status")->setString("Đã hết hạn");
                    utils::findChild<ui::Button *>(item, "btremove")->setVisible(false);
                }
            }
        } else {
            time_expiry->setString("Chưa\nxác định");
            utils::findChild<ui::Text *>(item, "status")->setString("Chưa sử dụng");
        }
        
        auto bt_remove = utils::findChild(item, "btremove");
        if (bt_remove)
            bt_remove->setName(p.license);
        
        listview_packages->pushBackCustomItem(item);
    }
    
    if (_packages.size() < 4) {
        listview_packages->setScrollBarEnabled(false);
    }
}

void MJListLicense::onEnter() {
    ui::Layout::onEnter();
}

#include "MJListDeviceView.h"
#include "GlobalFunc.h"
void MJListLicense::onManage(Ref * sender) {
    auto name = reinterpret_cast<Node *>(sender)->getName();
    CCLOG("license: %s", name.c_str());
    auto devices = StoryDataModel::getInstance()->getLicenseDeviceInfoOfLicense(name);
    auto license = LICENSE_INFO_MODEL->getLicenseByLicenseKey(name);
    Director::getInstance()->getRunningScene()->addChild(MJListDeviceView::createView(devices, std::atoi(MJ_PLATFORM_CONFIG.getDeviceId().c_str()), license, StringUtils::format("%d/%d", license.current_active, license.total_active)), this->getLocalZOrder() + 1);
}

void MJListLicense::onClose(Ref * sender) {
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MJListLicense::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJListLicense, onClose),
        CLICK_MAP(MJListLicense, onManage)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
