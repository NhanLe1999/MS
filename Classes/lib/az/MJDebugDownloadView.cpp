//
//  MJDebugDownloadView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/26/17.
//

#include "MJDebugDownloadView.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryInfoCover.h"
#include "MSServerManager.h"

USING_NS_CC;
INITIALIZE_READER(MJDebugDownloadView);

std::once_flag mj_debug_download_view_reader;

MJDebugDownloadView * MJDebugDownloadView::createView() {
    std::call_once(mj_debug_download_view_reader, [] {
        REGISTER_CSB_READER(MJDebugDownloadView);
    });
    
    auto view = static_cast<MJDebugDownloadView *>(CSLoader::createNodeWithVisibleSize("csb/debug/MJDebugDownloadView.csb"));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MJDebugDownloadView::didLoadFromCSB() {
    ms::ServerManager::getInstance().reset();
//    auto saved_servers_str = MJDEFAULT->getStringForKey(key_server_id_download, "");
    auto saved_servers = ms::ServerManager::getInstance().getServers();
    
    auto model = utils::findChild<ui::Layout *>(this, "model");
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    for (auto server : saved_servers) {
        auto item = model->clone();
        auto checkbox = item->getChildByName<ui::CheckBox *>("checkbox");
        checkbox->setSelected(false);
        utils::findChild<ui::Text *>(item, "title")->setString(StringUtils::toString(server));
        listview->pushBackCustomItem(item);
    }
}

ui::Widget::ccWidgetClickCallback MJDebugDownloadView::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJDebugDownloadView, onDownload),
        CLICK_MAP(MJDebugDownloadView, onClose),
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void MJDebugDownloadView::onDownload(Ref *) {
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    std::vector<int> servers;
    
    for (auto item : listview->getItems()) {
        auto checkbox = item->getChildByName<ui::CheckBox *>("checkbox");
        auto name = ATOI(item->getChildByName<ui::Text *>("title")->getString().c_str());
        if (checkbox->isSelected()) {
            servers.push_back(name);
        }
    }
    
    if (servers.empty()) {
        cocos2d::MessageBox("Hãy chọn server đi người đẹp!", "SERVER");
        return;
    }
    
    auto str = mj::helper::vectorIntToString(servers);
    
    ms::ServerManager::getInstance().setServerIds(servers);
    
    auto parent = dynamic_cast<StoryInfoCover *>(this->getParent());
    if (parent) {
        parent->startDownload();
    }
    this->removeFromParent();
}

void MJDebugDownloadView::onClose(Ref *) {
    this->removeFromParent();
}
