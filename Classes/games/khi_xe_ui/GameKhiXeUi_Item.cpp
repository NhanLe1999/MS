
#include "GameKhiXeUi_Item.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(GameKhiXeUi_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/khi_xe_ui/GameKhiXeUiItem.csb"

GameKhiXeUi_Item * GameKhiXeUi_Item::createGame(std::vector<ItemInfo> item_infos) {
    auto p = static_cast<GameKhiXeUi_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_infos);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

#include "MSGradeManager.h"
void GameKhiXeUi_Item::didLoadFromCSB(std::vector<ItemInfo> item_infos) {
	auto is_phonic_lesson = ms::GradeManager::getInstance().getScope() == ms::GradeManager::LESSONS_PHONIC;
	_item_infos = item_infos;
	//this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	if (item_infos.size() == 4){
		mainLayout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type1");
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type2")->removeFromParent();
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type3")->removeFromParent();
		house_type = HOUSE_TYPE::HT_SMALL;
		if (is_phonic_lesson) {
			img_crash_path = "games/khi_xe_ui/khixeui_6_b.png";
		}
		else {
			img_crash_path = "games/khi_xe_ui/khixeui_no4.png";
		}
		
	}
	else if (item_infos.size() == 6){
		mainLayout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type3");
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type2")->removeFromParent();
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type1")->removeFromParent();
		house_type = HOUSE_TYPE::HT_MEDIUM;
		if (is_phonic_lesson) {
			img_crash_path = "games/khi_xe_ui/khixeui_5_s.png";
		}
		else {
			img_crash_path = "games/khi_xe_ui/khixeui_no9.png";
		}
	}
	else {
		mainLayout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type2");
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type1")->removeFromParent();
		cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "Type3")->removeFromParent();
		house_type = HOUSE_TYPE::HT_BIG;
		if (is_phonic_lesson) {
			img_crash_path = "games/khi_xe_ui/khixeui_5_s.png";
		}
		else {
			img_crash_path = "games/khi_xe_ui/khixeui_no9.png";
		}
	}
	mainLayout->setVisible(true);
	int min_font_size = 100;
	for (int i = 0; i < _item_infos.size(); i++){
		std::string item_name = StringUtils::format("%s%d", "item", i + 1);
		_item_infos.at(i).aLayout = static_cast<cocos2d::ui::Layout*>(mainLayout->getChildByName(item_name));
		_item_infos.at(i).aLayout->setCascadeOpacityEnabled(false);
		_item_infos.at(i).aLayout->setOpacity(0);
		_item_infos.at(i).textItem = _item_infos.at(i).aLayout->getChildByName<cocos2d::ui::Text*>("text");
		_item_infos.at(i).textItem->setString(_item_infos.at(i).text);
        if (CONFIG_MANAGER->isGameVMonkey()){
            _item_infos.at(i).textItem->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
        }

		while (_item_infos.at(i).textItem->getContentSize().width > _item_infos.at(i).aLayout->getContentSize().width){
			_item_infos.at(i).textItem->setFontSize(_item_infos.at(i).textItem->getFontSize() - 1);
		}
		if (_item_infos.at(i).textItem->getFontSize() < min_font_size)
			min_font_size = _item_infos.at(i).textItem->getFontSize();
	}
	for (int i = 0; i < _item_infos.size(); i++){
		_item_infos.at(i).textItem->setFontSize(min_font_size);
	}
	
	boom = GAFWrapper::createGAF("gaf/khi_xe_ui/no/no.gaf");
	boom->play(false, nullptr);

	boomPanel = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "boomPanel");
	boomPanel->addChild(boom,100);
}

void GameKhiXeUi_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void GameKhiXeUi_Item::playBoom(int posBoom)
{
	boom->setPosition(_item_infos.at(posBoom).aLayout->getPosition() + _item_infos.at(posBoom).aLayout->getContentSize()/2 +Vec2(0,50));
	boom->play(false,nullptr);
}

