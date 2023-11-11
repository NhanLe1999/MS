#pragma once

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "MSPopularSearchItem.h"
#include "MSCollectionDataSource.h"
#include "MSComplexCollection.h"

USING_NS_CC;

class PopularSearchTemplate : public cocos2d::ui::Layout , public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::CollectionDataSource {

public:
	enum class Type {
		Story,
		Audiobook,
		Worksheet_Story,
        Worksheet_Phonics,
	};

	CREATE_FUNC(PopularSearchTemplate);

	static PopularSearchTemplate* createTemplate(PopularSearchTemplate::Type i_type);

	std::vector<std::string> getRecentSearches();

	void setRecentSearches(const std::string& keyword);

	void reloadData();

private:

    ~PopularSearchTemplate() {
        if (_ui_source) {
            _ui_source->release();
        }
    }
    
    void didLoadFromCSB(PopularSearchTemplate::Type i_type);
    
    // MARK: CollectionDataSource
    MSComplexCollection *   _popularCollection = nullptr;
    __Array *               _ui_source = nullptr;
    
    Size collectionSize() override {
        return utils::findChild<ui::Layout*>(this, "Popular_searches_list")->getContentSize();
    }
    
    Size modelSize() override {
        return Size(183, 275);
    }
    
    int numberOfCells() override {
        if (_ui_source) {
            return (int)_ui_source->count();
        }
        return 0;
    }
    
    int numberOfColumns() override {
        return (int)((collectionSize().width + 10) / (modelSize().width + 10));
    }
    
    ui::Widget * cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) override {
        auto index = row * numberOfColumns() + col;
        if (index < 0 || _ui_source == nullptr || index >= _ui_source->count()) {
            return nullptr;
        }
        return (MJPopularSearchItem *)_ui_source->getObjectAtIndex(index);
    }
    
    void offLoadCellAt(int index) override {
        // nothing here
    }
    
    // MARK: load popular searches
	template <class T>
	void loadPopularSearch(const std::vector<T>& popularItems) {
		auto recentSearchList = utils::findChild<ui::ListView*>(this, "Recent_searches_list");
		recentSearchList->removeAllChildren();
		recentSearchList->setScrollBarEnabled(false);

		auto  popularSearchLayout = utils::findChild<ui::Layout*>(this, "Popular_search_layout");

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		auto recentSearches = this->getRecentSearches();

		for (auto recentText : recentSearches) {
			
			if (auto recentSearchItem = dynamic_cast<ui::Button*>(utils::findChild<ui::Button*>(this, "recent_search")->clone())) {

				recentSearchItem->setTitleText(recentText);
				
				switch (m_type)
				{
				case PopularSearchTemplate::Type::Story:
					recentSearchItem->loadTextures("mjstory/material/mat_duc/hd/popular_search_item/recent_reading.png", "");
					break;
				case PopularSearchTemplate::Type::Audiobook:
					//loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchAudiobooks());
					recentSearchItem->loadTextures("mjstory/material/mat_duc/hd/popular_search_item/recent_listening.png", "");
					break;
				case PopularSearchTemplate::Type::Worksheet_Story:
					break;
                case PopularSearchTemplate::Type::Worksheet_Phonics:
                    break;
				default:
					break;
				}

				recentSearchItem->setContentSize(Size(std::max(recentSearchItem->getContentSize().width, recentSearchItem->getTitleLabel()->getContentSize().width*1.15f), recentSearchItem->getContentSize().height));
			
				recentSearchList->pushBackCustomItem(recentSearchItem);

				recentSearchItem->addClickEventListener([=](Ref* sender) {
					if (_recentItemClickCallback) {
						_recentItemClickCallback(sender);
					}
				});
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        // cách này sử dụng MSComplexCollection để hiển thị
        _ui_source = __Array::create();
        _ui_source->retain();
        
        auto popularSearchList = utils::findChild<ui::ListView*>(this, "Popular_searches_list");
        popularSearchList->removeAllChildren();
        popularSearchList->setScrollBarEnabled(false);
        
        _popularCollection = MSComplexCollection::createCollection(this);
        popularSearchList->addChild(_popularCollection);
        
        for (auto elem : popularItems) {
            auto popItem = MJPopularSearchItem::createItem(elem);
            if(auto button = utils::findChild<ui::Button*>(popItem,"button")) {
                button->addClickEventListener([=](Ref* sender) {
                    if (_popularItemClickCallback) {
                        _popularItemClickCallback(popItem);
                    }
                });
            }
            _ui_source->addObject(popItem);
        }
        
        _popularCollection->setPadding(0, 0);
        _popularCollection->reloadData();

#if 0 /* không sử dụng cách ListView vì không scroll được */
		auto popularSearchList = utils::findChild<ui::ListView*>(this, "Popular_searches_list");
		popularSearchList->removeAllChildren();

		cocos2d::Size itemSize = Size(183, 250);
        
		const float k_paddingFactor = 15.0f;

		int columNum = (popularSearchLayout->getContentSize().width + k_paddingFactor) / (k_paddingFactor + itemSize.width);

		cocos2d::Size rowSize = { std::min((columNum - 1)*k_paddingFactor + columNum * itemSize.width, popularSearchLayout->getContentSize().width), itemSize.height };

		int rowNum = ceil((double)popularItems.size() / (double)columNum);

		int count = 0;

		auto rowModal = utils::findChild<ui::ListView*>(this, "row_modal");

        if (rowNum <= 0 && columNum > 0 ) {
            
            auto row = dynamic_cast<ui::ListView*>(rowModal->clone());
            row->setContentSize(rowSize);
            row->setScrollBarEnabled(false);
            popularSearchList->pushBackCustomItem(row);
            
            for (int j = 0; j < columNum; j++) {
                
                if (count < popularItems.size()) {
                    
                    auto popItem = MJPopularSearchItem::createItem(popularItems.at(count));
                    
                    if(auto button = utils::findChild<ui::Button*>(popItem,"button"))
                    {
                        button->addClickEventListener([=](Ref* sender) {
                            
                            if (_popularItemClickCallback) {
                                _popularItemClickCallback(popItem);
                            }
                            
                        });
                    }
                    row->pushBackCustomItem(popItem);
                }
                
                count++;
                
                if (count >= popularItems.size()) {
                    break;
                }
            }
            
        }
        
		for (int i = 0; i < rowNum; i++) {

			auto row = dynamic_cast<ui::ListView*>(rowModal->clone());
			row->setContentSize(rowSize);
			row->setScrollBarEnabled(false);
			popularSearchList->pushBackCustomItem(row);

			for (int j = 0; j < columNum; j++) {

				if (count < popularItems.size()) {
				
					auto popItem = MJPopularSearchItem::createItem(popularItems.at(count));
                    
                    if(auto button = utils::findChild<ui::Button*>(popItem,"button"))
                    {
                        button->addClickEventListener([=](Ref* sender) {
                            
                            if (_popularItemClickCallback) {
                                _popularItemClickCallback(popItem);
                            }
                            
                        });
                    }
					row->pushBackCustomItem(popItem);
				}

				count++;

				if (count >= popularItems.size()) {
					break;
				}
			}

			if (count >= popularItems.size()) {
				break;
			}
		}
#endif
	}

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string m_recentLocalSavingKey = "";

	Type m_type;

	CC_SYNTHESIZE(std::function<void(Ref*)>, _recentItemClickCallback, RecentItemClickCallback);

	CC_SYNTHESIZE(std::function<void(Ref*)>, _popularItemClickCallback, PopularItemClickCallback);
};

CREATE_READER(PopularSearchTemplate);
