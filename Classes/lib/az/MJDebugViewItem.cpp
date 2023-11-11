//
// Created by To Ngoc Linh on 9/18/17.
//

#include "MJDebugViewItem.h"
#include "MJDefault.h"

USING_NS_CC;
INITIALIZE_READER(MJDebugViewItem);

std::once_flag mj_debug_view_item_reader;

MJDebugViewItem *MJDebugViewItem::createItem(DebugInfo info) {
    std::call_once(mj_debug_view_item_reader, [] {
        REGISTER_CSB_READER(MJDebugViewItem);
    });

    auto item = static_cast<MJDebugViewItem *>(CSLoader::createNode("csb/debug/MJDebugViewItem.csb"));
    if (item) {
        item->didLoadFromCSB(info);
        return item;
    }

    CC_SAFE_DELETE(item);
    return nullptr;
}

void MJDebugViewItem::didLoadFromCSB(DebugInfo info) {
    _info = info;
    utils::findChild<ui::Text *>(this, "title")->setString(_info.title);
    utils::findChild<ui::Text *>(this, "description")->setString(_info.description);
    auto saved_value = MJDEFAULT->getBoolForKey(_info.name, false);
    utils::findChild<ui::CheckBox *>(this, "checkbox")->setEnabled(true);
    utils::findChild<ui::CheckBox *>(this, "checkbox")->setSelected(saved_value);
}

void MJDebugViewItem::applyValue() {
    MJDEFAULT->setBoolForKey(_info.name, utils::findChild<ui::CheckBox *>(this, "checkbox")->isSelected());
    if (_info.name == "debugkey_fake_device_id") {
        if (utils::findChild<ui::CheckBox *>(this, "checkbox")->isSelected()) {
            auto fake = std::string("|") + StringUtils::toString(random(0, 2000000000)) + "|" +  StringUtils::toString(random(0, 2000000000));
            CCLOG("fake id %s", fake.c_str());
            MJDEFAULT->setStringForKey("fake_id", fake);
        } else {
            MJDEFAULT->setStringForKey("fake_id", "");
        }
    }
}
