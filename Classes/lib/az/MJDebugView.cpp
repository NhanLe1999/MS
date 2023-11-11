//
// Created by To Ngoc Linh on 9/18/17.
//

#include "MJDebugView.h"
#include "MJDebugViewItem.h"
#include "MJPlatformConfig.h"
#include "Key_Default_Model.h"
#include "UtilitySqlite.h"
USING_NS_CC;
INITIALIZE_READER(MJDebugView);

std::once_flag mj_debug_view_reader;
const std::vector<DebugInfo> DEBUG_ITEMS = {
    {
        debugkey_fake_device_id,
        "Fake Device ID",
        "Tạo Device ID mới",
        cocos2d::Value(false)
    },
    {
        debugkey_auto_story,
        "Auto story",
        "tải truyên tự động",
        cocos2d::Value(false)
    },
    {
        debugkey_auto_audiobook,
        "Auto Audiobook",
        "Nghe audiobook tu dong",
        cocos2d::Value(false)
    },
    {
        debugkey_skip_story,
        "Skip story",
        "Bỏ qua truyện",
        cocos2d::Value(false)
    },
    {
        debugkey_auto_play,
        "Auto Play",
        "Kích hoạt auto play",
        cocos2d::Value(false)
    },
    {
        debugkey_language_vn,
        "Vietnamese Stories",
        "Đọc các truyện Việt Nam",
        cocos2d::Value(false)
    },
    {
        debugkey_all_lessons,
        "Enable all lessons",
        "Học tất cả các bài học",
        cocos2d::Value(false)
    },
    {
        debugkey_dev_environment,
        "Test Environment",
        "Đổi sang môi trường test (dev.monkeyuni, api.dev.monkeyuni)",
        cocos2d::Value(false)
    },
    {
        debugkey_product_environment,
        "Product Environment",
        "Đổi sang môi trường product (daybehoc, api.daybehoc)",
        cocos2d::Value(false)
    },
    {
        debugkey_test_free,
        "Test Free Mode",
        "Dùng truyện theo chế độ Free (chưa kích hoạt)",
        cocos2d::Value(false)
    },
    {
        debugkey_full_features,
        "Full Features",
        "Kích hoạt đầy đủ các tính năng",
        cocos2d::Value(false)
    },
    {
        debugkey_country_gb,
        "Global device",
        "Giả lập thiết bị nước ngoài",
        cocos2d::Value(false)
    },
    {
        debugkey_license_disable,
        "Disable License Mode",
        "Tắt mục license trong trang Parents",
        cocos2d::Value(false)
    },
    {
        debugkey_test_config,
        "Config Story",
        "Bật chế độ config trong truyện",
        cocos2d::Value(false)
    },
    {
        debugkey_homescreen_landscape,
        "Landscape Orientation",
        "Giả lập thiết bị màn hình ngang",
        cocos2d::Value(false)
    },
    {
        debugkey_quick_play_game,
        "Quick Game Mode",
        "Chế độ đọc 1 trang rồi chơi game",
        cocos2d::Value(false)
    },
    {
        debugkey_test_7_days,
        "Lesson Mode",
        "Hiển thị tab lesson",
        cocos2d::Value(false)
    },
    {
        debugkey_fake_free_time,
        "Fake Free Time",
        "Tắt/Bật để đổi ngày",
        cocos2d::Value(false)
    },
    {
        debugkey_fake_ipx,
        "Fake iPhone X",
        "Giả lập màn hình iPhone X",
        cocos2d::Value(false)
    },
    {
        debugkey_is_test_license,
        "Test License",
        "License sẽ hết hạn sau 10'",
        cocos2d::Value(false)
    },
    {
        debugkey_test_act_vmonkey,
        "Test act Vmonkey",
        "test act sau truyện thông qua lesson",
        cocos2d::Value(false)
    }
};

MJDebugView *MJDebugView::createView() {
    std::call_once(mj_debug_view_reader, [] {
        REGISTER_CSB_READER(MJDebugView);
    });

    auto view = static_cast<MJDebugView *>(CSLoader::createNodeWithVisibleSize("csb/debug/MJDebugView.csb"));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);
    return nullptr;
}

void MJDebugView::didLoadFromCSB() {
    auto overlay = this->getChildByName<ui::Layout *>("Panel_4");
    overlay->setTouchEnabled(true);
    overlay->addClickEventListener([this](Ref *) {
        this->removeFromParent();
    });
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    for (auto info : DEBUG_ITEMS) {
        auto item = MJDebugViewItem::createItem(info);
        listview->pushBackCustomItem(item);
    }

    this->enumerateChildren("//.*", [](Node * n) {
        return false;
    });
}

cocos2d::ui::Widget::ccWidgetClickCallback MJDebugView::onLocateClickCallback(const std::string &name) {
    if (name == "onReset") {
        return CC_CALLBACK_1(MJDebugView::onReset, this);
    }
    return nullptr;
}

#include "StoryDataModel.h"
#include "MJDefault.h"
#include "APDatabase.h"
void MJDebugView::onReset(Ref *) {
    CCLOG("reset...");
    auto user_defaults_xml = cocos2d::UserDefault::getInstance()->getXMLFilePath();

    if (FileUtils::getInstance()->removeFile(user_defaults_xml)) {
        CCLOG("Removed User Defaults XML %s", user_defaults_xml.c_str());
    } else {
        CCLOG("Cannot remove %s", user_defaults_xml.c_str());
    }
    ap::Database::getInstance().dropDatabase();
    StoryDataModel::getInstance()->dropDatabase(Director::getInstance()->getRunningScene());

    mj::PlatformConfig::getInstance().clearUserDefaults();

    auto writable_path = FileUtils::getInstance()->getWritablePath();
//    writable_path.resize(writable_path.length() - 1);
    FileUtils::getInstance()->removeDirectory(writable_path + image_cache_path);
    FileUtils::getInstance()->removeDirectory(writable_path + "mjstory/");
    FileUtils::getInstance()->removeDirectory(writable_path + "audiobooks/");
    FileUtils::getInstance()->removeDirectory(writable_path + "cached/");
    FileUtils::getInstance()->removeDirectory(writable_path + "extracted/");
    FileUtils::getInstance()->removeDirectory(writable_path + "games/");

	MJDEFAULT->setStringForKey("use_url", "");
	UtilitySqlite::shareInstance()->deleteData("mjdefault");
	StoryDataModel::getInstance()->removeAllStories();

	UtilitySqlite::shareInstance()->onCloseDatabase();
	StoryDataModel::getInstance()->closeConnect();

    FileUtils::getInstance()->removeFile(writable_path + "database.sqlite");
    FileUtils::getInstance()->removeFile(writable_path + "mjdefault.sqlite");
    FileUtils::getInstance()->removeFile(writable_path + "ap.sqlite");

	UtilitySqlite::shareInstance()->encryptData();

    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    for (auto item : listview->getItems()) {
        auto debug_item = dynamic_cast<MJDebugViewItem *>(item);
        if (debug_item) {
            debug_item->applyValue();
        }
    }

    cocos2d::UserDefault::getInstance()->flush();

    auto button = utils::findChild<ui::Button *>(this, "button");
    button->setEnabled(false);

    button->runAction(ActionFloat::create(5.f, 5.f, 0.f, [button] (float value) {
        button->setTitleText(StringUtils::format("%0.2fs", value));
    }));

    this->scheduleOnce([](float) {
        exit(0);
    }, 5.1f, "delayexit");
}
