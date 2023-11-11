//
//  Coop.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#include "Coop.h"
#include "MJHelper.h"
#include "GAFWrapper.h"

INITIALIZE_READER(MJCoop);

USING_NS_CC;

MJCoop * MJCoop::createCoop(mj::GameObject word, int slot) {
    auto coop = static_cast<MJCoop *>(CSLoader::createNode("csb/game/chickens_to_coop/coop.csb"));
    if (coop) {
        coop->didLoadFromCSB(word, slot);
    }

    return coop;
}

void MJCoop::didLoadFromCSB(mj::GameObject word, int slot) {
    _word = word;

    auto row = ui::Helper::seekWidgetByName(this, "row_1");
    for (auto c : row->getChildren()) {
        _doors.push_back(static_cast<ui::ImageView *>(c));
    }

    if (slot == 3) {
        ui::Helper::seekWidgetByName(this, "row_2")->removeFromParent();
    } else {
        row = ui::Helper::seekWidgetByName(this, "row_2");
        for (auto c : row->getChildren()) {
            _doors.push_back(static_cast<ui::ImageView *>(c));
        }
    }

    mj::helper::shuffle(_doors);

    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    text->setString(_word.text);
    if (CONFIG_MANAGER->isGameVMonkey()){
        text->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
    }
}

void MJCoop::putChicken() {
    auto door = * _doors.begin();
    auto gaf = GAFWrapper::createGAF("gaf/chicken_in_coop/ga_chuong.gaf", door);
    gaf->play(false);

    _doors.erase(_doors.begin());
}

bool MJCoop::isFull() {
    return _doors.empty();
}
