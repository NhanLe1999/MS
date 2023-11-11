//
//  MSCollectionCategoryItem.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "MSCollectionCategoryItem.h"
#include "RemoteImageView.h"

USING_NS_CC;
INITIALIZE_READER(MSCollectionCategoryItem);

std::once_flag ms_collection_category_item;

MSCollectionCategoryItem * MSCollectionCategoryItem::createItem(mj::model::CategoryInfo info) {
    std::call_once(ms_collection_category_item, [] {
        REGISTER_CSB_READER(MSCollectionCategoryItem);
    });
    
    auto item = reinterpret_cast<MSCollectionCategoryItem *>(CSLoader::createNode("csb/new_ui/collection_items/CategoryItem.csb"));
    if (item) {
        item->didLoadFromCSB(info);
        return item;
    }
    
    CC_SAFE_DELETE(item);
    return nullptr;
}

MSCollectionCategoryItem * MSCollectionCategoryItem::createItem(mj::model::AudiobookSeriesInfo info) {
	std::call_once(ms_collection_category_item, [] {
		REGISTER_CSB_READER(MSCollectionCategoryItem);
	});

	auto item = reinterpret_cast<MSCollectionCategoryItem *>(CSLoader::createNode("csb/new_ui/collection_items/CategoryItem.csb"));
	if (item) {
		item->didLoadFromCSB(info);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}


void MSCollectionCategoryItem::didLoadFromCSB(mj::model::CategoryInfo info) {

    _info = info;
    
    utils::findChild<ui::Text *>(this, "placeholder")->setString(info.content);
    
    auto remoteview = RemoteImageView::createImage(_info.image_url, [=](RemoteImageView * img, bool is_success) {
        if (is_success) {
           // this->removeChildByName("image");
           if(img)
           {
               img->setContentSize(Size(370, 230));
               img->setOpacity(0);
               img->runAction(FadeIn::create(0.25));
           }
        }
    });
    
    remoteview->setContentSize(Size(370, 230));
    remoteview->setPosition(this->getContentSize() / 2);
    this->addChild(remoteview);
}

void MSCollectionCategoryItem::didLoadFromCSB(mj::model::AudiobookSeriesInfo info)
{
	_audiobookInfo = info;

	utils::findChild<ui::Text *>(this, "placeholder")->setString(info.title);

	auto remoteview = RemoteImageView::createImage(_audiobookInfo.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
		if (is_success) {
			//this->removeChildByName("image");
			if(img)
            {
                img->setContentSize(Size(370, 230));
                img->setOpacity(0);
                img->runAction(FadeIn::create(0.25));
            }
		}
	});

	remoteview->setContentSize(Size(370, 230));
	remoteview->setPosition(this->getContentSize() / 2);
	this->addChild(remoteview);
}
