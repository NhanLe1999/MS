//
//	CloudCatcher_CloudItem.h
//	Created by mintzy on 10/05/18
//

#include "CloudCatcher_CloudItem.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
INITIALIZE_READER(CloudCatcher_CloudItem);

#define CSB_NAME "csb/game/CloudCatcher/CloudCatcher_CloudItem.csb"
#define GAF_CLOUD "gaf/cloudcatcher/cloudcatcher_cloud/cloudcatcher_cloud.gaf"
#define VMONKEY_PUNCTUATION_MASKS_FONT "fonts/vmonkey_punctuation_custom_font.ttf"
CloudCatcher_CloudItem * CloudCatcher_CloudItem::createItem(mj::PHONIC_INFO phonic_info, CloudType type,bool upper_case)
{
	auto p = static_cast<CloudCatcher_CloudItem *> (cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(phonic_info, type,upper_case);
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void CloudCatcher_CloudItem::setEnableClick(bool clickable)
{
	this->clickable = clickable;
}

void CloudCatcher_CloudItem::setDelegate(CloudCatcher_CloudItemDelegate * delegate)
{
	_delegate = delegate;
}



cocos2d::ui::Widget::ccWidgetClickCallback CloudCatcher_CloudItem::onLocateClickCallback(const std::string & callback_name)
{
	if (callback_name == "onClickCloud") {
		return CC_CALLBACK_1(CloudCatcher_CloudItem::onClickedCloud, this);
	}
	return nullptr;
}

void CloudCatcher_CloudItem::didLoadFromCSB(mj::PHONIC_INFO phonic_info, CloudType type, bool upper_case)
{
	_phonic_info = phonic_info;
	this->type = type;
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout*>(this, "root_layout");

	gaf::GAFAsset * cloud_asset = nullptr;

	try {
		cloud_asset = PHONIC_MANAGER->gaf_cache.at("cloudcatcher_gaf_cloud");
	}
	catch (const std::out_of_range& oor) {
		PHONIC_MANAGER->cacheGAF("cloudcatcher_gaf_cloud", GAF_CLOUD);
		cloud_asset = PHONIC_MANAGER->gaf_cache.at("cloudcatcher_gaf_cloud");
	}
	gaf_cloud = GAFWrapper::createGAF(cloud_asset);
	gaf_cloud->setPosition(root_layout->getContentSize()/ 2);
	gaf_cloud->playSequence("idle",true);
	root_layout->addChild(gaf_cloud);
	lbl_phonic = root_layout->getChildByName<cocos2d::ui::Text *>("lbl_phonic");
	img_phonic = root_layout->getChildByName<cocos2d::ui::ImageView *>("img_phonic");
	img_phonic->setLocalZOrder(10000);
    img_phonic->setPositionY(root_layout->getContentSize().height*0.38);
	lbl_phonic->setLocalZOrder(10001);
    std::string font;
    auto font_size = 95;
    if (CONFIG_MANAGER->isGameVMonkey()){
        std::string text_item = phonic_info.text_phonic;
        if (upper_case && text_item.size() == 1) {
            std::transform(text_item.begin(), text_item.end(), text_item.begin(), ::toupper);
        }
        if (text_item == "`" || text_item == "/" || text_item == "~" || text_item == "." || text_item == "?" ||text_item == "´"){
            font = VMONKEY_PUNCTUATION_MASKS_FONT;
            font_size = 130;
        }else{
            font =  "fonts/Linotte Regular.ttf";
        }
    }else{
        font = font_name;
    }
	lbl_phonic->setFontName(font);
	lbl_phonic->setColor(Color3B::BLACK);
    lbl_phonic->setPositionY(root_layout->getContentSize().height*0.38);
    if (CONFIG_MANAGER->isGameVMonkey()){
        lbl_phonic->setPositionY(root_layout->getContentSize().height*0.5);
    }
	if (type == CloudType::TEXT_PHONIC) {
		std::string text_item = phonic_info.text_full;
		if (upper_case && text_item.size() == 1) {
			std::transform(text_item.begin(), text_item.end(), text_item.begin(), ::toupper);
		}
		lbl_phonic->setString(text_item);
		int font_size = 80;;
		if (text_item.length() >= 8) {
			font_size = 60;
		}
		lbl_phonic->setFontSize(font_size);
		lbl_phonic->setVisible(true);
		lbl_phonic->runAction(Repeat::create(Sequence::create(ScaleTo::create(18.0 / 28, 1.1, 1), ScaleTo::create(18.0 / 28, 1, 1.1), NULL), 10000000));
	}
	else if (phonic_info.image_path != "") {
		img_phonic->loadTexture(phonic_info.image_path);
		img_phonic->setVisible(true);
		img_phonic->runAction(Repeat::create(Sequence::create(ScaleTo::create(18.0 / 28, 1.1, 1), ScaleTo::create(18.0 / 28, 1, 1.1), NULL), 10000000));
	}
	
	onChangeCloudState(this, CloudCatcher_CloudItem::CloudState::SHOWED);
}

void CloudCatcher_CloudItem::onClickedCloud(cocos2d::Ref * sender)
{
	CCLOG("onClickedCloud");
	if (_delegate && clickable) {
		_delegate->onClickedCloudDelegate(this);
	}
}

void CloudCatcher_CloudItem::onChangeCloudState(cocos2d::Ref * sender,int state)
{
	cloud_state = state;
	if (_delegate) {
		_delegate->onChangeCloudStateDelegate(this,cloud_state);
	}
}


void CloudCatcher_CloudItem::onCatcherCloud(function<void()> callback)
{
	lbl_phonic->setVisible(false);
	img_phonic->setVisible(false);
	is_catcher = true;
	hideTapGuideGame();
	gaf_cloud->playSequence("action", false, [=](gaf::GAFObject * obj, const std::string &sequenceName) {
		gaf_cloud->playSequence("idle2", true);
		onChangeCloudState(this, CloudCatcher_CloudItem::CloudState::CATCHERED);
		if (callback) callback();
	});
}

void CloudCatcher_CloudItem::onAutoClick()
{
	if (_delegate && clickable) {
		_delegate->onClickedCloudDelegate(this);
	}
}

void CloudCatcher_CloudItem::showTapGuideGame(std::string gaf_path)
{
	auto guide_gaf = root_layout->getChildByName("guide_gaf");
	if (guide_gaf) return;
	auto hand = GAFWrapper::createGAF(gaf_path);
	root_layout->addChild(hand, 1900000000,"guide_gaf");
	hand->setPosition(root_layout->getContentSize()/2);
	hand->setScale(1.0f / this->getScaleX());
	hand->play(false, [=](gaf::GAFObject *obj) {
		hand->removeFromParent();
	});
}

void CloudCatcher_CloudItem::hideTapGuideGame()
{
	auto guide_gaf = root_layout->getChildByName("guide_gaf");
	if (guide_gaf) guide_gaf->removeFromParent();
}

CloudCatcher_CloudItem::CloudType CloudCatcher_CloudItem::getCloudType()
{
	return type;
}

bool CloudCatcher_CloudItem::isCatcher()
{
	return is_catcher;
}

bool CloudCatcher_CloudItem::isUsed()
{
	return is_used;
}

void CloudCatcher_CloudItem::setUsed(bool used)
{
	this->is_used = used;
}

cocos2d::ui::Text * CloudCatcher_CloudItem::getLabelPhonic()
{
	return lbl_phonic;
}

cocos2d::ui::ImageView * CloudCatcher_CloudItem::getImagePhonic()
{
	return img_phonic;
}
