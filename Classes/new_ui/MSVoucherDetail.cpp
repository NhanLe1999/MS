//
//  MSVoucherDetail.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/11/19.
//

#include "MSVoucherDetail.h"

USING_NS_CC;

INITIALIZE_READER(MSVoucherDetail);

std::once_flag ms_voucher_detail_reader_flag;

MSVoucherDetail * MSVoucherDetail::createView() {
    std::call_once(ms_voucher_detail_reader_flag, [] {
        REGISTER_CSB_READER(MSVoucherDetail);
    });
    
    auto node = reinterpret_cast<MSVoucherDetail *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/Voucher_Detail.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSVoucherDetail::didLoadFromCSB() {
    
}

void MSVoucherDetail::onEnter() {
    ui::Layout::onEnter();
}

void MSVoucherDetail::onExit() {
    ui::Layout::onExit();
}

void MSVoucherDetail::onClose(Ref * sender) {
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MSVoucherDetail::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSVoucherDetail, onClose)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

