//
//  MSTabAudiobooks.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#include "MSTabAudiobooks.h"
#include "MSGradeBox.h"
#include "StoryDataModel.h"
#include "MSCollectionAudiobookItem.h"
#include "MSSimpleCollection.h"
#include "MSAudiobookPlayer.h"
#include "MJHelper.h"
#include "MSExploreView_Audiobooks.h"
#include "MSSearchView_Audiobooks.h"
#include "StoryLanguageManager.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"
#include "MSPlaylistPrepare.h"
#include "MSChapterPrepare.h"
#include "MSCollectionCategoryItem.h"
#include "MSHome.h"

#define MORAL_STORIES	"Moral Stories"	//bai hoc cuoc song.
#define WORLD_OF_TALES	"World of Tales" //truyen kinh dien.
#define FAMOUS_FABLES	"Famous Fables" //truyen ngu ngon
#define POEMS			"Poems" //tho

USING_NS_CC;
INITIALIZE_READER(MSTabAudiobooks);

/*
 * 1. New, Popular
 * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
 */

std::once_flag ms_tab_audiobooks_reader;

MSTabAudiobooks * MSTabAudiobooks::createTab(MSHome * home)
{
    std::call_once(ms_tab_audiobooks_reader, []
    {
        REGISTER_CSB_READER(MSTabAudiobooks);
    });
    
    auto csbName = mj::helper::getCSBName("csb/new_ui/Audiobooks.csb");

    auto node = reinterpret_cast<MSTabAudiobooks *>(CSLoader::createNode(csbName));
    
    if (node)
    {
        node->didLoadFromCSB(home);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSTabAudiobooks::didLoadFromCSB(MSHome * home)
{
    this->setName("MSTabAudiobooks");
    MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "new");
    _mshome = home;
    
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
    this->setClippingEnabled(false);
 
    m_scope = ms::GradeManager::Scope::AUDIOBOOKS;
    _current_grade_id = ms::GradeManager::getInstance().getCurrentGradeId();

    auto gradebox_placeholder = utils::findChild(this, "gradebox");
    auto gradebox = MSGradeBox::createBox(true,_current_grade_id);
    
    gradebox->setAnchorPoint(Vec2(0, 0));
    gradebox->setPosition(Vec2(0,0));
    gradebox_placeholder->addChild(gradebox);
    //gradebox_placeholder->removeFromParent();
    gradebox->setName("audio_gradebox");
    gradebox->setDelegate(this);
    
    _content = utils::findChild<ui::ListView *>(this, "content");
    _content->setBottomPadding(25);
    _content->setScrollBarEnabled(false);
    _content->addEventListener([=](Ref* pSender, ui::ScrollView::EventType type)
    {
        ListView* listView = dynamic_cast<ListView*>(pSender);
     
        if(listView == nullptr || type != ScrollView::EventType::CONTAINER_MOVED)
        {
            return;
        }
        
        auto top = listView->getTopmostItemInCurrentView();
        auto bottom = listView->getBottommostItemInCurrentView();

        for (size_t i = 0; i < listView->getChildrenCount(); i++)
        {
            listView->getChildren().at(i)->setVisible(false);
        }

        for (size_t i = listView->getIndex(top); i <= listView->getIndex(bottom); i++)
        {
            if (auto node = listView->getItem(i))
            {
                node->setVisible(true);
            }
        }
    });

    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e)
    {
        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }
    });

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        this->reloadData(_current_grade_id);
    });

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
}

void MSTabAudiobooks::onEnter()
{
	ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);

	ui::Layout::onEnter();

	// tagrget: optimize end screen for phonic ( or lesson ) .
	// because when users exit reading story scene, the scene manager will pop the current scene
	// and THE TOP SCENE ON STACK (HomeScene) will call onEnter function for all children
	// actually, have some codes, we only want to excute once!
	// so we need to use the flag m_isReloadDataCalled 

	if (!m_isReloadDataFunCalled)
	{
		m_isReloadDataFunCalled = true;

		reloadData();

		this->unschedule("updateListViewSchedule");

		this->schedule([=](float dt) {

			if (!_content)
			{
				return;
			}

			auto top = _content->getTopmostItemInCurrentView();
			auto bottom = _content->getBottommostItemInCurrentView();

			for (size_t i = 0; i < _content->getChildrenCount(); i++)
			{
				_content->getChildren().at(i)->setVisible(false);
			}

			for (size_t i = _content->getIndex(top); i <= _content->getIndex(bottom); i++)
			{
				if (auto node = _content->getItem(i))
				{
					node->setVisible(true);
				}
			}

		}, 0.1f, "updateListViewSchedule"); // start updateListViewSchedule function with 0.15s/1 check .
	}

    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventViewAudiobookPage, {});
}

void MSTabAudiobooks::reloadData(const int i_gradeID)
{
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
//    auto audiobooks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(StoryLanguageManager::getCurrentLangId());
    _content->removeAllItems();
    _recent_collection = nullptr;
    _favourite_collection = nullptr;
    
    auto newest = MSSimpleCollection::createNewestAudiobooksCollection(i_gradeID==UNDEFINED_GRADE_ID ? ms::GradeManager::getInstance().getCurrentGradeId():i_gradeID);
    
    if (newest)  {
        newest->enablePlaylist();
        newest->setContentSize(Size(this->getContentSize().width, newest->getContentSize().height));
        newest->setLeftPadding(25);
        newest->setChildMargin(25);
        newest->setDelegate(this);
        _content->addChild(newest);
    }
    
    _categories_collection = MSSimpleCollection::createAudiobookCategoriesCollection();
    _categories_collection->setContentSize(Size(this->getContentSize().width, _categories_collection->getContentSize().height));
    _categories_collection->setDelegate(this);
    _content->addChild(_categories_collection);

    _content->jumpToTop();

    _content->scrollToTop(0.1f,false);

//    _content->jumpToTop();
//    updateRecently();
}

void MSTabAudiobooks::updateRecently()
{
    
}

void MSTabAudiobooks::setContentSize(const Size & size)
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
}

void MSTabAudiobooks::onSelectCollectionItem(ui::Widget * item)
{
    if (!item)
    {
        return;
    }
    
	ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);

	auto category_item = dynamic_cast<MSCollectionCategoryItem*>(item);
	if (category_item) {
        MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "categories");

		auto info = category_item->getAudiobookCategoryInfo();

        mj::helper::playButtonFX();

        auto explorer = MSExploreView_Audiobooks::createView(info.title, MSExploreView_Audiobooks::CollectionType::SERIES, _current_grade_id, info);
		explorer->setSeriesName(info.title);
		explorer->setContentSize(Director::getInstance()->getVisibleSize());
		explorer->setTag(113114);// cannot use setName method , because MSExploreView override this method , now I don't have time to fix this
		ui::Helper::doLayout(explorer);
        if (_mshome) {
            _mshome->addChild(explorer, kMSHomeZOrder_contentFull);
        }
		return;
	}

    auto audiobook_item = dynamic_cast<MSCollectionAudiobookItem *>(item);
    if (!audiobook_item)
    {
        return;
    }
    
    auto info = audiobook_item->getInfo();
    
    if (StoryDataModel::getInstance()->isAudiobookChapter(info.book_id))
    {
        auto list_chapter = StoryDataModel::getInstance()->getChapterOfAudiobook(info.book_id);
        auto view = MSChapter_Prepare::createView(info, list_chapter);
        view->setName("MSChapterPrepare");
        view->setContentSize(this->getContentSize());
        ui::Helper::doLayout(view);
        this->addChild(view, 1);
    }
    else
    {
        mj::helper::playTransitionAudiobookCover(audiobook_item, _content);
    }
}

void MSTabAudiobooks::onSelectMore(MSSimpleCollection * sender)
{
    MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "see more");
    auto type = sender->getCollectionType();

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);

    if (type == MSSimpleCollection::CollectionType::AUDIOBOOK)
    {
//        auto audiobooks = sender->getAudiobooks();
//        if (sender->getCollectionOrder() == MSSimpleCollection::CollectionOrder::NEW)
//        {
//            audiobooks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(StoryLanguageManager::getCurrentLangId());
//            std::stable_sort(audiobooks.begin(), audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool {
//                if (a.date_publish != b.date_publish) {
//                    return a.date_publish > b.date_publish;
//                }
//
//                if (a.quality != b.quality) {
//                    return a.quality > b.quality;
//                }
//
//                if (a.read != b.read) {
//                    return (int)a.read < (int)b.read;
//                }
//
//                return a.name.compare(b.name) < 0;
//            });
//        }
//        else
//        {
//            std::sort(audiobooks.begin(), audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool {
//                if (a.quality != b.quality) {
//                    return a.quality > b.quality;
//                } else {
//                    if (a.read != b.read) {
//                        return a.read < b.read;
//                    } else return a.date_publish > b.date_publish;
//                }
//            });
//        }
        
        auto explorer = MSExploreView_Audiobooks::createView(sender->getTitle(), MSExploreView_Audiobooks::CollectionType::NONE, _current_grade_id);
        explorer->setSeriesName(sender->getSeriesName());
        //explorer->setContentSize(this->getContentSize());
        explorer->setContentSize(Director::getInstance()->getVisibleSize());
        //explorer->setName("MSExploreView");
        explorer->setTag(113114);// cannot use setName method , because MSExploreView override this method , now I don't have time to fix this
        ui::Helper::doLayout(explorer);
        if (_mshome) {
            _mshome->addChild(explorer, kMSHomeZOrder_contentFull);
        }
    }
}

void MSTabAudiobooks::onPlayAll(MSSimpleCollection *sender)
{
    if (!CONFIG_MANAGER->isAppActive())
    {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("sub.popup.feature.free"), "OK", "");
        return;
    }

    ms::GradeManager::getInstance().setScope(m_scope);

    auto type = sender->getCollectionType();
    
    if (type == MSSimpleCollection::CollectionType::AUDIOBOOK)
    {
        auto audiobooks = sender->getAudiobooks();
        
        if (sender->getCollectionOrder() == MSSimpleCollection::CollectionOrder::NEW)
        {
            audiobooks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(StoryLanguageManager::getCurrentLangId());
            std::stable_sort(audiobooks.begin(), audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool {
                if (a.date_publish != b.date_publish) {
                    return a.date_publish > b.date_publish;
                }
                
                if (a.quality != b.quality) {
                    return a.quality > b.quality;
                }
                
                if (a.read != b.read) {
                    return (int)a.read < (int)b.read;
                }
                
                return a.name.compare(b.name) < 0;
            });
        }
        else
        {
            std::sort(audiobooks.begin(), audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool
            {
               
                if (a.quality != b.quality)
                {
                    return a.quality > b.quality;
                }
                else
                {
                    return (a.read != b.read)? a.read < b.read :(a.date_publish > b.date_publish);
                }
            });
        }
        
        std::vector<mj::model::AudiobookInfo> exclude_chapter;
        
        std::copy_if(audiobooks.begin(), audiobooks.end(), std::back_inserter(exclude_chapter), [](mj::model::AudiobookInfo a)
        {
            return !StoryDataModel::getInstance()->isAudiobookChapter(a.book_id);
        });

        auto explorer = MSPlaylist_Prepare::createView(sender->getTitle(), sender->getSeriesName(), exclude_chapter,_current_grade_id);
        explorer->setContentSize(this->getContentSize());
        explorer->setName("MSPlaylistPrepare");
        ui::Helper::doLayout(explorer);
        this->addChild(explorer, 1);
    }
}

void MSTabAudiobooks::onSearch(cocos2d::Ref *sender)
{
    mj::helper::playButtonFX();
    auto view = MSSearchView_Audiobooks::createView();
    view->setContentSize(this->getContentSize());
    view->setName("MSSearchView");
    ui::Helper::doLayout(view);
    this->addChild(view);
    view->setOpacity(0);
    view->runAction(FadeIn::create(0.25));
}

ui::Widget::ccWidgetClickCallback MSTabAudiobooks::onLocateClickCallback(const std::string &callback_name)
{
    if (callback_name == "onSearch")
    {
        return CC_CALLBACK_1(MSTabAudiobooks::onSearch, this);
    }
    
    return nullptr;
}

void MSTabAudiobooks::onDeselectGradeBox(int idx)
{
//    reloadData();
}

void MSTabAudiobooks::onReceivedEventGradeChanged(int grade_id)
{
    _current_grade_id = grade_id;
    reloadData(grade_id);
}

void MSTabAudiobooks::clearUnusedContents()
{
    if(auto msChapterPrepare = this->getChildByName("MSChapterPrepare"))
    {
        msChapterPrepare->removeFromParent();
    }

    if(auto msSearchView = dynamic_cast<MSSearchView_Audiobooks*>(this->getChildByName("MSSearchView")))
    {
        msSearchView->hideVirtualKeyboard();
        msSearchView->removeFromParent();
    }

    if(auto msPlaylistPrepare = this->getChildByName("MSPlaylistPrepare"))
    {
        msPlaylistPrepare->removeFromParent();
    }

    if(auto msExploreView = this->getChildByTag(113114))
    {
        msExploreView->removeFromParent();
    }
}


